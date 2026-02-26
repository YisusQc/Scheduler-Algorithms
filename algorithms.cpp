#include "algorithms.hpp"
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <chrono>

Process::Process(int burstTime) {
  int invalidBusrtTime = 15;
  if (burstTime < 1) {
    std::cerr << "--> ERROR: 'Process' constructor does not allow a burstTime less than 1.\n";
    exit(EXIT_FAILURE);
  } else if (burstTime > invalidBusrtTime) {
    std::cerr << "--> ERROR: 'Process' constructor does not allow a burstTime greater than " << invalidBusrtTime << ".\n";
    exit(EXIT_FAILURE);
  }
  name = "NONE";
  this->burstTime = burstTime;
  this->originalBurstTime = burstTime;
  
}

Process::Process(string name, int burstTime) {
  int invalidBusrtTime = 15;
  if (burstTime < 1) {
    std::cerr << "--> ERROR: 'Process' constructor does not allow a burstTime less than 1.\n";
    exit(EXIT_FAILURE);
  } else if (burstTime > invalidBusrtTime) {
    std::cerr << "--> ERROR: 'Process' constructor does not allow a burstTime greater than " << invalidBusrtTime << ".\n";
    exit(EXIT_FAILURE);
  } else if (name.length() > 5) {
    std::cerr << "--> ERROR: 'Process' constructor does not allow a name longer than 5 characters.\n";
    exit(EXIT_FAILURE);
  }
  this->name = name;
  this->burstTime = burstTime;
  this->originalBurstTime = burstTime;
  
}

Scheduler::Scheduler(vector<Process>& processes, vector<int>& arrivalTimes) {
  int invalidNumProcess = 10;
  if (processes.empty()) {
    std::cerr << "--> ERROR: 'Scheduler' constructor does not allow an empty 'processes' vector.\n";
    exit(EXIT_FAILURE);
  } else if (processes.size() > invalidNumProcess) {
    std::cerr << "--> ERROR: 'Scheduler' constructor does not allow a number of processes greater than " << invalidNumProcess << ".\n";
    exit(EXIT_FAILURE);
  } else if (arrivalTimes.empty()) {
    std::cerr << "--> ERROR: 'Scheduler' constructor does not allow an empty 'arrivalTimes' vector.\n";
    exit(EXIT_FAILURE);
  } else if (processes.size() != arrivalTimes.size()) {
    std::cerr << "--> ERROR: 'Scheduler' constructor does not allow vectors 'processes' and 'arrivalTimes' with differen sizes.\n";
    exit(EXIT_FAILURE);
  }
  this->processes = processes;
  this->arrivalTimes = arrivalTimes;
  for (Process& p : processes) processNames.push_back(p.name);
}

struct CompareBurstTime {
  vector<Process>* processes;
  vector<int>* arrivalTimes;

  CompareBurstTime(vector<Process>* p, vector<int>* a) : processes(p), arrivalTimes(a) {}
  bool operator()(int i, int j) {
    if ((*processes)[i].burstTime == (*processes)[j].burstTime) {
      if ((*arrivalTimes)[i] == (*arrivalTimes)[j]) {
        return i > j;
      }
      return (*arrivalTimes)[i] > (*arrivalTimes)[j];
    }
    return (*processes)[i].burstTime > (*processes)[j].burstTime;
  }
};

void FIFO::init(vector<Process>& processes, vector<int>& arrivalTimes, int& totalTime, vector<vector<string>>& processesLines, vector<string>& processNames, Process& CPU){
  timeCurrent = 0;
  bool terminateAlgorithm = false;
  queue<int> readyQueue;

  while (!terminateAlgorithm) {

    for (int i = 0; i < arrivalTimes.size(); i++) {
      if (arrivalTimes[i] == timeCurrent) {
        readyQueue.push(i);
        processes[i].processState = READY;
        processes[i].arrivalTime = timeCurrent;
      }
    }

    if (!readyQueue.empty() && (CPU.processState == NEW || CPU.processState == FINISHED)) {
      int idx = readyQueue.front();
      processes[idx].processState = RUNNING;
      CPU = processes[idx];
      readyQueue.pop();
    }

    for (Process& p : processes) {
      if (p.processState != RUNNING && p.arrivalTime != -1)
        p.processLine.push_back("X ");
      else if (p.processState != RUNNING)
        p.processLine.push_back("  ");
      else {
        p.decrementBurstTime();
        if (p.arrivalTime != -1)
          p.processLine.push_back("X#");
        else
          p.processLine.push_back(" #");
      }
      p.arrivalTime = -1;
    }

    if (CPU.processState == RUNNING)
      CPU.decrementBurstTime();
    if (CPU.processState == RUNNING && CPU.burstTime == 0)
      CPU.processState = FINISHED;

    for (Process& p : processes) {
      if (p.burstTime == 0) {
        p.processState = FINISHED;
        if (p.completionTime == -1) p.completionTime = timeCurrent + 1;
      }
    }

    timeCurrent++;

    int terminated = 0;
    for (Process& p : processes) {
      if (p.processState == FINISHED)
        terminated++;
    }
    if (terminated == processes.size()) terminateAlgorithm = true;

  }

  totalTime = timeCurrent;
  processesLines.resize(processes.size());
  for (int i = 0; i < processes.size(); i++)
    processesLines[i] = processes[i].processLine;
}

