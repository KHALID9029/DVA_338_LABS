#version 410 core
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightAmbient;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

uniform vec3 viewPos;
void main() {
    // Compute face normal using derivatives
    vec3 dPosX = dFdx(fragPos);
    vec3 dPosY = dFdy(fragPos);
    vec3 norm = normalize(cross(dPosX, dPosY));
 
    // Phong lighting with face normal
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  (diff * matDiffuse);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matShininess);
    vec3 specular = (spec * matSpecular);

    vec3 ambient = lightAmbient * matAmbient;

    vec3 result = ambient + diffuse + specular;
    result = clamp(result, 0.0, 1.0);

    fragColor = vec4(result, 1.0);
}