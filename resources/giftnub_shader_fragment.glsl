#version 330 core
out vec4 color;
in vec3 pos;
in vec3 normal;
in vec2 vertex_tex;

uniform sampler2D checkerTex;

void main()
{
	vec3 n = 0.5 * normal + 0.5;
	vec2 texcoord = vertex_tex;
    texcoord.y *= -1;
	color = texture(checkerTex, texcoord);
	if(color.r == 0.0 && color.g == 0.0 && color.b == 0.0)
		color = vec4(n, 1.0);
}
