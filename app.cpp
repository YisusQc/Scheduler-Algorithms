#include "app.hpp"
#include "algorithms.hpp"
#include <iostream>
#include <limits>

void App::run() {
  bool running = true;

  while (running) {
    clearConsole();
    std::cout << "\n";
    std::cout << "1) Modo Consola\n";
    std::cout << "2) Modo Grafico\n";
    std::cout << "0) Salir\n";
    std::cout << "\nSeleccione una opcion: ";

    int mode;

    if (!(std::cin >> mode)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "\nEntrada invalida.\n";
      cout << "-> Presione ENTER para continuar...";
      std::cin.get();
      continue;
    }

    if (mode == 0) {
      running = false;
      break;
    }

    switch (mode) {

      case 1:
        runConsole();
        clearConsole();
        break;

      case 2:
        runGraphic(); 
        running = false;
        break;

      default:
        std::cout << "\nOpcion invalida.\n";
        cout << "-> Presione ENTER para continuar...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
  }

  std::cout << "\n";
}

