#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // read, write, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY, socket etc...
#include <string.h> // memset
#include "svdpi.h"


struct sockaddr_in server, client;

int clientFd,serverFd ; 
int port =1234;
char buffer [1023];
int size;














void spi_clk_cycle(unsigned char * const spi_sclk, unsigned char period_us ){
    *spi_sclk=0;
    usleep(period_us);
    *spi_sclk=1;
    usleep(period_us);

}
int write_SPI(unsigned char * const spi_cs,
                unsigned char * const spi_sclk,
                unsigned char * const spi_mosi,
                const unsigned char spi_miso,
                unsigned char buffer[]){

    *spi_cs=0;
    *spi_sclk=1;
    for(int i =0; i<8;i++){
        *spi_mosi=buffer[0] & 1<<(8-i);
 }     
    *spi_cs=1;


}




extern  int create_socket_andbind(){
  serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverFd < 0) {
    perror("Cannot create socket");
    exit(1);
  }
  
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY;
  server.sin_port = htons(port);
  int len = sizeof(server);
  if (bind(serverFd, (struct sockaddr *)&server, len) < 0) {
    perror("Cannot bind socket");
    exit(2);
  }
  if (listen(serverFd, 10) < 0) {
    perror("Listen error");
    exit(3);
  }
    return serverFd;

}
extern  int receive (){
    
    int len = sizeof(client);
    printf("waiting for clients at port %d \n",port );
    if ((clientFd = accept(serverFd, (struct sockaddr *)&client, &len)) < 0) {
      perror("accept error");
      exit(4);
    }
    char *client_ip = inet_ntoa(client.sin_addr);
    printf("Accepted new connection from a client %s:%d\n", client_ip, ntohs(client.sin_port));
    memset(buffer, 0, sizeof(buffer));
    size = read(clientFd, buffer, sizeof(buffer));
    if ( size < 0 ) {
      perror("read error");
      exit(5);
    }
    printf("received %s from client\n", buffer);
    return size;
}


extern  int send_to_client()
{
    if (write(clientFd, buffer, size) < 0) {
      perror("write error");
      exit(6);
    }
    close(clientFd);
}

extern  void close_server(){
 close(serverFd);
}
int main() {

create_socket_andbind();


    unsigned char * const spi_cs;
    unsigned char * const spi_sclk;
    unsigned char * const spi_mosi;
    const unsigned char spi_miso;

  while (1) {
    
    receive();
    send_to_client();
    
  }
 close_server();
  return 0;
}