#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor for assigning unique PID
Process::Process(int pid)
    : pid_(0),
      cpu_(0),
      mem_(0),
      clock_hz_(sysconf(_SC_CLK_TCK)),
      stats_(21, "0") {
  pid_ = pid;
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  string line;
  float total_time = 0, seconds = 0;
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid_) +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    stats_.assign(std::istream_iterator<string>{linestream},
                  std::istream_iterator<string>{});
    /* Calculate CPU usage per process using
     * https://stackoverflow.com/questions/16726779/
     */
    total_time = std::stof(stats_.at(kUtime_)) + std::stof(stats_.at(kStime_)) +
                 std::stof(stats_.at(kCutime_)) +
                 std::stof(stats_.at(kCstime_));
    seconds =
        LinuxParser::UpTime() - (std::stof(stats_.at(kStarttime_)) / clock_hz_);
  }

  // Store values into class members for updates
  cpu_ = ((total_time / clock_hz_) / seconds);

  return cpu_;
}

// TODO: Return the command that generated this process
string Process::Command() {
  string line;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid_) +
                           LinuxParser::kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }

  return line;
}

// Return this process's memory utilization
string Process::Ram() {
  string line, key, value;

  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) +
                           LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          mem_ = (int)(stof(value) / 1000.0);
          break;
        }
      }
    }
  }

  // Pad spaces at the end to clear previous chars
  if (to_string(mem_).length() < 5) {
  }

  return to_string(mem_) + "M";
}

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return (long int)(stof(stats_.at(kStarttime_)) / sysconf(_SC_CLK_TCK));
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return cpu_ < a.cpu_; }