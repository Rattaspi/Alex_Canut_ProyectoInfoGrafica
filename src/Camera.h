#pragma once
#include <glm.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Camera {
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
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
	Camera(glm::vec3, glm::vec3, GLfloat, GLfloat);
	void DoMovement(GLFWwindow*);
	void MouseMove(GLFWwindow*,double,double);
	void MouseScroll(GLFWwindow*, double, double);
	glm::mat4 LookAt();
	GLfloat GetFOV();
};

Camera::Camera(glm::vec3 position, glm::vec3 direction, GLfloat sensivility, GLfloat fov) {
	lastFrame = 0;
	cameraPos = position;
	cameraFront = direction;
	this->sensivility = sensivility;
	FOV = fov;

	glm::vec3 directionX = glm::normalize(glm::vec3(0, direction.y, direction.z));
	glm::vec3 z(0, 0, 1);
	glm::vec3 directionY = glm::normalize(glm::vec3(direction.x, 0, direction.z));
	PITCH = 90 - glm::degrees(glm::acos(glm::dot(glm::vec3(0, 1, 0), directionX)));
	YAW = 90 - glm::degrees(glm::acos(glm::dot(directionY, z)));
}