#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "carte.h"
#include "fonctions.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

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


void afficheJeuJoueur(SDL_Renderer * ren,Lcarte jeuJoueur,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh,int effetCoutTerrain) {
    int distanceInterCartes = (winW/10)+(winW/120);
    int posXcarte = (winW/10)+(5*winW)/120;
    SDL_Rect carte;
    carte.y=(winH*5)/6;
    carte.w=(winW/10);
    carte.h=(winH/6);
    while (jeuJoueur!=NULL) {
        carte.x=posXcarte;
        //SDL_QueryTexture(jeuJoueur->carte->Tcarte,NULL,NULL,&carte.w,&carte.h);
        //SDL_RenderCopy(ren,jeuJoueur->carte->Tcarte,NULL,&carte);
        if (jeuJoueur->Rcarte!=NULL) {
            free(jeuJoueur->Rcarte);
            jeuJoueur->Rcarte=NULL;
        }
        jeuJoueur->Rcarte=copieRect(&carte);
        if (refresh) {
            refreshCarte(ren,jeuJoueur->carte,dejavu,&carte,0,0,effetCoutTerrain);
        }
        else {
            SDL_RenderCopy(ren,jeuJoueur->carte->TcarteComplet,NULL,&carte);
        }
        jeuJoueur=jeuJoueur->suiv;
        posXcarte+=distanceInterCartes;
    }
}

void afficheJeuEnnemi(SDL_Renderer * ren,SDL_Texture* Tdos,Lcarte jeuEnnemi,int winW,int winH) {
    int distanceInterCartes = (winW/10)+(winW/120);
    int posXcarte = (winW/10)+(5*winW)/120;
    SDL_Rect carte;
    carte.y=0;
    carte.w=(winW/10);
    carte.h=(winH/6);
    while (jeuEnnemi!=NULL) {
        carte.x=posXcarte;
        //SDL_QueryTexture(jeuJoueur->carte->Tcarte,NULL,NULL,&carte.w,&carte.h);
        SDL_RenderCopy(ren,Tdos,NULL,&carte);
        jeuEnnemi=jeuEnnemi->suiv;
        posXcarte+=distanceInterCartes;
    }
}

void afficheBoardJoueur(SDL_Renderer * ren, Lcarte boardJoueur,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh,int effetPVTerrainJoueur,int effetAttTerrainJoueur) {
    int distanceInterCartes = (winW/10)+(winW/120);
    int posXcarte = (winW/10)+(5*winW)/120;
    SDL_Rect carte;
    carte.y=(winH/2)+(winH/60);
    carte.w=(winW/10);
    carte.h=(winH/6);
    while (boardJoueur!=NULL) {


        carte.x=posXcarte;
        //SDL_QueryTexture(jeuJoueur->carte->Tcarte,NULL,NULL,&carte.w,&carte.h);
        if (boardJoueur->Rcarte!=NULL) {
            free(boardJoueur->Rcarte);
            boardJoueur->Rcarte=NULL;
        }
        boardJoueur->Rcarte=copieRect(&carte);
        if (refresh) {
            refreshCarte(ren,boardJoueur->carte,dejavu,&carte,effetPVTerrainJoueur,effetAttTerrainJoueur,0);
        }
        else {
            SDL_RenderCopy(ren,boardJoueur->carte->TcarteComplet,NULL,&carte);
        }
        boardJoueur=boardJoueur->suiv;
        posXcarte+=distanceInterCartes;
    }
}

void afficheBoardEnnemi(SDL_Renderer * ren, Lcarte boardEnnemi,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh,int effetPVTerrainEnnemi,int effetAttTerrainEnnemi) {
    int distanceInterCartes = (winW/10)+(winW/120);
    int posXcarte = (winW/10)+(5*winW)/120;
    SDL_Rect carte;
    carte.y=(winH/2)-(winH/6)-(winH/60);
    carte.w=(winW/10);
    carte.h=(winH/6);
    while (boardEnnemi!=NULL) {


        carte.x=posXcarte;
        //SDL_QueryTexture(jeuJoueur->carte->Tcarte,NULL,NULL,&carte.w,&carte.h);
        if (boardEnnemi->Rcarte!=NULL) {
            free(boardEnnemi->Rcarte);
            boardEnnemi->Rcarte=NULL;
        }
        boardEnnemi->Rcarte=copieRect(&carte);
        if (refresh) {
            refreshCarte(ren,boardEnnemi->carte,dejavu,&carte,effetPVTerrainEnnemi,effetAttTerrainEnnemi,0);
        }
        else {
            SDL_RenderCopy(ren,boardEnnemi->carte->TcarteComplet,NULL,&carte);
        }
        //SDL_RenderCopy(ren,boardEnnemi->carte->Tcarte,NULL,&carte);
        boardEnnemi=boardEnnemi->suiv;
        posXcarte+=distanceInterCartes;
    }
}

void afficheManaJoueur(SDL_Renderer * ren,SDL_Texture ** TmanaJoueur, int manaJoueur,int manaMaxJoueur,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh) {
    SDL_Color coulTextBout={255,255,255,255};
    SDL_Rect RmanaJoueur;
    RmanaJoueur.x=winW/120;
    RmanaJoueur.y=(winH*5)/6;
    RmanaJoueur.w=(winW/8);
    RmanaJoueur.h=winH/6;
    if (refresh) {
        if (*TmanaJoueur!=NULL) {
            SDL_DestroyTexture(*TmanaJoueur);
        }
        SDL_Surface * tmp;
        char textMana[6];
        sprintf(textMana,"%d/%d",manaJoueur,manaMaxJoueur);
        tmp=TTF_RenderText_Solid(dejavu,textMana,coulTextBout);
        *TmanaJoueur=SDL_CreateTextureFromSurface(ren,tmp);
        SDL_FreeSurface(tmp);
        SDL_RenderCopy(ren,*TmanaJoueur,NULL,&RmanaJoueur);
    }
    else {
        SDL_RenderCopy(ren,*TmanaJoueur,NULL,&RmanaJoueur);
    }
}

