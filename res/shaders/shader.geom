#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

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

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gs_out.position = gs_in[i].position;
        gs_out.texcoord = gs_in[i].texcoord;
        gs_out.TBN = gs_in[i].TBN;

        vec4 worldPosition = vec4(gs_in[i].position, 1.0);
        gl_Position = projection * view * worldPosition;

        EmitVertex();
    }

    EndPrimitive();

    for (int i = 0; i < 3; i++)
    {
        vec3 pos = gs_in[i].position + vec3(24.5, 0.0, 0.0);

        vec4 worldPosition = vec4(pos, 1.0);
        gl_Position = projection * view * worldPosition;

        gs_out.position = pos;
        gs_out.texcoord = gs_in[i].texcoord;
        gs_out.TBN = gs_in[i].TBN;
        EmitVertex();
    }


    EndPrimitive();
}
