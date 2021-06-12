/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <chrono>
#include <random>
#include <thread>

#include "UtilityTest.hpp"

// Try to get a seed from hardware, if available
static std::random_device::result_type seed = std::random_device()();
// This object generates randon numbers using the Mersenne-Twister algoritym
static std::mt19937 randomEngine(seed);

int UtilityTest::random(int min, int max) {
  // Produce random values with uniform discrete distribution
  std::uniform_int_distribution<int> randomDistribution(min, max - 1);
  // Generate and return a random number using the uniform distribution
  return randomDistribution(randomEngine);
}

void UtilityTest::sleepFor(int milliseconds) {
  if ( milliseconds < 0 ) {
    milliseconds = UtilityTest::random(0, std::abs(milliseconds));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
