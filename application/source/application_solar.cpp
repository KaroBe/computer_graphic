#include "application_solar.hpp"
#include "launcher.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>



ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
{
	initializeGeometry();
  initializeShaderPrograms();

  //initializes our planets and satellites
  planet sonne(7.0f, 0.0f, 0.0f);
  planet merkur(0.9f, 0.48f, 10.0f);
  planet venus(2.0f, 0.35f, 15.0f);
  planet erde(2.0f, 0.30f, 20.0f);
  satellite mond(erde, 0.5f, 0.5f, 3.0f);
  planet mars(0.9f, 0.24f, 25.0f);
  planet jupiter(5.0f, 0.13f, 33.0f);
  planet saturn(3.5f, 0.1f, 41.0f);
  planet uranus(3.7f, 0.07f, 50.0f);
  planet neptun(3.4f, 0.05f, 57.0f);
  planet pluto(0.5f, 3.6f, 62.0f); ///PLUTO IS A PLANET! 
  //We`re the reason, that plutonians keep destroying their planets.

  //...and inserts them in the corresponding containers
  all_planets.insert(std::end(all_planets), {sonne, erde, merkur, venus, mars,
    jupiter,saturn, uranus, neptun});

  all_satellites.insert(std::end(all_satellites), {mond});

  for (int i = 0; i < 599; ++i)
  {
    if (i % 6 < 3)
    {
      all_stars.insert(std::end(all_stars), -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100)));
    }else {
      all_stars.insert(std::end(all_stars), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/255)));// wert zwisdhen 0 und 1
    }
    
  }
}


//calculates the matrices of the satellites according to the fathers matrices and draws the satellites
void ApplicationSolar::upload_planet_transforms(satellite p) const{all_stars.in
	 // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  //calcs the matrices of the father planets
	glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()*p.m_father.m_rot_speed), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -p.m_father.m_dis_to_origin});

  //Calcs local matrices of the moons with those
  //of the father planets
  model_matrix = glm::rotate(model_matrix, float(glfwGetTime()*p.m_rot_speed), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -p.m_dis_to_father});
  model_matrix = glm::scale(model_matrix, glm::fvec3{p.m_size, p.m_size, p.m_size});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));


  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);// GL draw array für Sterne
}


void ApplicationSolar::upload_planet_transforms(planet p) const{
	 // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  //calcs the matrices of the planets
	glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()*p.m_rot_speed), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -p.m_dis_to_origin});
  model_matrix = glm::scale(model_matrix, glm::fvec3{p.m_size, p.m_size, p.m_size});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));


  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
}


void ApplicationSolar::render() const {
	for (unsigned int i = 0; i < all_planets.size(); i++)
	{
		upload_planet_transforms(all_planets[i]);
	}
	for (unsigned int i = 0; i < all_satellites.size(); i++)
	{
		upload_planet_transforms(all_satellites[i]);
	}
}

void ApplicationSolar::updateView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::updateProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
  updateUniformLocations();
  
  // bind new shader
  glUseProgram(m_shaders.at("planet").handle);
  
  updateView();
  updateProjection();
}

// handle key input
void ApplicationSolar::keyCallback(int key, int scancode, int action, int mods){
  //Forwards
  if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.5f});
    updateView();
  }
  //Backwards
  else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.5f});
    updateView();
  }
  //Left
  else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.5f, 0.0f, 0.0f});
    updateView();
  }
  //Right
  else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.5f, 0.0f, 0.0f});
    updateView();
  }
  //Up
  else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.5f, 0.0f});
    updateView();
  }
  //Down
  else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.5f, 0.0f});
    updateView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  m_view_transform = glm::rotate(m_view_transform, (float)pos_x/100, glm::fvec3(0.0f, -1.0f, 0.0f));
  m_view_transform = glm::rotate(m_view_transform, (float)pos_y/100, glm::fvec3(-1.0f, 0.0f, 0.0f));
  updateView();
}

// load shader programs
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{m_resource_path + "shaders/simple.vert",
                                           m_resource_path + "shaders/simple.frag"});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  //+++++++++++++++++++++++PlANETS++++++++++++++++++++

  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);


  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);// DAS HIER SELBEER MACHEN
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());

  //+++++++++++++++++STARS+++++++++++++++++++++++++++++++++

  // generate vertex array object
  glGenVertexArrays(1, &star_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(star_object.vertex_AO);
  // generate generic buffer
  glGenBuffers(1, &star_object.vertex_BO);// DAS HIER SELBEER MACHEN
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  // configure currently bound array buffer -> We can use size of container, because we don't have spheres
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * all_stars.size(), all_stars.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 3);

   // generate generic buffer
  glGenBuffers(1, &star_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * star_model.indices.size(), star_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object 
  star_object.num_elements = GLsizei(all_stars.size());
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

// exe entry point
int main(int argc, char* argv[]) {
  Launcher::run<ApplicationSolar>(argc, argv);
}