void SJF::init(vector<Process>& processes, vector<int>& arrivalTimes, int& totalTime, vector<vector<string>>& processesLines, vector<string>& processNames, Process& CPU) {
  timeCurrent = 0;
  bool terminateAlgorithm = false;
  priority_queue<int, vector<int>, CompareBurstTime> readyQueue((CompareBurstTime(&processes, &arrivalTimes)));

  while (!terminateAlgorithm) {

    for (int i = 0; i < arrivalTimes.size(); i++) {
      if (arrivalTimes[i] == timeCurrent) {
        readyQueue.push(i);
        processes[i].processState = READY;
        processes[i].arrivalTime = timeCurrent;
      }
    }

    if (!readyQueue.empty() && (CPU.processState == NEW || CPU.processState == FINISHED)) {
      int idx = readyQueue.top();
      processes[idx].processState = RUNNING;
      CPU = processes[idx];
      readyQueue.pop();
    }

    for (Process& p : processes) {
      if (p.processState != RUNNING && p.arrivalTime != -1)
        p.processLine.push_back("X ");
      else if (p.processState != RUNNING)
        p.processLine.push_back("  ");
      else {
        p.decrementBurstTime();
        if (p.arrivalTime != -1)
          p.processLine.push_back("X#");
        else
          p.processLine.push_back(" #");
      }
      p.arrivalTime = -1;
    }

    if (CPU.processState == RUNNING)
      CPU.decrementBurstTime();
    if (CPU.processState == RUNNING && CPU.burstTime == 0)
      CPU.processState = FINISHED;

    for (Process& p : processes) {
      if (p.burstTime == 0) {
        p.processState = FINISHED;
        if (p.completionTime == -1) p.completionTime = timeCurrent + 1;
      }
    }

    timeCurrent++;

    int terminated = 0;
    for (Process& p : processes) {
      if (p.processState == FINISHED)
        terminated++;
    }
    if (terminated == processes.size()) terminateAlgorithm = true;

  }

  totalTime = timeCurrent;
  processesLines.resize(processes.size());
  for (int i = 0; i < processes.size(); i++)
    processesLines[i] = processes[i].processLine;
}

void SRTF::init(vector<Process>& processes, vector<int>& arrivalTimes, int& totalTime, vector<vector<string>>& processesLines, vector<string>& processNames, Process& CPU) {
  timeCurrent = 0;
  bool terminateAlgorithm = false;
  priority_queue<int, vector<int>, CompareBurstTime> readyQueue((CompareBurstTime(&processes, &arrivalTimes)));
  int idx = -1;

  while (!terminateAlgorithm) {

    for (int i = 0; i < arrivalTimes.size(); i++) {
      if (arrivalTimes[i] == timeCurrent) {
        readyQueue.push(i);
        processes[i].processState = READY;
        processes[i].arrivalTime = timeCurrent;
      }
    }

    if (!readyQueue.empty()) {
      if (idx != -1 && CPU.processState != FINISHED) {
        CPU.processState = READY;
        processes[idx].processState = READY;
        readyQueue.push(idx);
      }
      idx = readyQueue.top();
      processes[idx].processState = RUNNING;
      CPU = processes[idx];
      readyQueue.pop();
    }

    for (Process& p : processes) {
      if (p.processState != RUNNING && p.arrivalTime != -1)
        p.processLine.push_back("X ");
      else if (p.processState != RUNNING)
        p.processLine.push_back("  ");
      else {
        p.decrementBurstTime();
        if (p.arrivalTime != -1)
          p.processLine.push_back("X#");
        else
          p.processLine.push_back(" #");
      }
      p.arrivalTime = -1;
    }

    if (CPU.processState == RUNNING)
      CPU.decrementBurstTime();
    if (CPU.processState == RUNNING && CPU.burstTime == 0)
      CPU.processState = FINISHED;

    for (Process& p : processes) {
      if (p.burstTime == 0) {
        p.processState = FINISHED;
        if (p.completionTime == -1) p.completionTime = timeCurrent + 1;
      }
    }

    timeCurrent++;

    int terminated = 0;
    for (Process& p : processes) {
      if (p.processState == FINISHED)
        terminated++;
    }
    if (terminated == processes.size()) terminateAlgorithm = true;

  }

  totalTime = timeCurrent;
  processesLines.resize(processes.size());
  for (int i = 0; i < processes.size(); i++)
    processesLines[i] = processes[i].processLine;
}

