#pragma once

#include <string>
#include <vector>
using namespace std;

enum ProcessState { NEW, READY, RUNNING, FINISHED };

class Process {
  private:
    string name;
    int burstTime;
    int originalBurstTime;
    int arrivalTime = -1;
    int completionTime = -1;
    vector<string> processLine;
    ProcessState processState = NEW;

    void decrementBurstTime() { burstTime--; }

    friend class Scheduler;
    friend class FIFO;
    friend class SJF;
    friend class SRTF;
    friend class RR;
    friend struct CompareBurstTime;

  public:
    Process() : name("NONE"), burstTime(1) {}
    Process(int burstTime);
    Process(string name, int burstTime);
    ~Process() {}
    int getCompletionTime() const { return completionTime; }
    
};

class FIFO {
  private:
    int timeCurrent;
    void init(vector<Process>& processes, vector<int>& arrivalTimes, int& totalTime, vector<vector<string>>& processesLines, vector<string>& processNames, Process& CPU);
    friend class Scheduler;
};

class SJF {
  private:
    int timeCurrent;
    void init(vector<Process>& processes, vector<int>& arrivalTimes, int& totalTime, vector<vector<string>>& processesLines, vector<string>& processNames, Process& CPU);
    friend class Scheduler;
};

class SRTF {
  private:
    int timeCurrent;
    void init(vector<Process>& processes, vector<int>& arrivalTimes, int& totalTime, vector<vector<string>>& processesLines, vector<string>& processNames, Process& CPU);
    friend class Scheduler;
};

class RR {
  private:
    int timeCurrent;
    void init(vector<Process>& processes, vector<int>& arrivalTimes, int& totalTime, vector<vector<string>>& processesLines, vector<string>& processNames, Process& CPU, int quantum);
    friend class Scheduler;
};

class Scheduler {
  private:
    FIFO fifo;
    SJF sjf;
    SRTF srtf;
    RR rr;

    string algorithmName;
    Process CPU;

    vector<Process> processes;
    vector<int> arrivalTimes;
    vector<int> completionTimes;
    int totalTime;
    vector<vector<string>> processesLines;
    vector<string> processNames;

  public:
    Scheduler(vector<Process>& processes, vector<int>& arrivalTimes);
    ~Scheduler() {}

    int getTotalTime();

    void runFIFO() { fifo.init(processes, arrivalTimes, totalTime, processesLines, processNames, CPU); algorithmName = "FIFO"; }
    void runSJF() { sjf.init(processes, arrivalTimes, totalTime, processesLines, processNames, CPU); algorithmName = "SJF"; }
    void runSRTF() { srtf.init(processes, arrivalTimes, totalTime, processesLines, processNames, CPU); algorithmName = "SRTF"; }
    void runRR(int quantum = 1) { rr.init(processes, arrivalTimes, totalTime, processesLines, processNames, CPU, quantum); algorithmName = "Round Robin, q = " + to_string(quantum); }

    void showProcessTable();
    void showGanttDiagram();
    void showAnimatedGanttDiagram(int milliseconds = 150);

    void drawProcessTable(int startX, int startY, int fontSize);
    void drawAnimated(int frame);
    void drawStepByStep(int frame);
    void drawStatic();
};

void showPresentation();
void clearConsole();
void pressToContinue();

