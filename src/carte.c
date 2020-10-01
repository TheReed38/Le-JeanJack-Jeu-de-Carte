#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "carte.h"
#include "fonctions.h"

#include "SDL2/SDL.h"
#include <WinSock2.h>
#define UNUSED(expr) \
  do                 \
  {                  \
    (void)(expr);    \
  } while (0)
#define JUST_ATTACKED 15
#define READY_TO_ATTACK 16

#define DP_FIRST_TURN 23
#define DP_SECOND_TURN 24
#define DP_THIRD_TURN 25

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

    SDL_Rect RAttackStatus = initRect(RCarte.w / 10, RCarte.h / 10, RCarte.w / 6, RCarte.h / 6);
    SDL_Texture *TAttackStatus = createBouton(RCarte.w, RCarte.h, coulFondDeck, coulFondEcran, "A");

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

      if ((!(c->peutAttaquer)) && (c->idboard != 0))
      {
        SDL_RenderCopy(ren, TAttackStatus, NULL, &RAttackStatus);
      }
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
    SDL_DestroyTexture(TAttackStatus);
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

void sendMyDeck(int sock, GameStatus g)
{
  LCarte deck = g->joueur->deck;
  char buffer[1024];
  while (deck)
  {
    sprintf(buffer, "/11 %d", deck->carte->id);
    printf("Je signale que %d fait partie de mon deck \n", deck->carte->id);
    send(sock, buffer, strlen(buffer), 0);
    deck = deck->suiv;
  }
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
  if (*l)
  {
    *l = (*l)->suiv;
  }
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
  int cartesBadLemon[13] = {249, 214, 216, 217, 218, 221, 222, 224, 226, 237, 248, 213, 250};
  int cartesKayzer[11] = {243, 204, 206, 209, 211, 215, 218, 233, 205, 250, 252};
  int cartesMusique[2] = {208, 207};
  int cartesSuppo[6] = {219, 210, 217, 207, 229, 239};
  int cartesTheReed[7] = {203, 245, 246, 225, 228, 231, 240};
  int cartesRexyz[6] = {204, 209, 212, 228, 236, 250};

  switch (type)
  {
  case 1:
    return idtocard(cartesBadLemon[r % 1]);
  case 2:
    return idtocard(cartesKayzer[r % 2]);
  case 3:
    return idtocard(cartesMusique[r % 1]);
  case 4:
    return idtocard(cartesSuppo[r % 1]);
  case 5:
    return idtocard(cartesTheReed[r % 3]);
  case 6:
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
    if (a->carte->id == 39)
    {
      if (a->carte->type == JUST_ATTACKED)
      {
        a->carte->type = READY_TO_ATTACK;
      }
      else if (a->carte->type == READY_TO_ATTACK)
      {
        a->carte->peutAttaquer = SDL_TRUE;
        a->carte->type = JUST_ATTACKED;
      }
    }
    else
    {
      a->carte->peutAttaquer = SDL_TRUE;
    }
    a = a->suiv;
  }
  return l;
}

