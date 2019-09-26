#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 hmodel;
uniform mat4 hview;
uniform mat4 hprojection;

void main()
{
    Normal = mat3(transpose(inverse(hmodel))) * aNormal;
    Position = vec3(hmodel * vec4(aPos, 1.0));
    gl_Position = hprojection * hview * hmodel * vec4(aPos, 1.0);
}