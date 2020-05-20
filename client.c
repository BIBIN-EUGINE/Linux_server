#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <pthread.h>

void *clientthread(void *arg)
{
    int sockfd = 0, n = 0,valread;
    char recvBuff[1024];
    char buffer[1024] = {0};
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        close(sockfd);
        pthread_exit(NULL);
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        close(sockfd);
        pthread_exit(NULL);
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       close(sockfd);
       pthread_exit(NULL);
    } 

    send(sockfd , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    valread = read( sockfd , buffer, 1024); 
    printf("%s\n",buffer ); 

    close(sockfd);
    pthread_exit(NULL);
}


int main(){

 int i = 0;
 pthread_t tid[51];
 while(i<50){
    if(pthread_create(&tid[i],NULL,clientthread,NULL)!=0)
       printf("Failed to create thread \n");
    i++;
 }
 sleep(20);
 i = 0;
 while(i<50){
   pthread_join(tid[i++],NULL);
 }
 return 0;
}
