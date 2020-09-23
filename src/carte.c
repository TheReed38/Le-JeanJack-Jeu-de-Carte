#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "carte.h"
#include "fonctions.h"

#include "SDL2/SDL.h"

extern SDL_Renderer *ren;
extern TTF_Font *dejavu;
extern SDL_Color *coulTextBout;
extern SDL_Color *coulFondBout;
extern SDL_Color *coulFondDeck;
extern SDL_Color *coulFondEcran;
extern Mix_Music *allStar;

int creerTextureCompleteCarte(Carte *c, int modifPv, int modifAtt, int modifCout)
{
  if (!(c->TCarteComplete))
  {
    char cout[16];
    sprintf(cout, "%d", c->cout + modifCout);

    SDL_Rect RCarte = initRect(0, 0, 0, 0);
    SDL_QueryTexture(c->TCarte, NULL, NULL, &(RCarte.w), &(RCarte.h));

    c->TCarteComplete = SDL_CreateTextureSimplified(RCarte.w, RCarte.h);
    SDL_SetRenderTarget(ren, c->TCarteComplete);

    SDL_SetTextureBlendMode(c->TCarteComplete, SDL_BLENDMODE_BLEND); //TODO tester cette fonction

    SDL_Rect RtextCout = initRect((11 * RCarte.w) / 24, (5 * RCarte.h) / 6, RCarte.w / 5, RCarte.h / 7);
    SDL_Texture *TtextCout = creerTextureTexte(cout, coulFondEcran, &(RtextCout.w), &(RtextCout.h));

    if (c->genre == 1)
    {

      char pv[16];
      char att[16];
      sprintf(pv, "%d", c->pv + modifPv);
      sprintf(att, "%d", c->att + modifAtt);
      SDL_Rect RtextPv = initRect((22 * RCarte.w) / 30, (5 * RCarte.h) / 6, RCarte.w / 100, RCarte.h / 7);
      SDL_Texture *TtextPv = creerTextureTexte(pv, coulFondEcran, &(RtextPv.w), &(RtextPv.h));

      SDL_Rect RtextAttaque = initRect(RCarte.w / 10, (5 * RCarte.h) / 6, RCarte.w / 5, RCarte.h / 7);
      SDL_Texture *TtextAttaque = creerTextureTexte(att, coulFondEcran, &(RtextAttaque.w), &(RtextAttaque.h));
      SDL_RenderCopy(ren, c->TCarte, NULL, &RCarte);
      SDL_RenderCopy(ren, TtextPv, NULL, &RtextPv);
      SDL_RenderCopy(ren, TtextAttaque, NULL, &RtextAttaque);
      SDL_RenderCopy(ren, TtextCout, NULL, &RtextCout);
      SDL_DestroyTexture(TtextCout);
      SDL_DestroyTexture(TtextPv);
      SDL_DestroyTexture(TtextAttaque);
    }
    else
    {
      SDL_RenderCopy(ren, c->TCarte, NULL, &RCarte);
      SDL_RenderCopy(ren, TtextCout, NULL, &RtextCout);
      SDL_DestroyTexture(TtextCout);
    }

    SDL_SetRenderTarget(ren, NULL);
  }
  return 0;
}

int refreshTextureCompleteCarte(Carte *c, int modifPv, int modifAtt, int modifCout)
{
  if (c)
  {
    if (c->TCarteComplete)
    {
      SDL_DestroyTexture(c->TCarteComplete);
      c->TCarteComplete = NULL;
    }
    return creerTextureCompleteCarte(c, modifPv, modifAtt, modifCout);
  }
  return 0;
}

//Texture reduite utilisée pendant la création de nouveaux decks
int creerTextureReduiteCarte(Carte *c)
{
  if (c == NULL)
  {
    printf("creerTextureReduiteCarte: ERREUR CARTE NULL\n");
    return 1;
  }
  if (c->TCarteReduite == NULL)
  {
    c->TCarteReduite = createBouton(1500, 200, coulFondBout, coulTextBout, c->nom);
  }
  return 0;
}

void creerTexturesReduitesCartes(LCarte jeuJoueur)
{
  while (jeuJoueur != NULL)
  {
    creerTextureReduiteCarte(jeuJoueur->carte);
    jeuJoueur = jeuJoueur->suiv;
  }
}

