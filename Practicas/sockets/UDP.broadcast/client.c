#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

int main(int argc, char* argv[])
{
  struct sockaddr_in si_me, si_other;
  int s, slen=sizeof(si_other);
  char buf[BUFLEN];
  int choice=0;
  struct packet p;
  int broadcast =1;
  
  if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("socket creation failed");
    exit(1);
  }

  memset((char *) &si_other, 0, sizeof(si_me));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  si_other.sin_addr.s_addr = htonl(INADDR_ANY);

  if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
    perror("setsockopt (SO_BROADCAST)");
    exit(1);
  }

  while(1) {
    printf("Enter a number ( enter 0 to quit) :");
    scanf("%d",&choice);
    if(choice == 0) {
      sprintf(buf, "quit");
      if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, slen)==-1) {
	perror("sendto() failed");
	exit(1);
      }
      break;
    }
    printf("\nSending request\n");
    sprintf(buf, "sendmeserverinfo");
    if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, slen)==-1) {
      perror("sendto() failed");
      exit(1);
    }
    if(recvfrom(s, &p, sizeof(struct packet), 0, (struct sockaddr*)&si_other, &slen)==-1) {
      perror("recvfrom() failed");
      exit(1);
    }
    printf("\nserverid=%d;\nservername=%s;\nservertime=%s;\nip=%s;\nport=%d\n",
	   p.serverid,p.servername,p.timenow,
	   inet_ntoa(si_other.sin_addr),ntohs(si_other.sin_port));
  }
  close(s);
  return 0;
}