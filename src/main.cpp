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

std::vector<jelly> jellies(35);

const int subsampling_factor = 2;

const int GLOBAL_FPS = 60;
const float GLOBAL_FRAME_TIME = 1.0f/float(GLOBAL_FPS);
float GLOBAL_FRAME_TIMER = 0.0f;

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
    //m_jelly_sprite = std::make_unique<olc::Sprite>("/Users/johnhoffman/Code/jellies/assets/jelly_anim.png");
    //m_jelly_sprite = std::make_unique<olc::Sprite>("/Users/johnhoffman/Code/jellies/assets/jelly.png");
    m_jelly_sprite = std::make_unique<olc::Sprite>("/home/john/Code/jellies/assets/jelly_anim.png");
    m_jelly_decal  = std::make_unique<olc::Decal>(m_jelly_sprite.get());

    // Render background
    //std::vector<int> background_color_1 = {13,23,161};
    //std::vector<int> background_color_2 = {12,184,255};

    std::vector<int> background_color_1 = {15,39,183};
    std::vector<int> background_color_2 = {10,173,255};
    

    auto dist = [](float x1, float y1, float x2, float y2) -> float{
                  return sqrt( (x1 - x2)*(x1 - x2)  +  (y1 - y2)*(y1 - y2));
                };
    
    float center_point[2] = {float(ScreenWidth())/2.0f,0.0f};
    float max_distance = dist(center_point[0],center_point[1],0.0f,float(ScreenHeight()));
                         
    for (int x = 0; x < ScreenWidth(); x++){
      for (int y = 0; y < ScreenHeight(); y++){
        // Smooth gradient
        //float blend_weight = (float)y/(float)(ScreenHeight());
        
        // 16 color gradient
        //float blend_weight = round(16.0f*(float)y/(float)(ScreenHeight()))/16.0f;

        // Smooth radial gradient
        //float blend_weight = dist(center_point[0],center_point[1],float(x),float(y))/max_distance;

        // N color radial gradient
        int n_colors = 24;
        float blend_weight = round(float(n_colors)*dist(center_point[0],center_point[1],float(x),float(y))/max_distance)/float(n_colors);

        Draw(x, y, olc::Pixel(blend_weight*background_color_1[0] + (1.0f-blend_weight)*background_color_2[0],
                              blend_weight*background_color_1[1] + (1.0f-blend_weight)*background_color_2[1],
                              blend_weight*background_color_1[2] + (1.0f-blend_weight)*background_color_2[2]));

        // Randomly dithered between two colors (non-radial)
        //float blend_weight = (float)y/(float)(ScreenHeight());
        //float blend_buffer = float(ScreenHeight())/4.0f;
        //
        //float blend_weight;
        //if (y < blend_buffer)
        //  blend_weight = 0.0f;
        //else if (y > 3*blend_buffer)
        //  blend_weight = 1.0f;
        //else
        //  blend_weight = (float)(y - blend_buffer) / (float)(ScreenHeight() - 2.0f*blend_buffer);
        //
        //// Random dithering based on blend weight
        //if ((float)rand()/(float)RAND_MAX < blend_weight)
        //  Draw(x, y, olc::Pixel(background_color_1[0], background_color_1[1], background_color_1[2]));
        //else
        //  Draw(x, y, olc::Pixel(background_color_2[0], background_color_2[1], background_color_2[2]));

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
      Eigen::Vector3f light_position(tank_size(0)/2.0f,tank_size(1),tank_size(2)/2.0f);
      Eigen::Vector3f bottom_corner_of_tank(0.0f,0.0f,0.0f);
      float max_distance = (light_position - bottom_corner_of_tank).norm();
      float curr_distance = (light_position - j.getPosition()).norm();
      float lighting_factor = 0.5f;
      float lighting = lighting_factor * (max_distance - curr_distance) / max_distance + (1.0f - lighting_factor);

      //DrawRotatedDecal(olc::vf2d(pix_x,pix_y),m_jelly_decal.get(),j.angle(),
      //                 {float(m_jelly_sprite->width)/2.0f,float(m_jelly_sprite->height)/2.0f},
      //                 {scale,scale},
      //                 olc::Pixel(scale*255,scale*255,scale*255));

      //std::cout << j.currentFrame() << std::endl;
      //DrawPartialDecal(olc::vf2d(pix_x,pix_y),m_jelly_decal.get(),{j.currentFrame()*32.0f,0.0f},{32.0f,32.0f});
      
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
