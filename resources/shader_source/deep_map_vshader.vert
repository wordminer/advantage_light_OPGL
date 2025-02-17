#version 330 core 

layout (location = 0) in vec3 aPos;

uniform mat4 Light_matrix;
uniform mat4 model;

void main()
{
    gl_Position = Light_matrix * model * vec4(aPos, 1.0f);
}