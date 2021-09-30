#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // read, write, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY, socket etc...
#include <string.h> // memset


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
                unsigned char* buffer){

    *spi_cs=0;
    *spi_sclk=1;
    for(int i =0; i<8;i++){
        *spi_mosi=buffer[0] & 1<<(8-i);
 }     
    *spi_cs=1;


}


struct sockaddr_in server, client;

int create_socket(){
  int serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverFd < 0) {
    perror("Cannot create socket");
    exit(1);
  }
  return serverFd;
}

int bind_port(int port,int serverFd ){
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


}
int receive (int serverFd,char * buffer, int port){
    int clientFd;
    int len = sizeof(client);
    printf("waiting for clients at port %d \n",port );
    if ((clientFd = accept(serverFd, (struct sockaddr *)&client, &len)) < 0) {
      perror("accept error");
      exit(4);
    }
    char *client_ip = inet_ntoa(client.sin_addr);
    printf("Accepted new connection from a client %s:%d\n", client_ip, ntohs(client.sin_port));
    *buffer=0;
    int size = read(clientFd, buffer, sizeof(buffer));
    if ( size < 0 ) {
      perror("read error");
      exit(5);
    }
    printf("received %s from client\n", buffer);
    return size;
}

int main() {
    int clientFd;
    int port=1234;
    int serverFd=create_socket(); 
    int out = bind_port(port,serverFd);

char* buffer;
unsigned char * const spi_cs;
unsigned char * const spi_sclk;
unsigned char * const spi_mosi;
const unsigned char spi_miso;

int len;

  while (1) {
    
    int size =receive(serverFd,buffer,port);

    if (write(clientFd, buffer, size) < 0) {
      perror("write error");
      exit(6);
    }
    write_SPI( spi_cs,
                spi_sclk,
                spi_mosi,
                spi_miso,
                buffer);
    close(clientFd);
  }
  close(serverFd);
  return 0;
}