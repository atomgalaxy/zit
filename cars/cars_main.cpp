#include "cars.hpp"

#include <iostream>

int main() {
  cars::lada h{5};
  h.honk(std::cout) << "\n";
}