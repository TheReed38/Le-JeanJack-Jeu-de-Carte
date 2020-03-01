#include "SDL.h"
#include "SDL_image.h"

#include "fonctions.h"

void libereTextureDecks(SDL_Texture * zoneDecks,SDL_Texture * bout1,SDL_Texture * TtextBout1,SDL_Texture * boutonRetour) {
    SDL_DestroyTexture(zoneDecks);
    SDL_DestroyTexture(boutonRetour);
    SDL_DestroyTexture(bout1);
    SDL_DestroyTexture(TtextBout1);
}

void afficheDeck(SDL_Renderer * ren,Lcarte jeuJoueur,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh) {
    int posYcarte =0;
    SDL_Rect carte;
    carte.x=0;
    carte.w=(winW/4);
    carte.h=(winH/30);
    while (jeuJoueur!=NULL) {
        carte.y=posYcarte;
        //SDL_QueryTexture(jeuJoueur->carte->Tcarte,NULL,NULL,&carte.w,&carte.h);
        //SDL_RenderCopy(ren,jeuJoueur->carte->Tcarte,NULL,&carte);
        if (jeuJoueur->Rcarte!=NULL) {
            free(jeuJoueur->Rcarte);
        }
        jeuJoueur->Rcarte=copieRect(&carte);
        if (refresh) {
            creerTextureReduiteCarte(ren,jeuJoueur->carte,dejavu );
            SDL_RenderCopy(ren,jeuJoueur->carte->TcarteReduite,NULL,&carte);
        }
        else {
            SDL_RenderCopy(ren,jeuJoueur->carte->TcarteReduite,NULL,&carte);
        }
        jeuJoueur=jeuJoueur->suiv;
        posYcarte+=winH/30;
    }
}

void afficheToutesLesCartes(SDL_Renderer * ren,Lcarte toutesLesCartes,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh,int page) {
    int posXcarte = (winW/4)+(winW*7)/120;
    int distanceInterCartes = (winW/5)+(winW/60);
    SDL_Rect carte;
    carte.y=(winH*3)/12;
    carte.w=(winW/5);//60
    carte.h=(winH/3);//100
    int i=0;
    while (toutesLesCartes!=NULL) {
        if ((i>=page*3)&&(i<(page+1)*3)) {
          carte.x=posXcarte;
          //SDL_QueryTexture(jeuJoueur->carte->Tcarte,NULL,NULL,&carte.w,&carte.h);
          //SDL_RenderCopy(ren,jeuJoueur->carte->Tcarte,NULL,&carte);
          if (toutesLesCartes->Rcarte!=NULL) {
              free(toutesLesCartes->Rcarte);
          }
          toutesLesCartes->Rcarte=copieRect(&carte);
          if (refresh) {
              refreshCarte(ren,toutesLesCartes->carte,dejavu,&carte,0,0,0);
          }
          else {
              SDL_RenderCopy(ren,toutesLesCartes->carte->TcarteComplet,NULL,&carte);
          }
          posXcarte+=distanceInterCartes;
        }
        else {
          toutesLesCartes->Rcarte=NULL;
        }
        toutesLesCartes=toutesLesCartes->suiv;
        i+=1;
    }
}

