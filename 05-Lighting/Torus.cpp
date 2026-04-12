#include "Torus.h"

Torus::Torus()
{
	prec = 128;
	inner = 0.2f;
	outer = 0.8f;
	init();
}

Torus::Torus(float innerRadius, float outerRadius, int precIn)
{
	prec = precIn;
	inner = innerRadius;
	outer = outerRadius;
	init();
}

float Torus::toRadians(float degrees) {
	return glm::radians(degrees);
}


void Torus::init() {
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;

    // 预分配容器大小
    vertices.resize(numVertices);
    texCoords.resize(numVertices);
    normals.resize(numVertices);
    sTrangents.resize(numVertices);
    tTrangents.resize(numVertices);
    indices.resize(numIndices);

    // ========== 第一阶段：生成大环角度为 0 时的横截面（第一个环） ==========
    for (int i = 0; i < prec + 1; i++) {
        // 横截面上的角度（绕管一圈）
        float amt = toRadians(i * 360.0f / prec);
        float cosV = cos(amt);
        float sinV = sin(amt);

        // 顶点位置：圆心固定在 (outer, 0, 0)，然后加上截面偏移
        vertices[i] = glm::vec3(outer + inner * cosV, inner * sinV, 0.0f);

        // 纹理坐标：V 方向沿管，U 方向暂时为 0（第二阶段会被更新）
        texCoords[i] = glm::vec2(0.0f, (float)i / (float)prec);

        // 切向量（T）：沿横截面圆的切线方向（对 amt 求导）
        tTrangents[i] = glm::vec3(-inner * sinV, inner * cosV, 0.0f);

        // 切向量（S）：沿大环切线方向（当前大环角度 u = 0，方向为 +Z）
        sTrangents[i] = glm::vec3(0.0f, 0.0f, 1.0f);

        // 法线 = S × T（注意叉积顺序以保证指向外侧）
        normals[i] = glm::cross(tTrangents[i], sTrangents[i]);
    }

    // ========== 第二阶段：绕 Y 轴旋转生成其余环 ==========
    for (int ring = 1; ring < prec + 1; ring++) {
        float amt = toRadians((float)ring * 360.0f / prec);
        glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));

        for (int vert = 0; vert < prec + 1; vert++) {
            int idx = ring * (prec + 1) + vert;

            // 旋转顶点
            vertices[idx] = glm::vec3(rMat * glm::vec4(vertices[vert], 1.0f));

            // 旋转切向量和法线（注意 w = 0 表示方向向量，但用 glm::vec4(v, 1.0f) 也可接受，因平移不影响方向）
            sTrangents[idx] = glm::vec3(rMat * glm::vec4(sTrangents[vert], 1.0f));
            tTrangents[idx] = glm::vec3(rMat * glm::vec4(tTrangents[vert], 1.0f));
            normals[idx] = glm::vec3(rMat * glm::vec4(normals[vert], 1.0f));

            // 纹理坐标 U 方向沿大环
            texCoords[idx] = glm::vec2((float)ring / (float)prec, texCoords[vert].y);
        }
    }

    // ========== 索引生成：连接相邻顶点形成两个三角形 ==========
    for (int ring = 0; ring < prec; ring++) {
        for (int vert = 0; vert < prec; vert++) {
            int current = ring * (prec + 1) + vert;
            int next = (ring + 1) * (prec + 1) + vert;

            // 第一个三角形：current → next → current+1
            indices[((ring * prec + vert) * 2) * 3 + 0] = current;
            indices[((ring * prec + vert) * 2) * 3 + 1] = next;
            indices[((ring * prec + vert) * 2) * 3 + 2] = current + 1;

            // 第二个三角形：current+1 → next → next+1
            indices[((ring * prec + vert) * 2 + 1) * 3 + 0] = current + 1;
            indices[((ring * prec + vert) * 2 + 1) * 3 + 1] = next;
            indices[((ring * prec + vert) * 2 + 1) * 3 + 2] = next + 1;
        }
    }
}

int Torus::getNumVertices()
{
	return numVertices;
}

int Torus::getNumIndices()
{
	return numIndices;
}

std::vector<int> Torus::getIndices()
{
	return indices;
}

std::vector<glm::vec3> Torus::getVertices()
{
	return vertices;
}

std::vector<glm::vec3> Torus::getNormals()
{
	return normals;
}

std::vector<glm::vec2> Torus::getTexCoords()
{
	return texCoords;
}

std::vector<glm::vec3> Torus::getSTrangents()
{
	return sTrangents;
}

std::vector<glm::vec3> Torus::getTTrangents()
{
	return tTrangents;
}
