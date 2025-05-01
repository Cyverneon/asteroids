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

out vec3 fragPosition;
out vec2 fragTexcoord;
out vec3 fragNormal;
out mat3 fragTBN;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0);
    gl_Position = projection * view * worldPosition;
    fragTexcoord = texcoord;
    fragPosition = worldPosition.xyz;

    fragNormal = normalize(mat3(transpose(inverse(model))) * normal);

    vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
    fragTBN = mat3(T, B, N);
}
