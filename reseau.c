#include "reseau.h"
#include "fon.h"

/* Pour les constantes EXIT_SUCCESS et EXIT_FAILURE */
#include <stdlib.h>
/* Pour fork() */
#include <unistd.h>
/* Pour perror() et errno */
#include <errno.h>
// Lib pour rendre les flux non bloquant
#include <fcntl.h>

#include <stdint.h>

#include <sys/stat.h>

#include <sys/sendfile.h>

char _buffer[SO_BUFFER_SIZE];
int _SO_length = 0;
int _is_main = 1;

// Renvoi 1 si le precessus est celui du serveur ou de ses sous connexions
int is_main()
{
	return _is_main;
}

// Toute les variables sont privées, la couche logiciel réseau est de type
// boite noire, la seule chose que l'utilisateur doit faire est de renseigner
// l'ip utilisée en appelant la fonction d'initialisation
// Pour le serveur il faut aussi le progamme à executer lors de la connexion d'un client

// Port Utilisé part défaut par l'application
#define N_PORT "1111"

// Identifiant de la connexion courante
int sock;
struct sockaddr_in sad, cad;

// Initialise une connexion sur l'ip en paramètre
// Pour le serveur et le client
void init_connexion(char *ip_serv)
{
	// On créer la socket
	sock = h_socket(AF_INET, SOCK_STREAM);
	// Renseignement de la structure sockaddr_inpar l'adresse et le port locaux
	// On utise le protocole tcp, on ne veut pas perdre de paquet (paquet non périssable)
	// et le trafique de l'application est faible
	adr_socket(N_PORT, ip_serv, "tcp", &sad);
}

/** void host_connect()
*	Démarre une connexion de type serveur
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*/
void host_connect(void (*process)())
{
	// socket enfant, sert aux sous programme pour discuter spécifiquement
	// avec leur client
	int disc;
	// variable utilisée pour différencier le serveur de ces sous processus
	pid_t pid;
	
	// Lancement du serveur
	h_bind(sock, &sad);
	h_listen(sock, 5);
	
	// Ce serveur ne dispose pas de bouton off, pour celà il faut fermer le
	// programme
	while(1)
	{
		// Récupération d'une demande de connexion
		disc = h_accept(sock, &cad);
		
		// Création d'un processus dédié
		do {
			pid = fork();
		} while ((pid == -1) && (errno == EAGAIN));
		
		// Si la création à échoué on arrête le programme
		if(pid==-1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		
		// Si on est dans le processus fils
		if(pid==0)
		{
			_is_main = 0;
			
			// Le socket de transmition dédié pour ce processus est "disc"
			sock = disc;
			
			// On lance le programme de traitement
			(*process)();
			
			// Fin de transmition on ferme le socket dédié
			disconnect();
			
			// Fin de partie on ferme le programme
			exit(EXIT_SUCCESS);
		
		}
		else
		{
			h_close(disc);
		}
		// Si on est le serveur on reste en attente de connexion
	}
}


/** void client_connect()
*	Démarre une connexion de type client
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*/
void client_connect()
{
	h_connect(sock, &sad);
}

/** void client_connect()
*	Ferme la connexion par socket courante
*	Attention: ne ferme pas les sous connexion (pour les serveurs)
*/
void disconnect()
{
	h_close(sock);
}

/** void envoiec(char t)
*	Envoie un charactère (char) sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
void envoiec(char t)
{
	h_writes(sock, &t, 1);
}


/** void envoie(int t, char *v)
*	Envoie une chaine de caractère sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
void envoie(int t, char *v)
{
	h_writes(sock, v, t);
}

/** void envoie(int t, char *v)
*	Envoie une chaine de caractère sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
void envoief(int file)
{
	printf("1\n");
  struct stat fileStat;
  uint32_t size;
  if(fstat(file,&fileStat) < 0)    
    return ;
  size = fileStat.st_size;
  envoie(4,(char*)&size);
  	printf("%i\n",(int) size);
  sendfile(sock, file, NULL, (int) size);
}

/** void recoitc(char *t)
*	Reçoit un charactère (char) sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
int recoitc(char *v)
{
	return recoit(1, v);
}

/** int recoit(int t, char *v)
*	Reçoit une chaine de caractère sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
int recoit(int t, char *v)
{
	int i;
	if(_SO_length < t)
	{
		i = read(sock, _buffer+_SO_length, t-_SO_length);
		
		// Pas de nouvelles données
		if(i < 0 && errno == EAGAIN)
			return 0;
		// i nouveaux octets
		else if(i >= 0)
			_SO_length += i;
		// Erreurs de lecture
		else
			exit(99);
	}
	
	// Si le message est entier on le libère
	if(_SO_length == t && v!=0)
	{
		for(i=0; i<t; i++)
			*(v+i) = _buffer[i];
		_SO_length = 0;
		return t;
	}
	else if(_SO_length == t)
	{
		_SO_length = 0;
		return t;
	}
	
	return 0;
}


/** void nb()
*	rend le socket non bloquant
*/
void nb()
{
	int flags; 


	/* Set socket to non-blocking */ 
	
	if ((flags = fcntl(sock, F_GETFL, 0)) < 0) 
	{ 
	    /* Handle error */ 
	}
			
	if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) 
	{ 
	    printf("fail\n");
	} 
}

/** int get_cmd(char *line, int (*exec)(char*, char))
*	Reçoit un paquet sur le socket courant
*	Une paquet est un message composé d'un octet de poids suivit d'un message
*	de taille poids
*	passe le paquet à exec, si exec est capable de l'exécuté le fait et renvoie 0
*	Sinon renvoie le paquet et son poids en valeur de retour
*	Non bloquant tant que le paquet n'as pas commncé à arriver.
*	Bloquant en court de réception
*/
int get_cmd(char *line, int (*exec)(char*, char))
{
	int b = 0;
	char taille;
	
	b = recoit(1, &taille);
	if(b)
	{
		taille -= 20;
		// On devient bloquant tant qu'on a pas reçut le message entier
		while(!(b = recoit(taille, 0)));
		if(!exec(_buffer, taille)) return 0;
		for(b=0; b<taille; b++)
			*(line+b) = _buffer[b];
	}
	return b;
}

/** void envoie_p(char size, char *paquet)
*	Envoit un paquet sur le socket courant
*	Une paquet est un message composé d'un octet de poids suivit d'un message
*	de taille poids
*/
void envoie_p(char size, char *paquet)
{
	char l = size + 20;
	envoie(1, &l);
	envoie(size, paquet);
}
