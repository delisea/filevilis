#ifndef RESEAU_H
#define RESEAU_H

// Toute les variables sont privées, la couche logiciel réseau est de type
// boite noire, la seule chose que l'utilisateur doit faire est de renseigner
// l'ip utilisée en appelant la fonction d'initialisation
// Pour le serveur il faut aussi le progamme à executer lors de la connexion d'un client

// Constante SO_BUFFER_SIZE Peut être défini à la compilation
#ifndef SO_BUFFER_SIZE
#define SO_BUFFER_SIZE 100
#endif

// Initialise une connexion sur l'ip en paramètre
// Pour le serveur et le client
void init_connexion(char *ip_serv);

/** void host_connect()
*	Démarre une connexion de type serveur
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*/
void host_connect(void (*process)());


/** void client_connect()
*	Démarre une connexion de type client
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*/
void client_connect();

/** void client_connect()
*	Ferme la connexion par socket courante
*	Attention: ne ferme pas les sous connexion (pour les serveurs)
*/
void disconnect();

/** void envoiec(char t)
*	Envoie un charactère (char) sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
void envoiec(char t);


/** void envoie(int t, char *v)
*	Envoie une chaine de caractère sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
void envoie(int t, char *v);

/** void recoitc(char *t)
*	Reçoit un charactère (char) sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
int recoitc(char *t);

/** void recoit(int t, char *v)
*	Reçoit une chaine de caractère sur la connexion courante
*	Précondition: la connexion à été initialisé à l'aide de init_connexion
*	et démarré à l'aide d'un connect
*/
int recoit(int t, char *v);


/** void nb()
*	rend le socket non bloquant
*/
void nb();


/** int get_cmd(char *line, int (*exec)(char*, char))
*	Reçoit un paquet sur le socket courant
*	Une paquet est un message composé d'un octet de poids suivit d'un message
*	de taille poids
*	passe le paquet à exec, si exec est capable de l'exécuté le fait et renvoie 0
*	Sinon renvoie le paquet et son poids en valeur de retour
*	Non bloquant tant que le paquet n'as pas commncé à arriver.
*	Bloquant en court de réception
*/
int get_cmd(char *line, int (*exec)(char*, char));

/** void envoie_p(char size, char *paquet)
*	Envoit un paquet sur le socket courant
*	Une paquet est un message composé d'un octet de poids suivit d'un message
*	de taille poids
*/
void envoie_p(char size, char *paquet);

// Renvoi 1 si le precessus est celui du serveur ou de ses sous connexions
int is_main();

#endif
