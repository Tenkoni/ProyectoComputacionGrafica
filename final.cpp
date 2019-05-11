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

#include "esfera.h"
#include "toroide.h"
#include <vector>


// Other Libs
#include "SOIL2/SOIL2.h"

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
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

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
fortuna_rot = 0.0f;

int estado = 0;
bool play, reversa = false;

//savestates_fortuna
std::vector < glm::mat4 > savestate(15); 
std::vector < glm::mat4 > savestate_adorno(15);

//factor de escala fortuna
float escala = 15.0f;

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
	if (play)
	{
		fortuna_rot += 0.2f; //rueda de la fortuna

	}

}

void display(Shader shader, Shader projectionShader, Model modelo, Model pista)
{
	//shader.use();

	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPosition);
	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
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
	//Soporte1
	model = savestate.at(0) = glm::translate(savestate.at(0) = glm::mat4(1.0f), glm::vec3(9.546*escala/2, 8.9 /2*escala, 4 * escala / 1.5 *2.3));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5*escala, 13.5*escala, 0.5*escala));
	projectionShader.setVec3("aColor", glm::vec3((float)100 / 255, (float)83 / 255, (float)148 / 255));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Soporte2
	model = savestate.at(0) = glm::translate(savestate.at(0), glm::vec3(-13.5 * escala * cos(45 * M_PI / 180) + 0.25, 0, 0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(0.5*escala, 13.5 * escala, 0.5*escala));
	projectionShader.setVec3("aColor", glm::vec3((float)100 / 255, (float)83 / 255, (float)148 / 255));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Soporte3
	model = savestate.at(0) = glm::translate(savestate.at(0) = glm::mat4(1.0f), glm::vec3(9.546*escala / 2, 8.9 / 2 * escala, -4*escala/1.5 + 4 * escala / 1.5 * 2.3));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5*escala, 13.5 * escala, 0.5*escala));
	projectionShader.setVec3("aColor", glm::vec3((float)100 / 255, (float)83 / 255, (float)148 / 255));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Soporte4
	model = savestate.at(0) = glm::translate(savestate.at(0), glm::vec3(-13.5 * escala * cos(45 * M_PI / 180) + 0.25, 0, 0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(0.5*escala, 13.5 * escala, 0.5*escala));
	projectionShader.setVec3("aColor", glm::vec3((float)100 / 255, (float)83 / 255, (float)148 / 255));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Eje
	model = glm::translate(savestate.at(0) = glm::mat4(1.0f), glm::vec3(-13.5 * escala * cos(45 * M_PI / 180) / 2 + 9.546*escala / 2, (13.5 * escala * cos(45 * M_PI / 180) / 2) + (8.9 / 2 )*escala, -2*escala/1.5+ 4 * escala / 1.5 * 2.3));
	model = savestate.at(0) = glm::rotate(model, glm::radians(fortuna_rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.25*escala, 0.25*escala, 2.25*escala/1.5));
	projectionShader.setVec3("aColor", glm::vec3((float)100 / 255, (float)83 / 255, (float)148 / 255));
	projectionShader.setMat4("model", model);
	my_sphere.render();
	glBindVertexArray(VAO);

	for (int ind = 0; ind < 12; ind++)
	{
		//Viga30d
		model = savestate.at(ind + 1) = glm::rotate(savestate.at(0), glm::radians(ind*30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = savestate.at(ind + 1) = glm::translate(savestate.at(ind + 1), glm::vec3(0, (8.0*escala) / 2, 0));
		model = glm::scale(model, glm::vec3(0.25*escala, 8.0*escala, 0.25*escala));
		projectionShader.setVec3("aColor", glm::vec3((float)100 / 255, (float)83 / 255, (float)148 / 255));
		projectionShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//cabina
		model = glm::translate(savestate.at(ind + 1), glm::vec3(0, (8.0*escala) / 2, 0));
		model = savestate_adorno.at(ind) = glm::rotate(model, glm::radians(180.0f) - glm::radians(fortuna_rot) - glm::radians(ind*30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5*escala, 0.5*escala, 0.5*escala));
		projectionShader.setVec3("aColor", glm::vec3((float)100 / 255, (float)83 / 255, (float)148 / 255));
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
	model = glm::scale(model, glm::vec3(2.5f, 0.5f, 2.5f));
	shader.setMat4("model", model);
	pista.Draw(shader);

	model = glm::translate(model = glm::mat4(1.0f), glm::vec3(movKit_x, movKit_y, movKit_z));
	model = glm::rotate(model, glm::radians(rotKit_y), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo.Draw(shader);





}

int main()
{
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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 11", NULL, NULL);
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


	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
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

		display(modelShader, projectionShader, ourModel, pista);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		play = true;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		play = false;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		reversa = false;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		reversa = true;

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
