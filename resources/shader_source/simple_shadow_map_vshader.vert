#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 Light_point;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 Light_matrix;

void main(){
    Light_point = Light_matrix * vec4(aPos, 1);
    gl_Position = projection * view * model * vec4(aPos, 1);
}