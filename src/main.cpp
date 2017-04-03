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

const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
bool paintQuad=false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
float mCoef = 0;
float degX = 0.f, degY = 0.f;
bool l, r, u, d;

bool fade = false;


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

glm::mat4 GenerateModelMatrix(glm::vec3 aTranslation, glm::vec3 aRotation, glm::vec3 CubesPosition, float aRot) {
	glm::mat4 temp;
	temp = glm::translate(temp, aTranslation);
	temp = glm::translate(temp, CubesPosition);
	temp = glm::rotate(temp, glm::radians(aRot), aRotation);

	return temp;
}

int main() {
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
//TODO
	GLFWwindow* window;



	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		std::cout << "Error al crear la ventana" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
//TODO

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}
	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	//set function when callback
//TODO
		glfwSetKeyCallback(window, key_callback);

	//set windows and viewport
//TODO
		glViewport(0, 0, screenWithd, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);

	//fondo
		glClearColor(0.5f, 0.5f, 0.5f, 1.0);


//TODO


	//cargamos los shader
	Shader s = Shader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");

	// Definir el buffer de vertices
	//Reserva de memoria

	GLfloat VertexBufferObject[] = {
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

	GLuint VAO, VBO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO); {

		glGenBuffers(1, &VBO);
		//Se enlaza el buffer para poder usarlo
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Se pasan los datos
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_DYNAMIC_DRAW);		

		//Propiedades
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 0, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3* sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);


		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}glBindVertexArray(0);



	//reservar memoria para el VAO, VBO y EBO&



	//Establecer el objeto
		//Declarar el VBO y el EBO

		//Enlazar el buffer con openGL

		//Establecer las propiedades de los vertices

		//liberar el buffer
	
	//liberar el buffer de vertices
	

	/*unsigned char* tex2 = SOIL_load_image("texture2.png", &width, &height, 0, SOIL_LOAD_RGB);
	SOIL_free_image_data(tex2);*/

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* tex2 = SOIL_load_image("./src/texture2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex2);
	SOIL_free_image_data(tex2);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glEnable(GL_DEPTH_TEST);

	GLint mixCoef;
	GLint shaderTrans;

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))	{

		glm::mat4 cam;
		glm::mat4 projection;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT);

		mixCoef = glGetUniformLocation(s.Program, "mCoef");
		glUniform1f(mixCoef, mCoef);

		glm::mat4 transMat;

		shaderTrans = glGetUniformLocation(s.Program, "transf");
		glUniformMatrix4fv(shaderTrans, 1, false, value_ptr(transMat));

		s.USE();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(s.Program, "ourTexture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(s.Program, "ourTexture2"), 1);

		glBindVertexArray(VAO);

		if (l) degY -= 1;
		else if (r) degY += 1;

		if (u) degX -= 1;
		else if (d)degX += 1;

		if (fade) {
			if (mCoef >= 0 && mCoef < 1) {
				mCoef += .1f;
			}
		}
		else {
			if (mCoef > .01f) {
				mCoef -= .1f;
			}
		}

		float FOV = 45.f;
		projection = glm::perspective(FOV, (float)(800 / 600), .1f, 100.f);
		cam = glm::translate(cam, glm::vec3(.0f,.0f,-3.f));

		
		for (int i = 0; i < 10; i++) {
			glm::mat4 matriz;
			if (i == 0) {
				matriz = glm::translate(matriz, CubesPositionBuffer[0]);
				matriz = glm::rotate(matriz, glm::radians(degX), glm::vec3(1, 0, 0));
				matriz = glm::rotate(matriz, glm::radians(degY), glm::vec3(0, 1, 0));
			}
			else {
				float timeRotation = (int)(glfwGetTime() * 100) % 360;
				matriz = GenerateModelMatrix(glm::vec3(.0f), glm::vec3(1, .5f, 0), CubesPositionBuffer[i], timeRotation);
			}

			glm::mat4 tranfMat = projection*cam*matriz;
			glUniformMatrix4fv(shaderTrans, 1, GL_FALSE, glm::value_ptr(tranfMat));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO
	glBindTexture(GL_TEXTURE_2D,0);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

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

	if (key == GLFW_KEY_UP&&action == GLFW_PRESS) {
		u = true;
	}
	else if (key == GLFW_KEY_UP&&action == GLFW_RELEASE) {
		u = false;
	}
	else if (key == GLFW_KEY_DOWN&&action == GLFW_PRESS) {
		d = true;
	}
	else if (key == GLFW_KEY_DOWN&&action == GLFW_RELEASE) {
		d = false;
	}

	if (key == GLFW_KEY_1&&action == GLFW_PRESS) {
		fade = true;
	}
	else if (key == GLFW_KEY_2&&action == GLFW_PRESS) {
		fade = false;
	}

}
