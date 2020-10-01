#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "carte.h"
#include "fonctions.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include <winsock2.h>
typedef int socklen_t;

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

void refreshTextureSizesJeu(SDL_Rect *RBoutonPasserSonTour,
                            SDL_Rect *RCarteCentre,
                            SDL_Rect *RPvJoueur,
                            SDL_Rect *RPvEnnemi,
                            SDL_Rect *RTerrainJoueur,
                            SDL_Rect *RTerrainEnnemi,
                            SDL_Rect *RZoneJeu,
                            SDL_Rect *RSeparationBoards,
                            SDL_Rect *RVictoryOrDefeat,
                            SDL_Rect *RCoeurJoueur,
                            SDL_Rect *RCoeurEnnemi,
                            SDL_Rect *RManaJoueur,
                            SDL_Rect *RManaEnnemi)
{
    *RCarteCentre = initRect(winW / 3, winH / 4, winW / 3, winH / 2);
    *RPvJoueur = initRect((7 * winW / 16), (42 * winH) / 60, (winW / 8), winH / 8);
    *RPvEnnemi = initRect((7 * winW / 16), (18 * winH) / 60 - winH / 8, (winW / 8), winH / 8);
    *RTerrainJoueur = initRect((winW / 60), (winH / 2) + (winH / 15), (winW / 10), (winH / 6));
    *RTerrainEnnemi = initRect((winW / 60), (winH / 2) - (winH / 15) - (winH / 6), (winW / 10), (winH / 6));
    *RBoutonPasserSonTour = initRect((2 * winW) / 3, (winH * 43) / 60, winW / 4, winH / 10);
    *RZoneJeu = initRect(0, winH / 6, winW, (winH * 2) / 3);
    *RSeparationBoards = initRect(0, winH / 2 - winH / 60, winW, winH / 30);
    *RVictoryOrDefeat = initRect((winW / 8), (winH / 3), (3 * winW) / 4, (winH / 3));
    *RCoeurJoueur = initRect((7 * winW / 16), (42 * winH) / 60, (winW / 8), winH / 8);
    *RCoeurEnnemi = initRect((7 * winW / 16), (18 * winH) / 60 - winH / 8, (winW / 8), winH / 8);
    *RManaJoueur = initRect(winW / 120, (winH * 5) / 6, (winW / 8), winH / 6);
    *RManaEnnemi = initRect(winW / 120, 0, (winW / 8), winH / 6);
}

void libereTextureJeu(SDL_Texture *boutonPasserSonTour,
                      SDL_Texture *TManaJoueur,
                      SDL_Texture *TManaEnnemi,
                      SDL_Texture *TPvJoueur,
                      SDL_Texture *TPvEnnemi,
                      SDL_Texture *TDos,
                      SDL_Texture *TCoeur,
                      SDL_Texture *zoneJeu,
                      SDL_Texture *separationBoards,
                      SDL_Texture *TDefaite,
                      SDL_Texture *TVictoire,
                      SDL_Texture *fondTerrain)
{
    SDL_DestroyTexture(boutonPasserSonTour);
    SDL_DestroyTexture(TManaJoueur);
    SDL_DestroyTexture(TManaEnnemi);
    SDL_DestroyTexture(TPvJoueur);
    SDL_DestroyTexture(TPvEnnemi);
    SDL_DestroyTexture(TDos);
    SDL_DestroyTexture(TCoeur);
    SDL_DestroyTexture(zoneJeu);
    SDL_DestroyTexture(separationBoards);
    SDL_DestroyTexture(TDefaite);
    SDL_DestroyTexture(TVictoire);
    SDL_DestroyTexture(fondTerrain);
}

void afficheJeuJoueur(LCarte jeuJoueur)
{
    int distanceInterCartes = (winW / 10) + (winW / 120);
    int posXcarte = (winW / 10) + (5 * winW) / 120;
    SDL_Rect carte = initRect(0, (winH * 5) / 6, winW / 10, winH / 6);
    while (jeuJoueur)
    {
        carte.x = posXcarte;
        if (jeuJoueur->RCarte)
        {
            free(jeuJoueur->RCarte);
        }
        jeuJoueur->RCarte = copieRect(&carte);
        SDL_RenderCopy(ren, jeuJoueur->carte->TCarteComplete, NULL, &carte);
        jeuJoueur = jeuJoueur->suiv;
        posXcarte += distanceInterCartes;
    }
}

void afficheJeuEnnemi(SDL_Texture *TDos, LCarte jeuEnnemi)
{
    int distanceInterCartes = (winW / 10) + (winW / 120);
    int posXcarte = (winW / 10) + (5 * winW) / 120;
    SDL_Rect carte = initRect(0, 0, winW / 10, winH / 6);
    while (jeuEnnemi)
    {
        carte.x = posXcarte;
        SDL_RenderCopy(ren, TDos, NULL, &carte);
        jeuEnnemi = jeuEnnemi->suiv;
        posXcarte += distanceInterCartes;
    }
}

