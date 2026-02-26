#include "raylib.h"
#include "algorithms.hpp"
#include <iostream>

void Scheduler::drawProcessTable(int startX, int startY, int fontSize) {
  int center = 10;

  DrawLine(startX, startY - 30 + fontSize, startX + 1000, startY - 30 + fontSize, BLACK);

  // Encabezados
  DrawText("Processes", startX + center, startY, fontSize, BLACK);
  DrawText("Burst_Time", startX + 150 + center, startY, fontSize, BLACK);
  DrawText("Arrival_Time", startX + 300 + center, startY, fontSize, BLACK);
  DrawText("Completion_Time", startX + 450 + center, startY, fontSize, BLACK);
  DrawText("Turnaround_Time", startX + 650 + center, startY, fontSize, BLACK);
  DrawText("Waiting_Time", startX + 850 + center, startY, fontSize, BLACK);

  DrawLine(startX, startY + fontSize + 5, startX + 1000, startY + fontSize + 5, BLACK);

  for (int i = 0; i < processNames.size(); i++) {
    int rowY = startY - 10 + (i + 2) * (fontSize + 5);

    DrawText(processNames[i].c_str(), startX + center * 6, rowY, fontSize, BLACK);

    DrawText(TextFormat("%d", processes[i].originalBurstTime), startX + 150 + center * 7, rowY, fontSize, BLACK);

    DrawText(TextFormat("%d", arrivalTimes[i]), startX + 300 + center * 7, rowY, fontSize, BLACK);
    std::string compStr = (processes[i].completionTime == -1) ? std::string("-") : TextFormat("%d", processes[i].completionTime);
    DrawText(compStr.c_str(), startX + 450 + center * 7, rowY, fontSize, BLACK);

    if (processes[i].completionTime == -1) {
      DrawText("-", startX + 650 + center * 7, rowY, fontSize, BLACK);
      DrawText("-", startX + 850 + center * 7, rowY, fontSize, BLACK);
    } else {
      int tat = processes[i].completionTime - arrivalTimes[i];
      DrawText(TextFormat("%d", tat), startX + 650 + center * 7, rowY, fontSize, BLACK);
      int wt = tat - processes[i].originalBurstTime;
      DrawText(TextFormat("%d", wt), startX + 850 + center * 7, rowY, fontSize, BLACK);
    }
  }

  DrawLine(startX, startY + (processNames.size() + 2) * (fontSize + 5),
           startX + 1000, startY + (processNames.size() + 2) * (fontSize + 5), BLACK);

  DrawLine(startX, startY - 30 + fontSize, startX, startY + (processNames.size() + 2) * (fontSize + 5), BLACK);
  DrawLine(startX + 150, startY - 30 + fontSize, startX + 150, startY + (processNames.size() + 2) * (fontSize + 5), BLACK);
  DrawLine(startX + 300, startY - 30 + fontSize, startX + 300, startY + (processNames.size() + 2) * (fontSize + 5), BLACK);
  DrawLine(startX + 450, startY - 30 + fontSize, startX + 450, startY + (processNames.size() + 2) * (fontSize + 5), BLACK);
  DrawLine(startX + 630, startY - 30 + fontSize, startX + 630, startY + (processNames.size() + 2) * (fontSize + 5), BLACK);
  DrawLine(startX + 850, startY - 30 + fontSize, startX + 850, startY + (processNames.size() + 2) * (fontSize + 5), BLACK);
  DrawLine(startX + 1000, startY - 30 + fontSize, startX + 1000, startY + (processNames.size() + 2) * (fontSize + 5), BLACK);
}

void Scheduler::drawAnimated(int frame) {
  if (frame < 0) {
    std::cerr << "--> ERROR: 'drawAnimated()' function does not allow a negative frame.\n";
    exit(EXIT_FAILURE);
  }

  int sepYbetween = 20;
  int posXgeneral = 150;
  int posYgeneral = 40;

  int finalTablePos = (processNames.size() + 2) * 25 + posYgeneral;

  int cellWidth = 30;  
  int cellHeight = 30; 
  int offsetX = 150;  
  int offsetY = finalTablePos + sepYbetween * 3; 

  Color colors[10] = { RED, GREEN, BLUE, ORANGE, PURPLE, PINK, GRAY, YELLOW, DARKBLUE, BROWN };

  if (frame >= totalTime) frame = totalTime - 1;

  DrawText(TextFormat("[%s]", algorithmName.c_str()), posXgeneral, sepYbetween + finalTablePos, 20, BLACK);

  for (int i = 0; i < processNames.size(); i++) {
    DrawText(processNames[i].c_str(), posXgeneral - 60, offsetY + i * cellHeight, 20, BLACK);
  }

  for (int i = 0; i < processNames.size(); i++) {
    for (int j = 0; j <= frame; j++) {
      std::string cell = processesLines[i][j];

      if (cell.find("#") != std::string::npos) {
          DrawRectangle(offsetX + j * cellWidth,
                        offsetY + i * cellHeight,
                        cellWidth, cellHeight,
                        colors[i % 10]);
      }
      if (cell.find("X") != std::string::npos) {
          DrawText("X",
             offsetX + j * cellWidth + cellWidth/2 - 5,  
             offsetY + i * cellHeight + cellHeight/2 - 10, 
             20, BLACK);
      }

      DrawRectangleLines(offsetX + j * cellWidth,
                         offsetY + i * cellHeight,
                         cellWidth, cellHeight,
                         BLACK);
    }
  }

  for (int j = 0; j < totalTime; j++) {
    DrawText(TextFormat("%d", j),
             offsetX + j * cellWidth + 10,
             offsetY + processNames.size() * cellHeight + 10,
             15, BLACK);
  }
}

