#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void Print_num(int x){
        int p;
	p = fork();

        if(p == 0)
        {
                int j;
                for(j=0; j <= x; j++){
			if(j%2 == 1){
                        printf("%d\n", j);
                        sleep(1);}
                }
                exit(0);
        }
        else if(p > 0)
        {
                int i;
                for(i=0; i <= x; i++){
			if(i%2 == 0){
                        printf("%d\n", i);
                        sleep(1);}
                }
        }
        else{//added error checking
                fprintf(stderr, "Failed to fork.");
                exit(1);
        }


/*

	if (p == 0){ 
		for(i = 0; i <= x; i++){ 
			if(i%2==1){ 
				printf("%d ", i); 		
			} 
		} 
		printf("\nChild complete "); 
		printf("with a process ID of: "); 
		printf("%d\n", getpid()); 
	}
	if(p != 0) { 
		for (i = 0; i <= x; i++){ 
			if(i%2==0){ 
				printf("%d ", i);
			} 
		} 
		printf("\nThe Parent process ID is: "); 
		printf("%d\n", getpid()); 
		 

	} */
}

int main()
{
	int maxnum;
	printf("Please enter a number: ");
	scanf("%d", &maxnum);
	
	Print_num(maxnum);
	

	return 0;
}
