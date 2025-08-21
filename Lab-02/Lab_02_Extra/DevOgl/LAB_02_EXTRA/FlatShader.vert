#version 410 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm; //ignored in flat shading
uniform mat4 PV;
uniform mat4 M;
out vec3 fragPos;

void main() {
    fragPos = vec3(M * vec4(vPos, 1.0));
   
    gl_Position = PV * vec4(vPos, 1.0);
}