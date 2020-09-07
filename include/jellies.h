#include <Eigen/Dense>

const Eigen::Vector3f tank_size(5.0f,5.0f,1.0f);
const uint32_t jelly_color = 0x00fca123;

const uint32_t background_color = 0x000000FF;

const uint32_t background_color_bottom = 0x001323a1;
const uint32_t background_color_top    = 0x0012b8ff;

class jelly{
public:
  jelly(){
    for (int i=0; i<3; i++){
      float max_val = (i==2) ? tank_size(2) : tank_size(1);
      position[i] = max_val*(float)rand()/(float)RAND_MAX;
      velocity[i] = 0.01f * (1.0f - 2.0f * (float)rand()/(float)RAND_MAX);
    }

    orientation = Eigen::Vector3f::Random().normalized();
  }
  ~jelly(){}

  void update(float fElapsedTime){

    // Apply h2o friction
    velocity -= (0.15f*velocity.dot(velocity)) * fElapsedTime * velocity.normalized();
    
    // Apply velocity field (offset to match vector field equations)
    Eigen::Vector3f tmp_pos = position + (-tank_size(0)/2.0f)*Eigen::Vector3f::Ones();
    Eigen::Vector3f tmp_vel;
    tmp_vel(0) = sin(tmp_pos(0))*tmp_pos(1);
    tmp_vel(1) = -abs(tmp_pos(0)) + tank_size(0)/4;
    tmp_vel(2) = 0;
    velocity += (0.00015f*tmp_vel) * fElapsedTime;

    // Attempt to align with the velocity field

    //float correction_magnitude = orientation.dot(tmp_vel);
    //Eigen::Vector3f orientation_offset = Eigen::Vector3f::Random();
    //orientation_offset *= (1.0f/orientation_offset.norm());
    //int count = 0;
    //while ((orientation + orientation_offset).dot(tmp_vel) > correction_magnitude && count < 100){
    //  orientation_offset = Eigen::Vector3f::Random();
    //  orientation_offset *= (1.0f/orientation_offset.norm());
    //}
    //
    //if (count<100){
    //  orientation += orientation_offset*fElapsedTime;
    //  orientation *= 1.0f/orientation.norm();
    //}

    orientation = tmp_vel.normalized();
    
    // Seems like jellyfish pulse every 3 seconds
    if (rand()%75==0){
      Eigen::Vector3f bump = 0.005*orientation;
      velocity += bump;
    }
    
    //if (velocity.norm() < 0.001){
    //  Eigen::Vector3f bump = 0.01f*orientation;
    //  velocity += bump;
    //}
      
    position+=velocity * fElapsedTime;

    for (int i=0;i<3;i++){
      if (position(i) > tank_size(i) ||
          position(i) < 0){
        velocity(i) = - velocity(i);
        //position-=velocity * fElapsedTime;
      }
    }
  }

  float size(){
    return radius;
  }

  float x(){
    return position(0);
  }
  
  float y(){
    return position(1);
  }
  
  float z(){
    return position(2);
  }

  float angle(){
    // Hacky, but the only way it actuall works with OLC PGE
    Eigen::Vector2f o = Eigen::Vector2f(orientation(0),orientation(1)).normalized();
    Eigen::Vector2f ref = Eigen::Vector2f(0.0f,1.0f);
    float a = acos(o.dot(ref));
    if (o(0) < 0)
      return a;
    else    
      return -a;
  }
  
private:
  Eigen::Vector3f position;
  Eigen::Vector3f velocity;
  Eigen::Vector3f orientation; // Direction of jellyfish propulsion vector.  I.e. when jellyfish pulses, adds velocity in this direction.
  float radius = 0.5f;

  float animation_timer = 0.0;
  float current_frame = rand()%16;
  
  //int offset = 1000*(float)rand()/(float)RAND_MAX;
  int offset = rand()%1000;
};
