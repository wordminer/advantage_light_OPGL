#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_floor;

void main(){
    FragColor = texture(texture_floor, TexCoord);
}