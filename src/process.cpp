#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

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

// Return this process's CPU utilization
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
    total_time = std::stof(stats_.at(LinuxParser::PIDStats::kUtime_)) +
                 std::stof(stats_.at(LinuxParser::PIDStats::kStime_)) +
                 std::stof(stats_.at(LinuxParser::PIDStats::kCutime_)) +
                 std::stof(stats_.at(LinuxParser::PIDStats::kCstime_));
    seconds =
        LinuxParser::UpTime() -
        (std::stof(stats_.at(LinuxParser::PIDStats::kStarttime_)) / clock_hz_);
  }

  // Store values into class members for updates
  cpu_ = ((total_time / clock_hz_) / seconds);

  return cpu_;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() {
  mem_ = (int)(stof(LinuxParser::Ram(pid_)) / 1024.0);
  return to_string(mem_) + "M\n";
}

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(LinuxParser::Uid(pid_)); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return (long int)LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return a.cpu_ < cpu_; }