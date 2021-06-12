/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef UTILITYTEST_HPP
#define UTILITYTEST_HPP

#include "common.hpp"

/**
 * @brief Container class for utility methods
 * This code is not needed for real projects
 */
class UtilityTest {
  DISABLE_COPY(UtilityTest);
  /// Constructor
  UtilityTest() = delete;
  /// Destructor
  ~UtilityTest() = delete;

 public:
  /// Generates a pseudo-random integer number in range [min, max[
  static int random(int min, int max);
  /// Utility method to simulate workload using sleep
  /// This method is for illustrative goals.
  /// Sleeps must be NEVER used for real projects.
  static void sleepFor(int milliseconds);
};

#endif  // UTILITYTEST_HPP
