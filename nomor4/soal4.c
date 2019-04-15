#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int status=0;

void* simpan_proses1(void *arg)
{
	system("ps -aux | head -n 11 | tail -n 10 > /home/becak/Documents/FolderProses1/SimpanProses1.txt");
}

void* simpan_proses2(void *arg)
{
	system("ps -aux | head -n 11 | tail -n 10 > /home/becak/Documents/FolderProses2/SimpanProses2.txt");
}

void* kompres_zip1(void *arg)
{
	system("zip -mj /home/becak/Documents/FolderProses1/KompresProses1.zip /home/becak/Documents/FolderProses1/SimpanProses1.txt");
	status = 1;
}

void* kompres_zip2(void *arg)
{
	system("zip -mj /home/becak/Documents/FolderProses2/KompresProses2.zip /home/becak/Documents/FolderProses2/SimpanProses2.txt");
	status = 1;
}

void* ekstrak_zip1(void *arg)
{
	system("unzip /home/becak/Documents/FolderProses1/KompresProses1.zip -d /home/becak/Documents/FolderProses1");
}

void* ekstrak_zip2(void *arg)
{
	system("unzip /home/becak/Documents/FolderProses2/KompresProses2.zip -d /home/becak/Documents/FolderProses2");
}

int main()
{
	pthread_t thread[6];

	pthread_create(&thread[0], NULL, simpan_proses1, NULL);
	pthread_create(&thread[1], NULL, simpan_proses2, NULL);
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);

	pthread_create(&thread[2], NULL, kompres_zip1, NULL);
	pthread_create(&thread[3], NULL, kompres_zip2, NULL);
	pthread_join(thread[2], NULL);
	pthread_join(thread[3], NULL);

	while(status!=1) {}
	printf("Menunggu 15 detik untuk mengekstrak kembali\n");
	sleep(15);

	pthread_create(&thread[4], NULL, ekstrak_zip1, NULL);
	pthread_create(&thread[5], NULL, ekstrak_zip2, NULL);
	pthread_join(thread[4], NULL);
	pthread_join(thread[5], NULL);

	return 0;
}