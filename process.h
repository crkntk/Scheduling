//
// Created by Carlos Reyes on 2/27/2024.
//

#ifndef CS420PROJECT2_PROCESS_H
#define CS420PROJECT2_PROCESS_H
#include <queue>
#include <array>

class process {
public:
    //This is a constructor that takes id and the array of all bursts and the size of that array
    process(int procID, unsigned int burstArr[], int ArraySize);
    int *allArrayBursts; // This array has all the process bursts from the start
    int sizeOfArrayBursts; //the size of the array of all bursts
    int arrayNextBurst; //The index for the nex burst in the allArrayBursts array
    int curBTime; //Current burst remaining time
    int ID; //ID of current process
    int remBurst; //remaining bursts time of process
    int cpuExecuted; //CPU bursts time so far
    int IOBursts; // executed IO bursts
    int turnTime;// Turnaround time of the process
    void const executeProcess(int Time, int procFlag);
    //executed the process takes in time to execute process for and flag
    // to tell weather a CpuBurst = 0 or IOBurst=1
};
//This is our custom comparator for our priority queue this ensures that priority queue for blocked queue is stables
//sorted
class Compare {
public:
    bool operator()(process *a, process *b){
        if((a->curBTime) > (b->curBTime)){
            return true;
        }
        return false;
    }
};


#endif
