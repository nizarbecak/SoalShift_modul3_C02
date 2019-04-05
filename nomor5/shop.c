#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

void main()
{
        key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);
	
	*value = 0;

	int c;

  	while(1)
	{
		system("clear");
		printf("Shop\n");
		printf("Food Stock: %d\n", *value);
		printf("Choices\n");
		printf("1. Restock\n");
		printf("2. Exit\n");
		system ("/bin/stty raw");
		c=getchar();
		system ("/bin/stty cooked");
		if(c=='1') *value+=1;
		if(c=='2')
		{
			system("clear");
			break;
		}
		
  	}
	
}
