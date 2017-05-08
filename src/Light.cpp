#include "Light.h"

Light::Light(vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, LType lType, int number) {
	switch (lType) {
	case DIRECTIONAL:
		SetPosition(pos);
		SetDirection(dir);
		break;
	case POINT:
		SetPosition(pos);
		break;
	case SPOT:
		SetPosition(pos);
		SetDirection(dir);
		break;
	default:
		break;
	}

	Lambient = ambient;
	Ldiffuse = diffuse;
	Lspecular = specular;
	LightType = lType;
	lightNumber = number;
}


void Light::SetAtt(float constant, float lineal, float quadratic) {
	c1 = constant;
	c2 = lineal;
	c3 = quadratic;
}

void Light::SetAperture(float min, float max) {
	MinAperture = min;
	MaxAperture = max;
}

void Light::SetLight(Shader *shad, vec3 CamPos) {
	std::string variable;

	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);
	switch (LightType){
	case DIRECTIONAL:
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.luzAmbiental"), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.luzDifusa"), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.luzEspecular"), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.pos"), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.dir"), Ldirection.x, Ldirection.y, Ldirection.z);
		break;
	case POINT:		
		variable = "plight[" + std::to_string(lightNumber) + "]";		
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".luzAmbiental").c_str()), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".luzDifusa").c_str()), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".luzEspecular").c_str()), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".pos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".luzAmbiental").c_str()), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".luzDifusa").c_str()), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".luzEspecular").c_str()), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".pos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".dir").c_str()), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".aperturaMax").c_str()), MaxAperture);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".aperturaMin").c_str()), MinAperture);
		break;
	default:
		break;
	}	
}

void Light::Rotate(vec3 rotation) {	
//opcional
}

void Light::SetDirection(vec3 dir) {
	Ldirection = dir;
}

void Light::SetPosition(vec3 pos) {
	Lpos = pos;
}

vec3 Light::GetColor() {
	return vec3(0);
}

Light::~Light() {}