//
// Created by Carlos Reyes on 2/28/2024.
//
#include <queue>
#include <string>
#include <iostream>
#include "schedule.h"
#include <fstream>
#include "process.h"
#include "log.h"

schedule:: schedule(){
    //schedule constructor starts with totalTime of zero
    totalTime = 0;
}
void schedule::scheduleRR(char const *fileName, int TimeQuantum ){
    //This function handle the Round Robin schedule
    Ready = readySetup(fileName); //This is a call to ready setup that sets up the Ready Queue with file name as input
    //int execNum = 1; //execution number for debugging
    while(!Ready.empty() || !Blocked.empty()){
        //Executes the Ready Queue first if its empty and the Blocked Queue after if its empty
        // The second argument for each execution of the queue is for same time execution if it's called from here
        // we are executing at different times not in the same time
        if(!Ready.empty()) {
            executeReady(TimeQuantum,false);
        }
        else if (!Blocked.empty()){
            executeBlocked(TimeQuantum,false);
        }
        //execNum++;
    }
    //logs the final completion of all processes
    logFinalComp();
}
void schedule::scheduleFCFS(char const *fileName){
    //This function handle the Round Robin schedule
    Ready = readySetup(fileName); //This is a call to ready setup that sets up the Ready Queue with file name as input
    while(!Ready.empty() || !Blocked.empty()){
        //Executes the Ready Queue first if its empty and the Blocked Queue after if its empty
        // The second argument for each execution of the queue is for same time execution if it's called from here
        // we are executing at different times not in the same time
        if(!Ready.empty()) {
            //Since we are doing first come firs serve we are executing for the current Burst time at the
            //front fo ready Queue
            int timeToExecute = Ready.front().curBTime;
            executeReady(timeToExecute,false);
        }
        else if (!Blocked.empty()){
            //Since we are doing first come first serve we are executing for the current Burst time at the
            //front fo Blocked Queue
            int timeToExecute = Ready.front().curBTime;
            executeBlocked(timeToExecute,false);
        }
    }
    logFinalComp();
    }
    //Schedule for FCFS
std::queue<process> schedule::readySetup(char const *fileName){
    //sets up the ready queue from given file;
    std::queue<process> currQue;
    std::string line; //line variable for each line for each process
    std::ifstream myfile (fileName);//variable for file processing
    int processID = 0; //ID for fist burst
    //File processing and error checking
    if (myfile.is_open())
    {
        while ( myfile.good() )//Checks if there are no erros on the file currently
        {
            getline (myfile,line); //get current line
            //calls create Process that creates the process with ID and the current line of bursts
            currQue.push(createProcess(line,processID));
            processID++;// adds 1 for next process
        }
        myfile.close();
        return currQue; //returns the queue for the Ready queue
    }
    //error handling for bad file
    else std::cout << "Unable to open <<burst.txt>>";
    return currQue;
}
process schedule::createProcess(std::string bursts, int ID){
    //This function takes a bursts string that has all bursts and ID of current burst to be created
    std::queue<int> BQueue;//Queue to store values
    std::string numberS; // This string is to build the integers from bursts strings
    int size=0; // This is to keep track of the size of the bursts which is the array
    for(int i =0;bursts[i] != '\0';i++) {
        //This for loop is to get the size to allocate memory for the size of the bursts array
        if ((bursts[i + 1] == '\0' && bursts[i] != ' ') || (bursts[i + 1] == ' ' && bursts[i] != ' ')) {
            size++;
            continue;
        }
        else if (bursts[i] == ' ') {
            continue;
        }
    }
    unsigned int burstArr[size];// array to store the bursts in the array
    int currNumber = 0;//This is to keep track of the burst added to the array index
    for(int i =0;bursts[i] != '\0';i++){
        //processes each burst in the bursts string and puts it in the burstsArr we convert and process each line white sapce
        //accordingly we clear the numberS string as well for next iteration
        if((bursts[i + 1] == '\0' && bursts[i] != ' ')||(bursts[i + 1] == ' ' && bursts[i] != ' ')){
            numberS += bursts[i];
            if(stoi(numberS)<=0){
                std::cout << "A burst number must be bigger than 0";
                exit(0);
            }
            burstArr[currNumber] = stoi(numberS);
            currNumber++;
            numberS.clear();
            numberS = "";
            continue;
        }
        else if(bursts[i] == ' '){
            continue;
        }
        numberS += bursts[i];
    }
    numberS.clear();
    //Checks that we have the correct number of bursts and not even number usting the size of the array and moding it
    if((sizeof(burstArr)/sizeof(int) % 2) == 0){
        std::cout << "There must be an odd number of bursts for each process";
        exit(0);
    }
    //calls the log bursts to print out the bursts
   log_process_bursts(burstArr, (unsigned int) size);
    process *currProccess = new process(ID,burstArr, size); // creates new process with ID burst array and size of the array
    currProccess->curBTime = currProccess->allArrayBursts[currProccess->arrayNextBurst];//instantiates the current burst
    currProccess->sizeOfArrayBursts = size; //instantiates teh size of the array
    currProccess->arrayNextBurst = (currProccess->arrayNextBurst) + 1; //to get the next burst we keep track of the current index to the next one
    return *currProccess;
}

