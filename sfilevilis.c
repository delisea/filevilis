#include "reseau.h"
#include <stdio.h>

void ftp_session()
{
	char str[50] = "";
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
	init_connexion("127.0.0.1");
	host_connect(&ftp_session);
	return 0;
}