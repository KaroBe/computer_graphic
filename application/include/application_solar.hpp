#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  
  // update uniform locations and values
  void uploadUniforms();
  // update projection matrix
  void updateProjection();
  // react to key input
  void keyCallback(int key, int scancode, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);

  //draws the given planet
  void upload_planet_transforms(planet p) const;
  //overloaded function to draw the satellits(moons)
  void upload_planet_transforms(satellite p) const;

  // calls upload_planet_transforms funct. for all planets/satellites
  void render() const;

 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  void updateView();

  // cpu representation of model
  model_object planet_object;

  //stores the planets/satellites to render
  std::vector<planet> all_planets;
  std::vector<satellite> all_satellites;
};

#endif