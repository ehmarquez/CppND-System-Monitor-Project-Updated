#include <iostream>
#include <string>

#include "linux_parser.h"
#include "processor.h"

// Initialize member variables
Processor::Processor()
    : idle_(0),
      iowait_(0),
      user_(0),
      nice_(0),
      system_(0),
      irq_(0),
      softirq_(0),
      steal_(0),
      total_(0) {}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::string line, cpu;
  float idle, iowait, user, nice, system, irq, softirq, steal;
  float diff_idle, diff_total;

  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal;
  }

  /* Calculate CPU usage based on following source:
   * https://stackoverflow.com/questions/23367857/
   * accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
   */

  diff_idle = (idle + iowait) - (idle_ + iowait_);
  diff_total =
      (idle + iowait + user + nice + system + irq + softirq + steal) -
      (idle_ + iowait_ + user_ + nice_ + system_ + irq_ + softirq_ + steal_);

  // Store read in values into class member variables for next call
  idle_ = idle;
  iowait_ = iowait;
  user_ = user;
  nice_ = nice;
  system_ = system;
  irq_ = irq;
  softirq_ = softirq;
  steal_ = steal;

  return (diff_total - diff_idle) / diff_total;
}