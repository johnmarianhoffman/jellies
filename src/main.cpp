#include <iostream>
#include <vector>
#include <random>
#include <cstring>
#include <chrono>
#include <thread>
#include <ctime>

#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>
#include <jellies.h>

const int SCREEN_HEIGHT = 2*24;
const int SCREEN_WIDTH  = 2*24;

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
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override
  {
    // called once per frame
    for (int x = 0; x < ScreenWidth(); x++)
      for (int y = 0; y < ScreenHeight(); y++)
        Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
    return true;
  }
};

int main(int argc, char** argv){

  JellyfishSimulation demo;
  if (demo.Construct(1440, 900, 1, 1))
    demo.Start();

  return 0;  
}

//uint32_t blend_colors(float weight, uint32_t color_1, uint32_t color_2){
//
//  uint8_t r1 = (color_1 & 0x00FF0000) >> 16;
//  uint8_t g1 = (color_1 & 0x0000FF00) >> 8;
//  uint8_t b1 = (color_1 & 0x000000FF) >> 0;
//
//  uint8_t r2 = (color_2 & 0x00FF0000) >> 16;
//  uint8_t g2 = (color_2 & 0x0000FF00) >> 8;
//  uint8_t b2 = (color_2 & 0x000000FF) >> 0;
//
//  //std::cout << (int)r1 << " " << (int)g1 << " " << (int)b1 << std::endl;
//  //std::cout << (int)r2 << " " << (int)g2 << " " << (int)b2 << std::endl;
//  //
//  //std::cout << color_1 << std::endl;
//  //std::cout << color_2 << std::endl;
//
//  uint8_t r_blend = (1.0f - weight)*(float)r1 + (weight)*(float)r2;
//  uint8_t g_blend = (1.0f - weight)*(float)g1 + (weight)*(float)g2;
//  uint8_t b_blend = (1.0f - weight)*(float)b1 + (weight)*(float)b2;
//  
//  uint32_t final_color = 0x00000000;
//
//  final_color = (((uint32_t)r_blend) << 16) + (((uint32_t)g_blend) << 8) + (((uint32_t)b_blend) << 0);
//
//  //std::cout << final_color << std::endl;
//  
//  return final_color;
//}
//
//uint32_t adjust_brightness_colors(float brightness, uint32_t color){
//  uint8_t r = (color & 0x00FF0000) >> 16;
//  uint8_t g = (color & 0x0000FF00) >> 8;
//  uint8_t b = (color & 0x000000FF) >> 0;
//
//  r =  (uint8_t)(brightness*(float)r);
//  g =  (uint8_t)(brightness*(float)g);
//  b =  (uint8_t)(brightness*(float)b);
//
//  uint32_t final_color = 0x00000000;
//  final_color = (((uint32_t)r) << 16) + (((uint32_t)g) << 8) + (((uint32_t)b) << 0);
//  return final_color;
//}
//
//void render(std::vector<jelly> &jellies,std::vector<uint32_t>& render_array){
//  
//  float dx = tank_size(0) / (float)SCREEN_WIDTH;
//  float dy = tank_size(1) / (float)SCREEN_HEIGHT;
//
//  // "Blank the screen"
//  //for (int i=0;i<SCREEN_HEIGHT;i++){
//  //  for (int j=0;j<SCREEN_WIDTH;j++){
//  //    int pixel_idx = i*SCREEN_WIDTH + j;
//  //    render_array[pixel_idx] = background_color;
//  //  }
//  //}
//
//  auto comp_jelly = [](jelly &a,jelly &b)->bool{return a.z() > b.z();};
//  std::sort(jellies.begin(),jellies.end(),comp_jelly);
//  
//  for (int i=0;i<SCREEN_HEIGHT;i++){
//    for (int j=0;j<SCREEN_WIDTH;j++){
//      
//      // Get the absolute x position of the pixel
//      float x_pos = dx * (float)j;
//      float y_pos = dy * (float)i;
//  
//      int pixel_idx = i*SCREEN_WIDTH + j;
//
//      int n_subsample_intersections = 0;
//      float max_brightness = 0.5f;
//      for (int subsample_col_idx=0;subsample_col_idx<subsampling_factor;subsample_col_idx++ ){
//        for (int subsample_row_idx=0;subsample_row_idx<subsampling_factor;subsample_row_idx++ ){
//
//          // Compute the xy location of the subsample
//          float x_offset = dx*(subsample_col_idx - ((float)subsampling_factor-1.0f)/2.0f);
//          float y_offset = dy*(subsample_row_idx - ((float)subsampling_factor-1.0f)/2.0f);
//
//          x_pos += x_offset;
//          y_pos += y_offset;
//          
//          for (size_t ii=0;ii<jellies.size();ii++){
//            jelly curr_jelly = jellies[ii];
//        
//            float del_x = curr_jelly.x()-x_pos;
//            float del_y = curr_jelly.y()-y_pos;
//
//            float scaling_factor = 0.9f;
//            float scale = scaling_factor*(tank_size(2)-curr_jelly.z())/tank_size(2)+(1.0f-scaling_factor);
//            
//            if (del_x*del_x + del_y*del_y < (scale*curr_jelly.size()*scale*curr_jelly.size())){
//              n_subsample_intersections++;
//
//              // Brightness (computed based on z distance from the camera);
//              // Note this approach only works if the jellies array is sorted
//              float normed_z_dist = 1.0f-curr_jelly.z()/tank_size(2);
//              float min_b = 0.7;
//              float max_b = 1.0;
//              max_brightness = normed_z_dist*(max_b - min_b) + min_b;
//            }
//
//          }
//        }
//      }
//
//      // Compute the input colors
//      uint32_t curr_jelly_color = adjust_brightness_colors(max_brightness,jelly_color);
//      uint32_t curr_background_color = blend_colors((float)i/(float)SCREEN_HEIGHT,background_color_top,background_color_bottom);
//
//      // Using the number of subsample intersections to compute a
//      // blend weight between background and foreground
//      n_subsample_intersections = std::min(n_subsample_intersections,subsampling_factor*subsampling_factor);
//
//      float blend_weight = (float)n_subsample_intersections/((float)subsampling_factor*(float)subsampling_factor);
//      render_array[pixel_idx] = blend_colors(blend_weight,curr_background_color,curr_jelly_color);
//
//    }
//  }
//}
//
//int main(int argc, char ** argv){
//
//  srand(time(NULL));
//
//  // Init visualization
//  imshow_initialize(SCREEN_WIDTH,SCREEN_HEIGHT,"color");
//  std::vector<uint32_t> array(SCREEN_HEIGHT * SCREEN_WIDTH);
//
//  // Init jellyfish
//  std::vector<jelly> jellies(20);
//  for (auto &j: jellies){
//    std::cout << j.x() << " " << j.y() << std::endl;
//  }
//  
//  int count = 0;
//  while (true){
//
//    if (imshow_checkforquit())
//      break;
//    
//    // Render
//    render(jellies,array);
//    imshow_update(&array[0]);
//    imshow_delay(1);
//
//    //std::this_thread::sleep_for(std::chrono::milliseconds(33));
//    
//    // Update
//    for (auto &j: jellies){
//      j.update();
//    }
//  
//    count++;
//  }
//  
//  imshow_destroy();
//  
//  return 0;
//}
