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
#include "share_memory.h"
#include "config.h"

using namespace std;

void *initializeMidTermSchedular(void *);

int main(int n, char **argv)
{
    // my code starts
    const int noOfCores = 1;
    const long int RAM = 100;
    const long int HDD = 500;
    cout << Shared::initialize();

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
            Shared::buttonClicked(choice);
        }

    } while (true);
    cout << "operating system is shutting down" << endl;
    // Scheduling::readyQueue.pop();

    return 0;
}

void *initializeMidTermSchedular(void *)
{
    Scheduling::shortTermSchedular();
    int *n;
    return n;
}
