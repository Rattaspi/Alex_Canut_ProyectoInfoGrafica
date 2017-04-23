#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "..\Shader.h"
#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
bool paintQuad=false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
glm::mat4 modelMatGen(glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate, float rot);
glm::mat4 LookAt();
float mCoef = 0;
float deg = 0;
bool l, r;

void DrawVao(GLuint programID,GLuint VAO) {
	//establecer el shader
	glUseProgram(programID);

	//pitar el VAO
	glBindVertexArray(VAO);
	if (!paintQuad) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

}
int main() {
	glEnable(GL_DEPTH_TEST);
	
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);
//TODO

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
		glClear(GL_COLOR_BUFFER_BIT);

	//fondo
		glClearColor(0.5f, 0.5f, 0.5f, 1.0);

	//cargamos los shader
	Shader s = Shader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");

	// Definir el buffer de vertices
	//Reserva de memoria
	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 CubesPositionBuffer[] = {
		glm::vec3(0.0f ,  0.0f,  0.0f),
		glm::vec3(2.0f ,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f , -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f , -2.0f, -2.5f),
		glm::vec3(1.5f ,  2.0f, -2.5f),
		glm::vec3(1.5f ,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	GLuint VAO, EBO, VBO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO); {

		glGenBuffers(1, &VBO);
		//Se enlaza el buffer para poder usarlo
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Se pasan los datos
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_DYNAMIC_DRAW);

		//Propiedades
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}glBindVertexArray(0);

	//CREAR TEXTURAS
	GLuint texture1, texture2;
	int width, height;
	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* tex1 = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
	SOIL_free_image_data(tex1);
	glBindTexture(GL_TEXTURE_2D, 0);

	//TEXTURA 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* tex2 = SOIL_load_image("./src/texture2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex2);
	SOIL_free_image_data(tex2);
	glBindTexture(GL_TEXTURE_2D, 0);
	

	GLint mixCoef;
	GLint shaderTrans;

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT);

		if (l) deg += 1;
		else if (r) deg -= 1;

		mixCoef = glGetUniformLocation(s.Program, "mCoef");
		glUniform1f(mixCoef, mCoef);

		float FOV = 60;

		//proyeccion * vista * modelo
		glm::mat4 modelMat, viewMat, projectionMat, finalMat;

		//calculo matriz modelo
		modelMat = glm::translate(modelMat, glm::vec3(0.f, -0.5f, 0.f));
		modelMat = glm::rotate(modelMat, glm::radians(50.f), glm::vec3(1, 0, 1));
		
		//calculo matriz vista
		viewMat = glm::translate(viewMat, glm::vec3(0.f, 0.f, 0.3f));
		//viewMat = glm::rotate(viewMat, glm::radians(180.f), glm::vec3(0,1,0));
		//UTILIZAR UN LOOKAT

		//calculo matriz proyeccion
		//projectionMat = glm::perspective(glm::radians(FOV), (GLfloat)(WIDTH/HEIGHT), 0.1f, 100.f);

		finalMat = modelMat * viewMat * projectionMat/* * viewMat * modelMat*/;

		shaderTrans = glGetUniformLocation(s.Program, "finalMat");
		glUniformMatrix4fv(shaderTrans, 1, false, value_ptr(finalMat));

		s.USE();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(s.Program, "ourTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(s.Program, "ourTexture2"), 1);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO
	glBindTexture(GL_TEXTURE_2D,0);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS) {
		
		if (mCoef < 1.f) {
			mCoef += 0.1f;
		}
	}
	else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS) {
		
		if (mCoef > 0.1f) {
			mCoef -= 0.1f;
		}
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		r = true;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		r = false;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		l = true;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		l = false;
	}
}

glm::mat4 modelMatGen(glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate, float rot) {
	glm::mat4 model;
	model = glm::translate(model, translate);
	model = glm::rotate(model,glm::radians(rot), rotate);
	model = glm::scale(model, scale);
}
