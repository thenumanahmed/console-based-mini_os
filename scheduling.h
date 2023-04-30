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
   static int noOfCores ;  //total cores -1 (1 core is usedfor os )
    static queue<Task *> readyQueue;
    static queue<Task *>runningQueue;
    static void shortTermSchedular();
};
queue<Task *> Scheduling::readyQueue;
queue<Task *>Scheduling::runningQueue ;
void Scheduling::shortTermSchedular()
{
 
    while (true)
    {
        // semaphore wait
        // cout<<"scheduling "<<endl;
        sleep(1);
        if(readyQueue.empty() && runningQueue.empty()){
            // both are empty no task

            // nothing to do
            cout<<"Both empty"<<endl;
        }
        else if(!readyQueue.empty() && runningQueue.size()<noOfCores){
            //accessing first task from ready queue
            Task * toRun = readyQueue.front();
            readyQueue.pop();
            
            // running the task
            runningQueue.push(toRun);
            kill(toRun->pid, SIGCONT);
            cout<<"Running task with pid : "<< toRun-> pid <<endl;

            
            cout<<"!ready but run<core"<<endl;
        }
        else if(!readyQueue.empty() && runningQueue.size()>=noOfCores){
            //accessing the task to pause
            cout<<"!ready but run         >         core"<<endl;

            Task * toPause = runningQueue.front();
            runningQueue.pop();

            //accessing the task to run
            Task * toRun = readyQueue.front();
            readyQueue.pop();

            if (toRun->pid != 9999 && kill(toRun->pid, SIGCONT) == 0)
            {
                    // Process is existing in memory
                    runningQueue.push(toRun);
                    cout<<"Running task with pid : "<< toRun-> pid <<endl;
            }
            else
            { // Process don't exist in memory (is killed)
                cout<<"Task killed with pid: "<<toRun->pid<<endl;
                toRun->resetTask(); 
            }

            if (toPause->pid != 9999 && kill(toPause->pid, SIGCONT) == 0)
            {
                    // Process is existing in memory
                    kill(toPause->pid, SIGSTOP);
                    cout<<"Paused task with pid : "<< toRun-> pid <<endl;

                    readyQueue.push(toPause);
            }
            else
            { // Process don't exist in memory (is killed)
                cout<<"Task killed with pid: "<<toPause->pid<<endl;
                toPause->resetTask();  
            }

        }
        // checking if the task is killed from the memory or not
        if(!runningQueue.empty()){
            // removing the task if it is killed
            if (runningQueue.front()->pid != 9999 && kill(runningQueue.front()->pid, SIGCONT) == 0)
            {
                    // Process is running in memory
            }
            else
            {
                // Process Not Continue || not exist
                cout<<"Task killed with pid: "<<runningQueue.front()->pid<<endl;
                runningQueue.front()->resetTask();
                runningQueue.pop();
            }
        }
        // if(!readyQueue.empty()){
        //     // removing the task if it is killed
        //     if (readyQueue.front()->pid != 9999 && kill(readyQueue.front()->pid, SIGSTOP) == 0)
        //     {
        //             // Process is running in memory
        //     }
        //     else
        //     {
        //         // Process Not Continue || not exist
        //         readyQueue.front()->resetTask();
        //         readyQueue.pop();
        //     }
        // }

//ERRRRRRRRRRRRRRRRRRRRRR:  handle when  central task is deleted
    }
//         if(runningQueue.empty()){

//         }else{

//         }

//         if (!readyQueue.empty())
//         {
//             cout << "Context Switching" << endl;
//             if(runningQueue.size() == 0){
//                 Task * newTask =  readyQueue.back();
//                 readyQueue.pop();
//                 runningQueue.push(newTask);
//                 int r = kill(runningQueue.front()->pid, SIGCONT);
//                     cout << "running null conitnue " << runningQueue.back()->pid << 
//                     " Result" << ((r == 0) ? "Succes" : "Process not Exist") << endl;
//             }
//             else if (runningQueue.size() < noOfCores)
//             {
//                 // running will pop only if the front process of running is closed
// //issue when centeral process is closed
//                 // no process running and process exist in ready queue
//                 Task * newTask =  readyQueue.back();
//                 readyQueue.pop();
//                 runningQueue.push(newTask);
//                 if (runningQueue.front()->pid == 9999)       
//                 {
//                     // task is closed
//                     cout << "Errrorr" << endl;
//                     runningQueue.pop();  //remove the closed task        
//                 }
//                 // else
//                 // {    
//                     //as we can only remove the first element of the queue
//                     // so only checking the front of the queue
//                     int r = kill(runningQueue.front()->pid, SIGCONT);
//                     cout << "running null conitnue " << runningQueue.back()->pid << " Result" << ((r == 0) ? "Succes" : "Process not Exist") << endl;
//                 // }
//             }
//             else
//             {

//                 Task *toPause = runningQueue.front();
//                 pid_t pid = toPause->pid;

//                 // get front of queue
//                 Task* toRun = readyQueue.front();
//                 runningQueue.push(toRun);
//                 cout<<"in running: "<<toRun->pid<<endl;
//                 readyQueue.pop();

//                 if (runningQueue.front()->pid != 9999 && kill(runningQueue.front()->pid, SIGCONT) == 0)
//                 {
//                     // Process Continue Succefully
//                 }
//                 else
//                 {
//                     // Process Not Continue || not exist
//                     runningQueue.front()->resetTask();
//                     runningQueue.pop();
//                 }

//                 if (toPause->pid != 9999 && kill(toPause->pid, SIGSTOP) == 0)
//                 {
//                     // Process succefully Stop
//                     readyQueue.push(toPause);
//                     cout<<"in Ready: "<<toPause->pid<<endl;
//                 }
//                 else
//                 {
//                     // Process Not Stop || not exist
//                     toPause->resetTask();
//                 }
//             }
//         }
//      }
}
int Scheduling::noOfCores = 1;  //total cores -1 (1 core is usedfor os )
