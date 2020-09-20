#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

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

void libereTextureDecks(SDL_Texture *zoneDeckCree,
                        SDL_Texture *zoneDecks,
                        SDL_Texture *boutonRetour,
                        SDL_Texture *boutonSuivant,
                        SDL_Texture *boutonDeck1,
                        SDL_Texture *boutonDeck2,
                        SDL_Texture *boutonDeck3,
                        SDL_Texture *boutonPrecedent,
                        SDL_Texture *boutonCreerDeck,
                        SDL_Texture *boutonSauvegarderDeck,
                        SDL_Texture *boutonSupprimerDeck)
{
  SDL_DestroyTexture(zoneDeckCree);
  SDL_DestroyTexture(zoneDecks);
  SDL_DestroyTexture(boutonRetour);
  SDL_DestroyTexture(boutonSuivant);
  SDL_DestroyTexture(boutonDeck1);
  SDL_DestroyTexture(boutonDeck2);
  SDL_DestroyTexture(boutonDeck3);
  SDL_DestroyTexture(boutonPrecedent);
  SDL_DestroyTexture(boutonCreerDeck);
  SDL_DestroyTexture(boutonSauvegarderDeck);
  SDL_DestroyTexture(boutonSupprimerDeck);
}

void refreshTextureSizesDecks(SDL_Rect *RZoneDeckCree,
                              SDL_Rect *RZoneDecks,
                              SDL_Rect *RBoutonRetour,
                              SDL_Rect *RBoutonSuivant,
                              SDL_Rect *RBoutonPrecedent,
                              SDL_Rect *RBoutonDeck1,
                              SDL_Rect *RBoutonDeck2,
                              SDL_Rect *RBoutonDeck3,
                              SDL_Rect *RBoutonCreerDeck,
                              SDL_Rect *RBoutonSauvegarderDeck,
                              SDL_Rect *RBoutonSupprimerDeck)
{
  *RZoneDeckCree = initRect(0, 0, winW / 4, winH);
  *RZoneDecks = initRect(winW / 12, winH / 12, (winW * 5) / 6, (winH * 4) / 6);
  *RBoutonRetour = initRect((winW * 52) / 60, winH / 30, (winW * 7 / 60), (winH * 7) / 60);
  *RBoutonSuivant = initRect((winW / 4) + (3 * winW) / 8 + (winW / 60), (winH * 2) / 3, (winW / 10), (winH / 10));
  *RBoutonPrecedent = initRect((winW / 4) + (3 * winW) / 8 - (winW / 60) - (winW / 10), (winH * 2) / 3, (winW / 10), (winH / 10));
  *RBoutonDeck1 = initRect(winW / 12, winH / 12, (winW * 5) / 6, (winH * 4) / 18);
  *RBoutonDeck2 = initRect(winW / 12, winH / 12 + (winH * 4) / 18, (winW * 5) / 6, (winH * 4) / 18);
  *RBoutonDeck3 = initRect(winW / 12, winH / 12 + (winH * 8) / 18, (winW * 5) / 6, (winH * 4) / 18);
  *RBoutonCreerDeck = initRect(winW / 8, (winH * 49) / 60, winW / 3, winH / 8);
  *RBoutonSauvegarderDeck = initRect(winW - winW / 8 - winW / 3, (winH * 49) / 60, winW / 3, winH / 8);
  *RBoutonSupprimerDeck = initRect(winW - winW / 8 - winW / 3, (winH * 49) / 60, winW / 3, winH / 8);
}