void checkStartTurn(LCarte *boardJoueur, LCarte jeuJoueur, LCarte *boardEnnemi, LCarte jeuEnnemi, int *idboard, int sock)
{

  int nbCards = len(*boardJoueur) + len(jeuJoueur) + len(*boardEnnemi) + len(jeuEnnemi);
  char buffer[1024];
  LCarte board = *boardJoueur;
  Carte *c = NULL;
  //Apparition des guerriers HOC War
  if (nbCards == 0)
  {
    c = idtocard(40);
    c->idboard = *idboard;
    *idboard = (*idboard) + 1;
    *boardJoueur = ajoutTete(*boardJoueur, c);
    sprintf(buffer, "/3 40 0");
    send(sock, buffer, strlen(buffer), 0);
    c = idtocard(40);
    c->idboard = *idboard;
    *idboard = (*idboard) + 1;
    *boardEnnemi = ajoutTete(*boardEnnemi, c);
    sprintf(buffer, "/3 40 1");
    send(sock, buffer, strlen(buffer), 0);
  }

  while (board != NULL)
  {

    int r;
    //Dark Plaegueis qui meurt
    if (board->carte->id == 25)
    {
      switch (board->carte->type)
      {
      case (DP_FIRST_TURN):
        board->carte->type = DP_SECOND_TURN;
        break;
      case (DP_SECOND_TURN):
        board->carte->type = DP_THIRD_TURN;
        break;
      case (DP_THIRD_TURN):
        board->carte->pv = 0;
        sprintf(buffer, "/10 %d %d %d", board->carte->idboard, 0, board->carte->att);
        send(sock, buffer, strlen(buffer), 0);
        break;
      }
    }

    //Stats du guerrier HOC War qui se shuffle
    if (board->carte->id == 40)
    {
      r = rand();
      board->carte->pv = r % 30 + 1;
      r = rand();
      board->carte->att = r % 30 + 1;
      sprintf(buffer, "/10 %d %d %d", board->carte->idboard, board->carte->pv, board->carte->att);
      send(sock, buffer, strlen(buffer), 0);
    }
      board = board->suiv;
    }
  
  board= *boardEnnemi;
  while (board) {
    if (board->carte->id == 34) {
      c = board->carte;
      board = board->suiv;
      *boardEnnemi = retirerCarte(*boardEnnemi,c);
      sprintf(buffer, "/10 %d %d %d", c->idboard, -10, c->att);
      send(sock, buffer, strlen(buffer), 0);
      libereCarte(c);  
      if (len(*boardJoueur) == 8) {
          sprintf(buffer, "/12 %d", (*boardJoueur)->suiv->suiv->suiv->carte->idboard);
         send(sock, buffer, strlen(buffer), 0);
         *boardJoueur = retirerCarte(*boardJoueur,(*boardJoueur)->suiv->suiv->suiv->carte);
      }
      c= creerKaaris();
      c->idboard=*idboard;
      (*idboard)++;
      *boardJoueur = ajoutTete(*boardJoueur,c);
      sprintf(buffer, "/3 34 2");
      send(sock, buffer, strlen(buffer), 0);
    }
    else {
      board = board->suiv;
    }
  }
}

void refreshBoard(GameStatus g)
{
  //Fonction chargé de refresh les creatures mortes et les effets de terrain
  LCarte board = g->joueur->board;
  LCarte tmp;
  if (g->joueur->terrain != NULL)
  {
    g->joueur->terrain->effetDirect(g->joueur->terrain, g);
  }

  while (board != NULL)
  {

    if (board->carte->pv + (g->joueur->effetPvTerrain) <= 0)
    {
      tmp = board->suiv;
      board->carte->raleDagonie(board->carte, g);
      if (isIn(board->carte, g->joueur->provocation))
      {
        g->joueur->provocation = retirerCarte(g->joueur->provocation, board->carte);
      }
      g->joueur->board = retirerCarte(g->joueur->board, board->carte);
    }
    else
    {
      tmp = board->suiv;
    }
    board = tmp;
  }
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

  for (int i = 0; i < 300; i++)
  {
    tmp = idtocard(i);
    if (tmp)
    {
      if (!((tmp->id == 37) || (tmp->id == 38) || (tmp->id == 39) || (tmp->id == 40)))
      {

        listeCarte = ajoutTete(listeCarte, tmp);
      }
    }
  }

  return listeCarte;
}

void effetDirectProvocation(Carte *c, GameStatus g)
{
  g->joueur->provocation = ajoutTete(g->joueur->provocation, c);
}

void effetDirectNeutre(Carte *c, GameStatus g)
{
  UNUSED(g);
  UNUSED(c);
}

void effetDirectBadLemon(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->jeu = ajoutTete(g->joueur->jeu, piocheRandom(1));
}

void effetDirectKayzer(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->jeu = ajoutTete(g->joueur->jeu, piocheRandom(2));
}

void effetDirectMantaro(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->jeu = ajoutTete(g->joueur->jeu, piocheRandom(3));
}

void effetDirectSuppo(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->jeu = ajoutTete(g->joueur->jeu, piocheRandom(4));
}

