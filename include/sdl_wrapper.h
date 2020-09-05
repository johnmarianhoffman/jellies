#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <iostream>
#include <string.h>
#include <SDL2/SDL.h>

int screen_width;
int screen_height;
char imshow_type[256];
SDL_Window * window;
SDL_Texture * tex;
SDL_Renderer * renderer;

void imshow_log_error(std::ostream &os, const std::string &msg){
  os << msg << " error: " << SDL_GetError() << std::endl;
}

void imshow_delay(int ticks){
  for (int i=0;i<ticks;i++){
    SDL_PumpEvents();
    SDL_Delay(1);
  }
}

bool imshow_checkforquit(){
  SDL_Event e;
  bool ret_val = false;
  while (SDL_PollEvent(&e)){
    if (e.type ==SDL_QUIT)
      ret_val = true;    
  }
  return ret_val;
}

void imshow_initialize(int w, int h,const char * type){

  screen_width = w;
  screen_height = h;
  strncpy(imshow_type,type,256);

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
    imshow_log_error(std::cout, "SDL_Init");
  }

  window = SDL_CreateWindow("My Window", 100, 100, w,h, SDL_WINDOW_SHOWN);

  SDL_SetWindowResizable(window,SDL_TRUE);
  if (window == nullptr){
    imshow_log_error(std::cout, "CreateWindow");
    SDL_Quit();
  }

  //| SDL_RENDERER_PRESENTVSYNC
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
  if (renderer == nullptr){
    imshow_log_error(std::cout, "CreateRenderer");
    //cleanup(window);
    SDL_Quit();
  }

  strncpy(imshow_type,type,256);
  tex  = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,w,h);    
}

void imshow_update(void * array){
    
  uint8_t * base;        
  void * pixels;
  int pitch;

  if (SDL_LockTexture(tex,NULL,&pixels,&pitch) < 0){
    imshow_log_error(std::cout,"Couldn't lock texture");
    exit(1);
  }

  if (!strcmp(imshow_type,"bool")){
    for (int i=0;i<screen_width;i++){
      for (int j=0;j<screen_height;j++){
        int idx = i*screen_height+j;
        bool * tmp = (bool * )array;
        base = ((uint8_t *)pixels) + 4*idx;
        if (tmp[idx]==true){
          base[0] = 255;
          base[1] = 255;
          base[2] = 255;
          base[3] = 255;
        }
        else{
          base[0] = 0;
          base[1] = 0;
          base[2] = 0;
          base[3] = 255;
        }

      }
    }
  }
  else if (!strcmp(imshow_type,"float_scaled")){
        
  }
  else if (!strcmp(imshow_type,"float")){
    float * base;
    float * tmp = (float * )array;
    for (int i=0;i<screen_width;i++){
      for (int j=0;j<screen_height;j++){
        int idx = i*screen_height+j;
        base = ((float *)pixels) + idx;
        base[0] = tmp[idx];
      }
    }
  }    
  else if (!strcmp(imshow_type,"color")){
    uint8_t * base;        
    uint8_t * tmp = (uint8_t * )array;
        
    base = ((uint8_t *)pixels);
    memcpy(base,tmp,4*screen_width*screen_height*sizeof(uint8_t));
        
    //for (int i=0;i<screen_width;i++){
    //    for (int j=0;j<screen_height;j++){
    //        int idx = i*screen_height+j;
    //        base = ((uint8_t *)pixels) + 4*idx;
    //        
    //        base[0] = tmp[idx];
    //        base[1] = tmp[idx+1];
    //        base[2] = tmp[idx+2];
    //        base[3] = tmp[idx+3];                        
    //
    //        //std::cout << (int) base[0] << std::endl;
    //        //std::cout << (int) base[1] << std::endl;
    //        //std::cout << (int) base[2] << std::endl;
    //        //std::cout << (int) base[3] << std::endl;
    //        
    //    }
    //}
  }
  else{ 
    imshow_log_error(std::cout,"Unrecognized 'type' paramter.");
    SDL_Quit();        
  }
    
  SDL_UnlockTexture(tex);    
  SDL_RenderCopy(renderer,tex,NULL,NULL);
  SDL_RenderPresent(renderer);
    
}

void imshow_destroy(){
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window); 
}

#endif
