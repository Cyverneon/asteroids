#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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

void main()
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