int deck(SDL_Window * fen, SDL_Renderer * ren,TTF_Font * dejavu,Mix_Chunk * hover, Mix_Chunk * select) {

    int winW;
    int winH;

    int page;
    SDL_bool refresh = SDL_TRUE;
    SDL_bool refreshDeck = SDL_FALSE;
    Lcarte toutesLesCartes=NULL;

    SDL_GetWindowSize(fen,&winW,&winH);

    SDL_Color coulTextBout={255,255,255,255};

    //Zone d'affichage du deck en cours de Creation
    SDL_Texture * zoneDeckCree = NULL;
    zoneDeckCree=SDL_CreateTexture(ren,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,winW/6,winH);
    if (zoneDeckCree==NULL) {
        SDL_Log("Unable to create texture:%s",SDL_GetError());
        return 1;
    }
    SDL_Rect RzoneDeckCree;
    RzoneDeckCree.x=0;
    RzoneDeckCree.y=0;
    RzoneDeckCree.w=winW/4;
    RzoneDeckCree.h=winH;
    SDL_SetRenderTarget(ren,zoneDeckCree);
    SDL_RenderFillRect(ren,&RzoneDeckCree);
    SDL_SetRenderTarget(ren,NULL);

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

    //Bouton Suivant
    SDL_Texture * boutonSuivant=NULL;
    boutonSuivant=loadPictures(ren,"image/menu/suivant.png");
    if (boutonRetour==NULL) {
        SDL_Log("Unable to create texture from image :%s",SDL_GetError());
        return 1;
    }
    SDL_Rect RboutonSuivant;
    RboutonSuivant.x=(winW/4)+(3*winW)/8+(winW/60);
    RboutonSuivant.y=(winH*2)/3;
    RboutonSuivant.w=RboutonSuivant.h=(winW/10);

    //Bouton Precedent
    SDL_Texture * boutonPrecedent=NULL;
    boutonPrecedent=loadPictures(ren,"image/menu/precedent.png");
    if (boutonPrecedent==NULL) {
        SDL_Log("Unable to create texture from image :%s",SDL_GetError());
        return 1;
    }
    SDL_Rect RboutonPrecedent;
    RboutonPrecedent.x=(winW/4)+(3*winW)/8-(winW/60)-(winW/10);
    RboutonPrecedent.y=(winH*2)/3;
    RboutonPrecedent.w=RboutonPrecedent.h=(winW/10);

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

    //Bouton 1 Nouveau Deck

    SDL_Texture * bout1 = NULL;
    SDL_Texture * TtextBout1 = NULL;
    SDL_Surface * textBout1 = NULL;
    textBout1=TTF_RenderText_Solid(dejavu,"Creer Deck",coulTextBout);
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
    SDL_SetRenderTarget(ren,bout1);
    if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
        printf("Erreur lors du changement de couleur : %s",SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_RenderFillRect(ren,&RBout1);
    RBout1.x=winW/8;
    RBout1.y=(winH*49)/60;
    SDL_SetRenderTarget(ren,NULL);

    //Bouton 2 Sauvergarder Deck

    SDL_Texture * bout2 = NULL;
    SDL_Texture * TtextBout2 = NULL;
    SDL_Surface * textBout2 = NULL;
    textBout2=TTF_RenderText_Solid(dejavu,"Sauvegarder Deck",coulTextBout);
    if (!textBout2) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    bout2=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,winW/3,winH/8);
    if (bout2==NULL) {
      SDL_Log("Unable to create texture:%s",SDL_GetError());
      return 1;
    }
    TtextBout2=SDL_CreateTextureFromSurface(ren,textBout2);
    SDL_FreeSurface(textBout2);
    if (TtextBout2==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return 1;
    }
    SDL_Rect RBout2;
    SDL_Rect RtextBout2;
    RtextBout2.x=winW-winW/8-winW/3+winW/60;
    RtextBout2.w=(winW*9)/30;
    RtextBout2.h=winH/10;
    RtextBout2.y=(winH*49)/60+(winH/100);
    RBout2.x=0;
    RBout2.y=0;
    RBout2.w=winW/3;
    RBout2.h=winH/8;
    SDL_SetRenderTarget(ren,bout2);
    if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
        printf("Erreur lors du changement de couleur : %s",SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_RenderFillRect(ren,&RBout2);
    RBout2.x=winW-winW/8-winW/3;
    RBout2.y=(winH*49)/60;
    SDL_SetRenderTarget(ren,NULL);

    //Bouton 3 Supprimer Deck

    SDL_Texture * bout3 = NULL;
    SDL_Texture * TtextBout3 = NULL;
    SDL_Surface * textBout3 = NULL;
    textBout3=TTF_RenderText_Solid(dejavu,"Supprimer Deck",coulTextBout);
    if (!textBout3) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    bout3=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,winW/3,winH/8);
    if (bout3==NULL) {
      SDL_Log("Unable to create texture:%s",SDL_GetError());
      return 1;
    }
    TtextBout3=SDL_CreateTextureFromSurface(ren,textBout3);
    SDL_FreeSurface(textBout3);
    if (TtextBout3==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return 1;
    }
    SDL_Rect RBout3;
    SDL_Rect RtextBout3;
    RtextBout3.x=winW-winW/8-winW/3+winW/60;
    RtextBout3.w=(winW*9)/30;
    RtextBout3.h=winH/10;
    RtextBout3.y=(winH*49)/60+(winH/100);
    RBout3.x=0;
    RBout3.y=0;
    RBout3.w=winW/3;
    RBout3.h=winH/8;
    SDL_SetRenderTarget(ren,bout3);
    if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
        printf("Erreur lors du changement de couleur : %s",SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_RenderFillRect(ren,&RBout3);
    RBout3.x=winW-winW/8-winW/3;
    RBout3.y=(winH*49)/60;
    SDL_SetRenderTarget(ren,NULL);

    //Variables

    int creation=0;
    Lcarte newDeck=NULL;
    int tailleNewDeck=0;
    int noDeck=0;
    int modify=0;
    int supprimerDeck=0;

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



    //Boucle d'affichage
    SDL_Point mousePos;
    SDL_Event event;
    Carte * cartetmp;
    Carte * carteAjoute;
    char nomFichier[10];
    int soundPlayed = 1;
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
                    if (SDL_PointInRect(&mousePos,&RboutonRetour)) {
                      Mix_PlayChannel(-1,select,0);
                      if (deck1!=NULL) {
                        libereListeCarte(deck1);
                        deck1=NULL;
                      }
                      if (deck2!=NULL) {
                        libereListeCarte(deck2);
                        deck2=NULL;
                      }
                      if (deck3!=NULL) {
                        libereListeCarte(deck3);
                        deck3=NULL;
                      }
                        libereTextureDecks(zoneDecks,bout1,TtextBout1,boutonRetour);
                        return 0;
                    }
                    if (SDL_PointInRect(&mousePos,&RBout1)) {
                        Mix_PlayChannel(-1,select,0);
                        toutesLesCartes=creerListeCarte(ren,dejavu);
                        newDeck=NULL;
                        page=0;
                        creation=1;
                        supprimerDeck=0;
                    }
                    if (SDL_PointInRect(&mousePos,&RBout2)) {
                        Mix_PlayChannel(-1,select,0);
                        if (modify) {
                            sprintf(nomFichier,"deck%d",modify);
                        }
                        else {
                            noDeck+=1;
                            sprintf(nomFichier,"deck%d",noDeck);
                        }
                        if (sauvDeck(nomFichier,newDeck)==1) {
                          printf("ERREUR SAUVERGARDE DECK\n");
                          return 1;
                        }

                        creation=0;
                        libereListeCarte(toutesLesCartes);
                        toutesLesCartes=NULL;
                        if (modify!=0) {
                          libereListeCarte(newDeck);
                          newDeck=NULL;
                        }
                        modify=0;
                        tailleNewDeck=0;
                        refreshDeck=SDL_TRUE;
                    }
                    if (SDL_PointInRect(&mousePos,&RBout3)) {
                        Mix_PlayChannel(-1,select,0);
                        supprimerDeck=1;
                    }
                    if (SDL_PointInRect(&mousePos,&RboutonSuivant)) {
                        Mix_PlayChannel(-1,select,0);
                        page+=1;
                        refresh=SDL_TRUE;
                    }
                    if (SDL_PointInRect(&mousePos,&RboutonPrecedent)) {
                        Mix_PlayChannel(-1,select,0);
                        page-=1;
                        refresh=SDL_TRUE;
                    }

                    if (SDL_PointInRect(&mousePos,&RboutDeck1)) {
                      Mix_PlayChannel(-1,select,0);
                      if (supprimerDeck) {
                        if (deck3!=NULL) {
                            libereListeCarte(deck1);
                            remove("deck1");
                            deck1=deck2;
                            deck2=deck3;
                            rename("deck2","deck1");
                            rename("deck3","deck2");
                            deck3=NULL;
                        }
                        else {
                          if (deck2!=NULL) {
                              libereListeCarte(deck1);
                              remove("deck1");
                              deck1=deck2;
                              deck2=NULL;
                              rename("deck2","deck1");
                          }
                          else {
                              remove("deck1");
                              libereListeCarte(deck1);
                              deck1=NULL;
                          }
                        }
                        noDeck-=1;
                        supprimerDeck=0;
                      }
                      else {
                        toutesLesCartes=creerListeCarte(ren,dejavu);
                        newDeck=deck1;
                        deck1=NULL;
                        creation=1;
                        page=0;
                        tailleNewDeck=len(newDeck);
                        refresh=SDL_TRUE;
                        modify=1;
                      }
                    }
                    if (SDL_PointInRect(&mousePos,&RboutDeck2)) {
                      Mix_PlayChannel(-1,select,0);
                      if (supprimerDeck) {
                        if (deck3!=NULL) {
                            libereListeCarte(deck2);
                            remove("deck2");
                            deck2=deck3;
                            deck3=NULL;
                            rename("deck3","deck2");
                        }
                        else {
                            remove("deck2");
                            libereListeCarte(deck2);
                            deck2=NULL;
                        }
                        noDeck-=1;
                        supprimerDeck=0;
                      }
                      else {
                        toutesLesCartes=creerListeCarte(ren,dejavu);
                        newDeck=deck2;
                        deck2=NULL;
                        creation=1;
                        page=0;
                        tailleNewDeck=len(newDeck);
                        refresh=SDL_TRUE;
                        modify=2;
                      }
                    }
                    if (SDL_PointInRect(&mousePos,&RboutDeck3)) {
                      Mix_PlayChannel(-1,select,0);
                      if (supprimerDeck) {
                          remove("deck3");
                          libereListeCarte(deck3);
                          deck3=NULL;
                          noDeck-=1;
                          supprimerDeck=0;
                      }
                      else {
                        toutesLesCartes=creerListeCarte(ren,dejavu);
                        newDeck=deck3;
                        deck3=NULL;
                        creation=1;
                        page=0;
                        tailleNewDeck=len(newDeck);
                        refresh=SDL_TRUE;
                        modify=3;
                      }
                    }

                    if (event.button.button==SDL_BUTTON_LEFT && creation) {
                        Mix_PlayChannel(-1,select,0);
                        cartetmp=carteClique(&mousePos,toutesLesCartes,NULL,NULL);
                        if (cartetmp!=NULL && tailleNewDeck<30) {
                          carteAjoute=calloc(1,sizeof(*carteAjoute));
                          if (carteAjoute==NULL) {
                              printf("\nImpossible d'ajouter carteReduite (erreur calloc)\n");
                              return 1;
                          }
                          carteAjoute->id=cartetmp->id;
                          carteAjoute->nom=strdup(cartetmp->nom);
                          carteAjoute->TcarteReduite=NULL;
                            newDeck=ajoutTete(newDeck,carteAjoute);
                            tailleNewDeck+=1;
                            refresh=SDL_TRUE;
                        }
                        else {
                            cartetmp=carteClique(&mousePos,newDeck,NULL,NULL);
                            if (cartetmp!=NULL) {
                                newDeck=retirerCarte(newDeck,cartetmp);
                                tailleNewDeck-=1;
                                refresh=SDL_TRUE;
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

                    //Gonflement du Bouton 2
                    else if (SDL_PointInRect(&mousePos,&RBout2)) {
                      if (!soundPlayed) {
                          Mix_PlayChannel(-1,hover,0);
                          soundPlayed=1;
                      }
                      RBout2.x=winW-winW/8-winW/3-(winW/120);
                      RBout2.y=(winH*49)/60-(winH/200);
                      RBout2.w=winW/3+(winW/60);
                      RBout2.h=winH/8+(winH/100);
                      RtextBout2.x=winW-winW/8-winW/3+winW/60-(winW/120);
                      RtextBout2.w=(winW*9)/30+(winW/60);
                      RtextBout2.h=winH/10+(winH/100);
                      RtextBout2.y=(winH*49)/60+(winH/100)-(winH/200);
                    }

                    //Gonflement du Bouton 3
                    else if (SDL_PointInRect(&mousePos,&RBout3)) {
                      if (!soundPlayed) {
                          Mix_PlayChannel(-1,hover,0);
                          soundPlayed=1;
                      }
                      RBout3.x=winW-winW/8-winW/3-(winW/120);
                      RBout3.y=(winH*49)/60-(winH/200);
                      RBout3.w=winW/3+(winW/60);
                      RBout3.h=winH/8+(winH/100);
                      RtextBout3.x=winW-winW/8-winW/3+winW/60-(winW/120);
                      RtextBout3.w=(winW*9)/30+(winW/60);
                      RtextBout3.h=winH/10+(winH/100);
                      RtextBout3.y=(winH*49)/60+(winH/100)-(winH/200);
                    }

                    //Gonflement des boutons suivants et precedent
                    else if (SDL_PointInRect(&mousePos,&RboutonSuivant)) {
                      if (!soundPlayed) {
                          Mix_PlayChannel(-1,hover,0);
                          soundPlayed=1;
                      }
                      RboutonSuivant.x=(winW/4)+(3*winW)/8+(winW/60)-(winW/120);
                      RboutonSuivant.y=(winH*2)/3-(winW/120);
                      RboutonSuivant.w=RboutonSuivant.h=(winW/10)+(winW/60);
                    }
                    else if (SDL_PointInRect(&mousePos,&RboutonPrecedent)) {
                      if (!soundPlayed) {
                          Mix_PlayChannel(-1,hover,0);
                          soundPlayed=1;
                      }
                      RboutonPrecedent.x=(winW/4)+(3*winW)/8-(winW/60)-(winW/10)-(winW/120);
                      RboutonPrecedent.y=(winH*2)/3-(winW/120);
                      RboutonPrecedent.w=RboutonPrecedent.h=(winW/10)+(winW/60);
                    }
                    else {
                      soundPlayed=0;
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
            //Fond zone de Decks


            if (creation) {
              SDL_RenderCopy(ren,zoneDeckCree,NULL,&RzoneDeckCree);
              afficheToutesLesCartes(ren,toutesLesCartes,winW,winH,dejavu,refresh,page);
              afficheDeck(ren,newDeck,winW,winH,dejavu,refresh);
              SDL_RenderCopy(ren,bout2,NULL,&RBout2);
              SDL_RenderCopy(ren,TtextBout2,NULL,&RtextBout2);
              if (page<20) {
                SDL_RenderCopy(ren,boutonSuivant,NULL,&RboutonSuivant);
              }
              if (page>0) {
                SDL_RenderCopy(ren,boutonPrecedent,NULL,&RboutonPrecedent);
              }
            }
            else {
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
              if (noDeck < 3) {
                //Bouton 1
                SDL_RenderCopy(ren,bout1,NULL,&RBout1);
                SDL_RenderCopy(ren,TtextBout1,NULL,&RtextBout1);
              }
              if (noDeck>0) {
                //Bouton 3
                SDL_RenderCopy(ren,bout3,NULL,&RBout3);
                SDL_RenderCopy(ren,TtextBout3,NULL,&RtextBout3);
              }
              //Bouton Retour
              SDL_RenderCopy(ren,boutonRetour,NULL,&RboutonRetour);
            }




            //Rendu
            SDL_RenderPresent(ren);

            if (refresh) {
                refresh=SDL_FALSE;
            }

            if (refreshDeck) {
              if (deck1!=NULL) {
                libereListeCarte(deck1);
                deck1=NULL;
              }
              if (deck2!=NULL) {
                libereListeCarte(deck2);
                deck2=NULL;
              }
              if (deck3!=NULL) {
                libereListeCarte(deck3);
                deck3=NULL;
              }
                deck1=loadDeck("deck1",ren,dejavu);
                deck2=loadDeck("deck2",ren,dejavu);
                deck3=loadDeck("deck3",ren,dejavu);
                refreshDeck=SDL_FALSE;
            }

            //Rafraichissement des textures en cas de modifications de taille de la fenêtre
            SDL_GetWindowSize(fen,&winW,&winH);
            //Zone decks
            RzoneDecks.x=winW/12;
            RzoneDecks.y=winH/12;
            RzoneDecks.w=(winW*5)/6;
            RzoneDecks.h=(winH*4)/6;
            //zone deck en cours de Creation
            RzoneDeckCree.x=0;
            RzoneDeckCree.y=0;
            RzoneDeckCree.w=winW/4;
            RzoneDeckCree.h=winH;
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
            //Bouton 2
            RtextBout2.x=winW-winW/8-winW/3+winW/60;
            RtextBout2.w=(winW*9)/30;
            RtextBout2.h=winH/10;
            RtextBout2.y=(winH*49)/60+(winH/100);
            RBout2.w=winW/3;
            RBout2.h=winH/8;
            RBout2.x=winW-winW/8-winW/3;
            RBout2.y=(winH*49)/60;
            //Bouton Suivant et précedent
            RboutonSuivant.x=(winW/4)+(3*winW)/8+(winW/60);
            RboutonSuivant.y=(winH*2)/3;
            RboutonSuivant.w=RboutonSuivant.h=(winW/10);
            RboutonPrecedent.x=(winW/4)+(3*winW)/8-(winW/60)-(winW/10);
            RboutonPrecedent.y=(winH*2)/3;
            RboutonPrecedent.w=RboutonPrecedent.h=(winW/10);
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
            //Bout3
            RtextBout3.x=winW-winW/8-winW/3+winW/60;
            RtextBout3.w=(winW*9)/30;
            RtextBout3.h=winH/10;
            RtextBout3.y=(winH*49)/60+(winH/100);
            RBout3.w=winW/3;
            RBout3.h=winH/8;
            RBout3.x=winW-winW/8-winW/3;
            RBout3.y=(winH*49)/60;

            if (creation) {
              RboutonRetour.w=RboutonRetour.h=0;
              if (page<=0) {
                RBout1.w=RBout1.h=RtextBout1.w=RtextBout1.h=RboutonPrecedent.w=RboutonPrecedent.h=0;
              }
              if (page>=20) {
                RBout1.w=RBout1.h=RtextBout1.w=RtextBout1.h=RboutonSuivant.w=RboutonSuivant.h=0;
              }
              RBout3.w=RBout3.h=RtextBout3.w=RtextBout3.h=RboutDeck3.w=RboutDeck3.h=RtextBoutDeck3.w=RtextBoutDeck3.h=RboutDeck2.w=RboutDeck2.h=RtextBoutDeck2.w=RtextBoutDeck2.h=RboutDeck1.w=RboutDeck1.h=RtextBoutDeck1.w=RtextBoutDeck1.h=0;
            }
            else {
              if (noDeck<1) {
                  RboutDeck1.w=RboutDeck1.h=RtextBoutDeck1.w=RtextBoutDeck1.h=0;
              }
              if (noDeck<2) {
                  RboutDeck2.w=RboutDeck2.h=RtextBoutDeck2.w=RtextBoutDeck2.h=0;
              }
              if (noDeck<3) {
                  RboutDeck3.w=RboutDeck3.h=RtextBoutDeck3.w=RtextBoutDeck3.h=0;
              }
              if (noDeck>=3) {
                  RBout1.w=RBout1.h=RtextBout1.w=RtextBout1.h=0;
              }
              RboutonSuivant.w=RboutonSuivant.h=RboutonPrecedent.w=RboutonPrecedent.h=RBout2.w=RBout2.h=RtextBout2.w=RtextBout2.h=0;
            }
        }
    }
}
