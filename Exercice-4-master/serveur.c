/* 
*  Author: SOSSOU A.Daniel
*  
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MY_ADDR "127.0.0.1"
#define MY_PORT 6005
#define LISTEN_BACKLOG 50

int main(int argc, char *argv[]){
   int sfd; // socket du serveur
   int cfd; // socket du client
   struct sockaddr_in my_addr; // socket addr du serveur 
   struct sockaddr_in peer_addr; // socket addr d'un client
   FILE* fichier = NULL; //Le nom du fichier à recevoir

   socklen_t peer_addr_size; // la taille du sock
   pid_t child;

   char buffer[10]; // pour recevoir la salutation du client
   char buf[1];//Pour lire le fichier envoyer

   sfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sfd < 0){ 
      printf("Le SE n'a pas pu créer la socket %d\n", sfd);
      exit(-1);
   }

   
   memset(&my_addr, 0, sizeof(struct sockaddr_in));
   
    
   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons (MY_PORT);
   inet_aton(MY_ADDR, (struct in_addr *)&my_addr.sin_addr.s_addr);
  
   
   if (bind(sfd, (struct sockaddr *) &my_addr,
                   sizeof(struct sockaddr_in)) < 0){
       printf("bind error\n"); // l'association a echouée
       exit(-1);
  }

   
   if (listen(sfd, LISTEN_BACKLOG) < -1)
               perror("listen\n");

   // on se bloque en attendant les connexion des clients
   peer_addr_size = sizeof(struct sockaddr_in);
   while(1){
         cfd = accept(sfd, (struct sockaddr *) &peer_addr,
                   &peer_addr_size);  
        if (cfd < 0){
            perror("accept\n");
            exit(-1);
        }
        /* Nous créons un fils pour gérer ce client */
        child = fork();
        if(child < 0){ // le fils n'a pas pu être créé
           perror("errreur de création du fils\n");
        }
        if(child==0){
             /* Nous sommes dans le fils nous attendons la requête du client */
             printf("identité du client %d\n", peer_addr.sin_port);
             /*Lecture des donnees envoyées par le client*/
             
             
             read(cfd, buffer, sizeof(buffer));
             
             fichier = fopen(buffer,"w");
             if(fichier==NULL){
                printf("Une erreur s'est produite lors de l'ouverture du fichier");
                exit(3);
             }
             else{
                printf("Fichier ouvert\n");
                while(read(cfd, buf, sizeof(buf))){
                    fwrite(buf,sizeof(buf),1,fichier );
               }
               printf("Fin d'ecriture du fichier\n");
               fclose(fichier);
               /*Fin du traitement le fils se termine*/
               
             }
            close(sfd);
            break;
             
        }

        else{
          /*Dans le père: le père attent un autre client*/
          close(cfd);
        }
  }
}
