#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec3 pos;
out vec3 normal;
out vec2 vertex_tex;

void main()
{
    pos = vertPos;
    // normal = (M * vec4(vertNor, 0.0)).xyz;
    if(vertNor != vec3(0.0, 0.0, 0.0))
        normal = vertNor;
    else {
        normal = vertPos - vec3(M[0][3], vertPos.y, M[2][3]);
        normal = normalize(normal);
    }
    vertex_tex = vertTex;
    gl_Position = P * V * M * vec4(pos, 1.0);
}