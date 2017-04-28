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
glm::mat4 LookAt(glm::vec3, glm::vec3 ,glm::vec3, glm::vec3);
void Mouse_Callback(GLFWwindow*, double, double);
void Wheel_Callback(GLFWwindow*, double, double);

float mCoef = 0;
float deg = 0;
bool rotRight, rotLeft, rotUp, rotDown, fade = false; //controla que siga rotando mientras se mantiene pulsado
float rotX, rotY = 0.0f; //controla el valor de rotacion que se aplicará a la rotacion en la modelMat
float inc = 0.2f; //coeficiente con el cual se incrementa la rotacion
glm::vec3 cameraPos, cameraFront, cameraUp, cameraRight; //vectores de la camara
float cameraSpeed = 20;
bool movLeft, movRight, movFront, movBack;
bool doOnce = false; //se utiliza en el mouse callback
float pitch, yaw, prevMouseX, prevMouseY;
float sensivilidadMouse = 0.04;
float FOV = 45; //Field of view

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
	glEnable(GL_DEPTH_TEST);

	GLuint VAO, EBO, VBO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO); {

		glGenBuffers(1, &VBO);
		//Se enlaza el buffer para poder usarlo
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Se pasan los datos
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);

		//Propiedades
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
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
	GLint shaderTrans = glGetUniformLocation(s.Program, "finalMat");

	//INICIALIZAR LA COSAS DE LA CAMARA
	cameraPos = glm::vec3(0.0f, 0.0f, -7.0f);

	cameraFront = glm::normalize(glm::vec3(0, 0, 0) - cameraPos);
	glm::vec3 dirX, dirY;
	dirX = glm::normalize(glm::vec3(0, cameraFront.y, cameraFront.z));
	dirY = glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z));

	pitch = 90 - glm::degrees(glm::acos(glm::dot(glm::vec3(0, 1, 0), dirX)));
	yaw = 90 - glm::degrees(glm::acos(glm::dot(dirY, glm::vec3(0,0,1))));
	
	cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0)));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

	float dt, prevT = 0, currT = 0; // esto es para el delta time

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		//Establecer el color de fondo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mixCoef = glGetUniformLocation(s.Program, "mCoef");
		glUniform1f(mixCoef, mCoef); //Se envia al shader el coeficiente del mix	

		s.USE();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(s.Program, "ourTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(s.Program, "ourTexture2"), 1);

		//Controla la rotacion del cubo
		//rotacion sobre el eje Y
		if (rotLeft) {
			rotY += inc;
		}
		else if (rotRight) {
			rotY -= inc;
		}
		//rotacion sobre el eje X
		if (rotUp) {
			rotX -= inc;
		}
		else if (rotDown) {
			rotX += inc;
		}

		//controla el intercambio de la textura
		if (fade) {
			if (mCoef >= 0 && mCoef < 1) {
				mCoef += 0.01f;
			}
		}
		else {
			if (mCoef >0.01f) {
				mCoef -= 0.01f;
			}
		}

		//MOVIMIENTO DE CAMARA
		if (movFront) {
			cameraPos -= cameraFront * cameraSpeed * dt;
		}
		else if (movBack) {
			cameraPos += cameraFront * cameraSpeed * dt;
		}

		if (movRight) {
			cameraPos -= cameraRight * cameraSpeed * dt;
		}
		else if (movLeft) {
			cameraPos += cameraRight * cameraSpeed * dt;
		}

		//CALCULO DELTA TIME
		currT = glfwGetTime();
		dt = currT - prevT;
		prevT = currT;

		//proyeccion * vista * modelo
		glm::mat4 modelMat, viewMat, projectionMat, finalMat;

		//calculo matriz vista (AQUI VA LA CAMARA)
		viewMat = LookAt(cameraRight, cameraUp, cameraFront, cameraPos);

		//calculo matriz proyeccion
		projectionMat = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; i++) {
			//calculo matriz modelo para cada cubo
			if (i == 0) {
				modelMat = glm::translate(modelMat, CubesPositionBuffer[i]);
				modelMat = glm::rotate(modelMat, glm::radians(rotX), glm::vec3(1, 0, 0));
				modelMat = glm::rotate(modelMat, glm::radians(rotY), glm::vec3(0, 1, 0));
			}
			else {
				float rotation = glfwGetTime() * 100;
				rotation = (int)rotation % 360;
				modelMat = modelMatGen(glm::vec3(1.0f), glm::vec3(1.f, 0.5f, 0.f), CubesPositionBuffer[i], rotation);
			}
			//calculo de la matriz final
			finalMat = projectionMat * viewMat * modelMat;
			//se envia la matriz al shader
			glUniformMatrix4fv(shaderTrans, 1, false, value_ptr(finalMat));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		//cout << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << endl;
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

	//movimiento camara horizontal
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		movLeft = true;
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		movLeft = false;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		movRight = true;
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		movRight = false;
	}

	//movimiento camara alante y atras
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		movFront = true;
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		movFront = false;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		movBack = true;
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		movBack = false;
	}
}

