#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

int main(int argc, char* argv[])
{
  struct sockaddr_in si_me, si_other;
  int s, slen=sizeof(si_other);
  char cmdbuf[BUFLEN];
  int len;
  time_t tmt;
  struct packet p;
  int broadcast =1;
  
  if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("socket creation failed");
    exit(1);
  }

  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if(setsockopt(s,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof broadcast) == -1) {
    perror("setsockopt (SO_BROADCAST)");
    exit(1);
  }
  
  if(bind(s, (struct sockaddr*)&si_me, sizeof(si_me))==-1) {
    perror("bind failed");
    exit(1);
  }
  
  while(1) {
    if(recvfrom(s, cmdbuf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)==-1) {
      perror("recvfrom() failed");
      exit(1);
    }
    if(!strcmp(cmdbuf,"quit")) {
      printf("user wants to quit\n");
      break;
    }
    p.serverid = 1729;
    strcpy(p.servername,"fictious server");
    tmt = time(NULL);
    strcpy(p.timenow, ctime(&tmt));
    p.timenow[strlen(p.timenow)-1] = '\0';
    if(sendto(s, (struct packet *)&p, sizeof(struct packet), 0, (struct sockaddr*)&si_other, slen)==-1) {
      perror("sendto() failed");
      exit(1);
    }
  }
  close(s);
  return 0;
}