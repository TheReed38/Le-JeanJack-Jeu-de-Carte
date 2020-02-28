#include "carte.h"
#include "fonctions.h"

#include "SDL.h"
#include "SDL_image.h"

int effetDirectFesses(Carte * c,SDL_Renderer * ren,void * provoc,void * bj,void * be,int *a,int * b, int * d) {
    c->pv-=2;
    return 1;
}

int raleDagonieFesses(Carte * c,SDL_Renderer * ren,void * provoc) {
    return 1;
}

Carte * creerFesses(SDL_Renderer * ren,TTF_Font * dejavu) {
    Carte * carte;
    carte=calloc(1,sizeof(*carte));
    if (carte==NULL) {
        printf("\nImpossible d'ajouter Fesses (erreur calloc)\n");
        return NULL;
    }
    carte->id=223;
    carte->nom="J'vais te faire les fesses";
    SDL_Texture * Tcarte = loadPictures(ren,"image/cartes/fesses.png");
    if (Tcarte==NULL) {
        SDL_Log("Unable to create texture from image :%s",SDL_GetError());
        return NULL;
    }
    carte->Tcarte=Tcarte;
    carte->TcarteComplet=NULL;
    carte->TcarteReduite=NULL;
    carte->genre=2;
    carte->type=0;
    carte->pvMax=carte->pv=0;
    carte->attMax=carte->att=0;
    carte->coutMax=carte->cout=2;
    carte->peutAttaquer=SDL_TRUE;
    carte->effetDirect=&effetDirectFesses;
    carte->raleDagonie=&raleDagonieFesses;
    creerTextureSort(ren,carte,dejavu,0);

    return carte;
}
