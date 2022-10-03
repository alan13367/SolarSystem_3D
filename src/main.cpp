/* Assignment 3 by Alan Beltran and Josep Segarra*/
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include<Windows.h>




//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//STEP 5: INCLUDE THE imageloader
#include "imageloader.h"

//include some custom code files
#include "glfunctions.h" //include all OpenGL stuff
#include "Shader.h" // class to compile shaders

#include <string>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "main.h"



using namespace std;
using namespace glm;

string basepath = "assets/";
string inputfilesphere = basepath + "sphere.obj";
string inputfilespaceship = basepath + "Low Poly Spaceship.obj";

vector< tinyobj::shape_t > shapes;    
vector< tinyobj::material_t > materials;

vector< tinyobj::shape_t > shapes2;
vector< tinyobj::material_t > materials2;

vector< tinyobj::shape_t > shapessun;
vector< tinyobj::material_t > materialssun;

vector< tinyobj::shape_t > shapesspaceship;
vector< tinyobj::material_t > materialsspaceship;




//global variables to help us do things
bool pressed = false;
int g_ViewportWidth = 512; int g_ViewportHeight = 512; // Default window size, in pixels
double mouse_x, mouse_y; //variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f); // background colour - a GLM 3-component vector

const float PI = 3.1415;

bool ismousepressed = false;


float yaw1 = -90.0f;
float pitch1 = 0.0f;
float ultX = g_ViewportWidth / 2.0;
float ultY = g_ViewportHeight / 2.0;

vec3 eye = vec3(1.0f, 1.0f, 1.0f);
vec3 center = vec3(0.0f, 0.0f, -3.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);

vec3 g_light_dir(0.0f, 0.0f, -10.0f);


GLuint g_simpleShader = 0; //shader identifier
GLuint g_Vao = 0; //vao
GLuint g_NumTriangles = 0; //  Numbre of triangles we are painting.

GLuint g_simpleShader_sun = 0;
GLuint g_Vao_Sun = 0;
GLuint g_NumTriangles_sun = 0;

GLuint g_simpleShader_spaceship = 0;
GLuint g_Vao_Spaceship = 0;
GLuint g_NumTriangles_spaceship = 0;

GLuint g_simpleShader_sky = 0; //shader identifier
GLuint g_Vao_sky = 0; //vao
GLuint g_NumTriangles_sky = 0;// Number of triangles we are painting.

float speedmercury = 7.0f;
float radiusmercury = 10.0f;

float speedvenus = 6.0f;
float radiusvenus = 15.0f;

float speedearth = 5.0f;
float radiusearth = 20.0f;

float speedmars = 3.0f;
float radiusmars = 25.0f;

float speedjupiter = 2.0f;
float radiusjupiter = 30.0f;

float speedsaturn = 1.5f;
float radiussaturn = 40.0f;

float speeduranus = 1.0f;
float radiusuranus = 45.0;

float speedneptune = 1.0f;
float radiusneptune = 50.0f;

//STEP 6: add a global variable for the texture ID
GLuint texture_id;
GLuint texture_id2;
GLuint texture_idspaceship;
GLuint texture_idsun;
GLuint texture_idmercury;
GLuint texture_idvenus;
GLuint texture_idmars;
GLuint texture_idjupiter;
GLuint texture_idsaturn;
GLuint texture_iduranus;
GLuint texture_idneptune;

