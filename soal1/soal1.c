#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread
#include <unistd.h>

void *fact( void *ptr );

long long hasil[1000]; 

int main(int argc, char* argv[])
{
    pthread_t th[argc-1];
    int arg[argc-1];
    //[10,8,13]
    for(int k=1;k<argc;k++){
        arg[k-1]= atoi(argv[k]);
        // printf("%d\n",arg[k-1]);
    }

    for(int i=0;i<argc-1;i++)
    {
        int *sekarang = malloc(sizeof(*sekarang));
        *sekarang = arg[i];
        pthread_create(&th[i],NULL,fact,sekarang);
    }
    for(int i=0;i<argc-1;i++) pthread_join(th[i],NULL);
    //[8,10,13]
    for(int i=0;i<argc-1;i++){
        for(int j=i+1;j<argc-1;j++){
            if(arg[i]>arg[j]){
                int temp = arg[i];
                arg[i]=arg[j];
                arg[j]=temp;
            }
        }        
    }
    for(int i=0;i<argc-1;i++){
        printf("%d! = %lld\n",arg[i],hasil[arg[i]]);
    }
}

void *fact( void *ptr )
{
    int from = *((int *)ptr);
    hasil[from]=1;
    for(int i=0;i<from;i++){
        hasil[from]*=(i+1);
    }
    return NULL;
}