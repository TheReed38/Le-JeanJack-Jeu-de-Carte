#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include "fonctions.h"

void libereTextureOptions(SDL_Texture * zoneDecks,SDL_Texture * bout1,SDL_Texture * TtextBout1,SDL_Texture * boutonRetour) {
    SDL_DestroyTexture(zoneDecks);
    SDL_DestroyTexture(bout1);
    SDL_DestroyTexture(TtextBout1);
    SDL_DestroyTexture(boutonRetour);
}

int options(SDL_Window * fen, SDL_Renderer * ren,TTF_Font * dejavu,Uint32 * fullscreen,Mix_Chunk * hover,Mix_Chunk * select) {

    int winW;
    int winH;

    SDL_GetWindowSize(fen,&winW,&winH);

    SDL_Color coulTextBout={255,255,255,255};

    //Zone d'affichage des decks déjà créés
    SDL_Texture * zoneDecks = NULL;

    zoneDecks=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,(winW*5)/6,(winH*5)/6);
    if (zoneDecks==NULL) {
        SDL_Log("Unable to create texture:%s",SDL_GetError());
        return 1;
    }
    SDL_Rect RzoneDecks;
    RzoneDecks.x=0;
    RzoneDecks.y=0;
    RzoneDecks.w=(winW*5)/6;
    RzoneDecks.h=(winH*5)/6;
    SDL_SetRenderTarget(ren,zoneDecks);
    SDL_RenderFillRect(ren,&RzoneDecks);
    SDL_SetRenderTarget(ren,NULL);
    RzoneDecks.x=winW/12;
    RzoneDecks.y=winH/12;

    //Bouton 1 FullScreen

    SDL_Texture * bout1 = NULL;
    SDL_Texture * TtextBout1 = NULL;
    SDL_Surface * textBout1 = NULL;
    textBout1=TTF_RenderText_Solid(dejavu,"FullScreen",coulTextBout);
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
    RtextBout1.x=(winW*21)/60;
    RtextBout1.w=(winW*9)/30;
    RtextBout1.h=winH/10;
    RtextBout1.y=winH/3+(winH/100)-(winH/20);
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
    RBout1.x=winW/3;
    RBout1.y=winH/3-(winH/20);
    SDL_SetRenderTarget(ren,NULL);

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

    //Boucle d'affichage
    SDL_Point mousePos;
    SDL_Event event;
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
                        libereTextureOptions(zoneDecks,bout1,TtextBout1,boutonRetour);
                        return 0;
                    }
                    if (SDL_PointInRect(&mousePos,&RBout1)) {
                        Mix_PlayChannel(-1,select,0);
                        if(SDL_SetWindowFullscreen(fen , *fullscreen) <0)
                        {
                            printf("Erreur lors du passage en mode plein ecran : %s",SDL_GetError());
                            return 1;
                        }
                        if (*fullscreen) {
                            *fullscreen=0;
                        }
                        else {
                            *fullscreen=SDL_WINDOW_FULLSCREEN;
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

                    //Gonflement du bouton fullscreen
                    else if (SDL_PointInRect(&mousePos,&RBout1)) {
                        if (!soundPlayed) {
                            Mix_PlayChannel(-1,hover,0);
                            soundPlayed=1;
                        }
                        RBout1.x=(winW/3)-(winW/120);
                        RBout1.y=(winH/3)-(winH/200)-(winH/20);
                        RBout1.w=(winW/3)+(winW/60);
                        RBout1.h=(winH/8)+(winH/100);
                        RtextBout1.x=(winW*21)/60-(winW/120);
                        RtextBout1.y=winH/3+(winH/100)-(winH/200)-(winH/20);
                        RtextBout1.w=(winW*9)/30+(winW/60);
                        RtextBout1.h=winH/10+(winH/100);
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
            //Fond zone de Decks
            SDL_RenderCopy(ren,zoneDecks,NULL,&RzoneDecks);

            //Bouton Retour
            SDL_RenderCopy(ren,boutonRetour,NULL,&RboutonRetour);

            //Bouton FullScreen
            SDL_RenderCopy(ren,bout1,NULL,&RBout1);
            SDL_RenderCopy(ren,TtextBout1,NULL,&RtextBout1);

            //Rendu
            SDL_RenderPresent(ren);

            //Rafraichissement des textures en cas de modifications de taille de la fenêtre
            SDL_GetWindowSize(fen,&winW,&winH);
            //Zone decks
            RzoneDecks.x=winW/12;
            RzoneDecks.y=winH/12;
            RzoneDecks.w=(winW*5)/6;
            RzoneDecks.h=(winH*5)/6;
            //Bouton fullscreen
            RBout1.x=winW/3;
            RBout1.w=winW/3;
            RBout1.h=winH/8;
            RBout1.y=winH/3-(winH/20);
            RtextBout1.x=(winW*21)/60;
            RtextBout1.w=(winW*9)/30;
            RtextBout1.h=winH/10;
            RtextBout1.y=winH/3+(winH/100)-(winH/20);
            //Bouton retour
            RboutonRetour.x=(winW*52)/60;
            RboutonRetour.y=(winH/30);
            RboutonRetour.w=RboutonRetour.h=(winH*7)/60;
        }

    }
}
