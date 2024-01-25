'''
6.9630 MIT POKERBOTS GAME ENGINE
DO NOT REMOVE, RENAME, OR EDIT THIS FILE
'''

'''
The variant this year centers around an auction that occurs immediately after the flop
is dealt for a single card. Both players are requested to submit a bid for the card, 
after which the player with the higher bids wins the card. We will be utilizing a second 
price auction in which the winner pays the amount that the other player bid. The player 
who wins the auction receives the card, the amount the winner pays is deducted from their 
bankroll, and the same amount is added to the pot. Note, in the case of ties, both players 
are awarded a card and both players contribute to the pot. Bids are not capped. 
'''

''' 
Note that changes made to the engine.py file will also have to be replicated in the 
each of the skeleton bot files accordingly
'''

from collections import namedtuple
from threading import Thread
from queue import Queue
import time
import json
import subprocess
import socket
import eval7
import sys
import os

sys.path.append(os.getcwd())
from config import *

FoldAction = namedtuple('FoldAction', [])
CallAction = namedtuple('CallAction', [])
CheckAction = namedtuple('CheckAction', [])
# we coalesce BetAction and RaiseAction for convenience
RaiseAction = namedtuple('RaiseAction', ['amount'])
BidAction = namedtuple('BidAction', ['amount'])
TerminalState = namedtuple('TerminalState', ['deltas', 'bids', 'previous_state'])

# will not include a "bid" street as a community card is not being revealed to the players
STREET_NAMES = ['Flop', 'Turn', 'River']
DECODE = {'F': FoldAction, 'C': CallAction, 'K': CheckAction, 'R': RaiseAction, 'A': BidAction}
CCARDS = lambda cards: ','.join(map(str, cards))
PCARDS = lambda cards: '[{}]'.format(' '.join(map(str, cards)))
PVALUE = lambda name, value: ', {} ({})'.format(name, value)
STATUS = lambda players: ''.join([PVALUE(p.name, p.bankroll) for p in players])

# Socket encoding scheme:
#
# T#.### the player's game clock
# P# the player's index
# H**,** the player's hand in common format
# F a fold action in the round history
# C a call action in the round history
# K a check action in the round history
# R### a raise action in the round history
# A### a bid action in the round history
# N**, ** the player's hand after the auction in common format
# B**,**,**,**,** the board cards in common format
# O**,** the opponent's hand in common format
# D### the player's bankroll delta from the round
# Q game over
#
# Clauses are separated by spaces
# Messages end with '\n'
# The engine expects a response of K at the end of the round as an ack,
# otherwise a response which encodes the player's action
# Action history is sent once, including the player's actions


