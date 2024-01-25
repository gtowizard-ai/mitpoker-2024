#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <utility>

constexpr int num_ranks = 13;
constexpr int max_num_cards = 8;

bool insert_basis(std::unordered_map<int, int>& sums_map, int basis) {
  // screening
  for (const auto& [key, value] : sums_map) {
    const int max_add = std::min(max_num_cards - value, 4);
    for (int add = 1; add <= max_add; ++add) {
      if (sums_map.count(key + add * basis)) {
        return false;
      }
    }
  }

  // insert
  std::unordered_map<int, int> new_sums_map = sums_map;
  for (const auto& [key, value] : sums_map) {
    const int max_add = std::min(max_num_cards - value, 4);
    for (int add = 1; add <= max_add; ++add) {
      if (!new_sums_map.insert({key + add * basis, value + add}).second) {
        return false;
      }
    }
  }

  sums_map = std::move(new_sums_map);
  return true;
}

void print_bases(int first_basis) {
  std::array<int, num_ranks> bases = {first_basis};
  std::cout << first_basis << std::flush;

  std::unordered_map<int, int> sums_map = {{0, 0}};
  insert_basis(sums_map, first_basis);

  for (int i = 1; i < num_ranks; ++i) {
    bases[i] = bases[i - 1] + first_basis;
    while (!insert_basis(sums_map, bases[i])) {
      ++bases[i];
    }
    std::cout << ", " << bases[i] << std::flush;
  }

  std::cout << std::endl;
}

int main() {
  for (int first_basis = 256; first_basis <= 4096; first_basis *= 2) {
    print_bases(first_basis);
  }
}
