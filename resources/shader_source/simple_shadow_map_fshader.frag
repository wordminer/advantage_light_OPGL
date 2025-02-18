#version 330 core

out vec4 FragColor;

in vec4 Light_point;

uniform sampler2D deepMap;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main(){
    vec3 projCoords = Light_point.xyz / Light_point.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    float deep = texture(deepMap, projCoords.xy).r;
    deep = LinearizeDepth(deep);

    float currentDepth = projCoords.z;
    float bias = 0.005; 
    // check whether current frag pos is in shadow
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(deepMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(deepMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if (projCoords.z > 1){shadow = 0;}

    FragColor = vec4(vec3(1-shadow), 1);
}
