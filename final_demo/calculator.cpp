#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include "task.h"
#include <semaphore.h>
#include <fcntl.h>
using namespace std;

int main(int argc, char *argv[])
{
    const int ram = 100;
    const int hardisk = 1000;
    const int core = 1;

    if (argc != 2)
    { // check for correct number of arguments
        fprintf(stderr, "Usage: %s <shm_id>\n", argv[0]);
        exit(1);
    }

    int shm_id = atoi(argv[1]);                         // convert shm_id argument to integer
    Task *shared_task = (Task *)shmat(shm_id, NULL, 0); // attach the shared memory segment

    if ((intptr_t)shared_task == -1)
    { // check for errors
        perror("shmat");
        exit(1);
    }

    // signal(SIGKILL, sigterm);
    sem_t *sem = sem_open("/calucaltor", O_CREAT | O_RDWR, 1);
    sem_t *sem1 = sem_open("/calucaltor1", O_CREAT | O_RDWR, 0);

    // sem_wait(sem);
    shared_task->pid = getpid(); // write process ID to shared memory
    shared_task->ram = 100;
    shared_task->hardisk = hardisk;
    shared_task->core = core;
    sem_post(sem);
    cout << "Waiting for signal" << endl;
    sem_wait(sem1);

    cout << shared_task->allowed << endl;
    printf("My process ID is %d and it has been written to shared memory\n", getpid());
    cout << shared_task->allowed << endl;

    if (shared_task->allowed != true)
        exit(0);

    shmdt(shared_task); // detach shared memory segment

    // Calulator Code

    while (true)
    {

        char op;
        double num1, num2, result;

        std::cout << "Enter operator (+, -, *, /): ";
        std::cin >> op;

        std::cout << "Enter first number: ";
        std::cin >> num1;

        std::cout << "Enter second number: ";
        std::cin >> num2;

        switch (op)
        {
        case '+':
            result = num1 + num2;
            break;

        case '-':
            result = num1 - num2;
            break;

        case '*':
            result = num1 * num2;
            break;

        case '/':
            if (num2 == 0)
            {
                std::cout << "Error: division by zero\n";
                return 1;
            }
            result = num1 / num2;
            break;

        default:
            std::cout << "Error: invalid operator\n";
            return 1;
        }

        std::cout << "Result: " << result << "\n";
    };
    return 0;
}
