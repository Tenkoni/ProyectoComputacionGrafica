/*---------------------------------------------------------*/
/* ----------------   Proyecto Final --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Alumnos Miramontes Sarabia Luis Enrique ---------------*/
/*-----------------     Varela García Alan Paul   ---------------------------*/
//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>
#include "camera.h"
#include "Model.h"
#include <random>
#include "esfera.h"
#include "toroide.h"
#include <vector>
#include <fstream>
#include <iterator>
#include <deque>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "irrKlang.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 50.0f, 50.0f));
double	lastX = 0.0f,
lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 3.0f, 0.0f);
glm::vec3 lightDirection(0.0f, 0.0f, -3.0f);

void myData(void);
void display(Shader, Shader, Model, Model);
void getResolution(void);
void animate(void);
void load_vectors(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);
void load_vectors_anim(void);
//Figuras
Esfera my_sphere(1.0);
Toroide my_torus(1.0, 1.0, 3.0);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

//Texture
unsigned int	t_smile,
t_toalla,
t_unam,
t_white,
t_panda,
t_cubo,
t_caja,
t_caja_brillo;

//For model
float movKit_z = 0.0f,
movKit_x = 0.0f,
movKit_y = 0.0f,
rotKit_y = 0.0f,
rotKit_x = 0.0f,
rotKit_z = 0.0f,
fortuna_rot = 0.0f;

int estado = 0;
bool play, reversa = false, available = true, recording = false, autoroute = false, animrecord = false, anim_on = false, available2 = true;

//savestates_fortuna
std::vector < glm::mat4 > savestate(15);
std::vector < glm::mat4 > savestate_adorno(15);


//camera movements
std::deque < glm::vec3 > camerapos;
std::deque < double > pitchvec, yawvec;

//animations

std::deque < glm::vec3 > a_anastasia, a_ayanami, a_bb, a_futaba, a_indy, a_suika, a_mai, a_miri, a_mochi;


//factor de escala fortuna
float escala = 15.0f;


//reja savestate
glm::mat4 savestatereja = glm::mat4(1.0f), savestatereja2;
std::vector <double> test_pos_x(12, 0.0f);
std::vector <double> test_pos_z(12, 0.0f);
std::vector <float> test_grados(12, 0.0f);


//camera route
std::ofstream outFile1("CameraTour/camera_route.nino", ios::out | ios::binary);
std::ofstream outFile2("CameraTour/camera_pitch.nino", ios::out | ios::binary);
std::ofstream outFile3("CameraTour/camera_yaw.nino", ios::out | ios::binary);
std::ifstream inFile1("CameraTour/saved/camera_route.nino");
std::ifstream inFile2("CameraTour/saved/camera_pitch.nino");
std::ifstream inFile3("CameraTour/saved/camera_yaw.nino");

//animation_generator
std::ofstream outAnim("Animations/ayanami.nino", ios::out | ios::binary);
std::ifstream inAnim1("Animations/saved/anastasia.nino");
std::ifstream inAnim2("Animations/saved/ayanami.nino");




unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{


}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void animate(void)
{
	//rueda
	if (play)
	{
		fortuna_rot += 0.2f; //rueda de la fortuna
	}
	//camara grabar
	if (recording)
	{
		camerapos.push_back(camera.Position);
		pitchvec.push_back(camera.Pitch);
		yawvec.push_back(camera.Yaw);
	}
	//camara
	if (autoroute & !camerapos.empty())
	{
		camera.Position = camerapos.front();
		camerapos.pop_front();
		camera.Pitch = pitchvec.front();
		pitchvec.pop_front();
		camera.Yaw = yawvec.front();
		yawvec.pop_front();
		camera.updateCameraVectors();
	}
	//carro montaña
	if (estado == 0) {

		if (movKit_z < 80) {
			movKit_z += 1.0f;
			rotKit_x = -5;
		}
		else {
			rotKit_x = -60;
			estado = 1;
		}
	}

	if (estado == 1) {
		if (movKit_y <200) {
			movKit_z += 1.35f;
			movKit_y += 2.0f;
			rotKit_x = -60;
		}
		else {
			rotKit_x = -45;
			estado = 2;
		}
	}

	if (estado == 2) {
		if (movKit_y <210) {
			movKit_z += 0.75f;
			movKit_y += 1.0f;
			rotKit_x = -45;
		}
		else {
			rotKit_x = -25;
			estado = 3;
		}
	}

	if (estado == 3) {
		if (movKit_z <240) {
			movKit_z += 1.0f;
			movKit_y += 0.75f;
			rotKit_x = -15;
		}
		else {
			rotKit_x = -5;
			estado = 4;
		}
	}

	if (estado == 4) {
		if (movKit_z <310) {
			movKit_z += 1.0f;
			rotKit_x = 5;
		}
		else {
			rotKit_x = 10;
			rotKit_y = -30;
			estado = 5;
		}
	}

	if (estado == 5) {
		if (movKit_y > 200) {
			movKit_y -= 1.0f;
			movKit_z += 0.75f;
			movKit_x -= 0.75f;
			rotKit_x = 30;
		}
		else {
			rotKit_x = 85;
			rotKit_y = -15;
			rotKit_z = 30;
			estado = 6;
		}
	}

	if (estado == 6) {
		if (movKit_y > 150) {
			movKit_y -= 1.0f;
			movKit_x -= 0.20f;
			movKit_z += 0.25f;
			rotKit_z = 60;
		}
		else {
			rotKit_x = 90;
			rotKit_z = 85;
			estado = 7;
		}
	}

	if (estado == 7) {
		if (movKit_y > 50) {
			movKit_y -= 1.0f;
			movKit_x -= 0.40f;
		}
		else {
			rotKit_y = -45;
			estado = 8;
		}
	}



	if (estado == 8) {
		if (movKit_y > 30) {
			movKit_y -= 1.0f;
			movKit_x -= 1.5f;
		}
		else {
			rotKit_y = -85;
			estado = 9;
		}
	}


	if (estado == 9) {
		if (movKit_x > -170) {
			movKit_x -= 1.0f;
		}
		else {
			rotKit_y = -150;
			rotKit_z = -5;
			estado = 10;
		}
	}

	if (estado == 10) {
		if (movKit_y < 210) {
			movKit_x -= 0.40f;
			movKit_z += 0.15f;
			movKit_y += 1.0f;
		}
		else {
			rotKit_x = 85;
			estado = 11;
		}
	}

	if (estado == 11) {
		if (movKit_y < 230) {
			movKit_y += 0.85f;
			movKit_z += 0.85f;
			rotKit_x = 100;
		}
		else {

			rotKit_x = 120;
			estado = 12;
		}
	}

	if (estado == 12) {
		if (movKit_y < 250) {
			movKit_y += 0.85f;
			movKit_z += 0.85f;
			movKit_x -= 0.85f;
		}
		else {

			rotKit_x = 190;
			estado = 13;
		}
	}

	if (estado == 13) {
		if (movKit_z < 440) {
			movKit_z += 1.0f;
			movKit_x -= 0.15f;
		}
		else {

			rotKit_x = 230;
			estado = 14;
		}
	}

	if (estado == 14) {
		if (movKit_x > -310) {
			movKit_x -= 0.25f;
			movKit_z += 0.10f;
			movKit_y -= 0.45f;
		}
		else {
			rotKit_x = 300;
			rotKit_y = -180;
			estado = 15;
		}
	}

	if (estado == 15) {
		if (movKit_y > 10) {
			movKit_z -= 0.7f;
			movKit_y -= 1.0f;
			movKit_x -= 0.10f;

		}
		else {
			rotKit_x = 360;
			estado = 16;
		}
	}

	if (estado == 16) {
		if (movKit_z > 200) {
			movKit_z -= 0.5f;
			movKit_x -= 0.06f;
		}
		else {
			rotKit_x = 400;
			estado = 17;
		}
	}

	if (estado == 17) {
		if (movKit_z > 135) {
			movKit_z -= 0.85f;
			movKit_y += 0.5f;

		}
		else {
			rotKit_x = 360;
			estado = 18;
		}
	}

	if (estado == 18) {
		if (movKit_z > -15) {
			movKit_z -= 1.0f;
		}
		else {
			rotKit_x = 310;
			estado = 19;
		}
	}

	if (estado == 19) {
		if (movKit_z > -80) {
			movKit_z -= 1.0f;
			movKit_y -= 0.75f;
		}
		else {
			rotKit_x = 360;
			rotKit_y = -270;
			estado = 20;
		}
	}


	if (estado == 20) {
		if (movKit_x < 0) {
			movKit_x += 1.00f;
			movKit_z += 0.05f;
		}
		else {
			rotKit_y = -360;
			estado = 21;
		}
	}

	if (estado == 21) {
		if (movKit_z < 0) {
			movKit_z += 2.00f;
		}
		else {

			estado = 0;
		}
	}
	//grabar mov
	if (animrecord)
		a_ayanami.push_back(glm::vec3(test_pos_x.at(11), test_pos_z.at(11), test_grados.at(11)));

	//animaciones
	//anastasia anim
	if (anim_on & !a_anastasia.empty())
	{
		glm::vec3 a = a_anastasia.front();
		test_pos_x.at(0) = a.x;
		test_pos_z.at(0) = a.y;
		test_grados.at(0) = a.z;
		a_anastasia.pop_front();

	}

	if (anim_on & !a_ayanami.empty())
	{
		glm::vec3 a = a_ayanami.front();
		test_pos_x.at(1) = a.x;
		test_pos_z.at(1) = a.y;
		test_grados.at(1) = a.z;
		a_ayanami.pop_front();

	}
	/*if (anim_on & a_anastasia.empty())
	{
	test_pos_x.at(0) = a_anastasia.front().x;
	test_pos_z.at(0) = a_anastasia.front().y;
	test_grados.at(0) = a_anastasia.front().z;

	}*/
}

