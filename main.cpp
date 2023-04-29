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


void openCalculator(){
    int shm_id = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // create shared memory segment

    if (shm_id < 0)
    { // check for errors
        perror("shmget");
        exit(1);
    }
    sem_t *sem = sem_open("/c1", O_CREAT | O_RDWR, 0666, 0);

    sem_t *sem1 = sem_open("/c2", O_CREAT | O_RDWR, 0666, 1);

    pid_t pid = fork(); // create a child process using fork()

    if (pid == 0)
    { // in the child process
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", shm_id); // convert shm_id to string

        execlp("/usr/bin/gnome-terminal", "/usr/bin/gnome-terminal", "--", "./calculator", shm_id_str, NULL); // launch a new terminal and run ./calculator with shm_id as an argument
        perror("execlp");                                                                                     // check for errors
        exit(1);
    }
    else if (pid < 0)
    { // check for errors
        perror("fork error");
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
        // cout << "hi";
        // cout << "ram" << shared_task->ram << endl;
        cout << "PID is " << shared_task->pid << endl;
        // cout << "Ram " << shared_task->ram << endl; 
cout<<"waiting 3"<<endl; 

        sem_post(sem1);

//semahpre wait
        // if( hardwareConfigs->canExecute(shared_task) ){

            Scheduling::readyQueue.push(shared_task);
            // sleep(1);
            // kill(shared_task->pid, SIGSTOP);
        // }
        // cout<<"Process was paused with pid: "<<shared_task->pid<<endl;
//semaphore up

         

        shmdt(shared_task);             // detach the shared memory segment
        shmctl(shm_id, IPC_RMID, NULL); // remove the shared memory segment
    }
}

void openNotepad(){
    int shm_id = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // create shared memory segment

    if (shm_id < 0)
    { // check for errors
        perror("shmget");
        exit(1);
    }
    sem_t *sem = sem_open("/n1", O_CREAT | O_RDWR, 0666, 0);

    sem_t *sem1 = sem_open("/n2", O_CREAT | O_RDWR, 0666, 1);

    pid_t pid = fork(); // create a child process using fork()

    if (pid == 0)
    { // in the child process
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", shm_id); // convert shm_id to string

        execlp("/usr/bin/gnome-terminal", "/usr/bin/gnome-terminal", "--", "./calculator", shm_id_str, NULL); // launch a new terminal and run ./calculator with shm_id as an argument
        perror("execlp");                                                                                     // check for errors
        exit(1);
    }
    else if (pid < 0)
    { // check for errors
        perror("fork error");
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
        // cout << "hi";
        // cout << "ram" << shared_task->ram << endl;
        cout << "PID is " << shared_task->pid << endl;
        // cout << "Ram " << shared_task->ram << endl; 
cout<<"waiting 3"<<endl; 

        sem_post(sem1);

//semahpre wait
        // if( hardwareConfigs->canExecute(shared_task) ){

            Scheduling::readyQueue.push(shared_task);
            // sleep(1);
            // kill(shared_task->pid, SIGSTOP);
        // }
//semaphore up

         

        shmdt(shared_task);             // detach the shared memory segment
        shmctl(shm_id, IPC_RMID, NULL); // remove the shared memory segment
    }
}

void openTime(){
    int shm_id = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // create shared memory segment

    if (shm_id < 0)
    { // check for errors
        perror("shmget");
        exit(1);
    }
    sem_t *sem = sem_open("/t1", O_CREAT | O_RDWR, 0666, 0);

    sem_t *sem1 = sem_open("/t2", O_CREAT | O_RDWR, 0666, 1);

    pid_t pid = fork(); // create a child process using fork()

    if (pid == 0)
    { // in the child process
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", shm_id); // convert shm_id to string

        execlp("/usr/bin/gnome-terminal", "/usr/bin/gnome-terminal", "--", "./calculator", shm_id_str, NULL); // launch a new terminal and run ./calculator with shm_id as an argument
        perror("execlp");                                                                                     // check for errors
        exit(1);
    }
    else if (pid < 0)
    { // check for errors
        perror("fork error");
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
        // cout << "hi";
        // cout << "ram" << shared_task->ram << endl;
        cout << "PID is " << shared_task->pid << endl;
        // cout << "Ram " << shared_task->ram << endl; 
cout<<"waiting 3"<<endl; 

        sem_post(sem1);

//semahpre wait
        // if( hardwareConfigs->canExecute(shared_task) ){

            Scheduling::readyQueue.push(shared_task);
            // sleep(1);
            // kill(shared_task->pid, SIGSTOP);
        // }
//semaphore up

         

        shmdt(shared_task);             // detach the shared memory segment
        shmctl(shm_id, IPC_RMID, NULL); // remove the shared memory segment
    }
}

