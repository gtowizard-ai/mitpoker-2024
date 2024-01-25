#pragma once
#include "range.h"
#include "card.h"
#include "hand.h"
#include <iostream>




namespace pokerbot{


struct Auctioneer{

	Auctioneer();

	int get_bid(const std::vector<Range>& ranges, const std::vector<Card>& board, Hand& hand, float time_budget);

};

} //pokerbot namespace

