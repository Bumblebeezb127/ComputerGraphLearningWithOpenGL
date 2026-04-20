# 06-Shadow - 阴影映射技术

## 概述

阴影是增强3D场景真实感的重要因素。本模块实现了基于深度图的阴影映射算法，通过从光源视角渲染场景生成深度图，然后与正常渲染时计算每个像素相对于光源的深度进行对比，判断该像素是否处于阴影中。本模块还实现了多层采样柔和阴影算法。

## 核心知识点

### 1. 阴影映射算法原理

**两遍渲染架构**：
- **Pass 1（深度图生成）**：从光源视角渲染场景，仅计算深度值存储到深度纹理
- **Pass 2（阴影检测）**：从相机视角渲染场景，对每个像素计算其到光源的深度

### 2. 帧缓冲区对象（FBO）与深度纹理

```cpp
void setupShadowBuffers(GLFWwindow* window) {
    glGenFramebuffers(1, &shadowBuffer);
    glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
                 scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}
```

关键参数：
- `GL_DEPTH_COMPONENT32`：32位深度分量格式
- `GL_COMPARE_REF_TO_TEXTURE`：启用深度值与纹理比较的模式
- `GL_COMPARE_FUNC`：比较函数设置为GL_LEQUAL

### 3. 深度图渲染Pass One

```glsl
#version 430
layout (location=0) in vec3 vertPos;
uniform mat4 shadowMVP;
void main(void)
{   gl_Position = shadowMVP * vec4(vertPos,1.0);
}
```

计算shadowMVP：
```cpp
shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
```

### 4. 阴影偏移与阴影痤疮

```cpp
b = glm::mat4(
    0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f);
shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;
```

矩阵b将深度值从[-1,1]范围映射到[0,1]范围。

### 5. 阴影检测与textureProj

```glsl
float lookup(float x, float y)
{   float t = textureProj(shadowTex, shadow_coord + vec4(x * 0.001 * shadow_coord.w,
                                                         y * 0.001 * shadow_coord.w,
                                                         -0.01, 0.0));
    return t;
}
```

### 6. 柔和阴影与多层采样

硬阴影边缘有明显的锯齿，使用64层采样平均：

```glsl
float width = 2.5;
float endp = width * 3.0 + width/2.0;
for (float m=-endp ; m<=endp ; m=m+width)
{   for (float n=-endp ; n<=endp ; n=n+width)
    {   shadowFactor += lookup(m,n);
    }
}
shadowFactor = shadowFactor / 64.0;
```

### 7. 片段着色器中的阴影着色

```glsl
void main(void)
{   float shadowFactor = 0.0;
    vec3 L = normalize(vLightDir);
    vec3 N = normalize(vNormal);
    vec3 V = normalize(-vVertPos);
    vec3 H = normalize(vHalfVec);

    // 计算柔和阴影因子（64次lookup采样）

    vec4 shadowColor = globalAmbient * material.ambient + light.ambient * material.ambient;
    vec4 lightedColor = light.diffuse * material.diffuse * max(dot(L,N),0.0)
                       + light.specular * material.specular
                       * pow(max(dot(H,N),0.0),material.shininess*3.0);

    fragColor = vec4((shadowColor.xyz + shadowFactor*(lightedColor.xyz)),1.0);
}
```

核心公式：`最终颜色 = 阴影颜色 + 阴影因子 × 受光颜色`

### 8. Pass管理渲染流程

```cpp
void display(GLFWwindow* window, double currentTime) {
    lightVmatrix = glm::lookAt(currentLightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightPmatrix = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

    // Pass 1: 渲染到阴影FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
    glDrawBuffer(GL_NONE);
    glEnable(GL_DEPTH_TEST);
    passOne();

    // Pass 2: 渲染到屏幕
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glDrawBuffer(GL_FRONT);
    passTwo();
}
```

## 关键点总结

- 理解阴影映射算法的两遍渲染架构
- 掌握FBO创建和深度纹理配置
- 理解`GL_COMPARE_REF_TO_TEXTURE`模式的作用
- 理解shadow痤疮产生原因和解决方案
- 掌握多层采样柔和阴影的原理
- 理解`textureProj`函数在阴影检测中的应用
- 理解阴影因子如何与光照分量混合
