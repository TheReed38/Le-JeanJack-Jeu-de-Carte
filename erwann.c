#include "carte.h"
#include "fonctions.h"

#include "SDL.h"
#include "SDL_image.h"

int effetDirectErwann(Carte * c,SDL_Renderer * ren,void * provoc,void * bj,void * be,int *a,int * b, int * d) {
    return 1;
}

int raleDagonieErwann(Carte * c,SDL_Renderer * ren,void * provoc) {
    return 1;
}

Carte * creerErwann(SDL_Renderer * ren,TTF_Font * dejavu) {
    Carte * erwann;
    erwann=calloc(1,sizeof(*erwann));
    if (erwann==NULL) {
        printf("\nImpossible d'ajouter Erwann (erreur calloc)\n");
        return NULL;
    }
    erwann->id=3;
    erwann->nom="Erwann";
    SDL_Texture * Terwann = loadPictures(ren,"image/cartes/erwann.png");
    if (Terwann==NULL) {
        SDL_Log("Unable to create texture from image :%s",SDL_GetError());
        return NULL;
    }
    erwann->Tcarte=Terwann;
    erwann->TcarteComplet=NULL;
    erwann->TcarteReduite=NULL;
    erwann->genre=1;
    erwann->type=0;
    erwann->pvMax=erwann->pv=2;
    erwann->attMax=erwann->att=2;
    erwann->coutMax=erwann->cout=2;
    erwann->peutAttaquer=SDL_FALSE;
    erwann->effetDirect=&effetDirectErwann;
    erwann->raleDagonie=&raleDagonieErwann;
    creerTextureCarte(ren,erwann,dejavu,0,0,0);

    return erwann;
}