void effetDirectTheReed(Carte *c, GameStatus g)
{
  g->joueur->jeu = ajoutTete(g->joueur->jeu, piocheRandom(5));

  LCarte board = g->joueur->board;
  Carte *cartetmp = NULL;
  while (board)
  {
    if (board->carte->id == 18)
    {
      g->joueur->board = retirerCarte(g->joueur->board, board->carte);
      g->joueur->board = retirerCarte(g->joueur->board, c);
      cartetmp = idtocard(39);
      cartetmp->idboard = *(g->idboard);
      (*(g->idboard))++;
      g->joueur->board = ajoutTete(g->joueur->board, cartetmp);
      break;
    }
    board = board->suiv;
  }
}

void effetDirectRexyz(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->jeu = ajoutTete(g->joueur->jeu, piocheRandom(6));
}

void effetDirectNellise(Carte *c, GameStatus g)
{
  LCarte board = g->joueur->board;
  Carte *cartetmp = NULL;
  while (board)
  {
    if (board->carte->id == 13)
    {
      g->joueur->board = retirerCarte(g->joueur->board, board->carte);
      g->joueur->board = retirerCarte(g->joueur->board, c);
      cartetmp = idtocard(39);
      cartetmp->idboard = *(g->idboard);
      (*(g->idboard))++;
      g->joueur->board = ajoutTete(g->joueur->board, cartetmp);
      break;
    }
    board = board->suiv;
  }
}

void effetDirectArthur(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->jeu = ajoutTete(g->joueur->jeu, creerFesses());
}

void effetDirectSuperBranleur(Carte *c, GameStatus g)
{
  UNUSED(c);
  int n = len(g->joueur->board);
  int i = 65 % n;
  LCarte l = g->joueur->board;
  while (i && l)
  {
    i--;
    l = l->suiv;
  }
  if (l)
  {
    if (l->carte->type == 1)
    {
      l->carte->pv += 2;
    }
    else
    {
      l->carte->pv++;
    }
  }
}

void effetDirectAlainSoral(Carte *c, GameStatus g)
{
  UNUSED(c);
  LCarte board = g->joueur->board;
  while (board)
  {
    board->carte->att = 5;
    board = board->suiv;
  }
  board = g->ennemi->board;
  while (board)
  {
    board->carte->att = 5;
    board = board->suiv;
  }
}

void effetDirectStaline(Carte *c, GameStatus g)
{
  UNUSED(c);
  LCarte board = g->joueur->board;
  while (board)
  {
    board->carte->pv = 5;
    board = board->suiv;
  }
  board = g->ennemi->board;
  while (board)
  {
    board->carte->pv = 5;
    board = board->suiv;
  }
}

void effetDirectSmashMouth(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->jeu = ajoutTete(g->joueur->jeu, creerAllStar());
}

void effetDirectFesses(Carte *c, GameStatus g)
{
  UNUSED(g);
  c->pv -= 2;
}

void effetDirectPUBG(Carte *c, GameStatus g)
{
  UNUSED(c);
  LCarte boardJoueur = g->joueur->board;
  LCarte boardEnnemi = g->ennemi->board;
  while (boardJoueur)
  {
    if (boardJoueur->carte->type != 4)
    {
      boardJoueur->carte->att = 1;
    }
    boardJoueur = boardJoueur->suiv;
  }
  while (boardEnnemi)
  {
    if (boardEnnemi->carte->type != 4)
    {
      boardEnnemi->carte->att = 1;
    }
    boardEnnemi = boardEnnemi->suiv;
  }
}

void effetDirectJeRe(Carte *c, GameStatus g)
{
  UNUSED(c);
  if (g->ennemi->deck)
  {
    LCarte nl = g->ennemi->deck->suiv;
    libereCarte(g->ennemi->deck->carte);
    free(g->ennemi->deck);
    g->ennemi->deck = nl;
  }
}

void effetDirectBellecour(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->pv -= 6;
  for (int i = 0; i < 5; i++)
  {
    if (len(g->joueur->jeu) >= 8)
      break;
    pioche(&(g->joueur->jeu), &(g->joueur->deck));
  }
}

