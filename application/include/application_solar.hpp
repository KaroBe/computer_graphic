#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"


#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;
//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

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
  void upload_planet_transforms(planet const& p, int k) const;
  //overloaded function to draw the satellits(moons)
  void upload_planet_transforms(satellite const& p) const;
  void upload_stars() const;
  void upload_particles() const;
  void upload_starsphere() const;
  void upload_orbits(planet const& p) const;
  void upload_orbits(satellite const& p) const;
  void upload_quad() const;

  // calls upload_planet_transforms funct. for all planets/satellites
  void render() const;
  void SortParticles();
  void FindUnusedParticle();

 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  void initializeStars();
  void initializeOrbits();
  void initializeTextures();
  void initializeFramebuffer();
  void initializeScreenQuad();
  void initializeParticles();


  void updateView();

  void fillOrbits();
  void fillStars();
  void fillPlanets();
  void fillParticles();
  

  // cpu representation of model
  model_object planet_object;
  model_object star_object;
  model_object orbit_object;

  std::vector<texture_object> planet_texture;
  std::vector<texture_object> normal_texture;

  texture_object frame_buffer_texture;
  texture_object frame_buffer_object;
  texture_object render_buffer_object;

  model_object screen_quad_object;

  //stores the planets/satellites/stars to render
  std::vector<planet> all_planets;
  std::vector<satellite> all_satellites;
  std::vector<float> all_stars;
  std::vector<float> all_orbits;
  std::vector<float> all_quads;

  GLfloat m_shading_method = GLfloat(0.0f);
  bool m_h_mirrored = bool(false);
  bool m_v_mirrored = bool(false);
  bool m_grayscale = bool(false);
  bool m_blur = bool(false);
  bool m_sobel = bool(true);

  static const int MaxParticles = 10000;
  particle ParticlesContainer[MaxParticles];
  int ParticlesCount = 0;
  int LastUsedParticle = 0;

};

#endif