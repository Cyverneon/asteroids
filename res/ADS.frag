#version 400 core

in vec3 fragPosition;
in vec2 fragTexcoord;
in vec3 fragNormal;
in mat3 fragTBN;

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;

// Hardcoded Light Properties
const vec3 lightPos = vec3(2.0, 4.0, 2.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const vec3 ambientColor = vec3(0.2, 0.2, 0.2);
const float shininess = 1.0;

void main() {
    vec3 normal = texture(normalSampler, fragTexcoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(fragTBN * normal);

    // Normalise Vectors
    vec3 norm = normal;
    vec3 lightDir = normalize(lightPos - fragPosition);
    
    // Ambient Light
    vec3 ambient = ambientColor;

    // Diffuse Light (Lambertian Reflection)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular Light (Blinn-Phong)
    vec3 viewDir = normalize(-fragPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    // Final Lighting (Ambient + Diffuse + Specular)
    vec3 lighting = ambient + diffuse + specular;

    // Texture Sampling
    vec4 texColor = texture(textureSampler, fragTexcoord);

    // Final Output
    FragColor = texColor * vec4(lighting, 1.0);
}
