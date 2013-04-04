//Andrew R Holman, CSC 562
//Assignment 4
//Requirements are labeled in the comments A-C

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

#define TRUE 1

//declare the mutex thread and the semaphores
pthread_mutex_t mutex;
sem_t full, empty;

//declare the buffer based on the header file
buffer_item buffer[BUFFER_SIZE];

//the counter to keep control of the buffer size and the sleep times input
int producer_sleep_time_max, consumer_sleep_time_max;//PART C*****************
int counter;

//the pthread id and attri declared
pthread_t tid;
pthread_attr_t attr;

//producer/consumer function declared
void *producer(void *param);
void *consumer(void *param);

//this function initializes the semaphores, counter and threads
//the empty semaphore is based upon the buffer size initially
void initialize(){
	pthread_mutex_init(&mutex, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	pthread_attr_init(&attr);
	counter = 0;
}

//the producer function.
void *producer(void *param){
//creates a buffer item
	buffer_item rand_num;	
	while(TRUE){
		//random sleep time based upon user input
		int rNum = rand() % producer_sleep_time_max;
		sleep(rNum);
		//the random number to input between 0 and 1000 PART B***********
		rand_num = rand() % 1001;
		sem_wait(&empty);
		//producer takes the lock
		pthread_mutex_lock(&mutex);
		//if the insert_item fails, it sends an error message
		if(insert_item(rand_num)){
			fprintf(stderr, "Producer error\n");
		}
		//otherwise, it outputs the number that was added
		else{
			//PART A***************************
			printf("Producer added %d to the buffer\n", rand_num);
		}
		//producer releases the lock
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

//consumer function
void *consumer(void *param){
	buffer_item rand_num;
	while(TRUE){
		//random sleep time based upon user input
		int rNum = rand() % consumer_sleep_time_max;
		sleep(rNum);
		sem_wait(&full);
		//takes lock
		pthread_mutex_lock(&mutex);
		//removes number from buffer
		if(remove_item(&rand_num)){
			fprintf(stderr, "Consumer error\n");
		}
		else{//PART A*******************************
			printf("Consumer consumed %d from the buffer\n", rand_num);
		}
		//releases lock
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}

int insert_item(buffer_item item){
	if(counter < BUFFER_SIZE){
		buffer[counter] = item;
		counter++;
		return 0;
	}
	else{
		return -1;
	}
}

int remove_item(buffer_item *item){
	if(counter > 0){
		*item = buffer[(counter-1)];
		counter--;
		return 0;
	}
	else{
		return -1;
	}
}

int main(int argc, char *argv[]){
	int i;
	if(argc != 6){
		fprintf(stderr, "Not enough arguments!");
	}
	//The above checks for (n-1) arguments from the command line, so 5 total

	producer_sleep_time_max = atoi(argv[1]);//arg1 is producer sleep
	consumer_sleep_time_max = atoi(argv[2]);//arg2 is consumer sleep
	int sleepTime = atoi(argv[3]);//arg3 is the program sleep time
	int numProd = atoi(argv[4]);//number of producers
	int numCons = atoi(argv[5]);//number of consumers

	initialize();//calls the initialize function
	
	//creates the producer threads
	for(i = 0; i < numProd; i++){
		pthread_create(&tid, &attr, producer, NULL);
	}

	//creates the consumer threads
	for(i = 0; i < numCons; i++){
		pthread_create(&tid, &attr, consumer, NULL);
	}

	//program sleeps while the producer produces and consumer consumes
	sleep(sleepTime);

	//outputs the end of the program
	printf("Exiting...\n");
	exit(0);
}
		