void effetDirectMadaMana(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->mana += 1;
}

void effetDirectAllStar(Carte *c, GameStatus g)
{
  UNUSED(c);
  Carte *carte = creerShrek();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
  }
  Mix_PlayMusic(allStar, -1);
}

void effetDirectSerge(Carte *c, GameStatus g)
{
  UNUSED(c);
  Carte *carte = creerCamera();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
    g->joueur->provocation = ajoutTete(g->joueur->provocation, carte);
  }
}

void effetDirectSpecialDixAbos(Carte *c, GameStatus g)
{
  UNUSED(c);
  Carte *carte = creerPizzaBoy();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
  }
  carte = creerBryanGaming();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
  }
}

void effetDirectPourLaTeam(Carte *c, GameStatus g)
{
  UNUSED(c);
  Carte *carte;
  carte = creerBadLemon();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
  }
  carte = creerKayzer();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
  }
  carte = creerSuppo();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
  }
  carte = creerRexyz();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
  }
  carte = creerTheReed();
  carte->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(carte, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, carte);
  }
}

void effetDirectSel(Carte *c, GameStatus g)
{
  UNUSED(c);
  LCarte tmpboard = g->ennemi->board;
  while (tmpboard != NULL)
  {
    tmpboard->carte->pv -= 2;
    tmpboard = tmpboard->suiv;
  }
}

void effetDirectBlagueDeMerde(Carte *c, GameStatus g)
{
  UNUSED(c);
  if (g->ennemi->terrain)
  {
    libereCarte(g->ennemi->terrain);
  }
  g->ennemi->terrain = creerLesPetitesBites();
}

void effetTerrainJapan(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->effetCoutTerrain = -1;
  g->joueur->effetAttTerrain = 0;
  g->joueur->effetPvTerrain = 0;
}

void effetTerrainMemphis(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->effetCoutTerrain = 0;
  g->joueur->effetAttTerrain = 0;
  g->joueur->effetPvTerrain = 1;
}

void effetTerrainAppartTR(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->effetCoutTerrain = 0;
  g->joueur->effetAttTerrain = 1;
  g->joueur->effetPvTerrain = 0;
}

void effetTerrainAppartJJ(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->effetCoutTerrain = 1;
  g->joueur->effetAttTerrain = 0;
  g->joueur->effetPvTerrain = 0;
}

void effetTerrainLesPetitesBites(Carte *c, GameStatus g)
{
  UNUSED(c);
  g->joueur->effetCoutTerrain = 0;
  g->joueur->effetAttTerrain = -1;
  g->joueur->effetPvTerrain = 0;
}

void raleDagonieNeutre(Carte *c, GameStatus g)
{
  UNUSED(c);
  UNUSED(g);
}

void raleDagonieWinston(Carte *c, GameStatus g)
{
  UNUSED(c);
  LCarte tmpbj = g->joueur->board;
  while (tmpbj != NULL)
  {
    tmpbj->carte->pv += 1;
    tmpbj = tmpbj->suiv;
  }
}

void raleDagonieLucio(Carte *c, GameStatus g)
{
  UNUSED(c);
  LCarte tmpbj = g->joueur->board;
  while (tmpbj != NULL)
  {
    tmpbj->carte->att += 1;
    tmpbj = tmpbj->suiv;
  }
}

void raleDagonieJeanLassalle(Carte *c, GameStatus g)
{
  UNUSED(c);
  Carte *newJean = creerJeanLassalle();
  newJean->idboard = *(g->idboard);
  (*(g->idboard))++;
  if (estInvocable(newJean, g->joueur->board))
  {
    g->joueur->board = ajoutTete(g->joueur->board, newJean);
    newJean = creerJeanLassalle();
    newJean->idboard = *(g->idboard);
    (*(g->idboard))++;
    g->joueur->board = ajoutTete(g->joueur->board, newJean);
  }
  else
  {
    g->joueur->board = ajoutTete(g->joueur->board, newJean);
  }
}

