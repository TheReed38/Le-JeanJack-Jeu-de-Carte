#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include "fonctions.h"

#if defined (WIN32)
    #include <winsock2.h>
    typedef int socklen_t;
#elif defined (linux)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
#endif

#define PORT 23

void libereTextureLobby(SDL_Texture * bout1,SDL_Texture * TtextBout1,SDL_Texture * boutonRetour) {
    SDL_DestroyTexture(boutonRetour);
    SDL_DestroyTexture(bout1);
    SDL_DestroyTexture(TtextBout1);
}

int lobby(SDL_Window * fen,SDL_Renderer * ren,TTF_Font * dejavu,Mix_Chunk * hover, Mix_Chunk * selectSE) {

  #if defined (WIN32)
      WSADATA WSAData;
      int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
  #else
      int erreur = 0;
  #endif

  int winW;
  int winH;

  SDL_GetWindowSize(fen,&winW,&winH);

  SOCKET sock;
  SOCKADDR_IN sin;

  SDL_Color coulTextBout={255,255,255,255};
/*
  //Joueur perso
  SDL_Texture * joueur=NULL;
  joueur=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,winW/3,winH/8);
  if (joueur==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  SDL_Rect Rjoueur;
  Rjoueur.x=0;
  Rjoueur.y=0;
  Rjoueur.w=winW/3;
  Rjoueur.h=winH/8;
  SDL_SetRenderTarget(ren,joueur);
  if (SDL_SetRenderDrawColor(ren,255,120,0,255)<0) {
      printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  SDL_RenderFillRect(ren,&Rjoueur);
  Rjoueur.x=winW/8;
  Rjoueur.y=(winH*49)/60;
  SDL_SetRenderTarget(ren,NULL);
*/
  //Bouton Retour
  SDL_Texture * boutonRetour = NULL;
  boutonRetour=loadPictures(ren,"image/menu/bouton_retour.png");
  if (boutonRetour==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return 1;
  }
  SDL_Rect RboutonRetour;
  RboutonRetour.x=(winW*52)/60;
  RboutonRetour.y=(winH/30);
  RboutonRetour.w=RboutonRetour.h=(winH*7)/60;

  //Bouton 1 Trouver un serveur
  SDL_Texture * bout1 = NULL;
  SDL_Texture * TtextBout1 = NULL;
  SDL_Surface * textBout1 = NULL;
  textBout1=TTF_RenderText_Solid(dejavu,"Trouver un serveur",coulTextBout);
  if (!textBout1) {
      printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
      exit(2);
  }
  bout1=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,winW/3,winH/8);
  if (bout1==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  TtextBout1=SDL_CreateTextureFromSurface(ren,textBout1);
  SDL_FreeSurface(textBout1);
  if (TtextBout1==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect RBout1;
  SDL_Rect RtextBout1;
  RtextBout1.x=winW/8+winW/60;
  RtextBout1.w=(winW*9)/30;
  RtextBout1.h=winH/10;
  RtextBout1.y=(winH*49)/60+(winH/100);
  RBout1.x=0;
  RBout1.y=0;
  RBout1.w=winW/3;
  RBout1.h=winH/8;

  //Zone d'affichage des decks déjà créés
  SDL_Texture * zoneDecks = NULL;

  zoneDecks=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,(winW*5)/6,(winH*4)/6);
  if (zoneDecks==NULL) {
      SDL_Log("Unable to create texture:%s",SDL_GetError());
      return 1;
  }
  SDL_Rect RzoneDecks;
  RzoneDecks.x=0;
  RzoneDecks.y=0;
  RzoneDecks.w=(winW*5)/6;
  RzoneDecks.h=(winH*4)/6;
  SDL_SetRenderTarget(ren,zoneDecks);
  SDL_RenderFillRect(ren,&RzoneDecks);
  SDL_SetRenderTarget(ren,NULL);
  RzoneDecks.x=winW/12;
  RzoneDecks.y=winH/12;

  //Bouton Deck 1
  SDL_Texture * boutDeck1 = NULL;
  SDL_Texture * textBoutDeck1 = NULL;
  SDL_Surface * StextBoutDeck1 = NULL;
  StextBoutDeck1=TTF_RenderText_Solid(dejavu,"Deck 1",coulTextBout);
  if (!StextBoutDeck1) {
    printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
    exit(2);
  }
  boutDeck1=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,(winW*5)/6,(winH*4)/18);
  if (boutDeck1==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  textBoutDeck1=SDL_CreateTextureFromSurface(ren,StextBoutDeck1);
  SDL_FreeSurface(StextBoutDeck1);
  if (textBoutDeck1==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect RboutDeck1;
  SDL_Rect RtextBoutDeck1;
  SDL_SetRenderTarget(ren,boutDeck1);
  RboutDeck1.x=0;
  RboutDeck1.y=0;
  RboutDeck1.w=(winW*5)/6;
  RboutDeck1.h=(winH*4)/18;
  if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
      printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  SDL_RenderFillRect(ren,&RboutDeck1);
  SDL_SetRenderTarget(ren,NULL);
  RboutDeck1.x=winW/12;
  RboutDeck1.y=winH/12;
  RtextBoutDeck1.x=winW/12+winW/60;
  RtextBoutDeck1.y=winW/12+(winH/100);
  RtextBoutDeck1.w=(winW*5)/6-winW/30;
  RtextBoutDeck1.h=(winH*4)/18-winH/50;

  //Bouton Deck 2
  SDL_Texture * boutDeck2 = NULL;
  SDL_Texture * textBoutDeck2 = NULL;
  SDL_Surface * StextBoutDeck2 = NULL;
  StextBoutDeck2=TTF_RenderText_Solid(dejavu,"Deck 2",coulTextBout);
  if (!StextBoutDeck2) {
    printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
    exit(2);
  }
  boutDeck2=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,(winW*5)/6,(winH*4)/18);
  if (boutDeck2==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  textBoutDeck2=SDL_CreateTextureFromSurface(ren,StextBoutDeck2);
  SDL_FreeSurface(StextBoutDeck2);
  if (textBoutDeck2==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect RboutDeck2;
  SDL_Rect RtextBoutDeck2;
  SDL_SetRenderTarget(ren,boutDeck2);
  RboutDeck2.x=0;
  RboutDeck2.y=0;
  RboutDeck2.w=(winW*5)/6;
  RboutDeck2.h=(winH*4)/18;
  if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
      printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  SDL_RenderFillRect(ren,&RboutDeck2);
  SDL_SetRenderTarget(ren,NULL);
  RboutDeck2.x=winW/12;
  RboutDeck2.y=winH/12+(winH*4)/18;
  RtextBoutDeck2.x=winW/12+winW/60;
  RtextBoutDeck2.y=winW/12+(winH*4)/18+(winH/100);
  RtextBoutDeck2.w=(winW*5)/6-winW/30;
  RtextBoutDeck2.h=(winH*4)/18-winH/50;

  //Bouton Deck 3
  SDL_Texture * boutDeck3 = NULL;
  SDL_Texture * textBoutDeck3 = NULL;
  SDL_Surface * StextBoutDeck3 = NULL;
  StextBoutDeck3=TTF_RenderText_Solid(dejavu,"Deck 3",coulTextBout);
  if (!StextBoutDeck3) {
    printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
    exit(2);
  }
  boutDeck3=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,(winW*5)/6,(winH*4)/18);
  if (boutDeck3==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  textBoutDeck3=SDL_CreateTextureFromSurface(ren,StextBoutDeck3);
  SDL_FreeSurface(StextBoutDeck3);
  if (textBoutDeck3==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect RboutDeck3;
  SDL_Rect RtextBoutDeck3;
  SDL_SetRenderTarget(ren,boutDeck3);
  RboutDeck3.x=0;
  RboutDeck3.y=0;
  RboutDeck3.w=(winW*5)/6;
  RboutDeck3.h=(winH*4)/18;
  if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
      printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  SDL_RenderFillRect(ren,&RboutDeck3);
  SDL_SetRenderTarget(ren,NULL);
  RboutDeck3.x=winW/12;
  RboutDeck3.y=winH/12+(winH*8)/18;
  RtextBoutDeck3.x=winW/12+winW/60;
  RtextBoutDeck3.y=winW/12+(winH*8)/18+(winH/100);
  RtextBoutDeck3.w=(winW*5)/6-winW/30;
  RtextBoutDeck3.h=(winH*4)/18-winH/50;

  //Texte d'attente d'adversaire
  SDL_Texture * TtextAttente = NULL;
  SDL_Surface * textAttente = NULL;
  textAttente=TTF_RenderText_Solid(dejavu,"En attente d'un adversaire ...",coulTextBout);
  if (!textAttente) {
      printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
      exit(2);
  }
  TtextAttente=SDL_CreateTextureFromSurface(ren,textAttente);
  SDL_FreeSurface(textAttente);
  if (TtextAttente==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect RtextAttente;
  RtextAttente.x=winW/8;
  RtextAttente.w=(winW*2)/3;
  RtextAttente.h=winH/10;
  RtextAttente.y=(winH*2)/5;


  //Booléen pour savoir qui est connecté
  int bClientConnect=0;
  int bChoixDeck=0;

  Lcarte deckChoisi=NULL;

  int quitValue;


  SDL_SetRenderTarget(ren,bout1);
  if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
      printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  SDL_RenderFillRect(ren,&RBout1);
  RBout1.x=winW/8;
  RBout1.y=(winH*49)/60;
  SDL_SetRenderTarget(ren,NULL);

  //Boucle d'affichage
  SDL_Point mousePos;
  SDL_Event event;
  char * text;

  //Pour le select
  fd_set readfs;
  int ret;
  char buffer[1024];
  int n;
  int noDeck;

  Lcarte decktmp;

  Lcarte deck1=NULL;
  Lcarte deck2=NULL;
  Lcarte deck3=NULL;
  deck1=loadDeck("deck1",ren,dejavu);
  deck2=loadDeck("deck2",ren,dejavu);
  deck3=loadDeck("deck3",ren,dejavu);
  if (deck3==NULL) {
      if (deck2==NULL) {
          if (deck1==NULL) {
              noDeck=0;
          }
          else {
              noDeck=1;
          }
      }
      else {
        noDeck=2;
      }
  }
  else {
    noDeck=3;
  }

  struct timeval * tv;
  tv=calloc(1,sizeof(*tv));
  tv->tv_sec=0;
  tv->tv_usec=0;

  int soundPlayed = 1;
  Mix_Music * playTheme = NULL;

  playTheme = Mix_LoadMUS("sound/music/battle.wav");

  while (1) {
      while(SDL_PollEvent(&event)) {
          switch(event.type) {

              //Pour quitter
              case SDL_QUIT:
                  return 1;
                  break;

              //En cas de clic
              case SDL_MOUSEBUTTONDOWN:
                  mousePos.x=event.button.x;
                  mousePos.y=event.button.y;
                  if (!bChoixDeck) {
                    if (noDeck>0 && SDL_PointInRect(&mousePos,&RboutDeck1)) {
                        Mix_PlayChannel(-1,selectSE,0);
                        deckChoisi=deck1;
                        bChoixDeck=1;
                    }
                    if (noDeck>1 && SDL_PointInRect(&mousePos,&RboutDeck2)) {
                        Mix_PlayChannel(-1,selectSE,0);
                        deckChoisi=deck2;
                        bChoixDeck=1;
                    }
                    if (noDeck>2 && SDL_PointInRect(&mousePos,&RboutDeck3)) {
                        Mix_PlayChannel(-1,selectSE,0);
                        deckChoisi=deck3;
                        bChoixDeck=1;
                    }
                  }
                  if (SDL_PointInRect(&mousePos,&RboutonRetour)) {
                      Mix_PlayChannel(-1,selectSE,0);
                      libereTextureLobby(bout1,TtextBout1,boutonRetour);
                      return 0;
                  }
                  if (SDL_PointInRect(&mousePos,&RBout1)) {
                      Mix_PlayChannel(-1,selectSE,0);
                      text=textInput("Renseignez l'ip",dejavu);
                      if (text==NULL) {
                          break;
                      }
                      if ( inet_addr(text)==(u_long)(-1)) {
                        printf("ERREUR ADRESSE IP INVALIDE : %s\n",text);
                      }
                      else {
                        printf("Adresse IP : %s\n",text);
                        /* Création de la socket */
                        sock = socket(AF_INET, SOCK_STREAM, 0);

                        /* Configuration de la connexion */
                        sin.sin_addr.s_addr = inet_addr(text);
                        sin.sin_family = AF_INET;
                        sin.sin_port = htons(PORT);

                        /* Si le client arrive à se connecter */
                        if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR) {
                            printf("Connexion à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
                            bClientConnect=1;
                        }
                        else {
                            printf("Impossible de se connecter\n");
                            closesocket(sock);
                        }
                     }
                  }
                  break;

              case SDL_MOUSEMOTION:
                  mousePos.x=event.motion.x;
                  mousePos.y=event.motion.y;

                  //Gonflement du bouton Retour
                  if (SDL_PointInRect(&mousePos,&RboutonRetour)) {
                    if (!soundPlayed) {
                          Mix_PlayChannel(-1,hover,0);
                          soundPlayed=1;
                    }
                    RboutonRetour.x=(winW*52)/60-(winW/120);
                    RboutonRetour.y=(winH/30)-(winH/120);
                    RboutonRetour.w=RboutonRetour.h=(winH*8)/60;
                  }

                  //Gonflement du Bouton 1
                  else if (SDL_PointInRect(&mousePos,&RBout1)) {
                    if (!soundPlayed) {
                          Mix_PlayChannel(-1,hover,0);
                          soundPlayed=1;
                    }
                    RBout1.x=winW/8-(winW/120);
                    RBout1.y=(winH*49)/60-(winH/200);
                    RBout1.w=winW/3+(winW/60);
                    RBout1.h=winH/8+(winH/100);
                    RtextBout1.x=winW/8+winW/60-(winW/120);
                    RtextBout1.w=(winW*9)/30+(winW/60);
                    RtextBout1.h=winH/10+(winH/100);
                    RtextBout1.y=(winH*49)/60+(winH/100)-(winH/200);
                  }
                  else {
                    soundPlayed = 0;
                  }
                  break;

              //Cas général
              default:
                  break;
          }

          //Affichage du menu
            //Couleur du fond
          if (SDL_SetRenderDrawColor(ren,255,0,0,255)<0) {
              printf("Erreur lors du changement de couleur : %s",SDL_GetError());
              return 1;
          }
          SDL_RenderClear(ren);




          if (!bClientConnect) {
            if (!bChoixDeck) {
              SDL_RenderCopy(ren,zoneDecks,NULL,&RzoneDecks);

              if (noDeck>=1) {
                  SDL_RenderCopy(ren,boutDeck1,NULL,&RboutDeck1);
                  SDL_RenderCopy(ren,textBoutDeck1,NULL,&RtextBoutDeck1);
              }
              if (noDeck>=2) {
                  SDL_RenderCopy(ren,boutDeck2,NULL,&RboutDeck2);
                  SDL_RenderCopy(ren,textBoutDeck2,NULL,&RtextBoutDeck2);
              }
              if (noDeck>=3) {
                  SDL_RenderCopy(ren,boutDeck3,NULL,&RboutDeck3);
                  SDL_RenderCopy(ren,textBoutDeck3,NULL,&RtextBoutDeck3);
              }
            }
            else {
              //Bouton Retour
              SDL_RenderCopy(ren,boutonRetour,NULL,&RboutonRetour);

              //Bouton 1
              SDL_RenderCopy(ren,bout1,NULL,&RBout1);
              SDL_RenderCopy(ren,TtextBout1,NULL,&RtextBout1);
            }
          }
          else {
              SDL_RenderCopy(ren,TtextAttente,NULL,&RtextAttente);
          }

          //Rendu
          SDL_RenderPresent(ren);

          //Rafraichissement des textures en cas de modifications de taille de la fenêtre
          SDL_GetWindowSize(fen,&winW,&winH);

          //Bouton retour
          RboutonRetour.x=(winW*52)/60;
          RboutonRetour.y=(winH/30);
          RboutonRetour.w=RboutonRetour.h=(winH*7)/60;
          //Bouton 1
          RBout1.x=winW/8;
          RBout1.y=(winH*49)/60;
          RBout1.w=winW/3;
          RBout1.h=winH/8;
          RtextBout1.x=winW/8+winW/60;
          RtextBout1.w=(winW*9)/30;
          RtextBout1.h=winH/10;
          RtextBout1.y=(winH*49)/60+(winH/100);
          RtextAttente.x=winW/8;
          RtextAttente.w=(winW*2)/3;
          RtextAttente.h=winH/10;
          RtextAttente.y=(winH*2)/5;

          //Zone decks
          RzoneDecks.x=winW/12;
          RzoneDecks.y=winH/12;
          RzoneDecks.w=(winW*5)/6;
          RzoneDecks.h=(winH*4)/6;
          //Bouton deck 1
          RboutDeck1.w=(winW*5)/6;
          RboutDeck1.h=(winH*4)/18;
          RboutDeck1.x=winW/12;
          RboutDeck1.y=winH/12;
          RtextBoutDeck1.x=winW/12+winW/60;
          RtextBoutDeck1.y=winW/12+(winH/100);
          RtextBoutDeck1.w=(winW*5)/6-winW/30;
          RtextBoutDeck1.h=(winH*4)/18-winH/50;
          //Bouton deck 2
          RboutDeck2.w=(winW*5)/6;
          RboutDeck2.h=(winH*4)/18;
          RboutDeck2.x=winW/12;
          RboutDeck2.y=winH/12+(winH*4)/18;
          RtextBoutDeck2.x=winW/12+winW/60;
          RtextBoutDeck2.y=winW/12+(winH*4)/18+(winH/100);
          RtextBoutDeck2.w=(winW*5)/6-winW/30;
          RtextBoutDeck2.h=(winH*4)/18-winH/50;
          //Bouton deck 3
          RboutDeck3.w=(winW*5)/6;
          RboutDeck3.h=(winH*4)/18;
          RboutDeck3.x=winW/12;
          RboutDeck3.y=winH/12+(winH*8)/18;
          RtextBoutDeck3.x=winW/12+winW/60;
          RtextBoutDeck3.y=winW/12+(winH*8)/18+(winH/100);
          RtextBoutDeck3.w=(winW*5)/6-winW/30;
          RtextBoutDeck3.h=(winH*4)/18-winH/50;

          if (bClientConnect) {
              RboutonRetour.w=RboutonRetour.h=RBout1.w=RBout1.h=RtextBout1.w=RtextBout1.h=0;
          }

          //Select
          if (bClientConnect) {
              FD_ZERO(&readfs);
              FD_SET(sock, &readfs);
              if((ret = select(sock + 1, &readfs, NULL, NULL, tv)) < 0) {
                printf("Erreur select\n");
              }
              if (FD_ISSET(sock, &readfs)) {
                if((n = recv(sock, buffer, sizeof buffer -1 , 0)) < 0) {
                  printf("Erreur recv\n");
                }


                buffer[n] = '\0';
                printf("%s",buffer);
                if (!strcmp("0",buffer)) {
                    printf("\nLe jeu peut commencer\n");
                    send(sock,"0 ",strlen("0"),0);
                    if( Mix_PlayMusic( playTheme, -1 ) == -1 ) {
                        return 1;
                      }
                    quitValue = jeu(sock,deckChoisi,fen,ren,dejavu);
                    bClientConnect=0;
                    if (quitValue==1) {
                      SDL_Event sdlevent;
                      sdlevent.type = SDL_QUIT;
                      SDL_PushEvent(&sdlevent);
                    }
                    closesocket(sock);
                }
              }
          }
      }
  }

  #if defined (WIN32)
      WSACleanup();
  #endif
}
