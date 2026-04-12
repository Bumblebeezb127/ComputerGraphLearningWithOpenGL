#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>
#include<vector>
#include<glm/glm.hpp>
class Sphere
{
private:
	int numVertices;
	int numIndices;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> texCoords;
	std::vector<glm::vec3> normals;

	void init(int prec);
	float toRadians(float degree);
public:
	Sphere();
	Sphere(int prec);
	~Sphere();

	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getTexCoords();
	std::vector<glm::vec3> getNormals();	
};

