#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<time.h>
#include<signal.h>
#include <arpa/inet.h> 
#include <netdb.h> 
#include <netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>

//Il faut exécuter avec:
//./Serveur

//Message
//Client1 -> fils 1 -> Serveur -> fils 2 -> client 2

//Client crée des fils à la connexion
//Les fils lisent les sockets
//Les fils envoient le message au serveurs par des tuples
//Le serveur renvoie le message par un autre tuple

#define BUFFER_SIZE 256

int cree_socket_tcp_ip() { 
	int sock; 
	struct sockaddr_in adresse; 
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		fprintf(stderr, "Erreur socket\n"); return -1; 
	} 
	
	memset(&adresse, 0, sizeof(struct sockaddr_in));
	adresse.sin_family = AF_INET; 
	// donner un numro de port disponible quelconque 
	adresse.sin_port = htons(2018);
 	// aucun contrle sur l’adresse IP : 
	adresse.sin_addr.s_addr = htons(INADDR_ANY); 
 	
 	// Autre exemple : 
 	// connexion sur le port 33016 fix 
 	// adresse.sin_port = htons(33016);
  	// depuis localhost seulement :
   	// inet_aton("127.0.0.1", &adresse.sin_addr);

	if (bind(sock, (struct sockaddr*) &adresse, sizeof(struct sockaddr_in)) < 0) { 
		close(sock);
	 	fprintf(stderr, "Erreur bind\n"); 
	 	return -1; 
	} 
	return sock; 
}

int affiche_adresse_socket(int sock) {
	struct sockaddr_in adresse;
	socklen_t longueur;
	longueur = sizeof(struct sockaddr_in);
	 
	if (getsockname(sock, (struct sockaddr*)&adresse, &longueur) < 0) {
		fprintf(stderr, "Erreur getsockname\n");
	    return -1;
	} 
	printf("IP = %s, Port = %u\n", inet_ntoa(adresse.sin_addr), ntohs(adresse.sin_port));
	return 0; 
}

/*
int traite_connection(int sock,int taba[5][2],int tabb[5][2],int j){
	char message[100];
	close(taba[j][0]);  //ecriture
	close(tabb[j][1]);  //lecture
	read(sock,message,100*sizeof (char));
	write(taba[j][1],message,100*sizeof (char));

	read(tabb[j][0],taba[j][0],j*sizeof(int));
	write(sock,taba[j][0],j*sizeof(int));
	
}
*/

int main(void)
{
	int tab1[5][2];
	int tab2[5][2];
	char message[256];
	int i=0;
	int sock_contact; 
	int sock_connectee; 
	struct sockaddr_in adresse; 
	socklen_t longueur; 
	pid_t pid_fils; 
	sock_contact = cree_socket_tcp_ip(); 

	if (sock_contact < 0){
		return -1;
	}

	listen(sock_contact, 5);
	printf("Mon adresse (sock contact) -> ");
	affiche_adresse_socket(sock_contact);

	while (1){ 
		longueur = sizeof(struct sockaddr_in);
	     sock_connectee = accept(sock_contact, (struct sockaddr*)&adresse, &longueur);

		if (sock_connectee < 0) { 
			fprintf(stderr, "Erreur accept\n");
 			return -1;
  		} 
  		
  		pipe(tab1[i]);
  		pipe(tab2[i]);

  		pid_fils = fork();

   		if (pid_fils == -1){ 
   			fprintf(stderr, "Erreur fork\n"); 
    		return -1;
    	} 

    	if (pid_fils == 0) {
    	/* fils */ 
    		{ 
    		close(sock_contact);
    		close(tab1[i][0]);  //ecriture
			close(tab2[i][1]);  //lecture
    	 	//traite_connection(sock_connectee,tab1,tab2,i);
    	 	read(sock_connectee,&tab2[i][0],i*sizeof (int));
    	 	write();
    	  	exit(0);
    	  	
    		}
    	}
    	else{ 
    	 	close(sock_connectee);
    	 	close(tab2[i][0]);  //lecture
    		close(tab1[i][1]);  //ecriture

    		read(tab2[i][1],&tab1[i][1],i*sizeof(int));
    		write(tab1[i][0],&tab2[i][0],i*sizeof (int));
    		i++;
    		oke; on fait des tests.
    		
    	}


    }
	return 0;
}