#version 410 core
in vec3 FragPos;
in vec3 Normal;
uniform vec3 lightPos;
uniform vec3 matDiffuse;
out vec4 FragColor;
void main() {
    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    // Ambient term (flat, always present)
    vec3 ambient = 0.1 * matDiffuse;
    // Diffuse term
    float diff = max(dot(norm, lightDir), 0.0);
    // Quantize the diffuse intensity into cartoon-like steps
    if (diff > 0.8)
        diff = 1.0;
    else if (diff > 0.6)
        diff = 0.8;
    else if (diff > 0.4)
        diff = 0.6;
    else if (diff > 0.2)
        diff = 0.4;
    else
        diff = 0.2;
    vec3 diffuse = diff * matDiffuse;
    // Final cartoon shaded color
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}