void refreshTexturesCompletesCartes(LCarte jeuJoueur, int modifPv, int modifAtt, int modifCout)
{
  while (jeuJoueur != NULL)
  {
    refreshTextureCompleteCarte(jeuJoueur->carte, modifPv, modifAtt, modifCout);
    jeuJoueur = jeuJoueur->suiv;
  }
}

void creerTexturesCompletesCartes(LCarte jeuJoueur)
{
  while (jeuJoueur != NULL)
  {
    creerTextureCompleteCarte(jeuJoueur->carte, 0, 0, 0);
    jeuJoueur = jeuJoueur->suiv;
  }
}

int len(LCarte l)
{
  if (l == NULL)
  {
    return 0;
  }
  return 1 + len(l->suiv);
}

int isIn(Carte *c, LCarte l)
{
  while (l != NULL)
  {
    if (l->carte == c)
    {
      return 1;
    }
    l = l->suiv;
  }
  return 0;
}

int idIsIn(int id, LCarte l)
{
  while (l != NULL)
  {
    if (l->carte->id == id)
    {
      return 1;
    }
    l = l->suiv;
  }
  return 0;
}

Carte *inBoard(int idboard, LCarte l)
{
  while (l != NULL)
  {
    if (l->carte->idboard == idboard)
    {
      return l->carte;
    }
    l = l->suiv;
  }
  return NULL;
}

LCarte ajoutTete(LCarte l, Carte *c)
{
  LCarte newl;
  newl = calloc(1, sizeof(*newl));
  if (newl == NULL)
  {
    printf("\nImpossible de rejouter cet élement dans la liste (echec calloc)\n");
  }
  newl->carte = c;
  newl->suiv = l;
  newl->RCarte = NULL;
  return newl;
}

LCarte retirerCarte(LCarte l, Carte *c)
{
  LCarte tmp;
  if (l == NULL)
  {
    return NULL;
  }
  if (l->carte == c)
  {
    tmp = l->suiv;
    free(l->RCarte);
    free(l);
    return tmp;
  }
  l->suiv = retirerCarte(l->suiv, c);
  return l;
}

void incrementeJeu(LCarte *l)
{
  *l = ajoutTete(*l, idtocard(1));
}
void decrementeJeu(LCarte *l)
{
  *l = (*l)->suiv;
}

LCarte piocheI(LCarte jeuJoueur, LCarte *deckJoueur, int i)
{
  if (i)
  {
    return piocheI(jeuJoueur, &((*deckJoueur)->suiv), i - 1);
  }
  Carte *cartetmp;
  cartetmp = (*deckJoueur)->carte;
  jeuJoueur = ajoutTete(jeuJoueur, cartetmp);
  *deckJoueur = retirerCarte(*deckJoueur, cartetmp);
  return jeuJoueur;
}

int pioche(LCarte *jeuJoueur, LCarte *deckJoueur)
{
  if (*deckJoueur != NULL)
  {
    int r = rand();
    *jeuJoueur = piocheI(*jeuJoueur, deckJoueur, r % (len(*deckJoueur)));
    return 0;
  }
  else
  {
    return 1;
  }
}

Carte *piocheRandom(int type)
{
  int r = rand();
  int cartesBadLemon[17] = {2, 3, 4, 5, 213, 214, 216, 217, 218, 221, 222, 224, 226, 237, 248, 249, 250};
  int cartesKayzer[12] = {6, 204, 205, 206, 209, 211, 215, 218, 233, 243, 250, 252};
  int cartesMusique[3] = {9, 207, 208};
  int cartesSuppo[7] = {12, 207, 210, 217, 219, 229, 239};
  int cartesTheReed[8] = {13, 203, 225, 228, 231, 240, 245, 246};
  int cartesRexyz[7] = {15, 204, 209, 212, 228, 236, 250};

  switch (type)
  {
  case 1: //BadLemon 213,214,216,217,218,221,222,224,226,237,248,249,250,2,3,4,5

    return idtocard(cartesBadLemon[r % 4]);
  case 2: //Kayzer 6,204,205,206,209,211,215,218,233,243,250,252

    return idtocard(cartesKayzer[r % 1]);
  case 3: //Musique 9,207,208

    return idtocard(cartesMusique[r % 1]);
  case 4: //Suppo 12,207,210,217,219,229,239

    return idtocard(cartesSuppo[r % 1]);
  case 5: //TheReed 13,203,225,228,231,240,245,246

    return idtocard(cartesTheReed[r % 2]);
  case 6: //Rexyz 15,204,209,212,228,236,250

    return idtocard(cartesRexyz[r % 1]);
  default:
    break;
  }
  return NULL;
}

