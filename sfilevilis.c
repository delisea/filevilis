#include "reseau.h"
#include <stdio.h>
#include <stdint.h>

#define NMAX 10;

void ftp_session()
{
	char str[NMAX] = "";
	int t=0;
	printf("Client connected.\n");
	envoie(11, "Gros penis");
	while(recoit(1, str))
	{
		switch(str[0])
		{
		case 1:
			printf("get\n");
		break;
		case 2:
			printf("set\n");
			t = recoit(1,str);
			while(t>NMAX){
				recoit(NMAX,str);
				printf("%s",str[NMAX]);
				t -= NMAX;
			}
			if(t!=0){
				recoit(t, str);
				printf("%s",str[t]);
			}
		break;
		case 3:
			printf("ls\n");
			envoie(2, "l");
		}
	}
	printf("quit\n");
	disconnect();
}

int main()
{
	char ip[20] = "";
	printf("ip?\n");
	scanf("%s", ip);
	init_connexion(ip);
	host_connect(&ftp_session);
	return 0;
}