void afficheBoardJoueur(LCarte boardJoueur)
{
    int distanceInterCartes = (winW / 10) + (winW / 120);
    int posXcarte = (winW / 10) + (5 * winW) / 120;
    SDL_Rect carte = initRect(0, (winH / 2) + (winH / 60), (winW / 10), winH / 6);
    while (boardJoueur)
    {
        carte.x = posXcarte;
        if (boardJoueur->RCarte)
        {
            free(boardJoueur->RCarte);
        }
        boardJoueur->RCarte = copieRect(&carte);
        SDL_RenderCopy(ren, boardJoueur->carte->TCarteComplete, NULL, &carte);
        boardJoueur = boardJoueur->suiv;
        posXcarte += distanceInterCartes;
    }
}

void afficheBoardEnnemi(LCarte boardEnnemi)
{
    int distanceInterCartes = (winW / 10) + (winW / 120);
    int posXcarte = (winW / 10) + (5 * winW) / 120;
    SDL_Rect carte = initRect(0, (winH / 2) - (winH / 6) - (winH / 60), winW / 10, winH / 6);
    while (boardEnnemi)
    {
        carte.x = posXcarte;
        if (boardEnnemi->RCarte)
        {
            free(boardEnnemi->RCarte);
        }
        boardEnnemi->RCarte = copieRect(&carte);
        SDL_RenderCopy(ren, boardEnnemi->carte->TCarteComplete, NULL, &carte);
        boardEnnemi = boardEnnemi->suiv;
        posXcarte += distanceInterCartes;
    }
}

void refreshMana(SDL_Texture **TMana, int mana, int manaMax)
{
    if (*TMana)
    {
        SDL_DestroyTexture(*TMana);
    }
    char textMana[6];
    sprintf(textMana, "%d/%d", mana, manaMax);
    *TMana = creerTextureTexte(textMana, coulTextBout, NULL, NULL);
}

void refreshPv(SDL_Texture **TPvJoueur, int pvJoueur)
{
    if (*TPvJoueur)
    {
        SDL_DestroyTexture(*TPvJoueur);
    }
    char textPv[6];
    sprintf(textPv, "%d", pvJoueur);
    *TPvJoueur = creerTextureTexte(textPv, coulTextBout, NULL, NULL);
}

