#version 410 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec3 vPosTarget; // Target vertex position
uniform mat4 PV;
uniform mat4 M;
uniform float morphFactor;
out vec3 fragPos;
out vec3 normal;

void main() {
    vec3 pos = mix(vPos, vPosTarget, morphFactor); // (1 - f) * a + f * b
    fragPos = vec3(M * vec4(pos, 1.0));
    normal = mat3(transpose(inverse(M))) * vNorm; // Use source normal
    gl_Position = PV * vec4(pos, 1.0);
}