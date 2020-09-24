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
                      SDL_Texture *TVictoire)
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

    int pvJoueur = 30;
    int pvEnnemi = 30;

    int manaJoueur = 0;
    int manaMaxJoueur = 0;
    int manaEnnemi = 0;
    int manaMaxEnnemi = 0;

    LCarte deckJoueur = deckChoisi;

    LCarte jeuJoueur = NULL;
    LCarte jeuEnnemi = NULL;

    LCarte boardJoueur = NULL;
    LCarte boardEnnemi = NULL;

    LCarte provocationJoueur = NULL;
    LCarte provocationEnnemi = NULL;

    Carte *terrainJoueur = NULL;
    Carte *terrainEnnemi = NULL;

    int isSwollen = 0;
    int idboard = 1;
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

    refreshMana(&TManaJoueur, manaJoueur, manaMaxJoueur);
    refreshMana(&TManaEnnemi, manaEnnemi, manaMaxEnnemi);
    //Pv des joueurs
    SDL_Texture *TPvJoueur = NULL;
    SDL_Texture *TPvEnnemi = NULL;

    refreshPv(&TPvJoueur, pvJoueur);
    refreshPv(&TPvEnnemi, pvEnnemi);

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

    //Victoire et Défaite
    SDL_Texture *TDefaite = creerTextureTexte("DEFAITE", coulTextBout, NULL, NULL);
    SDL_Texture *TVictoire = creerTextureTexte("VICTOIRE", coulTextBout, NULL, NULL);

    int victory = 0;

    //Boucle d'affichage
    SDL_Point mousePos;
    SDL_Event event;

    Carte *cartetmp = NULL;
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

    //Pour les Terrains
    int effetAttTerrainJoueur = 0;
    int effetPVTerrainJoueur = 0;
    int effetAttTerrainEnnemi = 0;
    int effetPVTerrainEnnemi = 0;
    int effetCoutTerrainJoueur = 0;
    int effetCoutTerrainEnnemi = 0;

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
                libereListeCarte(boardEnnemi);
                libereListeCarte(boardJoueur);
                libereListeCarte(jeuJoueur);
                libereListeCarte(jeuEnnemi);
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
                                 TVictoire);
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
                    if (manaMaxEnnemi < 10)
                    {
                        manaMaxEnnemi += 1;
                    }
                    manaEnnemi = manaMaxEnnemi;
                    boardEnnemi = refreshAttaque(boardEnnemi);
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
                        carteGonfle = carteClique(&mousePos, jeuJoueur, boardEnnemi, boardJoueur);
                        if (SDL_PointInRect(&mousePos, &RTerrainEnnemi) && terrainEnnemi != NULL)
                        {
                            carteGonfle = terrainEnnemi;
                        }
                        if (SDL_PointInRect(&mousePos, &RTerrainJoueur) && terrainJoueur != NULL)
                        {
                            carteGonfle = terrainJoueur;
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
                    if (SDL_PointInRect(&mousePos, &RPvEnnemi) && (provocationEnnemi == NULL))
                    {
                        printf("%s attaque face ! \n", carteAttaquante->nom);
                        pvEnnemi -= carteAttaquante->att + effetAttTerrainJoueur;
                        carteAttaquante->peutAttaquer = SDL_FALSE;
                        sprintf(buffer, "/5 %d %d", pvEnnemi , carteAttaquante->idboard);
                        carteAttaquante = NULL;
                        send(sock, buffer, strlen(buffer), 0);
                    }
                    else
                    {
                        cartetmp = carteClique(&mousePos, NULL, boardEnnemi, NULL);
                        if (cartetmp != NULL && cartetmp->id != 17 &&((provocationEnnemi == NULL) || (isIn(cartetmp, provocationEnnemi))))
                        {
                            printf("%s se fait attaquer par %s\n", cartetmp->nom, carteAttaquante->nom);
                            cartetmp->pv -= carteAttaquante->att + effetAttTerrainJoueur;
                            carteAttaquante->pv -= cartetmp->att + effetAttTerrainEnnemi;
                            carteAttaquante->peutAttaquer = SDL_FALSE;
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
                    cartetmp = carteClique(&mousePos, NULL, NULL, boardJoueur);
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
                        if (terrainEnnemi != NULL)
                        {
                            libereCarte(terrainEnnemi);
                        }
                        terrainEnnemi = terraintmp;
                        manaJoueur -= terraintmp->cout + effetCoutTerrainJoueur;
                        jeuJoueur = retirerCarte(jeuJoueur, terraintmp);
                        sprintf(buffer, "/6 %d %d", terrainEnnemi->id, 0);
                        send(sock, buffer, strlen(buffer), 0);
                        terraintmp = NULL;
                    }
                    if (SDL_PointInRect(&mousePos, &RTerrainJoueur))
                    {
                        if (terrainJoueur != NULL)
                        {
                            libereCarte(terrainJoueur);
                        }
                        terrainJoueur = terraintmp;
                        manaJoueur -= terraintmp->cout + effetCoutTerrainJoueur;
                        jeuJoueur = retirerCarte(jeuJoueur, terraintmp);
                        sprintf(buffer, "/6 %d %d", terrainJoueur->id, 1);
                        send(sock, buffer, strlen(buffer), 0);
                        terraintmp = NULL;
                    }
                }
                //Choisir la cible d'un sort
                if (isYourTurn && sorttmp != NULL && event.button.button == SDL_BUTTON_LEFT)
                {
                    cartetmp = carteClique(&mousePos, boardJoueur, boardEnnemi, NULL);
                    if (cartetmp != NULL && cartetmp->id != 17)
                    {
                        if (sorttmp->effetDirect(cartetmp, &provocationJoueur, &boardJoueur, &boardEnnemi, NULL, NULL, NULL, &jeuJoueur,&idboard) != 1)
                        {
                            printf("ERREUR LORS DU SORT DE %s\n", cartetmp->nom);
                        }
                        manaJoueur -= sorttmp->cout + effetCoutTerrainJoueur;
                        jeuJoueur = retirerCarte(jeuJoueur, sorttmp);
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
                    cartetmp = carteClique(&mousePos, jeuJoueur, NULL, NULL);
                    if (cartetmp != NULL && cartetmp->genre == 1 && cartetmp->cout + effetCoutTerrainJoueur <= manaJoueur)
                    {
                        if (estInvocable(cartetmp, boardJoueur))
                        {
                            boardJoueur = ajoutTete(boardJoueur, cartetmp);
                            if (cartetmp->effetDirect(cartetmp, &provocationJoueur, &boardJoueur, &boardEnnemi, NULL, NULL, NULL, &jeuJoueur,&idboard) != 1)
                            {
                                printf("ERREUR LORS DE L'INVOCATION DE %s\n", cartetmp->nom);
                            }
                            manaJoueur -= cartetmp->cout + effetCoutTerrainJoueur;
                            printf("L'idboard de la carte que je viens de jouer est %d \n",idboard);
                            cartetmp->idboard = idboard++;
                            sprintf(buffer, "/3 %d 0", cartetmp->id);
                            send(sock, buffer, strlen(buffer), 0);
                            jeuJoueur = retirerCarte(jeuJoueur, cartetmp);
                            cartetmp = NULL;
                        }
                    }
                    else if (cartetmp != NULL && cartetmp->genre == 3 && cartetmp->cout + effetCoutTerrainJoueur <= manaJoueur)
                    {
                        terraintmp = cartetmp;
                        cartetmp = NULL;
                    }
                    else if (cartetmp != NULL && cartetmp->genre == 2 && cartetmp->cout + effetCoutTerrainJoueur <= manaJoueur)
                    {
                        if (cartetmp->peutAttaquer)
                        {
                            sorttmp = cartetmp;
                            cartetmp = NULL;
                        }
                        else
                        {
                            if (cartetmp->effetDirect(cartetmp, &provocationJoueur, &boardJoueur, &boardEnnemi, NULL, NULL, NULL, &jeuJoueur, &idboard) != 1)
                            {
                                printf("ERREUR LORS DU SORT %s\n", cartetmp->nom);
                            }
                            manaJoueur -= cartetmp->cout + effetCoutTerrainJoueur;
                            jeuJoueur = retirerCarte(jeuJoueur, cartetmp);
                            sprintf(buffer, "/7 %d", cartetmp->id);
                            send(sock, buffer, strlen(buffer), 0);
                            cartetmp = NULL;
                        }
                    }
                }

                boardJoueur = refreshBoard(boardJoueur, &provocationJoueur, terrainJoueur, &effetPVTerrainJoueur, &effetAttTerrainJoueur, &effetCoutTerrainJoueur,&idboard);
                boardEnnemi = refreshBoard(boardEnnemi, &provocationEnnemi, terrainEnnemi, &effetPVTerrainEnnemi, &effetAttTerrainEnnemi, &effetCoutTerrainEnnemi,&idboard);

                refreshMana(&TManaJoueur, manaJoueur, manaMaxJoueur);
                refreshMana(&TManaEnnemi, manaEnnemi, manaMaxEnnemi);
                refreshPv(&TPvJoueur, pvJoueur);
                refreshPv(&TPvEnnemi, pvEnnemi);
                refreshTextureCompleteCarte(terrainJoueur, effetPVTerrainJoueur, effetAttTerrainJoueur, effetCoutTerrainJoueur);
                refreshTextureCompleteCarte(terrainEnnemi, effetPVTerrainJoueur, effetAttTerrainJoueur, effetCoutTerrainJoueur);
                refreshTexturesCompletesCartes(boardJoueur, effetPVTerrainJoueur, effetAttTerrainJoueur, effetCoutTerrainJoueur);
                refreshTexturesCompletesCartes(jeuJoueur, effetPVTerrainJoueur, effetAttTerrainJoueur, effetCoutTerrainJoueur);
                refreshTexturesCompletesCartes(boardEnnemi, effetPVTerrainEnnemi, effetAttTerrainEnnemi, effetCoutTerrainEnnemi);

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

        if (pvEnnemi <= 0 && victory == 0)
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
        afficheJeuJoueur(jeuJoueur);
        afficheJeuEnnemi(TDos, jeuEnnemi);

        afficheBoardJoueur(boardJoueur);
        afficheBoardEnnemi(boardEnnemi);

        SDL_RenderCopy(ren, TManaJoueur, NULL, &RManaJoueur);
        SDL_RenderCopy(ren, TManaEnnemi, NULL, &RManaEnnemi);

        SDL_RenderCopy(ren, TCoeur, NULL, &RCoeurEnnemi);
        SDL_RenderCopy(ren, TCoeur, NULL, &RCoeurJoueur);

        SDL_RenderCopy(ren, TPvJoueur, NULL, &RPvJoueur);
        SDL_RenderCopy(ren, TPvEnnemi, NULL, &RPvEnnemi);

        if (terrainJoueur != NULL)
        {
            SDL_RenderCopy(ren, terrainJoueur->TCarteComplete, NULL, &RTerrainJoueur);
        }

        if (terrainEnnemi != NULL)
        {
            SDL_RenderCopy(ren, terrainEnnemi->TCarteComplete, NULL, &RTerrainEnnemi);
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
                    if (!strcmp(bufftmp, "X"))
                    {
                        printf("DECONNEXION DU JOUEUR ADVERSE\n");
                        libereListeCarte(boardEnnemi);
                        libereListeCarte(boardJoueur);
                        libereListeCarte(jeuJoueur);
                        libereListeCarte(jeuEnnemi);
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
                                         TVictoire);
                        return 0;
                    }
                    if (!strcmp(bufftmp, "p"))
                    {
                        incrementeJeu(&jeuEnnemi);
                    }
                    if (!strcmp(bufftmp, "1"))
                    {
                        isYourTurn = 1;
                        if (len(jeuJoueur) >= 8)
                        {
                        }
                        else
                        {                          
                            if (!(pioche(&jeuJoueur, &deckJoueur)))
                            {
                                send(sock, "/p", strlen("/p"), 0);
                            }
                            checkStartTurn(&boardJoueur,jeuJoueur,&boardEnnemi,jeuEnnemi, &idboard,sock);
                        }
                        if (manaMaxJoueur < 10)
                        {
                            manaMaxJoueur++;
                        }
                        boardJoueur = refreshAttaque(boardJoueur);
                        manaJoueur = manaMaxJoueur;
                    }

                    if (!strcmp(bufftmp, "2"))
                    {
                        manaMaxEnnemi++;
                        manaEnnemi = manaMaxEnnemi;
                    }

                    if (bufftmp[0] == '3')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp,&idtmp2);
                        cartetmp = idtocard(idtmp);
                        printf("L'idboard de la carte qui vient d'être joué par l'adversaire est %d \n",idboard);
                        printf("Idtmp : %d et Idtmp2 : %d \n",idtmp,idtmp2);
                        cartetmp->idboard = idboard++;
                        if (idtmp2) {
                            boardJoueur = ajoutTete(boardJoueur,cartetmp);
                        }
                        else {
                            boardEnnemi = ajoutTete(boardEnnemi, cartetmp);
                            if (cartetmp->effetDirect(cartetmp, &provocationEnnemi, &boardEnnemi, &boardJoueur, NULL, NULL, NULL, &jeuEnnemi,&idboard) != 1)
                            {
                                printf("ERREUR LORS DE L'INVOCATION DE %s\n", cartetmp->nom);
                            }
                            manaEnnemi -= cartetmp->cout + effetCoutTerrainEnnemi;
                            decrementeJeu(&jeuEnnemi);
                        }
                    }
                    if (bufftmp[0] == '4')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        carteAttaquante = inBoard(idtmp, boardEnnemi);
                        cartetmp = inBoard(idtmp2, boardJoueur);
                        if (cartetmp == NULL || carteAttaquante == NULL)
                        {
                            printf("ERREUR ATTAQUE ENNEMI %p %p \n", cartetmp, carteAttaquante);
                            return 1;
                        }
                        carteAttaquante->peutAttaquer = SDL_FALSE;
                        cartetmp->pv -= carteAttaquante->att + effetAttTerrainEnnemi;
                        carteAttaquante->pv -= cartetmp->att + effetAttTerrainJoueur;
                        carteAttaquante = NULL;
                    }
                    if (bufftmp[0] == '5')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        cartetmp = inBoard(idtmp2, boardEnnemi);
                        if (cartetmp) {
                            cartetmp->peutAttaquer = SDL_FALSE;
                        }
                        pvJoueur = idtmp;
                    }
                    if (bufftmp[0] == '6')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        cartetmp = idtocard(idtmp);
                        if (idtmp2)
                        {
                            if (terrainEnnemi != NULL)
                            {
                                libereCarte(terrainEnnemi);
                            }
                            terrainEnnemi = cartetmp;
                        }
                        else
                        {
                            if (terrainJoueur != NULL)
                            {
                                libereCarte(terrainJoueur);
                            }
                            terrainJoueur = cartetmp;
                        }
                        decrementeJeu(&jeuEnnemi);
                        manaEnnemi -= cartetmp->cout + effetCoutTerrainEnnemi;
                    }
                    if (bufftmp[0] == '7')
                    {
                        sscanf(bufftmp, "%d %d", &instruction, &idtmp);
                        cartetmp = idtocard(idtmp);
                        if (cartetmp->effetDirect(cartetmp, &provocationEnnemi, &boardEnnemi, &boardJoueur, NULL, NULL, NULL, &jeuEnnemi,&idboard) != 1)
                        {
                            printf("ERREUR LORS DU SORT DE %s\n", cartetmp->nom);
                        }
                        decrementeJeu(&jeuEnnemi);
                        manaEnnemi -= cartetmp->cout + effetCoutTerrainEnnemi;
                    }

                    if (bufftmp[0] == '8')
                    {
                        sscanf(bufftmp, "%d %d %d", &instruction, &idtmp, &idtmp2);
                        sorttmp = idtocard(idtmp);
                        cartetmp = inBoard(idtmp2, boardJoueur);
                        if (cartetmp == NULL)
                        {
                            cartetmp = inBoard(idtmp2, boardEnnemi);
                        }
                        if (sorttmp->effetDirect(cartetmp, &provocationEnnemi, &boardEnnemi, &boardJoueur, NULL, NULL, NULL, &jeuEnnemi,&idboard) != 1)
                        {
                            printf("ERREUR LORS DU SORT DE %s\n", cartetmp->nom);
                        }
                        decrementeJeu(&jeuEnnemi);
                        manaEnnemi -= sorttmp->cout + effetCoutTerrainEnnemi;
                        sorttmp = NULL;
                    }

                    if (bufftmp[0] == '9')
                    {
                        victory = 1;
                    }

                    if ((strlen(bufftmp) > 1) && bufftmp[0] == '1' && bufftmp[1] == '0') {
                        sscanf(bufftmp,"%d %d %d %d", &instruction, &idtmp, &idtmp2, &idtmp3);
                        cartetmp = inBoard(idtmp,boardEnnemi);
                        cartetmp -> pv = idtmp2;
                        cartetmp -> att = idtmp3;
                    }

                    boardJoueur = refreshBoard(boardJoueur, &provocationJoueur, terrainJoueur, &effetPVTerrainJoueur, &effetAttTerrainJoueur, &effetCoutTerrainJoueur,&idboard);
                    boardEnnemi = refreshBoard(boardEnnemi, &provocationEnnemi, terrainEnnemi, &effetPVTerrainEnnemi, &effetAttTerrainEnnemi, &effetCoutTerrainEnnemi,&idboard);

                    refreshMana(&TManaJoueur, manaJoueur, manaMaxJoueur);
                    refreshMana(&TManaEnnemi, manaEnnemi, manaMaxEnnemi);
                    refreshPv(&TPvJoueur, pvJoueur);
                    refreshPv(&TPvEnnemi, pvEnnemi);
                    refreshTextureCompleteCarte(terrainJoueur, effetPVTerrainJoueur, effetAttTerrainJoueur, effetCoutTerrainJoueur);
                    refreshTextureCompleteCarte(terrainEnnemi, effetPVTerrainJoueur, effetAttTerrainJoueur, effetCoutTerrainJoueur);
                    refreshTexturesCompletesCartes(boardJoueur, effetPVTerrainJoueur, effetAttTerrainJoueur, effetCoutTerrainJoueur);
                    refreshTexturesCompletesCartes(jeuJoueur, effetPVTerrainJoueur, effetAttTerrainJoueur, effetCoutTerrainJoueur);
                    refreshTexturesCompletesCartes(boardEnnemi, effetPVTerrainEnnemi, effetAttTerrainEnnemi, effetCoutTerrainEnnemi);

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
