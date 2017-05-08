#version 330 core

#define NUM_MAX_PLIGHTS 2
#define NUM_MAX_SLIGHTS 2

struct Material {
	sampler2D TexturaDifusa;
	sampler2D TexturaEspecular;
	float shininess;
};  

struct PLight {
	vec3 luzAmbiental;
	vec3 luzDifusa;
	vec3 luzEspecular;
	vec3 pos;
};

struct DLight {
	vec3 luzAmbiental;
	vec3 luzDifusa;
	vec3 luzEspecular;
	vec3 pos;
	vec3 dir;
};

struct SLight {
	vec3 luzAmbiental;
	vec3 luzDifusa;
	vec3 luzEspecular;
	vec3 pos;
	vec3 dir;
	float aperturaMax;
	float aperturaMin;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material mat;
uniform DLight dlight;
uniform PLight plight[NUM_MAX_PLIGHTS];
uniform SLight slight[NUM_MAX_SLIGHTS];

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection);
vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection);
vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection);

void main(){
	vec3 viewPortD = normalize(viewPos - FragPos);
	color = vec4(DirectionalLight(dlight,Normal, viewPortD), 1);
	for(int i = 0; i <NUM_MAX_PLIGHTS; i++){
		color += vec4(PointLight(plight[i],Normal,viewPortD), 1);
	}
	for(int i = 0; i < NUM_MAX_SLIGHTS; i++){
		color += vec4(SpotLight(slight[i], Normal, viewPortD), 1);
	}
} 

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection){
	vec3 finalColor;
	vec3 ambient = vec3(texture(mat.TexturaDifusa, TexCoords)) * light.luzAmbiental;

	vec3 incidenciaLuzNormal = normalize(-light.dir);
	float diff = max(dot(normalize(Normal.xyz), incidenciaLuzNormal), 0.0);
	vec3 R = reflect(-incidenciaLuzNormal, normalize(Normal.xyz));
	float esp = pow(max(dot(R, viewDirection), 0.0), mat.shininess);
	vec3 diffuse = light.luzDifusa * diff * vec3(texture(mat.TexturaEspecular, TexCoords));
	vec3 specular = light.luzEspecular * esp * vec3(texture(mat.TexturaEspecular, TexCoords));

	finalColor = ambient + diffuse + specular;
	return finalColor;
}

vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection){
	vec3 finalColor;
	vec3 ambient = vec3(texture(mat.TexturaDifusa, TexCoords)) * light.luzAmbiental;

	vec3 lightDir = normalize(light.pos - FragPos);
	float diff = max(dot(normalize(Normal), lightDir), 0.0);
	vec3 diffuse = light.luzDifusa * diff * vec3(texture(mat.TexturaDifusa, TexCoords));

	vec3 R = reflect(-lightDir, normalize(Normal));
	float esp = pow(max(dot(R, viewDirection), 0.0), mat.shininess);
	vec3 specular = light.luzEspecular * esp * vec3(texture(mat.TexturaEspecular, TexCoords));

	float d = length(light.pos - FragPos);
	float c1, c2, c3;
	c1 = 1.0f;
	c2 = 0.045;
	c3 = 0.0075f;
	float factorAtenuacion = 1 / (1 + c2*d + c3*(d*d));

	finalColor = factorAtenuacion * (ambient + diffuse + specular);
	return finalColor;
}

vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection){
	vec3 finalColor;
	vec3 ambient = vec3(texture(mat.TexturaDifusa, TexCoords)) * light.luzAmbiental;

	vec3 lightDir = normalize(light.pos - FragPos);
	float diff = max(dot(normalize(Normal), lightDir), 0.0);
	vec3 diffuse = light.luzDifusa * diff * vec3(texture(mat.TexturaDifusa, TexCoords));

	vec3 R = reflect(-lightDir, normalize(Normal));
	float esp = pow(max(dot(R, viewDirection), 0.0), mat.shininess);
	vec3 specular = light.luzEspecular * esp * vec3(texture(mat.TexturaEspecular, TexCoords));

	float d = length(light.pos - FragPos);
	float c1, c2, c3;
	c1 = 1.0f;
	c2 = 0.045;
	c3 = 0.0075f;
	float factorAtenuacion = 1 / (1 + c2*d + c3*(d*d));

	vec3 incidenciaLuzNormal = normalize(light.pos - FragPos);
	float theta = dot(incidenciaLuzNormal, -normalize(light.dir));
	float epsilon = light.aperturaMin - light.aperturaMax;
	float inte = clamp((theta - light.aperturaMax)/epsilon, 0, 1);

	finalColor = inte * factorAtenuacion * (ambient + diffuse + specular);
	return finalColor;
}