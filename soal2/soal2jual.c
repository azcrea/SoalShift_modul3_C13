#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#define PORT 8080

int status=1;
int *value;

void* stok()
{
    while(1)
    {
        printf("Stok sekarang %d\n",*value);
        sleep(5);
        fflush(stdout);
    }
}

void* jualan(void* arg)
{
    int client = *(int*) arg;
    int red;
    char buffer[1024]={0};
    while(red=read(client,buffer,1024)){
        if(strcmp(buffer,"tambah")==0){
            printf("diminta %s\n",buffer);
            *value = *value+1;
            char msg[] = "berhasil menambahkan";
            send(client,msg,sizeof(msg),0);
            memset(buffer,0,sizeof(msg));
        }
        else{
            char msg[] = "query tidak bisa diproses";
            send(client,msg,sizeof(msg),0);
            memset(buffer,0,sizeof(msg));
        }
        memset(buffer,0,sizeof(buffer));
    }
    status=1;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket,valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024]={0};
    char *hello = "Nambah stok kuy";
    char *gagal = "Sabar ya ada yang lagi restok ";

    key_t key = 1234;
    int mem = shmget(key,sizeof(int),IPC_CREAT|0666);
    value = shmat(mem,NULL,0);
      
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
    pthread_t t;
    pthread_create(&t,NULL,stok,NULL);
    while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))){
        if(!status){
            send(new_socket , gagal , strlen(gagal) , 0 );
            close(new_socket);
        }
        else{
            int valread = read( new_socket , buffer, 1024);
            printf("%s\n",buffer );
            send(new_socket , hello , strlen(hello) , 0 );
            printf("Hello message sent\n");
            int *sekarang = malloc(sizeof(*sekarang));
            *sekarang = new_socket;
            pthread_t tid;
            pthread_create(&tid,NULL,jualan,sekarang);
            status=0;
        }
    }

    return 0;
}