void display(Shader shader, Shader projectionShader, Model modelo, Model pista, Model pista2, Model reja, vector<Model> tienda, vector<Model> gente, vector<Model> arbol)
{
	//shader.use();

	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPosition);
	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 temp3 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

											//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
	view = camera.GetViewMatrix();

	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);

	//////////MONTAÑA
	/////RIEL-BASE
	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(-500.0f, 10.0f, -485.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	temp3 = temp;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		//////////////////////
	}

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 1.0f));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////


	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 1.0f));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 1.0f));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 1.0f));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////


	for (int ind = 0; ind < 9; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		//////////////////////
	}

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, -1.0f));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, -1.0f));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////
	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, -1.0f));
		model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		//////////////////////
	}

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, -1.0f));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(3.0f, 21.0f, -1.0f));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(4.0f, 21.0f, -2.0f));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////


	////////////////////////
	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
	}

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(4.0f, 21.0f, 3.0f));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
	}

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(4.0f, 21.0f, 3.0f));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	//////////////////////

	for (int ind = 0; ind < 3; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////
	}

	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////
	}


	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////	
	}



	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	////////////////////////


	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////	
	}


	for (int ind = 0; ind < 5; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		//////////////////////
	}

	for (int ind = 0; ind < 3; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////	
	}



	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////
	}

	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////
	}


	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////	
	}

	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////	
	}


	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(1.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;

	////////////////////////
	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;

	////////////////////////
	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(2.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;

	////////////////////////

	for (int ind = 0; ind < 6; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		////////////////////////
	}

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
	model = glm::rotate(model, glm::radians(27.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	////////////////////////
	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	////////////////
	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;

	///////////////////
	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;




	for (int ind = 0; ind < 4; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
	}


	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
	}


	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
	}


	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;

	}


	for (int ind = 0; ind < 3; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;

	}

	////////////////////////////////////////////////

	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;


	}


	for (int ind = 0; ind < 2; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		//	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;

	}

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(3.0f, 21.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;


	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(3.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;




	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(3.0f, 21.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;


	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(3.0f, 21.0f, 10.0f));
	model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;


	for (int ind = 0; ind < 12; ind++)
	{
		/////////////////////////////////////////////////
		model = glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
		temp = model;//respaldo de matriz
		temp2 = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;


		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;//respaldo de matriz
		model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp;

		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		temp = model;
		model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
		projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = temp2;
		/////////////////////////////////////////////////
	}

	model = glm::translate(model, glm::vec3(2.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(2.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(2.0f, 21.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;

	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(2.0f, 21.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp2;
	///////////
	/////////////////////////////////////////////////
	model = glm::translate(model, glm::vec3(2.0f, 21.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(1, 0, 0));
	//model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 0, 1));
	temp = model;//respaldo de matriz
	temp2 = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;


	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;//respaldo de matriz
	model = glm::scale(model, glm::vec3(12.0f, 1.5f, 1.5f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = temp;

	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	temp = model;
	model = glm::scale(model, glm::vec3(3.0f, 21.0f, 3.0f));
	projectionShader.setVec3("aColor", glm::vec3((float)0.9, (float)0.7, (float)0));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	///////////
	/////////////////
	///////////////////////////
	///////////////////////////

	//rueda de la fortuna
	//Soporte1
	model = savestate.at(0) = glm::translate(savestate.at(0) = glm::mat4(1.0f), glm::vec3(9.546*escala / 2, 8.9 / 2 * escala, 4 * escala / 1.5 *2.3));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5*escala, 13.5*escala, 0.5*escala));
	projectionShader.setVec3("aColor", glm::vec3((float)214 / 255, (float)216 / 255, (float)201 / 255));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Soporte2
	model = savestate.at(0) = glm::translate(savestate.at(0), glm::vec3(-13.5 * escala * cos(45 * M_PI / 180) + 0.25, 0, 0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(0.5*escala, 13.5 * escala, 0.5*escala));
	projectionShader.setVec3("aColor", glm::vec3((float)214 / 255, (float)216 / 255, (float)201 / 255));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Soporte3
	model = savestate.at(0) = glm::translate(savestate.at(0) = glm::mat4(1.0f), glm::vec3(9.546*escala / 2, 8.9 / 2 * escala, -4 * escala / 1.5 + 4 * escala / 1.5 * 2.3));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5*escala, 13.5 * escala, 0.5*escala));
	projectionShader.setVec3("aColor", glm::vec3((float)214 / 255, (float)216 / 255, (float)201 / 255));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Soporte4
	model = savestate.at(0) = glm::translate(savestate.at(0), glm::vec3(-13.5 * escala * cos(45 * M_PI / 180) + 0.25, 0, 0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(0.5*escala, 13.5 * escala, 0.5*escala));
	projectionShader.setVec3("aColor", glm::vec3((float)214 / 255, (float)216 / 255, (float)201 / 255));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Eje
	model = glm::translate(savestate.at(0) = glm::mat4(1.0f), glm::vec3(-13.5 * escala * cos(45 * M_PI / 180) / 2 + 9.546*escala / 2, (13.5 * escala * cos(45 * M_PI / 180) / 2) + (8.9 / 2)*escala, -2 * escala / 1.5 + 4 * escala / 1.5 * 2.3));
	model = savestate.at(0) = glm::rotate(model, glm::radians(fortuna_rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.25*escala, 0.25*escala, 2.25*escala / 1.5));
	projectionShader.setVec3("aColor", glm::vec3((float)244 / 255, (float)90 / 255, (float)116 / 255));
	projectionShader.setMat4("model", model);
	my_sphere.render();
	glBindVertexArray(VAO);

	for (int ind = 0; ind < 12; ind++)
	{
		//Viga30d
		model = savestate.at(ind + 1) = glm::rotate(savestate.at(0), glm::radians(ind*30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = savestate.at(ind + 1) = glm::translate(savestate.at(ind + 1), glm::vec3(0, (8.0*escala) / 2, 0));
		model = glm::scale(model, glm::vec3(0.25*escala, 8.0*escala, 0.25*escala));
		projectionShader.setVec3("aColor", glm::vec3((float)(double)rand() / (256) / 255, (float)(double)rand() / (256) / 255, (float)(double)rand() / (256) / 255));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//cabina
		model = glm::translate(savestate.at(ind + 1), glm::vec3(0, (8.0*escala) / 2, 0));
		model = savestate_adorno.at(ind) = glm::rotate(model, glm::radians(180.0f) - glm::radians(fortuna_rot) - glm::radians(ind*30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5*escala, 0.5*escala, 0.5*escala));
		projectionShader.setVec3("aColor", glm::vec3(0.9f, 0.8f, 0.6f));
		projectionShader.setMat4("model", model);
		my_sphere.render();
		//adornos
		model = glm::translate(savestate_adorno.at(ind), glm::vec3(0, (0.5 + 0.25)*escala, 0));
		model = glm::scale(model, glm::vec3(0.25*escala, 0.25*escala, 0.25*escala));
		projectionShader.setVec3("aColor", glm::vec3((float)100 / 255, (float)83 / 255, (float)148 / 255));
		projectionShader.setMat4("model", model);
		my_sphere.render();
		glBindVertexArray(VAO);

	}

	//torus, rueda para la fortuna huehuehueue
	model = glm::scale(savestate.at(0), glm::vec3(1.8f*escala, 1.8f*escala, 0.3f*escala));
	//model = glm::translate(model, lightPos);
	projectionShader.setMat4("model", model);
	my_torus.render();

	shader.use();
	model = glm::mat4(1.0f);
	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 0.0f, 2.5f));
	shader.setMat4("model", model);
	pista.Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(0, 0, -600));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 0.0f, 2.5f));
	shader.setMat4("model", model);
	pista2.Draw(shader);
	////////MOUNTAIN CART
	model = glm::rotate(temp3, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(3.0f, 8.0f, 0.0f));
	model = glm::translate(model, glm::vec3(movKit_x, movKit_y, movKit_z));
	model = glm::rotate(model, glm::radians(rotKit_x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotKit_y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotKit_z), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	modelo.Draw(shader);
	///////////////
	//fence bottom start
	model = savestatereja = glm::translate(model = glm::mat4(1.0f), glm::vec3(15, 0, 52));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 6; ind++)
	{
		model = savestatereja = savestatereja2 = glm::translate(savestatereja, glm::vec3(11.3, 0, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = savestatereja = glm::translate(model = glm::mat4(1.0f), glm::vec3(-15, 0, 52));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 6; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(-11.3, 0, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}
	//first_ low left
	model = glm::translate(savestatereja, glm::vec3(-11.3 / 2, 0, 0));
	model = savestatereja = glm::rotate(model, glm::radians(32.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 6; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-130.75, 0, -12.5));
	model = savestatereja = glm::rotate(model, glm::radians(46.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 8; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-205.25, 0, -81.75));
	model = savestatereja = glm::rotate(model, glm::radians(62.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-243.25, 0, -101.5));
	model = savestatereja = glm::rotate(model, glm::radians(73.8f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 9; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}


	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-351.75, 0, -132.25));
	model = savestatereja = glm::rotate(model, glm::radians(84.8f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-396.5, 0, -135.75));
	model = savestatereja = glm::rotate(model, glm::radians(89.9f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-441.5, 0, -135.5));
	model = savestatereja = glm::rotate(model, glm::radians(94.9f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-483.75, 0, -130));
	model = savestatereja = glm::rotate(model, glm::radians(112.6f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 5; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}
	//second-fence low_right
	model = glm::translate(savestatereja2, glm::vec3(11.3 / 2, 0, 0));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 32.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 6; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, 11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(130.75, 0, -12.5));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 46.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 8; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, 11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(205.25, 0, -81.75));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 62.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, 11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(243.25, 0, -101.5));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 73.8f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 9; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, 11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}


	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(351.75, 0, -132.25));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 84.8f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, 11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(396.5, 0, -135.75));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 89.9f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, 11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(441.5, 0, -135.5));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 94.9f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, 11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(483.75, 0, -130));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 112.6f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 5; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, 11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}
	// fence top start

	model = savestatereja = glm::translate(model = glm::mat4(1.0f), glm::vec3(15, 0, -503));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 6; ind++)
	{
		model = savestatereja = savestatereja2 = glm::translate(savestatereja, glm::vec3(11.3, 0, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = savestatereja = glm::translate(model = glm::mat4(1.0f), glm::vec3(-15, 0, -503));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 6; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(-11.3, 0, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}
	//fence left top

	model = glm::translate(savestatereja, glm::vec3(-11.3 / 2, 0, 0));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 32.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 6; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-130.75, 0, 12.5 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 46.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 8; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-205.25, 0, 81.75 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 62.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-243.25, 0, 101.5 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 73.8f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 9; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}


	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-351.75, 0, 132.25 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 84.8f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-396.5, 0, 135.75 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 89.9f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-441.5, 0, 135.5 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 94.9f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-483.75, 0, 130 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 - 112.6f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 5; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}
	//fence right top

	model = glm::translate(savestatereja2, glm::vec3(11.3 / 2, 0, 0));
	model = savestatereja = glm::rotate(model, glm::radians(180 + 32.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 6; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(130.75, 0, 12.5 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 + 46.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 8; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(205.25, 0, 81.75 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 + 62.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(243.25, 0, 101.5 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 + 73.8f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 9; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}


	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(351.75, 0, 132.25 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 + 84.8f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(396.5, 0, 135.75 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 + 89.9f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(441.5, 0, 135.5 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 + 94.9f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 3; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(483.75, 0, 130 - 503 + 52));
	model = savestatereja = glm::rotate(model, glm::radians(180 + 112.6f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	reja.Draw(shader);

	for (int ind = 0; ind < 5; ind++)
	{
		model = savestatereja = glm::translate(savestatereja, glm::vec3(0, 0, -11.3));
		//model = glm::rotate(model, glm::radians(-114.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		shader.setMat4("model", model);
		reja.Draw(shader);
	}

	//tiendas
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(511.25, 0, -151.25));
	model = savestatereja = glm::rotate(model, glm::radians(-111.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(0).Draw(shader); //dagashi 

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(507.5, 0, -297.5));
	model = savestatereja = glm::rotate(model, glm::radians(-69.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(1).Draw(shader); //puesto box

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(137.5, 0, -195));
	model = savestatereja = glm::rotate(model, glm::radians(-164.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(2).Draw(shader); //puesto  rojo

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(72.5, 0, -480));
	model = savestatereja = glm::rotate(model, glm::radians(138.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(3).Draw(shader);//puesto rosa chico

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(518.75, 0, -216.25));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(4).Draw(shader); //puesto tom nook

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(151.25, 0, -248.75));
	model = savestatereja = glm::rotate(model, glm::radians(-43.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(5).Draw(shader);//puesto cafe madera

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-393.75, 0, -302.5));
	model = savestatereja = glm::rotate(model, glm::radians(134.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(6).Draw(shader);//periodico

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-532.5, 0, -226.25));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(7).Draw(shader); //muro

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-82.5, 0, 30));
	model = savestatereja = glm::rotate(model, glm::radians(-49.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	tienda.at(3).Draw(shader);

	//arboles
	//cuadrante pasto derecho bottom
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(168.75, 0, 17.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(233.75, 0, -70.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(300.75, 0, 37.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(338.75, 0, -73.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(438.75, 0, -15.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(203.75, 0, -32.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(273.75, 0, -22.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(436.75, 0, -98.75));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	//pasto izq bot


	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-168.75, 0, 17.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-233.75, 0, -70.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-300.75, 0, 37.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-338.75, 0, -73.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-438.75, 0, -15.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-203.75, 0, -32.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-273.75, 0, -22.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-436.75, 0, -98.75));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	//pasto izq top


	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-168.75, 0, 17.5 - 520));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-233.75, 0, -70.5 - 520));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-300.75, 0, 37.5 - 520));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-338.75, 0, -73.5 - 520));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-438.75, 0, -15.5 - 520));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-203.75, 0, -32.5 - 520));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-273.75, 0, -22.5 - 520));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-436.75, 0, -98.75 - 520));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	//pasto der top


	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(168.75, 0, 17.5 - 450));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(233.75, 0, -70.5 - 450));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(300.75, 0, 37.5 - 450));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(338.75, 0, -73.5 - 450));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(438.75, 0, -15.5 - 450));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(203.75, 0, -32.5 - 450));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(273.75, 0, -22.5 - 450));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(436.75, 0, -98.75 - 450));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(392.75, 0, -375));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(487.75, 0, -336));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	arbol.at(0).Draw(shader);

	//sucurity nigga
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-527.5, 0, -162.5));
	model = savestatereja = glm::rotate(model, glm::radians(-27.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(1).Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-520, 0, -290));
	model = savestatereja = glm::rotate(model, glm::radians(33.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(1).Draw(shader);

	////Anastasia
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(test_pos_x.at(0), 0, test_pos_z.at(0)));
	model = savestatereja = glm::rotate(model, glm::radians(test_grados.at(0)), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(2).Draw(shader);

	//Ayanami
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(test_pos_x.at(1), 0, test_pos_z.at(1)));
	model = savestatereja = glm::rotate(model, glm::radians(test_grados.at(1)), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(3).Draw(shader);

	//BB
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(82, 0, -42));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(4).Draw(shader);

	//futaba
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(225, 0, -116.5));
	model = savestatereja = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(5).Draw(shader);

	//indy
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(133.75, 0, -197.5));
	model = savestatereja = glm::rotate(model, glm::radians(184.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(6).Draw(shader);

	//suika
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(153, 0, -252));
	model = savestatereja = glm::rotate(model, glm::radians(322.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(7).Draw(shader);

	//mai
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-191, 0, -235));
	model = savestatereja = glm::rotate(model, glm::radians(266.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(8).Draw(shader);

	//miri
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-400, 0, -291));
	model = savestatereja = glm::rotate(model, glm::radians(353.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(9).Draw(shader);

	//mochi
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-390, 0, -147));
	model = savestatereja = glm::rotate(model, glm::radians(228.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(10).Draw(shader);

	//puesto heuehe
	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(-168.75, 0, -226.25));
	model = savestatereja = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	tienda.at(8).Draw(shader);

	//puede ser util para encontrar coordenadas de x, z, además de rotaciones en y
	/*model = glm::translate(model = glm::mat4(1.0f), glm::vec3(test_pos_x.at(11), 0, test_pos_z.at(11)));
	model = savestatereja = glm::rotate(model, glm::radians(test_grados.at(11)), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", model);
	tienda.at(8).Draw(shader); */


	//puede ser util para encontrar coordenadas de x, z, además de rotaciones en y
	/*model = glm::translate(model = glm::mat4(1.0f), glm::vec3(test_pos_x2, 0, test_pos_z2));
	model = glm::rotate(model, glm::radians(test_grados.at(11)), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.5f, 6.5f, 6.5f));
	shader.setMat4("model", model);
	gente.at(0).Draw(shader);*/



}

int main()
{
	ISoundEngine * engine = irrklang::createIrrKlangDevice();

	if (!engine)
	{
		printf("Could not startup engine\n");
		return 0; // error starting up the engine
	}

	// To play a sound, we only to call play2D(). The second parameter
	// tells the engine to play it looped.

	// play some sound stream, looped
	engine->play2D("fairground.mp3", true);
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Feria 1.0", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();



	//random colours


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	my_torus.init();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);

	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	//Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	//Shader projectionShader("shaders/shader_light_Gouraud.vs", "shaders/shader_light_Gouraud.fs");
	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");


	Shader modelShader("Shaders/modelLoading.vs", "Shaders/modelLoading.fs");
	// Load models
	Model ourModel = ((char *)"Models/kit2.obj");
	Model pista = ((char *)"Models/pista2.obj");
	Model pista2 = ((char *)"Models/piso3test.obj");
	Model reja = ((char *)"Models/reja/reja.obj");
	std::vector<Model> tienda{ (char *)"Models/tienda1/tienda1.obj" , (char *)"Models/tienda2/tienda2.obj" , (char *)"Models/tienda3/tienda3.obj" ,
		(char *)"Models/tienda4/tienda4.obj" , (char *)"Models/tienda5/tienda5.obj", (char *)"Models/tienda7/tienda7.obj",
		(char *)"Models/tienda6/untitled.obj" , (char *)"Models/muro/muro.obj" , (char *)"Models/puesto/tiendita.obj" };
	std::vector<Model> gente{ (char *)"Models/gente/girl1/untitled.obj", (char *)"Models/gente/nigga/nigga.obj", (char *)"Models/gente/anastasia/anastasia.obj",(char *)"Models/gente/ayanami/ayanami.obj"
		,(char *)"Models/gente/bb/bb.obj",(char *)"Models/gente/futaba/untitled.obj", (char *)"Models/gente/indy/indy.obj",
		(char *)"Models/gente/japanesegoburin/untitled.obj",(char *)"Models/gente/mai/mai.obj",(char *)"Models/gente/miri/miri.obj",(char *)"Models/gente/mochi/mochi.obj" ,(char *)"Models/gente/misaki/misaki.obj" };
	std::vector<Model> arbol{ (char *)"Models/nature/arbol1/arbol1.obj", (char *)"Models/nature/arbol2/arbol1.obj", (char *)"Models/nature/arbol3/arbol3.obj" };
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);

	//std::vector<Model> girlsepa{ (char *)"Models/Personaje/bota.obj", (char *)"Models/Personaje/piernader.obj", (char *)"Models/Personaje/piernaizq.obj", (char *)"Models/Personaje/torso.obj", (char *)"Models/Personaje/brazoder.obj",(char *)"Models/Personaje/brazoizq.obj",(char *)"Models/Personaje/cabeza.obj" };


	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		my_input(window);
		animate();

		// render
		// Backgound color
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(modelShader, projectionShader, ourModel, pista, pista2, reja, tienda, gente, arbol);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	engine->drop(); // delete engine
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		std::cout << "------CoordinatesCamera------" << std::endl;
		std::cout << "Xcamera: " << camera.Position.x << std::endl;
		std::cout << "Ycamera: " << camera.Position.y << std::endl;
		std::cout << "Zcamera: " << camera.Position.z << std::endl;
		std::cout << "Pitch: " << camera.Pitch << std::endl;
		std::cout << "Yaw: " << camera.Yaw << std::endl;
		std::cout << "-----------------------------" << camera.Yaw << std::endl;

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		std::cout << "------CoordinatesCamera------" << std::endl;
		std::cout << "Xcamera: " << camera.Position.x << std::endl;
		std::cout << "Ycamera: " << camera.Position.y << std::endl;
		std::cout << "Zcamera: " << camera.Position.z << std::endl;
		std::cout << "Pitch: " << camera.Pitch << std::endl;
		std::cout << "Yaw: " << camera.Yaw << std::endl;
		std::cout << "-----------------------------" << camera.Yaw << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
		std::cout << "------CoordinatesCamera------" << std::endl;
		std::cout << "Xcamera: " << camera.Position.x << std::endl;
		std::cout << "Ycamera: " << camera.Position.y << std::endl;
		std::cout << "Zcamera: " << camera.Position.z << std::endl;
		std::cout << "Pitch: " << camera.Pitch << std::endl;
		std::cout << "Yaw: " << camera.Yaw << std::endl;
		std::cout << "-----------------------------" << camera.Yaw << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
		std::cout << "------CoordinatesCamera------" << std::endl;
		std::cout << "Xcamera: " << camera.Position.x << std::endl;
		std::cout << "Ycamera: " << camera.Position.y << std::endl;
		std::cout << "Zcamera: " << camera.Position.z << std::endl;
		std::cout << "Pitch: " << camera.Pitch << std::endl;
		std::cout << "Yaw: " << camera.Yaw << std::endl;
		std::cout << "-----------------------------" << camera.Yaw << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		play = true;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		play = false;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		test_grados.at(11) += 2.0;
		std::cout << "Grados: " << test_grados.at(11) << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		recording = true;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		recording = false;
		for (const auto &e : camerapos)
		{
			outFile1 << e.x << "\n";
			outFile1 << e.y << "\n";
			outFile1 << e.z << "\n";
		}
		for (const auto &e : pitchvec) outFile2 << e << "\n";
		for (const auto &e : yawvec) outFile3 << e << "\n";
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		animrecord = true;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		animrecord = false;
		for (const auto &e : a_ayanami)
		{
			outAnim << e.x << "\n";
			outAnim << e.y << "\n";
			outAnim << e.z << "\n";
		}

	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		test_grados.at(11) -= 2.0;
		std::cout << "Grados: " << test_grados.at(11) << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		test_pos_z.at(11) += 1.25;
		std::cout << "PosZ: " << test_pos_z.at(11) << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		test_pos_z.at(11) -= 1.25;
		std::cout << "PosZ: " << test_pos_z.at(11) << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		test_pos_x.at(11) += 1.25;
		std::cout << "PosX: " << test_pos_x.at(11) << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		test_pos_x.at(11) -= 1.25;
		std::cout << "PosX: " << test_pos_x.at(11) << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		std::cout << "-----COORDINATES-----" << std::endl;
		std::cout << "PosX: " << test_pos_x.at(11) << std::endl;
		std::cout << "PosZ: " << test_pos_z.at(11) << std::endl;
		std::cout << "Grados: " << test_grados.at(11) << std::endl;
		std::cout << "---------------------" << std::endl;
	}


	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		if (available)
		{
			available = false;
			load_vectors();
		}
		autoroute = true;
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		autoroute = false;
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		available = true;
	}

	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		if (available2)
		{
			available2 = false;
			load_vectors_anim();
		}
		anim_on = true;
	}

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		anim_on = false;
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		available2 = true;
	}


}