void afficheManaEnnemi(SDL_Renderer * ren,SDL_Texture ** TmanaEnnemi, int manaEnnemi,int manaMaxEnnemi,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh) {
    SDL_Color coulTextBout={255,255,255,255};
    SDL_Rect RmanaEnnemi;
    RmanaEnnemi.x=winW/120;
    RmanaEnnemi.y=0;
    RmanaEnnemi.w=(winW/8);
    RmanaEnnemi.h=winH/6;
    if (refresh) {
        if (*TmanaEnnemi!=NULL) {
            SDL_DestroyTexture(*TmanaEnnemi);
        }
        SDL_Surface * tmp;
        char textMana[6];
        sprintf(textMana,"%d/%d",manaEnnemi,manaMaxEnnemi);
        tmp=TTF_RenderText_Solid(dejavu,textMana,coulTextBout);
        *TmanaEnnemi=SDL_CreateTextureFromSurface(ren,tmp);
        SDL_FreeSurface(tmp);
        SDL_RenderCopy(ren,*TmanaEnnemi,NULL,&RmanaEnnemi);
    }
    else {
        SDL_RenderCopy(ren,*TmanaEnnemi,NULL,&RmanaEnnemi);
    }
}

void affichePvJoueur(SDL_Renderer * ren,SDL_Texture ** TpvJoueur,int pvJoueur,int winW,int winH,SDL_Rect * RpvJoueur, TTF_Font * dejavu,SDL_bool refresh) {
  SDL_Color coulTextBout={255,255,255,255};
  if (refresh) {
      if (*TpvJoueur!=NULL) {
          SDL_DestroyTexture(*TpvJoueur);
      }
      SDL_Surface * tmpText;
      SDL_Texture * TtmpText;
      SDL_Texture * TtmpCoeur;
      SDL_Texture * TtmpImage=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,(winW/5),(winH/5));;
      SDL_Rect Rtext;
      SDL_Rect Rcoeur;
      Rtext.x=winH/31;
      Rtext.y=winW/50;
      Rtext.w=winW/7;
      Rtext.h=winH/7;
      Rcoeur.x=0;
      Rcoeur.y=0;
      Rcoeur.w=(winW/5);
      Rcoeur.h=winH/5;
      char textpv[6];
      sprintf(textpv,"%d",pvJoueur);
      tmpText=TTF_RenderText_Solid(dejavu,textpv,coulTextBout);
      TtmpText=SDL_CreateTextureFromSurface(ren,tmpText);
      SDL_FreeSurface(tmpText);
      TtmpCoeur=loadPictures(ren,"image/menu/coeur.png");
      SDL_SetRenderTarget(ren,TtmpImage);
      if (SDL_SetRenderDrawColor(ren,255,0,0,255)<0) {
          printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      }
      SDL_RenderFillRect(ren,&Rcoeur);
      SDL_RenderCopy(ren,TtmpText,NULL,&Rtext);
      SDL_RenderCopy(ren,TtmpCoeur,NULL,&Rcoeur);
      SDL_DestroyTexture(TtmpText);
      SDL_DestroyTexture(TtmpCoeur);
      SDL_SetRenderTarget(ren,NULL);
      *TpvJoueur=TtmpImage;
  }
  SDL_RenderCopy(ren,*TpvJoueur,NULL,RpvJoueur);
}

void affichePvEnnemi(SDL_Renderer * ren,SDL_Texture ** TpvJoueur,int pvJoueur,int winW,int winH,SDL_Rect * RpvEnnemi, TTF_Font * dejavu,SDL_bool refresh) {
  SDL_Color coulTextBout={255,255,255,255};
  if (refresh) {
      if (*TpvJoueur!=NULL) {
          SDL_DestroyTexture(*TpvJoueur);
      }
      SDL_Surface * tmpText;
      SDL_Texture * TtmpText;
      SDL_Texture * TtmpCoeur;
      SDL_Texture * TtmpImage=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,(winW/5),(winH/5));;
      SDL_Rect Rtext;
      SDL_Rect Rcoeur;
      Rtext.x=winH/31;
      Rtext.y=winW/50;
      Rtext.w=winW/7;
      Rtext.h=winH/7;
      Rcoeur.x=0;
      Rcoeur.y=0;
      Rcoeur.w=(winW/5);
      Rcoeur.h=winH/5;
      char textpv[6];
      sprintf(textpv,"%d",pvJoueur);
      tmpText=TTF_RenderText_Solid(dejavu,textpv,coulTextBout);
      TtmpText=SDL_CreateTextureFromSurface(ren,tmpText);
      SDL_FreeSurface(tmpText);
      TtmpCoeur=loadPictures(ren,"image/menu/coeur.png");
      SDL_SetRenderTarget(ren,TtmpImage);
      if (SDL_SetRenderDrawColor(ren,255,0,0,255)<0) {
          printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      }
      SDL_RenderFillRect(ren,&Rcoeur);
      SDL_RenderCopy(ren,TtmpText,NULL,&Rtext);
      SDL_RenderCopy(ren,TtmpCoeur,NULL,&Rcoeur);
      SDL_DestroyTexture(TtmpText);
      SDL_DestroyTexture(TtmpCoeur);
      SDL_SetRenderTarget(ren,NULL);
      *TpvJoueur=TtmpImage;
  }
  SDL_RenderCopy(ren,*TpvJoueur,NULL,RpvEnnemi);
}

