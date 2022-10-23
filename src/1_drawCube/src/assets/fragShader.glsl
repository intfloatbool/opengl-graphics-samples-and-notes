#version 430
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
out vec4 color;
in vec4 varyingColor;

void main(void)
{ 
    color = varyingColor;
}