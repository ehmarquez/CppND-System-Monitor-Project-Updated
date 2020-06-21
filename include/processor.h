#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp

  // Declare any necessary private members
 private:
  int idle_;
  int iowait_;
  int user_;
  int nice_;
  int system_;
  int irq_;
  int softirq_;
  int steal_;
  int total_;
};

#endif