int jeu(int sock, LCarte deckChoisi)
{

    printf("Initialisation en cours\n");
    char buffer[1024];
    char *bufftmp;
    int n;

    GameStatus g;
    GameStatus reversedg;
    g = calloc(1, sizeof(*g));
    g->joueur = calloc(1, sizeof(*(g->joueur)));
    g->ennemi = calloc(1, sizeof(*(g->ennemi)));
    g->idboard = calloc(1, sizeof(*(g->idboard)));
    reversedg = calloc(1, sizeof(*reversedg));

    reversedg->joueur = g->ennemi;
    reversedg->ennemi = g->joueur;
    reversedg->idboard = g->idboard;

    g->joueur->pv = 100;
    g->ennemi->pv = 100;
    g->joueur->mana = 0;
    g->joueur->manaMax = 0;
    g->ennemi->mana = 0;
    g->ennemi->manaMax = 0;

    g->joueur->deck = deckChoisi;
    g->ennemi->deck = NULL;

    g->joueur->jeu = NULL;
    g->ennemi->jeu = NULL;
    g->joueur->board = NULL;
    g->ennemi->board = NULL;
    g->joueur->provocation = NULL;
    g->ennemi->provocation = NULL;
    g->joueur->terrain = NULL;
    g->ennemi->terrain = NULL;

    int isSwollen = 0;
    (*(g->idboard)) = 1;
    int isYourTurn = 0;

    SDL_Rect RCarteCentre;
    SDL_Rect RCoeurJoueur;
    SDL_Rect RCoeurEnnemi;
    SDL_Rect RPvJoueur;
    SDL_Rect RPvEnnemi;
    SDL_Rect RTerrainJoueur;
    SDL_Rect RTerrainEnnemi;
    SDL_Rect RBoutonPasserSonTour;
    SDL_Rect RZoneJeu;
    SDL_Rect RSeparationBoards;
    SDL_Rect RVictoryOrDefeat;
    SDL_Rect RManaJoueur;
    SDL_Rect RManaEnnemi;

    refreshTextureSizesJeu(&RBoutonPasserSonTour,
                           &RCarteCentre,
                           &RPvJoueur,
                           &RPvEnnemi,
                           &RTerrainJoueur,
                           &RTerrainEnnemi,
                           &RZoneJeu,
                           &RSeparationBoards,
                           &RVictoryOrDefeat,
                           &RCoeurJoueur,
                           &RCoeurEnnemi,
                           &RManaJoueur,
                           &RManaEnnemi);

    SDL_Texture *boutonPasserSonTour = createBouton(RBoutonPasserSonTour.w, RBoutonPasserSonTour.h, coulFondBout, coulTextBout, "Passer");

    //Mana des joueurs
    SDL_Texture *TManaJoueur = NULL;
    SDL_Texture *TManaEnnemi = NULL;

    refreshMana(&TManaJoueur, g->joueur->mana, g->joueur->manaMax);
    refreshMana(&TManaEnnemi, g->ennemi->mana, g->ennemi->manaMax);
    //Pv des joueurs
    SDL_Texture *TPvJoueur = NULL;
    SDL_Texture *TPvEnnemi = NULL;

    refreshPv(&TPvJoueur, g->joueur->pv);
    refreshPv(&TPvEnnemi, g->ennemi->pv);

    //Dos des cartes
    SDL_Texture *TDos;
    TDos = loadPictures("image/cartes/dos.png");
    if (!TDos)
    {
        SDL_Log("Unable to create texture from image :%s", SDL_GetError());
        return 1;
    }

    //Coeur
    SDL_Texture *TCoeur;
    TCoeur = loadPictures("image/menu/coeur.png");
    if (!TCoeur)
    {
        SDL_Log("Unable to create texture from image :%s", SDL_GetError());
        return 1;
    }

    //Zone d'affichage du jeu
    SDL_Texture *zoneJeu = createBouton(RZoneJeu.w, RZoneJeu.h, coulFondDeck, coulFondDeck, " ");
    SDL_Texture *separationBoards = createBouton(RSeparationBoards.w, RSeparationBoards.h, coulFondEcran, coulFondEcran, " ");
    SDL_Texture *fondTerrain = createBouton(RTerrainJoueur.w, RTerrainJoueur.h, coulFondEcran, coulFondEcran, " ");

    //Victoire et Défaite
    SDL_Texture *TDefaite = creerTextureTexte("DEFAITE", coulTextBout, NULL, NULL);
    SDL_Texture *TVictoire = creerTextureTexte("VICTOIRE", coulTextBout, NULL, NULL);

    int victory = 0;

    //Boucle d'affichage
    SDL_Point mousePos;
    SDL_Event event;

    Carte *cartetmp = NULL;
    Carte *cartetmp2 = NULL;
    Carte *carteAttaquante = NULL;
    Carte *carteGonfle = NULL;
    Carte *terraintmp = NULL;
    Carte *sorttmp = NULL;

    //Pour le select
    fd_set readfs;
    int ret;
    struct timeval *tv;
    tv = calloc(1, sizeof(*tv));
    tv->tv_sec = 0;
    tv->tv_usec = 0;
    int i, j;
    int idtmp;
    int idtmp2;
    int idtmp3;
    int instruction;

    int swollenBouton = 0;
    int isFirstTurn = 1;

    //Pour les Terrains
    g->joueur->effetAttTerrain = 0;
    g->joueur->effetPvTerrain = 0;
    g->ennemi->effetAttTerrain = 0;
    g->ennemi->effetPvTerrain = 0;
    g->joueur->effetCoutTerrain = 0;
    g->ennemi->effetCoutTerrain = 0;

    printf("Initialisation achevé\n");

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            //Pour quitter
            case SDL_QUIT:
                send(sock, "/X", strlen("/X"), 0);
                libereListeCarte(g->ennemi->board);
                libereListeCarte(g->joueur->board);
                libereListeCarte(g->joueur->jeu);
                libereListeCarte(g->ennemi->jeu);
                free(g->joueur);
                free(g->ennemi);
                free(g->idboard);
                free(g);
                free(reversedg);
                libereTextureJeu(boutonPasserSonTour,
                                 TManaJoueur,
                                 TManaEnnemi,
                                 TPvJoueur,
                                 TPvEnnemi,
                                 TDos,
                                 TCoeur,
                                 zoneJeu,
                                 separationBoards,
                                 TDefaite,
                                 TVictoire,
                                 fondTerrain);
                return 1;
                break;

            //En cas de clic
            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = event.button.x;
                mousePos.y = event.button.y;

                //Passer son tour
                if (isYourTurn && SDL_PointInRect(&mousePos, &RBoutonPasserSonTour))
                {
                    isYourTurn = 0;
                    send(sock, "2", strlen("2"), 0);
                    if (g->ennemi->manaMax < 10)
                    {
                        g->ennemi->manaMax += 1;
                    }
                    g->ennemi->mana = g->ennemi->manaMax;
                    g->ennemi->board = refreshAttaque(g->ennemi->board);
                }

                //Mettre en évidence une carte
                if (isSwollen)
                {
                    isSwollen = 0;
                }
                else
                {
                    if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        carteGonfle = carteClique(&mousePos, g->joueur->jeu, g->ennemi->board, g->joueur->board);
                        if (SDL_PointInRect(&mousePos, &RTerrainEnnemi) && g->ennemi->terrain != NULL)
                        {
                            carteGonfle = g->ennemi->terrain;
                        }
                        if (SDL_PointInRect(&mousePos, &RTerrainJoueur) && g->joueur->terrain != NULL)
                        {
                            carteGonfle = g->joueur->terrain;
                        }
                        if (carteGonfle != NULL)
                        {
                            isSwollen = 1;
                        }
                    }
                }

                //ATTAQUE
                if (isYourTurn && carteAttaquante != NULL && event.button.button == SDL_BUTTON_LEFT)
                {
                    if (SDL_PointInRect(&mousePos, &RPvEnnemi) && (carteAttaquante->id != 30) && (g->ennemi->provocation == NULL))
                    {
                        printf("%s attaque face ! \n", carteAttaquante->nom);
                        g->ennemi->pv -= carteAttaquante->att + g->joueur->effetAttTerrain;
                        carteAttaquante->peutAttaquer = SDL_FALSE;
                        sprintf(buffer, "/5 %d %d", g->ennemi->pv, carteAttaquante->idboard);
                        carteAttaquante = NULL;
                        send(sock, buffer, strlen(buffer), 0);
                    }
                    else
                    {
                        cartetmp = carteClique(&mousePos, NULL, g->ennemi->board, NULL);
                        if (cartetmp != NULL && cartetmp->id != 17 && ((g->ennemi->provocation == NULL) || (isIn(cartetmp, g->ennemi->provocation))))
                        {
                            printf("%s se fait attaquer par %s\n", cartetmp->nom, carteAttaquante->nom);
                            cartetmp->pv -= carteAttaquante->att + g->joueur->effetAttTerrain;
                            carteAttaquante->pv -= cartetmp->att + g->ennemi->effetAttTerrain;
                            carteAttaquante->peutAttaquer = SDL_FALSE;
                            if (carteAttaquante->id == 30)
                            {
                                carteAttaquante->pv = -10;
                            }
                            if (carteAttaquante->pv <= 0 && carteAttaquante->id == 27)
                            {
                                cartetmp->pv = -10;
                            }
                            else if (cartetmp->pv <= 0 && cartetmp->id == 27)
                            {
                                carteAttaquante->pv = -10;
                            }
                            if (carteAttaquante->pv <= 0 && carteAttaquante->id == 33 && cartetmp -> pv > 0 )
                            {
                                g->ennemi->board = retirerCarte(g->ennemi->board,cartetmp);
                                cartetmp2 = creerLaFougere();
                                cartetmp2->idboard = *(g->idboard);
                                (*(g->idboard))++;
                                g->ennemi->board = ajoutTete(g->ennemi->board,cartetmp2);
                            }
                            else if (cartetmp->pv <= 0 && cartetmp->id == 33 && carteAttaquante -> pv > 0 )
                            {
                                g->joueur->board = retirerCarte(g->joueur->board,carteAttaquante);
                                cartetmp2 = creerLaFougere();
                                cartetmp2->idboard = *(g->idboard);
                                (*(g->idboard))++;
                                g->joueur->board = ajoutTete(g->joueur->board,cartetmp2);
                            }
                            
                            sprintf(buffer, "/4 %d %d", carteAttaquante->idboard, cartetmp->idboard);
                            send(sock, buffer, strlen(buffer), 0);
                            carteAttaquante = NULL;
                            cartetmp = NULL;
                        }
                        else
                        {
                            carteAttaquante = NULL;
                        }
                    }
                }
                else
                {
                    carteAttaquante = NULL;
                }

                //Faire attaquer un monstre
                if (isYourTurn && event.button.button == SDL_BUTTON_LEFT)
                {
                    cartetmp = carteClique(&mousePos, NULL, NULL, g->joueur->board);
                    if (cartetmp != NULL && cartetmp->peutAttaquer)
                    {
                        carteAttaquante = cartetmp;
                    }
                }

                //Choisir le côté du terrain
                if (isYourTurn && terraintmp != NULL && event.button.button == SDL_BUTTON_LEFT)
                {
                    if (SDL_PointInRect(&mousePos, &RTerrainEnnemi))
                    {
                        if (g->ennemi->terrain != NULL)
                        {
                            libereCarte(g->ennemi->terrain);
                        }
                        g->ennemi->terrain = terraintmp;
                        g->joueur->mana -= terraintmp->cout + g->joueur->effetCoutTerrain;
                        g->joueur->jeu = retirerCarte(g->joueur->jeu, terraintmp);
                        sprintf(buffer, "/6 %d %d", g->ennemi->terrain->id, 0);
                        send(sock, buffer, strlen(buffer), 0);
                        terraintmp = NULL;
                    }
                    if (SDL_PointInRect(&mousePos, &RTerrainJoueur))
                    {
                        if (g->joueur->terrain != NULL)
                        {
                            libereCarte(g->joueur->terrain);
                        }
                        g->joueur->terrain = terraintmp;
                        g->joueur->mana -= terraintmp->cout + g->joueur->effetCoutTerrain;
                        g->joueur->jeu = retirerCarte(g->joueur->jeu, terraintmp);
                        sprintf(buffer, "/6 %d %d", g->joueur->terrain->id, 1);
                        send(sock, buffer, strlen(buffer), 0);
                        terraintmp = NULL;
                    }
                }
                //Choisir la cible d'un sort
                if (isYourTurn && sorttmp != NULL && event.button.button == SDL_BUTTON_LEFT)
                {
                    cartetmp = carteClique(&mousePos, g->joueur->board, g->ennemi->board, NULL);
                    if (cartetmp != NULL && cartetmp->id != 17)
                    {
                        sorttmp->effetDirect(cartetmp, g);
                        g->joueur->mana -= sorttmp->cout + g->joueur->effetCoutTerrain;
                        g->joueur->jeu = retirerCarte(g->joueur->jeu, sorttmp);
                        sprintf(buffer, "/8 %d %d", sorttmp->id, cartetmp->idboard);
                        send(sock, buffer, strlen(buffer), 0);
                        sorttmp = NULL;
                    }
                    else
                    {
                        sorttmp = NULL;
                    }
                }
                else
                {
                    sorttmp = NULL;
                }

                //Jouer un monstre, un terrain ou un sort
                if (isYourTurn && event.button.button == SDL_BUTTON_LEFT && event.button.clicks == 2)
                {
                    cartetmp = carteClique(&mousePos, g->joueur->jeu, NULL, NULL);
                    if (cartetmp != NULL && cartetmp->genre == 1 && cartetmp->cout + g->joueur->effetCoutTerrain <= g->joueur->mana)
                    {
                        if (estInvocable(cartetmp, g->joueur->board))
                        {
                            g->joueur->board = ajoutTete(g->joueur->board, cartetmp);
                            cartetmp->idboard = (*(g->idboard))++;
                            cartetmp->effetDirect(cartetmp, g);
                            g->joueur->mana -= cartetmp->cout + g->joueur->effetCoutTerrain;
                            printf("L'idboard de la carte que je viens de jouer est %d \n", (*(g->idboard)));
                            sprintf(buffer, "/3 %d 0", cartetmp->id);
                            send(sock, buffer, strlen(buffer), 0);
                            g->joueur->jeu = retirerCarte(g->joueur->jeu, cartetmp);
                            cartetmp = NULL;
                        }
                    }
                    else if (cartetmp != NULL && cartetmp->genre == 3 && cartetmp->cout + g->joueur->effetCoutTerrain <= g->joueur->mana)
                    {
                        terraintmp = cartetmp;
                        cartetmp = NULL;
                    }
                    else if (cartetmp != NULL && cartetmp->genre == 2 && cartetmp->cout + g->joueur->effetCoutTerrain <= g->joueur->mana)
                    {
                        if (cartetmp->peutAttaquer)
                        {
                            sorttmp = cartetmp;
                            cartetmp = NULL;
                        }
                        else
                        {
                            cartetmp->effetDirect(cartetmp, g);
                            g->joueur->mana -= cartetmp->cout + g->joueur->effetCoutTerrain;
                            g->joueur->jeu = retirerCarte(g->joueur->jeu, cartetmp);
                            sprintf(buffer, "/7 %d", cartetmp->id);
                            send(sock, buffer, strlen(buffer), 0);
                            cartetmp = NULL;
                        }
                    }
                }

                refreshBoard(g);
                refreshBoard(reversedg);
                refreshMana(&TManaJoueur, g->joueur->mana, g->joueur->manaMax);
                refreshMana(&TManaEnnemi, g->ennemi->mana, g->ennemi->manaMax);
                refreshPv(&TPvJoueur, g->joueur->pv);
                refreshPv(&TPvEnnemi, g->ennemi->pv);
                refreshTextureCompleteCarte(g->joueur->terrain, g->joueur->effetPvTerrain, g->joueur->effetAttTerrain, g->joueur->effetCoutTerrain);
                refreshTextureCompleteCarte(g->ennemi->terrain, g->joueur->effetPvTerrain, g->joueur->effetAttTerrain, g->joueur->effetCoutTerrain);
                refreshTexturesCompletesCartes(g->joueur->board, g->joueur->effetPvTerrain, g->joueur->effetAttTerrain, g->joueur->effetCoutTerrain);
                refreshTexturesCompletesCartes(g->joueur->jeu, g->joueur->effetPvTerrain, g->joueur->effetAttTerrain, g->joueur->effetCoutTerrain);
                refreshTexturesCompletesCartes(g->ennemi->board, g->ennemi->effetPvTerrain, g->ennemi->effetAttTerrain, g->ennemi->effetCoutTerrain);

                break;

            case SDL_MOUSEMOTION:
                mousePos.x = event.motion.x;
                mousePos.y = event.motion.y;

                if (SDL_PointInRect(&mousePos, &RBoutonPasserSonTour) && isYourTurn)
                {
                    if (!swollenBouton)
                    {
                        Mix_PlayChannel(-1, hover, 0);
                        swollenBouton = 1;
                        swellRect(&RBoutonPasserSonTour);
                    }
                    break;
                }
                else
                {
                    swollenBouton = 0;
                    refreshTextureSizesJeu(&RBoutonPasserSonTour,
                                           &RCarteCentre,
                                           &RPvJoueur,
                                           &RPvEnnemi,
                                           &RTerrainJoueur,
                                           &RTerrainEnnemi,
                                           &RZoneJeu,
                                           &RSeparationBoards,
                                           &RVictoryOrDefeat,
                                           &RCoeurJoueur,
                                           &RCoeurEnnemi,
                                           &RManaJoueur,
                                           &RManaEnnemi);
                }
                break;

            case SDL_WINDOWEVENT:
                if ((&event)->window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    //Rafraichissement des textures en cas de modifications de la taille de la fenêtre
                    SDL_GetWindowSize(win, &winW, &winH);
                    refreshTextureSizesJeu(&RBoutonPasserSonTour,
                                           &RCarteCentre,
                                           &RPvJoueur,
                                           &RPvEnnemi,
                                           &RTerrainJoueur,
                                           &RTerrainEnnemi,
                                           &RZoneJeu,
                                           &RSeparationBoards,
                                           &RVictoryOrDefeat,
                                           &RCoeurJoueur,
                                           &RCoeurEnnemi,
                                           &RManaJoueur,
                                           &RManaEnnemi);
                }
                break;

            default:
                break;
            }
        }

        clearScreen();
        //Fond zone de Jeu
        SDL_RenderCopy(ren, zoneJeu, NULL, &RZoneJeu);
        SDL_RenderCopy(ren, separationBoards, NULL, &RSeparationBoards);
        SDL_RenderCopy(ren, fondTerrain, NULL, &RTerrainJoueur);
        SDL_RenderCopy(ren, fondTerrain, NULL, &RTerrainEnnemi);

        if (g->ennemi->pv <= 0 && victory == 0)
        {
            isYourTurn = 0;
            victory = 2;
            sprintf(buffer, "/9");
            send(sock, buffer, strlen(buffer), 0);
        }
        if (isYourTurn)
        {
            SDL_RenderCopy(ren, boutonPasserSonTour, NULL, &RBoutonPasserSonTour);
        }
        afficheJeuJoueur(g->joueur->jeu);
        afficheJeuEnnemi(TDos, g->ennemi->jeu);

        afficheBoardJoueur(g->joueur->board);
        afficheBoardEnnemi(g->ennemi->board);

        SDL_RenderCopy(ren, TManaJoueur, NULL, &RManaJoueur);
        SDL_RenderCopy(ren, TManaEnnemi, NULL, &RManaEnnemi);

        SDL_RenderCopy(ren, TCoeur, NULL, &RCoeurEnnemi);
        SDL_RenderCopy(ren, TCoeur, NULL, &RCoeurJoueur);

        SDL_RenderCopy(ren, TPvJoueur, NULL, &RPvJoueur);
        SDL_RenderCopy(ren, TPvEnnemi, NULL, &RPvEnnemi);

        if (g->joueur->terrain != NULL)
        {
            SDL_RenderCopy(ren, g->joueur->terrain->TCarteComplete, NULL, &RTerrainJoueur);
        }

        if (g->ennemi->terrain != NULL)
        {
            SDL_RenderCopy(ren, g->ennemi->terrain->TCarteComplete, NULL, &RTerrainEnnemi);
        }

        if (isSwollen)
        {
            SDL_RenderCopy(ren, carteGonfle->TCarteComplete, NULL, &RCarteCentre);
        }

        if (victory == 1)
        {
            SDL_RenderCopy(ren, TDefaite, NULL, &RVictoryOrDefeat);
        }
        if (victory == 2)
        {
            SDL_RenderCopy(ren, TVictoire, NULL, &RVictoryOrDefeat);
        }

        //Rendu
        SDL_RenderPresent(ren);

        FD_ZERO(&readfs);
        FD_SET(sock, &readfs);
        if ((ret = select(sock + 1, &readfs, NULL, NULL, tv)) < 0)
        {
            printf("Erreur select\n");
        }
        if (FD_ISSET(sock, &readfs))
        {
            if ((n = recv(sock, buffer, sizeof buffer - 1, 0)) < 0)
            {
                printf("Erreur recv\n");
            }
            buffer[n] = '\0';
            i = 0;
            j = 0;
            printf("%s\n", buffer);
            while (i < n)
            {
                if (buffer[i] == '/')
                {
                    buffer[i] = '\0';
                    j = i;
                    bufftmp = buffer + j + 1;
                    if (bufftmp[0] == 'X')
                    {
                        printf("DECONNEXION DU JOUEUR ADVERSE\n");
                        libereListeCarte(g->ennemi->board);
                        libereListeCarte(g->joueur->board);
                        libereListeCarte(g->joueur->jeu);
                        libereListeCarte(g->ennemi->jeu);
                        free(g->joueur);
                        free(g->ennemi);
                        free(g->idboard);
                        free(g);
                        free(reversedg);
                        libereTextureJeu(boutonPasserSonTour,
                                         TManaJoueur,
                                         TManaEnnemi,
                                         TPvJoueur,
                                         TPvEnnemi,
                                         TDos,
                                         TCoeur,
                                         zoneJeu,
                                         separationBoards,
                                         TDefaite,
                                         TVictoire,
                                         fondTerrain);
                        return 0;
                    }
                    if (bufftmp[0] == 'p')
                    {
                        pioche(&(g->ennemi->jeu), &(g->ennemi->deck));
                    }
                    if (bufftmp[0] == 's')
                    {
                        if (isFirstTurn)
                        {
                            sendMyDeck(sock, g);
                            for (int index = 0; index < 4; index++)
                            {
                                if (!(pioche(&(g->joueur->jeu), &(g->joueur->deck))))
                                {
                                    send(sock, "/p", strlen("/p"), 0);
                                }
                            }
                            isFirstTurn = 0;
                        }
                        isYourTurn = 1;
                        if (len(g->joueur->jeu) >= 8)
                        {
                        }
                        else
                        {
                            if (!(pioche(&(g->joueur->jeu), &(g->joueur->deck))))
                            {
                                send(sock, "/p", strlen("/p"), 0);
                            }
                            checkStartTurn(&(g->joueur->board), g->joueur->jeu, &(g->ennemi->board), g->ennemi->jeu, g->idboard, sock);
                        }
                        if (g->joueur->manaMax < 10)
                        {
                            g->joueur->manaMax++;
                        }
                        g->joueur->board = refreshAttaque(g->joueur->board);
                        g->joueur->mana = g->joueur->manaMax;
                    }

                    if (bufftmp[0] == '2')
                    {
                        if (isFirstTurn)
                        {
                            sendMyDeck(sock, g);
                            for (int index = 0; index < 4; index++)
                            {
                                if (!(pioche(&(g->joueur->jeu), &(g->joueur->deck))))
                                {
                                    send(sock, "/p", strlen("/p"), 0);
                                }
                            }
                            g->joueur->jeu = ajoutTete(g->joueur->jeu, creerMadaMana());
                            send(sock, "/p", strlen("/p"), 0);
                            isFirstTurn = 0;
                        }
                        g->ennemi->manaMax++;
                        g->ennemi->mana = g->ennemi->manaMax;
                    }

                    if (bufftmp[0] == '3')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        cartetmp = idtocard(idtmp);
                        printf("L'idboard de la carte qui vient d'être joué par l'adversaire est %d \n", (*(g->idboard)));
                        printf("Idtmp : %d et Idtmp2 : %d \n", idtmp, idtmp2);
                        cartetmp->idboard = (*(g->idboard))++;
                        if (idtmp2 == 1)
                        {
                            g->joueur->board = ajoutTete(g->joueur->board, cartetmp);
                        }
                        else
                        {
                            if (idtmp2 == 2) {
                                g->ennemi->board = ajoutTete(g->ennemi->board, cartetmp);
                            }
                            else {
                            g->ennemi->board = ajoutTete(g->ennemi->board, cartetmp);
                            cartetmp->effetDirect(cartetmp, reversedg);
                            g->ennemi->mana -= cartetmp->cout + g->ennemi->effetCoutTerrain;
                            decrementeJeu(&(g->ennemi->jeu));
                            }
                        }
                    }
                    if (bufftmp[0] == '4')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        printf("L'idboard %d attaque l'idboard %d \n",idtmp,idtmp2);
                        carteAttaquante = inBoard(idtmp, g->ennemi->board);
                        cartetmp = inBoard(idtmp2, g->joueur->board);
                        if (cartetmp == NULL || carteAttaquante == NULL)
                        {
                            printf("ERREUR ATTAQUE ENNEMI %p %p \n", cartetmp, carteAttaquante);
                            return 1;
                        }
                        carteAttaquante->peutAttaquer = SDL_FALSE;
                        cartetmp->pv -= carteAttaquante->att + g->ennemi->effetAttTerrain;
                        carteAttaquante->pv -= cartetmp->att + g->joueur->effetAttTerrain;
                        if (carteAttaquante->id == 30)
                        {
                            carteAttaquante->pv = -10;
                        }
                        if (carteAttaquante->pv <= 0 && carteAttaquante->id == 27)
                        {
                            cartetmp->pv = -10;
                        }
                        else if (cartetmp->pv <= 0 && cartetmp->id == 27)
                        {
                            carteAttaquante->pv = -10;
                        }
                        if (carteAttaquante->pv <= 0 && carteAttaquante->id == 33 && cartetmp -> pv > 0 )
                            {
                                g->joueur->board = retirerCarte(g->joueur->board,cartetmp);
                                cartetmp2 = creerLaFougere();
                                cartetmp2->idboard = *(g->idboard);
                                (*(g->idboard))++;
                                g->joueur->board = ajoutTete(g->joueur->board,cartetmp2);
                            }
                            else if (cartetmp->pv <= 0 && cartetmp->id == 33 && carteAttaquante -> pv > 0 )
                            {
                                g->ennemi->board = retirerCarte(g->ennemi->board,carteAttaquante);
                                libereCarte(carteAttaquante);
                                cartetmp2= creerLaFougere();
                                cartetmp2->idboard = *(g->idboard);
                                (*(g->idboard))++;
                                g->ennemi->board = ajoutTete(g->ennemi->board,cartetmp2);
                            }
                        carteAttaquante = NULL;
                    }
                    if (bufftmp[0] == '5')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        cartetmp = inBoard(idtmp2, g->ennemi->board);
                        if (cartetmp)
                        {
                            cartetmp->peutAttaquer = SDL_FALSE;
                        }
                        g->joueur->pv = idtmp;
                    }
                    if (bufftmp[0] == '6')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        cartetmp = idtocard(idtmp);
                        if (idtmp2)
                        {
                            if (g->ennemi->terrain != NULL)
                            {
                                libereCarte(g->ennemi->terrain);
                            }
                            g->ennemi->terrain = cartetmp;
                        }
                        else
                        {
                            if (g->joueur->terrain != NULL)
                            {
                                libereCarte(g->joueur->terrain);
                            }
                            g->joueur->terrain = cartetmp;
                        }
                        decrementeJeu(&(g->ennemi->jeu));
                        g->ennemi->mana -= cartetmp->cout + g->ennemi->effetCoutTerrain;
                    }
                    if (bufftmp[0] == '7')
                    {
                        sscanf(bufftmp, "%d %d", &instruction, &idtmp);
                        cartetmp = idtocard(idtmp);
                        cartetmp->effetDirect(cartetmp, reversedg);
                        decrementeJeu(&(g->ennemi->jeu));
                        g->ennemi->mana -= cartetmp->cout + g->ennemi->effetCoutTerrain;
                    }

                    if (bufftmp[0] == '8')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        sorttmp = idtocard(idtmp);
                        cartetmp = inBoard(idtmp2, g->joueur->board);
                        if (cartetmp == NULL)
                        {
                            cartetmp = inBoard(idtmp2, g->ennemi->board);
                        }
                        sorttmp->effetDirect(cartetmp, reversedg);
                        decrementeJeu(&(g->ennemi->jeu));
                        g->ennemi->mana -= sorttmp->cout + g->ennemi->effetCoutTerrain;
                        sorttmp = NULL;
                    }

                    if (bufftmp[0] == '9')
                    {
                        victory = 1;
                    }

                    if ((strlen(bufftmp) > 1) && bufftmp[0] == '1' && bufftmp[1] == '0')
                    {
                        sscanf(bufftmp, "%d %d %d %d", &instruction, &idtmp, &idtmp2, &idtmp3);
                        cartetmp = inBoard(idtmp, g->ennemi->board);
                        if (!cartetmp) {
                        cartetmp = inBoard(idtmp, g->joueur->board);
                        }
                        cartetmp->pv = idtmp2;
                        cartetmp->att = idtmp3;
                        
                    }

                    if ((strlen(bufftmp) > 1) && bufftmp[0] == '1' && bufftmp[1] == '1')
                    {
                        sscanf(bufftmp, "%d %d", &instruction, &idtmp);
                        printf("Je prends note que %d fait partie du deck ennemi\n", idtmp);
                        g->ennemi->deck = ajoutTete(g->ennemi->deck, idtocard(idtmp));
                    }

                    if ((strlen(bufftmp) > 1) && bufftmp[0] == '1' && bufftmp[1] == '2')
                    {
                        sscanf(bufftmp, "%d %d", &instruction, &idtmp);
                        g->ennemi->board = retirerCarte(g->ennemi->board, inBoard(idtmp,g->ennemi->board));
                    }

                    refreshBoard(g);
                    refreshBoard(reversedg);
                    refreshMana(&TManaJoueur, g->joueur->mana, g->joueur->manaMax);
                    refreshMana(&TManaEnnemi, g->ennemi->mana, g->ennemi->manaMax);
                    refreshPv(&TPvJoueur, g->joueur->pv);
                    refreshPv(&TPvEnnemi, g->ennemi->pv);
                    refreshTextureCompleteCarte(g->joueur->terrain, g->joueur->effetPvTerrain, g->joueur->effetAttTerrain, g->joueur->effetCoutTerrain);
                    refreshTextureCompleteCarte(g->ennemi->terrain, g->joueur->effetPvTerrain, g->joueur->effetAttTerrain, g->joueur->effetCoutTerrain);
                    refreshTexturesCompletesCartes(g->joueur->board, g->joueur->effetPvTerrain, g->joueur->effetAttTerrain, g->joueur->effetCoutTerrain);
                    refreshTexturesCompletesCartes(g->joueur->jeu, g->joueur->effetPvTerrain, g->joueur->effetAttTerrain, g->joueur->effetCoutTerrain);
                    refreshTexturesCompletesCartes(g->ennemi->board, g->ennemi->effetPvTerrain, g->ennemi->effetAttTerrain, g->ennemi->effetCoutTerrain);

                    buffer[i] = '\0';
                    i++;
                }
                else
                {
                    i++;
                }
            }
        }
    }
}
