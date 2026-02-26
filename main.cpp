#include "algorithms.hpp"
#include "raylib.h"

enum class AppState {
  MENU,
  SHOW_RESULT,
  ANIMATED,
  STEP_BY_STEP,
  ENTER_QUANTUM
};

int main() {
  showPresentation();

  // Procesos
  Process P1("P1", 7);
  Process P2("P2", 3);
  Process P3("P3", 4);
  Process P4("P4", 2);
  Process P5("P5", 4);

  vector<Process> processesOriginal = {P1, P2, P3, P4, P5};
  vector<int> arrivalTimes = {0, 1, 2, 4, 5};

  const int screenWidth = 1400;
  const int screenHeight = 900;

  SetTraceLogLevel(LOG_WARNING);

  InitWindow(screenWidth, screenHeight, "SCHEDULER");
  SetTargetFPS(60);

  AppState state = AppState::MENU;

  Scheduler* scheduler = nullptr;

  int frame = 0;
  float timer = 0.0f;
  std::string quantumInput = "";
  bool firstTime = true;

  while (!WindowShouldClose()) {

    if (state == AppState::MENU) {

      if (IsKeyPressed(KEY_ONE)) {
        auto processes = processesOriginal;
        delete scheduler;
        scheduler = new Scheduler(processes, arrivalTimes);

        scheduler->runFIFO();
        state = AppState::SHOW_RESULT;
      }

      if (IsKeyPressed(KEY_TWO)) {
        auto processes = processesOriginal;
        delete scheduler;
        scheduler = new Scheduler(processes, arrivalTimes);

        scheduler->runSJF();
        state = AppState::SHOW_RESULT;
      }

      if (IsKeyPressed(KEY_THREE)) {
        auto processes = processesOriginal;
        delete scheduler;
        scheduler = new Scheduler(processes, arrivalTimes);

        scheduler->runSRTF();
        state = AppState::SHOW_RESULT;
      }

      if (IsKeyPressed(KEY_FOUR)) {
        state = AppState::ENTER_QUANTUM;
        quantumInput = "";
      }
    }

    if (state == AppState::ENTER_QUANTUM) {

      int key = GetCharPressed();

      while (key > 0) {
        if (key >= '0' && key <= '9') {
          quantumInput += (char)key;
        }
        key = GetCharPressed();
      }

      if (IsKeyPressed(KEY_BACKSPACE) && !quantumInput.empty()) {
        quantumInput.pop_back();
      }

      if (IsKeyPressed(KEY_ENTER)) {

        int q = 0;
        if (!quantumInput.empty())
          q = std::stoi(quantumInput);

        auto processes = processesOriginal;
        delete scheduler;
        scheduler = new Scheduler(processes, arrivalTimes);

        scheduler->runRR(q);

        state = AppState::SHOW_RESULT;
      }
    }

    // animación automática
    if (state == AppState::ANIMATED) {
      timer += GetFrameTime();

      if (timer > 0.15f) {
        frame++;
        timer = 0;
      }
    }

    // paso a paso
    if (state == AppState::STEP_BY_STEP) {
      if (IsKeyPressed(KEY_ENTER)) {
        frame++;
      }
    }

    if (scheduler && frame >= scheduler->getTotalTime() - 1) {
      state = AppState::SHOW_RESULT;
      frame = 0;
      firstTime = false;
    }

    // volver al menú
    if (IsKeyPressed(KEY_M)) {
      state = AppState::MENU;
      frame = 0;
      firstTime = true;
    }

    // DRAW
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (state == AppState::MENU) {
      DrawText("=== SCHEDULER ===", 100, 50, 30, BLACK);
      DrawText("1) FIFO", 100, 120, 20, BLACK);
      DrawText("2) SJF", 100, 150, 20, BLACK);
      DrawText("3) SRTF", 100, 180, 20, BLACK);
      DrawText("4) Round Robin", 100, 210, 20, BLACK);

      DrawText("Elija una opción", 100, 260, 20, DARKGRAY);

    } else if (state == AppState::ENTER_QUANTUM) {

      DrawText("Ingrese Quantum:", 100, 100, 30, BLACK);

      DrawRectangle(100, 150, 200, 40, LIGHTGRAY);
      DrawText(quantumInput.c_str(), 110, 160, 30, BLACK);

      DrawText("ENTER = confirmar", 100, 220, 20, DARKGRAY);
    }

    else if (state == AppState::SHOW_RESULT && scheduler != nullptr) {

      DrawText("Presione A = animado", 1120, 20, 20, DARKGRAY);
      DrawText("Presione S = paso a paso", 1120, 50, 20, DARKGRAY);
      if (firstTime) {
        DrawText("Presione R = resultado", 1120, 80, 20, DARKGRAY);
        DrawText("Presione M = menu", 1120, 110, 20, DARKGRAY);
      } else {
        DrawText("Presione M = menu", 1120, 80, 20, DARKGRAY);
      }

      scheduler->drawProcessTable(100, 40, 20);
      if (!firstTime) scheduler->drawStatic();

      // cambiar modo
      if (IsKeyPressed(KEY_A)) {
        state = AppState::ANIMATED;
        frame = 0;
      }

      if (IsKeyPressed(KEY_S)) {
        state = AppState::STEP_BY_STEP;
        frame = 0;
      }

      if (IsKeyPressed(KEY_R)) {
        firstTime = false;
      }
    }

    else if (state == AppState::ANIMATED && scheduler != nullptr) {
      scheduler->drawProcessTable(100, 40, 20);
      scheduler->drawAnimated(frame);
    }

    else if (state == AppState::STEP_BY_STEP && scheduler != nullptr) {
      scheduler->drawProcessTable(100, 40, 20);
      scheduler->drawStepByStep(frame);
    }

    EndDrawing();
  }

  delete scheduler;
  CloseWindow();
  return 0;
}

