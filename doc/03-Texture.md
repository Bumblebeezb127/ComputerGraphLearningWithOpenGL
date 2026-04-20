# 03-Texture - 纹理映射技术

## 概述

纹理映射是给3D物体表面贴图的技术。本模块涵盖纹理坐标、纹理加载与绑定、纹理参数设置（过滤方式、环绕模式）、Mipmap多级渐远纹理、各向异性过滤，以及程序化纹理坐标生成等核心内容。

## 核心知识点

### 1. 纹理坐标系统

纹理坐标使用(u, v)二维坐标系，范围通常为[0, 1]：

```cpp
GLfloat pyramidTexCoords[36] = {
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    // ...
};
```

### 2. 纹理对象创建与绑定

使用SOIL2库加载图像文件创建OpenGL纹理对象：

```cpp
GLuint brickTexture;
brickTexture = Utils::getInstance().loadTexture("brick1.jpg");
```

### 3. 纹理单元与激活

```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, brickTexture);
```

着色器中通过uniform sampler2D类型变量关联：

```glsl
layout(binding=0) uniform sampler2D samp;
```

### 4. 片段着色器中的纹理采样

```glsl
#version 430
in vec2 tc;
out vec4 color;
layout(binding=0) uniform sampler2D samp;

void main(void)
{
    color = texture(samp, tc);
};
```

### 5. 纹理参数：过滤方式

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glGenerateMipmap(GL_TEXTURE_2D);
```

- GL_NEAREST：最近邻过滤
- GL_LINEAR：双线性过滤
- GL_LINEAR_MIPMAP_LINEAR：三线性过滤

### 6. Mipmap多级渐远纹理

Mipmap是预生成的纹理金字塔：
```
原始纹理: 256×256
Mipmap级别: 128×128, 64×64, 32×32, 16×16, 8×8, 4×4, 2×2, 1×1
```

### 7. 各向异性过滤（Anisotropic Filtering）

```cpp
if(glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
    GLfloat largest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
}
```

### 8. 纹理环绕模式

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
```

- GL_REPEAT：重复纹理
- GL_CLAMP_TO_EDGE：夹取到边缘
- GL_MIRRORED_REPEAT：镜像重复

### 9. 程序化纹理坐标

```glsl
void main(void)
{
    vec2 texCoord = worldPos.xz * repeatScale;
    color = texture(samp, texCoord);
};
```

### 10. 窗口回调与动态投影矩阵更新

```cpp
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

glfwSetWindowSizeCallback(window, window_reshape_callback);
```

## 关键点总结

- 理解纹理坐标(u,v)与顶点的对应关系
- 掌握纹理对象的创建、绑定和数据加载流程
- 理解纹理单元的作用和glActiveTexture的使用
- 掌握纹理参数设置（过滤模式和环绕模式）
- 理解Mipmap的工作原理及其对渲染质量的提升
- 理解各向异性过滤解决倾斜视角模糊的原理
