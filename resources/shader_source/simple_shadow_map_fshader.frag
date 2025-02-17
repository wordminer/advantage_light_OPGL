#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D deepMap;

void main(){
    float deep = texture(deepMap, TexCoord).r;
    FragColor = vec4(vec3(deep), 1);
}
