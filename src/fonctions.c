#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "carte.h"
#include "string.h"

SDL_Texture * loadPictures(SDL_Renderer* ren,char * nomFichier) {
    SDL_Surface * Simage = NULL;
    SDL_Texture * Timage = NULL;
    Simage=IMG_Load(nomFichier);
    if (Simage==NULL) {
        SDL_Log("Unable to load image :%s",SDL_GetError());
        return NULL;
    }
    Timage=SDL_CreateTextureFromSurface(ren,Simage);
    SDL_FreeSurface(Simage);
    return Timage;
}

SDL_Texture * actualiserTexteEcrit(SDL_Renderer* ren,SDL_Texture * Ttexte,char * text,SDL_Rect * Rtext,TTF_Font * dejavu) {
    SDL_Color coulTextEcrit={0,0,0,255};
    if (Ttexte!=NULL) {
        SDL_DestroyTexture(Ttexte);
    }
    SDL_Surface * tmp;
    tmp=TTF_RenderText_Solid(dejavu,text,coulTextEcrit);
    Ttexte=SDL_CreateTextureFromSurface(ren,tmp);
    SDL_FreeSurface(tmp);
    Rtext->w=14*strlen(text);//strlen(text)*10;
    return Ttexte;
}

Lcarte loadDeck(char * nomFichier,SDL_Renderer * ren,TTF_Font * dejavu) {
    Lcarte deck=NULL;
    int tmp=0;
    FILE * f;
    f=fopen(nomFichier,"r");
    if (f==NULL) {
        return NULL;
    }
    while ((fscanf(f,"%d",&tmp)==1)&&(tmp!=0)) {
        deck=ajoutTete(deck,idtocard(tmp,ren,dejavu));
        tmp=0;
    }
    if (fclose(f)!=0) {
        return NULL;
    }
    return deck;
}

int sauvDeck(char * nomFichier,Lcarte deck) {
    FILE * f;
    f=fopen(nomFichier,"w");
    if (f==NULL) {
          return 1;
    }
    while (deck!=NULL) {
        fprintf(f,"%d ",deck->carte->id);
        deck=deck->suiv;
    }
    if (fclose(f)!=0) {
        return 1;
    }
    return 0;
}

char * textInput(char * instructions,TTF_Font * dejavu) {
    SDL_Window * fen;
    SDL_Renderer * ren;
    SDL_Color coulTextBout={255,255,255,255};

    //Creation de la fenetre et du renderer

    fen = SDL_CreateWindow("Requête (qu'on me rende ma quequette)" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 300 , 200 , 0);
    if(fen == NULL){
        printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
        return NULL;
    }
    ren = SDL_CreateRenderer(fen, -1, SDL_RENDERER_SOFTWARE);
    if(ren == NULL) {
        printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
        return NULL;
    }

    //zone Instructions
    SDL_Rect zoneInstructions;
    zoneInstructions.w=220;
    zoneInstructions.h=70;
    zoneInstructions.x=40;
    zoneInstructions.y=15;

    //zone Texte
    SDL_Rect zoneTexte;
    zoneTexte.w=220;
    zoneTexte.h=70;
    zoneTexte.x=0;
    zoneTexte.y=0;

    //Texte
    SDL_Rect Rtext;
    Rtext.w=218;
    Rtext.h=68;
    Rtext.x=41;
    Rtext.y=116;

    //Texte
    SDL_Texture * Ttexte=NULL;
    Ttexte=actualiserTexteEcrit(ren,Ttexte,"",&Rtext,dejavu);
    //rectangle blanc
    SDL_Texture * TzoneTexte=NULL;
    TzoneTexte=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,220,70);
    if (TzoneTexte==NULL) {
      SDL_Log("Unable to create texture:%s",SDL_GetError());
      return NULL;
    }
    if (SDL_SetRenderDrawColor(ren,255,255,255,255)<0) {
        printf("Erreur lors du changement de couleur : %s",SDL_GetError());
        return NULL;
    }
    SDL_SetRenderTarget(ren,TzoneTexte);
    SDL_RenderFillRect(ren,&zoneTexte);
    SDL_SetRenderTarget(ren,NULL);
    zoneTexte.x=40;
    zoneTexte.y=115;

    //Affichage des instructions
    SDL_Surface * Sinstructions;
    SDL_Texture * Tinstructions;
    Sinstructions=TTF_RenderText_Solid(dejavu,instructions,coulTextBout);
    if (!Sinstructions) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    Tinstructions=SDL_CreateTextureFromSurface(ren,Sinstructions);
    SDL_FreeSurface(Sinstructions);
    if (Tinstructions==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return NULL;
    }

    //Boucle d'affichage
    SDL_Event event;
    SDL_StartTextInput();
    char text[16];

    while (1) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {

                //Pour quitter
                case SDL_QUIT:
                    SDL_DestroyTexture(Tinstructions);
                    SDL_DestroyRenderer(ren);
                    SDL_DestroyWindow(fen);
                    printf("GROS CHIBRE");
                    return NULL;
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event==SDL_WINDOWEVENT_CLOSE) {
                        SDL_DestroyTexture(Tinstructions);
                        SDL_DestroyRenderer(ren);
                        SDL_DestroyWindow(fen);
                        return NULL;
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym==SDLK_RETURN) {
                      SDL_DestroyTexture(Tinstructions);
                      SDL_DestroyRenderer(ren);
                      SDL_DestroyWindow(fen);
                      return strdup(text);
                    }
                    if (event.key.keysym.sym==SDLK_BACKSPACE) {
                        if (strlen(text)!=0) {
                            text[strlen(text)-1]='\0';
                        }
                        Ttexte=actualiserTexteEcrit(ren,Ttexte,text,&Rtext,dejavu);
                    }
                    break;

                case SDL_TEXTINPUT:
                /* Add new text onto the end of our text */
                  if (strlen(text)<15){
                      strcat(text, event.text.text);
                  }
                  Ttexte=actualiserTexteEcrit(ren,Ttexte,text,&Rtext,dejavu);
                  break;

                  //Cas général
                default:
                    break;
            }
            //Affichage du menu
              //Couleur du fond
            if (SDL_SetRenderDrawColor(ren,0,0,0,255)<0) {
                printf("Erreur lors du changement de couleur : %s",SDL_GetError());
                return NULL;
            }
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren,Tinstructions,NULL,&zoneInstructions);
            SDL_RenderCopy(ren,TzoneTexte,NULL,&zoneTexte);
            SDL_RenderCopy(ren,Ttexte,NULL,&Rtext);
            SDL_RenderPresent(ren);
        }
    }
}
