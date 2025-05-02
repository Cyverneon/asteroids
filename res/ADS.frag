#version 400 core

in VS_OUT
{
    vec3 position;
    vec2 texcoord;
    mat3 TBN;
} vs_out;

out vec4 FragColor;

uniform sampler2D colourSampler;
uniform sampler2D normalSampler;

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Hardcoded Light Properties
const vec3 lightPos = vec3(2.0, 10.0, 2.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const vec3 ambientColor = vec3(0.2, 0.2, 0.2);
const float shininess = 1.0;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 normal = texture(normalSampler, vs_out.texcoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(vs_out.TBN * normal);

    // Normalise Vectors
    vec3 lightDir = normalize(lightPos - vs_out.position);
    
    // Ambient Light
    vec3 ambient = ambientColor;

    // Diffuse Light (Lambertian Reflection)
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular Light (Blinn-Phong)
    vec3 viewDir = normalize(-vs_out.position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    // Final Lighting (Ambient + Diffuse + Specular)
    vec3 lighting = ambient + diffuse + specular;

    // Texture Sampling
    vec4 texColor = texture(colourSampler, vs_out.texcoord);

    // Final Output
    FragColor = texColor * vec4(lighting, 1.0);

    // Texture testing
    //FragColor = texture(normalSampler, vs_out.texcoord);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 /   (light.constant + 
                                (light.linear * distance) +
                                (light.quadratic * (distance * distance)));
    
    vec3 ambient = light.ambient * vec3(texture(colourSampler, vs_out.texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(colourSampler, vs_out.texcoord));
    ambient *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse);
}