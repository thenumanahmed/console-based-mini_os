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
#include "config.h"

using namespace std;

void openTask(const char *obj, const char *sem_str_1, const char *sem_str_2)
{
    int shm_id = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // create shared memory segment

    if (shm_id < 0)
    { // check for errors
        perror("shmget");
        exit(1);
    }
    sem_t *sem1 = sem_open(sem_str_1, O_CREAT | O_RDWR, 0666, 0);
    sem_t *sem2 = sem_open(sem_str_2, O_CREAT | O_RDWR, 0666, 1);

    pid_t pid = fork(); // create a child process using fork()
    cout << "HIIIII" << obj << endl;
    if (pid == 0)
    { // in the child process
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", shm_id);                                                                               // convert shm_id to string
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

        Task *shared_task = (Task *)shmat(shm_id, NULL, 0); // attach the shared memory segment

        if ((intptr_t)shared_task == -1)
        { // check for errors
            perror("shmat");
            exit(1);
        }
        cout << "hi";

        sem_wait(sem1);
        // cout << "hi";
        // cout << "ram" << shared_task->ram << endl;
        cout << "PID is " << shared_task->pid << endl;
        // cout << "Ram " << shared_task->ram << endl;
        cout << "waiting 3" << endl;

        sem_post(sem2);

        // semahpre wait
        //  if( hardwareConfigs->canExecute(shared_task) ){
        // cout << kill(shared_task->pid, SIGSTOP); // to pause the running process
        Scheduling::readyQueue.push(shared_task);
        // sleep(1);
        // kill(shared_task->pid, SIGSTOP);
        // }
        // cout<<"Process was paused with pid: "<<shared_task->pid<<endl;
        // semaphore up

        shmdt(shared_task);             // detach the shared memory segment
        shmctl(shm_id, IPC_RMID, NULL); // remove the shared memory segment
        cout << "Exiting CALCULATOR";
    }
    cout << "Exiting CALCULATOR";
}

void *initializeMidTermSchedular(void *)
{
    Scheduling::shortTermSchedular();
    int *n;
    return n;
}

int main(int n, char **argv)
{
    // my code starts
    const int noOfCores = 1;
    const long int RAM = 100;
    const long int HDD = 500;

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
        else if (choice == 1)
        {
            // s
            openTask("./calculator", "c1", "c2");
        }
        else if (choice == 2)
        {
            openTask("./notepad", "n1", "n2");
        }
        else if (choice == 3)
        {
            openTask("./time", "t1", "t2");
        }
        else if (choice == 4)
        {
            openTask("./calender", "cl1", "cl2");
        }
    } while (true);
    cout << "operating system is shutting down" << endl;
    // Scheduling::readyQueue.pop();

    return 0;
}
