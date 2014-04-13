#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp> 
#include <glm/gtc/type_ptr.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

struct Light{
	int type;//0:directional  1:point  2:spot
	vec4 pos;
	vec4 dir;
	float spotCutOff;//cos(a)
	vec3 specular;
	vec3 diffuse;
	vec3 ambient;
};

struct Material{
	vec3 Kd;
	vec3 Ka;
	vec3 Ks;
	float Shininess;
	vec2 ReflectFactor;// 0 to 1. 0 for no reflection. 1 for mirror.
	float Eta;//refraction: 0 to 1. 1 for no banding. 0 for band to nagtive normal.
	float DiffuseFactor;//global diffuse light: 0 to 1. 0 for no global diffuse. 1 for full global diffuse
	float SpecFactor;//global specular light: 0 to 1. 0 for no global specular. 1 for full global specular
};

struct Fog{
	float maxDist;
	float minDist;
	vec3 color;
};

class AABB{
public:
	AABB(){}
	AABB(vec3& a, vec3& b){ min = a; max = b; }
	AABB(const AABB& other){ min = other.min; max = other.max; }
	AABB& operator = (const AABB& other){ min = other.min; max = other.max; return *this; }
	vec3 min, max;
};