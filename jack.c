#include "carte.h"
#include "fonctions.h"

#include "SDL.h"
#include "SDL_image.h"

int effetDirectJack(Carte * c,SDL_Renderer * ren,void * provoc,void * bj,void * be,int *a,int * b, int * d) {
    Lcarte * Lprovoc= provoc;
    *Lprovoc=ajoutTete(*Lprovoc,c);
    return 1;
}

int raleDagonieJack(Carte * c,SDL_Renderer * ren,void * provoc) {
    return 1;
}

Carte * creerJack(SDL_Renderer * ren,TTF_Font * dejavu) {
    Carte * carte;
    carte=calloc(1,sizeof(*carte));
    if (carte==NULL) {
        printf("\nImpossible d'ajouter Jack (erreur calloc)\n");
        return NULL;
    }
    carte->id=20;
    carte->nom="Jack";
    SDL_Texture * Tcarte = loadPictures(ren,"image/cartes/jack.png");
    if (Tcarte==NULL) {
        SDL_Log("Unable to create texture from image :%s",SDL_GetError());
        return NULL;
    }
    carte->Tcarte=Tcarte;
    carte->TcarteComplet=NULL;
    carte->TcarteReduite=NULL;
    carte->genre=1;
    carte->type=0;
    carte->pvMax=carte->pv=3;
    carte->attMax=carte->att=1;
    carte->coutMax=carte->cout=2;
    carte->peutAttaquer=SDL_FALSE;
    carte->effetDirect=&effetDirectJack;
    carte->raleDagonie=&raleDagonieJack;
    creerTextureCarte(ren,carte,dejavu,0,0,0);

    return carte;
}