void RR::init(vector<Process>& processes, vector<int>& arrivalTimes, int& totalTime, vector<vector<string>>& processesLines, vector<string>& processNames, Process& CPU, int quantum) {
  timeCurrent = 0;
  bool terminateAlgorithm = false;
  queue<int> readyQueue;
  int idx = -1;
  int q = quantum;

  while (!terminateAlgorithm) {

    for (int i = 0; i < arrivalTimes.size(); i++) {
      if (arrivalTimes[i] == timeCurrent) {
        readyQueue.push(i);
        processes[i].processState = READY;
        processes[i].arrivalTime = timeCurrent;
      }
    }

    if (!readyQueue.empty() && (q == quantum || CPU.processState == FINISHED)) {
      if (idx != -1 && CPU.processState != FINISHED) {
        CPU.processState = READY;
        processes[idx].processState = READY;
        readyQueue.push(idx);
      }
      idx = readyQueue.front();
      processes[idx].processState = RUNNING;
      CPU = processes[idx];
      readyQueue.pop();
    }

    for (int i = 0; i < processes.size(); i++) {
      Process& p = processes[i];
      if (p.processState != RUNNING && p.arrivalTime != -1)
        p.processLine.push_back("X ");
      else if (p.processState != RUNNING)
        p.processLine.push_back("  ");
      else {
        p.decrementBurstTime();
        if (p.arrivalTime != -1)
          p.processLine.push_back("X#");
        else
          p.processLine.push_back(" #");
      }
      p.arrivalTime = -1;
    }

    if (CPU.processState == RUNNING) {
      CPU.decrementBurstTime();
      q--;
    }
    if (CPU.processState == RUNNING && CPU.burstTime == 0) {
      CPU.processState = FINISHED;
      if (processes[idx].completionTime == -1)
        processes[idx].completionTime = timeCurrent + 1;
      q = quantum;
    }

    for (Process& p : processes) {
      if (p.burstTime == 0) {
        p.processState = FINISHED;
        if (p.completionTime == -1) p.completionTime = timeCurrent + 1;
      }
    }

    timeCurrent++;

    int terminated = 0;
    for (Process& p : processes) {
      if (p.processState == FINISHED)
        terminated++;
    }
    if (terminated == processes.size()) terminateAlgorithm = true;

    if (q == 0) q = quantum;

  }

  totalTime = timeCurrent;
  processesLines.resize(processes.size());
  for (int i = 0; i < processes.size(); i++)
    processesLines[i] = processes[i].processLine;
}

