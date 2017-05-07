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
glm::mat4 LookAt(glm::vec3, glm::vec3, glm::vec3, glm::vec3);
void Mouse_Callback(GLFWwindow*, double, double);
void Wheel_Callback(GLFWwindow*, double, double);

float mCoef = 0;
bool rotRight, rotLeft, rotUp, rotDown, fade = false; //controla que siga rotando mientras se mantiene pulsado
float rotX, rotY = 0.0f; //controla el valor de rotacion que se aplicará a la rotacion en la modelMat
float inc = 0.2f; //coeficiente con el cual se incrementa la rotacion

Camera cam(glm::vec3(0, 0, 3), glm::normalize(glm::vec3(0, 0, 3)), 0.04f, 45.0f);

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
	Shader phongShader = Shader("./src/phongVertex.vertexshader", "./src/phongFragment.fragmentshader");
	Shader directionalShader = Shader("./src/phongVertex.vertexshader", "./src/directionalLight.fragmentshader");
	Shader pointShader = Shader("./src/phongVertex.vertexshader", "./src/pointLight.fragmentshader");
	Shader focalShader = Shader("./src/phongVertex.vertexshader", "./src/focalLight.fragmentshader");

	glEnable(GL_DEPTH_TEST);

	//se instancian las dos cajas
	Object movingBox(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0), glm::vec3(0), Object::cube);
	Object staticBox(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0), glm::vec3(4.f, 0.f, 0.f), Object::cube);

	GLint finalMatID = glGetUniformLocation(shader.Program, "finalMat");
	int shaderSelected = 3;

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//MOVIMIENTO DE CAMARA
		cam.DoMovement(window);

		//proyeccion * vista * modelo
		glm::mat4 modelMat, viewMat, projectionMat, finalMat;

		//calculo matriz vista (AQUI VA LA CAMARA)
		viewMat = cam.LookAt();

		movingBox.Update(window);


		//calculo matriz proyeccion
		projectionMat = glm::perspective(glm::radians(cam.GetFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);

		//se pintan los dos cubos
		//cubo que se va a iluminar y se puede mover
		//cambio de shader de iluminacion
		if (glfwGetKey(window, GLFW_KEY_1)) shaderSelected = 1;
		else if (glfwGetKey(window, GLFW_KEY_2)) shaderSelected = 2;
		else if (glfwGetKey(window, GLFW_KEY_3)) shaderSelected = 3;
		else if (glfwGetKey(window, GLFW_KEY_0)) shaderSelected = 0;

		if (shaderSelected == 1) {
			directionalShader.USE();
		}
		else if (shaderSelected == 2) {
			pointShader.USE();
		}
		else if (shaderSelected == 3) {
			focalShader.USE();
		}
		else {
			phongShader.USE();
		}
		//matrices de transformacion
		modelMat = movingBox.GetModelMatrix();
		finalMat = projectionMat*viewMat*modelMat;
		glUniformMatrix4fv(glGetUniformLocation(phongShader.Program, "finalMat"), 1, GL_FALSE, glm::value_ptr(finalMat));
		glUniformMatrix4fv(glGetUniformLocation(directionalShader.Program, "finalMat"), 1, GL_FALSE, glm::value_ptr(finalMat));
		glUniformMatrix4fv(glGetUniformLocation(pointShader.Program, "finalMat"), 1, GL_FALSE, glm::value_ptr(finalMat));
		glUniformMatrix4fv(glGetUniformLocation(focalShader.Program, "finalMat"), 1, GL_FALSE, glm::value_ptr(finalMat));

		//luces
		//AMBIENTAL
		float intensidadAmbiental = 0.1;
		float coeficienteReflexionAmbiental = 1.f;
		glm::vec3 colorAmbiental(1, 1, 1);
		glm::vec3 luzAmbiental = intensidadAmbiental * coeficienteReflexionAmbiental * colorAmbiental;
		//DIFUSA
		glm::vec3 incidenciaLuz = glm::normalize(glm::vec3(-1, -1, 0));
		float intensidadDifusa = 0.8f;
		float coeficienteReflexionDifuso = 0.7f;
		glm::vec3 luzDifusa = intensidadDifusa * coeficienteReflexionDifuso * glm::vec3(1, 1, 1);
		//ESPECULAR
		float intensidadEspecular = 0.2f;
		float coeficienteReflexionEspecular = 0.2f;
		glm::vec3 luzEspecular = intensidadEspecular * coeficienteReflexionEspecular * glm::vec3(1, 1, 1);
		float roughness = 230.0f;

		//atenuacion, se utiliza en phong
		float c1, c2, c3;
		c1 = 1.0f;
		c2 = 0.045;
		c3 = 0.0075f;
	
		float factorAtenuacion = 1 / (1 + c2*(incidenciaLuz.length()) + c3*(incidenciaLuz.length() * incidenciaLuz.length()));

		//componentes del foco
		glm::vec3 focoFront(-1, 0, 0);

		glm::vec3 focusPos = staticBox.GetPosition();

		//enviar datos de iluminacion al shader de phong
		glUniformMatrix4fv(glGetUniformLocation(phongShader.Program, "modelMat"),1, GL_FALSE, glm::value_ptr(movingBox.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(phongShader.Program, "luzAmbiental"), luzAmbiental.x, luzAmbiental.y, luzAmbiental.z);
		glUniform3f(glGetUniformLocation(phongShader.Program, "cameraPos"), cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
		glUniform3f(glGetUniformLocation(phongShader.Program, "incidenciaLuz"), incidenciaLuz.x, incidenciaLuz.y, incidenciaLuz.z);
		glUniform1f(glGetUniformLocation(phongShader.Program, "intensidadDifusa"), intensidadDifusa);
		//glUniform1f(glGetUniformLocation(phongShader.Program, "coeficienteReflexionDifuso"), coeficienteReflexionDifuso);
		glUniform1f(glGetUniformLocation(phongShader.Program, "intensidadEspecular"), intensidadEspecular);
		glUniform1f(glGetUniformLocation(phongShader.Program, "coeficienteReflexionEspecular"), coeficienteReflexionEspecular);
		glUniform1f(glGetUniformLocation(phongShader.Program, "roughness"), roughness);
		glUniform1f(glGetUniformLocation(phongShader.Program, "atenuacion"), factorAtenuacion);
		
		//enviar datos de iluminacion al shader de luz direccional
		glUniformMatrix4fv(glGetUniformLocation(directionalShader.Program, "modelMat"), 1, GL_FALSE, glm::value_ptr(movingBox.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(phongShader.Program, "luzAmbiental"), luzAmbiental.x, luzAmbiental.y, luzAmbiental.z);
		glUniform3f(glGetUniformLocation(directionalShader.Program, "direccionLuz"), incidenciaLuz.x, incidenciaLuz.y, incidenciaLuz.z);
		glUniform3f(glGetUniformLocation(directionalShader.Program, "camPos"), cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
		glUniform3f(glGetUniformLocation(directionalShader.Program, "focusPos"), focusPos.x, focusPos.y, focusPos.z);
		glUniform1f(glGetUniformLocation(directionalShader.Program, "roughness"), roughness);
		glUniform1f(glGetUniformLocation(directionalShader.Program, "luzDifusa"), intensidadDifusa);
		glUniform1f(glGetUniformLocation(directionalShader.Program, "luzEspecular"), intensidadEspecular);

		//enviar datos de iluminacion al shader de luz focal
		glUniformMatrix4fv(glGetUniformLocation(focalShader.Program, "modelMat"), 1, GL_FALSE, glm::value_ptr(movingBox.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(pointShader.Program, "focoPos"), focusPos.x, focusPos.y, focusPos.z);
		glUniform3f(glGetUniformLocation(pointShader.Program, "luzDifusa"), luzDifusa.x, luzDifusa.y, luzDifusa.z);
		glUniform3f(glGetUniformLocation(pointShader.Program, "luzAmbiental"), luzAmbiental.x, luzAmbiental.y, luzAmbiental.z);
		glUniform3d(glGetUniformLocation(pointShader.Program, "luzEspecular"), luzEspecular.x, luzEspecular.y, luzEspecular.z);
		glUniform3f(glGetUniformLocation(pointShader.Program, "camPos"), cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
		glUniform1d(glGetUniformLocation(pointShader.Program, "roughness"), roughness);
		glUniform3f(glGetUniformLocation(focalShader.Program, "direccionFoco"), focoFront.x, focoFront.y, focoFront.z);
		glUniform1f(glGetUniformLocation(focalShader.Program, "aperturaMax"), cos(radians(45.0f)));
		glUniform1f(glGetUniformLocation(focalShader.Program, "aperturaMin"), cos(radians(20.0f)));


		//enviar datos de iluminacion al shader de luz puntual
		glUniformMatrix4fv(glGetUniformLocation(pointShader.Program, "modelMat"), 1, GL_FALSE, glm::value_ptr(movingBox.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(pointShader.Program, "focoPos"), focusPos.x, focusPos.y, focusPos.z);
		glUniform3f(glGetUniformLocation(pointShader.Program, "luzDifusa"), luzDifusa.x, luzDifusa.y, luzDifusa.z);
		glUniform3f(glGetUniformLocation(pointShader.Program, "luzAmbiental"), luzAmbiental.x, luzAmbiental.y, luzAmbiental.z);
		glUniform3d(glGetUniformLocation(pointShader.Program, "luzEspecular"), luzEspecular.x, luzEspecular.y, luzEspecular.z);
		glUniform3f(glGetUniformLocation(pointShader.Program, "camPos"), cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
		glUniform1d(glGetUniformLocation(pointShader.Program, "roughness"), roughness);
		//glUniform1f(glGetUniformLocation(pointShader.Program, "atenuacion"), factorAtenuacion);

		
		
		//se pinta la caja
		movingBox.Draw();

		//cubo estatico
		shader.USE();
		modelMat = staticBox.GetModelMatrix();
		finalMat = projectionMat*viewMat*modelMat;
		glUniformMatrix4fv(finalMatID, 1, GL_FALSE, glm::value_ptr(finalMat));
		staticBox.Draw();

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