const std::string jellyfish_anim_base64 = "iVBORw0KGgoAAAANSUhEUgAAAgAAAAAgCAYAAABkS8DlAAAFm0lEQVR4nO2cQXLbMAxFcZMcIWfJgbrJFbrLgTSTTW/hXS/QRcZdtHRhGCTlBB9grf9mOLVZjT8kK/qfkBIRQgghhBBCCCGEEEIIIYQQQgghhBByZM7OqNTOroEQQggpocr8ziJyfnv5M35++zfAdVw+v2lbfVMDvI7BIIQQQsKpMmCRjvn++vH9MlQdMN23Fzmftu1q320NgDpuwocXQDKD0GAQQgh5MKoM+EZbG/Bp2y76bT64hqHxevqBNbjGP9v/4CCwq/sB7oLsCR4MIoSQQ5B9was04Cv9nvE4JhSufdq27Bq6XYekGqbdj14ACdB3g8edxz88BN0xCCEklLJ74IUGfKU/M8LTtsE7APo4ePUE1jA1wIn+lwx4ZPzgENI95l4AsZ2ooCDCAEIIWYYlWvBFAeCqhpEBo7SlYwZ6CO4CPNVuxyRYf2p+vWDwxRB0Ez72nntebZ+ogwGkU8sdgxASxDIt+IIVuIiIPD+NTVjAF57nJzl/vL8OL3pqG4j+SLsNgP6u8AMIQbt0WzgIroMBhAGEkGU4+gr8YsA9E/54fz3/NUmItohI029D16TnEPpWJ1O/1SA7Lrwg/QoDYABhALmp5Y5BHpSKL708AKywAhe5NmFrgCjz0/qtBqvtbYfQrtSvDiCF+gwgDCAMIAfn6mSYfOnh2iu04KtW4E2/va4MAIWrb2n6I0NGamvNiv2vCh+6hs/MBcAAwgBSGUBuahoMOBXiwy+g4nfhK1rwIuuswD0jzA4AvXrQ+r2OAFK3p5UZfir0dQ3VAYQdEAaQxAByqUWc7yE5eNwWkZl67MlgvwDv4AfXsEwLXqR2Bd7Tt9tk6XvvM/SfnyRN336ufdix6vZLhv6e2y/o828UQPR5gNRnADlUAHHrmPkfajE6bYGA/xTrXa0X1Ep81Ra89z5DX89VBIA2Vx1AKm+B6PfZK2DPlDINsM3peYQRe9+7Pf/0v2h9q+md/wh9b197gQRwHhwtgIin2/M6z3+jfPCuBIL6c6x777/rmoK0L6zagq8yQD1XYYBH0296ngHp1XjmKljXgNbXWlrfHvdoE9RBo2e6+nWWvv4/G4jQ+lYTqW9r8K59vbqi9KUmgIhMut/AzoNfwL0tkIgCZnV4ByVQ98JqK0A7Z2tE6esaVjJgW2OWflYAW6GGkQGjQoDe597Pm3fhRxuQ1UWvwj0D9Ey3Sh91/EcBpP2/rQmtb889cABxOxE9/5PgEPLp+yCIPwkrTkci6z78KgbY9PRcZQfCqydTP7sD0usAZehr0806//RnesdA12YNKboGLwD0jAmh7608bV0ofXvOtXm9/3oOrT87/oga9gYQxM/CKOh4xwJ0LZh2HhC603sRGatwdZJ3B9IEVzGgpq/njhRAeh0AvQJF1lChPwpfeh6pb82314FA1iByGz6y8I6Bna/Wzz4HZvOR2iJ+AFspgNhQGKnfPtP+DHqhOFpXZOcDERLcftBU7nzTb689A7DboPVXWYHrOaT+iGwzIKQKa7TZgchqe/MIvKAz0kcHEF0TOoDMAlCKB+5ZgaOSsD6gPfNv20Vr6xra6xVW4DYAZOo3zVUCACEkl+rQvUoHaLR9lO5I3+s+hNNLGl4aQem31z3zs9tV1FAVQNpctgFbPQYAQgiJxfOeNp/hv93VnxcIKvR726FqWKEF780xABBCyGPRe9YoZfXfhDyzWaEFfpRbALOavPfZVOsTQsijMXrWQAT/q7g3H1htwK2GtAQk4w6ArgdZw6gm7z0hhJDHQIcAb/Gpr//wAOA9lYkOAKMHATMDQKuhOgBYqvUJIYRg0Q8G2vmUZwFsyyHlQYSOflYAGdXTe08IIYRE4Bl8qudYA658Et97CBCtP6onW5sQQsixSfOc2UN/WV0AW9OeuSwYAAghhPwP/AamQK4c0st/VwAAAABJRU5ErkJgggAA";

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
    velocity += (.05f*tmp_vel.normalized()) * fElapsedTime;

    // Attempt to align with the velocity field
    float correction_magnitude = orientation.dot(tmp_vel);
    Eigen::Vector3f orientation_offset = Eigen::Vector3f::Random();
    orientation_offset *= (1.0f/orientation_offset.norm());
    int count = 0;
    while ((orientation + orientation_offset).dot(tmp_vel) > correction_magnitude && count < 100){
      orientation_offset = Eigen::Vector3f::Random();
      orientation_offset *= (1.0f/orientation_offset.norm());
    }
    
    if (count<100){
      orientation += orientation_offset*fElapsedTime;
      orientation *= 1.0f/orientation.norm();
    }

    position+=velocity * fElapsedTime;

    for (int i=0;i<3;i++){
      if (position(i) > tank_size(i) ||
          position(i) < 0){
        velocity(i) = - velocity(i);
        //position-=velocity * fElapsedTime;
      }
    }

    // Update the animation frame
    animation_timer += fElapsedTime;
    if (animation_timer > frame_time){
      current_frame = (current_frame+1)%16;
      animation_timer = 0.0f;
    }    
  }

  float size(){return radius;}
  float x(){return position(0);}
  float y(){return position(1);}
  float z(){return position(2);}
  float currentFrame(){return current_frame;}
  Eigen::Vector3f getPosition(){return position;}

  float angle(){
    // Hacky, but the only way it actually works with OLC PGE
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

  float animation_timer = frame_time * float(rand())/float(RAND_MAX);
  int current_frame = rand()%16;
  
  int offset = rand()%1000;
};
