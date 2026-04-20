# 07-Skybox - 天空盒与第一人称相机

## 概述

本模块在阴影映射基础上添加了天空盒效果和第一人称相机控制系统。天空盒模拟无限远天空和环境，第一人称相机控制系统支持键盘移动和鼠标视角控制。此外还集成了ImGui用于运行时调试参数的图形化调整界面。

## 核心知识点

### 1. 天空盒原理

天空盒是一个包围整个场景的立方体，立方体内表面贴有环境纹理：

```cpp
void passTwo(void) {
    isSkybox = 1.0f;
    mMat = Utils::getInstance().transform(cameraLoc, -cameraRot);
    mvMat = Utils::getInstance().transform(-cameraLoc) * mMat;

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_DEPTH_TEST);
}
```

### 2. 天空盒着色器

```glsl
void skybox(){
    tc = texCoord;
    gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}

void main(void)
{   if(isSkybox==1.0f){
        skybox();
    } else {
        model();
    }
}
```

uniform `isSkybox`用于在运行时切换着色器行为。

### 3. 第一人称相机数学

```cpp
struct Camera {
    glm::vec3 position;      // 相机位置
    glm::vec3 front;         // 观察方向
    glm::vec3 up;            // 世界坐标系上向量
    glm::vec3 right;         // 右向量
    float yaw;               // 偏航角（水平旋转）
    float pitch;             // 俯仰角（垂直旋转）
    float mouseSensitivity;  // 鼠标灵敏度
    float movementSpeed;      // 移动速度
};
```

### 4. 鼠标控制视角

```cpp
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= camera.mouseSensitivity;
    yoffset *= camera.mouseSensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    if (camera.pitch > 89.0f) camera.pitch = 89.0f;
    if (camera.pitch < -89.0f) camera.pitch = -89.0f;

    camera.updateVectors();
}
```

俯仰角限制在±89度防止万向锁。

### 5. 相机向量更新

```cpp
void Camera::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    this->front = front;
    this->right = glm::normalize(glm::cross(front, worldUp));
    this->up = glm::normalize(glm::cross(right, front));
}
```

### 6. 键盘移动控制

```cpp
void processKeyboard(GLFWwindow* window, float deltaTime) {
    float velocity = camera.movementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += camera.front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= camera.front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= camera.right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += camera.right * velocity;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.position -= camera.worldUp * velocity;  // 下降
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.position += camera.worldUp * velocity;   // 上升
}
```

使用deltaTime确保移动速度与帧率无关。

### 7. ImGui集成

```cpp
Utils::getInstance().ImGuiInit(window, "#version 430");

Utils::getInstance().ImGuiRender([]()->void {
    ImGui::Begin("Controls");
    ImGui::SliderFloat3("Light Position", &lightLoc.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Camera Position", &cameraLoc.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Camera Rotation", &cameraRot.x, -180.0f, 180.0f);
    ImGui::Text("FPS: %.0f", FPS);
    ImGui::End();
});
```

### 8. 鼠标锁定与Tab切换

```cpp
glfwSetCursorPosCallback(window, mouse_callback);
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (cursorVisible) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorVisible = false;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorVisible = true;
        }
    }
}
```

Tab键切换鼠标锁定状态。

### 9. FPS计算

```cpp
#define START_DRAW currentFrame = glfwGetTime(); \
                    if (lastFrame == 0.0) lastFrame = currentFrame; \
                    deltaTime = currentFrame - lastFrame; \
                    if (FPS == 0.0) FPS = 1.0 / deltaTime; \
                    addTime += deltaTime; \
                    if (addTime >= 1.0) { FPS = 1.0 / deltaTime; addTime = 0.0; }
```

### 10. 统一的transform工具函数

```cpp
glm::mat4 transform(glm::vec3 translation, glm::vec3 rotation) {
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), translation);
    mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return mat;
}
```

## 关键点总结

- 理解天空盒作为无限远背景的实现原理
- 掌握立方体纹理坐标映射以消除接缝
- 理解第一人称相机的数学模型（偏航角、俯仰角）
- 掌握鼠标控制视角的算法实现
- 理解相机三个正交向量的计算方法
- 掌握键盘WASD+QE移动控制的实现
- 理解ImGui集成用于运行时调试
- 理解鼠标锁定切换和deltaTime时间无关运动
