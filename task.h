#pragma once
#include <iostream>
#include <cstring>

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
        strcpy(this->name, "");
        this->ram = 0;
        this->hard = 0;
        this->pid = 0;
        this->noOfcores = 0;
        this->allowRun = false;
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
};

// is exitable ->> as main program is not exit able