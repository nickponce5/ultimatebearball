#version 330 core
out vec4 color;

uniform vec3 semiSphere;
uniform float maxDistFromSemi;

in vec3 position;

void main()
{
	// Where the value of the vector is at the far point of the sphere (aka -raduis)
	float dist = distance(position, vec3(0.0, 0.0, -0.15));
	float semiDist = distance(position, semiSphere);
	float alpha = 1.0;
	if(semiDist < maxDistFromSemi) {
		alpha = 0.25;
	}
	else {
		alpha = 1;//0.8;
	}
	vec3 colormix = vec3(0.5, 0.5, 0.9) * dist + vec3(0.1, 0.1, 0.1) * (1-dist);
	color = vec4(colormix, alpha);

	if(semiSphere.x == 0.0 && semiSphere.y == 0.0 && semiSphere.z == 0.0)
		color = vec4(1, 0, 0, 1);
}