int schedule::executeReady(int Qtime, bool sameTime) {
    //Function to execute the readying Queue for Qtime variable and a check for same time execution
    if(!Ready.empty()) {
        process *const currProc = &Ready.front(); //Gets the front of ready queue
        int execTime; //Time that was executed in ready queue
        //This is the branch for quantum expiration
        if ((Qtime - (currProc->curBTime)) < 0) {
            //If we aren't executing at the same time then we want to add the time to total Time
            if(!sameTime){
                totalTime += Qtime;
            }
            currProc->executeProcess(Qtime, 0); //makes call to execute the process for the time necessary and 0 meaning its a CPU burst
            //logs execution
            log_cpuburst_execution(currProc->ID, currProc->cpuExecuted, currProc->IOBursts, totalTime, QUANTUM_EXPIRED);
            if (!Blocked.empty() && !sameTime) {
                //IF the block queuue is not empty and we aren't in same time execution
                int topBlocked = Blocked.top()->curBTime;
                //This is to tell which execution gets to the back of the queue since we are runing same time execution on blocked
                //we want the right process to get pushed to blocked in the right order based on time
                // we push then execute blocked or we push ready back and execute block
                if(topBlocked >= Qtime){
                    Ready.push(*currProc);
                    executeBlocked(Qtime,true);
                    //getting the front of queue off
                    if (!Ready.empty()) {
                        Ready.pop();
                    }
                    return 0;
                }
                executeBlocked(Qtime,true);
            }
            Ready.push(*currProc);
            execTime = Qtime;
        } else { //This branch is for sending a process to IO or Completing the process
            if(!sameTime) { //updating time if its not same time execution
                totalTime += currProc->curBTime;
            }
            execTime = currProc->curBTime;
            int timeToExec = currProc->curBTime;//execution time for current process
            currProc->executeProcess(timeToExec, 0);//execute the current process for the current burst time left
            int totalArraySize = currProc->sizeOfArrayBursts; // bursts array size in case we push current process to Block queue
            //This branch for when the process is completed
            if ((currProc->arrayNextBurst) >= totalArraySize) {
                currProc->turnTime = totalTime;//assigns turn time for current process
                //loging completion burst
                log_cpuburst_execution(currProc->ID, currProc->cpuExecuted, currProc->IOBursts, totalTime, COMPLETED);
                completionLog.push(*currProc);//pushing completed burst to queue for logging
                //This is to run blocke for same time execution
                if (!Blocked.empty() && !sameTime) {
                    executeBlocked(timeToExec, true);
                }
                Ready.pop();
                return execTime;
            } else { // This branch sends process to blocked queueu for its next IO burst
                //logs first
                log_cpuburst_execution(currProc->ID, currProc->cpuExecuted, currProc->IOBursts, totalTime, ENTER_IO);
                //Execution of io burst at the same time of ready block burst
                if (!Blocked.empty() && !sameTime) {
                    executeBlocked(timeToExec, true);
                }
                //Attribute setting for next burst for array of bursts and index of next burst
                currProc->curBTime = currProc->allArrayBursts[currProc->arrayNextBurst];
                currProc->arrayNextBurst = currProc->arrayNextBurst + 1;
                Blocked.push(currProc);
            }
        }
        if (!Ready.empty()) {
            Ready.pop();
        }
        return execTime;
    }
    return 0;
}
void schedule::executeBlocked(int Qtime, bool sameTime) {
    //this function executes the blocked queueue for Qtime and check if we are doing same time execution
    std::priority_queue <process*, std::vector<process*>, Compare> NewBlocked;
    //We se this NewBlocked queue in order to process all the processes in the queue at the sametime and
    //not have reordering issuses or execution of the same process
    int maxTimeEx = 0; //This is the max time that has been executed
    bool ReadyEmptyBef = Ready.empty();// Checks to see if Ready was empty before iteration
    if(!Blocked.empty()) {
        int topTime = Blocked.top()->curBTime;
        //if we aren't doing same time execution we only try to execute by the top of the blocked queue current burst
        if(!sameTime){
            Qtime = topTime;
        }
    while(!Blocked.empty())
    //For this while loop we are iterating over all the blocked queue in order to execute all for the same amount of time
        {
            process *currProc = Blocked.top();
            //If process times out we change our mas time acccordingly execute process and push to our new block queue
            if ((Qtime - (currProc->curBTime)) < 0) {
                maxTimeEx = std::max(Qtime,maxTimeEx);
                currProc->executeProcess(Qtime, 1);//procflag = 1 is IO burst so we execute
                NewBlocked.push(currProc); // We push to our New block
            } else {
                //If we don't time out for this we push to the ready queueu our priority keeps track of the ordering so
                //we don't have to worry about the timing of the pushes
                maxTimeEx = currProc->curBTime;
                currProc->executeProcess(currProc->curBTime, 1);
                //we execute for the current burst time which is the max burst time executed so far
                // we then set the attributes for the next io burst before sending to the Ready queue
                currProc->curBTime = currProc->allArrayBursts[currProc->arrayNextBurst];
                currProc->arrayNextBurst = (currProc->arrayNextBurst) + 1;
                Ready.push(*currProc);
            }
                Blocked.pop();//We pop from current block
        }
        //if We aren't executing from the ready queueu at the same time we update our total time to add the time we executed
        if (!sameTime) {
            totalTime += maxTimeEx;
        }
        //we swap our new block content and our old block that has nothing
        NewBlocked.swap(Blocked);
        return;
    }
    return;
}
void schedule::logFinalComp(){
    //This function logs all completion log process in the completionLog queue set up when a process was completed
    //The loop iterates through each process in the queueue and calculated the wait time accordingly to
    // Turnaround =  CPUBursts + IOBursts + WaitTime
    while(!completionLog.empty()){
        process currProc = completionLog.front();
        unsigned int waitTime = currProc.turnTime - currProc.IOBursts - currProc.cpuExecuted;
        log_process_completion(currProc.ID,currProc.turnTime,waitTime);
        completionLog.pop();
    }
}

