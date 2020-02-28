#ifndef _jujulLeSang_
#define _jujulLeSang_

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "carte.h"

void refreshMain(SDL_Window * fen, int * winW,int * winH,SDL_Rect * Rlogo,SDL_Rect * RBout1,SDL_Rect * RBout2,SDL_Rect * RBout3,SDL_Rect * RtextBout1,SDL_Rect * RtextBout2,SDL_Rect * RtextBout3);

int deck(SDL_Window * fen, SDL_Renderer * ren,TTF_Font * dejavu,Mix_Chunk * hover, Mix_Chunk * select);
Lcarte loadDeck(char * nomFichier,SDL_Renderer * ren,TTF_Font * dejavu);
int sauvDeck(char * nomFichier,Lcarte deck);
void afficheToutesLesCartes(SDL_Renderer * ren,Lcarte toutesLesCartes,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh,int page);
void libereTextureDecks(SDL_Texture * zoneDecks,SDL_Texture * bout1,SDL_Texture * TtextBout1,SDL_Texture * boutonRetour);

int lobby(SDL_Window * fen,SDL_Renderer * ren,TTF_Font * dejavu,Mix_Chunk * hover, Mix_Chunk * selectSE);
void libereTextureLobby(SDL_Texture * bout1,SDL_Texture * TtextBout1,SDL_Texture * boutonRetour);

void afficheJeuJoueur(SDL_Renderer * ren,Lcarte jeuJoueur,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh,int effetCoutTerrain);
void afficheJeuEnnemi(SDL_Renderer * ren,SDL_Texture* Tdos,Lcarte jeuEnnemi,int winW,int winH);
void afficheBoardJoueur(SDL_Renderer * ren, Lcarte boardJoueur,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh,int effetPVTerrainJoueur,int effetAttTerrainJoueur);
void afficheBoardEnnemi(SDL_Renderer * ren, Lcarte boardEnnemi,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh,int effetPVTerrainEnnemi,int effetAttTerrainEnnemi);
void afficheManaJoueur(SDL_Renderer * ren,SDL_Texture ** TmanaJoueur, int manaJoueur,int manaMaxJoueur,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh);
void afficheManaEnnemi(SDL_Renderer * ren,SDL_Texture ** TmanaEnnemi, int manaEnnemi,int manaMaxEnnemi,int winW,int winH,TTF_Font * dejavu,SDL_bool refresh);
int jeu(int sock,Lcarte deckChoisi,SDL_Window * fen,SDL_Renderer * ren,TTF_Font * dejavu);

int options(SDL_Window * fen, SDL_Renderer * ren,TTF_Font * dejavu,Uint32 * fullscreen,Mix_Chunk * hover,Mix_Chunk * select);
void libereTextureOptions(SDL_Texture * zoneDecks,SDL_Texture * bout1,SDL_Texture * TtextBout1,SDL_Texture * boutonRetour);

char * textInput(char * instructions,TTF_Font * dejavu);
SDL_Texture * actualiserTexteEcrit(SDL_Renderer*ren,SDL_Texture * Ttexte,char * text,SDL_Rect * Rtext,TTF_Font * dejavu);

SDL_Texture * loadPictures(SDL_Renderer* ren,char * nomFichier);


#endif
