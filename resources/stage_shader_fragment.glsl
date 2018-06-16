#version 330 core
out vec4 color;

in vec3 vertex_color;
in vec3 vertex_pos;

void main()
{
	color = vec4(vertex_color, 1.0);
	if(vertex_color.r == 0.0 && vertex_color.g == 0.0 && vertex_color.b == 0.0) {
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	
}