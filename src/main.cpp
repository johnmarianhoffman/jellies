#include <iostream>
#include <vector>
#include <random>
#include <cstring>
#include <chrono>
#include <thread>
#include <ctime>
#include <memory>

#include <Eigen/Dense>

const int SCREEN_HEIGHT = 128;
const int SCREEN_WIDTH  = 128;
const Eigen::Vector3f tank_size(5.0f,5.0f,1.0f);
const float animation_fps = 10;
const float frame_time = 1.0f/float(animation_fps);//ms

#include <jellies.h>

#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>

std::vector<jelly> jellies(35);

class JellyfishSimulation : public olc::PixelGameEngine{

 public:
  JellyfishSimulation()
    {
      sAppName = "JellyfishSimulation";
    }

 public:
  bool OnUserCreate() override
  {

    // Load sprite and create decal
    m_jelly_sprite = std::make_unique<olc::Sprite>("/Users/johnhoffman/Code/jellies/assets/jelly_anim.png");
    m_jelly_decal  = std::make_unique<olc::Decal>(m_jelly_sprite.get());

    // Render background
    std::vector<int> background_color_1 = {15,39,183};
    std::vector<int> background_color_2 = {10,173,255};

    auto dist = [](float x1, float y1, float x2, float y2) -> float{
                  return sqrt( (x1 - x2)*(x1 - x2)  +  (y1 - y2)*(y1 - y2));
                };
    
    float center_point[2] = {float(ScreenWidth())/2.0f,0.0f};
    float max_distance = dist(center_point[0],center_point[1],0.0f,float(ScreenHeight()));
                         
    for (int x = 0; x < ScreenWidth(); x++){
      for (int y = 0; y < ScreenHeight(); y++){

        // N color radial gradient
        int n_colors = 24;
        float blend_weight = round(float(n_colors)*dist(center_point[0],center_point[1],float(x),float(y))/max_distance)/float(n_colors);

        Draw(x, y, olc::Pixel(blend_weight*background_color_1[0] + (1.0f-blend_weight)*background_color_2[0],
                              blend_weight*background_color_1[1] + (1.0f-blend_weight)*background_color_2[1],
                              blend_weight*background_color_1[2] + (1.0f-blend_weight)*background_color_2[2]));
      
      }
    }

    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override
  {
    
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

      // "Lighting"
      Eigen::Vector3f light_position(tank_size(0)/2.0f,0.0f,tank_size(2)/2.0f);
      Eigen::Vector3f bottom_corner_of_tank(0.0f,tank_size(1),0.0f);
      float max_distance = (light_position - bottom_corner_of_tank).norm();
      float curr_distance = (light_position - j.getPosition()).norm();
      float lighting_factor = 0.6f;
      float lighting = lighting_factor * (max_distance - curr_distance) / max_distance + (1.0f - lighting_factor);
      
      DrawPartialRotatedDecal(olc::vf2d(pix_x,pix_y),m_jelly_decal.get(),j.angle(),
                              {float(32.0f)/2.0f,float(32.0f)/2.0f},
                              {j.currentFrame()*32,0},
                              {32,32},
                              {scale,scale},
                              olc::Pixel(lighting*255,lighting*255,lighting*255));      
    }

    return true;
  }

private:
  std::unique_ptr<olc::Sprite> m_jelly_sprite;
  std::unique_ptr<olc::Decal> m_jelly_decal;
};

int main(int argc, char** argv){

  JellyfishSimulation demo;
  if (demo.Construct(SCREEN_WIDTH,SCREEN_HEIGHT, 5, 5, false, true))
    demo.Start();

  return 0;  
}
