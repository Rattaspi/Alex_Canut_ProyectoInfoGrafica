#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "..\Shader.hpp"
#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Object.hpp"


using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
glm::mat4 modelMatGen(glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate, float rot);
glm::mat4 LookAt(glm::vec3, glm::vec3 ,glm::vec3, glm::vec3);
void Mouse_Callback(GLFWwindow*, double, double);
void Wheel_Callback(GLFWwindow*, double, double);

float mCoef = 0;
bool rotRight, rotLeft, rotUp, rotDown, fade = false; //controla que siga rotando mientras se mantiene pulsado
float rotX, rotY = 0.0f; //controla el valor de rotacion que se aplicará a la rotacion en la modelMat
float inc = 0.2f; //coeficiente con el cual se incrementa la rotacion

Camera cam(glm::vec3(0, 0, -3), glm::normalize(glm::vec3(0, 0, -3)), 0.04f, 45.0f);

int main() {
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//create a window
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse_Callback);
	glfwSetScrollCallback(window, Wheel_Callback);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}
	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	//set function when callback
	glfwSetKeyCallback(window, key_callback);

	//set windows and viewport
	glViewport(0, 0, screenWithd, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//fondo
	glClearColor(0.f, 0.f, 0.f, 1.0);

	//cargamos los shader
	Shader shader = Shader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");
	glEnable(GL_DEPTH_TEST);

	GLint shaderTrans = glGetUniformLocation(shader.Program, "finalMat");

	//se instancian las dos cajas
	Object movingBox(glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0),glm::vec3(0),Object::cube);
	Object staticBox(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0), glm::vec3(4.f, 0.f, 0.f), Object::cube);

	GLint finalMatID = glGetUniformLocation(shader.Program, "finalMat");

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.USE();
		//MOVIMIENTO DE CAMARA
		cam.DoMovement(window);

		//proyeccion * vista * modelo
		glm::mat4 modelMat, viewMat, projectionMat, finalMat;

		//calculo matriz vista (AQUI VA LA CAMARA)
		viewMat = cam.LookAt();

		movingBox.Update(window);
		modelMat = movingBox.GetModelMatrix();

		//calculo matriz proyeccion
		projectionMat = glm::perspective(glm::radians(cam.GetFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
		
		finalMat = projectionMat*viewMat*modelMat;
		glUniformMatrix4fv(finalMatID, 1, GL_FALSE, glm::value_ptr(finalMat));
		movingBox.Draw();

		//modelMat = staticBox.GetModelMatrix();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

//genera la matriz model
glm::mat4 modelMatGen(glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate, float rot) {
	glm::mat4 model;
	model = glm::translate(model, translate);
	model = glm::rotate(model, glm::radians(rot), rotate);
	model = glm::scale(model, scale);
	return model;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//para el fade de texturas
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		fade = true;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		fade = false;
	}

	//rotacion en el eje Y
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		rotRight = true;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		rotRight = false;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		rotLeft = true;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		rotLeft = false;
	}

	//rotacion en el eje X
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		rotUp = true;
	}
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
		rotUp = false;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		rotDown = true;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
		rotDown = false;
	}
}

void Mouse_Callback(GLFWwindow* window, double xPos, double yPos) {
	cam.MouseMove(window, xPos, yPos);
}

void Wheel_Callback(GLFWwindow* window, double xOffset, double yOffset) {
	cam.MouseScroll(window, xOffset, yOffset);
}