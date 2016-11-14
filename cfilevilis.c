#include "reseau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char ip[20] = "";
	printf("ip?\n");
	scanf("%s", ip);
	init_connexion(ip);
	client_connect();
	printf("perhaps connected\n");
	char str[50] = "";
	char c;
	recoit(11, str);
	printf("%s\n",str);
	do {
		printf("command:\n");
		scanf("%s", str);
		if(!strcmp("get", str))
			 c = 1;
		else if(!strcmp("set", str))
			 c = 2;
		else if(!strcmp("ls", str))
			 c = 3;
		else if(!strcmp("quit", str))
			 c = 0;
		else
			 c = -1;
		
		if(c != -1)
			envoie(1, &c);
		switch(c)
		{
		case 3:
			recoit(2, str);
			printf("%s\n",str);
		}
	} while(c != 0);
	printf("quit\n");
	disconnect();
	return 0;
}