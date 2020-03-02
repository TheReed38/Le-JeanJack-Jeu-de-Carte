#ifndef _onmappellelovni_
#define _onmappellelovni_
#include "SDL.h"
#include "SDL_ttf.h"

typedef struct _carte {
    int id;
    int idboard;
    char * nom;
    SDL_Texture * Tcarte;
    SDL_Texture * TcarteComplet;
    SDL_Texture * TcarteReduite;
    int genre; //3 Terrain ou 1 perso ou 2 sort
    int type; //1:BadLemon,2:Kayzer,3:Musique,4:Suppo,5:TheReed,6:Rexyz
    int pvMax;
    int pv;
    int attMax;
    int att;
    int coutMax;
    int cout;
    SDL_bool peutAttaquer; //peutAttaquer sert aussi pour les sorts (il dit si le sort requiert de désigner quelqu'un)
    int (*effetDirect) (struct _carte *,SDL_Renderer*,TTF_Font*,void *,void *,void*,int*,int*,int*,void*);
    int (*raleDagonie) (struct _carte *,SDL_Renderer*,TTF_Font*,void *,void *,void *);
} Carte;

typedef struct maillon {
    Carte * carte;
    struct maillon* suiv;
    SDL_Rect * Rcarte;
} * Lcarte;


int len(Lcarte l);
int isIn(Carte * c,Lcarte l);
int idIsIn(int id,Lcarte l);
Carte * inBoard(int idboard,Lcarte l);
Lcarte ajoutTete(Lcarte l,Carte * c);
Lcarte retirerCarte(Lcarte l,Carte * c);
Lcarte creerListeCarte(SDL_Renderer * ren,TTF_Font * dejavu);
Lcarte pioche(Lcarte jeuJoueur,Lcarte * deckJoueur);
Carte * idtocard(int i,SDL_Renderer * ren, TTF_Font * dejavu);
void libereListeCarte(Lcarte l);
void libereCarte(Carte * c);

SDL_Rect * copieRect(SDL_Rect * r);

Carte * creerJJ(SDL_Renderer * ren, TTF_Font * dejavu);
Carte * creerBadLemon(SDL_Renderer * ren, TTF_Font * dejavu);
Carte * creerErwann(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerErwannn(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerErwannnn(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerKayzer(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerPizzaBoy(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerBryanGaming(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerMantaro(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerLucio(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerWinston(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerSuppo(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerTheReed(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerJeanLassalle(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerRexyz(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerLipton(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerJack(SDL_Renderer * ren,TTF_Font * dejavu);

Carte * creerLesPetitesBites(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerSel(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerFesses(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerMemphis(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerJapan(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerAppartJJ(SDL_Renderer * ren,TTF_Font * dejavu);
Carte * creerAppartTR(SDL_Renderer * ren,TTF_Font * dejavu);

Carte * piocheRandom(SDL_Renderer * ren, TTF_Font * dejavu, int type);
Carte * creerCarte(SDL_Renderer * ren,TTF_Font * dejavu,int id,char * nom,char * path,int genre,int type,int pv,int att,int cout,SDL_bool peutAttaquer);

SDL_bool estInvocable(Carte * c, Lcarte board);
SDL_bool isFull(Lcarte board);

Lcarte refreshAttaque(Lcarte l);
void refreshCarte(SDL_Renderer * ren,Carte * c,TTF_Font * dejavu,SDL_Rect * Raff, int modifPv,int modifAtt,int modifCout);
Lcarte refreshBoardJoueur(SDL_Renderer * ren,TTF_Font * dejavu,Lcarte boardJoueur,Lcarte * provocationJoueur,Carte * terrainJoueur,int *effetPVTerrainJoueur,int *effetAttTerrainJoueur,int *effetCoutTerrain);
Lcarte refreshBoardEnnemi(SDL_Renderer* ren,TTF_Font * dejavu,Lcarte boardEnnemi,Lcarte * provocationEnnemi,Carte * terrainEnnemi,int *effetPVTerrainEnnemi,int *effetAttTerrainEnnemi,int *effetCoutTerrain);
Carte * carteClique(SDL_Point * mousePos,Lcarte jeuJoueur,Lcarte boardEnnemi,Lcarte boardJoueur);
int creerTextureCarte(SDL_Renderer * ren, Carte * c,TTF_Font * dejavu,int modifPv , int modifAtt, int modifCout);
int creerTextureSort(SDL_Renderer * ren, Carte * c,TTF_Font * dejavu, int modifCout);
int creerTextureReduiteCarte(SDL_Renderer * ren,Carte * c,TTF_Font * dejavu );


#endif
