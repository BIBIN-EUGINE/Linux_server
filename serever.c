#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>

char client_message[2000] = "hai";
char buffer[1024]= {0}; ;
char *hello = "Hello from server"; 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *fromMc(void *arg){
//Get the datas from the microcontroller
pthread_exit(NULL);

}

void *socketThread(void *arg){
  
  int new_socket = *((int *)arg);
  int valread;
  pthread_mutex_lock(&lock);

  //Bussiness logic
  valread = read( new_socket , buffer, 1024); 
  printf("%s\n",buffer ); 
  send(new_socket , hello , strlen(hello) , 0 ); 
  printf("Hello message sent\n"); 
  pthread_mutex_unlock(&lock);
  sleep(1);
  close(new_socket);
  pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 50); 
    pthread_t tid[60],MC; 
    int i = 0;
    while(1)
    {
        if(pthread_create(&MC,NULL,fromMc,NULL)!=0){
          printf("Failed to create thread \n");
        }
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        
        //creating new thread for every client request
        if(pthread_create(&tid[i],NULL,socketThread, &connfd)!=0)
              printf("Failed to create thread \n");
        if(i>=50)
        {
          i=0;
          while(i<50)
          {
            pthread_join(tid[i++],NULL);
          }
          i=0;
        }
    }
    pthread_join(MC,NULL);
    return 0;
}