//LOAD EVERYTHING
void load()
{
    
	string err;

    bool  ret = tinyobj::LoadObj(shapes, materials, err, inputfilesphere.c_str(), basepath.c_str());
	ret = tinyobj::LoadObj(shapes2, materials2, err,inputfilesphere.c_str(), basepath.c_str());
	ret = tinyobj::LoadObj(shapessun, materialssun, err,inputfilesphere.c_str(), basepath.c_str());
	ret = tinyobj::LoadObj(shapesspaceship, materialsspaceship, err, inputfilespaceship.c_str(), basepath.c_str());


	//we assign a colour to each vertex (all of the vertices as assigned WHITE)
	vector<GLfloat> colors(shapes[0].mesh.positions.size(), 1.0);
	

	//test if OBJ loaded correctly
	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}
	else
	//print out number of meshes described in file
	    std::cout << "Loaded "<< inputfilesphere << " with shapes: " << shapes.size() << std::endl;


 	//**********************
	// CODE TO LOAD EVERYTHING INTO MEMORY
	//**********************

	//load the shader
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_simpleShader = simpleShader.program;

	// Create the VAO where we store all geometry (stored in g_Vao)
	g_Vao = gl_createAndBindVAO();

	gl_createAndBindAttribute(&(shapes[0].mesh.positions[0]), shapes[0].mesh.positions.size() * sizeof(float), g_simpleShader, "a_vertex", 3);
	gl_createAndBindAttribute(&(colors[0]), colors.size()* sizeof(GLfloat), g_simpleShader, "a_color", 3);
	gl_createAndBindAttribute(&(shapes[0].mesh.normals[0]), shapes[0].mesh.normals.size() * sizeof(float), g_simpleShader, "a_normal", 3);
	gl_createIndexBuffer(&(shapes[0].mesh.indices[0]), shapes[0].mesh.indices.size() * sizeof(unsigned int));
	gl_createAndBindAttribute(&(shapes[0].mesh.texcoords[0]),shapes[0].mesh.texcoords.size() * sizeof(GLfloat),g_simpleShader,"a_uv", 2);

	gl_unbindVAO();

	g_NumTriangles = shapes[0].mesh.indices.size() / 3;


	Shader simpleShader_sky("src/shader_sky.vert", "src/shader_sky.frag");
	g_simpleShader_sky = simpleShader_sky.program;

	g_Vao_sky = gl_createAndBindVAO();

	gl_createAndBindAttribute(&(shapes2[0].mesh.positions[0]),
		shapes2[0].mesh.positions.size() * sizeof(float), g_simpleShader_sky, "a_vertex", 3);
	gl_createIndexBuffer(&(shapes2[0].mesh.indices[0]),
		shapes2[0].mesh.indices.size() * sizeof(unsigned int));
	gl_createAndBindAttribute(&(shapes2[0].mesh.texcoords[0]),
		shapes2[0].mesh.texcoords.size() * sizeof(GLfloat), g_simpleShader_sky, "a_uv", 2);

	

	gl_unbindVAO();
	g_NumTriangles_sky = shapes2[0].mesh.indices.size() / 3;


	Shader simpleShader_sun("src/shader_sun.vert", "src/shader_sun.frag");
	g_simpleShader_sun = simpleShader_sun.program;


	g_Vao_Sun = gl_createAndBindVAO();
	gl_createAndBindAttribute(&(shapessun[0].mesh.positions[0]),
		shapessun[0].mesh.positions.size() * sizeof(float), g_simpleShader_sun, "a_vertex", 3);
	gl_createIndexBuffer(&(shapessun[0].mesh.indices[0]),
		shapessun[0].mesh.indices.size() * sizeof(unsigned int));
	gl_createAndBindAttribute(&(shapessun[0].mesh.texcoords[0]),
		shapessun[0].mesh.texcoords.size() * sizeof(GLfloat), g_simpleShader_sun, "a_uv", 2);

	
	gl_unbindVAO();
	g_NumTriangles_sun = shapessun[0].mesh.indices.size() / 3;

	Shader simpleShader_spaceship("src/shader_spaceship.vert", "src/shader_spaceship.frag");
	g_simpleShader_spaceship = simpleShader_spaceship.program;
	g_Vao_Spaceship = gl_createAndBindVAO();
	gl_createAndBindAttribute(&(shapesspaceship[0].mesh.positions[0]),
		shapesspaceship[0].mesh.positions.size() * sizeof(float), g_simpleShader_spaceship, "a_vertex", 3);
	gl_createIndexBuffer(&(shapesspaceship[0].mesh.indices[0]),
		shapesspaceship[0].mesh.indices.size() * sizeof(unsigned int));
	gl_createAndBindAttribute(&(shapesspaceship[0].mesh.texcoords[0]),
		shapesspaceship[0].mesh.texcoords.size() * sizeof(GLfloat), g_simpleShader_spaceship, "a_uv", 2);
	gl_unbindVAO();
	g_NumTriangles_spaceship = shapesspaceship[0].mesh.indices.size() / 3;


	Image* image;

	//FUNCTION TO LOAD ALL BMPS INTO THE MEMORY
	loadBMPS(image);


}

