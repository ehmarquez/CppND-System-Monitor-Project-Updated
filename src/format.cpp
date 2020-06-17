#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  return std::to_string(seconds / 3600) + ":" +
         std::to_string(seconds % 3600 / 60) + ":" +
         std::to_string(seconds % 60);
}
