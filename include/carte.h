#ifndef _onmappellelovni_
#define _onmappellelovni_
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
struct _playerInfo;

typedef struct _gameStatus {
  struct _playerInfo * joueur;
  struct _playerInfo * ennemi;
  int * idboard;
} * GameStatus;

typedef struct _carte {
    int id;
    int idboard;
    char * nom;
    SDL_Texture * TCarte;
    SDL_Texture * TCarteComplete;
    SDL_Texture * TCarteReduite;
    int genre; //3 Terrain ou 1 perso ou 2 sort
    int type; //1:BadLemon,2:Kayzer,3:Musique,4:Suppo,5:TheReed,6:Rexyz
    int pvMax;
    int pv;
    int attMax;
    int att;
    int coutMax;
    int cout;
    SDL_bool peutAttaquer; //peutAttaquer sert aussi pour les sorts (il dit si le sort requiert de désigner quelqu'un)
    void (*effetDirect) (struct _carte *, GameStatus);
    void (*raleDagonie) (struct _carte *, GameStatus);
} Carte;

typedef struct maillon {
    Carte * carte;
    struct maillon* suiv;
    SDL_Rect * RCarte;
} * LCarte;

typedef struct _deck {
  LCarte deckList;
  char * nom;
} Deck;

typedef struct _playerInfo {
  int pv;
  int mana;
  int manaMax;
  int effetPvTerrain;
  int effetCoutTerrain;
  int effetAttTerrain;
  LCarte jeu;
  LCarte deck;
  LCarte board;
  LCarte provocation;
  Carte * terrain;
} * PlayerInfo;



void afficherCarteDansLCarte(LCarte l);
Deck initDeck(void);
void libereDeck(Deck * d);
int len(LCarte l);
int isIn(Carte * c,LCarte l);
int idIsIn(int id,LCarte l);
Carte * inBoard(int idboard,LCarte l);
LCarte ajoutTete(LCarte l,Carte * c);
LCarte retirerCarte(LCarte l,Carte * c);
LCarte creerListeDeToutesLesCartes();
int pioche(LCarte * jeuJoueur,LCarte * deckJoueur);
Carte * idtocard(int i);
void libereListeCarte(LCarte l);
void libereCarte(Carte * c);
void incrementeJeu(LCarte * l);
void decrementeJeu(LCarte * l);

void sendMyDeck(int sock, GameStatus g);

void checkStartTurn(LCarte * boardJoueur, LCarte jeuJoueur, LCarte * boardEnnemi, LCarte jeuEnnemi, int * idboard, int sock);



Carte * creerJJ();
Carte * creerBadLemon();
Carte * creerErwann();
Carte * creerErwannn();
Carte * creerErwannnn();
Carte * creerKayzer();
Carte * creerPizzaBoy();
Carte * creerBryanGaming();
Carte * creerMantaro();
Carte * creerLucio();
Carte * creerWinston();
Carte * creerSuppo();
Carte * creerTheReed();
Carte * creerJeanLassalle();
Carte * creerRexyz();
Carte * creerPhiid();
Carte * creerAisharo();
Carte * creerNellise();
Carte * creerLipton();
Carte * creerJack();
Carte * creerJeanMi();
Carte * creerCthulhu();
Carte * creerKim();
Carte * creerDarkPlaegueis();
Carte * creerArthur();
Carte * creerBoromir();
Carte * creerSuperBranleur();
Carte * creerAlainSoral();
Carte * creerHitler();
Carte * creerKaiba();
Carte * creerStaline();
Carte * creerLaFougere();
Carte * creerKaaris();
Carte * creerYondu();
Carte * creerShrek();
Carte * creerBlocMinecraft();
Carte * creerSmashMouth();
Carte * creerCamera();
Carte * creerLeCouple();
Carte * creerGuerrierHoc();

Carte * creerLesPetitesBites();
Carte * creerPourLaTeam();
Carte * creerSel();
Carte * creerBlagueDeMerde();
Carte * creerAllStar();
Carte * creerFesses();
Carte * creerJeRe();
Carte * creerSerge();
Carte * creerSpecialDixAbos();
Carte * creerBellecour();
Carte * creerMadaMana();
Carte * creerPUBG();


Carte * creerMemphis();
Carte * creerJapan();
Carte * creerAppartJJ();
Carte * creerAppartTR();

Carte * piocheRandom(int type);
Carte * creerCarte(int id,char * nom,char * path,int genre,int type,int pv,int att,int cout,SDL_bool peutAttaquer);

SDL_bool estInvocable(Carte * c, LCarte board);
SDL_bool isFull(LCarte board);

LCarte refreshAttaque(LCarte l);
void refreshBoard(GameStatus g);
Carte * carteClique(SDL_Point * mousePos,LCarte jeuJoueur,LCarte boardEnnemi,LCarte boardJoueur);

int creerTextureCompleteCarte(Carte *c, int modifPv, int modifAtt, int modifCout);
void refreshTexturesCompletesCartes(LCarte jeuJoueur, int modifPv, int modifAtt, int modifCout);
int refreshTextureCompleteCarte(Carte *c, int modifPv, int modifAtt, int modifCout);
int creerTextureReduiteCarte(Carte *c);
void creerTexturesReduitesCartes(LCarte jeuJoueur);
void creerTexturesCompletesCartes(LCarte jeuJoueur);


#endif
