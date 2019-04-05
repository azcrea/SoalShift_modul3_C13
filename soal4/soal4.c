#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> //library thread
#include <unistd.h>
#include <sys/stat.h>

int un1=0;
int un2=0;
int msg=0;
int save=0;
int press=0;
int un=0;

void* save1()
{
    while(!save);
    struct stat st = {0};
    if(stat("/home/duhbuntu/Documents/FolderProses1",&st)==-1){
        mkdir("/home/duhbuntu/Documents/FolderProses1",0777);
    }
    system("ps -aux | head -n10 > /home/duhbuntu/Documents/FolderProses1/SimpanProses1.txt");
}

void* save2()
{
    while(!save);
    struct stat st = {0};
    if(stat("/home/duhbuntu/Documents/FolderProses2",&st)==-1){
        mkdir("/home/duhbuntu/Documents/FolderProses2",0777);
    }
    system("ps -aux | head -n10 > /home/duhbuntu/Documents/FolderProses2/SimpanProses2.txt");
}

void* press1()
{
    while(!press);
    un1=0;
    msg=0;
    system("zip -qmj /home/duhbuntu/Documents/FolderProses1/KompresProses1.zip /home/duhbuntu/Documents/FolderProses1/SimpanProses1.txt");
    msg=1;
    sleep(15);
    un1=1;
}

void* press2()
{
    while(!press);
    un2=0;
    msg=0;
    system("zip -qmj /home/duhbuntu/Documents/FolderProses2/KompresProses2.zip /home/duhbuntu/Documents/FolderProses2/SimpanProses2.txt");
    msg=1;
    sleep(15);
    un2=1;
}

void* unzip1()
{
    while(!un1);
    while(!un);
    system("unzip /home/duhbuntu/Documents/FolderProses1/KompresProses1.zip -d /home/duhbuntu/Documents/FolderProses1/");
}

void* unzip2()
{
    while(!un2);
    while(!un);
    system("unzip /home/duhbuntu/Documents/FolderProses2/KompresProses2.zip -d /home/duhbuntu/Documents/FolderProses2/");
}

int main()
{
    // while(1)
    // {
        pthread_t t[6];
        pthread_create(&t[0],NULL,save1,NULL);
        pthread_create(&t[1],NULL,save2,NULL);
        save = 1;
        pthread_join(t[0],NULL);
        pthread_join(t[1],NULL);
        pthread_create(&t[2],NULL,press1,NULL);
        pthread_create(&t[3],NULL,press2,NULL);
        press = 1;
        while(!msg);
        printf("Menunggu 15 detik untuk mengekstrak kembali\n");
        pthread_join(t[2],NULL);
        pthread_join(t[3],NULL);
        pthread_create(&t[4],NULL,unzip1,NULL);
        pthread_create(&t[5],NULL,unzip2,NULL);
        un = 1;
        pthread_join(t[4],NULL);
        pthread_join(t[5],NULL);
        un1=0;
        un2=0;
        msg=0;
        save=0;
        press=0;
        un=0;
    // }
}