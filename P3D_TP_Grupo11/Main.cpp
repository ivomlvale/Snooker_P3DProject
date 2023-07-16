#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <vector>
#include <string>

#include <Windows.h>
#include <gl\GL.h>

#include <glm/glm.hpp> // vec3, vec4, ivec4, mat4, ...
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective, ...
#include <glm/gtc/type_ptr.hpp> // value_ptr
#include <glm\gtc\matrix_inverse.hpp> // glm::inverseTranspose()


#include "Ball.h"
#include "Camera.h"
#include "LoadShaders.h"
#include "Table.h"
#include "Material.h"

using namespace std;

void init(void);

#define WIDTH 1920
#define HEIGHT 1080

float lastX = WIDTH;

GLuint programa;

//				Position						Direction			
Camera camera(glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

//mesa
Table table;

//material
Material material("Ball1.mtl");


//obj files
string obj_files[15]{
	"Ball1.obj", "Ball2.obj", "Ball3.obj", "Ball4.obj", "Ball5.obj",
	"Ball6.obj", "Ball7.obj", "Ball8.obj", "Ball9.obj", "Ball10.obj",
	"Ball11.obj", "Ball12.obj", "Ball13.obj", "Ball14.obj", "Ball15.obj",
};

Ball ball1(obj_files[0]);
Ball ball2(obj_files[1]);
Ball ball3(obj_files[2]);
Ball ball4(obj_files[3]);
Ball ball5(obj_files[4]);
Ball ball6(obj_files[5]);
Ball ball7(obj_files[6]);
Ball ball8(obj_files[7]);
Ball ball9(obj_files[8]);
Ball ball10(obj_files[9]);
Ball ball11(obj_files[10]);
Ball ball12(obj_files[11]);
Ball ball13(obj_files[12]);
Ball ball14(obj_files[13]);
Ball ball15(obj_files[14]);

//estado da janela
bool windowState = false; // 0 = normal, 1 = fullscreen
bool window_as_changed = false, ambient_active = true, directional_active = false, point_active = false, spot_active = false;

//animacao da bola
bool animation1 = false, animation2 = false;

//inicializacao das funcoes
void keyReceiver(GLFWwindow* window, int key, int scancode, int action, int mods);
void scrollReceiver(GLFWwindow* window, double xoffset, double yoffset);
void mouseReceiver(GLFWwindow* window, double xpos, double ypos);
void ChangeWindow(GLFWwindow* window, GLFWmonitor* monitor, const GLFWvidmode* mode);

glm::vec3 cameraPosition;

int main(void) {

	GLFWwindow* window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "P3D_TP_11", glfwGetPrimaryMonitor(), NULL);

	GLFWmonitor* monitor = glfwGetWindowMonitor(window);
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glEnable(GL_LIGHTING);

	// funçao para chamar o scroll do rato
	glfwSetScrollCallback(window, scrollReceiver);
	// funçao para chamar a posiçao do rato
	glfwSetCursorPosCallback(window, mouseReceiver);
	// funçao para chamar as teclas fisicas do teclado
	glfwSetKeyCallback(window, keyReceiver);

	ball1.Read();
	ball2.Read();
	ball3.Read();
	ball4.Read();
	ball5.Read();
	ball6.Read();
	ball7.Read();
	ball8.Read();
	ball9.Read();
	ball10.Read();
	ball11.Read();
	ball12.Read();
	ball13.Read();
	ball14.Read();
	ball15.Read();

	//mover a bola para posicoes diferentes
	ball2.model_matrix = glm::translate(ball2.model_matrix, glm::vec3(-2.0f, 0.0f, 2.0f));
	ball3.model_matrix = glm::translate(ball3.model_matrix, glm::vec3(4.0f, 0.0f, 7.0f));
	ball4.model_matrix = glm::translate(ball4.model_matrix, glm::vec3(-3.0f, 0.0f, -4.0f));
	ball5.model_matrix = glm::translate(ball5.model_matrix, glm::vec3(-2.0f, 0.0f, -2.0f));
	ball6.model_matrix = glm::translate(ball6.model_matrix, glm::vec3(-1.0f, 0.0f, 5.0f));
	ball7.model_matrix = glm::translate(ball7.model_matrix, glm::vec3(2.0f, 0.0f, -3.0f));
	ball8.model_matrix = glm::translate(ball8.model_matrix, glm::vec3(4.0f, 0.0f, -3.0f));
	ball9.model_matrix = glm::translate(ball9.model_matrix, glm::vec3(-3.0f, 0.0f, 0.0f));
	ball10.model_matrix = glm::translate(ball10.model_matrix, glm::vec3(2.0f, 0.0f, 7.0f));
	ball11.model_matrix = glm::translate(ball11.model_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
	ball12.model_matrix = glm::translate(ball12.model_matrix, glm::vec3(3.0f, 0.0f, 2.0f));
	ball13.model_matrix = glm::translate(ball13.model_matrix, glm::vec3(-1.0f, 0.0f, -4.0f));
	ball14.model_matrix = glm::translate(ball14.model_matrix, glm::vec3(-2.0f, 0.0f, 7.0f));
	ball15.model_matrix = glm::translate(ball15.model_matrix, glm::vec3(1.0f, 0.0f, 4.0f));

	material.LoadMaterial();

	init();

	while (!glfwWindowShouldClose(window)) {

		//funçao para mudar a janela 
		ChangeWindow(window, monitor, mode);

		float posZ = ball1.model_matrix[3][2];

		//animaçao da bola1
		if (animation1 == true && posZ >= -10)
		{
			glm::mat4 rotationMatrix = glm::rotate(ball1.model_matrix, glm::radians(1.0f), glm::vec3(-0.01, 0.0, 0.0));


			glm::mat4  translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.01f));

			ball1.model_matrix = translationMatrix * rotationMatrix;
			posZ = ball1.model_matrix[3][2];
		}

		//animaçao da bola1
		if (animation2 == true && posZ <= 10)
		{
			glm::mat4 rotationMatrix = glm::rotate(ball1.model_matrix, glm::radians(1.0f), glm::vec3(0.01, 0.0, 0.0));


			glm::mat4  translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));

			ball1.model_matrix = translationMatrix * rotationMatrix;
			posZ = ball1.model_matrix[3][2];
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cameraPosition = glm::vec3(glm::inverse(camera.view)[3]);

		//UNIFORMS
		glUniform1i(glGetUniformLocation(programa, "is_table"), true);
		glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "viewPos"), 1, glm::value_ptr(cameraPosition));
		glProgramUniformMatrix4fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));

		//Desenhar as bolas
		ball1.Draw(programa, camera.getVP());
		ball2.Draw(programa, camera.getVP());
		ball3.Draw(programa, camera.getVP());
		ball4.Draw(programa, camera.getVP());
		ball5.Draw(programa, camera.getVP());
		ball6.Draw(programa, camera.getVP());
		ball7.Draw(programa, camera.getVP());
		ball8.Draw(programa, camera.getVP());
		ball9.Draw(programa, camera.getVP());
		ball10.Draw(programa, camera.getVP());
		ball11.Draw(programa, camera.getVP());
		ball12.Draw(programa, camera.getVP());
		ball13.Draw(programa, camera.getVP());
		ball14.Draw(programa, camera.getVP());
		ball15.Draw(programa, camera.getVP());
		table.Draw(programa, camera.getVP());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void init(void) {

	// Shaders
	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "VertShader.vert" },
		{ GL_FRAGMENT_SHADER, "FragShader.frag" },
		{ GL_NONE, NULL }
	};

	//programa
	programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	cameraPosition = glm::vec3(glm::inverse(camera.view)[3]);

	//criar a transposta para usar nos shaders
	glm::mat3 NormalMatrix = glm::inverseTranspose(glm::mat3(camera.getMV()));

	//UNIFORMS

	//Posicao da camera
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "viewPos"), 1, glm::value_ptr(cameraPosition));

	//cor da luz
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "light_color"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

	//fatores de atenuacao
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "constant"), 1.0f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "linear"), 0.09f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "quadratic"), 0.032f);

	//angulos de recorte
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotCutoff"), 0.3f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotOuterCutoff"), 0.2f);

	//Inversa
	glProgramUniformMatrix3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));
	glUniform1i(glGetUniformLocation(programa, "ambient_active"), true);

	material.Init(programa);
	table.Init(programa, camera.getVP());

	ball1.Send(programa, camera.getVP());
	ball2.Send(programa, camera.getVP());
	ball3.Send(programa, camera.getVP());
	ball4.Send(programa, camera.getVP());
	ball5.Send(programa, camera.getVP());
	ball6.Send(programa, camera.getVP());
	ball7.Send(programa, camera.getVP());
	ball8.Send(programa, camera.getVP());
	ball9.Send(programa, camera.getVP());
	ball10.Send(programa, camera.getVP());
	ball11.Send(programa, camera.getVP());
	ball12.Send(programa, camera.getVP());
	ball13.Send(programa, camera.getVP());
	ball14.Send(programa, camera.getVP());
	ball15.Send(programa, camera.getVP());

	glEnable(GL_DEPTH_TEST);
}

