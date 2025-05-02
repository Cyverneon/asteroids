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

// Hardcoded Light Properties
const vec3 lightPos = vec3(2.0, 10.0, 2.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const vec3 ambientColor = vec3(0.2, 0.2, 0.2);
const float shininess = 1.0;

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
}