void refreshDecks(Deck *deck1, SDL_Texture **boutonDeck1, SDL_Rect *RBoutonDeck1,
                  Deck *deck2, SDL_Texture **boutonDeck2, SDL_Rect *RBoutonDeck2,
                  Deck *deck3, SDL_Texture **boutonDeck3, SDL_Rect *RBoutonDeck3)
{
  *deck1 = loadDeck("deck1");
  *deck2 = loadDeck("deck2");
  *deck3 = loadDeck("deck3");
  if (*boutonDeck1)
  {
    SDL_DestroyTexture(*boutonDeck1);
  }
  *boutonDeck1 = createBouton(RBoutonDeck1->w, RBoutonDeck1->h, coulFondBout, coulTextBout, deck1->nom);
  if (*boutonDeck2)
  {
    SDL_DestroyTexture(*boutonDeck2);
  }
  *boutonDeck2 = createBouton(RBoutonDeck2->w, RBoutonDeck2->h, coulFondBout, coulTextBout, deck2->nom);
  if (*boutonDeck3)
  {
    SDL_DestroyTexture(*boutonDeck3);
  }
  *boutonDeck3 = createBouton(RBoutonDeck3->w, RBoutonDeck3->h, coulFondBout, coulTextBout, deck3->nom);
}

void afficheDeck(LCarte jeuJoueur)
{
  int posYcarte = 0;
  SDL_Rect RCarte = initRect(0, 0, winW / 4, winH / 30);

  while (jeuJoueur != NULL)
  {
    RCarte.y = posYcarte;
    if (jeuJoueur->RCarte != NULL)
    {
      free(jeuJoueur->RCarte);
    }
    jeuJoueur->RCarte = copieRect(&RCarte);
    SDL_RenderCopy(ren, jeuJoueur->carte->TCarteReduite, NULL, &RCarte);

    jeuJoueur = jeuJoueur->suiv;
    posYcarte += winH / 30;
  }
}

void afficheToutesLesCartes(LCarte toutesLesCartes, int page)
{
  int posXcarte = (winW / 4) + (winW * 7) / 120;
  int distanceInterCartes = (winW / 5) + (winW / 60);
  SDL_Rect RCarte = initRect(0, (winH * 3) / 12, winW / 5, winH / 3);
  int i = 0;
  while (toutesLesCartes != NULL)
  {
    if ((i >= page * 3) && (i < (page + 1) * 3))
    {
      RCarte.x = posXcarte;
      if (toutesLesCartes->RCarte != NULL)
      {
        free(toutesLesCartes->RCarte);
      }
      toutesLesCartes->RCarte = copieRect(&RCarte);
      SDL_RenderCopy(ren, toutesLesCartes->carte->TCarteComplete, NULL, &RCarte);
      posXcarte += distanceInterCartes;
    }
    else
    {
      toutesLesCartes->RCarte = NULL;
    }
    toutesLesCartes = toutesLesCartes->suiv;
    i += 1;
  }
}