void load_vectors()
{
	std::cout << "Load funcion begins" << std::endl;
	inFile1.clear();
	inFile2.clear();
	inFile3.clear();

	if (inFile1)
	{
		std::cout << "Deque1 load started" << std::endl;
		inFile1.seekg(0, ios::beg);
		camerapos.clear();
		double value, value2, value3;
		while (inFile1 >> value)
		{
			inFile1 >> value2;
			inFile1 >> value3;
			camerapos.push_back(glm::vec3(value, value2, value3));
		}
		std::cout << "Deque1 load finished" << std::endl;
	}

	if (inFile2)
	{
		std::cout << "Deque2 load started" << std::endl;
		inFile2.seekg(0, ios::beg);
		pitchvec.clear();
		double value;
		while (inFile2 >> value)
			pitchvec.push_back(value);
		std::cout << "Deque2 load finished" << std::endl;
	}

	if (inFile3)
	{
		std::cout << "Deque3 load started" << std::endl;
		inFile3.seekg(0, ios::beg);
		yawvec.clear();
		double value;
		while (inFile3 >> value)
			yawvec.push_back(value);
		std::cout << "Deque3 load finished" << std::endl;
	}
	std::cout << "Load funcion ends" << std::endl;
}


void load_vectors_anim()
{
	std::cout << "Load funcion begins" << std::endl;
	inAnim1.clear();
	/*inAnim2.clear();
	inAnim3.clear();
	inAnim4.clear();
	inAnim5.clear();
	inAnim6.clear();
	inAnim7.clear();
	inAnim8.clear();*/

	if (inAnim1)
	{
		std::cout << "Deque1 load started" << std::endl;
		inAnim1.seekg(0, ios::beg);
		a_anastasia.clear();
		double value, value2, value3;
		while (inAnim1 >> value)
		{
			inAnim1 >> value2;
			inAnim1 >> value3;
			a_anastasia.push_back(glm::vec3(value, value2, value3));
		}
		std::cout << "Deque1 load finished" << std::endl;
	}

	if (inAnim2)
	{
		std::cout << "Deque1 load started" << std::endl;
		inAnim2.seekg(0, ios::beg);
		a_ayanami.clear();
		double value, value2, value3;
		while (inAnim2 >> value)
		{
			inAnim2 >> value2;
			inAnim2 >> value3;
			a_ayanami.push_back(glm::vec3(value, value2, value3));
		}
		std::cout << "Deque1 load finished" << std::endl;
	}
	/*
	if (inAnim3)
	{
	std::cout << "Deque1 load started" << std::endl;
	inAnim3.seekg(0, ios::beg);
	a_anastasia.clear();
	double value, value2, value3;
	while (inAnim1 >> value)
	{
	inAnim3 >> value2;
	inAnim3 >> value3;
	a_anastasia.push_back(glm::vec3(value, value2, value3));
	}
	std::cout << "Deque1 load finished" << std::endl;
	}

	if (inAnim1)
	{
	std::cout << "Deque1 load started" << std::endl;
	inAnim1.seekg(0, ios::beg);
	a_anastasia.clear();
	double value, value2, value3;
	while (inAnim1 >> value)
	{
	inAnim1 >> value2;
	inAnim1 >> value3;
	a_anastasia.push_back(glm::vec3(value, value2, value3));
	}
	std::cout << "Deque1 load finished" << std::endl;
	}

	if (inAnim1)
	{
	std::cout << "Deque1 load started" << std::endl;
	inAnim1.seekg(0, ios::beg);
	a_anastasia.clear();
	double value, value2, value3;
	while (inAnim1 >> value)
	{
	inAnim1 >> value2;
	inAnim1 >> value3;
	a_anastasia.push_back(glm::vec3(value, value2, value3));
	}
	std::cout << "Deque1 load finished" << std::endl;
	}

	if (inAnim1)
	{
	std::cout << "Deque1 load started" << std::endl;
	inAnim1.seekg(0, ios::beg);
	a_anastasia.clear();
	double value, value2, value3;
	while (inAnim1 >> value)
	{
	inAnim1 >> value2;
	inAnim1 >> value3;
	a_anastasia.push_back(glm::vec3(value, value2, value3));
	}
	std::cout << "Deque1 load finished" << std::endl;
	}

	if (inAnim1)
	{
	std::cout << "Deque1 load started" << std::endl;
	inAnim1.seekg(0, ios::beg);
	a_anastasia.clear();
	double value, value2, value3;
	while (inAnim1 >> value)
	{
	inAnim1 >> value2;
	inAnim1 >> value3;
	a_anastasia.push_back(glm::vec3(value, value2, value3));
	}
	std::cout << "Deque1 load finished" << std::endl;
	}

	if (inAnim1)
	{
	std::cout << "Deque1 load started" << std::endl;
	inAnim1.seekg(0, ios::beg);
	a_anastasia.clear();
	double value, value2, value3;
	while (inAnim1 >> value)
	{
	inAnim1 >> value2;
	inAnim1 >> value3;
	a_anastasia.push_back(glm::vec3(value, value2, value3));
	}
	std::cout << "Deque1 load finished" << std::endl;
	}
	*/
	std::cout << "Load funcion ends" << std::endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (autoroute)
		return;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
