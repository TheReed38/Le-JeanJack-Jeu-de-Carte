#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include "fonctions.h"
#include "carte.h"

#if defined (WIN32)
#include <windows.h>
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow)
#else
int main(int argc, char *argv[]) 
#endif
{

  SDL_SetMainReady();

  int winW=800;
  int winH=600;

  int posLogoX=250;
  int posLogoY=20;
  int posBoutX=200;
  int posBoutY1=200;
  int posBoutY2=350;
  int posBoutY3=500;

  int largBout=200;
  int hautBout=50;

  int tailleCara=100;
  
  srand(time(NULL));

  SDL_Color coulTextBout={255,255,255,255};

  Uint32 fullscreen=SDL_WINDOW_FULLSCREEN;

  //Initialisation de SDL et TTF et mixer
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
      return 1;
  }

  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) == -1 )
  {
      return 1;
  }

  if(TTF_Init()==-1)
  {
      printf("TTF_Init: %s\n", TTF_GetError());
      exit(2);
  }

  TTF_Font * dejavu = NULL;
  dejavu = TTF_OpenFont("Fonts/dejavu/DejaVuSans.ttf",tailleCara);

  if(!dejavu) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      exit(2);
      // handle error
  }

  SDL_Window * fen = NULL;
  SDL_Renderer * ren = NULL;

  //Creation de la fenetre et du renderer

  fen = SDL_CreateWindow("Le Jean Jack Jeu de Carte" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 600 , 600 , SDL_WINDOW_RESIZABLE);
  if(fen == NULL){
      printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
      return EXIT_FAILURE;
  }
  ren = SDL_CreateRenderer(fen, -1, SDL_RENDERER_SOFTWARE);
  if(ren == NULL) {
      printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
      return EXIT_FAILURE;
  }

  //Chargement des cartes
  Lcarte listeCartes = NULL;
  listeCartes = creerListeCarte(ren,dejavu);


  /*
  if ((SDL_CreateWindowAndRenderer(600,600,SDL_WINDOW_RESIZABLE,&fen,&ren))||(fen==NULL)||(ren==NULL)) {
      SDL_Log("Unable to create fen or ren :%s",SDL_GetError());
      return 1;
  }
  */

  //Creation des textures

  //Background

  SDL_Rect Rscreen;
  Rscreen.x=0;
  Rscreen.y=0;
  SDL_GetWindowSize(fen,&Rscreen.w,&Rscreen.h);


  //Logo de la JJT
  //SDL_Surface * logo = NULL
  SDL_Texture * logoT = loadPictures(ren,"logoRed.png");
  /*logo=IMG_Load("logoRed.png");
  if (logo==NULL) {
    SDL_Log("Unable to load image :%s",SDL_GetError());
    return 1;
  }
  logoT=SDL_CreateTextureFromSurface(ren,logo);
  SDL_FreeSurface(logo);*/
  if (logoT==NULL) {
      SDL_Log("Unable to create texture from image :%s",SDL_GetError());
      return 1;
  }
  SDL_Rect Rlogo;
  SDL_QueryTexture(logoT,NULL,NULL,&Rlogo.w,&Rlogo.h);
  Rlogo.x=posLogoX;
  Rlogo.y=posLogoY;

  //Couleur fond bouton

  if (SDL_SetRenderDrawColor(ren,120,120,120,255)<0) {
      printf("Erreur lors du changement de couleur : %s",SDL_GetError());
      return EXIT_FAILURE;
  }

  //Bouton Jouer
  SDL_Texture * bout1 = NULL;
  SDL_Texture * TtextBout1 = NULL;
  SDL_Surface * textBout1 = NULL;
  textBout1=TTF_RenderText_Solid(dejavu,"Jouer",coulTextBout);
  if (!textBout1) {
      printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
      exit(2);
  }
  bout1=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,largBout,hautBout);
  if (bout1==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  TtextBout1=SDL_CreateTextureFromSurface(ren,textBout1);
  SDL_FreeSurface(textBout1);
  if (TtextBout1==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect RBout1;
  SDL_Rect RtextBout1;
  RtextBout1.x=posBoutX+10;
  RtextBout1.y=posBoutY1+5;
  RtextBout1.w=largBout-20;
  RtextBout1.h=hautBout-10;
  RBout1.x=0;
  RBout1.y=0;
  RBout1.w=largBout;
  RBout1.h=hautBout;
  SDL_SetRenderTarget(ren,bout1);
  SDL_RenderFillRect(ren,&RBout1);
  RBout1.x=posBoutX;
  RBout1.y=posBoutY1;


  //Bouton Decks
  SDL_Texture * bout2 = NULL;
  SDL_Texture * TtextBout2 = NULL;
  SDL_Surface * textBout2 = NULL;
  textBout2=TTF_RenderText_Solid(dejavu,"Decks",coulTextBout);
  if (!textBout2) {
      printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
      exit(2);
  }
  bout2=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,largBout,hautBout);
  if (bout2==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  TtextBout2=SDL_CreateTextureFromSurface(ren,textBout2);
  SDL_FreeSurface(textBout2);
  if (TtextBout2==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect RBout2;
  SDL_Rect RtextBout2;
  RtextBout2.x=posBoutX+10;
  RtextBout2.y=posBoutY2+5;
  RtextBout2.w=largBout-20;
  RtextBout2.h=hautBout-10;
  RBout2.x=0;
  RBout2.y=0;
  RBout2.w=largBout;
  RBout2.h=hautBout;
  SDL_SetRenderTarget(ren,bout2);
  SDL_RenderFillRect(ren,&RBout2);
  RBout2.x=posBoutX;
  RBout2.y=posBoutY2;

  //Bouton Options
  SDL_Texture * bout3 = NULL;
  SDL_Texture * TtextBout3 = NULL;
  SDL_Surface * textBout3 = NULL;
  textBout3=TTF_RenderText_Solid(dejavu,"Options",coulTextBout);
  if (!textBout3) {
      printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
      exit(2);
  }
  bout3=SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,largBout,hautBout);
  if (bout3==NULL) {
    SDL_Log("Unable to create texture:%s",SDL_GetError());
    return 1;
  }
  TtextBout3=SDL_CreateTextureFromSurface(ren,textBout3);
  SDL_FreeSurface(textBout3);
  if (TtextBout3==NULL) {
    SDL_Log("Unable to create texture from image :%s",SDL_GetError());
    return 1;
  }
  SDL_Rect RBout3;
  SDL_Rect RtextBout3;
  RtextBout3.x=posBoutX+10;
  RtextBout3.y=posBoutY3+5;
  RtextBout3.w=largBout-20;
  RtextBout3.h=hautBout-10;
  RBout3.x=0;
  RBout3.y=0;
  RBout3.w=largBout;
  RBout3.h=hautBout;
  SDL_SetRenderTarget(ren,bout3);
  SDL_RenderFillRect(ren,&RBout3);
  RBout3.x=posBoutX;
  RBout3.y=posBoutY3;

  SDL_SetRenderTarget(ren,NULL);

  //Musique et Effets
  Mix_Music * mainTheme = NULL;
  Mix_Music * lobbyTheme = NULL;
  Mix_Music * deckTheme = NULL;
  Mix_Chunk * hoverButton = NULL;
  Mix_Chunk * select=NULL;

  mainTheme=Mix_LoadMUS("sound/music/mainTheme.wav");
  deckTheme=Mix_LoadMUS("sound/music/deckTheme.wav");
  lobbyTheme=Mix_LoadMUS("sound/music/lobbyTheme.wav");
  hoverButton=Mix_LoadWAV("sound/se/click1.ogg");
  select=Mix_LoadWAV("sound/se/click2.ogg");

  if( Mix_PlayMusic( mainTheme, -1 ) == -1 ) {
      return 1;
  }
  //Boucle principale
  SDL_Point mousePos;
  SDL_Event event;
  int quitValue;
  int done = 0;
  int soundPlayed=0;
  while (!done) {
      while(SDL_PollEvent(&event)) {
          switch(event.type) {
              //En cas de clic
              case SDL_MOUSEBUTTONDOWN:
                  //printf("\nTu as clique je le sais\n");
                  mousePos.x=event.button.x;
                  mousePos.y=event.button.y;
                  if (SDL_PointInRect(&mousePos,&RBout1)) {
                      //printf("\nTu as cliqu sur jouer !\n");
                      Mix_PlayChannel(-1,select,0);
                      if( Mix_PlayMusic( lobbyTheme, -1 ) == -1 ) {
                        return 1;
                      }
                      quitValue = lobby(fen,ren,dejavu,hoverButton,select);
                      if( Mix_PlayMusic( mainTheme, -1 ) == -1 ) {
                        return 1;
                      }
                      if (quitValue==1) {
                          done=1;
                      }
                      refreshMain(fen,&winW,&winH,&Rlogo,&RBout1,&RBout2,&RBout3,&RtextBout1,&RtextBout2,&RtextBout3);
                  }
                  if (SDL_PointInRect(&mousePos,&RBout2)) {
                      //printf("\nTu as clique sur Decks !\n");
                      //Mix_HaltMusic();
                      Mix_PlayChannel(-1,select,0);
                      if( Mix_PlayMusic( deckTheme, -1 ) == -1 ) {
                        return 1;
                      }
                      quitValue = deck(fen,ren,dejavu,hoverButton,select);
                      if( Mix_PlayMusic( mainTheme, -1 ) == -1 ) {
                        return 1;
                      }
                      if (quitValue==1) {
                          done=1;
                      }
                      refreshMain(fen,&winW,&winH,&Rlogo,&RBout1,&RBout2,&RBout3,&RtextBout1,&RtextBout2,&RtextBout3);
                  }
                  if (SDL_PointInRect(&mousePos,&RBout3)) {
                      //printf("\nTu as clique sur Options !\n");
                      Mix_PlayChannel(-1,select,0);
                      quitValue = options(fen,ren,dejavu,&fullscreen,hoverButton,select);
                      if (quitValue==1) {
                          done=1;
                      }
                      refreshMain(fen,&winW,&winH,&Rlogo,&RBout1,&RBout2,&RBout3,&RtextBout1,&RtextBout2,&RtextBout3);
                  }

                  break;

              case SDL_MOUSEMOTION:
                  mousePos.x=event.motion.x;
                  mousePos.y=event.motion.y;

                  //Gonflement du bouton 1
                  if (SDL_PointInRect(&mousePos,&RBout1)) {
                      if (!soundPlayed) {
                          Mix_PlayChannel(-1,hoverButton,0);
                          soundPlayed=1;
                      }
                      RBout1.x=(winW/3)-(winW/120);
                      RBout1.y=(winH/3)-(winH/200)-(winH/20);
                      RBout1.w=(winW/3)+(winW/60);
                      RBout1.h=(winH/8)+(winH/100);
                      RtextBout1.x=(winW*21)/60-(winW/120);
                      RtextBout1.y=winH/3+(winH/100)-(winH/200)-(winH/20);
                      RtextBout1.w=(winW*9)/30+(winW/60);
                      RtextBout1.h=winH/10+(winH/100);
                  }
                  else {
                    //Gonflement du bouton 2
                    if (SDL_PointInRect(&mousePos,&RBout2)) {
                      if (!soundPlayed) {
                          Mix_PlayChannel(-1,hoverButton,0);
                          soundPlayed=1;
                      }
                        RBout2.x=(winW/3)-(winW/120);
                        RBout2.y=(winH*7)/12-(winH/200)-(winH/20);
                        RBout2.w=(winW/3)+(winW/60);
                        RBout2.h=(winH/8)+(winH/100);
                        RtextBout2.x=(winW*21)/60-(winW/120);
                        RtextBout2.y=(winH*7)/12+(winH/100)-(winH/200)-(winH/20);
                        RtextBout2.w=(winW*9)/30+(winW/60);
                        RtextBout2.h=winH/10+(winH/100);
                    }
                    else {

                      //Gonflement du bouton 3
                      if (SDL_PointInRect(&mousePos,&RBout3)) {
                        if (!soundPlayed) {
                            Mix_PlayChannel(-1,hoverButton,0);
                            soundPlayed=1;
                        }
                          RBout3.x=(winW/3)-(winW/120);
                          RBout3.y=(winH*5)/6-(winH/200)-(winH/20);
                          RBout3.w=(winW/3)+(winW/60);
                          RBout3.h=(winH/8)+(winH/100);
                          RtextBout3.x=(winW*21)/60-(winW/120);
                          RtextBout3.y=(winH*5)/6+(winH/100)-(winH/200)-(winH/20);
                          RtextBout3.w=(winW*9)/30+(winW/60);
                          RtextBout3.h=winH/10+(winH/100);
                      }
                      else {
                          soundPlayed=0;
                      }
                    }
                  }
                  break;

              //Pour quitter
              case SDL_QUIT:
                  done=1;
                  break;

              //Cas general
              default:
                  break;
          }
          //Affichage du menu
            //Couleur du fond
          if (SDL_SetRenderDrawColor(ren,0,0,0,255)<0) {
              printf("Erreur lors du changement de couleur : %s",SDL_GetError());
              return EXIT_FAILURE;
          }
          SDL_RenderClear(ren);
            //logo
          SDL_RenderCopy(ren,logoT,NULL,&Rlogo);

            //bouton1
          SDL_RenderCopy(ren,bout1,NULL,&RBout1);
          SDL_RenderCopy(ren,TtextBout1,NULL,&RtextBout1);
            //bouton2
          SDL_RenderCopy(ren,bout2,NULL,&RBout2);
          SDL_RenderCopy(ren,TtextBout2,NULL,&RtextBout2);
            //bouton3
          SDL_RenderCopy(ren,bout3,NULL,&RBout3);
          SDL_RenderCopy(ren,TtextBout3,NULL,&RtextBout3);

          //Rendu
          SDL_RenderPresent(ren);

          refreshMain(fen,&winW,&winH,&Rlogo,&RBout1,&RBout2,&RBout3,&RtextBout1,&RtextBout2,&RtextBout3);
      }
  }

    //Destruction des structures utilisees

  libereListeCarte(listeCartes);

  SDL_DestroyTexture(logoT);
  SDL_DestroyTexture(bout1);
  SDL_DestroyTexture(TtextBout1);
  SDL_DestroyTexture(bout2);
  SDL_DestroyTexture(TtextBout2);
  SDL_DestroyTexture(bout3);
  SDL_DestroyTexture(TtextBout3);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(fen);

  TTF_CloseFont(dejavu);



  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
  return 0;
}

void refreshMain(SDL_Window * fen, int * winW,int * winH,SDL_Rect * Rlogo,SDL_Rect * RBout1,SDL_Rect * RBout2,SDL_Rect * RBout3,SDL_Rect * RtextBout1,SDL_Rect * RtextBout2,SDL_Rect * RtextBout3) {
    //Rafraichissement des textures en cas de modifications de taille de la fenêtre
    SDL_GetWindowSize(fen,winW,winH);
    //Logo
    Rlogo->x=(*winW*5)/12;
    Rlogo->y=*winH/30;
    Rlogo->w=*winW/6;
    Rlogo->h=*winH/6;
    //Boutons
    RBout1->x=RBout2->x=RBout3->x=*winW/3;
    RBout1->w=RBout2->w=RBout3->w=*winW/3;
    RBout1->h=RBout2->h=RBout3->h=*winH/8;
    RBout1->y=*winH/3-(*winH/20);
    RBout2->y=(*winH*7)/12-(*winH/20);
    RBout3->y=(*winH*5)/6-(*winH/20);
    //textes
    RtextBout1->x=RtextBout2->x=RtextBout3->x=(*winW*21)/60;
    RtextBout1->w=RtextBout2->w=RtextBout3->w=(*winW*9)/30;
    RtextBout1->h=RtextBout2->h=RtextBout3->h=*winH/10;
    RtextBout1->y=*winH/3+(*winH/100)-(*winH/20);
    RtextBout2->y=(*winH*7)/12+(*winH/100)-(*winH/20);
    RtextBout3->y=(*winH*5)/6+(*winH/100)-(*winH/20);
}
