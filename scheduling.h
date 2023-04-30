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
    static Task *running;
    static void shortTermSchedular();
};
queue<Task *> Scheduling::readyQueue;
Task *Scheduling::running = nullptr;
void Scheduling::shortTermSchedular()
{
    while (true)
    {
        // semaphore wait
        sleep(100);

        if (!readyQueue.empty())
        {
            cout << "Context Switching" << endl;
            if (running == nullptr)
            {
                // no process running and process exist in ready queue

                running = readyQueue.back();
                readyQueue.pop();
                if (running->pid == 9999)
                {
                    // task is closed
                    cout << "Errrorr" << endl;
                    running = nullptr;
                }
                else
                {
                    int r = kill(running->pid, SIGCONT);
                    cout << "running null conitnue " << running->pid << " Result" << ((r == 0) ? "Succes" : "Process not Exist") << endl;
                }
            }
            else
            {

                Task *toPause = running;
                pid_t pid = toPause->pid;

                // get front of queue
                running = readyQueue.front();
                readyQueue.pop();

                if (running->pid != 9999 && kill(running->pid, SIGCONT) == 0)
                {
                    // Process Continue Succefully
                }
                else
                {
                    // Process Not Continue || not exist
                    running->resetTask();
                    running = nullptr;
                }

                if (toPause->pid != 9999 && kill(toPause->pid, SIGSTOP) == 0)
                {
                    // Process succefully Stop
                    readyQueue.push(toPause);
                }
                else
                {
                    // Process Not Stop || not exist
                    toPause->resetTask();
                }
            }
        }
    }
}