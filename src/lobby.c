#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include "fonctions.h"

#include <winsock2.h>
typedef int socklen_t;

extern Mix_Music *playTheme;
extern Mix_Chunk *hover;
extern Mix_Chunk *selectSE;
extern SDL_Window *win;
extern int winW;
extern int winH;
extern SDL_Renderer *ren;
extern TTF_Font *dejavu;
extern SDL_Color *coulTextBout;
extern SDL_Color *coulFondBout;
extern SDL_Color *coulFondDeck;
extern SDL_Color *coulFondEcran;

void refreshTextureSizesLobby(SDL_Rect *RBoutonTrouverUnServeur,
                              SDL_Rect *RBoutonRetour,
                              SDL_Rect *RZoneDecks,
                              SDL_Rect *RBoutonDeck1,
                              SDL_Rect *RBoutonDeck2,
                              SDL_Rect *RBoutonDeck3,
                              SDL_Rect *RTexteAttente)
{
  *RBoutonTrouverUnServeur = initRect(winW / 8, (winH * 49) / 60, winW / 3, winH / 8);
  *RZoneDecks = initRect(winW / 12, winH / 12, (winW * 5) / 6, (winH * 4) / 6);
  *RBoutonDeck1 = initRect(winW / 12, winH / 12, (winW * 5) / 6, (winH * 4) / 18);
  *RBoutonDeck2 = initRect(winW / 12, winH / 12 + (winH * 4) / 18, (winW * 5) / 6, (winH * 4) / 18);
  *RBoutonDeck3 = initRect(winW / 12, winH / 12 + (winH * 8) / 18, (winW * 5) / 6, (winH * 4) / 18);
  *RTexteAttente = initRect(winW / 8, (winH * 2) / 5, (winW * 2) / 3, winH / 10);
  *RBoutonRetour = initRect((winW * 52) / 60, (winH / 30), (winH * 7) / 60, (winH * 7) / 60);
}

void libereTextureLobby(SDL_Texture *boutonTrouverUnServeur,
                        SDL_Texture *boutonRetour,
                        SDL_Texture *zoneDecks,
                        SDL_Texture *boutonDeck1,
                        SDL_Texture *boutonDeck2,
                        SDL_Texture *boutonDeck3,
                        SDL_Texture *texteAttente)
{
  SDL_DestroyTexture(boutonRetour);
  SDL_DestroyTexture(boutonTrouverUnServeur);
  SDL_DestroyTexture(zoneDecks);
  SDL_DestroyTexture(boutonDeck1);
  SDL_DestroyTexture(boutonDeck2);
  SDL_DestroyTexture(boutonDeck3);
  SDL_DestroyTexture(texteAttente);
}

