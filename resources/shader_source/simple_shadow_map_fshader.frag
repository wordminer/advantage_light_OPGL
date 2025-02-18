#version 330 core

out vec4 FragColor;

in vec4 Light_point;

uniform sampler2D deepMap;

void main(){
    vec3 projCoords = Light_point.xyz / Light_point.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    float deep = texture(deepMap, projCoords.xy).r;

    float currentDepth = projCoords.z;
    float bias = 0.005; 
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > deep  ? 1.0 : 0.0;

    if (projCoords.z > 1){shadow = 0;}

    FragColor = vec4(vec3(1-shadow), 1);
}
