#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int WakeUp_Status = 0, Spirit_Status = 100;
int status = 0;
int count_ayobangun = 0, count_ayotidur = 0;

void* All_Status(void* arg)
{
    while(1)
    {
        if(status == 1)
        {
            printf("Agmal WakeUp_Status = %d\n", WakeUp_Status);
            printf("Iraj Spirit_Status = %d\n", Spirit_Status);
            status = 0;
        }
    }
}

void* Agmal_Ayo_Bangun(void* arg)
{
    while(1)
    {
        if(count_ayotidur == 3)
        {
            printf("Agmal Ayo Bangun disabled for 10 s\n");
            count_ayotidur = 0;
            sleep(10);
            continue;
        }
        
        if(status == 2)
        {
            count_ayobangun = count_ayobangun + 1;

            WakeUp_Status = WakeUp_Status + 15;

            if(WakeUp_Status >= 100)
            {
                printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
                exit(0);
                return EXIT_SUCCESS;
            }

            status = 0;
        }
    }
}

void* Iraj_Ayo_Tidur(void* arg)
{
    while(1)
    {
        if(count_ayobangun == 3)
        {
            printf("Iraj Ayo Tidur disabled for 10 s\n");
            count_ayobangun = 0;
            sleep(10);
            continue;
        }

        if(status == 3)
        {
            count_ayotidur = count_ayotidur + 1;

            Spirit_Status = Spirit_Status - 20;

            if(Spirit_Status <= 0)
            {
                printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
                exit(0);
                return EXIT_SUCCESS;
            }

            status = 0;
        }
    }
}

int main(void)
{
	pthread_t thread1, thread2, thread3;
    char input[50];

    pthread_create(&(thread1), NULL, All_Status, NULL);
    pthread_create(&(thread2), NULL, Agmal_Ayo_Bangun, NULL);
    pthread_create(&(thread3), NULL, Iraj_Ayo_Tidur, NULL);

    while(1)
    {
        fgets(input, 50, stdin);

        if(strcmp(input, "All Status\n") == 0)
        {
            status = 1;
        }

        else if(strcmp(input, "Agmal Ayo Bangun\n") == 0)
        {
            status = 2;
        }

        else if(strcmp(input, "Iraj Ayo Tidur\n") == 0)
        {
            status = 3;
        }

    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

	exit(0);
	return EXIT_SUCCESS;
}