int jeu(int sock,Lcarte deckChoisi,SDL_Window * fen,SDL_Renderer * ren,TTF_Font * dejavu) {

    int winW;
    int winH;

    SDL_GetWindowSize(fen,&winW,&winH);
    SDL_Color coulTextBout={255,255,255,255};
    SDL_Color coulTextVictory={0,0,100,255};

    char buffer[1024];
    char * bufftmp;
    int n;

    //Jeu en lui-même
    int pvJoueur=30;
    int pvEnnemi=30;

    int manaJoueur=0;
    int manaMaxJoueur=0;
    int manaEnnemi=0;
    int manaMaxEnnemi=0;

    Lcarte deckJoueur=deckChoisi;
    Lcarte deckEnnemi=NULL;

    Lcarte jeuJoueur=NULL;
    Lcarte jeuEnnemi=NULL;

    Lcarte boardJoueur=NULL;
    Lcarte boardEnnemi=NULL;

    Lcarte provocationJoueur=NULL;
    Lcarte provocationEnnemi=NULL;

    Carte * terrainJoueur = NULL;
    Carte * terrainEnnemi = NULL;

    int gonfle=0; //Indique si une carte est gonflé ou pas
    int idboard=0;
    int tour = 0; //1 si tour de J1 en cours 0 sinon

    SDL_bool refresh = SDL_TRUE;

    //Position d'une carte mise en évidence
    SDL_Rect RcarteCentre;
    RcarteCentre.x=winW/3;
    RcarteCentre.y=winH/4;
    RcarteCentre.h=winH/2;
    RcarteCentre.w=winW/3;

    //Position vie joueur
    SDL_Rect RpvJoueur;
    RpvJoueur.x=(7*winW/16);
    RpvJoueur.y=(42*winH)/60;
    RpvJoueur.w=(winW/8);
    RpvJoueur.h=winH/8;

    //Position vie Ennemi
    SDL_Rect RpvEnnemi;
    RpvEnnemi.x=(7*winW/16);
    RpvEnnemi.y=(18*winH)/60-winH/8;
    RpvEnnemi.w=(winW/8);
    RpvEnnemi.h=winH/8;

    //Position terrainJoueur
    SDL_Rect RterrainJoueur;
    RterrainJoueur.w=(winW/10);
    RterrainJoueur.h=(winH/6);
    RterrainJoueur.x=(winW/60);
    RterrainJoueur.y=(winH*2/6)+(winH/15);

    //Position terrainEnnemi
    SDL_Rect RterrainEnnemi;
    RterrainEnnemi.w=(winW/10);
    RterrainEnnemi.h=(winH/6);
    RterrainEnnemi.x=(winW/60);
    RterrainEnnemi.y=(winH*2/6)-(winH/6)-(winH/15);

    //test


    boardEnnemi=NULL;
    boardJoueur=NULL;

    int j;
    for (j=0;j<4;j++) {
      //boardEnnemi=pioche(boardEnnemi,&deckEnnemi);
      jeuJoueur=pioche(jeuJoueur,&deckJoueur);
    }

    //Terrain de Jeu

    //Bouton Passer son tour
    SDL_Texture * bout1 = NULL;
    SDL_Texture * TtextBout1 = NULL;
    SDL_Surface * textBout1 = NULL;
    textBout1=TTF_RenderText_Solid(dejavu,"Passer",coulTextBout);
    if (!textBout1) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    bout1=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,winW/4,winH/10);
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
    RtextBout1.x=(2*winW)/3+winW/60;
    RtextBout1.y=(winH*43)/60+(winH/100);
    RtextBout1.w=(winW/4)-(winW/30);
    RtextBout1.h=winH/10-(winH/40);
    RBout1.x=0;
    RBout1.y=0;
    RBout1.w=winW/4;
    RBout1.h=winH/10;
    SDL_SetRenderTarget(ren,bout1);
    if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
        printf("Erreur lors du changement de couleur : %s",SDL_GetError());
        return 1;
    }
    SDL_RenderFillRect(ren,&RBout1);
    RBout1.x=(2*winW)/3;
    RBout1.y=(winH*43)/60;

    //Mana des joueurs
    SDL_Texture * TmanaJoueur = NULL;
    SDL_Texture * TmanaEnnemi = NULL;
    //Pv des joueurs
    SDL_Texture * TpvJoueur = NULL;
    SDL_Texture * TpvEnnemi = NULL;

    //Dos des cartes
    SDL_Texture * Tdos;
    Tdos=loadPictures(ren,"image/cartes/dos.png");
    if (Tdos==NULL) {
        SDL_Log("Unable to create texture from image :%s",SDL_GetError());
        return 1;
    }

    //Zone d'affichage du jeu
    SDL_Texture * zoneJeu = NULL;

    zoneJeu=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,winW,(winH*2)/3);
    if (zoneJeu==NULL) {
      SDL_Log("Unable to create texture:%s",SDL_GetError());
      return 1;
    }
    SDL_Rect RzoneJeu;
    RzoneJeu.x=winW/7-winW/100;
    RzoneJeu.y=0;
    RzoneJeu.w=winW;
    RzoneJeu.h=(winH*2)/3;
    SDL_SetRenderTarget(ren,zoneJeu);
    if (SDL_SetRenderDrawColor(ren,255,0,0,255)<0) {
        printf("Erreur lors du changement de couleur : %s",SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_RenderFillRect(ren,&RzoneJeu);
    SDL_RenderFillRect(ren,&RterrainEnnemi);
    SDL_RenderFillRect(ren,&RterrainJoueur);
    if (SDL_SetRenderDrawColor(ren,0,0,0,255)<0) {
        printf("Erreur lors du changement de couleur : %s",SDL_GetError());
        return EXIT_FAILURE;
    }
    int i;
    for (i=(winH/3)-(winH/120);i<=(winH/3)+(winH/120);i++) {
        SDL_RenderDrawLine(ren,0,i,winW,i);
    }
    SDL_SetRenderTarget(ren,NULL);
    RzoneJeu.x=0;
    RzoneJeu.y=winH/6;

    //Victoire et Défaite
    SDL_Surface * surfacetmp=NULL;
    SDL_Texture * Tdefaite;
    SDL_Texture * Tvictoire;
    surfacetmp=TTF_RenderText_Solid(dejavu,"VICTOIRE",coulTextVictory);
    if (!surfacetmp) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    Tvictoire=SDL_CreateTextureFromSurface(ren,surfacetmp);
    SDL_FreeSurface(surfacetmp);

    surfacetmp=TTF_RenderText_Solid(dejavu,"DEFAITE",coulTextVictory);
    if (!surfacetmp) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    Tdefaite=SDL_CreateTextureFromSurface(ren,surfacetmp);
    SDL_FreeSurface(surfacetmp);

    SDL_Rect Rvictory;
    Rvictory.x=(winW/8);
    Rvictory.y=(winH/3);
    Rvictory.w=(3*winW)/4;
    Rvictory.h=(winH/3);

    int victory=0;
    //Boucle d'affichage
    SDL_Point mousePos;
    SDL_Event event;
    Carte * cartetmp=NULL;
    Carte * carteAttaquante=NULL;
    Carte * carteGonfle=NULL;
    Carte * terraintmp=NULL;
    Carte * sorttmp=NULL;

    //Pour le select
    fd_set readfs;
    int ret;
    struct timeval * tv;
    tv=calloc(1,sizeof(*tv));
    tv->tv_sec=0;
    tv->tv_usec=0;

    int idtmp;
    int idtmp2;
    int instruction;

    //Pour les Terrains
    int effetAttTerrainJoueur=0;
    int effetPVTerrainJoueur=0;
    int effetAttTerrainEnnemi=0;
    int effetPVTerrainEnnemi=0;
    int effetCoutTerrainJoueur=0;
    int effetCoutTerrainEnnemi=0;


    while (1) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {

                //Pour quitter
                case SDL_QUIT:
                    send(sock,"/X",strlen("/X"),0);
                    libereListeCarte(boardEnnemi);
                    libereListeCarte(boardJoueur);
                    libereListeCarte(jeuJoueur);
                    libereListeCarte(jeuEnnemi);
                    SDL_DestroyTexture(TmanaJoueur);
                    SDL_DestroyTexture(TmanaEnnemi);
                    SDL_DestroyTexture(Tdos);
                    SDL_DestroyTexture(zoneJeu);
                    return 1;
                    break;

                //En cas de clic
                case SDL_MOUSEBUTTONDOWN:
                    mousePos.x=event.button.x;
                    mousePos.y=event.button.y;

                    //Passer son tour
                    if (tour && SDL_PointInRect(&mousePos,&RBout1)) {
                        tour=0;
                        send(sock,"2",strlen("2"),0);
                        if (manaMaxEnnemi<10) {
                          manaMaxEnnemi+=1;
                        }
                        manaEnnemi=manaMaxEnnemi;
                        refresh=SDL_TRUE;
                    }

                    //Mettre en évidence une carte
                    if (gonfle) {
                      gonfle=0;
                    }
                    else  {
                        if (event.button.button==SDL_BUTTON_RIGHT) {
                            carteGonfle = carteClique(&mousePos,jeuJoueur,boardEnnemi,boardJoueur);
                            if (SDL_PointInRect(&mousePos,&RterrainEnnemi)&& terrainEnnemi!=NULL) {
                                carteGonfle=terrainEnnemi;
                            }
                            if (SDL_PointInRect(&mousePos,&RterrainJoueur)&& terrainJoueur!=NULL) {
                                carteGonfle=terrainJoueur;
                            }
                            if (carteGonfle!=NULL) {
                                gonfle=1;
                            }

                        }
                    }

                    //ATTAQUE
                    if (tour && carteAttaquante!=NULL && event.button.button==SDL_BUTTON_LEFT) {
                      if (SDL_PointInRect(&mousePos,&RpvEnnemi)&&((provocationEnnemi==NULL)||(isIn(cartetmp,provocationEnnemi)))) {
                          printf("%s attaque face ! \n",carteAttaquante->nom);
                          pvEnnemi-=carteAttaquante->att+effetAttTerrainJoueur;
                          carteAttaquante->peutAttaquer=SDL_FALSE;
                          refresh=SDL_TRUE;
                          carteAttaquante=NULL;
                          sprintf(buffer,"/5 %d",pvEnnemi);
                          send(sock,buffer,strlen(buffer),0);
                      }
                      else {
                        cartetmp=carteClique(&mousePos,NULL,boardEnnemi,NULL);
                        if (cartetmp!=NULL&&((provocationEnnemi==NULL)||(isIn(cartetmp,provocationEnnemi)))) {
                          printf("%s se fait attaquer par %s\n",cartetmp->nom,carteAttaquante->nom);
                          cartetmp->pv-=carteAttaquante->att+effetAttTerrainJoueur;
                          carteAttaquante->pv-=cartetmp->att+effetAttTerrainEnnemi;
                          carteAttaquante->peutAttaquer=SDL_FALSE;
                          sprintf(buffer,"/4 %d %d",carteAttaquante->idboard,cartetmp->idboard);
                          send(sock,buffer,strlen(buffer),0);

                          /*
                          if (cartetmp->pv<=0) {
                              if (cartetmp->raleDagonie(cartetmp,ren,&provocationEnnemi) != 1) {
                                  printf("ERREUR LORS DU RALE D'AGONIE DE %s\n",cartetmp->nom);
                              }
                              if (isIn(cartetmp,provocationEnnemi)) {
                                  provocationEnnemi=retirerCarte(provocationEnnemi,cartetmp);
                              }
                              boardEnnemi=retirerCarte(boardEnnemi,cartetmp);
                          }

                          if (carteAttaquante->pv<=0) {
                              if (carteAttaquante->raleDagonie(cartetmp,ren,&provocationJoueur) != 1) {
                                  printf("ERREUR LORS DU RALE D'AGONIE DE %s\n",carteAttaquante->nom);
                              }
                              if (isIn(carteAttaquante,provocationJoueur)) {
                                  provocationJoueur=retirerCarte(provocationJoueur,carteAttaquante);
                              }
                              boardJoueur=retirerCarte(boardJoueur,carteAttaquante);
                          }
                          */
                          refresh=SDL_TRUE;
                          carteAttaquante=NULL;
                        }
                        else {
                            carteAttaquante=NULL;
                        }
                      }
                    }
                    else {
                        carteAttaquante=NULL;
                    }

                    //Faire attaquer un monstre
                    if (tour && event.button.button==SDL_BUTTON_LEFT) {
                        cartetmp=carteClique(&mousePos,NULL,NULL,boardJoueur);
                        if (cartetmp!=NULL && cartetmp->peutAttaquer) {
                            carteAttaquante=cartetmp;
                            printf("ATTAQUE DE %s\n",carteAttaquante->nom);
                        }
                    }

                    //Choisir le côté du terrain
                    if (tour && terraintmp!=NULL && event.button.button==SDL_BUTTON_LEFT) {
                        if (SDL_PointInRect(&mousePos,&RterrainEnnemi)) {
                          if(terrainEnnemi!=NULL) {
                              libereCarte(terrainEnnemi);
                              terrainEnnemi=NULL;
                          }
                          terrainEnnemi=terraintmp;
                          manaJoueur-=terraintmp->cout+effetCoutTerrainJoueur;
                          jeuJoueur=retirerCarte(jeuJoueur,terraintmp);
                          terraintmp=NULL;
                          refresh=SDL_TRUE;
                          sprintf(buffer,"/6 %d %d",terrainEnnemi->id,0);
                          send(sock,buffer,strlen(buffer),0);
                        }
                        if (SDL_PointInRect(&mousePos,&RterrainJoueur)) {
                          if (terrainJoueur!=NULL) {
                              libereCarte(terrainJoueur);
                              terrainJoueur=NULL;
                          }
                          terrainJoueur=terraintmp;
                          manaJoueur-=terraintmp->cout+effetCoutTerrainJoueur;
                          jeuJoueur=retirerCarte(jeuJoueur,terraintmp);
                          terraintmp=NULL;
                          refresh=SDL_TRUE;
                          sprintf(buffer,"/6 %d %d",terrainJoueur->id,1);
                          send(sock,buffer,strlen(buffer),0);
                        }

                    }
                    //Choisir la cible d'un sort
                    if (tour && sorttmp!=NULL && event.button.button==SDL_BUTTON_LEFT) {
                      cartetmp=carteClique(&mousePos,boardJoueur,boardEnnemi,NULL);
                      if (cartetmp!=NULL) {
                        if (sorttmp->effetDirect(cartetmp,ren,dejavu,&provocationJoueur,&boardJoueur,&boardEnnemi,NULL,NULL,NULL,&jeuJoueur) != 1) {
                            printf("ERREUR LORS DU SORT DE %s\n",cartetmp->nom);
                        }
                        manaJoueur-=sorttmp->cout+effetCoutTerrainJoueur;
                        jeuJoueur=retirerCarte(jeuJoueur,sorttmp);
                        refresh=SDL_TRUE;
                        sprintf(buffer,"/8 %d %d",sorttmp->id,cartetmp->idboard);
                        send(sock,buffer,strlen(buffer),0);
                        sorttmp=NULL;
                      }
                      else {
                        sorttmp=NULL;
                      }
                    }
                    else {
                      sorttmp=NULL;
                    }

                    //Jouer un monstre, un terrain ou un sort
                    if (tour && event.button.button==SDL_BUTTON_LEFT && event.button.clicks==2) {
                        cartetmp=carteClique(&mousePos,jeuJoueur,NULL,NULL);
                        if (cartetmp!=NULL && cartetmp->genre==1 && cartetmp->cout + effetCoutTerrainJoueur<= manaJoueur) {
                            if (estInvocable(cartetmp,boardJoueur)) {
                                boardJoueur=ajoutTete(boardJoueur,cartetmp);
                                if (cartetmp->effetDirect(cartetmp,ren,dejavu,&provocationJoueur,&boardJoueur,&boardEnnemi,NULL,NULL,NULL,&jeuJoueur) != 1) {
                                    printf("ERREUR LORS DE L'INVOCATION DE %s\n",cartetmp->nom);
                                }
                                manaJoueur-=cartetmp->cout+effetCoutTerrainJoueur;
                                cartetmp->idboard=idboard;
                                idboard++;
                                sprintf(buffer,"/3 %d",cartetmp->id);
                                send(sock,buffer,strlen(buffer),0);
                                jeuJoueur=retirerCarte(jeuJoueur,cartetmp);
                                refresh=SDL_TRUE;
                            }
                        }
                        else if (cartetmp!=NULL && cartetmp->genre==3 && cartetmp->cout + effetCoutTerrainJoueur<= manaJoueur) {
                            terraintmp=cartetmp;
                            refresh=SDL_TRUE;
                        }
                        else if (cartetmp!=NULL && cartetmp->genre==2 && cartetmp->cout + effetCoutTerrainJoueur<= manaJoueur) {
                          if (cartetmp->peutAttaquer) {
                              sorttmp=cartetmp;
                          }
                          else {
                              if (cartetmp->effetDirect(cartetmp,ren,dejavu,&provocationJoueur,&boardJoueur,&boardEnnemi,NULL,NULL,NULL,&jeuJoueur) != 1) {
                                  printf("ERREUR LORS DU SORT %s\n",cartetmp->nom);
                              }
                              manaJoueur-=cartetmp->cout+effetCoutTerrainJoueur;
                              refresh=SDL_TRUE;
                              jeuJoueur=retirerCarte(jeuJoueur,cartetmp);
                              sorttmp=NULL;
                              sprintf(buffer,"/7 %d",cartetmp->id);
                              send(sock,buffer,strlen(buffer),0);

                          }
                        }
                    }

                    break;

                case SDL_MOUSEMOTION:
                    mousePos.x=event.motion.x;
                    mousePos.y=event.motion.y;


                    //Gonflement du Bouton 1
                    if (SDL_PointInRect(&mousePos,&RBout1)&&tour) {
                        RBout1.x=(2*winW)/3-(winW/120);
                        RBout1.y=(winH*43)/60-(winH/200);
                        RBout1.w=winW/4+(winW/60);
                        RBout1.h=winH/10+(winH/100);
                        RtextBout1.x=(2*winW)/3+winW/60-(winW/120);
                        RtextBout1.w=(winW/4)-(winW/30)+(winW/60);
                        RtextBout1.h=winH/10-(winH/40)+(winH/100);
                        RtextBout1.y=(winH*43)/60+(winH/100)-(winH/200);
                    }
                    break;

                //Cas général
                default:
                    break;
            }

            //Affichage du menu
              //Couleur du fond
            if (SDL_SetRenderDrawColor(ren,0,0,0,255)<0) {
                printf("Erreur lors du changement de couleur : %s",SDL_GetError());
                return 1;
            }
            SDL_RenderClear(ren);
            //Fond zone de Jeu
            SDL_RenderCopy(ren,zoneJeu,NULL,&RzoneJeu);
            if (refresh) {
                boardJoueur=refreshBoardJoueur(ren,dejavu,boardJoueur,&provocationJoueur,terrainJoueur,&effetPVTerrainJoueur,&effetAttTerrainJoueur,&effetCoutTerrainJoueur);
                boardEnnemi=refreshBoardEnnemi(ren,dejavu,boardEnnemi,&provocationEnnemi,terrainEnnemi,&effetPVTerrainEnnemi,&effetAttTerrainEnnemi,&effetCoutTerrainEnnemi);
            }
            if (pvEnnemi<=0 && victory==0) {
                tour = 0;
                victory=2;
                sprintf(buffer,"/9");
                send(sock,buffer,strlen(buffer),0);
            }
            //bouton1
            SDL_RenderCopy(ren,bout1,NULL,&RBout1);
            SDL_RenderCopy(ren,TtextBout1,NULL,&RtextBout1);

            afficheJeuJoueur(ren,jeuJoueur,winW,winH,dejavu,refresh,effetCoutTerrainJoueur);
            afficheJeuEnnemi(ren,Tdos,jeuEnnemi,winW,winH);

            afficheBoardJoueur(ren,boardJoueur,winW,winH,dejavu,refresh,effetPVTerrainJoueur,effetAttTerrainJoueur);
            afficheBoardEnnemi(ren,boardEnnemi,winW,winH,dejavu,refresh,effetPVTerrainEnnemi,effetAttTerrainEnnemi);

            afficheManaJoueur(ren,&TmanaJoueur,manaJoueur,manaMaxJoueur,winW,winH,dejavu,refresh);
            afficheManaEnnemi(ren,&TmanaEnnemi,manaEnnemi,manaMaxEnnemi,winW,winH,dejavu,refresh);

            affichePvJoueur(ren,&TpvJoueur,pvJoueur,winW,winH,&RpvJoueur,dejavu,refresh);
            affichePvEnnemi(ren,&TpvEnnemi,pvEnnemi,winW,winH,&RpvEnnemi,dejavu,refresh);

            if (terrainJoueur!=NULL) {
                SDL_RenderCopy(ren,terrainJoueur->TcarteComplet,NULL,&RterrainJoueur);
            }

            if (terrainEnnemi!=NULL) {
                SDL_RenderCopy(ren,terrainEnnemi->TcarteComplet,NULL,&RterrainEnnemi);
            }

            if (gonfle) {
                SDL_RenderCopy(ren,carteGonfle->TcarteComplet,NULL,&RcarteCentre);
            }

            if (victory==1) {
                SDL_RenderCopy(ren,Tdefaite,NULL,&Rvictory);
            }
            if (victory==2) {
                SDL_RenderCopy(ren,Tvictoire,NULL,&Rvictory);
            }


            //Rendu
            SDL_RenderPresent(ren);

            if (refresh) {
                refresh=SDL_FALSE;
            }

            //Rafraichissement des textures en cas de modifications de taille de la fenêtre
            SDL_GetWindowSize(fen,&winW,&winH);
            //zoneJeu
            RzoneJeu.w=winW;
            RzoneJeu.h=(winH*2)/3;
            RzoneJeu.x=0;
            RzoneJeu.y=winH/6;
            //Carte au centre
            RcarteCentre.x=winW/3;
            RcarteCentre.y=winH/4;
            RcarteCentre.h=winH/2;
            RcarteCentre.w=winW/3;
            //Bout1
            RtextBout1.x=(2*winW)/3+winW/60;
            RtextBout1.y=(winH*43)/60+(winH/100);
            RtextBout1.w=(winW/4)-(winW/30);
            RtextBout1.h=winH/10-(winH/40);
            RBout1.w=winW/4;
            RBout1.h=winH/10;
            RBout1.x=(2*winW)/3;
            RBout1.y=(winH*43)/60;
            //Position vie
            RpvJoueur.x=(7*winW/16);
            RpvJoueur.y=(42*winH)/60;
            RpvJoueur.w=(winW/8);
            RpvJoueur.h=winH/8;
            RpvEnnemi.x=(7*winW/16);
            RpvEnnemi.y=(18*winH)/60-winH/8;
            RpvEnnemi.w=(winW/8);
            RpvEnnemi.h=winH/8;
            //Terrains
            RterrainJoueur.w=(winW/10);
            RterrainJoueur.h=(winH/6);
            RterrainJoueur.x=(winW/60);
            RterrainJoueur.y=(winH/2)+(winH/15);
            RterrainEnnemi.w=(winW/10);
            RterrainEnnemi.h=(winH/6);
            RterrainEnnemi.x=(winW/60);
            RterrainEnnemi.y=(winH/2)-(winH/6)-(winH/15);
            //VICTOIRE
            Rvictory.x=(winW/8);
            Rvictory.y=(winH/3);
            Rvictory.w=(3*winW)/4;
            Rvictory.h=(winH/3);

            FD_ZERO(&readfs);
            FD_SET(sock, &readfs);
            if((ret = select(sock + 1, &readfs, NULL, NULL, tv)) < 0) {
              printf("Erreur select\n");
            }
            if (FD_ISSET(sock, &readfs)) {
              if((n = recv(sock, buffer, sizeof buffer-1, 0)) < 0) {
                printf("Erreur recv\n");
              }
              buffer[n] = '\0';
              i=0;
              j=0;
              printf("%s\n",buffer);
              while(i<n) {
                  if (buffer[i]=='/') {
                      buffer[i]='\0';
                      j=i;
                      bufftmp=buffer+j+1;
                      if (!strcmp(bufftmp,"X")) {
                          printf("DECONNEXION DU JOUEUR ADVERSE\n");
                          return 0;
                      }
                      if (!strcmp(bufftmp,"1")) {
                          tour=1;
                          if (len(jeuJoueur)>=8) {

                          }
                          else {
                              jeuJoueur=pioche(jeuJoueur,&deckJoueur);
                          }
                          if (manaMaxJoueur<10) {
                            manaMaxJoueur++;
                          }
                          boardJoueur=refreshAttaque(boardJoueur);
                          refresh=SDL_TRUE;
                          manaJoueur=manaMaxJoueur;
                      }
                      if (!strcmp(bufftmp,"2")) {
                          manaMaxEnnemi++;
                          refresh=SDL_TRUE;
                          manaEnnemi=manaMaxEnnemi;
                      }
                      if (bufftmp[0]=='3') {
                          sscanf(bufftmp,"%d %d",&instruction,&idtmp);
                          cartetmp=idtocard(idtmp,ren,dejavu);
                          cartetmp->idboard=idboard;
                          idboard++;
                          boardEnnemi=ajoutTete(boardEnnemi,cartetmp);
                          if (cartetmp->effetDirect(cartetmp,ren,dejavu,&provocationEnnemi,&boardEnnemi,&boardJoueur,NULL,NULL,NULL,&jeuEnnemi) != 1) {
                              printf("ERREUR LORS DE L'INVOCATION DE %s\n",cartetmp->nom);
                          }
                          manaEnnemi-=cartetmp->cout+effetCoutTerrainEnnemi;
                          refresh=SDL_TRUE;
                          boardJoueur=refreshBoardJoueur(ren,dejavu,boardJoueur,&provocationJoueur,terrainJoueur,&effetPVTerrainJoueur,&effetAttTerrainJoueur,&effetCoutTerrainJoueur);
                          boardEnnemi=refreshBoardEnnemi(ren,dejavu,boardEnnemi,&provocationEnnemi,terrainEnnemi,&effetPVTerrainEnnemi,&effetAttTerrainEnnemi,&effetCoutTerrainEnnemi);
                      }
                      if (bufftmp[0]=='4') {
                          sscanf(bufftmp,"%d %d %d",&instruction,&idtmp,&idtmp2);
                          cartetmp=inBoard(idtmp,boardEnnemi);
                          carteAttaquante=inBoard(idtmp2,boardJoueur);
                          if (cartetmp==NULL || carteAttaquante==NULL) {
                              printf("ERREUR ATTAQUE ENNEMI %p %p \n",cartetmp,carteAttaquante);
                              return 1;
                          }
                          cartetmp->pv-=carteAttaquante->att+effetAttTerrainJoueur;
                          carteAttaquante->pv-=cartetmp->att+effetAttTerrainEnnemi;
                          carteAttaquante=NULL;
                          refresh=SDL_TRUE;
                          boardJoueur=refreshBoardJoueur(ren,dejavu,boardJoueur,&provocationJoueur,terrainJoueur,&effetPVTerrainJoueur,&effetAttTerrainJoueur,&effetCoutTerrainJoueur);
                          boardEnnemi=refreshBoardEnnemi(ren,dejavu,boardEnnemi,&provocationEnnemi,terrainEnnemi,&effetPVTerrainEnnemi,&effetAttTerrainEnnemi,&effetCoutTerrainEnnemi);

                      }
                      if (bufftmp[0]=='5') {
                          sscanf(bufftmp,"%d %d",&instruction,&idtmp);
                          pvJoueur=idtmp;
                          refresh=SDL_TRUE;
                      }
                      if (bufftmp[0]=='6') {
                          sscanf(bufftmp,"%d %d %d",&instruction,&idtmp,&idtmp2);
                          cartetmp=idtocard(idtmp,ren,dejavu);
                          if (idtmp2) {
                            if (terrainEnnemi!=NULL) {
                                libereCarte(terrainEnnemi);
                                terrainEnnemi=NULL;
                            }
                            terrainEnnemi=cartetmp;
                          }
                          else {
                            if (terrainJoueur!=NULL) {
                                libereCarte(terrainJoueur);
                                terrainJoueur=NULL;
                            }
                            terrainJoueur=cartetmp;
                          }
                          refresh=SDL_TRUE;
                          manaEnnemi-=cartetmp->cout+effetCoutTerrainEnnemi;
                          boardJoueur=refreshBoardJoueur(ren,dejavu,boardJoueur,&provocationJoueur,terrainJoueur,&effetPVTerrainJoueur,&effetAttTerrainJoueur,&effetCoutTerrainJoueur);
                          boardEnnemi=refreshBoardEnnemi(ren,dejavu,boardEnnemi,&provocationEnnemi,terrainEnnemi,&effetPVTerrainEnnemi,&effetAttTerrainEnnemi,&effetCoutTerrainEnnemi);
                      }
                      if (bufftmp[0]=='7') {
                          sscanf(bufftmp,"%d %d",&instruction,&idtmp);
                          cartetmp=idtocard(idtmp,ren,dejavu);
                          if (cartetmp->effetDirect(cartetmp,ren,dejavu,&provocationEnnemi,&boardEnnemi,&boardJoueur,NULL,NULL,NULL,&jeuEnnemi) != 1) {
                              printf("ERREUR LORS DU SORT DE %s\n",cartetmp->nom);
                          }
                          manaEnnemi-=cartetmp->cout+effetCoutTerrainEnnemi;
                          refresh=SDL_TRUE;
                          boardJoueur=refreshBoardJoueur(ren,dejavu,boardJoueur,&provocationJoueur,terrainJoueur,&effetPVTerrainJoueur,&effetAttTerrainJoueur,&effetCoutTerrainJoueur);
                          boardEnnemi=refreshBoardEnnemi(ren,dejavu,boardEnnemi,&provocationEnnemi,terrainEnnemi,&effetPVTerrainEnnemi,&effetAttTerrainEnnemi,&effetCoutTerrainEnnemi);
                      }

                      if (bufftmp[0]=='8') {
                          sscanf(bufftmp,"%d %d %d",&instruction,&idtmp,&idtmp2);
                          sorttmp=idtocard(idtmp,ren,dejavu);
                          cartetmp=inBoard(idtmp2,boardJoueur);
                          if (cartetmp==NULL) {
                              cartetmp=inBoard(idtmp2,boardEnnemi);
                          }
                          if (sorttmp->effetDirect(cartetmp,ren,dejavu,&provocationEnnemi,&boardEnnemi,&boardJoueur,NULL,NULL,NULL,&jeuEnnemi) != 1) {
                              printf("ERREUR LORS DU SORT DE %s\n",cartetmp->nom);
                          }
                          manaEnnemi-=sorttmp->cout+effetCoutTerrainEnnemi;
                          refresh=SDL_TRUE;
                          boardJoueur=refreshBoardJoueur(ren,dejavu,boardJoueur,&provocationJoueur,terrainJoueur,&effetPVTerrainJoueur,&effetAttTerrainJoueur,&effetCoutTerrainJoueur);
                          boardEnnemi=refreshBoardEnnemi(ren,dejavu,boardEnnemi,&provocationEnnemi,terrainEnnemi,&effetPVTerrainEnnemi,&effetAttTerrainEnnemi,&effetCoutTerrainEnnemi);
                          sorttmp=NULL;
                      }

                      if (bufftmp[0]=='9') {
                          victory=1;
                      }

                      buffer[i]='\0';
                      i++;
                  }
                  else {
                      i++;
                  }
              }
            }

        }
    }
}
