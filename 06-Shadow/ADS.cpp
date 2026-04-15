#include "ADS.h"

ADS::ADS()
{
	ambient = glm::vec4(0.0f);
	diffuse = glm::vec4(0.0f);
	specular = glm::vec4(0.0f);
	shininess = 0.0f;
}

ADS::ADS(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

ADS::~ADS()
{}

glm::vec4& ADS::getAmbient()
{
    return ambient;
}

glm::vec4& ADS::getDiffuse()
{
    return diffuse;
}

glm::vec4& ADS::getSpecular()
{
    return specular;
}

float& ADS::getShininess()
{
    return shininess;
}

void ADS::setAmbient(glm::vec4 ambient)
{
    this->ambient = ambient;
}

void ADS::setDiffuse(glm::vec4 diffuse)
{
    this->diffuse = diffuse;
}

void ADS::setSpecular(glm::vec4 specular)
{
    this->specular = specular;
}

void ADS::setShininess(float shininess)
{
    this->shininess = shininess;
}
