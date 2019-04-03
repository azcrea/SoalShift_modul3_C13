#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> //library thread
#include <unistd.h>

int wakeup_status = 0;
int spirit_status = 100;
int tidur=0;
int bangun=0;
int woiagmal=0;
int woiiraj=0;

void* iraj(){
    while(1){
        if(bangun==3){
            printf("Fitur Iraj Ayo tidur disabled 10 s\n");
            sleep(10);
            bangun = 0;
            woiiraj = 0;
            continue;
        }
        if(!woiiraj) continue;
        if(bangun<3){
            spirit_status-=20;
            tidur++;
        }
        woiiraj = 0;
    }
}

void* agmal(){
    while(1){
        if(tidur==3){
            printf("Fitur Agmal Ayo bangun disabled 10 s\n");
            sleep(10);
            tidur = 0;
            woiagmal = 0;
            continue;
        }
        if(!woiagmal) continue;
        if(tidur<3){
            wakeup_status+=15;
            bangun++;
        }
        woiagmal = 0;
    }
}

int main(){
    char query[1000]={0};
    pthread_t ag,ir;
    pthread_create(&ag,NULL,agmal,NULL);
    pthread_create(&ir,NULL,iraj,NULL);
    printf("Gambaran hidup Iraj dan Agmal\n");
    while(wakeup_status<100&&spirit_status>0){
        gets(query);
        if(!strcmp(query,"All status")){
            printf("Agmal Wakeup_Status = %d\nIraj Spirit_Status = %d\n",wakeup_status,spirit_status);
        }
        else if(!strcmp(query,"Iraj Ayo Tidur")&&bangun<3) woiiraj = 1;
        else if(!strcmp(query,"Agmal Ayo Bangun")&&tidur<3) woiagmal = 1;
        while(woiiraj || woiagmal);
    }
    if(wakeup_status >= 100) printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
    else if(spirit_status<=0) printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");

}