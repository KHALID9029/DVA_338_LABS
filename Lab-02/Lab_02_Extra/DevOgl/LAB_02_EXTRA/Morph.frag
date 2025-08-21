#version 410 core
in vec3 fragPos;
in vec3 normal;
out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

void main() {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * matDiffuse);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matShininess);
    vec3 specular =  (spec * matSpecular);

    vec3 ambient =  matAmbient;
    vec3 result = ambient + diffuse + specular;
    result = clamp(result, 0.0, 1.0);

    fragColor = vec4(result, 1.0);
}