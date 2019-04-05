#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <termios.h>
#define ATT 20
#define Stand_by 0
#define battle 1
#define shop 2

int status=0;
int *value;
char a = '\0';
int hp=300;
int hung=200;
int hyg=100;
int stk = 5;
int cd=20;
int eat=0;
int bath = 0;
int where = 0;
int opponent = 100;
int turn = 0;
int stop = 0;

key_t key = 1234;
int mem;

struct termios old, new;
/* Read 1 character - echo defines echo mode */
char getch() 
{
    char ch;
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    // new.c_lflag |= ECHO; /* set echo mode */
    new.c_lflag &= ~ECHO; /* set no echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
    ch = getchar();
    tcsetattr(0, TCSANOW, &old);
    return ch;
}

void* starv()
{
    int i=0;
    while(i<10){
        // while(where == battle);
        if(where!=battle){
            sleep(1);
            if(eat){
                i=0;
                hung+=15;
                eat=0;
                continue;
            }
            i++;
            if(i==10) hung-=5;
            i%=10;
        }
    }
}

void* regen()
{
    int i=0;
    while(i<10){
        // while(where == battle);
        if(where!=battle){
            sleep(1);
            i++;
            if(i==10) hp+=5;
            i%=10;
        }
    }
}

void* hygine()
{
    int i=0;
    while(i<30){
        // while(where == battle);
        if(where!=battle){
            sleep(1);
            if(bath){
                i=0;
                hyg+=30;
                cd = 20;
                bath=0;
                continue;
            }
            i++;
            if(cd>0) cd--;
            if(i==30) hyg-=10;
            i%=30;
        }
    }
}

void* input(){
    while ((a = getch())) {
        if(where == Stand_by){
            if(a=='1'){
                if(stk>0){
                    eat = 1;
                    stk--;
                }
                else{
                    printf("Tidak punya makanan");
                }
            }
            if(a=='2'){
                if(!cd){
                    bath=1;
                }
            }
            if(a=='3'){
                system("clear");
                turn=0;
                where = battle;
            }
            if(a=='4'){
                system("clear");
                where = shop;
            }
            if(a=='5'){
                stop=1;
            }
        }
        else if(where == battle){
            if(a=='1'){
                if(turn==0&&opponent>=20){opponent-=ATT;sleep(1);turn = 1;}
                else printf("Your monster has already won\n");
            }
            if(a=='2'){
                system("clear");
                turn = 0;
                opponent = 100;
                where = Stand_by;
            }
        }
        else if(where == shop){
            if(a=='1'){
                if(*value > 0){
                    stk++;
                    *value = *value - 1;
                }
                else{
                    printf("Stock is empty");
                }
            }
            if(a=='2'){
                system("clear");
                where = Stand_by;
            }
        }
    }
}

void* tampilan()
{
    while(1){
        if(where==Stand_by){
            printf("Standby Mode\n");
            printf("Health : %d\n",hp);
            printf("Hunger : %d\n",hung);
            printf("Hygiene : %d\n",hyg);
            printf("Food left : %d\n",stk);
            if(cd>0)printf("Bath will be ready in %ds\n",cd);
            else printf("Bath is ready\n");
            printf("Choices\n");
            printf("1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n");
            sleep(1);
            system("clear");
        }
        if(where==battle){
            printf("Battle Mode\n");
            printf("Monster's Health : %d\n",hp);
            printf("Opponent's Health : %d\n",opponent);    
            printf("Choices\n");
            printf("1. Attack\n2. Run\n");
            sleep(1);
            system("clear");
            if(turn==1 && opponent>0){hp-=ATT;turn = 0;}
        }
        if(where == shop){
            printf("Shop Mode\n");
            printf("Shop Food Stock : %d\n",*value);
            printf("Your Food Stock : %d\n",stk);    
            printf("Choices\n");
            printf("1. Buy\n2. Back\n");
            sleep(1);
            system("clear");
        }
    }
}

int main(int argc, char const *argv[]) {

    mem = shmget(key,sizeof(int),IPC_CREAT|0666);
    value = shmat(mem,NULL,0);
    pthread_t t[5];
    pthread_create(&t[0],NULL,input,NULL);
    pthread_create(&t[1],NULL,tampilan,NULL);
    pthread_create(&t[2],NULL,starv,NULL);
    pthread_create(&t[3],NULL,regen,NULL);
    pthread_create(&t[4],NULL,hygine,NULL);
    while(hung>0&&hyg>0&&hp>0&&!stop);
    pthread_cancel(t[0]);
    tcsetattr(0, TCSANOW, &old);
    printf("game has ended\n");
    return 0;
}