void openCalender(){
    int shm_id = shmget(IPC_PRIVATE, sizeof(Task), IPC_CREAT | 0666); // create shared memory segment

    if (shm_id < 0)
    { // check for errors
        perror("shmget");
        exit(1);
    }
    sem_t *sem = sem_open("/cl1", O_CREAT | O_RDWR, 0666, 0);

    sem_t *sem1 = sem_open("/cl2", O_CREAT | O_RDWR, 0666, 1);

    pid_t pid = fork(); // create a child process using fork()

    if (pid == 0)
    { // in the child process
        char shm_id_str[20];
        sprintf(shm_id_str, "%d", shm_id); // convert shm_id to string

        execlp("/usr/bin/gnome-terminal", "/usr/bin/gnome-terminal", "--", "./calculator", shm_id_str, NULL); // launch a new terminal and run ./calculator with shm_id as an argument
        perror("execlp");                                                                                     // check for errors
        exit(1);
    }
    else if (pid < 0)
    { // check for errors
        perror("fork error");
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
        // cout << "hi";
        // cout << "ram" << shared_task->ram << endl;
        cout << "PID is " << shared_task->pid << endl;
        // cout << "Ram " << shared_task->ram << endl; 
cout<<"waiting 3"<<endl; 

        sem_post(sem1);

//semahpre wait
        // if( hardwareConfigs->canExecute(shared_task) ){

            Scheduling::readyQueue.push(shared_task);
            // sleep(1);
            // kill(shared_task->pid, SIGSTOP);
        // }
//semaphore up

         

        shmdt(shared_task);             // detach the shared memory segment
        shmctl(shm_id, IPC_RMID, NULL); // remove the shared memory segment
    }
}

void* initializeMidTermSchedular(void *){ 
    Scheduling::shortTermSchedular(); 
    int *n;
    return n;
}


int main(int n, char **argv)
{
    // my code starts
    const int noOfCores  = 1;
    const long int RAM = 100;
    const long int HDD = 500;

    SystemConfigs * hardwareConfigs  = new SystemConfigs(); 

    pthread_t t1;
    pthread_create(&t1, NULL, &initializeMidTermSchedular, NULL);

    

    Task * OS= new Task("Youngstr OS",RAM, HDD, noOfCores, getpid());  
    // os will be using the resources but not added added to the tasks queue
    // as it cannot be stopped 

    
    if(hardwareConfigs->getResources(OS) == false){
        cout<<"Could not start operating sytem ."<<endl;
        exit(0);
    }

    // Scheduling::readyQueue.push(OS);
    cout<<"main"<<endl;

    int choice = 0;
    do{

            // call the function open calculator(); 
            // fuunction will return pid name RAM core in main function through the shared memory  
            // main function will wait the binary semaphore and check add the task to ready or running queue(scheduling class) 
            // and then up the semaphore
            // 
        cout<<"Enter choice . : ";
        cin >> choice;
        if(choice == 2){  // shut down option selected
            break;
        }
        else if(choice == 1){openNotepad();}
        else if(choice == 3){openCalculator();}
        else if(choice == 4){openTime();}
        else if(choice == 5){openCalender();}
    }while(true);
    cout<<"operating system is shutting down"<<endl;
    // Scheduling::readyQueue.pop();

    return 0;
}


