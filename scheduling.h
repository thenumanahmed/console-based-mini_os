#pragma once
#include <iostream>
#include <vector>
#include <semaphore.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h> 
#include <sys/ipc.h>
#include <sys/shm.h>

#include "task.h"

using namespace std;

class Scheduling
{
public: 
   static int noOfCores ;  //total cores -1 (1 core is usedfor os )
    static vector<Task *> readyQueue;
    static vector<Task *>runningQueue;
    static void shortTermSchedular();
    
    // static vector<Task *>runningQueue;
    // static void longTermSchdeular();


};
vector<Task *> Scheduling::readyQueue;
// vector<Task *>Scheduling::runningQueue ;
vector<Task *>Scheduling::runningQueue ;
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
            readyQueue.erase(readyQueue.begin());
            
            // running the task
            runningQueue.push_back(toRun);
            kill(toRun->pid, SIGCONT);
            cout<<"Running task with pid : "<< toRun-> pid <<endl;

            
            cout<<"!ready but run<core"<<endl;
        }
        else if(!readyQueue.empty() && runningQueue.size()>=noOfCores){
            //accessing the task to pause
            cout<<"!ready but run         >         core"<<endl;

            Task * toPause = runningQueue.front();
            runningQueue.erase(runningQueue.begin());

            //accessing the task to run
            Task * toRun = readyQueue.front();
            readyQueue.erase(readyQueue.begin());

            if (toRun->pid != 9999 && kill(toRun->pid, SIGCONT) == 0)
            {
                    // Process is existing in memory
                    runningQueue.push_back(toRun);
                    cout<<"Running task with pid : "<< toRun-> pid <<endl;
            }
            else
            { // Process don't exist in memory (is killed)
                cout<<"Task killed. with pid: "<<toRun->pid<<endl;
                toRun->resetTask(); 
            }

            if (toPause->pid != 9999 && kill(toPause->pid, SIGCONT) == 0)
            {
                    // Process is existing in memory
                    kill(toPause->pid, SIGSTOP);
                    cout<<"Paused task with pid : "<< toRun-> pid <<endl;

                    readyQueue.push_back(toPause);
            }
            else
            { // Process don't exist in memory (is killed)
                cout<<"Task killed with pid: "<<toPause->pid<<endl;
                toPause->resetTask();  
            }

        }

        // check if any task in running is killed through terminal 
        if(!runningQueue.empty()){
            for(auto it = runningQueue.begin(); it!= runningQueue.end() ;it++){
                if( kill((*it)->pid, SIGCONT) != 0){
                    cout<<"Process with pid "<<(*it)->pid <<" was killed"<<endl;
                    (*it)->resetTask();
                    runningQueue.erase(it); // removing from running queue
                    break;
                }
            }
        }

        // checking if the task is killed from the memory or not
        // if(!runningQueue.empty()){
        //     // removing the task if it is killed
        //     if (runningQueue.front()->pid != 9999 && kill(runningQueue.front()->pid, SIGCONT) == 0)
        //     {
        //             // Process is running in memory
        //     }
        //     else
        //     {
        //         // Process Not Continue || not exist
        //         cout<<"Task killed with pid: "<<runningQueue.front()->pid<<endl;
        //         runningQueue.front()->resetTask();
        //         runningQueue.erase(runningQueue.begin());
        //     }
        // }
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

        

       


        // if(!readyQueue.empty()){
        //     for(auto it = readyQueue.begin(); it!= readyQueue.end() ;it++){
        //         if((*it)->pid != 9999 && kill((*it)->pid, SIGCONT) == 0){
        // // sigsend(P_PID, (*it)->pid, 0);
        //             cout<<"Process with pid "<<(*it)->pid <<" was killed"<<endl;
        //             (*it)->resetTask();
        //             readyQueue.erase(it); // removing from running queue
        //         }
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
int Scheduling::noOfCores = 3;  //total cores -1 (1 core is usedfor os )
