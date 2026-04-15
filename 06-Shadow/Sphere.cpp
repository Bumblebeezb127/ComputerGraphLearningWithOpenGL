#include "Sphere.h"


Sphere::Sphere()
{
	init(48);
}

Sphere::Sphere(int prec)
{
	init(prec);
}
Sphere::~Sphere()
{}
void Sphere::init(int prec)
{
	numVertices = (prec + 1) * (prec + 1);
	numIndices = 6 * prec * prec;
	//初始化顶点、纹理坐标和法线数据
	for (int i = 0; i < numVertices; i++)
	{
		vertices.push_back(glm::vec3(0.0f));
		texCoords.push_back(glm::vec3(0.0f));
		normals.push_back(glm::vec3(0.0f));
	}
	//初始化索引数据
	for (int i = 0;i < numIndices; i++)
	{
		indices.push_back(0);
	}
	//计算三角形顶点
	for (int i = 0; i <= prec; i++)
	{
		for(int j=0;j<=prec;j++)
		{
			int index = i * (prec + 1) + j;
			float theta = toRadians((float)i / prec * 180.0f);
			float phi = toRadians((float)j / prec * 360.0f);
			vertices[index].x = sin(theta) * cos(phi);
			vertices[index].y = cos(theta);
			vertices[index].z = sin(theta) * sin(phi);
			texCoords[index].x = (float)j / prec;
			texCoords[index].y = (float)i / prec;
			normals[index] = vertices[index];
		}
	}
	//计算三角形索引
	for (int i = 0; i < prec; i++)
	{
		for(int j=0;j<prec;j++)
		{
			int index = (i * prec + j) * 6;
			int v1 = i * (prec + 1) + j;
			int v2 = v1 + prec + 1;
			int v3 = v2 + 1;
			int v4 = v1 + 1;
			indices[index] = v1;
			indices[index + 1] = v2;
			indices[index + 2] = v3;
			indices[index + 3] = v1;
			indices[index + 4] = v3;
			indices[index + 5] = v4;
		}
	}

}

float Sphere::toRadians(float degree)
{
	return glm::radians(degree);
}

int Sphere::getNumVertices()
{
	return numVertices;
}

int Sphere::getNumIndices()
{
	return numIndices;
}

std::vector<int> Sphere::getIndices()
{
	return indices;
}

std::vector<glm::vec3> Sphere::getVertices()
{
	return vertices;
}

std::vector<glm::vec3> Sphere::getTexCoords()
{
	return texCoords;
}

std::vector<glm::vec3> Sphere::getNormals()
{
	return normals;
}
