#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include "task.h"

using namespace std;

int main(int n, char **argv)
{
    int shm_id = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // create shared memory segment

    if (shm_id < 0)
    { // check for errors
        perror("shmget");
        exit(1);
    }

    pid_t child_pid = fork(); // create a child process using fork()

    if (child_pid == 999)
    { // in the child process
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", shm_id); // convert shm_id to string

        execlp("/usr/bin/gnome-terminal", "/usr/bin/gnome-terminal", "--", "./calculator", shm_id_str, NULL); // launch a new terminal and run ./calculator with shm_id as an argument

        perror("execlp"); // check for errors
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
        sleep(3);

        Task *shared_task = (Task *)shmat(shm_id, NULL, 0); // attach the shared memory segment

        if ((intptr_t)shared_task == -1)
        { // check for errors
            perror("shmat");
            exit(1);
        }
        cout << "ram" << shared_task->RAM << endl;
        cout << "PID is " << shared_task->pid << endl;
        cout << "Ram " << shared_task->RAM << endl;
        // shared_task->allowed = true;
        sleep(1);

        cout << "Enter key to quit Calculator" << endl;
        cin.get();
        // kill child process
        int r = kill(shared_task->pid, SIGKILL);
        if (r == -1)
        {
            std::cout << "Error killing child process" << std::endl;
            ;
            return 1;
        }
        cout << r << endl;

        cout << "Child Process Killeddddd yahooooooooo" << endl;

        shmdt(shared_task);             // detach the shared memory segment
        shmctl(shm_id, IPC_RMID, NULL); // remove the shared memory segment
    }

    return 0;
}
