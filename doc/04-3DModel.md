# 04-3DModel - 3D模型加载与几何体生成

## 概述

本模块专注于复杂3D几何体的创建和外部模型的加载。内容涵盖球体和圆环体的参数化生成算法、OBJ模型格式的解析、顶点数据与索引数据的组织，以及EBO（元素缓冲区对象）的使用。

## 核心知识点

### 1. 球体的参数化生成

球体通过球面坐标公式生成顶点：

```cpp
void Sphere::init(int prec) {
    numVertices = (prec + 1) * (prec + 1);
    numIndices = 6 * prec * prec;

    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            int index = i * (prec + 1) + j;
            float theta = toRadians((float)i / prec * 180.0f);
            float phi = toRadians((float)j / prec * 360.0f);

            vertices[index].x = sin(theta) * cos(phi);
            vertices[index].y = cos(theta);
            vertices[index].z = sin(theta) * sin(phi);

            texCoords[index].s = (float)j / prec;
            texCoords[index].t = (float)i / prec;

            normals[index] = vertices[index];
        }
    }
}
```

球面坐标公式：
- x = sin(θ) × cos(φ)
- y = cos(θ)
- z = sin(θ) × sin(φ)

### 2. 圆环体的参数化生成

圆环体（Torus）通过两阶段生成：

```cpp
Torus::Torus(float innerRadius, float outerRadius, int precIn) {
    prec = precIn;
    inner = innerRadius;
    outer = outerRadius;
    init();
}
```

**第一阶段：生成横截面圆**

```cpp
for (int i = 0; i < prec + 1; i++) {
    float amt = toRadians(i * 360.0f / prec);
    float cosV = cos(amt);
    float sinV = sin(amt);

    vertices[i] = glm::vec3(outer + inner * cosV, inner * sinV, 0.0f);
    texCoords[i] = glm::vec2(0.0f, (float)i / (float)prec);

    tTrangents[i] = glm::vec3(-inner * sinV, inner * cosV, 0.0f);
    sTrangents[i] = glm::vec3(0.0f, 0.0f, 1.0f);
    normals[i] = glm::cross(sTrangents[i], tTrangents[i]);
}
```

**第二阶段：绕Y轴旋转生成完整环**

```cpp
for (int ring = 1; ring < prec + 1; ring++) {
    float amt = toRadians((float)ring * 360.0f / prec);
    glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));

    for (int vert = 0; vert < prec + 1; vert++) {
        int idx = ring * (prec + 1) + vert;

        vertices[idx] = glm::vec3(rMat * glm::vec4(vertices[vert], 1.0f));
        sTrangents[idx] = glm::vec3(rMat * glm::vec4(sTrangents[vert], 1.0f));
        tTrangents[idx] = glm::vec3(rMat * glm::vec4(tTrangents[vert], 1.0f));
        normals[idx] = glm::vec3(rMat * glm::vec4(normals[vert], 1.0f));

        texCoords[idx] = glm::vec2((float)ring / (float)prec, texCoords[vert].y);
    }
}
```

### 3. OBJ模型文件解析

```cpp
void ModelImporter::parseOBJ(const char *filePath) {
    ifstream fileStream(filePath, ios::in);
    string line = "";

    while (!fileStream.eof()) {
        getline(fileStream, line);

        if (line.compare(0, 2, "v ") == 0) {  // 顶点位置
            stringstream ss(line.erase(0, 1));
            ss >> x >> y >> z;
            vertVals.push_back(x); vertVals.push_back(y); vertVals.push_back(z);
        }

        if (line.compare(0, 2, "vt") == 0) {  // 纹理坐标
            stringstream ss(line.erase(0, 2));
            ss >> x >> y;
            stVals.push_back(x); stVals.push_back(y);
        }

        if (line.compare(0, 2, "vn") == 0) {  // 法线
            stringstream ss(line.erase(0, 2));
            ss >> x >> y >> z;
            normVals.push_back(x); normVals.push_back(y); normVals.push_back(z);
        }

        if (line.compare(0, 2, "f ") == 0) {  // 面
            string oneCorner, v, t, n;
            stringstream ss(line.erase(0, 2));

            for (int i = 0; i < 3; i++) {
                getline(ss, oneCorner, ' ');
                stringstream oneCornerSS(oneCorner);
                getline(oneCornerSS, v, '/');
                getline(oneCornerSS, t, '/');
                getline(oneCornerSS, n, '/');

                int vertRef = (stoi(v) - 1) * 3;
                int tcRef = (stoi(t) - 1) * 2;
                int normRef = (stoi(n) - 1) * 3;

                triangleVerts.push_back(vertVals[vertRef]);
                triangleVerts.push_back(vertVals[vertRef + 1]);
                triangleVerts.push_back(vertVals[vertRef + 2]);

                textureCoords.push_back(stVals[tcRef]);
                textureCoords.push_back(stVals[tcRef + 1]);

                normals.push_back(normVals[normRef]);
                normals.push_back(normVals[normRef + 1]);
                normals.push_back(normVals[normRef + 2]);
            }
        }
    }
}
```

### 4. 元素缓冲区对象（EBO）

```cpp
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
```

绘制时使用：
```cpp
glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);
```

### 5. 多VBO数据组织

```cpp
glGenBuffers(numVBOs, vbo);

glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
glBufferData(GL_ARRAY_BUFFER, texCoordData.size() * sizeof(float), &texCoordData[0], GL_STATIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), &normalData[0], GL_STATIC_DRAW);
```

## 关键点总结

- 理解球面坐标参数化生成的数学原理
- 掌握圆环体两阶段生成算法（横截面圆+旋转）
- 理解OBJ格式中顶点、纹理坐标、法线、面的组织方式
- 掌握简化的OBJ解析器实现
- 理解EBO（元素缓冲区）的作用和glDrawElements的使用
- 掌握多VBO分别存储顶点、纹理坐标、法线数据的组织方式
