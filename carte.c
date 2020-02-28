#include <stdio.h>
#include <stdlib.h>

#include "carte.h"
#include "fonctions.h"

#include "SDL.h"

int creerTextureCarte(SDL_Renderer * ren, Carte * c,TTF_Font * dejavu,int modifPv , int modifAtt, int modifCout) {

    int w;
    int h;
    char vie[4];
    char att[4];
    char mana[4];

    sprintf(vie, "%d", c->pv + modifPv);
    sprintf(att, "%d", c->att + modifAtt);
    sprintf(mana, "%d", c->cout + modifCout);

    SDL_Color coulTextBout={0,0,0,255};

    SDL_Rect carte;
    SDL_QueryTexture(c->Tcarte,NULL,NULL,&w,&h);
    if (c->TcarteComplet!=NULL) {
        SDL_DestroyTexture(c->TcarteComplet);
    }
    
    c->TcarteComplet=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,w,h);
    if (c->TcarteComplet==NULL) {
      SDL_Log("Unable to create texture:%s",SDL_GetError());
      return 1;
    }
    SDL_SetRenderTarget(ren,c->TcarteComplet);

    carte.x=carte.y=0;
    carte.w=w;
    carte.h=h;
    
    SDL_SetTextureBlendMode(c->TcarteComplet, SDL_BLENDMODE_BLEND);

    SDL_RenderCopy(ren,c->Tcarte,NULL,&carte);


    //ATTAQUE
    SDL_Texture * TtextBout1 = NULL;
    SDL_Surface * textBout1 = NULL;
    textBout1=TTF_RenderText_Solid(dejavu,att,coulTextBout);
    if (!textBout1) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    TtextBout1=SDL_CreateTextureFromSurface(ren,textBout1);
    SDL_FreeSurface(textBout1);
    if (TtextBout1==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return 1;
    }
    SDL_Rect RtextBout1;
    RtextBout1.x=w/10;
    RtextBout1.w=w/5;
    RtextBout1.h=h/7;
    RtextBout1.y=(5*h)/6;

    SDL_RenderCopy(ren,TtextBout1,NULL,&RtextBout1);

    //MANA
    SDL_Texture * TtextBout2 = NULL;
    SDL_Surface * textBout2 = NULL;
    textBout2=TTF_RenderText_Solid(dejavu,mana,coulTextBout);
    if (!textBout2) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    TtextBout2=SDL_CreateTextureFromSurface(ren,textBout2);
    SDL_FreeSurface(textBout2);
    if (TtextBout2==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return 1;
    }
    SDL_Rect RtextBout2;
    RtextBout2.x=(5*w)/12;
    RtextBout2.y=(5*h)/6;
    RtextBout2.w=w/5;
    RtextBout2.h=h/7;

    SDL_RenderCopy(ren,TtextBout2,NULL,&RtextBout2);

    //VIE
    SDL_Texture * TtextBout3 = NULL;
    SDL_Surface * textBout3 = NULL;
    textBout3=TTF_RenderText_Solid(dejavu,vie,coulTextBout);
    if (!textBout3) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    TtextBout3=SDL_CreateTextureFromSurface(ren,textBout3);
    SDL_FreeSurface(textBout3);
    if (TtextBout3==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return 1;
    }
    SDL_Rect RtextBout3;
    RtextBout3.x=(21*w)/30;
    RtextBout3.y=(5*h)/6;
    RtextBout3.w=w/5;
    RtextBout3.h=h/7;

    SDL_RenderCopy(ren,TtextBout3,NULL,&RtextBout3);

    SDL_DestroyTexture(TtextBout1);
    SDL_DestroyTexture(TtextBout2);
    SDL_DestroyTexture(TtextBout3);

    SDL_SetRenderTarget(ren,NULL);

    return 0;
}

