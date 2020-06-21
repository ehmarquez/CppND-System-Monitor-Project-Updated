#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string hour = std::to_string(seconds / 3600);
  string minute = std::to_string(seconds % 3600 / 60);
  string second = std::to_string(seconds % 60);

  if (hour.length() < 2) {
    hour = "0" + hour;
  }
  if (minute.length() < 2) {
    minute = "0" + minute;
  }
  if (second.length() < 2) {
    second = "0" + second;
  }
  return hour + ":" + minute + ":" + second;
}
