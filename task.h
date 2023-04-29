#pragma once
#include<iostream> 

using namespace std;

class Task{ 
public:
    string name;
    pid_t pid;
    long int ram;
    long int hard;
    int noOfcores;
    // string sharedMemName;
public:
    Task(string name, long int ram, long int hdd,  int noOfcores, pid_t pid){
        this->name = name;
        this->ram = ram;
        this->hard = hard;
        this->pid = pid;
        noOfcores = noOfcores;
    }
};

// is exitable ->> as main program is not exit able 