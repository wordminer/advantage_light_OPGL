#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vs_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
}vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    vs_out.FragPos = vec3(model * vec4(aPos, 1));
    vs_out.Normal = vec3(model * vec4(aNormal,1));
    vs_out.TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1);
}