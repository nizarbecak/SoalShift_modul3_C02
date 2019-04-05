#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<pthread.h>
#include<sys/types.h>
#define PORT 8081

int *stok;

pthread_t tid[3];

int length=5;
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
			sleep(5);
			printf("Stok : %d\n", *stok);
		}
	}
	else if(pthread_equal(id,tid[1]))
	{
		execv("/usr/bin/xlogo", argv2);
	}
	else if(pthread_equal(id,tid[2]))
	{
		execv("/usr/bin/pkill", argv2);
	}

	return NULL;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello1 = "transaksi berhasil";
    char *hello2 = "transaksi gagal";

    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    *value=0;
    stok=value;
    

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

	int i=0;
	int err;
	while(i<1)
	{
		err=pthread_create(&(tid[i]),NULL,&playandcount,NULL);
		i++;
	}

    while(strcmp(buffer, "tutup"))
    {
        memset(buffer, 0, sizeof(buffer));
        valread = read( new_socket , buffer, 1024);

	if(!strcmp(buffer, "tambah"))
	{
	    *value+=1;
	    stok = value;
	}
    }
    exit(0);
    return 0;
}