LCarte refreshAttaque(LCarte l)
{
  LCarte a = l;
  while (a != NULL)
  {
    a->carte->peutAttaquer = SDL_TRUE;
    a = a->suiv;
  }
  return l;
}

LCarte refreshBoard(LCarte boardJoueur, LCarte *provocation, Carte *terrain, int *effetPVTerrain, int *effetAttTerrain, int *effetCoutTerrain)
{
  //Fonction chargé de refresh les creatures mortes et les effets de terrain
  LCarte board = boardJoueur;
  LCarte tmp;
  if (terrain != NULL)
  {
    if (terrain->effetDirect(terrain, NULL, &boardJoueur, NULL, effetPVTerrain, effetAttTerrain, effetCoutTerrain, NULL) != 1)
    {
      printf("ERREUR LORS DE L'EFFET DU TERRAIN DE %s\n", terrain->nom);
    }
  }
  while (board != NULL)
  {
    if (board->carte->pv + (*effetPVTerrain) <= 0)
    {
      tmp = board->suiv;
      if (board->carte->raleDagonie(board->carte, provocation, &boardJoueur, NULL) != 1)
      {
        printf("ERREUR LORS DU RALE D'AGONIE DE %s\n", board->carte->nom);
      }
      if (isIn(board->carte, *provocation))
      {
        *provocation = retirerCarte(*provocation, board->carte);
      }
      boardJoueur = retirerCarte(boardJoueur, board->carte);
    }
    else
    {
      tmp = board->suiv;
    }
    board = tmp;
  }
  return boardJoueur;
}

Carte *carteClique(SDL_Point *mousePos, LCarte jeuJoueur, LCarte boardEnnemi, LCarte boardJoueur)
{
  while (jeuJoueur != NULL)
  {
    if (jeuJoueur->RCarte != NULL)
    {
      if (SDL_PointInRect(mousePos, jeuJoueur->RCarte))
      {
        return jeuJoueur->carte;
      }
    }
    jeuJoueur = jeuJoueur->suiv;
  }
  while (boardEnnemi != NULL)
  {
    if (boardEnnemi->RCarte != NULL)
    {
      if (SDL_PointInRect(mousePos, boardEnnemi->RCarte))
      {
        return boardEnnemi->carte;
      }
    }
    boardEnnemi = boardEnnemi->suiv;
  }
  while (boardJoueur != NULL)
  {
    if (boardJoueur->RCarte != NULL)
    {
      if (SDL_PointInRect(mousePos, boardJoueur->RCarte))
      {
        return boardJoueur->carte;
      }
    }
    boardJoueur = boardJoueur->suiv;
  }
  return NULL;
}

