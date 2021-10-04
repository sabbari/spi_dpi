/*
A trial to implement a master SPI that can be used in simulation

TODO : clean the code and make it areal code , be aware that a lot of function
are wrapped around a no argument function to make calling them from verilog
easier


*/

#include "svdpi.h"
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY, socket etc...
#include <fcntl.h>
#include <stdio.h>  // perror, printf
#include <stdlib.h> // exit, atoi
#include <string.h> // memset
#include <unistd.h> // read, write, close
#define OPEN_PORT 0
#define RECEIVE 1
#define SETUP_SPI 2
#define SEND_SPI 3
#define END_SPI 4
#define CLOCK_LOW 0
#define CLOCK_HIGH 1
#define DEBUG 0
#define BUFFER_SIZE 32


int size;

unsigned int buffer_index = 0;

int receive(int clientFd, char *buffer, char* miso_buffer);
int send_to_client(int clientFd, char *miso_buffer, int size);

int create_socket_and_bind(int port,int* clientFd, int* serverFd, struct sockaddr_in* server, struct sockaddr_in* client );

extern int spi(svBit *spi_cs, svBit *spi_sclk, svBit *spi_mosi,
                     const unsigned int spi_miso, int port) {

  static unsigned char buffer[BUFFER_SIZE];
  static  char miso_buffer[BUFFER_SIZE];	
  static int size, counter, state, clock_state = 0;
  static int clientFd, serverFd; // client and servet file descriptor
  static struct sockaddr_in server, client;
  int i = counter % 8;
  if (DEBUG)
    printf("counter %d  state %d i %d clock_state %d buffer_index %d  size %d "
           "mosi[i] %d mosi %d  \n",
           counter, state, i, clock_state, buffer_index, size,
           buffer[buffer_index], *spi_mosi);

  switch (state) {
  case(OPEN_PORT):
    state= create_socket_and_bind(port,&clientFd,&serverFd, &server, &client)>0 ? RECEIVE :OPEN_PORT; 
    return 1;
  case (RECEIVE):
    counter = 0;
    buffer_index = 0;
    *spi_cs = 1;
    size = receive(clientFd, buffer,miso_buffer);
    state = size > 0 ? SETUP_SPI : RECEIVE;
    return 1;

  case (SETUP_SPI):
    *spi_cs = 0;
    *spi_sclk = 1;
    state = SEND_SPI;
    return 1;

  case (SEND_SPI):

    *spi_mosi = buffer[buffer_index] & 1 << (7 - i) ? 1 : 0;

    switch (clock_state) {

    case (CLOCK_LOW):
      *spi_sclk = 0;
      clock_state = 1;
      return 1;

    case (CLOCK_HIGH):
      *spi_sclk = 1;
      clock_state = 0;
      counter = counter + 1;
      miso_buffer[buffer_index] = miso_buffer[buffer_index] << 1 | spi_miso;

      if (i == 7) {
        if (buffer_index == (size - 1)) {
          state = END_SPI;
          return 1;
        } else {
          buffer_index = buffer_index + 1;
          return 1;
        }
      }
      return 1;
    }

  case (END_SPI):
    *spi_cs = 1;
    *spi_sclk = 1;
    state = RECEIVE;
    int sent = send_to_client(clientFd, miso_buffer, size);
    if (sent < size) {
      perror("Couldn't send all miso buffer to client ");
      return -1;
    }
    return 0;
  }
}

int create_socket_and_bind(int port,int* clientFd, int* serverFd, struct sockaddr_in* server, struct sockaddr_in* client ) {
	memset(server, 0, sizeof(*server));

  *serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (*serverFd < 0) {
    perror("Cannot create socket");
   // exit(1);
  }

  server->sin_family = AF_INET;
  server->sin_addr.s_addr = inet_addr("127.0.0.1"); // INADDR_ANY;
  server->sin_port = htons(port);
  int len = sizeof(*server);
  if (bind(*serverFd,(struct sockaddr *)server, len) < 0) {
    perror("Cannot bind socket");
    exit(2);
  }
  if (listen(*serverFd, 10) < 0) {
    perror("Listen error");
    exit(3);
  }

  len = sizeof(*client);
  printf("waiting for clients at port %d \n", port);
  if ((*clientFd = accept(*serverFd, (struct sockaddr *)client, &len)) < 0) {
    perror("accept error");
    exit(4);
  }
  char *client_ip = inet_ntoa(client->sin_addr);
  printf("Accepted new connection from a client %s:%d\n", client_ip,
         ntohs(client->sin_port));

  int flags = fcntl(*clientFd, F_GETFL, 0);
  fcntl(*clientFd, F_SETFL, flags | O_NONBLOCK);

  return 1;
}

int receive(int clientFd, char *buffer, char *miso_buffer) {

  memset(buffer, 0, sizeof(buffer));
  memset(miso_buffer, 0, sizeof(miso_buffer));
  size = read(clientFd, buffer, sizeof(buffer));
  if (size < 0) {
    perror("read error");
    return -1;
  }
  // printf("received %s from client\n", buffer);
  return size;
}

int send_to_client(int clientFd, char *miso_buffer, int size) {
  int sent = write(clientFd, miso_buffer, size);
  if (sent < 0) {
    perror("write error");
    return -1;
  }
  return sent;
}