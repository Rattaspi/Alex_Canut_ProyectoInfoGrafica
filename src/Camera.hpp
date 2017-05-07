#pragma once
#include <glm.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Camera {
	
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
	glm::vec3 cameraRight;
	GLfloat deltaTime;
	GLfloat lastFrame;
	GLfloat lastMX;
	GLfloat lastMY;
	GLfloat sensivility;
	GLboolean firstMouse;
	GLfloat PITCH;
	GLfloat YAW;
	GLfloat FOV;

public:
	glm::vec3 cameraPos;
	Camera(glm::vec3, glm::vec3, GLfloat, GLfloat);
	void DoMovement(GLFWwindow*);
	void MouseMove(GLFWwindow*,double,double);
	void MouseScroll(GLFWwindow*, double, double);
	glm::mat4 LookAt();
	GLfloat GetFOV();
	glm::vec3 GetPosition();
};

glm::vec3 Camera::GetPosition() {
	return cameraPos;
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, GLfloat sensivility, GLfloat fov) {
	lastFrame = 0;
	cameraPos = position;
	cameraFront = glm::normalize(direction);
	cameraRight = glm::cross(glm::vec3(0, 1, 0), cameraFront);
	this->sensivility = sensivility;
	FOV = fov;

	glm::vec3 directionX = glm::normalize(glm::vec3(0, direction.y, direction.z));
	glm::vec3 z(0, 0, 1);
	glm::vec3 directionY = glm::normalize(glm::vec3(direction.x, 0, direction.z));
	PITCH = 90 - glm::degrees(glm::acos(glm::dot(glm::vec3(0, 1, 0), directionX)));
	YAW = 90 - glm::degrees(glm::acos(glm::dot(directionY, z)));
}

void Camera::DoMovement(GLFWwindow* window) {
	deltaTime = glfwGetTime() - lastFrame;
	lastFrame = glfwGetTime();
	float camSpeed = 5;

	//inputs para controlar el movimiento de la camara
	bool back = glfwGetKey(window, GLFW_KEY_W);
	bool front = glfwGetKey(window, GLFW_KEY_S);
	bool left = glfwGetKey(window, GLFW_KEY_A);
	bool right = glfwGetKey(window, GLFW_KEY_D);

	//movimiento de camara
	if (back) {
		cameraPos -= cameraFront*camSpeed*deltaTime;
	}
	else if (front) {
		cameraPos += cameraFront*camSpeed*deltaTime;
	}

	if (left) {
		cameraPos -= cameraRight*camSpeed*deltaTime;
	}
	else if (right) {
		cameraPos += cameraRight*camSpeed*deltaTime;
	}
}

void Camera::MouseMove(GLFWwindow* window, double xPos, double yPos) {
	static double offsetX, offsetY;

	//la primera vez se inician las primeras coordenadas del raton para tener
	//una primera referencia
	if (!firstMouse) {
		lastMX = xPos;
		lastMY = yPos;
		firstMouse = true;
	}

	offsetX = (xPos - lastMX)*sensivility;
	offsetY = (yPos - lastMY)*sensivility;
	YAW += offsetX;
	PITCH += offsetY;
	//ajustar los valores de PITCH y YAW para que no se vayan de madre
	PITCH = glm::clamp(PITCH, -89.f, 89.f);
	YAW = glm::mod(YAW, 360.f);

	//calcular el nuevo vector front a partir del PITCH y YAW
	glm::vec3 front;
	front.x = glm::cos(glm::radians(YAW)) * glm::cos(glm::radians(PITCH));
	front.y = glm::sin(glm::radians(PITCH));
	front.z = glm::sin(glm::radians(YAW)) * glm::cos(glm::radians(PITCH));
	cameraFront = glm::normalize(front);
	//nuevo vector right
	cameraRight = glm::normalize(glm::cross(glm::vec3(0, 1, 0), cameraFront));

	lastMX = xPos;
	lastMY = yPos;
}

void Camera::MouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
	FOV -= yScroll;
	FOV = glm::clamp(FOV, 1.f, 60.f);
}

glm::mat4 Camera::LookAt() {
	glm::mat4 lookAt, transl;
	glm::vec3 up = glm::normalize(glm::cross(cameraFront, cameraRight));

	lookAt[3][3] = 1;

	lookAt[0][0] = cameraRight.x;
	lookAt[0][1] = cameraRight.y;
	lookAt[0][2] = cameraRight.z;

	lookAt[1][0] = up.x;
	lookAt[1][1] = up.y;
	lookAt[1][2] = up.z;

	lookAt[2][0] = cameraFront.x;
	lookAt[2][1] = cameraFront.y;
	lookAt[2][2] = cameraFront.z;

	lookAt = glm::transpose(lookAt);

	transl[3][0] = -cameraPos.x;
	transl[3][1] = -cameraPos.y;
	transl[3][2] = -cameraPos.z;
	
	lookAt = lookAt * transl;

	return lookAt;
}

GLfloat Camera::GetFOV() {
	return FOV;
}
