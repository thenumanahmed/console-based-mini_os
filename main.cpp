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
// #include "share_memory.h"
#include "config.h"
#include "task_object.h"

using namespace std;

void *initializeMidTermSchedular(void *);



class Driver{
    TaskObject *shared_tasks[15];
    bool initialize();
    void buttonClicked(int index);

    

public:
    Driver(){
        cout<<"in driver program"<<endl;
        // my code starts
    const int noOfCores = 1;
    const long int RAM = 100;
    const long int HDD = 500;
    initialize();

    SystemConfigs *hardwareConfigs = new SystemConfigs();

    pthread_t t1;
    pthread_create(&t1, NULL, &initializeMidTermSchedular, NULL);

    Task *OS = new Task("Youngstr OS", RAM, HDD, noOfCores, getpid());
    // os will be using the resources but not added added to the tasks queue
    // as it cannot be stopped

    if (hardwareConfigs->getResources(OS) == false)
    {
        cout << "Could not start operating sytem ." << endl;
        exit(0);
    }

    // Scheduling::readyQueue.push(OS);
    cout << "main" << endl;

    int choice = 0;
    do
    {
        cout << "Enter choice . : ";
        cin >> choice;
        if (choice == -1)
        { // shut down option selected
            break;
        }
        else if (choice >= 0 && choice < 4)
        {
            buttonClicked(choice);
        }

    } while (true);
    cout << "operating system is shutting down" << endl;
    // Scheduling::readyQueue.pop();
    }

    
};

int main(int n, char **argv)
{
    Driver d1;

    return 0;
}

void *initializeMidTermSchedular(void *)
{
    Scheduling::shortTermSchedular();
    int *n;
    return n;
}

bool Driver::initialize()
{
    shared_tasks[0] = new TaskObject("./calculator", "cal1", "cal2");
    shared_tasks[1] = new TaskObject("./notepad", "not1", "not2");
    shared_tasks[2] = new TaskObject("./time", "tim1", "time2");
    shared_tasks[3] = new TaskObject("./calender", "clr1", "clr2");
    // shared_tasks[0] = new TaskObject("./calculator", "cal1", "cal2");
    // shared_tasks[0] = new TaskObject("./calculator", "cal1", "cal2");
    // shared_tasks[0] = new TaskObject("./calculator", "cal1", "cal2");
    // shared_tasks[0] = new TaskObject("./calculator", "cal1", "cal2");
    // shared_tasks[0] = new TaskObject("./calculator", "cal1", "cal2");

    return 0;
}

void Driver::buttonClicked(int index)
{
    if (shared_tasks[index]->pid() == 9999)
    {
        // to open: Task not present
        shared_tasks[index]->openTask();
    }
    else if ((kill(shared_tasks[index]->pid(), 0) != 0))
    {
        // To Open: Task was not close properly
        shared_tasks[index]->task->resetTask();
        shared_tasks[index]->openTask();
    }
    else
    {
        // to Close: Task Exist
        shared_tasks[index]->closeTask();
    }
} 