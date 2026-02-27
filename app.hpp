#pragma once

#include "algorithms.hpp"
#include <vector>
#include <string>

class App {
public:
  App(const vector<Process>& processes, const vector<int>& arrivals);

  ~App() {};

  void run();

private:
  enum class State {
    MENU,
    SHOW_RESULT,
    ANIMATED,
    STEP_BY_STEP,
    ENTER_QUANTUM
  };

  // datos
  vector<Process> processesOriginal;
  vector<int> arrivalTimes;

  Scheduler* scheduler;

  // estado
  State state;

  int frame;
  float timer;
  string quantumInput;
  bool firstTime;

  // modos de app
  void runGraphic();
  void runConsole();

  // funciones internas
  void init();
  void update();
  void draw();
  void unload();

  void handleMenu();
  void handleQuantumInput();
  void handleSimulation();
};

