#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

in VS_OUT
{
    vec3 position;
    vec2 texcoord;
    mat3 TBN;
} gs_in[3];

out VS_OUT
{
    vec3 position;
    vec2 texcoord;
    mat3 TBN;
} gs_out;

layout(std140) uniform Matrices {
    mat4 model;
    mat4 view;
    mat4 projection;
};

const float minX = -12.8;
const float maxX = 12.8;
const float minZ = -7.2;
const float maxZ = 7.2;

bool xTri = false;
bool zTri = false;
float offsetX = 0.0;
float offsetZ = 0.0;

void outputTri();
void outputTriOffset(vec3 offset);

void main()
{
    outputTri();

    // Spawn copies of the triangle if it exceeds the screen boundaries
    // This is just a visual effect to made the teleportion wrap-around for certain objects more convincing
    
    xTri = false;
    zTri = false;
    offsetX = 0.0;
    offsetZ = 0.0;

    // Minimum X - Left side of screen
    if (gs_in[0].position.x < minX || gs_in[1].position.x < minX || gs_in[2].position.x < minX)
    {
        offsetX = maxX - minX;
        xTri = true;
    }
    // Maximum X - Right side of screen
    else if (gs_in[0].position.x > maxX || gs_in[1].position.x > maxX || gs_in[2].position.x > maxX)
    {
        offsetX = -(maxX - minX);
        xTri = true;
    }

    // Minimum Z - Top side of screen
    if (gs_in[0].position.z < minZ || gs_in[1].position.z < minZ || gs_in[2].position.z < minZ)
    {
        offsetZ = maxZ - minZ;
        zTri = true;
    }
    // Maximum Z - Bottom side of screen
    else if (gs_in[0].position.z > maxZ || gs_in[1].position.z > maxZ || gs_in[2].position.z > maxZ)
    {
        offsetZ = -(maxZ - minZ);
        zTri = true;
    }

    if (xTri)
    {
        outputTriOffset(vec3(offsetX, 0.0, 0.0));
    }
    if (zTri)
    {
        outputTriOffset(vec3(0.0, 0.0, offsetZ));
    }
    // if the triangle overlaps both X and Z boundaries a third copy should be spawned with both offsets
    // if the object is at a corner, the 3 quadrants offscreen should wrap round to the 3 other corners
    if (xTri && zTri)
    {
        outputTriOffset(vec3(offsetX, 0.0, offsetZ));
    }
}

void outputTri()
{
    for (int i = 0; i < 3; i++)
    {
        gs_out.position = gs_in[i].position;
        gs_out.texcoord = gs_in[i].texcoord;
        gs_out.TBN = gs_in[i].TBN;

        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }
    EndPrimitive();
}

void outputTriOffset(vec3 offset)
{
    for (int i = 0; i < 3; i++)
    {
        vec4 worldPosition = vec4((gs_in[i].position + offset), 1.0);

        gl_Position = projection * view * worldPosition;

        gs_out.position = worldPosition.xyz;
        gs_out.texcoord = gs_in[i].texcoord;
        gs_out.TBN = gs_in[i].TBN;
        EmitVertex();
    }
    EndPrimitive();
}