int creerTextureSort(SDL_Renderer * ren, Carte * c,TTF_Font * dejavu, int modifCout) {

    int w;
    int h;

    char mana[4];

    sprintf(mana, "%d", c->cout + modifCout);

    SDL_Color coulTextBout={0,0,0,255};

    SDL_Rect carte;
    SDL_QueryTexture(c->Tcarte,NULL,NULL,&w,&h);
    if (c->TcarteComplet!=NULL) {
        SDL_DestroyTexture(c->TcarteComplet);
    }
    c->TcarteComplet=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,w,h);
    if (c->TcarteComplet==NULL) {
      SDL_Log("Unable to create texture:%s",SDL_GetError());
      return 1;
    }
    SDL_SetRenderTarget(ren,c->TcarteComplet);

    carte.x=carte.y=0;
    carte.w=w;
    carte.h=h;

    SDL_SetTextureBlendMode(c->TcarteComplet, SDL_BLENDMODE_BLEND);

    SDL_RenderCopy(ren,c->Tcarte,NULL,&carte);

    //MANA
    SDL_Texture * TtextBout2 = NULL;
    SDL_Surface * textBout2 = NULL;
    textBout2=TTF_RenderText_Solid(dejavu,mana,coulTextBout);
    if (!textBout2) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    TtextBout2=SDL_CreateTextureFromSurface(ren,textBout2);
    SDL_FreeSurface(textBout2);
    if (TtextBout2==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return 1;
    }
    SDL_Rect RtextBout2;
    RtextBout2.x=(5*w)/12;
    RtextBout2.y=(5*h)/6;
    RtextBout2.w=w/5;
    RtextBout2.h=h/7;

    SDL_RenderCopy(ren,TtextBout2,NULL,&RtextBout2);

    SDL_DestroyTexture(TtextBout2);

    SDL_SetRenderTarget(ren,NULL);

    return 0;
}

