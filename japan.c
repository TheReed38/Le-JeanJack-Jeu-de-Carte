#include "carte.h"
#include "fonctions.h"

#include "SDL.h"
#include "SDL_image.h"

int effetTerrainJapan(Carte * c,SDL_Renderer * ren,void * provoc,void * boardJoueur,void * boardEnnemi,int *effetPVTerrain,int *effetAttTerrain,int *effetCoutTerrain) {
    *effetCoutTerrain=-1;
    *effetAttTerrain=0;
    *effetPVTerrain=0;
    return 1;
}

int raleDagonieJapan(Carte * c,SDL_Renderer * ren,void * provoc) {
    return 1;
}

Carte * creerJapan(SDL_Renderer * ren,TTF_Font * dejavu) {
    Carte * carte;
    carte=calloc(1,sizeof(*carte));
    if (carte==NULL) {
        printf("\nImpossible d'ajouter Japan(erreur calloc)\n");
        return NULL;
    }
    carte->id=103;
    carte->nom="Japan Touch";
    SDL_Texture * Tcarte = loadPictures(ren,"image/cartes/japan.png");
    if (Tcarte==NULL) {
        SDL_Log("Unable to create texture from image :%s",SDL_GetError());
        return NULL;
    }
    carte->Tcarte=Tcarte;
    carte->TcarteComplet=NULL;
    carte->TcarteReduite=NULL;
    carte->genre=3;
    carte->type=0;
    carte->pvMax=carte->pv=0;
    carte->attMax=carte->att=0;
    carte->coutMax=carte->cout=3;
    carte->peutAttaquer=SDL_TRUE;
    carte->effetDirect=&effetTerrainJapan;
    carte->raleDagonie=&raleDagonieJapan;
    creerTextureSort(ren,carte,dejavu,0);

    return carte;
}
