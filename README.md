# SoalShift_modul3_C02

## Soal 1
## Soal 2
   Client yang terhubung dengan Server Pembeli.
    
    ```
    #include <stdio.h>
    #include <sys/socket.h>
    #include <stdlib.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #define PORT 8080

    int main(int argc, char const *argv[]) {
        struct sockaddr_in address;
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        char hello[1024];
        char buffer[1024] = {0};
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }

        while(strcmp(hello, "tutup"))
        {
            memset(buffer, 0, sizeof(buffer));
            memset(hello, 0, sizeof(hello));
      scanf("%s", hello);
            send(sock , hello , strlen(hello) , 0 );

            if(strcmp(hello, "tutup"))
      {
              valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
      }
        }
        return 0;
    }
    ```
    
   Client yang terhubung dengan Server Penjual.
   
    ```
    #include <stdio.h>
    #include <sys/socket.h>
    #include <stdlib.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #define PORT 8081

    int main(int argc, char const *argv[]) {
        struct sockaddr_in address;
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        char hello[1024];
        char buffer[1024] = {0};
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }

        while(strcmp(hello, "tutup"))
        {
      memset(buffer, 0, sizeof(buffer));
            memset(hello, 0, sizeof(hello));
      scanf("%s", hello);
            send(sock , hello , strlen(hello) , 0 );
        }
        return 0;
    }
    ```
   
   Server Pembeli.
   
    ```
    #include <stdio.h>
    #include <sys/socket.h>
    #include <stdlib.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #define PORT 8080

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

        while(strcmp(buffer, "tutup"))
        {
            memset(buffer, 0, sizeof(buffer));
            valread = read( new_socket , buffer, 1024);

      if(!strcmp(buffer, "beli"))
      {
          if(*value>0)
          {
        *value-=1;
        send(new_socket , hello1 , strlen(hello1) , 0 );
          }
          else
          {
        send(new_socket , hello2 , strlen(hello2) , 0 );
          }
      }
        }
        return 0;
    }
    ```
    
   Server Penjual.
   
    ```
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
    ```
   
   Berikut adalah SS nya.
   ![Gambar1](/images/nomor2.png)
   
   Penjelasan
   - Disini saya menggunakan 4 program.
   - Programnya terdiri dari 2 Client dan 2 server.
   - Menggunakan share memory untuk menyimpan nilai stok barang pada variabel value.
   - Server pembeli akan mengirimkan pesan "transaksi berhasil" jika value > 0. Sebalikan akan mengirimkan "transaksi gagal" jika value tidak lebih dari 0.
   - Thread digunakan pada server penjual untuk mengeprint stok tersisa setiap 5 detik.
   
## Soal 3
## Soal 4
## Soal 5

   Game
   
    ```
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
    ```
   
   Shop
   
    ```
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
    ```
   
  Berikut adalah hasilnya.
  ![Gambar2](/images/nomor51.png)
  ![Gambar3](/images/nomor52.png)
  ![Gambar4](/images/nomor53.png)
  
  Penjelasan
  - Disini saya menggunakan 2 program, yaitu program game nya sendiri, dan shop untuk menambah jumlah barang di market
  - Menggunakan share memory untuk jumlah stok makanan di market
  - Global variabel pada hunger, hygiene, healthmonster, bathcooldown, dan battlestatus agar mudah dipakai dalam thread yang nantinya akan mengurangi nilai hunger, hygiene, bathcooldown dan menambah nilai healthmonster pada status standby.
  - Setiap scene nya saya menggunakan system("clear") untuk menghilangkan tulisan-tulisan sebelumnya,
  - system ("/bin/stty raw"); c=getchar(); system ("/bin/stty cooked"); untuk menangkap input tanpa harus menekan enter.
