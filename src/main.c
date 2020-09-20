#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include "fonctions.h"

TTF_Font *dejavu;
SDL_Window *win;
int winW, winH;
int isOnFullscreen;
SDL_Renderer *ren;
Mix_Music *playTheme;
Mix_Chunk *hover;
Mix_Chunk *selectSE;
SDL_Color *coulTextBout;
SDL_Color *coulFondBout;
SDL_Color *coulFondDeck;
SDL_Color *coulFondEcran;

void refreshTextureSizesMain(SDL_Rect *RLogo, SDL_Rect *RBoutonJouer, SDL_Rect *RBoutonDecks, SDL_Rect *RBoutonOptions)
{
    *RLogo = initRect((winW * 5) / 12, winH / 30, winW / 6, winH / 6);
    *RBoutonJouer = initRect(winW / 3, winH / 3 - winH / 20, winW / 3, winH / 8);
    *RBoutonDecks = initRect(winW / 3, (winH * 7) / 12 - winH / 20, winW / 3, winH / 8);
    *RBoutonOptions = initRect(winW / 3, (winH * 5) / 6 - winH / 20, winW / 3, winH / 8);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow)
{
    (void)hInstance;
    (void)hPrevIns;
    (void)lpszArgument;
    (void)iShow;
    //Variables
    winW = 800;
    winH = 600;

    SDL_Rect RLogo;
    SDL_Rect RBoutonJouer;
    SDL_Rect RBoutonDecks;
    SDL_Rect RBoutonOptions;
    refreshTextureSizesMain(&RLogo, &RBoutonJouer, &RBoutonDecks, &RBoutonOptions);

    int charSize = 100;
    isOnFullscreen = 0;

    srand(time(NULL));

    coulTextBout = initColor(255, 255, 255, 255);
    coulFondBout = initColor(120, 120, 120, 255);
    coulFondDeck = initColor(255, 0, 0, 255);
    coulFondEcran = initColor(0, 0, 0, 255);

    //Initialisations
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
    {
        return 1;
    }
    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    //Chargement de la police
    dejavu = NULL;
    dejavu = TTF_OpenFont("Fonts/dejavu/DejaVuSans.ttf", charSize);
    if (!dejavu)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(2);
    }

    //Creation de la fenetre et du renderer
    win = NULL;
    win = SDL_CreateWindow("Le Jean Jack Jeu de Carte",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           winW,
                           winH,
                           SDL_WINDOW_RESIZABLE);
    if (win == NULL)
    {
        printf("Erreur lors de la creation d'une fenetre : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    ren = NULL;
    ren = SDL_CreateRenderer(win,
                             -1,
                             SDL_RENDERER_SOFTWARE);
    if (ren == NULL)
    {
        printf("Erreur lors de la creation d'un renderer : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    //Creation des textures
    //Logo de la JJT
    SDL_Texture *logo = loadPictures("logoRed.png");
    if (logo == NULL)
    {
        SDL_Log("Unable to create texture from image :%s", SDL_GetError());
        return 1;
    }
    SDL_QueryTexture(logo, NULL, NULL, &RLogo.w, &RLogo.h);

    //Boutons
    SDL_Texture *boutonJouer = createBouton(RBoutonJouer.w, RBoutonJouer.h, coulFondBout, coulTextBout, "Jouer");
    SDL_Texture *boutonDecks = createBouton(RBoutonDecks.w, RBoutonDecks.h, coulFondBout, coulTextBout, "Decks");
    SDL_Texture *boutonOptions = createBouton(RBoutonOptions.w, RBoutonOptions.h, coulFondBout, coulTextBout, "Options");

    //Musique et Effets
    Mix_Music *mainTheme = NULL;
    Mix_Music *lobbyTheme = NULL;
    Mix_Music *deckTheme = NULL;
    mainTheme = Mix_LoadMUS("sound/music/mainTheme.wav");
    deckTheme = Mix_LoadMUS("sound/music/deckTheme.wav");
    lobbyTheme = Mix_LoadMUS("sound/music/lobbyTheme.wav");
    playTheme = Mix_LoadMUS("sound/music/battle.wav");
    hover = Mix_LoadWAV("sound/se/click1.ogg");
    selectSE = Mix_LoadWAV("sound/se/click2.ogg");

    if (Mix_PlayMusic(mainTheme, -1) == -1)
    {
        return 1;
    }

    //Boucle principale
    SDL_Point mousePos;
    SDL_Event event;

    int done = 0;
    int swollen = 0;

    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = event.button.x;
                mousePos.y = event.button.y;

                if (SDL_PointInRect(&mousePos, &RBoutonJouer))
                {
                    Mix_PlayChannel(-1, selectSE, 0);
                    if (Mix_PlayMusic(lobbyTheme, -1) == -1)
                    {
                        return 1;
                    }
                    done = lobby();
                    refreshTextureSizesMain(&RLogo, &RBoutonJouer, &RBoutonDecks, &RBoutonOptions);
                    if (Mix_PlayMusic(mainTheme, -1) == -1)
                    {
                        return 1;
                    }
                }

                if (SDL_PointInRect(&mousePos, &RBoutonDecks))
                {
                    ;
                    Mix_PlayChannel(-1, selectSE, 0);
                    if (Mix_PlayMusic(deckTheme, -1) == -1)
                    {
                        return 1;
                    }
                    done = deck();
                    if (Mix_PlayMusic(mainTheme, -1) == -1)
                    {
                        return 1;
                    }
                }

                if (SDL_PointInRect(&mousePos, &RBoutonOptions))
                {
                    Mix_PlayChannel(-1, selectSE, 0);
                    done = options();
                }
                break;

            case SDL_MOUSEMOTION:
                mousePos.x = event.motion.x;
                mousePos.y = event.motion.y;

                if (SDL_PointInRect(&mousePos, &RBoutonJouer))
                {
                    if (!swollen)
                    {
                        Mix_PlayChannel(-1, hover, 0);
                        swollen = 1;
                        swellRect(&RBoutonJouer);
                    }
                    break;
                }

                if (SDL_PointInRect(&mousePos, &RBoutonDecks))
                {
                    if (!swollen)
                    {
                        Mix_PlayChannel(-1, hover, 0);
                        swollen = 1;
                        swellRect(&RBoutonDecks);
                    }
                    break;
                }

                if (SDL_PointInRect(&mousePos, &RBoutonOptions))
                {
                    if (!swollen)
                    {
                        Mix_PlayChannel(-1, hover, 0);
                        swollen = 1;
                        swellRect(&RBoutonOptions);
                    }
                    break;
                }

                swollen = 0;
                refreshTextureSizesMain(&RLogo, &RBoutonJouer, &RBoutonDecks, &RBoutonOptions);

                break;

            case SDL_WINDOWEVENT:
                if ((&event)->window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    //Rafraichissement des textures en cas de modifications de la taille de la fenêtre
                    SDL_GetWindowSize(win, &winW, &winH);
                    refreshTextureSizesMain(&RLogo, &RBoutonJouer, &RBoutonDecks, &RBoutonOptions);
                }
                break;

            case SDL_QUIT:
                done = 1;
                break;

            default:
                break;
            }
            //Affichage du menu
            clearScreen();
            SDL_RenderCopy(ren, logo, NULL, &RLogo);
            SDL_RenderCopy(ren, boutonJouer, NULL, &RBoutonJouer);
            SDL_RenderCopy(ren, boutonDecks, NULL, &RBoutonDecks);
            SDL_RenderCopy(ren, boutonOptions, NULL, &RBoutonOptions);

            //Rendu
            SDL_RenderPresent(ren);
        }
    }

    //Destruction des structures utilisees
    SDL_DestroyTexture(logo);
    SDL_DestroyTexture(boutonJouer);
    SDL_DestroyTexture(boutonDecks);
    SDL_DestroyTexture(boutonOptions);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_CloseFont(dejavu);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