// scrollReceiver para receber o movimento do scroll
void scrollReceiver(GLFWwindow* window, double xoffset, double yoffset) {
	
	
	camera.SetZoom(yoffset);
}

// mouseReceiver para obter a localização do rato na janela e assim rodar o objeto dependendo da posição do rato
void mouseReceiver(GLFWwindow* window, double xpos, double ypos) {
	
	float sensivity = 0.1f;	

	float x_offset = xpos - lastX;

	lastX = xpos;

	x_offset *= sensivity;

	if (x_offset > 0) {

		camera.angle += glm::radians(0.5f);
		camera.update();

	}

	if (x_offset < 0) {

		camera.angle -= glm::radians(0.5f);
		camera.update();
	}
}

// funçao para reconhecer as teclas do teclado e quando estas são pressionadas fazerem algo em especifico
void keyReceiver(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	// ****************************************************
	// Janela
	// ****************************************************
	// recolhe um apontador do janela 
	GLclampf* myColor = (GLclampf*)glfwGetWindowUserPointer(window);

	// tecla f1 para fullscreen
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		windowState = true;
		window_as_changed = true;
	}

	//tecla f2 para windowed
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		windowState = false;
		window_as_changed = true;
	}

	// tecla esc par sair da janela 
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//FONTES DE LUZES

	//Ambiente
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if (ambient_active == true) {
			glUniform1i(glGetUniformLocation(programa, "ambient_active"), false);
			ambient_active = false;
		}
		else
		{ 
			glUniform1i(glGetUniformLocation(programa, "ambient_active"), true);
			ambient_active = true;
		}
		
	}
	//Direcional
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		if (directional_active == true) {
			glUniform1i(glGetUniformLocation(programa, "directional_active"), false);
			directional_active = false;
		}
		else
		{
			glUniform1i(glGetUniformLocation(programa, "directional_active"), true);
			directional_active = true;
		}
	}
	//Pontual
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		if (point_active == true) {
			glUniform1i(glGetUniformLocation(programa, "point_active"), false);
			point_active = false;
		}
		else
		{
			glUniform1i(glGetUniformLocation(programa, "point_active"), true);
			point_active = true;
		}
	}
	//Conica
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		if (spot_active == true) {
			glUniform1i(glGetUniformLocation(programa, "spot_active"), false);
			spot_active = false;
		}
		else
		{
			glUniform1i(glGetUniformLocation(programa, "spot_active"), true);
			spot_active = true;
		}
	}
	//Animacao da bola
	//andar para a frente
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		
		animation1 = true;
		animation2 = false;
	}
	//andar para tras
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {

		animation2 = true;
		animation1 = false;
	}
	//parar a bola
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {

		animation1 = false;
		animation2 = false;
	}

}

//Muda a janela 
void ChangeWindow(GLFWwindow* window, GLFWmonitor* monitor, const GLFWvidmode* mode) {

	if (window_as_changed == true)
	{

		//FullScreen
		if (windowState == true)
		{
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			window_as_changed = false;
		}

		//Window
		if (windowState == false)
		{
			glfwSetWindowMonitor(window, NULL, 50, 50, WIDTH, HEIGHT, 0);
			window_as_changed = false;
		}
	}
}

