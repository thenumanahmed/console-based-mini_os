#pragma once
#include<iostream>
#include<queue>
#include <semaphore.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include"task.h"

using namespace std;

class Scheduling{
public:
    static queue<Task *> readyQueue;
    // static queue<Task *> readySusQueue;
    static Task * running ;
    // static queue<Task *> runningSusQueue;


    static void shortTermSchedular();


    // pop the the when exited through the terminal or through the 
    // task manager

};
queue<Task*> Scheduling::readyQueue;
Task* Scheduling::running = nullptr;
void Scheduling::shortTermSchedular(){

    while(true){
// semaphore wait
sleep(1);
        // if(!readyQueue.empty()){
        //     cout<<"Starting the swapping"<<endl;
        //     if(running == nullptr){
        //         cout<<"Running was null"<<endl;
        //         cout<<"before  raedy";
        //         running = readyQueue.back();
        //         cout<<"after raedy";
        //         int r = kill(running->pid,SIGCONT);     // to continue the process
        //         cout<<"Process is running with pid :::: ";
        //         cout<<running->pid<<endl;
        //         cout<<r<<endl;
        //         readyQueue.pop();
        //     }
        //     else
        //     {
        //         cout<<"running was not null"<<endl;
                
        //         Task * paused = running ;
        //         pid_t pid = paused->pid ;

        //         running = readyQueue.front();
        //         readyQueue.pop(); 

        //         cout<<"Process with pid was pasued: "<< paused->pid<<endl;
        //         cout<<"Process with pid was added in running: "<< running->pid<<endl;
        //         kill( paused->pid,SIGSTOP);     // to pause the running process
        //         kill(running->pid,SIGCONT);     // to continue the process

        //         readyQueue.push(paused);
        //     } 
            
        // }
        cout<<"Doing nothing"<<endl;
// semaphore up
    }
}