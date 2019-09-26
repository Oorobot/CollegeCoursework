#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 lmodel;
uniform mat4 lview;
uniform mat4 lprojection;

void main()
{
    Normal = mat3(transpose(inverse(lmodel))) * aNormal;
    Position = vec3(lmodel * vec4(aPos, 1.0));
    gl_Position = lprojection * lview * lmodel * vec4(aPos, 1.0);
}