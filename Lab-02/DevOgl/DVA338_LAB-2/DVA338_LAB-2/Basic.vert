#version 410 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
out vec4 color;
uniform mat4 PV;
void main() {
    color = vec4(vNorm, 1.0);
    gl_Position = PV * vec4(vPos, 1.0);
}