void Scheduler::showProcessTable() {
  int maxLength = 9;
  bool lengthClassic = true;
  for (int i = 0; i < processNames.size(); i++) {
    if (processNames[i].length() > maxLength)
      maxLength = processNames[i].length();
    if (processNames[i].length() > 2)
      lengthClassic = false;
  }

  cout << "\t___________________________________________________________________________________________" << endl;
  cout << "\t| Processes | Burst_Time | Arrival_Time | Completion_Time| Turnaround_Time | Waiting_Time | \n";
  cout << "\t|___________|____________|______________|________________|_________________|______________|" << endl;

  for (int i = 0; i < processNames.size(); i++) {
    if (lengthClassic == false) {
      cout << "\t| " << processNames[i];
      for (int k = 0; k < maxLength - processNames[i].length(); k++) cout << " ";
      cout << " |";
    } else {
      if (processNames[i].length() == 1)
        cout << "\t|     " << processNames[i] << "     |";
      else
        cout << "\t|     " << processNames[i] << "    |";  
      
    }
    
    if (processes[i].burstTime < 10)
      cout << "      " << processes[i].burstTime << "     |";
    else
      cout << "     " << processes[i].burstTime << "     |";
    if (arrivalTimes[i] < 10)
      cout << "       " << arrivalTimes[i] << "      |";
    else
      cout << "      " << arrivalTimes[i] << "      |";
    if (processes[i].completionTime < 10)
      cout << "       " << processes[i].completionTime << "       |";
    else
      cout << "      " << processes[i].completionTime << "       |";

    int tat = processes[i].completionTime - arrivalTimes[i];
    if (tat < 10)
        cout << "        " << tat << "       |";
    else
        cout << "       " << tat << "       |";

    int wt = tat - processes[i].originalBurstTime;
    if (wt < 10)
        cout << "      " << wt << "      |";
    else
        cout << "     " << wt << "     |";
  
    cout << endl;
  }

  cout << "\t|___________|____________|______________|________________|_________________|______________|" << endl;
  cout << endl;
}

void Scheduler::showGanttDiagram() {

  int maxLength = 0;
  for (int i = 0; i < processNames.size(); i++) {
    if (processNames[i].length() > maxLength)
      maxLength = processNames[i].length();
  }
  cout << "    " << "[" << algorithmName << "]" << endl;

  cout << "     ";
  for (int i = 0; i < maxLength; i++) cout << " ";
  for (int i = 0; i < totalTime; i++)
    cout << "---";
  cout << "-" << endl;

  for (int i = 0; i < processNames.size(); i++) {
    cout << "    " << processNames[i];
    for (int k = 0; k < maxLength - processNames[i].length(); k++) cout << " ";
    cout << " |";
    for (int j = 0; j < processesLines[i].size(); j++) {
      cout << processesLines[i][j] << "|";
    }
    cout << endl;
  }

  cout << "     ";
  for (int i = 0; i < maxLength; i++) cout << " ";
  for (int i = 0; i < totalTime; i++)
    cout << "---";
  cout << "-" << endl;

  cout << "      ";
  for (int i = 0; i < maxLength; i++) cout << " ";
  for (int i = 0; i < totalTime; i++) {
    if (i >= 0 && i < 10)
      cout << " " << i << " ";
    else if (i >= 10)
      cout << i << " ";
  }
  
  cout << "\n" << endl;
}

void Scheduler::showAnimatedGanttDiagram(int milliseconds) {
  if (milliseconds < 0) {
    std::cerr << "-- ERROR: 'showAnimatedGanttDiagram()' function does not allow a negative parameter --\n";
    exit(EXIT_FAILURE);
  }

  int maxLength = 0;
  for (int i = 0; i < processNames.size(); i++) {
    if (processNames[i].length() > maxLength)
      maxLength = processNames[i].length();
  }

  cout << "-> Presione cualquier tecla para continuar...";
  system("PAUSE");

  for (int frame = 0; frame < totalTime; frame++) {

    system("cls");

    showProcessTable();
    cout << "    " << "[" << algorithmName << "]" << endl;

    cout << "     ";
    for (int i = 0; i < maxLength; i++) cout << " ";
    for (int i = 0; i < totalTime; i++)
      cout << "---";
    cout << "-" << endl;

    for (int i = 0; i < processNames.size(); i++) {
      cout << "    " << processNames[i];
      for (int k = 0; k < maxLength - processNames[i].length(); k++) cout << " ";
      cout << " |";

      for (int j = 0; j <= frame; j++) {
        cout << processesLines[i][j] << "|";
      }
      cout << endl;
    }

    cout << "     ";
    for (int i = 0; i < maxLength; i++) cout << " ";
    for (int i = 0; i < totalTime; i++)
      cout << "---";
    cout << "-" << endl;

    cout << "      ";
    for (int i = 0; i < maxLength; i++) cout << " ";
    for (int i = 0; i < totalTime; i++) {
      if (i >= 0 && i < 10)
        cout << " " << i << " ";
      else if (i >= 10)
        cout << i << " ";
    }
    
    cout << "\n" << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

  }
}

void showPresentation() {
  cout << "\t_________________________________________" << endl;
  cout << "\t|\t\t\t\t\t|" << endl;
  cout << "\t|\tPlanificador de procesos\t|" << endl;
  cout << "\t|_______________________________________|\n" << endl;
}

int Scheduler::getTotalTime() {
  return totalTime;
}

