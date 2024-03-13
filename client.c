#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<errno.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "aes.h"
#define CTR 1
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wpointer-sign"
struct AES_ctx ctx;
uint8_t key[] = "@yj$4(bMaxyz$$$$";
uint8_t iv[] = "jesper$$$$$$$$$$";
void error(void);
int main(int argc ,char *argv[]){
int sockfd,portno,n;
struct sockaddr_in serv_addr;
struct hostent *server;
uint8_t buff[1000000];
if (argc <3){
fprintf(stderr,"usage %s hostname port\n",argv[0]);
exit(0);
}
portno=atoi(argv[2]);
sockfd =socket(AF_INET,SOCK_STREAM,0);
if (sockfd <0){
error();
}
server =gethostbyname(argv[1]);
if (server ==NULL){
fprintf(stderr,"error no such host\n");
}
bzero((char *) &serv_addr,sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *) server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
serv_addr.sin_port =htons(portno);
if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
error();
}
while(1){
bzero(buff,sizeof(buff));
 fgets(buff,sizeof(buff),stdin);
AES_init_ctx_iv(&ctx, key, iv);
AES_CTR_xcrypt_buffer(&ctx,buff,strlen(buff));
n= write(sockfd,buff,strlen(buff));
if (n<0){
error();
printf("error on writing\n");
}
bzero(buff,sizeof(buff));
n=read(sockfd,buff,sizeof(buff));
if (n <0){
error();
printf("error on reading");
}
AES_init_ctx_iv(&ctx, key, iv);
AES_CTR_xcrypt_buffer(&ctx,buff,strlen(buff));
printf("server: %s",buff);
int i = strncmp("Bye",buff,3);
if (i==0){
break;
}
}
close(sockfd);

return 0;
}
void error(void){
printf("%s[ERROR NO]:%d",strerror(errno),errno);
exit(1);
}

