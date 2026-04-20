# 01-Pipeline - 渲染管道与基础绘制

## 概述

本模块是整个学习路径的起点，专注于建立对OpenGL渲染管道的基本认知。内容涵盖了从窗口初始化、上下文创建，到顶点着色器和片段着色器的编写，再到最终的图形绘制。通过本模块，学习者将理解现代OpenGL的核心工作流程：数据如何从CPU传输到GPU，经过可编程着色器的处理，最终生成像素输出到屏幕。

## 核心知识点

### 1. OpenGL上下文与窗口初始化

窗口创建采用GLFW库实现：

```cpp
if (!glfwInit()) {
    cerr << "Failed to initialize GLFW" << endl;
    exit(EXIT_FAILURE);
}

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Window", nullptr, nullptr);
```

GLEW（OpenGL Extension Wrangler Library）负责在运行时动态加载扩展函数：

```cpp
glewExperimental = GL_TRUE;
if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    exit(EXIT_FAILURE);
}
```

### 2. 着色器基础与GLSL语法

顶点着色器负责处理输入顶点数据：

```glsl
#version 430
uniform float offset;

void main(void)
{
    if(gl_VertexID == 0){
        gl_Position = vec4(0.25 + offset, -0.25+offset, 0.0, 1.0);
    }
    else if(gl_VertexID == 1){
        gl_Position = vec4(-0.25 + offset, -0.25+offset, 0.0, 1.0);
    }
    else{
        gl_Position = vec4(0.25 + offset, 0.25+offset, 0.0, 1.0);
    }
};
```

片段着色器负责计算每个像素的最终颜色：

```glsl
#version 430
out vec4 color;
void main(void)
{
    color = vec4(1.0, 0.0, 0.0, 1.0);
};
```

### 3. 顶点属性与VAO/VBO机制

VAO（顶点数组对象）存储顶点属性配置，VBO（顶点缓冲区对象）存储实际顶点数据：

```cpp
#define numVAOs 1
GLuint vao[numVAOs];
glGenVertexArrays(numVAOs, vao);
glBindVertexArray(vao[0]);

GLuint vbo[numVBOs];
glGenBuffers(numVBOs, vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(0);
```

### 4. uniform变量与CPU-GPU通信

uniform是全局变量，用于从CPU端向GPU着色器传递数据：

```cpp
utils.setFloatUniform(renderingProgram, "offset", x);
```

### 5. 变换矩阵：平移、旋转、缩放

图形学中的变换通过矩阵实现：

```glsl
mat4 buildTranslate(float x, float y, float z)
{
    mat4 translate = mat4(1.0, 0.0, 0.0, 0.0,
                         0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0,
                         x, y, z, 1.0);
    return translate;
}

mat4 buildRotateX(float angle)
{
    float rad = radians(angle);
    mat4 rotate = mat4(1.0, 0.0, 0.0, 0.0,
                       0.0, cos(rad), -sin(rad), 0.0,
                       0.0, sin(rad), cos(rad), 0.0,
                       0.0, 0.0, 0.0, 1.0);
    return rotate;
}
```

### 6. 矩阵运算与变换顺序

矩阵乘法遵循左乘原则：`gl_Position = P * V * M * vertex`

### 7. 渲染循环与双缓冲

```cpp
while (!glfwWindowShouldClose(window)) {
    display(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

### 8. 着色器编译与链接

```cpp
GLuint renderingProgram = utils.createShaderProgram("VertexShader.glsl", "FragmentShader.glsl");
```

## 关键点总结

- 理解现代OpenGL的渲染管道工作原理
- 掌握VAO/VBO的创建、绑定和数据传输流程
- 熟练编写顶点着色器和片段着色器
- 理解MVP矩阵的概念和变换顺序
- 理解双缓冲机制和垂直同步的意义