class RoundState(namedtuple('_RoundState', ['button', 'street', 'auction', 'bids', 'pips', 'stacks', 'hands', 'deck', 'previous_state'])):
    '''
    Encodes the game tree for one round of poker.
    '''

    def showdown(self):
        '''
        Compares the players' hands and computes payoffs.
        '''
        score0 = eval7.evaluate(self.deck.peek(5) + self.hands[0])
        score1 = eval7.evaluate(self.deck.peek(5) + self.hands[1])
        if score0 > score1:
            delta = STARTING_STACK - self.stacks[1]
        elif score0 < score1:
            delta = self.stacks[0] - STARTING_STACK
        else:  # split the pot
            delta = (self.stacks[0] - self.stacks[1]) // 2
        return TerminalState([delta, -delta], self.bids, self)

    def legal_actions(self):
        '''
        Returns a set which corresponds to the active player's legal moves.
        '''
        if self.auction: 
            return {BidAction}
        active = self.button % 2
        continue_cost = self.pips[1-active] - self.pips[active]
        if continue_cost == 0:
            # we can only raise the stakes if both players can afford it
            bets_forbidden = (self.stacks[0] == 0 or self.stacks[1] == 0)
            return {CheckAction} if bets_forbidden else {CheckAction, RaiseAction}
        # continue_cost > 0
        # similarly, re-raising is only allowed if both players can afford it
        raises_forbidden = (continue_cost >= self.stacks[active] or self.stacks[1-active] == 0)
        return {FoldAction, CallAction} if raises_forbidden else {FoldAction, CallAction, RaiseAction}

    def raise_bounds(self):
        '''
        Returns a tuple of the minimum and maximum legal raises.
        '''
        active = self.button % 2
        continue_cost = self.pips[1-active] - self.pips[active]
        # can not raise to a value opponent can't afford.
        max_contribution = min(self.stacks[active], self.stacks[1-active] + continue_cost)
        min_contribution = min(max_contribution, continue_cost + max(continue_cost, BIG_BLIND))
        return (self.pips[active] + min_contribution, self.pips[active] + max_contribution)

    def bid_bounds(self):
        '''
        Returns a tuple of the minimum and maximum legal bid amounts
        '''
        active = self.button % 2
        min_bid = 0
        max_bid = self.stacks[active]
        return (min_bid, max_bid)

    def proceed_street(self):
        '''
        Resets the players' pips and advances the game tree to the next round of betting.
        '''
        if self.street == 5:
            return self.showdown()
        if self.street == 0:        # immediately after flop is dealt, we enter the auction
            return RoundState(1, 3, True, self.bids, [0, 0], self.stacks, self.hands, self.deck, self)
        # new_street = 3 if self.street == 0 else self.street + 1
        # return RoundState(1, new_street, [0, 0], self.stacks, self.hands, self.deck, self)
        return RoundState(1, self.street + 1, False, self.bids, [0, 0], self.stacks, self.hands, self.deck, self)

    def proceed(self, action):
        '''
        Advances the game tree by one action performed by the active player.
        '''
        active = self.button % 2
        if isinstance(action, FoldAction):
            delta = self.stacks[0] - STARTING_STACK if active == 0 else STARTING_STACK - self.stacks[1]
            return TerminalState([delta, -delta], self.bids, self)
        if isinstance(action, CallAction):
            if self.button == 0:  # sb calls bb preflop
                return RoundState(1, 0, self.auction, self.bids, [BIG_BLIND] * 2, [STARTING_STACK - BIG_BLIND] * 2, self.hands, self.deck, self)
            # both players acted
            new_pips = list(self.pips)
            new_stacks = list(self.stacks)
            contribution = new_pips[1-active] - new_pips[active]
            new_stacks[active] -= contribution
            new_pips[active] += contribution
            state = RoundState(self.button + 1, self.street, self.auction, self.bids, new_pips, new_stacks, self.hands, self.deck, self)
            return state.proceed_street()
        if isinstance(action, CheckAction):
            if (self.street == 0 and self.button > 0) or self.button > 1:  # both players acted
                return self.proceed_street()
            # let opponent act
            return RoundState(self.button + 1, self.street, self.auction, self.bids, self.pips, self.stacks, self.hands, self.deck, self)
        if isinstance(action, BidAction):
            self.bids[active] = action.amount 
            if None not in self.bids:       # both players have submitted bids and we deal the extra card
                # case in which bids are equal, both players receive card
                if self.bids[0] == self.bids[1]:
                    self.hands[0].append(self.deck.peek(48)[-1])
                    self.hands[1].append(self.deck.peek(48)[-2])
                    new_stacks = list(self.stacks)
                    new_stacks[0] -= self.bids[0]
                    new_stacks[1] -= self.bids[1]
                    state = RoundState(1, self.street, False, self.bids, self.pips, new_stacks, self.hands, self.deck, self)
                else:
                # case in which bids are not equal
                    winner = self.bids.index(max(self.bids))
                    self.hands[winner].append(self.deck.peek(48)[-1])
                    new_stacks = list(self.stacks)
                    new_stacks[winner] -= self.bids[1 - winner]
                    state = RoundState(1, self.street, False, self.bids, self.pips, new_stacks, self.hands, self.deck, self)
                return state
            else:
                return RoundState(self.button + 1, self.street, True, self.bids, self.pips, self.stacks, self.hands, self.deck, self)
        if isinstance(action, RaiseAction):
            new_pips = list(self.pips)
            new_stacks = list(self.stacks)
            contribution = action.amount - new_pips[active]
            new_stacks[active] -= contribution
            new_pips[active] += contribution
            return RoundState(self.button + 1, self.street, self.auction, self.bids, new_pips, new_stacks, self.hands, self.deck, self)
        


