#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "resrever.h"

#define BUFLEN 256

static char recv[BUFLEN];

int main() {
  int fd;

  #define DEV "/dev/" TQ84_DEVICE_NAME
  
  fd = open(DEV, O_RDWR);

  
  char *send = "esaelp ,txet siht esreveR";

  if (fd < 0) {
    perror("Could not open " DEV);
    exit (1);
  }

  if (write(fd, send, strlen(send)) < 0) {
    perror("Could not write send\n");
    exit (2);
  }

  if (read(fd, recv, BUFLEN) < 0) {
    perror("Could ot read recv\n");
    exit (3);
  }

  printf("I have received: %s\n", recv);
}