void raleDagoniePhiid(Carte *c, GameStatus g)
{
  UNUSED(c);
  Carte *kim = creerKim();
  kim->idboard = *(g->idboard);
  (*(g->idboard))++;
  g->joueur->board = ajoutTete(g->joueur->board, kim);
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

Carte *creerAisharo()
{
  Carte *tmp = creerCarte(17, "Aisharo", "image/cartes/aisharo.png", 1, 0, 2, 1, 5, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerNellise()
{
  Carte *tmp = creerCarte(18, "Nellise", "image/cartes/nellise.png", 1, 0, 4, 5, 4, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNellise;
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

Carte *creerSmashMouth()
{
  Carte *tmp = creerCarte(24, "Smash Mouth", "image/cartes/smashMouth.png", 1, 3, 4, 4, 5, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectSmashMouth;
  return tmp;
}

Carte *creerDarkPlaegueis()
{
  Carte *tmp = creerCarte(25, "Dark Plaegueis", "image/cartes/darkPlaeguis.png", 1, DP_FIRST_TURN, 99, 9, 7, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerArthur()
{
  Carte *tmp = creerCarte(26, "Arthur", "image/cartes/arthur.png", 1, 0, 4, 3, 3, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectArthur;
  return tmp;
}

Carte *creerBoromir()
{
  Carte *tmp = creerCarte(27, "Boromir", "image/cartes/boromir.png", 1, 0, 1, 1, 3, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerSuperBranleur()
{
  Carte *tmp = creerCarte(28, "Super Branleur", "image/cartes/superBranleur.png", 1, 0, 5, 4, 5, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectSuperBranleur;
  return tmp;
}

Carte *creerAlainSoral()
{
  Carte *tmp = creerCarte(29, "Alain Soral", "image/cartes/alainSoral.png", 1, 0, 2, 5, 5, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectAlainSoral;
  return tmp;
}

Carte *creerHitler()
{
  Carte *tmp = creerCarte(30, "Adolf Hitler", "image/cartes/adolfHitler.png", 1, 0, 45, 39, 5, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerKaiba()
{
  Carte *tmp = creerCarte(31, "Kaiba", "image/cartes/kaiba.png", 1, 0, 5, 6, 6, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerStaline()
{
  Carte *tmp = creerCarte(32, "Staline", "image/cartes/staline.png", 1, 0, 5, 2, 5, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectStaline;
  return tmp;
}

Carte *creerLaFougere()
{
  Carte *tmp = creerCarte(33, "La Fougère", "image/cartes/laFougere.png", 1, 0, 1, 2, 3, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerKaaris()
{
  Carte *tmp = creerCarte(34, "Kaaris", "image/cartes/kaaris.png", 1, 0, 3, 3, 3, SDL_TRUE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerYondu()
{
  Carte *tmp = creerCarte(35, "Yondu", "image/cartes/yondu.png", 1, 0, 0, 0, 0, SDL_FALSE);
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

Carte *creerBlocMinecraft()
{
  Carte *tmp = creerCarte(37, "Bloc Minecraft", "image/cartes/blocMinecraft.png", 1, 0, 1, 0, 0, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectProvocation;
  return tmp;
}

Carte *creerCamera()
{
  Carte *tmp = creerCarte(38, "Camera", "image/cartes/camera.png", 1, 0, 10, 0, 0, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectProvocation;
  return tmp;
}

Carte *creerLeCouple()
{
  Carte *tmp = creerCarte(39, "Le Couple", "image/cartes/leCouple.png", 1, READY_TO_ATTACK, 9, 9, 8, SDL_FALSE);
  tmp->raleDagonie = &raleDagonieNeutre;
  tmp->effetDirect = &effetDirectNeutre;
  return tmp;
}

Carte *creerGuerrierHoc()
{
  Carte *tmp = creerCarte(40, "Guerrier HOC War", "image/cartes/guerrierHocWar.png", 1, 0, 1, 1, 1, SDL_FALSE);
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

Carte *creerPourLaTeam()
{
  Carte *tmp = creerCarte(201, "Pour La Team JeanJack", "image/cartes/pourLaTeamJeanJack.png", 2, 0, 0, 0, 10, SDL_FALSE);
  tmp->effetDirect = &effetDirectPourLaTeam;
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

Carte *creerBlagueDeMerde()
{
  Carte *tmp = creerCarte(204, "BlagueDeMerde", "image/cartes/blagueDeMerde.png", 2, 2, 0, 0, 3, SDL_FALSE);
  tmp->effetDirect = &effetDirectBlagueDeMerde;
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

Carte *creerPUBG()
{
  Carte *tmp = creerCarte(219, "PUBG", "image/cartes/pubg.png", 2, 4, 0, 0, 3, SDL_FALSE);
  tmp->effetDirect = &effetDirectPUBG;
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

Carte *creerMadaMana()
{
  Carte *tmp = creerCarte(241, "Mada Mana", "image/cartes/madaMana.png", 2, 0, 0, 0, 0, SDL_FALSE);
  tmp->effetDirect = &effetDirectMadaMana;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerJeRe()
{
  Carte *tmp = creerCarte(243, "Je Re", "image/cartes/jeRe.png", 2, 2, 0, 0, 4, SDL_FALSE);
  tmp->effetDirect = &effetDirectJeRe;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerSerge()
{
  Carte *tmp = creerCarte(245, "Serge", "image/cartes/serge.png", 2, 5, 0, 0, 5, SDL_FALSE);
  tmp->effetDirect = &effetDirectSerge;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerSpecialDixAbos()
{
  Carte *tmp = creerCarte(246, "Spécial Dix Abonnés", "image/cartes/specialDixAbos.png", 2, 5, 0, 0, 2, SDL_FALSE);
  tmp->effetDirect = &effetDirectSpecialDixAbos;
  tmp->raleDagonie = &raleDagonieNeutre;
  return tmp;
}

Carte *creerBellecour()
{
  Carte *tmp = creerCarte(249, "Bellecour", "image/cartes/bellecour.png", 2, 1, 0, 0, 4, SDL_FALSE);
  tmp->effetDirect = &effetDirectBellecour;
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
  c->idboard = 0;
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
  case 17:
    tmp = creerAisharo();
    return tmp;
  case 18:
    tmp = creerNellise();
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
  case 24:
    tmp = creerSmashMouth();
    return tmp;
  case 25:
    tmp = creerDarkPlaegueis();
    return tmp;
  case 26:
    tmp = creerArthur();
    return tmp;
  case 27:
    tmp = creerBoromir();
    return tmp;
  case 28:
    tmp = creerSuperBranleur();
    return tmp;
  case 29:
    tmp = creerAlainSoral();
    return tmp;
  case 30:
    tmp = creerHitler();
    return tmp;
  case 31:
    tmp = creerKaiba();
    return tmp;
  case 32:
    tmp = creerStaline();
    return tmp;
  case 33:
    tmp = creerLaFougere();
    return tmp;
  case 34:
    tmp = creerKaaris();
    return tmp;
  case 35:
    tmp = creerYondu();
    return tmp;
  case 36:
    tmp = creerShrek();
    return tmp;
  case 37:
    tmp = creerBlocMinecraft();
    return tmp;
  case 38:
    tmp = creerCamera();
    return tmp;
  case 39:
    tmp = creerLeCouple();
    return tmp;
  case 40:
    tmp = creerGuerrierHoc();
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
  case 201:
    tmp = creerPourLaTeam();
    return tmp;
  case 203:
    tmp = creerSel();
    return tmp;
  case 204:
    tmp = creerBlagueDeMerde();
    return tmp;
  case 208:
    tmp = creerAllStar();
    return tmp;
  case 219:
    tmp = creerPUBG();
    return tmp;
  case 223:
    tmp = creerFesses();
    return tmp;
  case 241:
    tmp = creerMadaMana();
    return tmp;
  case 243:
    tmp = creerJeRe();
    return tmp;
  case 245:
    tmp = creerSerge();
    return tmp;
  case 246:
    tmp = creerSpecialDixAbos();
    return tmp;
  case 249:
    tmp = creerBellecour();
    return tmp;
  default:
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
