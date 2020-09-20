#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include "fonctions.h"

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
extern int isOnFullscreen;

void libereTextureOptions(SDL_Texture *zoneOptions, SDL_Texture *boutonFullscreen, SDL_Texture * boutonFenetre, SDL_Texture *boutonRetour)
{
    SDL_DestroyTexture(zoneOptions);
    SDL_DestroyTexture(boutonFullscreen);
    SDL_DestroyTexture(boutonFenetre);
    SDL_DestroyTexture(boutonRetour);
}

void refreshTextureSizesOptions(SDL_Rect *RZoneOptions, SDL_Rect *RBoutonFullscreen, SDL_Rect *RBoutonFenetre, SDL_Rect *RBoutonRetour)
{
    *RZoneOptions = initRect(winW / 12, winH / 12, (winW * 5) / 6, (winH * 5) / 6);
    *RBoutonFullscreen = initRect(winW / 3, winH / 3 - winH / 20, winW / 3, winH / 8);
    *RBoutonFenetre = initRect(winW / 3, winH / 3 - winH / 20, winW / 3, winH / 8);
    *RBoutonRetour = initRect((winW * 52) / 60, winH / 30, (winH * 7) / 60, (winH * 7) / 60);
}

int options()
{

    SDL_Rect RZoneOptions;
    SDL_Rect RBoutonFullscreen;
    SDL_Rect RBoutonFenetre;
    SDL_Rect RBoutonRetour;
    refreshTextureSizesOptions(&RZoneOptions, &RBoutonFullscreen, &RBoutonFenetre, &RBoutonRetour);

    SDL_Texture *zoneOptions = createBouton(RZoneOptions.w, RZoneOptions.h, coulFondDeck, coulFondDeck, " ");
    SDL_Texture *boutonFullscreen = createBouton(RBoutonFullscreen.w, RBoutonFullscreen.h, coulFondBout, coulTextBout, "Fullscreen");
    SDL_Texture *boutonFenetre = createBouton(RBoutonFenetre.w, RBoutonFenetre.h, coulFondBout, coulTextBout, "Fenetré");
    SDL_Texture *boutonRetour = NULL;
    boutonRetour = loadPictures("image/menu/bouton_retour.png");
    if (boutonRetour == NULL)
    {
        SDL_Log("Unable to create texture from image :%s", SDL_GetError());
        return 1;
    }

    //Boucle d'affichage
    SDL_Point mousePos;
    SDL_Event event;
    int swollen = 0;
    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            //Pour quitter
            case SDL_QUIT:
                libereTextureOptions(zoneOptions, boutonFullscreen,boutonFenetre, boutonRetour);
                return 1;
                break;

            //En cas de clic
            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = event.button.x;
                mousePos.y = event.button.y;

                if (SDL_PointInRect(&mousePos, &RBoutonRetour))
                {
                    Mix_PlayChannel(-1, selectSE, 0);
                    libereTextureOptions(zoneOptions, boutonFullscreen,boutonFenetre, boutonRetour);
                    return 0;
                }

                if (SDL_PointInRect(&mousePos, &RBoutonFullscreen))
                {
                    Mix_PlayChannel(-1, selectSE, 0);
                    if (isOnFullscreen)
                    {
                        if (SDL_SetWindowFullscreen(win, 0) < 0)
                        {
                            printf("Erreur lors du passage de plein écran à fenetré : %s", SDL_GetError());
                            return 1;
                        }
                        isOnFullscreen = 0;
                    }
                    else
                    {
                        if (SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN) < 0)
                        {
                            printf("Erreur lors du passage de fenetré à plein écran : %s", SDL_GetError());
                            return 1;
                        }
                        isOnFullscreen = 1;
                    }
                }
                break;

            case SDL_MOUSEMOTION:
                mousePos.x = event.motion.x;
                mousePos.y = event.motion.y;

                //Gonflement du bouton Retour
                if (SDL_PointInRect(&mousePos, &RBoutonRetour))
                {
                    if (!swollen)
                    {
                        Mix_PlayChannel(-1, hover, 0);
                        swollen = 1;
                        swellRect(&RBoutonRetour);
                    }
                }
                //Gonflement du bouton fullscreen
                else
                {
                    if ((!isOnFullscreen) && SDL_PointInRect(&mousePos, &RBoutonFullscreen))
                    {
                        if (!swollen)
                        {
                            Mix_PlayChannel(-1, hover, 0);
                            swollen = 1;
                            swellRect(&RBoutonFullscreen);
                        }
                    }
                    else
                    {
                        if (isOnFullscreen && SDL_PointInRect(&mousePos, &RBoutonFenetre))
                        {
                            if (!swollen)
                            {
                                Mix_PlayChannel(-1, hover, 0);
                                swollen = 1;
                                swellRect(&RBoutonFenetre);
                            }
                        }
                        else
                        {
                            swollen = 0;
                            refreshTextureSizesOptions(&RZoneOptions, &RBoutonFullscreen, &RBoutonFenetre, &RBoutonRetour);
                        }
                    }
                }
                break;

            case SDL_WINDOWEVENT:
                if ((&event)->window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_GetWindowSize(win, &winW, &winH);
                    refreshTextureSizesOptions(&RZoneOptions, &RBoutonFullscreen, &RBoutonFenetre, &RBoutonRetour);
                }
                break;

            default:
                break;
            }

            clearScreen();

            SDL_RenderCopy(ren, zoneOptions, NULL, &RZoneOptions);
            SDL_RenderCopy(ren, boutonRetour, NULL, &RBoutonRetour);
            
            if (!isOnFullscreen) {
                SDL_RenderCopy(ren, boutonFullscreen, NULL, &RBoutonFullscreen);
            }
            else {
                SDL_RenderCopy(ren, boutonFenetre , NULL, &RBoutonFenetre);
            }
            SDL_RenderPresent(ren);
        }
    }
}