void Scheduler::drawStepByStep(int frame) {
  if (frame < 0) {
    std::cerr << "--> ERROR: 'drawStepByStep()' function does not allow a negative frame.\n";
    exit(EXIT_FAILURE);
  }

  int sepYbetween = 20;
  int posXgeneral = 150;
  int posYgeneral = 40;

  int finalTablePos = (processNames.size() + 2) * 25 + posYgeneral;

  int cellWidth = 30;   
  int cellHeight = 30; 
  int offsetX = 150;   
  int offsetY = finalTablePos + sepYbetween * 3;   

  Color colors[10] = { RED, GREEN, BLUE, ORANGE, PURPLE, PINK, GRAY, YELLOW, DARKBLUE, BROWN };

  if (frame >= totalTime) frame = totalTime - 1;

  DrawText(TextFormat("[%s]", algorithmName.c_str()), posXgeneral, sepYbetween + finalTablePos, 20, BLACK);

  for (int i = 0; i < processNames.size(); i++) {
    DrawText(processNames[i].c_str(), posXgeneral - 60, offsetY + i * cellHeight, 20, BLACK);
  }

  for (int i = 0; i < processNames.size(); i++) {
    for (int j = 0; j <= frame; j++) {
      std::string cell = processesLines[i][j];

      if (cell.find("#") != std::string::npos) {
        DrawRectangle(offsetX + j * cellWidth,
                      offsetY + i * cellHeight,
                      cellWidth, cellHeight,
                      colors[i % 10]);
      }
      if (cell.find("X") != std::string::npos) {
        DrawText("X",
           offsetX + j * cellWidth + cellWidth/2 - 5,   
           offsetY + i * cellHeight + cellHeight/2 - 10, 
           20, BLACK);
      }

      DrawRectangleLines(offsetX + j * cellWidth,
                         offsetY + i * cellHeight,
                         cellWidth, cellHeight,
                         BLACK);
    }
  }

  for (int j = 0; j < totalTime; j++) {
    DrawText(TextFormat("%d", j),
             offsetX + j * cellWidth + 10,
             offsetY + processNames.size() * cellHeight + 10,
             15, BLACK);
  }

  if (IsKeyPressed(KEY_ENTER)) {
    frame++;
  }
}

void Scheduler::drawStatic() {

  int sepYbetween = 20;
  int posXgeneral = 150;
  int posYgeneral = 40;

  int finalTablePos = (processNames.size() + 2) * 25 + posYgeneral;

  int cellWidth = 30;  
  int cellHeight = 30; 
  int offsetX = 150;  
  int offsetY = finalTablePos + sepYbetween * 3; 

  Color colors[10] = { RED, GREEN, BLUE, ORANGE, PURPLE, PINK, GRAY, YELLOW, DARKBLUE, BROWN };

  DrawText(TextFormat("[%s]", algorithmName.c_str()), posXgeneral, sepYbetween + finalTablePos, 20, BLACK);

  for (int i = 0; i < processNames.size(); i++) {
    DrawText(processNames[i].c_str(), posXgeneral - 60, offsetY + i * cellHeight, 20, BLACK);
  }

  for (int i = 0; i < processNames.size(); i++) {
    for (int j = 0; j < processesLines[i].size(); j++) {

      std::string cell = processesLines[i][j];

      if (cell.find("#") != std::string::npos) {
        DrawRectangle(offsetX + j * cellWidth,
                      offsetY + i * cellHeight,
                      cellWidth, cellHeight,
                      colors[i % 10]);
      }
      if (cell.find("X") != std::string::npos) {
        DrawText("X",
           offsetX + j * cellWidth + cellWidth/2 - 5,  
           offsetY + i * cellHeight + cellHeight/2 - 10, 
           20, BLACK);
      }

      DrawRectangleLines(offsetX + j * cellWidth,
                         offsetY + i * cellHeight,
                         cellWidth, cellHeight,
                         BLACK);
    }
  }

  for (int j = 0; j < totalTime; j++) {
    DrawText(TextFormat("%d", j),
             offsetX + j * cellWidth + 10,
             offsetY + processNames.size() * cellHeight + 10,
             15, BLACK);
  }
}

