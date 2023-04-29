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

void openTask(const char *obj, const int index, const char *sem_str_1, const char *sem_str_2);
void closeTask(const int index);

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
        else if (choice == 0)
        {
            if (Shared::shared_tasks[0]->pid == 0)
            {
                cout << "PID IS 0" << endl;
                openTask("./calculator", 0, "c1", "c2");
            }
            else
            {
                cout << "PID IS Not 0" << endl;
                closeTask(0);
                // Send the SIGUSR1 signal to the process to minimize it
            }
        }
        else if (choice == 1)
        {
            if (Shared::shared_tasks[1]->pid == 0)
            {
                cout << "PID IS 0" << endl;
                openTask("./notepad", 1, "n1", "n2");
            }
            else
            {
                cout << "PID IS Not 0" << endl;
                closeTask(1);
                // Send the SIGUSR1 signal to the process to minimize it
            }
        }
        else if (choice == 2)
        {
            if (Shared::shared_tasks[2]->pid == 0)
            {
                cout << "PID IS 0" << endl;
                openTask("./time", 2, "t1", "t2");
            }
            else
            {
                cout << "PID IS Not 0" << endl;
                closeTask(2);
                // Send the SIGUSR1 signal to the process to minimize it
            }
        }
        else if (choice == 3)
        {
            if (Shared::shared_tasks[3]->pid == 0)
            {
                cout << "PID IS 0" << endl;
                openTask("./calender", 3, "cl1", "cl2");
            }
            else
            {
                cout << "PID IS Not 0" << endl;
                closeTask(3);
                // Send the SIGUSR1 signal to the process to minimize it
            }
        }
    } while (true);
    cout << "operating system is shutting down" << endl;
    // Scheduling::readyQueue.pop();

    return 0;
}

void openTask(const char *obj, const int index, const char *sem_str_1, const char *sem_str_2)
{
    sem_t *sem1 = sem_open(sem_str_1, O_CREAT | O_RDWR, 0666, 0);
    sem_t *sem2 = sem_open(sem_str_2, O_CREAT | O_RDWR, 0666, 1);

    pid_t pid = fork(); // create a child process using fork()
    if (pid == 0)
    {
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", Shared::shared_id[index]);                                                             // c                                                                                              // in the child process
        execlp("/usr/bin/gnome-terminal", "/usr/bin/gnome-terminal", "--", obj, shm_id_str, sem_str_1, sem_str_2, NULL); // launch a new terminal and run ./calculator with shm_id as an argument
        perror("execlp");                                                                                                // check for errors
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
        // check ram
        // shared_task->isAllowed = SystemConfigs().canExecute(shared_task);
        // shared_task->allowRun = true;
        // if (shared_task->isAllowed)
        // {
        //     shmdt(shared_task); // detach the shared memory segment
        //     shmctl(shm_id, IPC_RMID, NULL);
        //     return;
        // }
        sem_post(sem2);
        Scheduling::readyQueue.push(Shared::shared_tasks[index]);
    }
}

void *initializeMidTermSchedular(void *)
{
    Scheduling::shortTermSchedular();
    int *n;
    return n;
}

void closeTask(const int index)
{
    cout << "kill" << endl;

    kill(Shared::shared_tasks[index]->pid, SIGKILL);

    Shared::shared_tasks[index]
        ->pid = 0;
    Shared::shared_tasks[index]->ram = 0;
    memset(Shared::shared_tasks[index]->name, '\0', sizeof(Shared::shared_tasks[index]->name));
    strcpy(Shared::shared_tasks[index]->name, "");
    Shared::shared_tasks[index]->hard = 0;
    Shared::shared_tasks[index]->noOfcores = 0;
    cout << "hill" << endl;
}