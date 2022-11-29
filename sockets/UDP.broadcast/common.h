#ifndef _COMMON_H_
#define _COMMON_H_

#define PORT 65000
#define BUFLEN 512

struct packet
{
	int serverid;
	char servername[64];
	char timenow[64];
};
#endif