//
// Created by Carlos Reyes on 2/27/2024.
//
#include "process.h"
#include <string>
process::process(int procID, unsigned int arrayBursts[],int arraySize){
    //Constructor for process
    //We first set up our bursts array with malloc for the array size
    allArrayBursts = (int*) malloc(arraySize * sizeof (int));
    //we iterate array to set up elements
    for(int i = 0; i<arraySize;i++){
        allArrayBursts[i] = arrayBursts[i];
    }
    arrayNextBurst = 0;//array of next burst is zero at start
    ID = procID; //our current process ID
    remBurst = 0; //remaining bursts of process
    cpuExecuted = 0;//CPU bursts so far
    IOBursts = 0; // executed IO bursts
    turnTime = 0;// Turnaround time of the pro
}
void const process::executeProcess(int Time, int procFlag){
    //This function sets attributes when current process is executed for Time procFlag
    //Determines if process is CPU:0 or IO:1
    remBurst -= Time; //update remaining bursts
    turnTime += Time; //update turn arround time
    //updated cpu bursts based on proc flag being cpu or IO burst
    if(procFlag==0){
        cpuExecuted += Time;
    }
    else{
        IOBursts += Time;
    }
    //we updated the current burst remaining time
    curBTime -= Time;
}

bool comp(process *a, process *b){
    if((a->curBTime)>(b->curBTime)){
        return true;
    }
    return false;
}


