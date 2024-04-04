# scheduling-RR-FCFS

Scheduling algorithm for a CPU burst and IO bursts implementing Round Robin and First Come First serve.
 Assuming a uniprocessor environment, a simulator for short-term process scheduling using a First
Come First Served (FCFS) strategy and a Round Robin (RR) strategy.

Functionality
Program first reads the process bursts from a file, then uses them to simulate the execution of the
processes based on the scheduling strategy specified from a command line argument and prints the
execution sequence of the process CPU bursts.

Scheduling algorithms
The scheduling algorithms you need to implement are:
First Come First Served (FCFS)
Round Robin (RR)

For simplicity, assume context switch time and other overheads are comparatively negligible, and
assume:
• Each process is accessing a separate I/O, when the process enters the next I/O burst, it will exe-
cute it immediately, this implies:
o During the CPU burst (execution) of a process, all blocked processes due to I/O would be
executing their respective current I/O burst at the same time.
