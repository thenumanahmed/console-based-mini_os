#pragma once
#include <iostream>
#include <queue>
#include <semaphore.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "task.h"

using namespace std;

class Scheduling
{
public:
    static queue<Task *> readyQueue;
    // static queue<Task *> readySusQueue;
    static Task *running;
    // static queue<Task *> runningSusQueue;

    static void shortTermSchedular();

    // pop the the when exited through the terminal or through the
    // task manager
};
queue<Task *> Scheduling::readyQueue;
Task *Scheduling::running = nullptr;
void Scheduling::shortTermSchedular()
{

    while (true)
    {
        // semaphore wait
        sleep(1);

        if (!readyQueue.empty())
        {
            // cout << "Starting the swapping" << endl;
            if (running == nullptr)
            {
                running = readyQueue.back();
                int r = kill(running->pid, SIGCONT);
                cout << "running null conitnue " << running->pid << " Result" << ((r == 0) ? "Succes" : "Process not Exist") << endl; // to continue the process
                readyQueue.pop();
            }
            else
            {

                Task *paused = running;
                pid_t pid = paused->pid;

                running = readyQueue.front();
                readyQueue.pop();

                int pauseRes = kill(paused->pid, SIGSTOP);
                int runningRes = kill(running->pid, SIGCONT);
                cout << "Pasue Result of " << paused->pid << " Result" << ((pauseRes == 0) ? "Succes" : "Process not Exist") << endl;
                cout << "Running Result of " << running->pid << " Result" << ((runningRes == 0) ? "Succes" : "Process not Exist") << endl;

                readyQueue.push(paused);
            }
        }
        // cout << "Doing nothing" << endl;
        // semaphore up
    }
}