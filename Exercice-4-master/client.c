/* 
*  Author : SOSSOU A.Daniel
*  
*/
#include <sys/socket.h>
#include <netinet/in.h>
 #include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define GREETING "BONJOUR SERVEUR"

int main(int argc, char *argv[]){
   int cfd; 
   int success; 
   int port;
   char *element;
   long fileSize;
   FILE* fichier = NULL;
   char buffer[1];
   struct sockaddr_in srv_addr;

   if(argc!=4){
     printf("USAGE: ./client <serveur_ip_addr> <serveur_port> <file_name>\n");
     exit(-1);
   }

   cfd = socket(AF_INET, SOCK_STREAM, 0); // création de la socket
   if (cfd < 0){ // Une erreur s'est produite la socket n'a pas pu être créer
      printf("Le SE n'a pas pu créer la socket %d\n", cfd);
      exit(-1);
   }
   
   
   port = atoi(argv[2]);

   srv_addr.sin_family = AF_INET;
   srv_addr.sin_port = htons (port);
   inet_aton(argv[1], (struct in_addr *)&srv_addr.sin_addr.s_addr);

   
   success = connect(cfd, (struct sockaddr *) &srv_addr,
                 sizeof(struct sockaddr_in));
   if(success < 0){ 
      printf("Impossible de se connecter au serveur %s:%d error %d\n", 
               argv[1], port, success);
      exit(-1);
   }
   
   
   send(cfd, argv[3], sizeof(argv[3]),0);
   sleep(5);
   fichier = fopen(argv[3],"r");
   if(fichier==NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(3);
   }
   else{
        printf("Fichier bien ouvert\n");
        printf("Envoi du fichier %s\n",argv[3]);
        while (fread(buffer,sizeof(buffer), 1,fichier),!feof(fichier)) {
			 send(cfd,buffer, sizeof(buffer), 0);
	   }
   }
  
   
   fclose(fichier);
   close(cfd);
   return(0);
}

