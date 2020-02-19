#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int ac, char** av) {
  int hostcount = ac-1;
  if(hostcount==0){
    fprintf(stderr, "udp-sender target\n");
    return 255;
  }

  int sock[hostcount];
  struct sockaddr_in addr[hostcount];
  for(int i=0;i<hostcount;i++){
    sock[i] = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock<0){
      fprintf(stderr, "%s\n", strerror(errno));
      return 255;
    }
    addr[i].sin_family = AF_INET;
    addr[i].sin_port = htons(5005);
    addr[i].sin_addr.s_addr = inet_addr(av[i+1]);
  }

  struct timeval curtime;
  gettimeofday(&curtime, NULL);

  char msg[64];

  struct timespec tsec = { 0, (1000000 - curtime.tv_usec) * 1000 };
  nanosleep(&tsec, NULL);

  gettimeofday(&curtime, NULL);
  snprintf(msg, 64, "%ld:%ld", curtime.tv_sec, curtime.tv_usec);
  for(int i=0;i<hostcount; i++){
    sendto(sock[i], msg, strlen(msg), 0, (struct sockaddr*) &addr[i], sizeof(addr[i]));
  }
  while(1) {
    gettimeofday(&curtime, NULL);
    tsec.tv_sec = 0;
    tsec.tv_nsec =  (1000000 - curtime.tv_usec) * 1000;
    nanosleep(&tsec, NULL);
  
    gettimeofday(&curtime, NULL);
    snprintf(msg, 64, "%ld:%ld", curtime.tv_sec, curtime.tv_usec);
    for(int i=0;i<hostcount; i++){
      sendto(sock[i], msg, strlen(msg), 0, (struct sockaddr*) &addr[i], sizeof(addr[i]));
    }
  }

  for(int i=0;i>hostcount; i++) close(sock[i]);
  return 0;
}
