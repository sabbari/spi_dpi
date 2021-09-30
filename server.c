#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // read, write, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY, socket etc...
#include <string.h> // memset
#include "svdpi.h"


struct sockaddr_in server, client;

int clientFd,serverFd ; 
int port =1235;
unsigned char buffer [1023];
int counter =0;
int size;

unsigned int buffer_index=0;





int receive ();
 int send_to_client();

extern  int create_socket_andbind();
extern  void close_server(){
 close(serverFd);
}


//
int state=0;
int clock_state=0;
unsigned char miso_buffer[1023];
extern int write_SPI(svBit *  spi_cs,
		        svBit *  spi_sclk,
		        svBit *  spi_mosi,
		        const unsigned int spi_miso){
int i =counter % 8;

if(state) printf("counter %d  state %d i %d clock_state %d buffer_index %d  size %d miso %d \n",counter,state,i,clock_state,buffer_index,size,miso_buffer[buffer_index]);



	if(state==0){
		*spi_cs = 1;
		size=receive();
		if(size>0) {
			state=1;
			}
		return 1;
	}

	if(state ==1){
		*spi_cs = 0;
		*spi_sclk=1;
		state =2;
		counter=0;
		buffer_index=0;
		return 1;
	}
	if(state==2){
	
		*spi_mosi=buffer[buffer_index] & 1<<(7-i)?1:0;
		if(clock_state==0){
			*spi_sclk=0;
			clock_state=1;	
			return 1;		
			}
		if(clock_state==1){
			*spi_sclk=1;
			clock_state=0;
			counter=counter+1;
			miso_buffer[buffer_index]=miso_buffer[buffer_index]<<1 | spi_miso;
			
			if(i==7){
				if(buffer_index==(size-3)){
					state=3;
					return 1;		
					}
				else {
				buffer_index=buffer_index+1;
				return 1;
					}			
				}
			return 1 ;
		}

	  }
	if(state==3){
	    	*spi_cs =1;
		*spi_sclk=1;
		state=0;
		send_to_client();
		return 0;
		}


}




int create_socket_andbind(){
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
 int receive (){
    
    int len = sizeof(client);
    printf("waiting for clients at port %d \n",port );
    if ((clientFd = accept(serverFd, (struct sockaddr *)&client, &len)) < 0) {
      perror("accept error");
      exit(4);
    }
    char *client_ip = inet_ntoa(client.sin_addr);
    printf("Accepted new connection from a client %s:%d\n", client_ip, ntohs(client.sin_port));
    memset(buffer, 0, sizeof(buffer));
	memset(miso_buffer, 0, sizeof(miso_buffer));
    size = read(clientFd, buffer, sizeof(buffer));
    if ( size < 0 ) {
      perror("read error");
      exit(5);
    }
    printf("received %s from client\n", buffer);
    return size;
}


  int send_to_client()
{
    if (write(clientFd, miso_buffer, size-2) < 0) {
      perror("write error");
      exit(6);
    }
    close(clientFd);
}

/*
int main() {

create_socket_andbind();


    unsigned int   spi_cs;
    unsigned int   spi_sclk;
    unsigned int   spi_mosi;
    const unsigned int spi_miso;


  while (1) {
    
    receive();
    send_to_client();
    write_SPI(&spi_cs,&spi_sclk,&spi_mosi,spi_miso);
    
  }
 close_server();
  return 0;
}*/