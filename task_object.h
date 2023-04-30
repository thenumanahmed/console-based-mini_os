#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "task.h"
#include "scheduling.h"

class TaskObject
{
public:
    int shm_id;
    Task *task;
    string command;
    sem_t *sem1;
    sem_t *sem2;
    string sem1_str;
    string sem2_str;

    TaskObject(string command, string s1, string s2);
    bool openTask();  // to open task
    bool closeTask(); // to close task
    bool isActive();  // to check process is running or not
    bool isClose();   // to check pid is 0 or not

    pid_t pid() { return task->pid; }
};

TaskObject::TaskObject(string c, string s1, string s2)
{
    command = c;
    shm_id = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // Create shared memory segment
    if (shm_id < 0)
    {
        cout << "Shared Memory creating error" << endl;
        exit(1);
    }

    task = reinterpret_cast<Task *>(shmat(shm_id, nullptr, 0)); // Attach the shared memory segment
    if (task == reinterpret_cast<Task *>(-1))
    {
        cout << "Shared Memory Address assigin error" << endl;
        exit(1);
    }

    sem1_str = s1;
    sem2_str = s2;
    sem1 = sem_open(sem1_str.data(), O_CREAT | O_RDWR, 0666, 0);
    if (sem1 == SEM_FAILED)
    {
        cout << "Semaphore 1 creating failed" << endl;
        exit(EXIT_FAILURE);
    }

    sem2 = sem_open(sem2_str.data(), O_CREAT | O_RDWR, 0666, 1);
    if (sem2 == SEM_FAILED)
    {
        cout << "Semaphore 2 creating failed" << endl;
        exit(EXIT_FAILURE);
    }

    task->resetTask();
}

bool TaskObject::openTask()
{
    pid_t pid = fork(); // create a child process using fork()
    if (pid == 0)
    {
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", shm_id);
        cout << command << endl;
        cout << sem1_str << endl;
        cout << sem2_str << endl;
        const char *c = command.c_str();
        const char *s1 = sem1_str.c_str();
        const char *s2 = sem2_str.c_str();
        // execl("./calculator", "calculator", NULL); // c
        // execlp(c, c, shm_id_str, s1, s2, nullptr);
        execlp("/usr/bin/gnome-terminal", "/usr/bin/gnome-terminal", "--", c, shm_id_str, s1, s2, NULL);

        perror("execlp"); // check for errors
        exit(1);
    }
    else if (pid < 0)
    { // check for errors
        perror("fork error");
        exit(1);
    }
    else
    { // in the parent process
        sem_wait(sem1);

        // TODO:  check resources thaan allow it or not
        sem_post(sem2);

        // process is allowed
        Scheduling::readyQueue.push(task);
        return 0;
    }
    return -1;
}

bool TaskObject::closeTask()
{
    if (task->pid == 9999)
        return -1;

    cout << "Kill Start" << endl;
    cout << "Task->pid " << task->pid << "   Going to exit" << endl;
    kill(task->pid, SIGKILL);
    task->pid = 9999;
    task->ram = 0;
    task->hard = 0;
    task->noOfcores = 0;
    memset(task->name, '\0', sizeof(task->name));
    strcpy(task->name, "");
    cout << "Kill End" << endl;

    return 0;
};

bool TaskObject::isActive()
{
    if (task->pid == 9999)
    {
        cout << "Process PID 0 " << endl;
        return 0;
    }
    const int result = kill(task->pid, 0);

    if (result == 0)
    {
        cout << "Process is running" << endl;
        return 1;
    }
    else
    {
        cout << "Process not running" << endl;
        return 0;
    }
}

bool TaskObject::isClose()
{
    return task->pid == 9999;
}