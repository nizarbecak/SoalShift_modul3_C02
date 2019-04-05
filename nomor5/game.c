#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include<sys/types.h>
#include<sys/wait.h>

char monstername[256];
int hunger;
int hygiene;
int monsterhealth;
int bathcooldown;
int battlestatus;

pthread_t tid[5];

void* playandcount(void *arg)
{
	char *argv1[] = {"clear", NULL};
	char *argv2[] = {"xlogo", NULL};
	unsigned long i=0;
	pthread_t id=pthread_self();
	int iter;
	if(pthread_equal(id,tid[0]))
	{
		while(1)
		{
			sleep(10);
			if(battlestatus==0) monsterhealth+=5;
			if(monsterhealth>300) monsterhealth=300;
		}
	}
	else if(pthread_equal(id,tid[1]))
	{
		while(1)
		{
			sleep(10);
			if(battlestatus==0) hunger-=5;
		}
	}
	else if(pthread_equal(id,tid[2]))
	{
		while(1)
		{
			sleep(30);
			if(battlestatus==0) hygiene-=10;
		}
	}
	else if(pthread_equal(id,tid[3]))
	{
		while(1)
		{
			sleep(1);
			if(bathcooldown>0) bathcooldown--;
		}
	}

	return NULL;
}

void main()
{
	system("clear");
	printf("Enter the monster name\n");
	scanf("%s", monstername);
	hunger=200;
	hygiene=100;
	monsterhealth=300;
	bathcooldown=0;
	battlestatus=0;
	int food;
	food=0;
	
	key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

	system("clear");
	int c;
	
	int i=0;
	int err;
	while(i<4) // loop sejumlah thread
	{
		err=pthread_create(&(tid[i]),NULL,&playandcount,NULL); //membuat thread
		i++;
	}
	//pthread_join(tid[0],NULL);
	//pthread_join(tid[1],NULL);
	//pthread_join(tid[2],NULL);

	while(1){
		if(monsterhealth<=0 || hygiene<=0 || hunger<=0)
		{
			system("clear");
			printf("Game Over");
			break;
		}
		system("clear");
		printf("Standby Mode\n");
		printf("Health : %d\n", monsterhealth);
		printf("Hunger : %d\n", hunger);
		printf("Hygiene : %d\n", hygiene);
		printf("Food left: %d\n", food);
		if(bathcooldown==0) printf("Bath is ready\n");
		else printf("Bath will be ready in %d s\n", bathcooldown);
		printf("Choices\n");
		printf("1. Eat\n");
		printf("2. Bath\n");
		printf("3. Battle\n");
		printf("4. Shop\n");
		printf("5. Exit\n");

		system ("/bin/stty raw");
		c=getchar();
		system ("/bin/stty cooked");
		if(monsterhealth<=0 || hygiene<=0 || hunger<=0)
		{
			system("clear");
			printf("Game Over");
			break;
		}
		if (c=='1')
		{
			if(monsterhealth<=0 || hygiene<=0 || hunger<=0)
			{
				system("clear");
				printf("Game Over");
				break;
			}
			if (food>0)
			{
				hunger+=15;
				if (hunger>200) hunger=200;
				food--;
			}
		}

		else if (c=='2')
		{
			if(monsterhealth<=0 || hygiene<=0 || hunger<=0)
			{
				system("clear");
				printf("Game Over");
				break;
			}
			if(bathcooldown==0)
			{
				hygiene+=30;
				if(hygiene>100) hygiene=100;
				bathcooldown=20;
			}
		}

		else if (c=='3')
		{
			if(monsterhealth<=0 || hygiene<=0 || hunger<=0)
			{
				system("clear");
				printf("Game Over");
				break;
			}
			battlestatus=1;
			int enemyhealth;
			enemyhealth = 100;
			while(1)
			{
				system("clear");
				printf("Battle Mode\n");
				printf("Monster's Health : %d\n", monsterhealth);
				printf("Enemy's Health : %d\n", enemyhealth);
				printf("Choices\n");
				printf("1. Attack\n");
				printf("2. Run\n");

				system ("/bin/stty raw");
				c=getchar();
				system ("/bin/stty cooked");

				if (c=='1')
				{
					enemyhealth-=20;
					if(enemyhealth<=0 || monsterhealth<=0)
					{
						battlestatus=0;
						break;
					}
					monsterhealth-=20;
					if(enemyhealth<=0 || monsterhealth<=0)
					{
						battlestatus=0;
						break;
					}
					system("clear");
				}
				
				else if (c=='2')
				{
					battlestatus=0;
					break;
				}
			}
		}

		else if (c=='4')
		{
			if(monsterhealth<=0 || hygiene<=0 || hunger<=0)
			{
				system("clear");
				printf("Game Over");
				break;
			}
			while(1)
			{
				system("clear");
				printf("Shop Mode\n");
				printf("Shop food stock : %d\n", *value);
				printf("Your food stcok : %d\n", food);
				printf("Choices\n");
				printf("1. Buy\n");
				printf("2. Back\n");

				system ("/bin/stty raw");
				c=getchar();
				system ("/bin/stty cooked");

				if (c=='1')
				{
					if(*value>0)
					{
						*value-=1;
						food+=1;
					}
				}
				
				else if (c=='2')
				{
					break;
				}
			}
		}

		else if (c=='5')
		{
			if(monsterhealth<=0 || hygiene<=0 || hunger<=0)
			{
				system("clear");
				printf("Game Over\n\n");
				break;
			}
			system("clear");
			break;
		}
	}
	exit(0);
}
