#version 420 core

in vec3 vPos;
in vec3 vNorm;
out vec4 color;
out vec4 pos;
uniform mat4 PV;

void main(void)
{
	color = vec4(vNorm,1);
	pos  = PV * vec4(vPos, 1.0f);
	gl_Position = pos;
	
}