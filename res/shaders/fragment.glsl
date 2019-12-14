#version 460 core
out vec4 color;
in vec3 pos;

void main()
{
    color = vec4(pos.xyz + 1, 1.0);
}