//
// Created by Carlos Reyes on 2/28/2024.
//

#ifndef CS420PROJECT2_SCHEDULE_H
#define CS420PROJECT2_SCHEDULE_H
#include "process.h"
#include <queue>
#include <string>

class schedule {
public:
    schedule();
    int totalTime;
    std::queue<process> completionLog; //keep track on completed bursts
    std::queue<process> Ready; // our ready queue
    std::priority_queue <process*, std::vector<process*>, Compare> Blocked;
    //Our priority queue with custom comparator to have it in aschending order according to current burst time left
    void scheduleRR(char const *fileName, int TimeQuantum = 2 ); // Schedule for RoundRobin
    void scheduleFCFS(char const *fileName); //Schedule for FCFS
    std::queue<process> readySetup(char const *fileName); //Setup  for ready queue returns the reeady queue
    process createProcess(std::string bursts, int ID); //creates a process given a string of bursts
    int executeReady(int Qtime, bool sameTime);//Function to execute Ready queue for CPU bursts
    void executeBlocked(int Qtime,bool sameTime); //Function to execute Blocking queue for IO bursts
    void logFinalComp(); // function to log the final data for the completed process of the completion log queue
};



#endif