void loadBMPS(Image*& image)
{
	image = loadBMP("assets/earthmap1k.bmp");
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/milkyway.bmp");
	glGenTextures(1, &texture_id2);
	glBindTexture(GL_TEXTURE_2D, texture_id2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/sun.bmp");
	glGenTextures(1, &texture_idsun);
	glBindTexture(GL_TEXTURE_2D, texture_idsun);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/mercury.bmp");
	glGenTextures(1, &texture_idmercury);
	glBindTexture(GL_TEXTURE_2D, texture_idmercury);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/venus.bmp");
	glGenTextures(1, &texture_idvenus);
	glBindTexture(GL_TEXTURE_2D, texture_idvenus);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/mars.bmp");
	glGenTextures(1, &texture_idmars);
	glBindTexture(GL_TEXTURE_2D, texture_idmars);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/jupiter.bmp");
	glGenTextures(1, &texture_idjupiter);
	glBindTexture(GL_TEXTURE_2D, texture_idjupiter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/saturn.bmp");
	glGenTextures(1, &texture_idsaturn);
	glBindTexture(GL_TEXTURE_2D, texture_idsaturn);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/uranus.bmp");
	glGenTextures(1, &texture_iduranus);
	glBindTexture(GL_TEXTURE_2D, texture_iduranus);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	image = loadBMP("assets/neptune.bmp");
	glGenTextures(1, &texture_idneptune);
	glBindTexture(GL_TEXTURE_2D, texture_idneptune);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	/*
	#######################################################
	#######################################################
	################# SKYBOX DRAWING  ######################
	#######################################################
	#######################################################
	*/

	glUseProgram(g_simpleShader_sky);
	gl_bindVAO(g_Vao_sky);

	GLuint u_model = glGetUniformLocation(g_simpleShader_sky, "u_model");
	GLuint u_view = glGetUniformLocation(g_simpleShader_sky, "u_view");
	GLuint u_projection = glGetUniformLocation(g_simpleShader_sky, "u_projection");

	mat4 tSky = translate(mat4(1.0f), eye); 
	mat4 Ssky = scale(mat4(1.0f), vec3(12.0f, 12.0f, 12.0f));
	mat4 projection_matrix = perspective(
		90.0f, 
		1.0f,  
		0.1f,  
		50.0f  
	);
	mat4 view_matrix = lookAt(
		eye,
		eye + center,
		up
	);
	mat4 model_matrixsky = tSky * Ssky;
	
	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrixsky));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint u_texturesky = glGetUniformLocation(g_simpleShader_sky, "u_texture");
	glUniform1i(u_texturesky, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id2);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sky, GL_UNSIGNED_INT, 0);
	gl_unbindVAO();

	/*
	#######################################################
	#######################################################
	################# SUN DRAWING  ########################
	#######################################################
	#######################################################
	*/

	glUseProgram(g_simpleShader_sun);
	gl_bindVAO(g_Vao_Sun);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);


	GLuint modelsun_loc = glGetUniformLocation(g_simpleShader_sun, "u_model");
	mat4 Tsun = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)); //x y z
	mat4 Ssun = scale(mat4(1.0f), vec3(4.0f, 4.0f, 4.0f));
	mat4 modelsun = Tsun * Ssun;

	GLuint u_texturesun = glGetUniformLocation(g_simpleShader_sun, "u_texture");
	glUniform1i(u_texturesun, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_idsun);

	glUniformMatrix4fv(modelsun_loc, 1, GL_FALSE, glm::value_ptr(modelsun));

	GLuint projection_locsun = glGetUniformLocation(g_simpleShader_sun, "u_projection");
	glUniformMatrix4fv(projection_locsun, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locsun  = glGetUniformLocation(g_simpleShader_sun, "u_view");
	glUniformMatrix4fv(view_locsun , 1, GL_FALSE, glm::value_ptr(view_matrix));

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sun, GL_UNSIGNED_INT, 0);

	gl_unbindVAO();
	
	/*
	#######################################################
	#######################################################
	################# EARTH DRAWING  ######################
	#######################################################
	#######################################################
	*/

	glUseProgram(g_simpleShader);

	gl_bindVAO(g_Vao);  

	GLuint u_texture = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	GLuint model_loc = glGetUniformLocation(g_simpleShader, "u_model");
	
	float x, y;
	float angle = (float)glfwGetTime() * speedearth;
	
	x = radiusearth * sin(PI * 2 * angle / 360);
	y = radiusearth * cos(PI * 2 * angle / 360);
	
	mat4 tEarth = translate(mat4(1.0f), vec3(x, 0.0f, y)); 
	mat4 rEarth = rotate(mat4(1.0f), (float)glfwGetTime() * 25.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 modelEarth = tEarth * rEarth;
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(modelEarth));
	
	GLuint normal_loc = glGetUniformLocation(g_simpleShader, "u_normal");
	mat3 normal_earth = mat3(transpose(inverse(modelEarth)));
	glUniformMatrix3fv(normal_loc, 1, GL_FALSE, glm::value_ptr(normal_earth));

	GLuint projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_loc = glGetUniformLocation(g_simpleShader, "u_light_dir"); glUniform3f(light_loc, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint cam_pos_loc = glGetUniformLocation(g_simpleShader, "u_cam_pos"); // this is the camera position, if you a view matrix this is the position of eye glUniform3f(cam_pos_loc, eye.x, eye.y, eye.z); 
	GLuint ambient_loc = glGetUniformLocation(g_simpleShader, "u_ambient"); glUniform3f(ambient_loc, 0.0, 0.0, 0.2);
	GLuint diffuse_loc = glGetUniformLocation(g_simpleShader, "u_diffuse"); glUniform3f(diffuse_loc, 0.0, 0.0, 1.0);   // blue light 
	GLuint specular_loc = glGetUniformLocation(g_simpleShader, "u_specular"); glUniform3f(specular_loc, 1.0, 1.0, 1.0);
	GLuint shininess_loc = glGetUniformLocation(g_simpleShader, "u_shininess"); glUniform1f(shininess_loc, 80.0);
	glUniform3f(light_loc, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);


	/*
	#######################################################
	#######################################################
	################# MERCURY DRAWING  ####################
	#######################################################
	#######################################################
	*/

	GLuint u_texturemercury = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texturemercury, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture_idmercury);

	GLuint model_locmercury = glGetUniformLocation(g_simpleShader, "u_model");

	angle = (float)glfwGetTime() * speedmercury;

	x = radiusmercury * sin(PI * 2 * angle / 360);
	y = radiusmercury * cos(PI * 2 * angle / 360);

	mat4 tMercury = translate(mat4(1.0f), vec3(x, 0.0f, y));
	mat4 rMercury = rotate(mat4(1.0f), (float)glfwGetTime() * 25.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 sMercury = scale(mat4(1.0f), vec3(0.383f, 0.383f, 0.383f));
	mat4 modelMercury = tMercury * rMercury * sMercury;
	glUniformMatrix4fv(model_locmercury, 1, GL_FALSE, glm::value_ptr(modelMercury));

	GLuint normal_locmercury = glGetUniformLocation(g_simpleShader, "u_normal");
	mat3 normal_mercury = mat3(transpose(inverse(modelMercury)));
	glUniformMatrix3fv(normal_locmercury, 1, GL_FALSE, glm::value_ptr(normal_mercury));

	GLuint projection_locmercury = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_locmercury, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locmercury = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_locmercury, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_locmercury = glGetUniformLocation(g_simpleShader, "u_light_dir"); glUniform3f(light_locmercury, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint cam_pos_locmercury = glGetUniformLocation(g_simpleShader, "u_cam_pos"); // this is the camera position, if you a view matrix this is the position of eye glUniform3f(cam_pos_loc, eye.x, eye.y, eye.z); 
	GLuint ambient_locmercury = glGetUniformLocation(g_simpleShader, "u_ambient"); glUniform3f(ambient_locmercury, 0.0, 0.0, 0.2);
	GLuint diffuse_locmercury = glGetUniformLocation(g_simpleShader, "u_diffuse"); glUniform3f(diffuse_locmercury, 0.0, 0.0, 1.0);   // blue light 
	GLuint specular_locmercury = glGetUniformLocation(g_simpleShader, "u_specular"); glUniform3f(specular_locmercury, 1.0, 1.0, 1.0);
	GLuint shininess_locmercury = glGetUniformLocation(g_simpleShader, "u_shininess"); glUniform1f(shininess_locmercury, 80.0);
	glUniform3f(light_locmercury, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	/*
	#######################################################
	#######################################################
	################# VENUS DRAWING  ####################
	#######################################################
	#######################################################
	*/
	GLuint u_texturevenus = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texturevenus, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture_idvenus);

	GLuint model_locvenus = glGetUniformLocation(g_simpleShader, "u_model");

	angle = (float)glfwGetTime() * speedvenus;
	x = radiusvenus * sin(PI * 2 * angle / 360);
	y = radiusvenus * cos(PI * 2 * angle / 360);

	mat4 tVenus = translate(mat4(1.0f), vec3(x, 0.0f, y));
	mat4 rVenus = rotate(mat4(1.0f), (float)glfwGetTime() * 25.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 sVenus = scale(mat4(1.0f), vec3(0.949f, 0.949f, 0.949f));
	mat4 modelVenus = tVenus * rVenus * sVenus;
	glUniformMatrix4fv(model_locvenus, 1, GL_FALSE, glm::value_ptr(modelVenus));

	GLuint normal_locvenus = glGetUniformLocation(g_simpleShader, "u_normal");
	mat3 normal_venus = mat3(transpose(inverse(modelVenus)));
	glUniformMatrix3fv(normal_locvenus, 1, GL_FALSE, glm::value_ptr(normal_venus));

	GLuint projection_locvenus = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_locvenus, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locvenus = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_locvenus, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_locvenus = glGetUniformLocation(g_simpleShader, "u_light_dir"); glUniform3f(light_locvenus, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint cam_pos_locvenus = glGetUniformLocation(g_simpleShader, "u_cam_pos"); // this is the camera position, if you a view matrix this is the position of eye glUniform3f(cam_pos_loc, eye.x, eye.y, eye.z); 
	GLuint ambient_locvenus = glGetUniformLocation(g_simpleShader, "u_ambient"); glUniform3f(ambient_locvenus, 0.0, 0.0, 0.2);
	GLuint diffuse_locvenus = glGetUniformLocation(g_simpleShader, "u_diffuse"); glUniform3f(diffuse_locvenus, 0.0, 0.0, 1.0);   // blue light 
	GLuint specular_locvenus = glGetUniformLocation(g_simpleShader, "u_specular"); glUniform3f(specular_locvenus, 1.0, 1.0, 1.0);
	GLuint shininess_locvenus = glGetUniformLocation(g_simpleShader, "u_shininess"); glUniform1f(shininess_locvenus, 80.0);
	glUniform3f(light_locvenus, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	/*
	#######################################################
	#######################################################
	################# MARS DRAWING  ####################
	#######################################################
	#######################################################
	*/
	GLuint u_texturemars = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texturemars, 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture_idmars);

	GLuint model_locmars = glGetUniformLocation(g_simpleShader, "u_model");

	angle = (float)glfwGetTime() * speedmars;
	x = radiusmars * sin(PI * 2 * angle / 360);
	y = radiusmars * cos(PI * 2 * angle / 360);

	mat4 tMars = translate(mat4(1.0f), vec3(x, 0.0f, y));
	mat4 rMars = rotate(mat4(1.0f), (float)glfwGetTime() * 25.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 sMars = scale(mat4(1.0f), vec3(0.532f, 0.532f, 0.532f));
	mat4 modelMars = tMars * rMars * sMars;
	glUniformMatrix4fv(model_locmars, 1, GL_FALSE, glm::value_ptr(modelMars));

	GLuint normal_locmars = glGetUniformLocation(g_simpleShader, "u_normal");
	mat3 normal_mars = mat3(transpose(inverse(modelMars)));
	glUniformMatrix3fv(normal_locmars, 1, GL_FALSE, glm::value_ptr(normal_mars));

	GLuint projection_locmars = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_locmars, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locmars = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_locmars, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_locmars = glGetUniformLocation(g_simpleShader, "u_light_dir"); glUniform3f(light_locmars, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint cam_pos_locmars = glGetUniformLocation(g_simpleShader, "u_cam_pos"); // this is the camera position, if you a view matrix this is the position of eye glUniform3f(cam_pos_loc, eye.x, eye.y, eye.z); 
	GLuint ambient_locmars = glGetUniformLocation(g_simpleShader, "u_ambient"); glUniform3f(ambient_locmars, 0.0, 0.0, 0.2);
	GLuint diffuse_locmars = glGetUniformLocation(g_simpleShader, "u_diffuse"); glUniform3f(diffuse_locmars, 0.0, 0.0, 1.0);   // blue light 
	GLuint specular_locmars = glGetUniformLocation(g_simpleShader, "u_specular"); glUniform3f(specular_locmars, 1.0, 1.0, 1.0);
	GLuint shininess_locmars = glGetUniformLocation(g_simpleShader, "u_shininess"); glUniform1f(shininess_locmars, 80.0);
	glUniform3f(light_locmars, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	/*
	#######################################################
	#######################################################
	################# JUPITERS DRAWING  ####################
	#######################################################
	#######################################################
	*/
	GLuint u_texturejupiter = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texturejupiter, 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, texture_idjupiter);

	GLuint model_locjupiter = glGetUniformLocation(g_simpleShader, "u_model");

	angle = (float)glfwGetTime() * speedjupiter;
	x = radiusjupiter * sin(PI * 2 * angle / 360);
	y = radiusjupiter * cos(PI * 2 * angle / 360);

	mat4 tJupiter = translate(mat4(1.0f), vec3(x, 0.0f, y));
	mat4 rJupiter = rotate(mat4(1.0f), (float)glfwGetTime() * 15.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 sJupiter = scale(mat4(1.0f), vec3(2.5f, 2.5f, 2.5f));
	mat4 modelJupiter = tJupiter * rJupiter * sJupiter;
	glUniformMatrix4fv(model_locjupiter, 1, GL_FALSE, glm::value_ptr(modelJupiter));

	GLuint normal_locjupiter = glGetUniformLocation(g_simpleShader, "u_normal");
	mat3 normal_jupiter = mat3(transpose(inverse(modelJupiter)));
	glUniformMatrix3fv(normal_locjupiter, 1, GL_FALSE, glm::value_ptr(normal_jupiter));

	GLuint projection_locjupiter = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_locjupiter, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locjupiter = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_locjupiter, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_locjupiter = glGetUniformLocation(g_simpleShader, "u_light_dir"); glUniform3f(light_locjupiter, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint cam_pos_locjupiter = glGetUniformLocation(g_simpleShader, "u_cam_pos"); // this is the camera position, if you a view matrix this is the position of eye glUniform3f(cam_pos_loc, eye.x, eye.y, eye.z); 
	GLuint ambient_locjupiter = glGetUniformLocation(g_simpleShader, "u_ambient"); glUniform3f(ambient_locjupiter, 0.0, 0.0, 0.2);
	GLuint diffuse_locjupiter = glGetUniformLocation(g_simpleShader, "u_diffuse"); glUniform3f(diffuse_locjupiter, 0.0, 0.0, 1.0);   // blue light 
	GLuint specular_locjupiter = glGetUniformLocation(g_simpleShader, "u_specular"); glUniform3f(specular_locjupiter, 1.0, 1.0, 1.0);
	GLuint shininess_locjupiter = glGetUniformLocation(g_simpleShader, "u_shininess"); glUniform1f(shininess_locjupiter, 80.0);
	glUniform3f(light_locjupiter, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	/*
	#######################################################
	#######################################################
	################# SATURN DRAWING  ####################
	#######################################################
	#######################################################
	*/
	GLuint u_texturesaturn = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texturesaturn, 8);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, texture_idsaturn);

	GLuint model_locsaturn = glGetUniformLocation(g_simpleShader, "u_model");

	angle = (float)glfwGetTime() * speedsaturn;
	x = radiussaturn * sin(PI * 2 * angle / 360);
	y = radiussaturn * cos(PI * 2 * angle / 360);

	mat4 tSaturn = translate(mat4(1.0f), vec3(x, 0.0f, y));
	mat4 rSaturn = rotate(mat4(1.0f), (float)glfwGetTime() * 20.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 sSaturn = scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
	mat4 modelSaturn = tSaturn * rSaturn * sSaturn;
	glUniformMatrix4fv(model_locsaturn, 1, GL_FALSE, glm::value_ptr(modelSaturn));

	GLuint normal_locsaturn = glGetUniformLocation(g_simpleShader, "u_normal");
	mat3 normal_saturn = mat3(transpose(inverse(modelSaturn)));
	glUniformMatrix3fv(normal_locsaturn, 1, GL_FALSE, glm::value_ptr(normal_saturn));

	GLuint projection_locsaturn = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_locsaturn, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locsaturn = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_locsaturn, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_locsaturn = glGetUniformLocation(g_simpleShader, "u_light_dir"); glUniform3f(light_locsaturn, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint cam_pos_locsaturn = glGetUniformLocation(g_simpleShader, "u_cam_pos"); // this is the camera position, if you a view matrix this is the position of eye glUniform3f(cam_pos_loc, eye.x, eye.y, eye.z); 
	GLuint ambient_locsaturn = glGetUniformLocation(g_simpleShader, "u_ambient"); glUniform3f(ambient_locsaturn, 0.0, 0.0, 0.2);
	GLuint diffuse_locsaturn = glGetUniformLocation(g_simpleShader, "u_diffuse"); glUniform3f(diffuse_locsaturn, 0.0, 0.0, 1.0);   // blue light 
	GLuint specular_locsaturn = glGetUniformLocation(g_simpleShader, "u_specular"); glUniform3f(specular_locsaturn, 1.0, 1.0, 1.0);
	GLuint shininess_locsaturn = glGetUniformLocation(g_simpleShader, "u_shininess"); glUniform1f(shininess_locsaturn, 80.0);
	glUniform3f(light_locsaturn, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	/*
	#######################################################
	#######################################################
	################# URANUS DRAWING  #####################
	#######################################################
	#######################################################
	*/
	GLuint u_textureuranus = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_textureuranus, 9);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, texture_iduranus);

	GLuint model_locuranus = glGetUniformLocation(g_simpleShader, "u_model");

	angle = (float)glfwGetTime() * speeduranus;
	x = radiusuranus * sin(PI * 2 * angle / 360);
	y = radiusuranus * cos(PI * 2 * angle / 360);

	mat4 tUranus = translate(mat4(1.0f), vec3(x, 0.0f, y));
	mat4 rUranus = rotate(mat4(1.0f), (float)glfwGetTime() * 25.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 sUranus = scale(mat4(1.0f), vec3(0.6f, 0.6f, 0.6f));
	mat4 modelUranus = tUranus * rUranus * sUranus;
	glUniformMatrix4fv(model_locuranus, 1, GL_FALSE, glm::value_ptr(modelUranus));

	GLuint normal_locuranus = glGetUniformLocation(g_simpleShader, "u_normal");
	mat3 normal_uranus = mat3(transpose(inverse(modelUranus)));
	glUniformMatrix3fv(normal_locuranus, 1, GL_FALSE, glm::value_ptr(normal_uranus));

	GLuint projection_locuranus = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_locuranus, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locuranus = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_locuranus, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_locuranus = glGetUniformLocation(g_simpleShader, "u_light_dir"); glUniform3f(light_locuranus, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint cam_pos_locuranus = glGetUniformLocation(g_simpleShader, "u_cam_pos"); // this is the camera position, if you a view matrix this is the position of eye glUniform3f(cam_pos_loc, eye.x, eye.y, eye.z); 
	GLuint ambient_locuranus = glGetUniformLocation(g_simpleShader, "u_ambient"); glUniform3f(ambient_locuranus, 0.0, 0.0, 0.2);
	GLuint diffuse_locuranus = glGetUniformLocation(g_simpleShader, "u_diffuse"); glUniform3f(diffuse_locuranus, 0.0, 0.0, 1.0);   // blue light 
	GLuint specular_locuranus = glGetUniformLocation(g_simpleShader, "u_specular"); glUniform3f(specular_locuranus, 1.0, 1.0, 1.0);
	GLuint shininess_locuranus = glGetUniformLocation(g_simpleShader, "u_shininess"); glUniform1f(shininess_locuranus, 80.0);
	glUniform3f(light_locuranus, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	/*
	#######################################################
	#######################################################
	################# NEPTUNE DRAWING  ####################
	#######################################################
	#######################################################
	*/
	
	GLuint u_textureneptune = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_textureneptune, 10);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, texture_idneptune);

	GLuint model_locneptune = glGetUniformLocation(g_simpleShader, "u_model");

	angle = (float)glfwGetTime() * speedneptune;
	x = radiusneptune * sin(PI * 2 * angle / 360);
	y = radiusneptune * cos(PI * 2 * angle / 360);

	mat4 tNeptune = translate(mat4(1.0f), vec3(x, 0.0f, y));
	mat4 rNeptune = rotate(mat4(1.0f), (float)glfwGetTime() * 20.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 sNeptune = scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));
	mat4 modelNeptune = tNeptune * rNeptune * sNeptune;
	glUniformMatrix4fv(model_locneptune, 1, GL_FALSE, glm::value_ptr(modelNeptune));

	GLuint normal_locneptune = glGetUniformLocation(g_simpleShader, "u_normal");
	mat3 normal_neptune = mat3(transpose(inverse(modelNeptune)));
	glUniformMatrix3fv(normal_locneptune, 1, GL_FALSE, glm::value_ptr(normal_neptune));

	GLuint projection_locneptune = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_locneptune, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locneptune = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_locneptune, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_locneptune = glGetUniformLocation(g_simpleShader, "u_light_dir"); glUniform3f(light_locneptune, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint cam_pos_locneptune = glGetUniformLocation(g_simpleShader, "u_cam_pos"); // this is the camera position, if you a view matrix this is the position of eye glUniform3f(cam_pos_loc, eye.x, eye.y, eye.z); 
	GLuint ambient_locneptune = glGetUniformLocation(g_simpleShader, "u_ambient"); glUniform3f(ambient_locneptune, 0.0, 0.0, 0.2);
	GLuint diffuse_locneptune = glGetUniformLocation(g_simpleShader, "u_diffuse"); glUniform3f(diffuse_locneptune, 0.0, 0.0, 1.0);   // blue light 
	GLuint specular_locneptune = glGetUniformLocation(g_simpleShader, "u_specular"); glUniform3f(specular_locneptune, 1.0, 1.0, 1.0);
	GLuint shininess_locneptune = glGetUniformLocation(g_simpleShader, "u_shininess"); glUniform1f(shininess_locneptune, 80.0);
	glUniform3f(light_locneptune, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);
	
	gl_unbindVAO();


	/*
	#######################################################
	#######################################################
	################# SPACESHIP DRAWING  ####################
	#######################################################
	#######################################################
	*/

	glUseProgram(g_simpleShader_spaceship);
	gl_bindVAO(g_Vao_Spaceship);

	
	GLuint modelspaceship_loc = glGetUniformLocation(g_simpleShader_spaceship, "u_model");
	mat4 Tspaceship = translate(mat4(1.0f), vec3(5.0f, 0.0f, 5.0f + (float)glfwGetTime() * 0.1)); //x y z
	mat4 Sspaceship = scale(mat4(1.0f), vec3(0.3f, 0.3f, 0.3f));
	mat4 modelspaceship = Tspaceship * Sspaceship;

	GLuint colorLoc = glGetUniformLocation(g_simpleShader_spaceship, "u_color");
	glUniform3f(colorLoc, 0.0, 1.0, 0.0);

	glUniformMatrix4fv(modelspaceship_loc, 1, GL_FALSE, glm::value_ptr(modelspaceship));

	GLuint projection_locspaceship = glGetUniformLocation(g_simpleShader_spaceship, "u_projection");
	glUniformMatrix4fv(projection_locspaceship, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint view_locspaceship = glGetUniformLocation(g_simpleShader_spaceship, "u_view");
	glUniformMatrix4fv(view_locspaceship, 1, GL_FALSE, glm::value_ptr(view_matrix));

	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_spaceship, GL_UNSIGNED_INT, 0);

	gl_unbindVAO();
}

// ------------------------------------------------------------------------------------------
// This function is called every time you press a screen
// ------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//quit
	float cameraSpeed = 0.15f;

	if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT))
		glfwSetWindowShouldClose(window, 1);

	//reload
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		load();

	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		eye -= normalize(cross(center, up)) * cameraSpeed;

	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		eye += normalize(cross(center, up)) * cameraSpeed;

	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		eye -= cameraSpeed * center;

	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		eye += cameraSpeed * center;

}

// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		ismousepressed = true;

	}
	else {
		ismousepressed = false;
	}
}

void onMouseMove(GLFWwindow* window, double x, double y) {
	cout << "Mouse at " << x << " , " << y << endl;

	if (ismousepressed == true) {

		float xoffset = x - ultX;
		float yoffset = ultY - y;

		ultX = x;
		ultY = y;

		//Sensitivity Can be changed at your preference
		float sensitivity = 0.25f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw1 += xoffset;
		pitch1 += yoffset;

		//This is to make sure pitch is in bounds
		if (pitch1 > 89.0f)
			pitch1 = 89.0f;
		if (pitch1 < -89.0f)
			pitch1 = -89.0f;

		vec3 frontvec;
		frontvec.x = cos(radians(yaw1)) * cos(radians(pitch1));
		frontvec.y = sin(radians(pitch1));
		frontvec.z = sin(radians(yaw1)) * cos(radians(pitch1));

		center = normalize(frontvec);

	}

}

int main(void)
{
	//setup window and boring stuff, defined in glfunctions.cpp
	GLFWwindow* window;
	if (!glfwInit())return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "Solar System", NULL, NULL);
	if (!window) { glfwTerminate();	return -1; }
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetCursorPosCallback(window, onMouseMove);


	//load all the resources
	load();
	//bool playing = PlaySound(TEXT("spacesound.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);


	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		
		draw();

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

		//mouse position must be tracked constantly (callbacks do not give accurate delta)
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
	}

	//terminate glfw and exit
	glfwTerminate();
	return 0;
}


