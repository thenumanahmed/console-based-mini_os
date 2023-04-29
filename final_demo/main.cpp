#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <semaphore.h>
#include "task.h"
#include <fcntl.h>

using namespace std;

void function(){
    int shm_id = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // create shared memory segment

    if (shm_id < 0)
    { // check for errors
        perror("shmget");
        exit(1);
    }
    sem_t *sem = sem_open("/calucaltor", O_CREAT | O_RDWR, 0666, 0);

    sem_t *sem1 = sem_open("/calucaltor1", O_CREAT | O_RDWR, 0666, 1);
    pid_t child_pid = fork(); // create a child process using fork()

    if (child_pid == 0)
    { // in the child process
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", shm_id); // convert shm_id to string

        execlp("/usr/bin/gnome-terminal", "/usr/bin/gnome-terminal", "--", "./calculator", shm_id_str, NULL); // launch a new terminal and run ./calculator with shm_id as an argument
        perror("execlp");                                                                                     // check for errors
        exit(1);
    }
    else if (child_pid < 0)
    { // check for errors
        perror("fork");
        exit(1);
    }
    else
    { // in the parent process
      // waitpid(child_pid, NULL, 0); // wait for the child process to exit

        Task *shared_task = (Task *)shmat(shm_id, NULL, 0); // attach the shared memory segment

        if ((intptr_t)shared_task == -1)
        { // check for errors
            perror("shmat");
            exit(1);
        }
        cout << "hi";

        sem_wait(sem);
        cout << "hi";
        cout << "ram" << shared_task->ram << endl;
        cout << "PID is " << shared_task->pid << endl;
        cout << "Ram " << shared_task->ram << endl;
        shared_task->allowed = true;

        sem_post(sem1);

       

        cout << "Child Process Killeddddd yahooooooooo" << endl;

        shmdt(shared_task);             // detach the shared memory segment
        shmctl(shm_id, IPC_RMID, NULL); // remove the shared memory segment
        cout<<"removed shared mem"<<endl;
    }

}

int main(int n, char **argv)
{
    function();
    function();
    return 0;
}
