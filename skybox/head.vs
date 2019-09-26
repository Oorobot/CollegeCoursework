#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 Hmodel;
uniform mat4 Hview;
uniform mat4 Hprojection;

void main()
{
    Normal = mat3(transpose(inverse(Hmodel))) * aNormal;
    Position = vec3(Hmodel * vec4(aPos, 1.0));
    gl_Position = Hprojection * Hview * Hmodel * vec4(aPos, 1.0);
}