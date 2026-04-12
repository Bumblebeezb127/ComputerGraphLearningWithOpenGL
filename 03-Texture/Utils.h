#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<fstream>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<SOIL2/SOIL2.h>
class Utils
{
private:
	Utils() {};
public:
	static Utils& getInstance() {
		static Utils instance;
		return instance;
	}
	GLuint createShaderProgram(const char *vp, const char *fp);
	GLuint createShaderProgram(const char *vp, const char *gp, const char *fp);
	GLuint createShaderProgram(const char *vp, const char *tCS, const char* tES, const char *fp);
	GLuint createShaderProgram(const char *vp, const char *tCS, const char* tES, const char* gp, const char *fp);

	std::string readShaderSource(const char* filePath);
	GLuint loadTexture(const char* filePath);

	void setFloatUniform(GLuint program, const char* name, float value);
	void setIntUniform(GLuint program, const char* name, int value);
	void setVec3Uniform(GLuint program, const char* name, glm::vec3 value);
	void setVec4Uniform(GLuint program, const char* name, glm::vec4 value);
	void setMat4Uniform(GLuint program, const char* name, glm::mat4 value);
};
                                                                                                   
