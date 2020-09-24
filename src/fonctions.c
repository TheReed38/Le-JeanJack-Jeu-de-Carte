#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "carte.h"
#include "string.h"

extern TTF_Font *dejavu;
extern SDL_Window *win;
extern SDL_Renderer *ren;
extern SDL_Color *coulTextBout;
extern SDL_Color *coulFondBout;
extern SDL_Color *coulFondEcran;
extern int winW;
extern int winH;

void clearScreen(void)
{
    if (SDL_SetRenderDrawColor(ren, coulFondEcran->r, coulFondEcran->g, coulFondEcran->b, coulFondEcran->a) < 0)
    {
        printf("Erreur lors du changement de couleur : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_RenderClear(ren);
}

SDL_Texture *loadPictures(char *nomFichier)
{
    SDL_Surface *Simage = NULL;
    SDL_Texture *Timage = NULL;
    Simage = IMG_Load(nomFichier);
    if (Simage == NULL)
    {
        SDL_Log("Unable to load image :%s", SDL_GetError());
        return NULL;
    }
    Timage = SDL_CreateTextureFromSurface(ren, Simage);
    SDL_FreeSurface(Simage);
    return Timage;
}

Deck loadDeck(char *nomFichier)
{
    Deck deck = initDeck();
    char deckName[16];
    int tmp = 0;
    FILE *f;
    f = fopen(nomFichier, "r");
    if (f == NULL)
    {
        return deck;
    }
    deck.nom = strdup(fgets(deckName, 16, f));
    deck.nom[strlen(deck.nom) - 1] = '\0';
    while ((fscanf(f, "%d", &tmp) == 1) && (tmp != 0))
    {
        deck.deckList = ajoutTete(deck.deckList, idtocard(tmp));
        tmp = 0;
    }
    if (fclose(f) != 0)
    {
        return deck;
    }
    return deck;
}

int sauvDeck(char *nomFichier, Deck deck)
{
    FILE *f;
    LCarte deckList = deck.deckList;
    f = fopen(nomFichier, "w");
    if (f == NULL)
    {
        return 1;
    }
    fprintf(f, deck.nom);
    fprintf(f, "\n");
    while (deckList != NULL)
    {
        fprintf(f, "%d ", deckList->carte->id);
        deckList = deckList->suiv;
    }
    if (fclose(f) != 0)
    {
        return 1;
    }
    return 0;
}

SDL_Texture *SDL_CreateTextureSimplified(int width, int height)
{
    SDL_Texture *texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!texture)
    {
        SDL_Log("Unable to create texture:%s", SDL_GetError());
        exit(2);
    }
    return texture;
}

SDL_Color *initColor(int r, int g, int b, int a)
{
    SDL_Color *tmp;
    tmp = calloc(1, sizeof(*tmp));
    tmp->r = r;
    tmp->g = g;
    tmp->b = b;
    tmp->a = a;
    return tmp;
}

SDL_Rect initRect(int x, int y, int w, int h)
{
    SDL_Rect a;
    a.x = x;
    a.y = y;
    a.w = w;
    a.h = h;
    return a;
}

SDL_Rect *copieRect(SDL_Rect *r)
{
    SDL_Rect *nr;
    nr = calloc(1, sizeof(*nr));
    if (nr == NULL)
    {
        printf("Impossible d'allouer la mémoire pour le rectangle\n");
        return NULL;
    }
    nr->x = r->x;
    nr->y = r->y;
    nr->w = r->w;
    nr->h = r->h;
    return nr;
}

SDL_Texture *creerTextureTexte(char *text, SDL_Color *color, int *w, int *h)
{
    SDL_Texture *TtextBout = NULL;
    SDL_Surface *textBout = NULL;
    textBout = TTF_RenderUTF8_Blended(dejavu, text, *color);
    if (!textBout)
    {
        printf("TTF_RenderUTF8_Blended: %s\n", TTF_GetError());
        exit(2);
    }
    TtextBout = SDL_CreateTextureFromSurface(ren, textBout);
    SDL_FreeSurface(textBout);
    if (TtextBout == NULL)
    {
        SDL_Log("Unable to create texture from image :%s", SDL_GetError());
        return NULL;
    }
    if (w && h)
    {
        SDL_QueryTexture(TtextBout, NULL, NULL, w, h);
    }
    return TtextBout;
}

SDL_Texture *createBouton(int width, int height, SDL_Color *coulFond, SDL_Color *coulText, char *text)
{
    SDL_Texture *bout1 = NULL;
    SDL_Texture *TtextBout1 = NULL;
    SDL_Surface *textBout1 = NULL;
    if (text)
    {
        textBout1 = TTF_RenderUTF8_Blended(dejavu, text, *coulText);
    }
    else
    {
        textBout1 = TTF_RenderUTF8_Blended(dejavu, " ", *coulText);
    }
    if (!textBout1)
    {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    bout1 = SDL_CreateTextureSimplified(width, height);
    if (bout1 == NULL)
    {
        SDL_Log("Unable to create texture:%s", SDL_GetError());
        exit(2);
    }
    TtextBout1 = SDL_CreateTextureFromSurface(ren, textBout1);
    SDL_FreeSurface(textBout1);
    if (TtextBout1 == NULL)
    {
        SDL_Log("Unable to create texture from image :%s", SDL_GetError());
        exit(2);
    }
    SDL_Rect RBout = initRect(0, 0, width, height);
    SDL_Rect RTextBout1 = initRect(width / 20, height / 20, width - 2 * width / 20, height - 2 * height / 20);

    SDL_SetRenderTarget(ren, bout1);
    if (SDL_SetRenderDrawColor(ren, coulFond->r, coulFond->g, coulFond->b, coulFond->a) < 0)
    {
        printf("Erreur lors du changement de couleur : %s", SDL_GetError());
        exit(2);
    }
    SDL_RenderFillRect(ren, &RBout);
    SDL_RenderCopy(ren, TtextBout1, NULL, &RTextBout1);
    SDL_DestroyTexture(TtextBout1);

    SDL_SetRenderTarget(ren, NULL);

    return bout1;
}

//The swollen factor could be put in a variable
void swellRect(SDL_Rect *r)
{

    int newWidth = (r->w * 6) / 5;
    int newHeight = (r->h * 6) / 5;

    r->x = r->x - (newWidth - r->w) / 2;
    r->y = r->y - (newHeight - r->h) / 2;
    r->w = newWidth;
    r->h = newHeight;
}

SDL_Texture *refreshTextTexture(SDL_Texture *textTexture, char *text, SDL_Rect *RText)
{
    if (textTexture != NULL)
    {
        SDL_DestroyTexture(textTexture);
    }
    SDL_Surface *tmp;
    if (strlen(text) > 0)
    {
        tmp = TTF_RenderText_Solid(dejavu, text, *coulFondEcran);
    }
    else
    {
        tmp = TTF_RenderText_Solid(dejavu, " ", *coulFondEcran);
    }
    textTexture = SDL_CreateTextureFromSurface(ren, tmp);
    SDL_FreeSurface(tmp);
    SDL_QueryTexture(textTexture, NULL, NULL, &(RText->w), &(RText->h));
    RText->w /= 3;
    RText->h /= 3;
    return textTexture;
}

void refreshTextureSizesIpPrompt(SDL_Rect *RInstructions, SDL_Rect *RZoneTexte, SDL_Rect *RText, SDL_Rect *RZonePrompt)
{
    *RInstructions = initRect(winW / 2 - winW / 4, winH / 2 - winH / 3 + winH / 50, winW / 2, winH / 6);
    *RZoneTexte = initRect(winW / 2 - winW / 3 + winW / 50, winH / 2 + winH / 12, (2 * winW) / 3 - winW / 25, winH / 12);
    *RText = initRect(winW / 2 - winW / 3 + winW / 50 + winW / 100, winH / 2 + winH / 12 + winH / 100, 0, 0);
    *RZonePrompt = initRect(winW / 2 - winW / 3, winH / 2 - winH / 3, (2 * winW) / 3, (2 * winH) / 3);
}

int playerPrompt(char *instructions, char **ipAddress)
{
    SDL_Rect RInstructions;
    SDL_Rect RZoneTexte;
    SDL_Rect RText;
    SDL_Rect RZonePrompt;
    refreshTextureSizesIpPrompt(&RInstructions, &RZoneTexte, &RText, &RZonePrompt);

    //Texte
    SDL_Texture *textTexture = NULL;
    textTexture = refreshTextTexture(textTexture, " ", &RText);
    SDL_Texture *zoneTexte = createBouton(RZoneTexte.w, RZoneTexte.h, coulTextBout, coulTextBout, "The cake is a lie");
    SDL_Texture *instructionsTexture = createBouton(RInstructions.w, RInstructions.h, coulFondBout, coulTextBout, instructions);
    SDL_Texture *zonePrompt = createBouton(RZonePrompt.w, RZonePrompt.h, coulFondBout, coulFondBout, " ");

    //Boucle d'affichage
    SDL_Event event;
    SDL_StartTextInput();
    char text[16];

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            //Pour quitter
            case SDL_QUIT:
                SDL_StopTextInput();
                return 1;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_RETURN && strlen(text) > 0)
                {
                    SDL_StopTextInput();
                    *ipAddress = strdup(text);
                    return 0;
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (strlen(text) != 0)
                    {
                        text[strlen(text) - 1] = '\0';
                        textTexture = refreshTextTexture(textTexture, text, &RText);
                    }
                }
                else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                {
                    SDL_SetClipboardText(strdup(text));
                }
                else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
                {
                    strncpy(text, SDL_GetClipboardText(), 15);
                    textTexture = refreshTextTexture(textTexture, text, &RText);
                }
                break;

            case SDL_TEXTINPUT:
                /* Add new text onto the end of our text */
                if ((strlen(text) < 15) && !(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V')))
                {
                    strcat(text, event.text.text);
                    textTexture = refreshTextTexture(textTexture, text, &RText);
                }

                break;

            case SDL_WINDOWEVENT:
                if ((&event)->window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    //Rafraichissement des textures en cas de modifications de la taille de la fenêtre
                    SDL_GetWindowSize(win, &winW, &winH);
                    refreshTextureSizesIpPrompt(&RInstructions, &RZoneTexte, &RText, &RZonePrompt);
                }
                break;

                //Cas général
            default:
                break;
            }
        }
        //Affichage du menu
        clearScreen();
        SDL_RenderCopy(ren, zonePrompt, NULL, &RZonePrompt);
        SDL_RenderCopy(ren, instructionsTexture, NULL, &RInstructions);
        SDL_RenderCopy(ren, zoneTexte, NULL, &RZoneTexte);
        SDL_RenderCopy(ren, textTexture, NULL, &RText);
        SDL_RenderPresent(ren);
    }
}
