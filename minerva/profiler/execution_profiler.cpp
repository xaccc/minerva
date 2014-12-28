#include "profiler/execution_profiler.h"
#include <cstdio>

using namespace std;

namespace minerva {

ExecutionProfiler::ExecutionProfiler() {
}

ExecutionProfiler::~ExecutionProfiler() {
  for (auto it : time_) {
    delete[] (it.second);
  }
}

void ExecutionProfiler::RecordTime(TimerType type, const string& name, const Timer& timer) {
  lock_guard<mutex> lock_(m_);
  auto it = time_.find(name);
  if (it == time_.end()) {  // Not existent before
    time_.insert({name, new double[static_cast<size_t>(TimerType::kCount)]()});
  } else {
    (it->second)[static_cast<size_t>(type)] += timer.ReadMicrosecond();
  }
}

void ExecutionProfiler::PrintResult() {
  printf("%33s|%6sMemory%8sCalculation\n", "", "", "");
  for (int i = 0; i < 33; ++i) {
    printf("-");
  }
  printf("|");
  for (int i = 0; i < 34; ++i) {
    printf("-");
  }
  printf("\n");
  for (auto it : time_) {
    printf("%32.32s | %16f %16f\n", it.first.c_str(), it.second[0], it.second[1]);
  }
}

}  // namespace minerva
