#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#define UDPPACKETPORT 5005

int main(int ac, char **av)
{
  int sock;
  struct sockaddr_in addr;

  char buf[2048]; // over 1500

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(UDPPACKETPORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(sock, (struct sockaddr *)&addr, sizeof(addr));

  struct timeval lastclock, nextclock;
  gettimeofday(&lastclock, NULL);

  struct sockaddr_in fromaddr;
  int fromaddrsize = sizeof(struct sockaddr);
  char ipaddrbuf[64];

  while(1){
    memset(buf, 0, sizeof(buf));
    recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*) &fromaddr, &fromaddrsize);
    gettimeofday(&nextclock, NULL);
    /*
    long sec = nextclock.tv_sec - lastclock.tv_sec;
    long usec = nextclock.tv_usec - lastclock.tv_usec;
    if(usec<0){
      sec -= 1;
      usec += 1000000;
    } */
    inet_ntop(AF_INET, &(fromaddr.sin_addr), ipaddrbuf, sizeof(ipaddrbuf));
    printf("%s,%s,%ld,%ld,-\n", ipaddrbuf, buf, nextclock.tv_sec, nextclock.tv_usec);//, sec, usec);
    //lastclock = nextclock;
  }

 close(sock);

 return 0;
}