LCarte creerListeDeToutesLesCartes()
{
  LCarte listeCarte = NULL;
  Carte *tmp;

  //JJLR
  tmp = creerJJ();
  if (tmp == NULL)
  {
    printf("Impossible d'ajouter JJLR\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //BadLemon
  tmp = creerBadLemon();
  if (tmp == NULL)
  {
    printf("Impossible d'ajouter BadLemon\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Erwann
  tmp = creerErwann();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Erwann\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Erwannn
  tmp = creerErwannn();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Erwannn\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Erwannnn
  tmp = creerErwannnn();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Erwannnn\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Kayzer
  tmp = creerKayzer();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Kayzer\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //PizzaBoy21
  tmp = creerPizzaBoy();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter PizzaBoy21\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //BryanGaming
  tmp = creerBryanGaming();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter BryanGaming\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Mantaro
  tmp = creerMantaro();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Mantaro");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Lucio
  tmp = creerLucio();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Lucio");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Winston
  tmp = creerWinston();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Winston");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Suppo
  tmp = creerSuppo();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Suppo\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //TheReed
  tmp = creerTheReed();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter TheReed\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //JeanLassalle
  tmp = creerJeanLassalle();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Jean Lassalle\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Rexyz
  tmp = creerRexyz();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Rexyz\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Phiid
  tmp = creerPhiid();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Phiid\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Lipton
  tmp = creerLipton();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Lipton\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Jack
  tmp = creerJack();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Jack\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //JeanMi
  tmp = creerJeanMi();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter JeanMi\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Cthulhu
  tmp = creerCthulhu();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Cthulhu\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Kim Fromentier
  tmp = creerKim();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Kim\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Shrek
  tmp = creerShrek();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Shrek\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //lespetitesbites
  tmp = creerLesPetitesBites();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter lé peuttites bites\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //memphis
  tmp = creerMemphis();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Memphis\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Japan
  tmp = creerJapan();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Japan\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Appart JJ
  tmp = creerAppartJJ();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Appart JJ\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Appart TR
  tmp = creerAppartTR();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Appart TR\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Sel
  tmp = creerSel();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Sel\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //AllStar
  tmp = creerAllStar();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter AllStar\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  //Fesses
  tmp = creerFesses();
  if (tmp == NULL)
  {
    printf("\nImpossible d'ajouter Fesses\n");
  }
  else
  {
    listeCarte = ajoutTete(listeCarte, tmp);
  }

  return listeCarte;
}

int effetDirectProvocation(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *Lprovoc = provoc;
  *Lprovoc = ajoutTete(*Lprovoc, c);
  return 1;
}

int effetDirectNeutre(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  return 1;
}

int effetDirectBadLemon(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *jeuJoueur = jj;
  *jeuJoueur = ajoutTete(*jeuJoueur, piocheRandom(1));
  return 1;
}

int effetDirectKayzer(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *jeuJoueur = jj;
  *jeuJoueur = ajoutTete(*jeuJoueur, piocheRandom(2));
  return 1;
}

int effetDirectMantaro(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *jeuJoueur = jj;
  *jeuJoueur = ajoutTete(*jeuJoueur, piocheRandom(3));
  return 1;
}

int effetDirectSuppo(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *jeuJoueur = jj;
  *jeuJoueur = ajoutTete(*jeuJoueur, piocheRandom(4));
  return 1;
}

int effetDirectTheReed(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *jeuJoueur = jj;
  *jeuJoueur = ajoutTete(*jeuJoueur, piocheRandom(5));
  return 1;
}

int effetDirectRexyz(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *jeuJoueur = jj;
  *jeuJoueur = ajoutTete(*jeuJoueur, piocheRandom(6));
  return 1;
}

int effetDirectFesses(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  c->pv -= 2;
  return 1;
}

int effetDirectAllStar(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *boardJoueur = bj;
  Carte *carte = idtocard(36);
  if (estInvocable(carte, *boardJoueur))
  {
    *boardJoueur = ajoutTete(*boardJoueur, carte);
  }
  Mix_PlayMusic(allStar, -1);
  return 1;
}

int effetDirectSel(Carte *c, void *provoc, void *bj, void *be, int *a, int *b, int *d, void *jj)
{
  LCarte *boardEnnemi = be;
  LCarte tmpboard = *boardEnnemi;
  while (tmpboard != NULL)
  {
    tmpboard->carte->pv -= 2;
    tmpboard = tmpboard->suiv;
  }
  return 1;
}

int effetTerrainJapan(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi, int *effetPVTerrain, int *effetAttTerrain, int *effetCoutTerrain, void *jj)
{
  *effetCoutTerrain = -1;
  *effetAttTerrain = 0;
  *effetPVTerrain = 0;
  return 1;
}

int effetTerrainMemphis(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi, int *effetPVTerrain, int *effetAttTerrain, int *effetCoutTerrain, void *jj)
{
  *effetCoutTerrain = 0;
  *effetAttTerrain = 0;
  *effetPVTerrain = 1;
  return 1;
}

int effetTerrainAppartTR(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi, int *effetPVTerrain, int *effetAttTerrain, int *effetCoutTerrain, void *jj)
{
  *effetCoutTerrain = 0;
  *effetAttTerrain = 1;
  *effetPVTerrain = 0;
  return 1;
}

int effetTerrainAppartJJ(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi, int *effetPVTerrain, int *effetAttTerrain, int *effetCoutTerrain, void *jj)
{
  *effetCoutTerrain = 1;
  *effetAttTerrain = 0;
  *effetPVTerrain = 0;
  return 1;
}

int effetTerrainLesPetitesBites(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi, int *effetPVTerrain, int *effetAttTerrain, int *effetCoutTerrain, void *jj)
{
  *effetCoutTerrain = 0;
  *effetPVTerrain = 0;
  *effetAttTerrain = -1;
  return 1;
}

int raleDagonieNeutre(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi)
{
  return 1;
}

int raleDagonieWinston(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi)
{
  LCarte *bj = boardJoueur;
  LCarte tmpbj = *bj;
  while (tmpbj != NULL)
  {
    tmpbj->carte->pv += 1;
    tmpbj = tmpbj->suiv;
  }
  return 1;
}

int raleDagonieLucio(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi)
{
  LCarte *bj = boardJoueur;
  LCarte tmpbj = *bj;
  while (tmpbj != NULL)
  {
    tmpbj->carte->att += 1;
    tmpbj = tmpbj->suiv;
  }
  return 1;
}

int raleDagonieJeanLassalle(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi)
{
  LCarte *bj = boardJoueur;
  Carte *newJean = idtocard(14);
  if (estInvocable(newJean, *bj))
  {
    *bj = ajoutTete(*bj, newJean);
    newJean = idtocard(14);
    *bj = ajoutTete(*bj, newJean);
  }
  else
  {
    *bj = ajoutTete(*bj, newJean);
  }
  return 1;
}

int raleDagoniePhiid(Carte *c, void *provoc, void *boardJoueur, void *boardEnnemi)
{
  LCarte *bj = boardJoueur;
  Carte *kim = idtocard(23);
  *bj = ajoutTete(*bj, kim);
  return 1;
}

Carte *creerJJ()
{
  Carte *tmp = creerCarte(1, "JeanJack Le Roux", "image/cartes/JJLR.png", 1, 0, 100, 100, 1, SDL_FALSE);
  tmp->effetDirect = &effetDirectNeutre;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerBadLemon()
{
  Carte *tmp = creerCarte(2, "BadLemon", "image/cartes/BadLemon.png", 1, 1, 1, 1, 1, SDL_FALSE);
  tmp->effetDirect = &effetDirectBadLemon;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerErwann()
{
  Carte *tmp = creerCarte(3, "Erwann", "image/cartes/erwann.png", 1, 1, 2, 2, 2, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerErwannn()
{
  Carte *tmp = creerCarte(4, "Erwannn", "image/cartes/erwannn.png", 1, 1, 4, 4, 4, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerErwannnn()
{
  Carte *tmp = creerCarte(5, "Erwannnn", "image/cartes/erwannnn.png", 1, 1, 8, 8, 8, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerKayzer()
{
  Carte *tmp = creerCarte(6, "Kayzer", "image/cartes/kayzer.png", 1, 2, 7, 2, 4, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectKayzer;
  return tmp;
}

Carte *creerPizzaBoy()
{
  Carte *tmp = creerCarte(7, "PizzaBoy21", "image/cartes/PizzaBoy21.png", 1, 0, 1, 2, 1, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerBryanGaming()
{
  Carte *tmp = creerCarte(8, "BryanGaming", "image/cartes/BryanGaming.png", 1, 0, 2, 1, 1, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerMantaro()
{
  Carte *tmp = creerCarte(9, "Mantaro", "image/cartes/Mantaro.png", 1, 3, 3, 3, 3, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectMantaro;
  return tmp;
}

Carte *creerLucio()
{
  Carte *tmp = creerCarte(10, "Lucio", "image/cartes/Lucio.png", 1, 0, 5, 3, 4, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieLucio;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerWinston()
{
  Carte *tmp = creerCarte(11, "Winston", "image/cartes/Winston.png", 1, 0, 3, 5, 4, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieWinston;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerSuppo()
{
  Carte *tmp = creerCarte(12, "Suppo", "image/cartes/Suppo.png", 1, 4, 2, 7, 4, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectSuppo;
  return tmp;
}

Carte *creerTheReed()
{
  Carte *tmp = creerCarte(13, "TheReed", "image/cartes/TheReed.png", 1, 5, 5, 4, 4, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectTheReed;
  return tmp;
}

Carte *creerJeanLassalle()
{
  Carte *tmp = creerCarte(14, "Jean Lassalle", "image/cartes/jeanLassalle.png", 1, 0, 1, 1, 8, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieJeanLassalle;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerRexyz()
{
  Carte *tmp = creerCarte(15, "Rexyz", "image/cartes/Rexyz.png", 1, 6, 3, 2, 2, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectRexyz;
  return tmp;
}

Carte *creerPhiid()
{
  Carte *tmp = creerCarte(16, "PhiiD", "image/cartes/phiid.png", 1, 0, 2, 2, 6, SDL_FALSE);
  tmp->raleDagonie = &raleDagoniePhiid;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerLipton()
{
  Carte *tmp = creerCarte(19, "Lipton", "image/cartes/Lipton.png", 1, 0, 1, 3, 2, SDL_TRUE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerJack()
{
  Carte *tmp = creerCarte(20, "Jack", "image/cartes/jack.png", 1, 0, 3, 1, 2, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectProvocation;
  return tmp;
}

Carte *creerJeanMi()
{
  Carte *tmp = creerCarte(21, "Jean Mi Du 13", "image/cartes/jeanMi.png", 1, 0, 3, 1, 1, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerCthulhu()
{
  Carte *tmp = creerCarte(22, "Cthulhu", "image/cartes/cthulhu.png", 1, 0, 8, 8, 8, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerKim()
{
  Carte *tmp = creerCarte(23, "Kim Fromentier", "image/cartes/kimFromentier.png", 1, 0, 6, 6, 6, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerShrek()
{
  Carte *tmp = creerCarte(36, "Shrek", "image/cartes/shrek.png", 1, 0, 7, 7, 7, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerJapan()
{
  Carte *tmp = creerCarte(103, "Japan Touch", "image/cartes/japan.png", 3, 0, 0, 0, 3, SDL_TRUE);
  tmp->effetDirect = &effetTerrainJapan;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerMemphis()
{
  Carte *tmp = creerCarte(102, "Memphis Coffee", "image/cartes/memphis.png", 3, 0, 0, 0, 3, SDL_TRUE);
  tmp->effetDirect = &effetTerrainMemphis;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerAppartJJ()
{
  Carte *tmp = creerCarte(104, "L'appart de JeanJack", "image/cartes/appartJJ.png", 3, 0, 0, 0, 3, SDL_TRUE);
  tmp->effetDirect = &effetTerrainAppartJJ;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerAppartTR()
{
  Carte *tmp = creerCarte(105, "L'appart de TheReed", "image/cartes/appartTR.png", 3, 0, 0, 0, 3, SDL_TRUE);
  tmp->effetDirect = &effetTerrainAppartTR;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerLesPetitesBites()
{
  Carte *tmp = creerCarte(101, "Les Peuttites Bites", "image/cartes/lespetitesbites.png", 3, 0, 0, 0, 3, SDL_TRUE);
  tmp->effetDirect = &effetTerrainLesPetitesBites;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerSel()
{
  Carte *tmp = creerCarte(203, "Le Sel", "image/cartes/sel.png", 2, 5, 0, 0, 4, SDL_FALSE);
  tmp->effetDirect = &effetDirectSel;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerAllStar()
{
  Carte *tmp = creerCarte(208, "All Star", "image/cartes/allstar.png", 2, 3, 0, 0, 7, SDL_FALSE);
  tmp->effetDirect = &effetDirectAllStar;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerFesses()
{
  Carte *tmp = creerCarte(223, "J'vais te faire les fesses", "image/cartes/fesses.png", 2, 0, 0, 0, 2, SDL_TRUE);
  tmp->effetDirect = &effetDirectFesses;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerCarte(int id, char *nom, char *path, int genre, int type, int pv, int att, int cout, SDL_bool peutAttaquer)
{
  Carte *c;
  c = calloc(1, sizeof(*c));
  if (c == NULL)
  {
    printf("\nImpossible d'ajouter la carte (erreur calloc)\n");
    return NULL;
  }
  c->id = id;
  c->nom = strdup(nom);
  SDL_Texture *Tc = loadPictures(path);
  if (Tc == NULL)
  {
    SDL_Log("Unable to create texture from image :%s", SDL_GetError());
    return NULL;
  }
  c->TCarte = Tc;
  c->TCarteComplete = NULL;
  c->TCarteReduite = NULL;
  c->genre = genre;
  c->type = type;
  c->pvMax = c->pv = pv;
  c->attMax = c->att = att;
  c->coutMax = c->cout = cout;
  c->peutAttaquer = peutAttaquer;
  return c;
}

SDL_bool estInvocable(Carte *c, LCarte board)
{
  if (isFull(board))
  {
    return SDL_FALSE;
  }
  if (c->id == 1)
  {
    if (idIsIn(2, board))
    {
      if (idIsIn(6, board))
      {
        if (idIsIn(12, board))
        {
          if (idIsIn(13, board))
          {
            if (idIsIn(15, board))
            {
              return SDL_TRUE;
            }
          }
        }
      }
    }
    return SDL_FALSE;
  }
  return SDL_TRUE;
}

SDL_bool isFull(LCarte board)
{
  if (len(board) >= 8)
  {
    return SDL_TRUE;
  }
  return SDL_FALSE;
}

Carte *idtocard(int i)
{
  Carte *tmp;
  switch (i)
  {
  case 1:
    tmp = creerJJ();
    return tmp;
  case 2:
    tmp = creerBadLemon();
    return tmp;
  case 3:
    tmp = creerErwann();
    return tmp;
  case 4:
    tmp = creerErwannn();
    return tmp;
  case 5:
    tmp = creerErwannnn();
    return tmp;
  case 6:
    tmp = creerKayzer();
    return tmp;
  case 7:
    tmp = creerPizzaBoy();
    return tmp;
  case 8:
    tmp = creerBryanGaming();
    return tmp;
  case 9:
    tmp = creerMantaro();
    return tmp;
  case 10:
    tmp = creerLucio();
    return tmp;
  case 11:
    tmp = creerWinston();
    return tmp;
  case 12:
    tmp = creerSuppo();
    return tmp;
  case 13:
    tmp = creerTheReed();
    return tmp;
  case 14:
    tmp = creerJeanLassalle();
    return tmp;
  case 15:
    tmp = creerRexyz();
    return tmp;
  case 16:
    tmp = creerPhiid();
    return tmp;
  case 19:
    tmp = creerLipton();
    return tmp;
  case 20:
    tmp = creerJack();
    return tmp;
  case 21:
    tmp = creerJeanMi();
    return tmp;
  case 22:
    tmp = creerCthulhu();
    return tmp;
  case 23:
    tmp = creerKim();
    return tmp;
  case 36:
    tmp = creerShrek();
    return tmp;
  case 101:
    tmp = creerLesPetitesBites();
    return tmp;
  case 102:
    tmp = creerMemphis();
    return tmp;
  case 103:
    tmp = creerJapan();
    return tmp;
  case 104:
    tmp = creerAppartJJ();
    return tmp;
  case 105:
    tmp = creerAppartTR();
    return tmp;
  case 203:
    tmp = creerSel();
    return tmp;
  case 208:
    tmp = creerAllStar();
    return tmp;
  case 223:
    tmp = creerFesses();
    return tmp;
  default:
    printf("ID INCONNU \n");
    return NULL;
  }
}

void libereListeCarte(LCarte l)
{
  if (l)
  {
    LCarte nl = l->suiv;
    if (l->RCarte)
    {
      free(l->RCarte);
    }
    libereCarte(l->carte);
    free(l);
    libereListeCarte(nl);
  }
}

void libereCarte(Carte *c)
{
  if (c != NULL)
  {
    if (c->TCarte != NULL)
    {
      SDL_DestroyTexture(c->TCarte);
    }
    if (c->TCarteComplete != NULL)
    {
      SDL_DestroyTexture(c->TCarteComplete);
    }
    if (c->TCarteReduite != NULL)
    {
      SDL_DestroyTexture(c->TCarteReduite);
    }
    free(c);
  }
}

Deck initDeck()
{
  Deck d;
  d.nom = NULL;
  d.deckList = NULL;
  return d;
}

void libereDeck(Deck *d)
{
  if (d->nom)
    free(d->nom);
  if (d->deckList)
    libereListeCarte(d->deckList);
  d->nom = NULL;
  d->deckList = NULL;
}

void afficherCarteDansLCarte(LCarte l)
{
  while (l)
  {
    printf("%s\n", l->carte->nom);
    l = l->suiv;
  }
}
