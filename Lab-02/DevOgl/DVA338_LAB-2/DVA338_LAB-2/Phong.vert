// Converts 3D vertex positions from model space to clip space using matrices
#version 410 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
out vec3 FragPos;
out vec3 Normal;
uniform mat4 PV;
uniform mat4 M; // model matrix
void main() {
    FragPos = vec3(M * vec4(vPos, 1.0)); //Transforms the vertex position from local (model) space to world space.
    Normal = mat3(transpose(inverse(M))) * vNorm;
    gl_Position = PV * vec4(vPos, 1.0);
}