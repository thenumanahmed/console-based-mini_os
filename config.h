#pragma once
// #include<iostream>
#include"task.h"

using namespace std;

class SystemConfigs{
    long int RAM;
    int noOfCores;
    long int HDD;
    long int usedHDD;
    long int usedRAM;
    int usedCores;

    // methods
    void getConfigs(){
        cout<<"Enter the RAM(in GB): ";
        while(RAM<1){
            cout<<"Invalid no of cores, please re enter no of RAM: ";
            cin>> RAM;
        }
        RAM*= 1024; // converted to bytes

        cout<<"Enter the HDD(in GB): ";
        while(HDD<1){
            cout<<"Invalid no of cores, please re enter no of HDD: ";
            cin>> HDD;
        }
        HDD*=1024;

        cout<<"Enter the no of cores: ";
        cin>>noOfCores; 
        while(noOfCores<2){
            cout<<"Invalid no of cores, please re enter no of cores: ";
            cin>> noOfCores;
        }
    }
    public:
    //constructor
    SystemConfigs(){
        usedHDD = usedRAM = usedCores= RAM = HDD = noOfCores = 1000;
        // cin.ignore();
        // getConfigs();
    }

    //functions
    long int getusedRAM(){return usedRAM;}
    int getusedCores(){return usedCores;}
    long int getusedHDD(){return usedHDD;}
    
    bool getResources(Task *task){          // to check before adding in ready queue
        if( task->ram>RAM  
            || task->noOfcores > noOfCores 
            ||  task->hard >HDD){
                return false;
        }
        usedCores+=task->noOfcores;
        usedRAM+=task->ram; 
        usedHDD+=task->hard;
        return true;
    }

    bool canExecute(Task* task){      // to check before reading to running queue
        if( usedRAM+task->ram>RAM 
        || usedCores+task->noOfcores > noOfCores 
        || usedHDD+task->hard >HDD ){ 
            return true;
        }
        return false;
    }
    void freeResources(Task* task){  
        usedCores-=task->noOfcores;
        usedRAM-=task->ram;
        usedHDD-=task->hard;
    }
}; 