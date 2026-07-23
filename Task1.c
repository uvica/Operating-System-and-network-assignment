/*
 * Task 1 - Process Management and Threading
 * Operating Systems Coursework
 * Author: Uvica Shrestha
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

#define NUM_THREADS 3
#define NUM_ITERATIONS 100000
#define QUANTUM 2

// Shared resource
int counter = 0;

// Mutex
pthread_mutex_t mutex;

// Semaphore
sem_t semaphore;

// thread without mutex
void* raceCondition(void *arg)
{
    int id = *(int*)arg;

    printf("Thread %d started.\n", id);

    for(int i=0;i<NUM_ITERATIONS;i++)
    {
        counter++;
    }

    printf("Thread %d finished.\n", id);

    return NULL;
}

// thread with mutex
void* mutexFunction(void *arg)
{
    int id = *(int*)arg;

    printf("Thread %d started.\n", id);

    for(int i=0;i<NUM_ITERATIONS;i++)
    {
        pthread_mutex_lock(&mutex);

        counter++;

        pthread_mutex_unlock(&mutex);
    }

    printf("Thread %d finished.\n", id);

    return NULL;
}

// semaphore demonstration
void* semaphoreFunction(void *arg)
{
    int id = *(int*)arg;

    sem_wait(&semaphore);

    printf("Thread %d entered shared resource.\n", id);

    sleep(2);

    printf("Thread %d leaving shared resource.\n", id);

    sem_post(&semaphore);

    return NULL;
}

// round robin process structure
struct Process
{
    int id;
    int burst;
};

// round robin scheduler
void roundRobin()
{
    struct Process processes[3] = {
        {1, 5},
        {2, 7},
        {3, 4}
    };

    int completed = 0;

    printf("\n ROUND ROBIN SCHEDULER \n");

    while(completed < 3)
    {
        completed = 0;

        for(int i = 0; i < 3; i++)
        {
            if(processes[i].burst > 0)
            {
                printf("Running Process P%d\n", processes[i].id);

                if(processes[i].burst > QUANTUM)
                {
                    processes[i].burst -= QUANTUM;
                    printf("Remaining Burst Time : %d\n\n", processes[i].burst);
                }
                else
                {
                    printf("Process P%d Completed\n\n", processes[i].id);
                    processes[i].burst = 0;
                }
            }

            if(processes[i].burst == 0)
            {
                completed++;
            }
        }
    }
}

// deadlock preventation

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void* deadlockFunction(void *arg)
{
    int id = *(int*)arg;

    printf("Thread %d waiting for Lock 1\n", id);

    pthread_mutex_lock(&lock1);

    printf("Thread %d acquired Lock 1\n", id);

    sleep(1);

    printf("Thread %d waiting for Lock 2\n", id);

    pthread_mutex_lock(&lock2);

    printf("Thread %d acquired Lock 2\n", id);

    printf("Thread %d executing critical section\n", id);

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);

    printf("Thread %d released both locks\n\n", id);

    return NULL;
}

// main function
int main()
{
    pid_t pid;
    pthread_t threads[NUM_THREADS];
    int threadID[NUM_THREADS];

    printf("PROCESS CREATION \n");

    fflush(stdout);

    pid = fork();

    if(pid < 0)
    {
        printf("Process creation failed!\n");
        return 1;
    }

    else if(pid == 0)
    {
        printf("Child Process Created\n");
        printf("Child PID  : %d\n", getpid());
        printf("Parent PID : %d\n\n", getppid());

        exit(0);
    }

    else
    {
        wait(NULL);

        printf("Parent Process\n");
        printf("Parent PID : %d\n", getpid());
        printf("Child PID  : %d\n\n", pid);
    }

// thread creation and raace conditon

    printf("Thread creation \n");

    counter = 0;

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threadID[i] = i + 1;
        pthread_create(&threads[i], NULL, raceCondition, &threadID[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nRACE CONDITION \n");
    printf("Expected Counter Value : %d\n", NUM_THREADS * NUM_ITERATIONS);
    printf("Actual Counter Value   : %d\n\n", counter);

//   mutex synchronization

    printf("MUTEX SYNCHRONIZATION \n");

    counter = 0;

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threadID[i] = i + 1;
        pthread_create(&threads[i], NULL, mutexFunction, &threadID[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Expected Counter Value : %d\n", NUM_THREADS * NUM_ITERATIONS);
    printf("Actual Counter Value   : %d\n\n", counter);

//    semaphore

    printf("SEMAPHORE \n");

    sem_init(&semaphore, 0, 2);

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threadID[i] = i + 1;
        pthread_create(&threads[i], NULL, semaphoreFunction, &threadID[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);

    // round robin

    roundRobin();

    // deadlock preventation

    printf("\n DEADLOCK PREVENTION \n");

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    for(int i = 0; i < 2; i++)
    {
        threadID[i] = i + 1;
        pthread_create(&threads[i], NULL, deadlockFunction, &threadID[i]);
    }

    for(int i = 0; i < 2; i++)
    {
   

     pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    printf("\nProgram completed successfully.\n");

    return 0;
}