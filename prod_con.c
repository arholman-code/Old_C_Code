#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "buffer.h"
#include <semaphore.h>

#define max_rand 1000;
#define TRUE 1;	//defines max number rand() can produce
int size = 0;			

char *in1;				//argv[1]
char *num_prod_threads; //argv[2]
char *num_con_threads;	//argv[3]

int sum;

buffer_item buffer[BUFFER_SIZE];	//create the buffer
pthread_mutex_t mutex; //creation of pthread mutex lock

sem_t sem_full;		   //semaphore declaration
sem_t sem_empty;

//function prototypes
void *Producer(void *);	
void *Consumer (void *);
int insert_item(buffer_item);
int remove_item(buffer_item *);
void Initialize();

int main(int argc, char *argv[])
{
	//read in command line arguments and set them
	//to their respective variables
	in1 = argv[1];			
	num_prod_threads = argv[2];
	num_con_threads = argv[3];
	
	int time_to_sleep = atoi(in1); //convert sleep time to an integer
	
	//initialize the buffer
	int i;				
	for(i = 0; i<5; i++)
		buffer[i] = 0;
	
	Initialize();	//function to initialize semaphores for empty/full and 
	                //to initialize the pthread mutex lock
	
	//create the producer and consumer threads
	pthread_t con_thread, prod_thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&prod_thread, &attr, Producer, num_prod_threads);
	pthread_create(&con_thread, &attr, Consumer, num_con_threads);
	
	//sleep a random amount of time
	sleep(time_to_sleep);
	
	//run through the buffer after main wakes up and display it to STDOUT
	int o;
	for (o=0; o<5; o++)
	{
		printf("Buffer[%i] = %i\n", o, buffer[o]);
	}
	exit(0);
}

void Initialize()
{ 
	pthread_mutex_init(&mutex, NULL);	//initialize mutex lock
	
	sem_init(&sem_full, 0, 0);			//semaphore for full
	sem_init(&sem_empty, 0, 5);			//semaphore for empty
	
	return;
}

void *Producer(void *p)
{		
	buffer_item rand_num;
	
	while (TRUE)
	{
		int sleep_time = rand()%3;		//sleep producer (1-3 seconds)
		sleep(sleep_time);	
		
		rand_num = rand() % max_rand;		//number produced to insert into the buffer
		printf("Producer produced %i\n", rand_num);
		if(insert_item(rand_num)){		//insert it into the buffer
			printf("Insert Problem\n");
		}
	}	
}

void *Consumer(void *p)
{		
	buffer_item rand_num;
	while(TRUE)
	{
		int sleep_time = rand() % 6;  //sleep consumer(1-6 seconds)
		sleep(sleep_time);			
		
		if(remove_item(&rand_num))	//remove the most recently inserted item
				                    //and check for success
		printf("Remove problem\n");	
		else
		printf("consumer consumed %i\n", rand_num);
	}
}

int insert_item(buffer_item item)
{
	while (TRUE)
	{
	sem_wait(&sem_empty);		
	pthread_mutex_lock(&mutex);  //set the pthread mutex lock
	
	buffer[size] = item;  	//insert the new item
	size++;					//increment the location in the buffer
	
	pthread_mutex_unlock(&mutex); //unlock the pthread mutex lock
	sem_post(&sem_full);		  //lock producer if buffer is now full
	return 0;	
	}
	return 1;
}

int remove_item(buffer_item *item)
{
	while(TRUE)
	{
	sem_wait(&sem_full);
	pthread_mutex_lock(&mutex);	//set the pthread mutex lock
		
	size--;			//decrement location in the buffer
		
	//remove item
	*item = buffer[size];	
	buffer[size] = 0;		
		
	pthread_mutex_unlock(&mutex);	//release the lock
	sem_post(&sem_empty);			//lock consumer if buffer is now empty
	return 0;	
	}
	return 1;
}
