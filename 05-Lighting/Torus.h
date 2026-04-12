#pragma once
#include <vector>
#include <cmath>
#include<iostream>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Torus
{
private:
	int numVertices;
	int numIndices;
	int prec;
	float inner;
	float outer;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> sTrangents;
	std::vector<glm::vec3> tTrangents;

public:
	Torus();
	Torus(float innerRadius, float outerRadius, int precIn);
	void init();
	float toRadians(float degrees);
	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getSTrangents();
	std::vector<glm::vec3> getTTrangents();
};

