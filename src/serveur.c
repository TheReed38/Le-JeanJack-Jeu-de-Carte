/*
Code de communication serveur client
0:Début de la partie
1:Indication de début de tour
2:Indication de fin de tour
3:Invocation d'une créature, paramètre : idCarteInvoqué;
4:Attaque d'une créature sur une autre créature, paramètres : idAttaquant et idAttaqué
5:Attaque face, paramètre : pvEnnemi
6:Terrain Joué, paramètres : idTerrain, côtéTerrain (1 pour Ennemi, 0 pour soi);
7:Sort Généraux, paramètre : idSort
8:Sort ciblé, paramètres : idSort, idAttaqué;
9:Fin de partie Victoire !
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Si nous sommes sous Windows
    #include <winsock2.h>
    // typedef, qui nous serviront par la suite
    typedef int socklen_t;

// On inclut les fichiers standards
int main(int argc, char *argv[])
{
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,2), &WSAData);

    //Pour le select
    fd_set readfs;
    int ret;
    char buffer[1024];
    int n;

    SOCKET socketServeur;
    SOCKADDR_IN adressSocketS = {0};
    socketServeur=socket(AF_INET,SOCK_STREAM,0);

    if (socketServeur==INVALID_SOCKET) {
      printf("Echec creation socket Serveur\n");
      exit(EXIT_FAILURE);
    }

    adressSocketS.sin_family=AF_INET;
    adressSocketS.sin_port=htons(45565);
    adressSocketS.sin_addr.s_addr=htonl(INADDR_ANY);//inet_addr("2");

    printf("Socket à l'adresse %s\n",inet_ntoa(adressSocketS.sin_addr));

    if ((bind(socketServeur,(SOCKADDR*)&adressSocketS,sizeof(adressSocketS)))==SOCKET_ERROR) {
      printf("Echec bind socket Serveur\n");
      exit(EXIT_FAILURE);
    }

    SOCKET socketJ1;
    SOCKADDR_IN adressSocketJ1;
    socklen_t adrSockJ1Size=sizeof(adressSocketJ1);

    if (listen(socketServeur,5)) {
      printf("Echec listen socket Serveur\n");
      exit(EXIT_FAILURE);
    }

    socketJ1=accept(socketServeur,(SOCKADDR*)&adressSocketJ1,&adrSockJ1Size);
    printf("Le joueur 1 s'est connecté avec succès avec la socket %d de %s:%d\n", socketJ1, inet_ntoa(adressSocketJ1.sin_addr), htons(adressSocketJ1.sin_port));


    SOCKET socketJ2;
    SOCKADDR_IN adressSocketJ2;
    socklen_t adrSockJ2Size=sizeof(adressSocketJ2);

    if (listen(socketServeur,5)) {
      printf("Echec listen socket Serveur\n");
      exit(EXIT_FAILURE);
    }

    socketJ2=accept(socketServeur,(SOCKADDR*)&adressSocketJ2,&adrSockJ2Size);
    printf("Le joueur 2 s'est connecté avec succès avec la socket %d de %s:%d\n", socketJ2, inet_ntoa(adressSocketJ2.sin_addr), htons(adressSocketJ2.sin_port));



    //Envoi signal début de partie
    send(socketJ1,"0",strlen("0"),0);

    if((n = recv(socketJ1, buffer, sizeof buffer - 1, 0)) < 0) {
      printf("Erreur recv\n");
    }

    send(socketJ2,"0",strlen("0"),0);


    //ATTENTE RECEPTION
    if((n = recv(socketJ2, buffer, sizeof buffer - 1, 0)) < 0) {
      printf("Erreur recv\n");
    }

    send(socketJ1,"/1",strlen("/1"),0);
    send(socketJ2,"/2",strlen("/2"),0);

    int game=1;
    int max;
    if (socketJ1<socketJ2) {
        max=socketJ2;
    }
    else {
        max=socketJ1;
    }
    while (game) {


      FD_ZERO(&readfs);
      FD_SET(socketJ1, &readfs);
      FD_SET(socketJ2, &readfs);
      if((ret = select(max + 1, &readfs, NULL, NULL, NULL)) < 0) {
        printf("Erreur select\n");
      }
      if (FD_ISSET(socketJ1, &readfs)) {
        if((n = recv(socketJ1, buffer, sizeof buffer-1, 0)) < 0) {
          printf("Erreur recv\n");
        }
        buffer[n] = '\0';
        if (!strcmp("2",buffer)) {
            send(socketJ2,"/1",strlen("/1"),0);
        }
        else {
            if (!strcmp("/X",buffer)) {
              game=0;
              printf("Serveur Fermé : Deconnexion d'un des joueurs\n");
            }
            send(socketJ2,buffer,strlen(buffer),0);
        }
      }
      if (FD_ISSET(socketJ2, &readfs)) {
        if((n = recv(socketJ2, buffer, sizeof buffer -1, 0)) < 0) {
          printf("Erreur recv\n");
        }
        buffer[n] = '\0';

        if (!strcmp("2",buffer)) {
            send(socketJ1,"/1",strlen("/1"),0);
        }
        else {
            if (!strcmp("/X",buffer)) {
                game=0;
                printf("Serveur Fermé : Deconnexion d'un des joueurs\n");
            }
            send(socketJ1,buffer,strlen(buffer),0);
        }
      }
    }
    closesocket(socketServeur);
    closesocket(socketJ1);
    closesocket(socketJ2);

    WSACleanup();

    return EXIT_SUCCESS;
}
