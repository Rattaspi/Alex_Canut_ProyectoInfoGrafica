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
