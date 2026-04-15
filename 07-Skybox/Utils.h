#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<unordered_map>
#include<glm/glm.hpp>
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include<Imgui/imgui.h>
#include<Imgui/imgui_internal.h>
#include<Imgui/imgui_impl_glfw.h>
#include<Imgui/imgui_impl_opengl3.h>
//#include<Imgui/imgui_impl_opengl3_loader.h>
#include<glm/gtc/type_ptr.hpp>
#include<SOIL2/SOIL2.h>
#include"ADS.h"
#include"ImportedModel.h"
enum MaterialType
{
	MT_GOLD,
	MT_SILVER,
	MT_BRONZE,
	MT_JADE
};

// 相机状态
struct Camera {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// 欧拉角
	float yaw = -90.0f;   // 初始朝向 -Z
	float pitch = 0.0f;

	// 移动参数
	float movementSpeed = 2.5f;
	float mouseSensitivity = 0.1f;

	// 更新方向向量（由欧拉角计算）
	void updateVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	// 获取观察矩阵
	glm::mat4 GetViewMatrix() const {
		return glm::lookAt(position, position + front, up);
	}
};

class Utils
{
private:
	ADS goldAds;
	ADS silverAds;
	ADS bronzeAds;
	ADS jadeAds;

	bool isImGuiInit = false;

	std::unordered_map<const char*, GLuint> textureIDs;
private:
	Utils();
	void setupADS();
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

	// Uniform 变量设置函数
	void setFloatUniform(GLuint program, const char* name, float value);
	void setIntUniform(GLuint program, const char* name, int value);
	void setVec3Uniform(GLuint program, const char* name, glm::vec3 value);
	void setVec4Uniform(GLuint program, const char* name, glm::vec4 value);
	void setMat4Uniform(GLuint program, const char* name, glm::mat4 value);

	// 预设材质光照属性获取函数
	glm::vec4& getAmbient(MaterialType type);
	glm::vec4& getDiffuse(MaterialType type);
	glm::vec4& getSpecular(MaterialType type);
	float& getShininess(MaterialType type);

	// 纹理管理函数
	void addTextureID(const char* name, const char*path);
	GLuint getTextureID(const char* name);

	// ImGui 相关函数
	bool ImGuiInit(GLFWwindow* window, const char* glsl_version);
	void ImGuiRender(void (*uiCallback)() = nullptr);
	void ImGuiShutdown();

	//transform
	glm::mat4 transform(glm::vec3 translate);
	glm::mat4 transform(glm::vec3 translate, glm::vec3 rotation);
	glm::mat4 transform(glm::vec3 translate, glm::vec3 rotation, glm::vec3 scale);
	glm::mat4 transform(glm::mat4 &mat, glm::vec3 translate);
	glm::mat4 transform(glm::mat4 &mat, glm::vec3 translate, glm::vec3 rotation);
	glm::mat4 transform(glm::mat4 &mat, glm::vec3 translate, glm::vec3 rotation, glm::vec3 scale);
};
                                                                                                   
