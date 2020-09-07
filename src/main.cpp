#include <iostream>
#include <vector>
#include <random>
#include <cstring>
#include <chrono>
#include <thread>
#include <ctime>
#include <memory>

#include <jellies.h>

#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>


const int SCREEN_HEIGHT = 128;
const int SCREEN_WIDTH  = 128;

std::vector<jelly> jellies(20);

const int subsampling_factor = 2;

class JellyfishSimulation : public olc::PixelGameEngine{

 public:
  JellyfishSimulation()
    {
      sAppName = "JellyfishSimulation";
    }

 public:
  bool OnUserCreate() override
  {
    // Called once at the start, so create things here

    // Load sprite and create decal
    m_jelly_sprite = std::make_unique<olc::Sprite>("/Users/johnhoffman/Code/jellies/assets/jelly.png");
    m_jelly_decal  = std::make_unique<olc::Decal>(m_jelly_sprite.get());

    
    // Render background
    std::vector<int> background_color_1 = {13,23,161};
    std::vector<int> background_color_2 = {12,184,255};
    
    for (int x = 0; x < ScreenWidth(); x++){
      for (int y = 0; y < ScreenHeight(); y++){
        float blend_weight = (float)y/(float)(ScreenHeight());
        Draw(x, y, olc::Pixel(blend_weight*background_color_1[0] + (1.0f-blend_weight)*background_color_2[0],
                              blend_weight*background_color_1[1] + (1.0f-blend_weight)*background_color_2[1],
                              blend_weight*background_color_1[2] + (1.0f-blend_weight)*background_color_2[2]));
      }
    }

    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override
  {
    // Called once per frame

    // Update the simulation
    float dx = tank_size(0) / (float)SCREEN_WIDTH;
    float dy = tank_size(1) / (float)SCREEN_HEIGHT;

    auto comp_jelly = [](jelly &a,jelly &b)->bool{return a.z() > b.z();};
    std::sort(jellies.begin(),jellies.end(),comp_jelly);

    for (auto &j: jellies)
      j.update(fElapsedTime);

    // Draw jellies
    for (auto &j: jellies){

      // Compute the xy pixel coordinates of the decal
      float pix_x = j.x()/dx;
      float pix_y = j.y()/dy;
      float scaling_factor = 0.5f;
      float scale = scaling_factor*(tank_size(2)-j.z())/tank_size(2)+(1.0f-scaling_factor);
      
      DrawRotatedDecal(olc::vf2d(pix_x,pix_y),m_jelly_decal.get(),j.angle(),
                       {float(m_jelly_sprite->width)/2.0f,float(m_jelly_sprite->height)/2.0f},
                       {scale,scale},
                       olc::Pixel(scale*255,scale*255,scale*255));
    }

    return true;
  }

private:
  std::unique_ptr<olc::Sprite> m_jelly_sprite;
  std::unique_ptr<olc::Decal> m_jelly_decal;
};

int main(int argc, char** argv){

  JellyfishSimulation demo;
  if (demo.Construct(SCREEN_WIDTH,SCREEN_HEIGHT, 5, 5))
    demo.Start();

  return 0;  
}
