#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SEQUENCE 10

/*struct for shared data segment, per page 125*/
typedef struct{
long fib_sequence[MAX_SEQUENCE];
int sequence_size;
}shared_data;

int main()
{
/*fib number input by user*/
int fib;
/*identifier for the shared memory segment*/
int segment_id;
/*pointer to the shared segment*/
shared_data *shared_mem;
/*process id*/
pid_t pid;

/*read input from command line*/	
printf("Please input a number for the fibonacci sequence: ");	
scanf("%d", &fib);

/*bounds check*/
if(fib > MAX_SEQUENCE){
	printf("Max Input Size: %d\n", MAX_SEQUENCE);
	exit(1);
}

/*allocate shared memory segment*/
segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), S_IRUSR | S_IWUSR);
	
/*attach shared memory segment*/
shared_mem = (shared_data*)shmat(segment_id, NULL, 0);

/*size of sequence to arg input on command line*/
shared_mem -> sequence_size = fib;
	
/*fork a child process*/
pid = fork();

/*check for successful fork*/
if(pid < 0){
	fprintf(stderr, "Fork failed\n");
	return 1;
}
/*if equal to zero, child process successful*/
else if(pid == 0){
	int counter = 0;

/*fibonacci sequence formula is = fib(n-1)+fib(n-2), always starting with zero and one*/
	while(counter < shared_mem -> sequence_size){
		if(counter == 0){
			shared_mem -> fib_sequence[counter] = 0;
		}
		else if(counter == 1){
			shared_mem -> fib_sequence[counter] = 1;
		}
		else{
			shared_mem -> fib_sequence[counter] = shared_mem -> fib_sequence[counter-1]+shared_mem -> fib_sequence[counter-2];
		}	
	counter++;
	}
}
/*into the parent process*/
else{
	/*wait for child to complete*/
	wait(NULL);
	int count = 0;
	/*print the fibonacci sequence to screen*/
	while(count < shared_mem -> sequence_size){
		printf("%li ", shared_mem -> fib_sequence[count]);
		count++;
	}
	printf("\n\n");

	/*now detach shared memory segment*/
	shmdt(shared_mem);
	
	/*now remove shared memory segment*/
	shmctl(segment_id, IPC_RMID, NULL);
	}

	return 0;
}
