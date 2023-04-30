#pragma once

#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include "task.h"
#include <semaphore.h>
#include <fcntl.h>

using namespace std;

class Task
{
public:
    char name[20];
    pid_t pid;
    long int ram;
    long int hard;
    int noOfcores;
    bool allowRun;
    // string sharedMemName;
public:
    Task()
    {
        cout << "Constructor" << endl;
        resetTask();
    }

    Task(const char *n, long int ram, long int hdd, int noOfcores, pid_t pid)
    {
        strcpy(this->name, n);
        this->ram = ram;
        this->hard = hard;
        this->pid = pid;
        this->noOfcores = noOfcores;
        allowRun = false;
    }

    void resetTask()
    {
        strcpy(this->name, "");
        pid = 9999;
        ram = 0;
        hard = 0;
        noOfcores = 0;
        allowRun = false;
    }

    static Task *sharePid(const char *shm_str, const char *s1, const char *s2, const char *n, const int r, const int h, const int c);
};

Task *Task::sharePid(const char *shm_str, const char *s1, const char *s2, const char *n, const int r, const int h, const int c)
{
    int shm_id = atoi(shm_str);                         // convert shm_id argument to integer
    Task *shared_task = (Task *)shmat(shm_id, NULL, 0); // attach the shared memory segment

    if ((intptr_t)shared_task == -1)
    { // check for errors
        perror("shmat");
        exit(1);
    }

    // signal(SIGKILL, sigterm_handler);
    sem_t *sem = sem_open(s1, O_CREAT | O_RDWR, 1);
    sem_t *sem1 = sem_open(s2, O_CREAT | O_RDWR, 0);

    memset(shared_task->name, '\0', sizeof(shared_task->name));
    strcpy(shared_task->name, n);

    shared_task->pid = getpid(); // write process ID to shared memory
    shared_task->ram = r;
    shared_task->hard = h;
    shared_task->noOfcores = c;
    sem_post(sem);
    cout << "Waiting for signal" << endl;
    sem_wait(sem1);

    return shared_task;
}

// is exitable ->> as main program is not exit able