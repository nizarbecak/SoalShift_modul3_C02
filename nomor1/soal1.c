#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* print_factorial(void* arg)
{
	int answer = 1;
	int *number = (int*) arg;
	int iter;

	for(iter = 1; iter <= *number; iter++)
	{
		answer = answer * iter;
	}

	printf("%d! = %d\n", *number, answer);
}

int main(int argc, char* argv[])
{
	int i, j, numbers[argc - 1], temp;
	pthread_t thread[argc - 1];

	for (i = 0; i < argc - 1; i++)
	{
		numbers[i] = atoi(argv[i + 1]);
	}

	for (i = 0; i < argc - 1; i++)
	{
		for (j = i + 1; j < argc - 1; j++)
		{
			if (numbers[i] > numbers[j])
			{
				temp = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = temp;
			}
		}
	}

	for (i = 0; i < argc - 1; i++)
	{
		temp = pthread_create(&(thread[i]), NULL, print_factorial, &numbers[i]);

		if (temp != 0)
		{
			printf("ERROR : can't generate factorial\n");
		}

		pthread_join(thread[i], NULL);
	}

	exit(0);
	return EXIT_SUCCESS;
}
