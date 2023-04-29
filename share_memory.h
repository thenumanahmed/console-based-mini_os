#pragma once

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

using namespace std;

class Shared
{
public:
    static Task *shared_tasks[15];
    static int shared_id[15];
    static bool initialize();
};

Task *Shared::shared_tasks[15];
int Shared::shared_id[15];

bool Shared::initialize()
{
    const int num_shared_segments = 4; // Change this to create more or less shared memory segments.

    // Creating shared memory
    for (int i = 0; i < num_shared_segments; i++)
    {
        shared_id[i] = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // Create shared memory segment
        if (shared_id[i] < 0)
            exit(1);

        // Getting shared memory address
        shared_tasks[i] = reinterpret_cast<Task *>(shmat(shared_id[i], nullptr, 0)); // Attach the shared memory segment
        if (shared_tasks[i] == reinterpret_cast<Task *>(-1))
            exit(1);
    }

    return 0;
}