void Mouse_Callback(GLFWwindow* window, double xPos, double yPos) {
	double offsetX, offsetY; //coordenadas anteriores y actuales de la posicion del raton
	glm::vec3 front; //nuevo vector front que tendrá nuestra camara
	if (!doOnce) {
		prevMouseX = xPos;
		prevMouseY = yPos;
		doOnce = true;
	}

	offsetX = xPos - prevMouseX;
	offsetY = yPos - prevMouseY;
	offsetX *= sensivilidadMouse;
	offsetY *= sensivilidadMouse;

	yaw += offsetX;
	pitch -= offsetY;
	pitch = glm::clamp(pitch, -89.0f, 89.0f);
	yaw = glm::mod(yaw, 360.f);
	front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0,1,0)));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

	prevMouseX = xPos;
	prevMouseY = yPos;
}

void Wheel_Callback(GLFWwindow* window, double xOffset, double yOffset) {
	if (FOV >= 1.0f && FOV <= 60.f) {
		FOV -= yOffset;
	}
	if (FOV <= 1.0f) {
		FOV = 1.0f;
	}
	else if (FOV >= 60.0f) {
		FOV = 60.0f;
	}
}

glm::mat4 modelMatGen(glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate, float rot) {
	glm::mat4 model;
	model = glm::translate(model, translate);
	model = glm::rotate(model,glm::radians(rot), rotate);
	model = glm::scale(model, scale);
	return model;
}

glm::mat4 LookAt(glm::vec3 right, glm::vec3 up, glm::vec3 front, glm::vec3 pos) {
	glm::mat4 lookAt;
	lookAt[3][3] = 1;

	//lookAt[0][0] = right.x;
	//lookAt[1][0] = right.y;
	//lookAt[2][0] = right.z;

	//lookAt[0][1] = up.x;
	//lookAt[1][1] = up.y;
	//lookAt[2][1] = up.z;

	//lookAt[0][2] = front.x;
	//lookAt[1][2] = front.y;
	//lookAt[2][2] = front.z;

	//lookAt[3][0] = -pos.x;
	//lookAt[3][1] = -pos.y;
	//lookAt[3][2] = -pos.z;

	lookAt[0][0] = right.x;
	lookAt[0][1] = right.y;
	lookAt[0][2] = right.z;

	lookAt[1][0] = up.x;
	lookAt[1][1] = up.y;
	lookAt[1][2] = up.z;

	lookAt[2][0] = front.x;
	lookAt[2][1] = front.y;
	lookAt[2][2] = front.z;

	lookAt[0][3] = -pos.x;
	lookAt[1][3] = -pos.y;
	lookAt[2][3] = -pos.z;

	lookAt = glm::transpose(lookAt);

	return lookAt;
}
