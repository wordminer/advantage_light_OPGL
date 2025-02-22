#version 330 core

out vec4 FragColor;

struct Light_info
{
    vec3 lightPos;
    float far_plane;
    vec3 color_ambient;
};


in vs_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
}vs_in;

uniform vec3 viewPos;

uniform int light_source_number;
uniform Light_info light_sources[10];

uniform samplerCube depthMap[10];
uniform sampler2D diffuseTexture;

uniform bool Is_sun;
uniform vec3 sun_color;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);  

float ShadowCalculation(Light_info light, int key)
{
    vec3 fragToLight = vs_in.FragPos - light.lightPos;
    float currentDepth = length(fragToLight);
    float viewrange = length(viewPos - vs_in.FragPos);
    float shadow  = 0.0;
    float bias    = 0.05; 
    float radius = (1+viewrange/light.far_plane)/25;
    float offset = 20;
    for(int x = 0; x < offset; x += 1)
    {
        float closestDepth = texture(depthMap[key], fragToLight + sampleOffsetDirections[x]*radius).r; 
        closestDepth *= light.far_plane;  
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= offset;  
    
    return shadow;
}

vec3 light_caculator(){
    vec3 lighting = vec3(0.0f);
    for (int light_key = 0; light_key < light_source_number; light_key ++){
        vec3 color = texture(diffuseTexture, vs_in.TexCoord).rgb;

        vec3 normal = normalize(vs_in.Normal);
        vec3 lightColor = light_sources[light_key].color_ambient;
        // ambient
        vec3 ambient = 0.3 * lightColor;
        // diffuse
        vec3 lightDir = normalize(light_sources[light_key].lightPos - vs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * lightColor;
        // specular
        vec3 viewDir = normalize(viewPos - vs_in.FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = 0.0;
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);
        vec3 specular = spec * lightColor;    
        // calculate shadow
        float shadow = ShadowCalculation(light_sources[light_key], light_key);                      
        lighting += (ambient + (1.0 - shadow) * (diffuse + specular)) * color; 
    }
    return lighting;
}

void main()
{           
    if (Is_sun){
        FragColor = vec4(sun_color, 1);
        return;
    }
    
    FragColor = vec4(light_caculator(), 1.0);
}

