#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <buffer.h>

#define CONSUMER_SLEEP_SEC 3
#define PRODUCER_SLEEP_SEC 1
#define KEY 1010

//A structure to store BUFER and semaphores for synchronization
typedef struct
{
    int buff[BUFFER_SIZE];
    sem_t mutex, empty, full;

} MEM;

//Method for shared memory allocation
MEM *memory()
{
    key_t key = KEY;
    int shmid;
    shmid = shmget(key, sizeof(MEM), IPC_CREAT | 0666);
    return (MEM *) shmat(shmid, NULL, 0);
}

void init()
{
    //Initialize structure pointer with shared memory
    MEM *M = memory();

    //Initialize semaphores
    sem_init(&M->mutex,1,1);
    sem_init(&M->empty,1,BUFFER_SIZE);
    sem_init(&M->full,1,0);
}
