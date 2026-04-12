#pragma once
#include <glm/glm.hpp>
class ADS
{
private:
	glm::vec4 ambient;	//环境光分量
	glm::vec4 diffuse;	//漫反射光分量
	glm::vec4 specular;	//镜面反射光分量
	float shininess;	//高光指数
public:
    ADS();
	ADS(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess);
	~ADS();
	glm::vec4& getAmbient();
	glm::vec4& getDiffuse();
	glm::vec4& getSpecular();
	float& getShininess();
	void setAmbient(glm::vec4 ambient);
	void setDiffuse(glm::vec4 diffuse);
	void setSpecular(glm::vec4 specular);
	void setShininess(float shininess);
};