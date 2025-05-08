#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

layout(std140) uniform Matrices {
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout(std140) uniform MatInfo {
    float uvMult;
};

out VS_OUT
{
    vec3 position;
    vec2 texcoord;
    mat3 TBN;
} vs_out;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0);
    gl_Position = projection * view * worldPosition;
    vs_out.texcoord = texcoord * uvMult;
    vs_out.position = worldPosition.xyz;

    vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
    vs_out.TBN = mat3(T, B, N);
}
