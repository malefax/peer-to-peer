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
struct interfaces{
char ifaces[255];
};//global  structure;
struct AES_ctx ctx;
uint8_t key[] = "@yj$4(bMaxyz$$$$";
uint8_t iv[] = "jesper$$$$$$$$$$";
void error(void);
char *interface(char *iface);
struct interfaces  *interface_names(void);
int main(int argc ,char *argv[]){
if (argc<2){
char hostbuff[256];
int hostname;
hostname=gethostname(hostbuff,sizeof(hostbuff));
if (hostname==-1){
error();
}
fprintf(stderr,"port number doesnt provided by [USER]:%s\n",hostbuff);
exit(1);
}
struct interfaces *adr=NULL;
adr=interface_names();
if (adr==NULL){
puts("error to find interface names");
exit(1);
}
printf("available interfaces\n");
printf("[%s]\n[%s]\n",adr[1].ifaces,adr[2].ifaces);
char inface[255];
printf("enter the interface name:");
scanf("%s",inface);
printf("\n");
if (strcmp(inface,adr[1].ifaces) == 0 || strcmp(inface,adr[2].ifaces)==0 ){
printf("sucessfully assined the interface [%s]\n",inface);
}
else{
perror("invalid interface selected\n exit status");
exit(0);
}
char *some_addr=NULL;
some_addr=interface(inface);
if (some_addr==NULL){
printf("error to find ip adresss associated to interface %s",inface);
exit(1);
}
printf("[%s]:%s\n",inface,some_addr);
int sockfd,newsockfd,portno,n,lis;
char lisddr[255];
uint8_t buff[1000000];
struct sockaddr_in serv_addr,client_addr;
socklen_t clilen;
sockfd = socket(AF_INET,SOCK_STREAM,0);
if (sockfd < 0){
error();
printf("error in opening sockets");
}
bzero((char*)&serv_addr,sizeof(serv_addr));
portno=atoi(argv[1]);
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=inet_addr(some_addr);
serv_addr.sin_port=htons(portno);
if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))<0){
error();
printf("failed to bind");
}
fflush(stdout);
lis=listen(sockfd,5);
if (lis <0){
error();
}
sprintf(lisddr,"listening %s........",some_addr);
puts(lisddr);
fflush(stdout);
newsockfd=accept(sockfd,(struct sockaddr*)&client_addr,&clilen);
if (newsockfd <0){
error();
printf("error to accept any connections");
}
while(1){
bzero(buff,sizeof(buff));
n=read(newsockfd,buff,sizeof(buff));
if (n < 0){
error();
printf("error in reading");
}
AES_init_ctx_iv(&ctx, key, iv);
AES_CTR_xcrypt_buffer(&ctx,buff,strlen(buff));
printf("client: %s",buff);
bzero(buff,sizeof(buff));
fgets(buff,sizeof(buff),stdin);
AES_init_ctx_iv(&ctx, key, iv);
AES_CTR_xcrypt_buffer(&ctx,buff,strlen(buff));
n=write(newsockfd,buff,strlen(buff));
if (n<0){
error();
printf("failed to write");
}
int i = strncmp("Bye",buff,3);
if (i==0){
break;
}
}
close(newsockfd);
close(sockfd);
return 0;
}
void error(void){
printf("%s[ERROR NO]:%d",strerror(errno),errno);
exit(1);
}
char *interface(char *iface){
int sock;
struct ifreq ifr;
sock = socket(AF_INET,SOCK_STREAM,0);
if (sock ==-1){
error();
}
ifr.ifr_addr.sa_family=AF_INET;
strncpy(ifr.ifr_name,iface,IFNAMSIZ);
ioctl(sock,SIOCGIFADDR,&ifr);
if (ioctl(sock,SIOCGIFADDR,&ifr)<0){
error();
}
close(sock);
//printf("[%s]ip:%s",iface,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
char *addr;
addr=inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
return addr;
}
struct interfaces  *interface_names(void){
struct if_nameindex *if_ni, *i;
 if_ni = if_nameindex();
 if (if_ni == NULL) {
               perror("if_nameindex");
               exit(EXIT_FAILURE);
 }
 printf("maximum number of interfaces deployed: 2\n");
 struct interfaces *addr=malloc(3*sizeof(struct interfaces));
 if(addr==NULL){
 error();
 exit(1);
 }
 int k=0;
 for (i = if_ni; !(i->if_index == 0 && i->if_name == NULL); i++){
 strncpy(addr[k].ifaces,i->if_name,strlen(i->if_name));
 k++;
 }
 if_freenameindex(if_ni);
 return addr;
}


 