int lobby()
{
  WSADATA WSAData;
  int erreur = WSAStartup(MAKEWORD(2, 2), &WSAData);
  SOCKET sock;
  SOCKADDR_IN sin;

   int noDeck;
  Deck deck1,deck2,deck3;
  deck1 = loadDeck("deck1");
  deck2 = loadDeck("deck2");
  deck3 = loadDeck("deck3");
  noDeck = (deck1.deckList != NULL) + (deck2.deckList != NULL) + (deck3.deckList != NULL);

  SDL_Rect RBoutonRetour;
  SDL_Rect RBoutonTrouverUnServeur;
  SDL_Rect RZoneDecks;
  SDL_Rect RBoutonDeck1;
  SDL_Rect RBoutonDeck2;
  SDL_Rect RBoutonDeck3;
  SDL_Rect RTexteAttente;
  refreshTextureSizesLobby(&RBoutonTrouverUnServeur, &RBoutonRetour, &RZoneDecks, &RBoutonDeck1, &RBoutonDeck2, &RBoutonDeck3, &RTexteAttente);

  SDL_Texture *boutonRetour = NULL;
  boutonRetour = loadPictures("image/menu/bouton_retour.png");
  if (boutonRetour == NULL)
  {
    SDL_Log("Unable to create texture from image :%s", SDL_GetError());
    return 1;
  }

  SDL_Texture *boutonTrouverUnServeur = createBouton(RBoutonTrouverUnServeur.w, RBoutonTrouverUnServeur.h, coulFondBout, coulTextBout, "Trouver un serveur");
  SDL_Texture *zoneDecks = createBouton(RZoneDecks.w, RZoneDecks.h, coulFondDeck, coulFondDeck, "Bush did 9 11");
  SDL_Texture *boutonDeck1 = createBouton(RBoutonDeck1.w, RBoutonDeck1.h, coulFondBout, coulTextBout, deck1.nom);
  SDL_Texture *boutonDeck2 = createBouton(RBoutonDeck2.w, RBoutonDeck2.h, coulFondBout, coulTextBout, deck2.nom);
  SDL_Texture *boutonDeck3 = createBouton(RBoutonDeck3.w, RBoutonDeck3.h, coulFondBout, coulTextBout, deck3.nom);
  SDL_Texture *texteAttente = createBouton(RTexteAttente.w, RTexteAttente.h, coulFondDeck, coulTextBout, "En attente d'un adversaire ...");

  //Booléen pour savoir qui est connecté
  int playerIsConnected = 0;
  int deckIsChosen = 0;

  //Pour le select
  fd_set readfs;
  int ret;
  char buffer[1024];
  int n;
  struct timeval *tv;
  tv = calloc(1, sizeof(*tv));
  tv->tv_sec = 0;
  tv->tv_usec = 0;
  char *ipAddress;

 

  //Boucle d'affichage
  SDL_Point mousePos;
  SDL_Event event;

  int soundPlayed = 0;
  LCarte deckChoisi = NULL;
  int quitValue;

  while (1)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {

      //Pour quitter
      case SDL_QUIT:
        libereTextureLobby(boutonTrouverUnServeur, boutonRetour, zoneDecks, boutonDeck1, boutonDeck2, boutonDeck3, texteAttente);
        return 1;
        break;

      //En cas de clic
      case SDL_MOUSEBUTTONDOWN:
        mousePos.x = event.button.x;
        mousePos.y = event.button.y;

        if (SDL_PointInRect(&mousePos, &RBoutonRetour))
        {
          Mix_PlayChannel(-1, selectSE, 0);
          libereTextureLobby(boutonTrouverUnServeur, boutonRetour, zoneDecks, boutonDeck1, boutonDeck2, boutonDeck3, texteAttente);
          return 0;
        }

        if (!deckIsChosen)
        {
          if (noDeck >= 1 && SDL_PointInRect(&mousePos, &RBoutonDeck1))
          {
            Mix_PlayChannel(-1, selectSE, 0);
            deckChoisi = deck1.deckList;
            deckIsChosen = 1;
          }
          if (noDeck >= 2 && SDL_PointInRect(&mousePos, &RBoutonDeck2))
          {
            Mix_PlayChannel(-1, selectSE, 0);
            deckChoisi = deck2.deckList;
            deckIsChosen = 1;
          }
          if (noDeck >= 3 && SDL_PointInRect(&mousePos, &RBoutonDeck3))
          {
            Mix_PlayChannel(-1, selectSE, 0);
            deckChoisi = deck3.deckList;
            deckIsChosen = 1;
          }
        }
        else
        {
          if (!playerIsConnected)
          {
            if (SDL_PointInRect(&mousePos, &RBoutonTrouverUnServeur))
            {
              Mix_PlayChannel(-1, selectSE, 0);
              quitValue = playerPrompt("Renseignez l'ip", &ipAddress);
              if (quitValue == 1)
              {
                libereTextureLobby(boutonTrouverUnServeur, boutonRetour, zoneDecks, boutonDeck1, boutonDeck2, boutonDeck3, texteAttente);
                return 1;
              }
              if (inet_addr(ipAddress) == (u_long)(-1))
              {
                printf("ERREUR ADRESSE IP INVALIDE : %s\n", ipAddress);
              }
              else
              {
                printf("Adresse IP : %s\n", ipAddress);
                /* Création de la socket */
                sock = socket(AF_INET, SOCK_STREAM, 0);

                /* Configuration de la connexion */
                sin.sin_addr.s_addr = inet_addr(ipAddress);
                sin.sin_family = AF_INET;
                sin.sin_port = htons(25565);

                /* Si le client arrive à se connecter */
                if (connect(sock, (SOCKADDR *)&sin, sizeof(sin)) != SOCKET_ERROR)
                {
                  printf("Connexion à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
                  playerIsConnected = 1;
                }
                else
                {
                  printf("Impossible de se connecter\n");
                  closesocket(sock);
                }
              }
            }
          }
        }
        break;

      case SDL_MOUSEMOTION:
        mousePos.x = event.motion.x;
        mousePos.y = event.motion.y;

        //Gonflement du bouton Retour
        if (SDL_PointInRect(&mousePos, &RBoutonRetour))
        {
          if (!soundPlayed)
          {
            Mix_PlayChannel(-1, hover, 0);
            soundPlayed = 1;
            swellRect(&RBoutonRetour);
          }
        }
        else
        {
          if (deckIsChosen && (!playerIsConnected) && SDL_PointInRect(&mousePos, &RBoutonTrouverUnServeur))
          {
            //Gonflement du Bouton 1
            if (!soundPlayed)
            {
              Mix_PlayChannel(-1, hover, 0);
              soundPlayed = 1;
              swellRect(&RBoutonTrouverUnServeur);
            }
          }
          else
          {
            soundPlayed = 0;
            refreshTextureSizesLobby(&RBoutonTrouverUnServeur, &RBoutonRetour, &RZoneDecks, &RBoutonDeck1, &RBoutonDeck2, &RBoutonDeck3, &RTexteAttente);
          }
        }
        break;

      case SDL_WINDOWEVENT:
        if ((&event)->window.event == SDL_WINDOWEVENT_RESIZED)
        {
          SDL_GetWindowSize(win, &winW, &winH);
          refreshTextureSizesLobby(&RBoutonTrouverUnServeur, &RBoutonRetour, &RZoneDecks, &RBoutonDeck1, &RBoutonDeck2, &RBoutonDeck3, &RTexteAttente);
        }
        break;

      //Cas général
      default:
        break;
      }

      //Affichage du menu
      clearScreen();

      if (!deckIsChosen)
      {
        SDL_RenderCopy(ren, zoneDecks, NULL, &RZoneDecks);

        if (noDeck >= 1)
        {
          SDL_RenderCopy(ren, boutonDeck1, NULL, &RBoutonDeck1);
        }
        if (noDeck >= 2)
        {
          SDL_RenderCopy(ren, boutonDeck2, NULL, &RBoutonDeck2);
        }
        if (noDeck >= 3)
        {
          SDL_RenderCopy(ren, boutonDeck3, NULL, &RBoutonDeck3);
        }
      }
      else
      {
        if (!playerIsConnected)
        {
          SDL_RenderCopy(ren, boutonTrouverUnServeur, NULL, &RBoutonTrouverUnServeur);
        }
        else
        {
          SDL_RenderCopy(ren, texteAttente, NULL, &RTexteAttente);
        }
      }

      //Bouton Retour
      SDL_RenderCopy(ren, boutonRetour, NULL, &RBoutonRetour);

      //Rendu
      SDL_RenderPresent(ren);

      //Select
      if (playerIsConnected)
      {
        FD_ZERO(&readfs);
        FD_SET(sock, &readfs);
        if ((ret = select(sock + 1, &readfs, NULL, NULL, tv)) < 0)
        {
          printf("Erreur select\n");
        }
        if (FD_ISSET(sock, &readfs))
        {
          if ((n = recv(sock, buffer, sizeof buffer - 1, 0)) < 0)
          {
            printf("Erreur recv\n");
          }

          buffer[n] = '\0';
          printf("%s", buffer);
          if (!strcmp("0", buffer))
          {
            printf("\nLe jeu peut commencer\n");
            send(sock, "0 ", strlen("0"), 0);
            if (Mix_PlayMusic(playTheme, -1) == -1)
            {
              return 1;
            }
            quitValue = jeu(sock, deckChoisi);
            refreshTextureSizesLobby(&RBoutonTrouverUnServeur, &RBoutonRetour, &RZoneDecks, &RBoutonDeck1, &RBoutonDeck2, &RBoutonDeck3, &RTexteAttente);
            playerIsConnected = 0;
            if (quitValue == 1)
            {
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
  WSACleanup();
}