class Player():
    '''
    Handles subprocess and socket interactions with one player's pokerbot.
    '''

    def __init__(self, name, path):
        self.name = name
        self.path = path
        self.game_clock = STARTING_GAME_CLOCK
        self.bankroll = 0
        self.commands = None
        self.bot_subprocess = None
        self.socketfile = None
        self.bytes_queue = Queue()

    def build(self):
        '''
        Loads the commands file and builds the pokerbot.
        '''
        try:
            with open(self.path + '/commands.json', 'r') as json_file:
                commands = json.load(json_file)
            if ('build' in commands and 'run' in commands and
                    isinstance(commands['build'], list) and
                    isinstance(commands['run'], list)):
                self.commands = commands
            else:
                print(self.name, 'commands.json missing command')
        except FileNotFoundError:
            print(self.name, 'commands.json not found - check PLAYER_PATH')
        except json.decoder.JSONDecodeError:
            print(self.name, 'commands.json misformatted')
        if self.commands is not None and len(self.commands['build']) > 0:
            try:
                proc = subprocess.run(self.commands['build'],
                                      stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                      cwd=self.path, timeout=BUILD_TIMEOUT, check=False)
                self.bytes_queue.put(proc.stdout)
            except subprocess.TimeoutExpired as timeout_expired:
                error_message = 'Timed out waiting for ' + self.name + ' to build'
                print(error_message)
                self.bytes_queue.put(timeout_expired.stdout)
                self.bytes_queue.put(error_message.encode())
            except (TypeError, ValueError):
                print(self.name, 'build command misformatted')
            except OSError:
                print(self.name, 'build failed - check "build" in commands.json')

    def run(self):
        '''
        Runs the pokerbot and establishes the socket connection.
        '''
        if self.commands is not None and len(self.commands['run']) > 0:
            try:
                server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                with server_socket:
                    server_socket.bind(('', 0))
                    server_socket.settimeout(CONNECT_TIMEOUT)
                    server_socket.listen()
                    port = server_socket.getsockname()[1]
                    proc = subprocess.Popen(self.commands['run'] + [str(port)],
                                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                            cwd=self.path)
                    self.bot_subprocess = proc
                    # function for bot listening
                    def enqueue_output(out, queue):
                        try:
                            for line in out:
                                queue.put(line)
                        except ValueError:
                            pass
                    # start a separate bot listening thread which dies with the program
                    Thread(target=enqueue_output, args=(proc.stdout, self.bytes_queue), daemon=True).start()
                    # block until we timeout or the player connects
                    client_socket, _ = server_socket.accept()
                    with client_socket:
                        client_socket.settimeout(CONNECT_TIMEOUT)
                        sock = client_socket.makefile('rw')
                        self.socketfile = sock
                        print(self.name, 'connected successfully')
            except (TypeError, ValueError):
                print(self.name, 'run command misformatted')
            except OSError:
                print(self.name, 'run failed - check "run" in commands.json')
            except socket.timeout:
                print('Timed out waiting for', self.name, 'to connect')

    def stop(self):
        '''
        Closes the socket connection and stops the pokerbot.
        '''
        if self.socketfile is not None:
            try:
                self.socketfile.write('Q\n')
                self.socketfile.close()
            except socket.timeout:
                print('Timed out waiting for', self.name, 'to disconnect')
            except OSError:
                print('Could not close socket connection with', self.name)
        if self.bot_subprocess is not None:
            try:
                outs, _ = self.bot_subprocess.communicate(timeout=CONNECT_TIMEOUT)
                self.bytes_queue.put(outs)
            except subprocess.TimeoutExpired:
                print('Timed out waiting for', self.name, 'to quit')
                self.bot_subprocess.kill()
                outs, _ = self.bot_subprocess.communicate()
                self.bytes_queue.put(outs)
        with open(self.name + '.txt', 'wb') as log_file:
            bytes_written = 0
            for output in self.bytes_queue.queue:
                try:
                    bytes_written += log_file.write(output)
                    if bytes_written >= PLAYER_LOG_SIZE_LIMIT:
                        break
                except TypeError:
                    pass

    def query(self, round_state, player_message, game_log):
        '''
        Requests one action from the pokerbot over the socket connection.
        At the end of the round, we request a CheckAction from the pokerbot.
        '''
        legal_actions = round_state.legal_actions() if isinstance(round_state, RoundState) else {CheckAction}
        if self.socketfile is not None and self.game_clock > 0.:
            clause = ''
            try:
                player_message[0] = 'T{:.3f}'.format(self.game_clock)
                message = ' '.join(player_message) + '\n'
                del player_message[1:]  # do not send redundant action history
                start_time = time.perf_counter()
                self.socketfile.write(message)
                self.socketfile.flush()
                clause = self.socketfile.readline().strip()
                end_time = time.perf_counter()
                if ENFORCE_GAME_CLOCK:
                    self.game_clock -= end_time - start_time
                if self.game_clock <= 0.:
                    raise socket.timeout
                action = DECODE[clause[0]]
                if action in legal_actions:
                    if clause[0] == 'R':
                        amount = int(clause[1:])
                        min_raise, max_raise = round_state.raise_bounds()
                        if min_raise <= amount <= max_raise:
                            return action(amount)
                    elif clause[0] == 'A':
                        amount = int(clause[1:])
                        min_bid, max_bid = round_state.bid_bounds()
                        if min_bid <= amount <= max_bid:
                            return action(amount)
                    else:
                        return action()
                if clause[0] in ('R', 'A'):
                    game_log.append(self.name + ' attempted illegal ' + action.__name__ + ' with amount ' + str(int(clause[1:])))
                else:
                    game_log.append(self.name + ' attempted illegal ' + action.__name__)

            except socket.timeout:
                error_message = self.name + ' ran out of time'
                game_log.append(error_message)
                print(error_message)
                self.game_clock = 0.
            except OSError:
                error_message = self.name + ' disconnected'
                game_log.append(error_message)
                print(error_message)
                self.game_clock = 0.
            # except (IndexError, KeyError, ValueError):
            #     # TODO: responses are being misformatted when running game
            #     game_log.append(self.name + ' response misformatted: ' + str(clause))
            except IndexError:
                # TODO: responses are being misformatted when running game
                game_log.append(self.name + ' response misformatted: ' + str(clause))
                game_log.append('IndexError')
            except KeyError:
                # TODO: responses are being misformatted when running game
                game_log.append(self.name + ' response misformatted: ' + str(clause))
                game_log.append('KeyError')
            except ValueError:
                # TODO: responses are being misformatted when running game
                game_log.append(self.name + ' response misformatted: ' + str(clause))
                game_log.append('ValueError')
        # set a base bid action of 0 if pokerbot fails to submit legal bid action
        if BidAction in legal_actions: 
            return BidAction(0)
        return CheckAction() if CheckAction in legal_actions else FoldAction()