int deck()
{

  int page;

  LCarte toutesLesCartes = creerListeDeToutesLesCartes();
  creerTexturesCompletesCartes(toutesLesCartes);

  //Zone d'affichage du deck en cours de Creation
  SDL_Rect RZoneDeckCree;
  SDL_Rect RZoneDecks;
  SDL_Rect RBoutonRetour;
  SDL_Rect RBoutonSuivant;
  SDL_Rect RBoutonPrecedent;
  SDL_Rect RBoutonDeck1;
  SDL_Rect RBoutonDeck2;
  SDL_Rect RBoutonDeck3;
  SDL_Rect RBoutonCreerDeck;
  SDL_Rect RBoutonSauvegarderDeck;
  SDL_Rect RBoutonSupprimerDeck;
  refreshTextureSizesDecks(&RZoneDeckCree, &RZoneDecks, &RBoutonRetour, &RBoutonSuivant, &RBoutonPrecedent, &RBoutonDeck1, &RBoutonDeck2, &RBoutonDeck3, &RBoutonCreerDeck, &RBoutonSauvegarderDeck, &RBoutonSupprimerDeck);

  SDL_Texture *zoneDeckCree = createBouton(RZoneDeckCree.w, RZoneDeckCree.h, coulFondDeck, coulFondDeck, " ");
  SDL_Texture *zoneDecks = createBouton(RZoneDecks.w, RZoneDecks.h, coulFondDeck, coulFondDeck, " ");
  SDL_Texture *boutonCreerDeck = createBouton(RBoutonCreerDeck.w, RBoutonCreerDeck.h, coulFondBout, coulTextBout, "Créer Deck");
  SDL_Texture *boutonDeck1 = NULL;
  SDL_Texture *boutonDeck2 = NULL;
  SDL_Texture *boutonDeck3 = NULL;
  SDL_Texture *boutonSauvegarderDeck = createBouton(RBoutonSauvegarderDeck.w, RBoutonSauvegarderDeck.h, coulFondBout, coulTextBout, "Sauvegarder Deck");
  SDL_Texture *boutonSupprimerDeck = createBouton(RBoutonSupprimerDeck.w, RBoutonSupprimerDeck.h, coulFondBout, coulTextBout, "Supprimer Deck");
  SDL_Texture *boutonRetour = NULL;
  boutonRetour = loadPictures("image/menu/bouton_retour.png");
  if (boutonRetour == NULL)
  {
    SDL_Log("Unable to create texture from image :%s", SDL_GetError());
    return 1;
  }

  SDL_Texture *boutonSuivant = NULL;
  boutonSuivant = loadPictures("image/menu/suivant.png");
  if (boutonRetour == NULL)
  {
    SDL_Log("Unable to create texture from image :%s", SDL_GetError());
    return 1;
  }

  SDL_Texture *boutonPrecedent = NULL;
  boutonPrecedent = loadPictures("image/menu/precedent.png");
  if (boutonPrecedent == NULL)
  {
    SDL_Log("Unable to create texture from image :%s", SDL_GetError());
    return 1;
  }

  //Variables

  int noDeck;
  Deck deck1, deck2, deck3;
  refreshDecks(&deck1, &boutonDeck1, &RBoutonDeck1, &deck2, &boutonDeck2, &RBoutonDeck2, &deck3, &boutonDeck3, &RBoutonDeck3);
  noDeck = (deck1.deckList != NULL) + (deck2.deckList != NULL) + (deck3.deckList != NULL);

  int deckIsBeingCreated = 0;
  Deck nouveauDeck;
  int tailleNouveauDeck = 0;
  int deckIsBeingModified = 0;
  int deletingADeck = 0;

  //Boucle d'affichage
  SDL_Point mousePos;
  SDL_Event event;

  Carte *cartetmp;
  Carte *carteAjoute;
  char nomFichier[10];
  int swollen = 0;
  while (1)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        libereListeCarte(toutesLesCartes);
        libereListeCarte(deck1.deckList);
        libereListeCarte(deck2.deckList);
        libereListeCarte(deck3.deckList);
        libereTextureDecks(zoneDeckCree,
                           zoneDecks,
                           boutonRetour,
                           boutonSuivant,
                           boutonDeck1,
                           boutonDeck2,
                           boutonDeck3,
                           boutonPrecedent,
                           boutonCreerDeck,
                           boutonSauvegarderDeck,
                           boutonSupprimerDeck);
        return 1;
        break;

      case SDL_MOUSEBUTTONDOWN:
        mousePos.x = event.button.x;
        mousePos.y = event.button.y;

        if (deckIsBeingCreated)
        {
          if (SDL_PointInRect(&mousePos, &RBoutonSauvegarderDeck))
          {
            Mix_PlayChannel(-1, selectSE, 0);
            if (deckIsBeingModified)
            {
              sprintf(nomFichier, "deck%d", deckIsBeingModified);
            }
            else
            {
              noDeck += 1;
              sprintf(nomFichier, "deck%d", noDeck);
            }
            playerPrompt("Renseignez le nom du deck (16 caractères max)", &(nouveauDeck.nom));
            if (sauvDeck(nomFichier, nouveauDeck) == 1)
            {
              printf("ERREUR SAUVERGARDE DECK\n");
              return 1;
            }

            deckIsBeingCreated = 0;
            deckIsBeingModified = 0;
            tailleNouveauDeck = 0;
            libereListeCarte(nouveauDeck.deckList);
            nouveauDeck.deckList = NULL;

            refreshDecks(&deck1, &boutonDeck1, &RBoutonDeck1, &deck2, &boutonDeck2, &RBoutonDeck2, &deck3, &boutonDeck3, &RBoutonDeck3);
          }
          else

              if ((page < (((len(toutesLesCartes) - 1) / 3) + 1)) && (SDL_PointInRect(&mousePos, &RBoutonSuivant)))
          {
            Mix_PlayChannel(-1, selectSE, 0);
            page += 1;
          }
          else if ((page > 0) && (SDL_PointInRect(&mousePos, &RBoutonPrecedent)))
          {

            Mix_PlayChannel(-1, selectSE, 0);
            page -= 1;
          }
          else
          {
            cartetmp = carteClique(&mousePos, toutesLesCartes, NULL, NULL);
            if (cartetmp != NULL && tailleNouveauDeck < 30)
            {
              Mix_PlayChannel(-1, selectSE, 0);
              carteAjoute = calloc(1, sizeof(*carteAjoute));
              if (carteAjoute == NULL)
              {
                printf("\nImpossible d'ajouter carteReduite (erreur calloc)\n");
                return 1;
              }
              carteAjoute->id = cartetmp->id;
              carteAjoute->nom = strdup(cartetmp->nom);
              creerTextureReduiteCarte(carteAjoute);
              nouveauDeck.deckList = ajoutTete(nouveauDeck.deckList, carteAjoute);
              tailleNouveauDeck += 1;
            }
            else
            {
              cartetmp = carteClique(&mousePos, nouveauDeck.deckList, NULL, NULL);
              if (cartetmp != NULL)
              {
                Mix_PlayChannel(-1, selectSE, 0);
                nouveauDeck.deckList = retirerCarte(nouveauDeck.deckList, cartetmp);
                tailleNouveauDeck -= 1;
              }
            }
          }
        }

        else
        {
          if (noDeck >= 1)
          {
            if (SDL_PointInRect(&mousePos, &RBoutonSupprimerDeck))
            {
              Mix_PlayChannel(-1, selectSE, 0);
              deletingADeck = 1;
              break;
            }

            if (SDL_PointInRect(&mousePos, &RBoutonDeck1))
            {
              Mix_PlayChannel(-1, selectSE, 0);
              if (deletingADeck)
              {
                libereDeck(&deck1);
                if (deck3.deckList != NULL)
                {
                  remove("deck1");
                  deck1 = deck2;
                  deck2 = deck3;
                  rename("deck2", "deck1");
                  rename("deck3", "deck2");
                  refreshDecks(&deck1, &boutonDeck1, &RBoutonDeck1, &deck2, &boutonDeck2, &RBoutonDeck2, &deck3, &boutonDeck3, &RBoutonDeck3);
                }
                else
                {
                  if (deck2.deckList != NULL)
                  {
                    remove("deck1");
                    deck1 = deck2;
                    rename("deck2", "deck1");
                    refreshDecks(&deck1, &boutonDeck1, &RBoutonDeck1, &deck2, &boutonDeck2, &RBoutonDeck2, &deck3, &boutonDeck3, &RBoutonDeck3);
                  }
                  else
                  {
                    remove("deck1");
                  }
                }
                noDeck -= 1;
              }
              else
              {
                nouveauDeck = deck1;
                deckIsBeingCreated = 1;
                deckIsBeingModified = 1;
                page = 0;
                tailleNouveauDeck = len(nouveauDeck.deckList);
                creerTexturesReduitesCartes(nouveauDeck.deckList);
              }
            }
          }
          if (noDeck >= 2)
          {
            if (SDL_PointInRect(&mousePos, &RBoutonDeck2))
            {
              Mix_PlayChannel(-1, selectSE, 0);
              if (deletingADeck)
              {
                libereDeck(&deck2);
                if (deck3.deckList != NULL)
                {
                  remove("deck2");
                  deck2.deckList = deck3.deckList;
                  deck3.deckList = NULL;
                  rename("deck3", "deck2");
                  refreshDecks(&deck1, &boutonDeck1, &RBoutonDeck1, &deck2, &boutonDeck2, &RBoutonDeck2, &deck3, &boutonDeck3, &RBoutonDeck3);
                }
                else
                {
                  remove("deck2");

                  deck2.deckList = NULL;
                }
                noDeck -= 1;
              }
              else
              {
                nouveauDeck = deck2;
                deckIsBeingCreated = 1;
                deckIsBeingModified = 2;
                page = 0;
                tailleNouveauDeck = len(nouveauDeck.deckList);
                creerTexturesReduitesCartes(nouveauDeck.deckList);
              }
            }
          }
          if (noDeck >= 3)
          {
            if (SDL_PointInRect(&mousePos, &RBoutonDeck3))
            {
              Mix_PlayChannel(-1, selectSE, 0);
              if (deletingADeck)
              {
                remove("deck3");
                libereDeck(&deck3);
                deck3.deckList = NULL;
                noDeck -= 1;
              }
              else
              {
                nouveauDeck = deck3;
                deckIsBeingCreated = 1;
                deckIsBeingModified = 3;
                page = 0;
                tailleNouveauDeck = len(nouveauDeck.deckList);
                creerTexturesReduitesCartes(nouveauDeck.deckList);
              }
            }
          }
          if (noDeck < 3)
          {
            if (SDL_PointInRect(&mousePos, &RBoutonCreerDeck))
            {
              Mix_PlayChannel(-1, selectSE, 0);
              nouveauDeck.deckList = NULL;
              tailleNouveauDeck = 0;
              page = 0;
              deckIsBeingCreated = 1;
            }
          }

          if (SDL_PointInRect(&mousePos, &RBoutonRetour))
          {
            Mix_PlayChannel(-1, selectSE, 0);
            libereListeCarte(toutesLesCartes);
            libereListeCarte(deck1.deckList);
            libereListeCarte(deck2.deckList);
            libereListeCarte(deck3.deckList);
            libereTextureDecks(zoneDeckCree, zoneDecks, boutonRetour, boutonSuivant, boutonDeck1, boutonDeck2, boutonDeck3, boutonPrecedent, boutonCreerDeck, boutonSauvegarderDeck, boutonSupprimerDeck);
            return 0;
          }
          deletingADeck = 0;
        }

        break;

      case SDL_MOUSEMOTION:
        mousePos.x = event.motion.x;
        mousePos.y = event.motion.y;

        if (deckIsBeingCreated)
        {
          //Gonflement du Bouton Sauvegarder Deck
          if (SDL_PointInRect(&mousePos, &RBoutonSauvegarderDeck))
          {
            if (!swollen)
            {
              Mix_PlayChannel(-1, hover, 0);
              swollen = 1;
              swellRect(&RBoutonSauvegarderDeck);
            }
            break;
          }
          //Gonflement des boutons suivants et precedent
          if (SDL_PointInRect(&mousePos, &RBoutonSuivant))
          {
            if (!swollen)
            {
              Mix_PlayChannel(-1, hover, 0);
              swollen = 1;
              swellRect(&RBoutonSuivant);
            }
            break;
          }

          if (SDL_PointInRect(&mousePos, &RBoutonPrecedent))
          {
            if (!swollen)
            {
              Mix_PlayChannel(-1, hover, 0);
              swollen = 1;
              swellRect(&RBoutonPrecedent);
            }
            break;
          }
        }
        else
        {
          if (noDeck < 3)
          {
            //Gonflement du Bouton Creer Deck
            if (SDL_PointInRect(&mousePos, &RBoutonCreerDeck))
            {
              if (!swollen)
              {
                Mix_PlayChannel(-1, hover, 0);
                swollen = 1;
                swellRect(&RBoutonCreerDeck);
              }
              break;
            }
          }
          if (noDeck > 0)
          {
            //Gonflement du Bouton Supprimer Deck
            if (SDL_PointInRect(&mousePos, &RBoutonSupprimerDeck))
            {
              if (!swollen)
              {
                Mix_PlayChannel(-1, hover, 0);
                swollen = 1;
                swellRect(&RBoutonSupprimerDeck);
              }
              break;
            }
          }
          //Gonflement du bouton Retour
          if (SDL_PointInRect(&mousePos, &RBoutonRetour))
          {
            if (!swollen)
            {
              Mix_PlayChannel(-1, hover, 0);
              swollen = 1;
              swellRect(&RBoutonRetour);
            }
            break;
          }
        }

        swollen = 0;
        refreshTextureSizesDecks(&RZoneDeckCree, &RZoneDecks, &RBoutonRetour, &RBoutonSuivant, &RBoutonPrecedent, &RBoutonDeck1, &RBoutonDeck2, &RBoutonDeck3, &RBoutonCreerDeck, &RBoutonSauvegarderDeck, &RBoutonSupprimerDeck);

        break;

      case SDL_WINDOWEVENT:
        if ((&event)->window.event == SDL_WINDOWEVENT_RESIZED)
        {
          SDL_GetWindowSize(win, &winW, &winH);
          refreshTextureSizesDecks(&RZoneDeckCree, &RZoneDecks, &RBoutonRetour, &RBoutonSuivant, &RBoutonPrecedent, &RBoutonDeck1, &RBoutonDeck2, &RBoutonDeck3, &RBoutonCreerDeck, &RBoutonSauvegarderDeck, &RBoutonSupprimerDeck);
        }
        break;

      //Cas général
      default:
        break;
      }

      //Affichage du menu
      clearScreen();

      if (deckIsBeingCreated)
      {
        SDL_RenderCopy(ren, zoneDeckCree, NULL, &RZoneDeckCree);
        afficheToutesLesCartes(toutesLesCartes, page);
        afficheDeck(nouveauDeck.deckList);
        SDL_RenderCopy(ren, boutonSauvegarderDeck, NULL, &RBoutonSauvegarderDeck);
        if (page < (((len(toutesLesCartes) - 1) / 3) + 1))
        {
          SDL_RenderCopy(ren, boutonSuivant, NULL, &RBoutonSuivant);
        }
        if (page > 0)
        {
          SDL_RenderCopy(ren, boutonPrecedent, NULL, &RBoutonPrecedent);
        }
      }
      else
      {
        SDL_RenderCopy(ren, zoneDecks, NULL, &RZoneDecks);

        if (noDeck >= 1)
        {
          SDL_RenderCopy(ren, boutonSupprimerDeck, NULL, &RBoutonSupprimerDeck);
          SDL_RenderCopy(ren, boutonDeck1, NULL, &RBoutonDeck1);
        }
        if (noDeck >= 2)
        {
          SDL_RenderCopy(ren, boutonDeck2, NULL, &RBoutonDeck2);
        }
        if (noDeck >= 3)
        {
          SDL_RenderCopy(ren, boutonDeck3, NULL, &RBoutonDeck3);
        }
        if (noDeck < 3)
        {
          SDL_RenderCopy(ren, boutonCreerDeck, NULL, &RBoutonCreerDeck);
        }

        SDL_RenderCopy(ren, boutonRetour, NULL, &RBoutonRetour);
      }

      SDL_RenderPresent(ren);
    }
  }
}
