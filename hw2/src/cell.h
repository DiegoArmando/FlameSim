#ifndef _CELL_H_
#define _CELL_H_

#include <cassert>
#include <vector>
#include <time.h>

// ==============================================================================

class FluidParticle {

public:
  // accessor
  const glm::vec3& getPosition() const { return position; }
  // modifer
  void setPosition(const glm::vec3 &p) { position = p; }

  FluidParticle(glm::vec3 sentOrign, glm::vec3 initVel)
  {
    srand(time(NULL));
    origin = sentOrign;
    velocity = initVel;
    initialV = initVel;
    fuel = 5000;
    heat = 5;
  }
   FluidParticle(glm::vec3 sentOrign)
  {
    srand(time(NULL));
    origin = sentOrign;
    fuel = 3000 + rand() % 1500;
    heat = 5;
  }

  FluidParticle()
  {
    fuel = 5000;
    heat = 5;
  }

  void update(glm::vec3 netForce)
  {
  //  velocity += netForce;
  //  position += velocity;

    fuel -= heat;
    if(fuel <= 0)
    {
      reset();
    }
  }
private:
  // representation
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 initialV;

  int heat;
  int fuel;

  glm::vec3 origin;

  //TODO: Add noise to initial velocity, allow for random inital velocity, add noise to inital position based on origin
  void reset()
  {
    //srand(time(NULL));
    position = origin;
    fuel = 3000 + rand() % 1500;
  }
};

// ==============================================================================
enum CELL_STATUS { CELL_EMPTY, CELL_SURFACE, CELL_FULL };

class Cell {

public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Cell() { 
    pressure = 0;
    status = CELL_SURFACE; 
    u_plus = 0;
    v_plus = 0;
    w_plus = 0;
    new_u_plus = 0;
    new_v_plus = 0;
    new_w_plus = 0;
  }
  ~Cell() { 
    for (unsigned int i = 0; i < particles.size(); i++) {
      delete particles[i]; }
  }
  
  // =========
  // ACCESSORS
  double getPressure() const { return pressure; }
  enum CELL_STATUS getStatus() const { return status; }
  double get_u_plus() const { return u_plus; }
  double get_v_plus() const { return v_plus; }
  double get_w_plus() const { return w_plus; }
  double get_new_u_plus() const { return new_u_plus; }
  double get_new_v_plus() const { return new_v_plus; }
  double get_new_w_plus() const { return new_w_plus; }
  int numParticles() const { return particles.size(); }
  std::vector<FluidParticle*>& getParticles() { return particles; }

  // =========
  // MODIFIERS
  void setPressure(double p) { pressure = p; }
  void setStatus(enum CELL_STATUS s) { status = s; }
  void set_u_plus(double f) { u_plus = new_u_plus = f; }
  void set_v_plus(double f) { v_plus = new_v_plus = f; }
  void set_w_plus(double f) { w_plus = new_w_plus = f; }
  void set_new_u_plus(double f) { new_u_plus = f; }
  void set_new_v_plus(double f) { new_v_plus = f; }
  void set_new_w_plus(double f) { new_w_plus = f; }
  void adjust_new_u_plus(double f) { new_u_plus += f; }
  void adjust_new_v_plus(double f) { new_v_plus += f; }
  void adjust_new_w_plus(double f) { new_w_plus += f; }
  void copyVelocity() { 
    u_plus = new_u_plus; new_u_plus = 0;
    v_plus = new_v_plus; new_v_plus = 0;
    w_plus = new_w_plus; new_w_plus = 0; 
  }
  void addParticle(FluidParticle *p) {
    assert(p != NULL);
    particles.push_back(p); 
  }
  void removeParticle(FluidParticle *p) {
    assert(p != NULL);
    for (std::vector<FluidParticle*>::iterator i = particles.begin(); i != particles.end(); i++) {
      if (*i == p) {
        particles.erase(i);
        return;
      }
    }
    assert (0); 
  }

private:

  // ==============
  // REPRESENTATION
  double pressure;
  enum CELL_STATUS status;

  // velocities at the center of each face (flowing in the positive direction)
  double u_plus,v_plus,w_plus;
  double new_u_plus,new_v_plus,new_w_plus;

  std::vector<FluidParticle*> particles;
};

// ==============================================================================

#endif
