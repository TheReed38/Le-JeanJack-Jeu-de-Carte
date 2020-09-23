#ifndef _jujulLeSang_
#define _jujulLeSang_

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "carte.h"

Deck loadDeck(char *nomFichier);
int sauvDeck(char *nomFichier, Deck deck);

int lobby(void);
int deck(void);
int options(void);
int jeu(int sock, LCarte deckChoisi);


SDL_Texture *SDL_CreateTextureSimplified(int width, int height);

void swellRect(SDL_Rect * r);
SDL_Rect initRect(int x, int y, int w, int h);
SDL_Rect * copieRect(SDL_Rect * r);
SDL_Color *initColor(int r, int g, int b, int a);

SDL_Texture *creerTextureTexte(char *text, SDL_Color *color, int *w, int *h);
SDL_Texture *createBouton(int width, int height, SDL_Color *coulFond, SDL_Color *coulText, char *text);
void clearScreen(void);

int playerPrompt(char *instructions, char **ipAddress);

SDL_Texture *loadPictures(char *nomFichier);

#endif
