
#include <string.h>
#include "schedule.h"
#include <unistd.h>
#include <string>

int main(int argc, char *arg[]) {
    int c;
    int  errflg; //flags for errors
    int quantumTime = 2; // default quantum time
    errflg =0;
    const char *strategy = "fcfs"; //default strategy
    const char *FileName = arg[1];// file name is in first argument assumed
    //This while loop using hte getopt function to get the arguments as shown by our string s:q:
    //s is for the schedule type and q is for the quantum type
    while ((c = getopt(argc, arg, "s:q:")) != -1) {
        switch (c) {
            case 's':
                strategy = optarg;
                break;
            case 'q':
                quantumTime = std::stoi(optarg);
                break;
            case '?':
                fprintf(stderr,
                        "Unrecognised option: -%c\n", optopt);
                errflg++;
                break;
        }
    }
    //Error checking for quantum time invalid quantum time being less than zero
   if(quantumTime<=0){
       printf("Time quantum must be a number and bigger than 0");
       exit(0);
   }
    const char *rr = "rr"; // setting a string to check if the strategy variable is round robing for choosing strategy
    schedule firstSch; //First schedule in case we have multiple schedule handles
    //Checks strategy for choosing between RR and FCFS;
    if(strcmp(strategy, rr) == 0){
    firstSch.scheduleRR(FileName,quantumTime);
    }
    else{
        firstSch.scheduleFCFS(FileName);
    }
    return 0;
}
