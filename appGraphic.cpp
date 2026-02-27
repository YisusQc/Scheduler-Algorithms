#include "app.hpp"
#include "raylib.h"

App::App(const vector<Process>& processes, const vector<int>& arrivals)
: processesOriginal(processes),
  arrivalTimes(arrivals),
  scheduler(nullptr),
  state(State::MENU),
  frame(0),
  timer(0.0f),
  quantumInput(""),
  firstTime(true)
{}

void App::init() {
  const int screenWidth = 1400;
  const int screenHeight = 900;

  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screenWidth, screenHeight, "SCHEDULER");
  SetTargetFPS(60);
}

void App::runGraphic() {
  init();

  while (!WindowShouldClose()) {
    update();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw();

    EndDrawing();
  }

  unload();
}

void App::update() {

  switch (state) {

  case State::MENU:
    handleMenu();
    break;

  case State::ENTER_QUANTUM:
    handleQuantumInput();
    break;

  case State::ANIMATED:
    timer += GetFrameTime();
    if (timer > 0.15f) {
      frame++;
      timer = 0;
    }
    break;

  case State::STEP_BY_STEP:
    if (IsKeyPressed(KEY_ENTER)) {
      frame++;
    }
    break;

  default:
    break;
  }

  if (scheduler && frame >= scheduler->getTotalTime() - 1) {
    state = State::SHOW_RESULT;
    frame = 0;
    firstTime = false;
  }

  if (IsKeyPressed(KEY_M)) {
    state = State::MENU;
    frame = 0;
    firstTime = true;
  }
}

void App::handleMenu() {

  if (IsKeyPressed(KEY_ONE)) {
    auto processes = processesOriginal;
    delete scheduler;
    scheduler = new Scheduler(processes, arrivalTimes);

    scheduler->runFIFO();
    state = State::SHOW_RESULT;
  }

  if (IsKeyPressed(KEY_TWO)) {
    auto processes = processesOriginal;
    delete scheduler;
    scheduler = new Scheduler(processes, arrivalTimes);

    scheduler->runSJF();
    state = State::SHOW_RESULT;
  }

  if (IsKeyPressed(KEY_THREE)) {
    auto processes = processesOriginal;
    delete scheduler;
    scheduler = new Scheduler(processes, arrivalTimes);

    scheduler->runSRTF();
    state = State::SHOW_RESULT;
  }

  if (IsKeyPressed(KEY_FOUR)) {
    state = State::ENTER_QUANTUM;
    quantumInput = "";
  }
}

void App::handleQuantumInput() {

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

    state = State::SHOW_RESULT;
  }
}

void App::draw() {

  if (state == State::MENU) {

    DrawText("=== SCHEDULER ===", 100, 50, 30, BLACK);
    DrawText("1) FIFO", 100, 120, 20, BLACK);
    DrawText("2) SJF", 100, 150, 20, BLACK);
    DrawText("3) SRTF", 100, 180, 20, BLACK);
    DrawText("4) Round Robin", 100, 210, 20, BLACK);
    DrawText("Elija una opción", 100, 260, 20, DARKGRAY);
    DrawText("ESC -> Salir", 100, 290, 20, DARKGRAY);

  } else if (state == State::ENTER_QUANTUM) {

    DrawText("Ingrese Quantum:", 100, 100, 30, BLACK);
    DrawRectangle(100, 150, 200, 40, LIGHTGRAY);
    DrawText(quantumInput.c_str(), 110, 160, 30, BLACK);
    DrawText("ENTER = confirmar", 100, 220, 20, DARKGRAY);
    DrawText("M = regresar a Menú", 100, 250, 20, DARKGRAY);

  } else if (state == State::SHOW_RESULT && scheduler) {

    DrawText("Presione A = animado", 1120, 20, 20, DARKGRAY);
    DrawText("Presione S = paso a paso", 1120, 50, 20, DARKGRAY);

    if (firstTime) {
      DrawText("Presione R = resultado", 1120, 80, 20, DARKGRAY);
      DrawText("Presione M = menu", 1120, 110, 20, DARKGRAY);
    } else {
      DrawText("Presione M = menu", 1120, 80, 20, DARKGRAY);
    }

    scheduler->drawProcessTable(100, 40, 20);

    if (!firstTime)
      scheduler->drawStatic();

    if (IsKeyPressed(KEY_A)) {
      state = State::ANIMATED;
      frame = 0;
    }

    if (IsKeyPressed(KEY_S)) {
      state = State::STEP_BY_STEP;
      frame = 0;
    }

    if (IsKeyPressed(KEY_R)) {
      firstTime = false;
    }

  } else if (state == State::ANIMATED && scheduler) {
      scheduler->drawProcessTable(100, 40, 20);
      scheduler->drawAnimated(frame);

  } else if (state == State::STEP_BY_STEP && scheduler) {
      scheduler->drawProcessTable(100, 40, 20);
      scheduler->drawStepByStep(frame);
  }
}

void App::unload() {
  delete scheduler;
  scheduler = nullptr;
  CloseWindow();
}

