#include "app.hpp"

int main() {

  Process P1("P1", 7);
  Process P2("P2", 3);
  Process P3("P3", 4);
  Process P4("P4", 2);
  Process P5("P5", 4);

  vector<Process> processes = {P1, P2, P3, P4, P5};
  vector<int> arrivals = {0, 1, 2, 4, 5};

  App Scheduler(processes, arrivals);
  Scheduler.run();

  return 0;
}

