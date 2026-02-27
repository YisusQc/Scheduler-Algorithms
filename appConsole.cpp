#include "app.hpp"
#include "algorithms.hpp"
#include <iostream>
#include <limits>

void App::runConsole() {
  clearConsole();
  showPresentation();

  bool running = true;

  while (running) {
    std::cout << "\n=== MENU Planificador ===\n\n";
    std::cout << "1) FIFO\n";
    std::cout << "2) SJF\n";
    std::cout << "3) SRTF\n";
    std::cout << "4) Round Robin (RR)\n";
    std::cout << "0) Salir\n";
    std::cout << "\nSeleccione una opcion: ";

    int opt;

    if (!(std::cin >> opt)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "\nEntrada invalida.\n";
      cout << "-> Presione ENTER para continuar...";
      std::cin.get();
      continue;
    }

    if (opt == 0) {
      running = false;
      break;
    }

    switch (opt) {

      case 1: {
        clearConsole();
        auto processes = processesOriginal;
        delete scheduler;
        scheduler = new Scheduler(processes, arrivalTimes);
        scheduler->runFIFO();
        scheduler->showProcessTable();
        scheduler->showAnimatedGanttDiagram();
        clearConsole();
        scheduler->showProcessTable();
        scheduler->showGanttDiagram();
        pressToContinue();
        break;
      }

      case 2: {
        clearConsole();
        auto processes = processesOriginal;
        delete scheduler;
        scheduler = new Scheduler(processes, arrivalTimes);
        scheduler->runSJF();
        scheduler->showProcessTable();
        scheduler->showAnimatedGanttDiagram();
        clearConsole();
        scheduler->showProcessTable();
        scheduler->showGanttDiagram();
        pressToContinue();
        break;
      }

      case 3: {
        clearConsole();
        auto processes = processesOriginal;
        delete scheduler;
        scheduler = new Scheduler(processes, arrivalTimes);
        scheduler->runSRTF();
        scheduler->showProcessTable();
        scheduler->showAnimatedGanttDiagram();
        clearConsole();
        scheduler->showProcessTable();
        scheduler->showGanttDiagram();
        pressToContinue();
        break;
      }

      case 4: {
        std::cout << "Ingrese quantum: ";
        int q;
        if (!(std::cin >> q) || q < 0) {
          std::cout << "Quantum invalido. Se usara 0 por defecto.\n";
          std::cout << "...ENTER";
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          q = 0;
        }

        clearConsole();
        auto processes = processesOriginal;
        delete scheduler;
        scheduler = new Scheduler(processes, arrivalTimes);
        scheduler->runRR(q);
        scheduler->showProcessTable();
        scheduler->showAnimatedGanttDiagram();
        clearConsole();
        scheduler->showProcessTable();
        scheduler->showGanttDiagram();
        pressToContinue();
        break;
      }

      default:
        std::cout << "\nOpcion invalida.\n";
        cout << "-> Presione ENTER para continuar...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
  }
}