class Game():
    '''
    Manages logging and the high-level game procedure.
    '''

    def __init__(self):
        self.log = ['6.9630 MIT Pokerbots - ' + PLAYER_1_NAME + ' vs ' + PLAYER_2_NAME]
        self.player_messages = [[], []]

    def log_round_state(self, players, round_state):
        '''
        Incorporates RoundState information into the game log and player messages.
        '''
        # engine communicates cards after the auction
        if round_state.street == 3 and round_state.auction is False and round_state.button == 1:
            for i in range(2):
                if len(round_state.hands[i]) > 2:
                    new_cards = PCARDS(round_state.hands[i]).split(" ")
                    self.log.append('{} won the auction and was dealt {}'.format(players[i].name, "["+new_cards[-1]))
            self.player_messages[0].append('P0')
            self.player_messages[0].append('N' + ','.join([str(x) for x in round_state.stacks]) + '_' + ','.join([str(x) for x in round_state.bids]) + '_' + CCARDS(round_state.hands[0]))
            self.player_messages[1].append('P1')
            self.player_messages[1].append('N' + ','.join([str(x) for x in round_state.stacks]) + '_' + ','.join([str(x) for x in round_state.bids]) + '_' + CCARDS(round_state.hands[1]))

        if round_state.street == 0 and round_state.button == 0:
            self.log.append('{} posts the blind of {}'.format(players[0].name, SMALL_BLIND))
            self.log.append('{} posts the blind of {}'.format(players[1].name, BIG_BLIND))
            self.log.append('{} dealt {}'.format(players[0].name, PCARDS(round_state.hands[0])))
            self.log.append('{} dealt {}'.format(players[1].name, PCARDS(round_state.hands[1])))
            self.player_messages[0] = ['T0.', 'P0', 'H' + CCARDS(round_state.hands[0])]
            self.player_messages[1] = ['T0.', 'P1', 'H' + CCARDS(round_state.hands[1])]
        elif round_state.street > 0 and round_state.button == 1:
            board = round_state.deck.peek(round_state.street)
            self.log.append(STREET_NAMES[round_state.street - 3] + ' ' + PCARDS(board) +
                            PVALUE(players[0].name, STARTING_STACK-round_state.stacks[0]) +
                            PVALUE(players[1].name, STARTING_STACK-round_state.stacks[1]))
            compressed_board = 'B' + CCARDS(board)
            self.player_messages[0].append(compressed_board)
            self.player_messages[1].append(compressed_board)
            
    def log_action(self, name, action, bet_override):
        '''
        Incorporates action information into the game log and player messages.
        '''
        if isinstance(action, FoldAction):
            phrasing = ' folds'
            code = 'F'
        elif isinstance(action, CallAction):
            phrasing = ' calls'
            code = 'C'
        elif isinstance(action, CheckAction):
            phrasing = ' checks'
            code = 'K'
        elif isinstance(action, BidAction):
            phrasing = ' bids ' + str(action.amount)
            code = 'A' + str(action.amount)
        else:  # isinstance(action, RaiseAction)
            phrasing = (' bets ' if bet_override else ' raises to ') + str(action.amount)
            code = 'R' + str(action.amount)
        self.log.append(name + phrasing)
        self.player_messages[0].append(code)
        self.player_messages[1].append(code)

    def log_terminal_state(self, players, round_state):
        '''
        Incorporates TerminalState information into the game log and player messages.
        '''
        previous_state = round_state.previous_state
        if FoldAction not in previous_state.legal_actions():
            self.log.append('{} shows {}'.format(players[0].name, PCARDS(previous_state.hands[0])))
            self.log.append('{} shows {}'.format(players[1].name, PCARDS(previous_state.hands[1])))
            self.player_messages[0].append('O' + CCARDS(previous_state.hands[1]))
            self.player_messages[1].append('O' + CCARDS(previous_state.hands[0]))
        self.log.append('{} awarded {}'.format(players[0].name, round_state.deltas[0]))
        self.log.append('{} awarded {}'.format(players[1].name, round_state.deltas[1]))
        if None in round_state.bids:
            self.log.append('Players did not reach flop. No auction occured.')
        else:
            self.log.append('Players submitted bids of {} and {}'.format(round_state.bids[0], round_state.bids[1]))
        
        self.player_messages[0].append('D' + str(round_state.deltas[0]))
        self.player_messages[1].append('D' + str(round_state.deltas[1]))

    def run_round(self, players):
        '''
        Runs one round of poker (1 hand).
        '''
        deck = eval7.Deck()
        deck.shuffle()
        hands = [deck.deal(2), deck.deal(2)]
        auction = False
        bids = [None, None]
        pips = [SMALL_BLIND, BIG_BLIND]
        stacks = [STARTING_STACK - SMALL_BLIND, STARTING_STACK - BIG_BLIND]
        round_state = RoundState(0, 0, auction, bids, pips, stacks, hands, deck, None)
        while not isinstance(round_state, TerminalState):
            self.log_round_state(players, round_state)
            active = round_state.button % 2
            player = players[active]
            action = player.query(round_state, self.player_messages[active], self.log)
            bet_override = (round_state.pips == [0, 0])
            self.log_action(player.name, action, bet_override)
            round_state = round_state.proceed(action)
        self.log_terminal_state(players, round_state)
        for player, player_message, delta in zip(players, self.player_messages, round_state.deltas):
            player.query(round_state, player_message, self.log)
            player.bankroll += delta

    def run(self):
        '''
        Runs one game of poker.
        '''
        print('   __  _____________  ___       __           __        __    ')
        print('  /  |/  /  _/_  __/ / _ \\___  / /_____ ____/ /  ___  / /____')
        print(' / /|_/ // /  / /   / ___/ _ \\/  \'_/ -_) __/ _ \\/ _ \\/ __(_-<')
        print('/_/  /_/___/ /_/   /_/   \\___/_/\\_\\\\__/_/ /_.__/\\___/\\__/___/')
        print()
        print('Starting the Pokerbots engine...')
        players = [
            Player(PLAYER_1_NAME, PLAYER_1_PATH),
            Player(PLAYER_2_NAME, PLAYER_2_PATH)
        ]
        for player in players:
            player.build()
            player.run()
        for round_num in range(1, NUM_ROUNDS + 1):
            self.log.append('')
            self.log.append('Round #' + str(round_num) + STATUS(players))
            self.run_round(players)
            players = players[::-1]
        self.log.append('')
        self.log.append('Final' + STATUS(players))
        for player in players:
            player.stop()
        name = GAME_LOG_FILENAME + '.txt'
        print('Writing', name)
        with open(name, 'w') as log_file:
            log_file.write('\n'.join(self.log))


if __name__ == '__main__':
    Game().run()