//Texture reduite utilisée pendant la création de nouveaux decks
int creerTextureReduiteCarte(SDL_Renderer * ren,Carte * c,TTF_Font * dejavu ) {

  if (c==NULL) {
      printf("creerTextureCarte: ERREUR CARTE NULL\n");
      return 1;
  }

  SDL_Color coulTextBout={255,255,255,255};

  if (c->TcarteReduite!=NULL) {
      SDL_DestroyTexture(c->TcarteReduite);
  }

  c->TcarteReduite=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,1500,200);
  if (c->TcarteReduite==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  SDL_SetRenderTarget(ren,c->TcarteReduite);
  SDL_Rect carte;
  carte.x=carte.y=0;
  carte.w=1500;
  carte.h=200;
  if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
      printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  SDL_RenderFillRect(ren,&carte);

  SDL_Texture * text;
  SDL_Surface * Stext;
  Stext=TTF_RenderText_Solid(dejavu,c->nom,coulTextBout);
  if (!Stext) {
      printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
      exit(2);
  }
  text=SDL_CreateTextureFromSurface(ren,Stext);
  SDL_FreeSurface(Stext);
  if (text==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect Rtext;
  Rtext.x=100;
  Rtext.w=1300;
  Rtext.h=190;
  Rtext.y=5;
  SDL_RenderCopy(ren,text,NULL,&Rtext);
  SDL_DestroyTexture(text);

  SDL_SetRenderTarget(ren,NULL);

  return 0;
}

int len(Lcarte l) {
    if (l==NULL) {
        return 0;
    }
    return 1+len(l->suiv);
}

int isIn(Carte * c,Lcarte l) {
    while (l!=NULL) {
        if (l->carte==c) {
          return 1;
        }
        l=l->suiv;
    }
    return 0;
}

int idIsIn(int id,Lcarte l) {
  while (l!=NULL) {
    if (l->carte->id==id) {
      return 1;
    }
    l=l->suiv;
  }
  return 0;
}

Carte * inBoard(int idboard,Lcarte l) {
    while (l!=NULL) {
        if (l->carte->idboard==idboard) {
          return l->carte;
        }
        l=l->suiv;
    }
    return NULL;
}

Lcarte ajoutTete(Lcarte l,Carte * c) {
    Lcarte newl;
    newl=calloc(1,sizeof(*newl));
    if (newl==NULL) {
        printf("\nImpossible de rejouter cet élement dans la liste (echec calloc)\n");
    }
    newl->carte=c;
    newl->suiv=l;
    newl->Rcarte=NULL;
    return newl;
}

Lcarte retirerCarte(Lcarte l,Carte * c) {
    Lcarte tmp;
    if (l==NULL) {
      return NULL;
    }
    if (l->carte==c) {
      tmp=l->suiv;
      free(l->Rcarte);
      free(l);
      return tmp;
    }
    l->suiv=retirerCarte(l->suiv,c);
    return l;
}

SDL_Rect * copieRect(SDL_Rect * r) {
    SDL_Rect * nr;
    nr=calloc(1,sizeof(*nr));
    if (nr==NULL) {
        printf("Impossible d'allouer la mémoire pour le rectangle\n");
        return NULL;
    }
    nr->x=r->x;
    nr->y=r->y;
    nr->w=r->w;
    nr->h=r->h;
    return nr;
}

Lcarte pioche(Lcarte jeuJoueur,Lcarte * deckJoueur) {
  if (*deckJoueur!=NULL) {
    Carte * cartetmp;
    cartetmp=(*deckJoueur)->carte;
    jeuJoueur=ajoutTete(jeuJoueur,cartetmp);
    *deckJoueur=retirerCarte(*deckJoueur,cartetmp);
    return jeuJoueur;
  }
  else {
    return jeuJoueur;
  }
}

Carte * piocheRandom(SDL_Renderer * ren, TTF_Font * dejavu, int type) {
  int r = rand();
  int cartesBadLemon[17] = {2,3,4,5,213,214,216,217,218,221,222,224,226,237,248,249,250};
  int cartesKayzer[12]= {6,204,205,206,209,211,215,218,233,243,250,252};
  int cartesMusique[3]={9,207,208};
  int cartesSuppo[7]={12,207,210,217,219,229,239};
  int cartesTheReed[8]={13,203,225,228,231,240,245,246};
  int cartesRexyz[7]={15,204,209,212,228,236,250};

  switch(type) {
    case 1: //BadLemon 213,214,216,217,218,221,222,224,226,237,248,249,250,2,3,4,5
      
      return idtocard(cartesBadLemon[r%4],ren,dejavu);
    case 2: //Kayzer 6,204,205,206,209,211,215,218,233,243,250,252
      
      return idtocard(cartesKayzer[r%1],ren,dejavu);
    case 3: //Musique 9,207,208
      
      return idtocard(cartesMusique[r%1],ren,dejavu);
    case 4: //Suppo 12,207,210,217,219,229,239
      
      return idtocard(cartesSuppo[r%1],ren,dejavu);
    case 5: //TheReed 13,203,225,228,231,240,245,246
      
      return idtocard(cartesTheReed[r%2],ren,dejavu);
    case 6: //Rexyz 15,204,209,212,228,236,250
      
      return idtocard(cartesRexyz[r%1],ren,dejavu);
    default:
      break;
  }
  return NULL;
}

Lcarte refreshAttaque(Lcarte l) {
    Lcarte a=l;
    while (a!=NULL) {
        a->carte->peutAttaquer=SDL_TRUE;
        a=a->suiv;
    }
    return l;
}

void refreshCarte(SDL_Renderer * ren,Carte * c,TTF_Font * dejavu,SDL_Rect * Raff, int modifPv,int modifAtt,int modifCout) {
    switch(c->genre) {
        case 1:
          creerTextureCarte(ren,c,dejavu ,modifPv,modifAtt,modifCout);
          break;
        case 2:
          creerTextureSort(ren,c,dejavu,modifCout);
          break;
        case 3:
          creerTextureSort(ren,c,dejavu,modifCout);
          break;
        default:
          printf("ERREUR REFRESH CARTE\n");
          break;
    }
    SDL_RenderCopy(ren,c->TcarteComplet,NULL,Raff);
}

Lcarte refreshBoardJoueur(SDL_Renderer* ren,Lcarte boardJoueur,Lcarte * provocationJoueur,Carte * terrainJoueur,int *effetPVTerrainJoueur,int *effetAttTerrainJoueur,int *effetCoutTerrain) {
    //Fonction chargé de refresh les creatures mortes et les effets de terrain
    Lcarte board=boardJoueur;
    Lcarte tmp;
    if (terrainJoueur!=NULL) {
      if (terrainJoueur->effetDirect(terrainJoueur,ren,NULL,NULL,&boardJoueur,NULL,effetPVTerrainJoueur,effetAttTerrainJoueur,effetCoutTerrain,NULL) != 1) {
          printf("ERREUR LORS DE L'EFFET DU TERRAIN DE %s\n",terrainJoueur->nom);
      }
    }
    while (board!=NULL) {
        if (board->carte->pv+(*effetPVTerrainJoueur)<=0) {
            tmp=board->suiv;
            if (board->carte->raleDagonie(board->carte,ren,provocationJoueur,&boardJoueur,NULL) != 1) {
                printf("ERREUR LORS DU RALE D'AGONIE DE %s\n",board->carte->nom);
            }
            if (isIn(board->carte,*provocationJoueur)) {
                *provocationJoueur=retirerCarte(*provocationJoueur,board->carte);
            }
            boardJoueur=retirerCarte(boardJoueur,board->carte);
        }
        else {
          tmp=board->suiv;
        }
        board=tmp;
    }
    return boardJoueur;
}

Lcarte refreshBoardEnnemi(SDL_Renderer* ren,Lcarte boardEnnemi,Lcarte * provocationEnnemi,Carte * terrainEnnemi,int *effetPVTerrainEnnemi,int* effetAttTerrainEnnemi,int *effetCoutTerrain) {
    Lcarte board=boardEnnemi;
    Lcarte tmp;
    if (terrainEnnemi!=NULL) {
      if (terrainEnnemi->effetDirect(terrainEnnemi,ren,NULL,NULL,&boardEnnemi,NULL,effetPVTerrainEnnemi,effetAttTerrainEnnemi,effetCoutTerrain,NULL) != 1) {
          printf("ERREUR LORS DE L'EFFET DU TERRAIN DE %s\n",terrainEnnemi->nom);
      }
    }
    while (board!=NULL) {
        if (board->carte->pv+(*effetPVTerrainEnnemi)<=0) {
            tmp=board->suiv;
            if (board->carte->raleDagonie(board->carte,ren,provocationEnnemi,&boardEnnemi,NULL) != 1) {
                printf("ERREUR LORS DU RALE D'AGONIE DE %s\n",board->carte->nom);
            }
            if (isIn(board->carte,*provocationEnnemi)) {
                *provocationEnnemi=retirerCarte(*provocationEnnemi,board->carte);
            }
            boardEnnemi=retirerCarte(boardEnnemi,board->carte);
        }
        else {
          tmp=board->suiv;
        }
        board=tmp;
    }
    return boardEnnemi;
}

Carte * carteClique(SDL_Point * mousePos,Lcarte jeuJoueur,Lcarte boardEnnemi,Lcarte boardJoueur) {
    while (jeuJoueur!=NULL) {
      if (jeuJoueur->Rcarte!=NULL) {
        if (SDL_PointInRect(mousePos,jeuJoueur->Rcarte)) {
            return jeuJoueur->carte;
        }
      }
      jeuJoueur=jeuJoueur->suiv;
    }
    while (boardEnnemi!=NULL) {
      if (boardEnnemi->Rcarte!=NULL) {
        if (SDL_PointInRect(mousePos,boardEnnemi->Rcarte)) {
            return boardEnnemi->carte;
        }
      }
      boardEnnemi=boardEnnemi->suiv;
    }
    while (boardJoueur!=NULL) {
      if (boardJoueur->Rcarte!=NULL) {
        if (SDL_PointInRect(mousePos,boardJoueur->Rcarte)) {
            return boardJoueur->carte;
        }
      }
      boardJoueur=boardJoueur->suiv;
    }
    return NULL;
}

Lcarte creerListeCarte(SDL_Renderer * ren,TTF_Font * dejavu) {
    Lcarte listeCarte = NULL;
    Carte * tmp;

    //JJLR
    tmp = creerJJ(ren,dejavu);
    if (tmp == NULL) {
      printf("Impossible d'ajouter JJLR\n");
    }
    else {
      listeCarte=ajoutTete(listeCarte,tmp);
    }

    //BadLemon
    tmp = creerBadLemon(ren,dejavu);
    if (tmp == NULL) {
      printf("Impossible d'ajouter BadLemon\n");
    }
    else {
      listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Erwann
    tmp=creerErwann(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Erwann\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Erwannn
    tmp=creerErwannn(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Erwannn\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Erwannnn
    tmp=creerErwannnn(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Erwannnn\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Kayzer
    tmp=creerKayzer(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Kayzer\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //PizzaBoy21
    tmp=creerPizzaBoy(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter PizzaBoy21\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //BryanGaming
    tmp=creerBryanGaming(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter BryanGaming\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Mantaro
    tmp=creerMantaro(ren,dejavu);
    if (tmp==NULL) {
      printf("\nImpossible d'ajouter Mantaro");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Lucio
    tmp=creerLucio(ren,dejavu);
    if (tmp==NULL) {
      printf("\nImpossible d'ajouter Lucio");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Winston
    tmp=creerWinston(ren,dejavu);
    if (tmp==NULL) {
      printf("\nImpossible d'ajouter Winston");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Suppo
    tmp=creerSuppo(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Suppo\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //TheReed
    tmp=creerTheReed(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter TheReed\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Rexyz
    tmp=creerRexyz(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Rexyz\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Jack
    tmp=creerJack(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Erwann\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //lespetitesbites
    tmp=creerLesPetitesBites(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter lé peuttites bites\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //memphis
    tmp=creerMemphis(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Memphis\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Japan
    tmp=creerJapan(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Japan\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Appart JJ
    tmp=creerAppartJJ(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Appart JJ\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Appart TR
    tmp=creerAppartTR(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Appart TR\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Sel
    tmp=creerSel(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Sel\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    //Fesses
    tmp=creerFesses(ren,dejavu);
    if (tmp==NULL) {
        printf("\nImpossible d'ajouter Fesses\n");
    }
    else {
        listeCarte=ajoutTete(listeCarte,tmp);
    }

    return listeCarte;
}

int effetDirectProvocation(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    Lcarte * Lprovoc= provoc;
    *Lprovoc=ajoutTete(*Lprovoc,c);
    return 1;
}

int effetDirectNeutre(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    return 1;
}

int effetDirectBadLemon(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    Lcarte * jeuJoueur = jj;
    *jeuJoueur = ajoutTete(*jeuJoueur,piocheRandom(ren,dejavu,1));
    return 1;
}

int effetDirectKayzer(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    Lcarte * jeuJoueur = jj;
    *jeuJoueur = ajoutTete(*jeuJoueur,piocheRandom(ren,dejavu,2));
    return 1;
}

int effetDirectMantaro(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    Lcarte * jeuJoueur = jj;
    *jeuJoueur = ajoutTete(*jeuJoueur,piocheRandom(ren,dejavu,3));
    return 1;
}

int effetDirectSuppo(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    Lcarte * jeuJoueur = jj;
    *jeuJoueur = ajoutTete(*jeuJoueur,piocheRandom(ren,dejavu,4));
    return 1;
}

int effetDirectTheReed(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    Lcarte * jeuJoueur = jj;
    *jeuJoueur = ajoutTete(*jeuJoueur,piocheRandom(ren,dejavu,5));
    return 1;
}

int effetDirectRexyz(Carte * c,SDL_Renderer * ren,TTF_Font * dejavu,void * provoc,void * bj,void * be,int *a,int * b, int * d, void * jj) {
    Lcarte * jeuJoueur = jj;
    *jeuJoueur = ajoutTete(*jeuJoueur,piocheRandom(ren,dejavu,6));
    return 1;
}

/*
//Je vais te faire les fesses
int effetDirectFesses(Carte * c,SDL_Renderer * ren,void * provoc,void * bj,void * be,int *a,int * b, int * d) {
    c->pv-=2;
    return 1;
}

//Sel
int effetDirectSel(Carte * c,SDL_Renderer * ren,void * provoc,void * bj,void * be,int *a,int * b, int * d) {
    Lcarte * boardEnnemi= be;
    Lcarte tmpboard= *boardEnnemi;
    while (tmpboard!=NULL) {
        tmpboard->carte->pv-=2;
        tmpboard=tmpboard->suiv;
    }
    return 1;
}

//La japan Touch
int effetTerrainJapan(Carte * c,SDL_Renderer * ren,void * provoc,void * boardJoueur,void * boardEnnemi,int *effetPVTerrain,int *effetAttTerrain,int *effetCoutTerrain) {
    *effetCoutTerrain=-1;
    *effetAttTerrain=0;
    *effetPVTerrain=0;
    return 1;
}

//Le memphis coffee l'ambiance des vrais dinners américains
int effetTerrainMemphis(Carte * c,SDL_Renderer * ren,void * provoc,void * boardJoueur,void * boardEnnemi,int *effetPVTerrain,int *effetAttTerrain,int *effetCoutTerrain) {
    *effetCoutTerrain=0;
    *effetAttTerrain=0;
    *effetPVTerrain=1;
    return 1;
}

//L'appart de TheReed
int effetTerrainAppartTR(Carte * c,SDL_Renderer * ren,void * provoc,void * boardJoueur,void * boardEnnemi,int *effetPVTerrain,int *effetAttTerrain,int *effetCoutTerrain) {
    *effetCoutTerrain=0;
    *effetAttTerrain=1;
    *effetPVTerrain=0;
    return 1;
}

//L'appart de JeanJack
int effetTerrainAppartJJ(Carte * c,SDL_Renderer * ren,void * provoc,void * boardJoueur,void * boardEnnemi,int *effetPVTerrain,int *effetAttTerrain,int *effetCoutTerrain) {
    *effetCoutTerrain=1;
    *effetAttTerrain=0;
    *effetPVTerrain=0;
    return 1;
}

//Les petites bites
int effetTerrainLesPetitesBites(Carte * c,SDL_Renderer * ren,void * provoc,void * boardJoueur,void * boardEnnemi,int *effetPVTerrain,int *effetAttTerrain,int *effetCoutTerrain) {
    *effetCoutTerrain=0;
    *effetPVTerrain=0;
    *effetAttTerrain=-1;
    return 1;
}
*/
int raleDagonieNeutre(Carte * c,SDL_Renderer * ren,void * provoc, void * boardJoueur,void * boardEnnemi) {
    return 1;
}

int raleDagonieWinston(Carte * c,SDL_Renderer * ren,void * provoc, void * boardJoueur,void * boardEnnemi) {
    Lcarte * bj = boardJoueur;
    Lcarte tmpbj = *bj;
    while (tmpbj!=NULL) {
      tmpbj->carte->pv+=1;
      tmpbj=tmpbj->suiv;
    }
    return 1;
}

int raleDagonieLucio(Carte * c,SDL_Renderer * ren,void * provoc, void * boardJoueur,void * boardEnnemi) {
    Lcarte * bj = boardJoueur;
    Lcarte tmpbj = *bj;
    while (tmpbj!=NULL) {
      tmpbj->carte->att+=1;
      tmpbj=tmpbj->suiv;
    }
    return 1;
}

Carte * creerJJ(SDL_Renderer * ren, TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,1,"JeanJack Le Roux","image/cartes/JJLR.png",1,0,100,100,1,SDL_FALSE);
  tmp->effetDirect=&effetDirectNeutre;
  tmp->raleDagonie=&raleDagonieNeutre;
  return tmp;
}

Carte * creerBadLemon(SDL_Renderer * ren, TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,2,"BadLemon","image/cartes/BadLemon.png",1,1,1,1,1,SDL_FALSE);
  tmp->effetDirect=&effetDirectBadLemon;
  tmp->raleDagonie=&raleDagonieNeutre;
  return tmp;
}

Carte * creerErwann(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,3,"Erwann","image/cartes/erwann.png",1,1,2,2,2,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectNeutre;
  return tmp;
}

Carte * creerErwannn(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,4,"Erwannn","image/cartes/erwannn.png",1,1,4,4,4,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectNeutre;
  return tmp;
}

Carte * creerErwannnn(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,5,"Erwannnn","image/cartes/erwannnn.png",1,1,8,8,8,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectNeutre;
  return tmp;
}

Carte * creerKayzer(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,6,"Kayzer","image/cartes/kayzer.png",1,2,7,2,4,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectKayzer;
  return tmp;
}

Carte * creerPizzaBoy(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,7,"PizzaBoy21","image/cartes/PizzaBoy21.png",1,0,1,2,1,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectNeutre;
  return tmp;
}

Carte * creerBryanGaming(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,8,"BryanGaming","image/cartes/BryanGaming.png",1,0,2,1,1,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectNeutre;
  return tmp;
}

Carte * creerMantaro(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,9,"Mantaro","image/cartes/Mantaro.png",1,3,3,3,3,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectMantaro;
  return tmp;
}

Carte * creerLucio(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,10,"Lucio","image/cartes/Lucio.png",1,0,5,3,4,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieLucio;
  tmp->effetDirect=&effetDirectNeutre;
  return tmp;
}

Carte * creerWinston(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,11,"Winston","image/cartes/Winston.png",1,0,3,5,4,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieWinston;
  tmp->effetDirect=&effetDirectNeutre;
  return tmp;
}

Carte * creerSuppo(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,12,"Suppo","image/cartes/Suppo.png",1,4,2,7,4,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectSuppo;
  return tmp;
}

Carte * creerTheReed(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,13,"TheReed","image/cartes/TheReed.png",1,5,5,4,4,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectTheReed;
  return tmp;
}

Carte * creerRexyz(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,15,"Rexyz","image/cartes/Rexyz.png",1,6,3,2,2,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectRexyz;
  return tmp;
}

Carte * creerJack(SDL_Renderer * ren,TTF_Font * dejavu) {
  Carte * tmp = creerCarte(ren,dejavu,20,"Jack","image/cartes/jack.png",1,0,3,1,2,SDL_FALSE);
  tmp->raleDagonie=&raleDagonieNeutre;
  tmp->effetDirect=&effetDirectProvocation;
  return tmp;
}

Carte * creerCarte(SDL_Renderer * ren,TTF_Font * dejavu,int id,char * nom,char * path,int genre,int type,int pv,int att,int cout,SDL_bool peutAttaquer) {
    Carte * c;
    c=calloc(1,sizeof(*c));
    if (c==NULL) {
        printf("\nImpossible d'ajouter la carte (erreur calloc)\n");
        return NULL;
    }
    c->id=id;
    c->nom=strdup(nom);
    SDL_Texture * Tc = loadPictures(ren,path);
    if (Tc==NULL) {
        SDL_Log("Unable to create texture from image :%s",SDL_GetError());
        return NULL;
    }
    c->Tcarte=Tc;
    c->TcarteComplet=NULL;
    c->TcarteReduite=NULL;
    c->genre=genre;
    c->type=type;
    c->pvMax=c->pv=pv;
    c->attMax=c->att=att;
    c->coutMax=c->cout=cout;
    c->peutAttaquer=peutAttaquer;
    creerTextureCarte(ren,c,dejavu,0,0,0);

    return c;
}

SDL_bool estInvocable(Carte * c, Lcarte board) {
  if (c->id == 1) {
    if (idIsIn(2,board)) {
      if (idIsIn(6,board)) {
        if (idIsIn(12,board)) {
          if (idIsIn(13,board)) {
            if (idIsIn(15,board)) {
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


Carte * idtocard(int i,SDL_Renderer * ren, TTF_Font * dejavu) {
    Carte * tmp;
    switch(i) {
        case 1:
          tmp=creerJJ(ren,dejavu);
          return tmp;
        case 2:
          tmp=creerBadLemon(ren,dejavu);
          return tmp;
        case 3:
          tmp=creerErwann(ren,dejavu);
          return tmp;
        case 4:
          tmp=creerErwannn(ren,dejavu);
          return tmp;
        case 5:
          tmp=creerErwannnn(ren,dejavu);
          return tmp;
        case 6:
          tmp=creerKayzer(ren,dejavu);
          return tmp;  
        case 7:
          tmp=creerPizzaBoy(ren,dejavu);
          return tmp;
        case 8:
          tmp=creerBryanGaming(ren,dejavu);
          return tmp;
        case 9:
          tmp=creerMantaro(ren,dejavu);
          return tmp;
        case 10:
          tmp=creerLucio(ren,dejavu);
          return tmp;
        case 11:
          tmp=creerWinston(ren,dejavu);
          return tmp;
        case 12:
          tmp=creerSuppo(ren,dejavu);
          return tmp;
        case 13:
          tmp=creerTheReed(ren,dejavu);
          return tmp;
        case 15:
          tmp=creerRexyz(ren,dejavu);
          return tmp;
        case 20:
          tmp=creerJack(ren,dejavu);
          return tmp;
        case 101:
          tmp=creerLesPetitesBites(ren,dejavu);
          return tmp;
        case 102:
          tmp=creerMemphis(ren,dejavu);
          return tmp;
        case 103:
          tmp=creerJapan(ren,dejavu);
          return tmp;
        case 104:
          tmp=creerAppartJJ(ren,dejavu);
          return tmp;
        case 105:
          tmp=creerAppartTR(ren,dejavu);
          return tmp;
        case 203:
          tmp=creerSel(ren,dejavu);
          return tmp;
        case 223:
          tmp=creerFesses(ren,dejavu);
          return tmp;
        default:
          printf("ID INCONNU \n");
          return NULL;
    }
}

void libereListeCarte(Lcarte l) {
    if (l!=NULL) {
        Lcarte nl=l->suiv;

        free(l->Rcarte);
        libereCarte(l->carte);
        free(l);
        libereListeCarte(nl);
    }
}

void libereCarte(Carte * c) {
  if (c!=NULL){
    if (c->Tcarte!=NULL) {
        SDL_DestroyTexture(c->Tcarte);
    }
    if (c->TcarteComplet!=NULL) {
        SDL_DestroyTexture(c->TcarteComplet);
    }
    free(c);
  }
}
