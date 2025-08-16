#version 330 core
layout (location = 0) in vec2 aPos; // 位置变量的属性位置值为0
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
uniform mat4 projection; // 正射投影矩阵
uniform mat4 model; // 模型矩阵（每个精灵独立）

void main()
{   
    // 应用矩阵
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);

    // 传递纹理坐标
    texCoord = aTexCoord;
}