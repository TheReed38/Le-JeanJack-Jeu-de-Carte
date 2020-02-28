#include "carte.h"
#include "fonctions.h"

#include "SDL.h"
#include "SDL_image.h"

int effetDirectSel(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    Lcarte * boardEnnemi= be;
    Lcarte tmpboard= *boardEnnemi;
    while (tmpboard!=NULL) {
        tmpboard->carte->pv-=2;
        tmpboard=tmpboard->suiv;
    }
    return 1;
}

int raleDagonieSel(Carte * c,SDL_Renderer * ren,void * provoc) {
    return 1;
}

Carte * creerSel(SDL_Renderer * ren,TTF_Font * dejavu) {
    Carte * carte;
    carte=calloc(1,sizeof(*carte));
    if (carte==NULL) {
        printf("\nImpossible d'ajouter Sel (erreur calloc)\n");
        return NULL;
    }
    carte->id=203;
    carte->nom="Sel";
    SDL_Texture * Tcarte = loadPictures(ren,"image/cartes/sel.png");
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
    carte->coutMax=carte->cout=4;
    carte->peutAttaquer=SDL_FALSE;
    carte->effetDirect=&effetDirectSel;
    carte->raleDagonie=&raleDagonieSel;
    creerTextureSort(ren,carte,dejavu,0);

    return carte;
}
