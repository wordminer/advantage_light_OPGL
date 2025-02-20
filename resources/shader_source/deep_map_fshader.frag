#version 330 core

in vec4 FragPos;

uniform vec3 LightPos;
uniform float far_plane;

void main(){
    float distance = length(FragPos.xyz - LightPos);

    distance = distance / far_plane;

    gl_FragDepth = distance;
}