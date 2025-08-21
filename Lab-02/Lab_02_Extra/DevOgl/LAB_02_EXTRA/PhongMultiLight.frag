#version 410 core
in vec3 FragPos;
in vec3 Normal;
out vec4 fragColor;

uniform vec3 lightPos[2];
uniform vec3 lightAmbient[2];
uniform vec3 lightDiffuse[2];
uniform vec3 lightSpecular[2];
uniform vec3 viewPos;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

void main() {
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);

    for (int i = 0; i < 2; i++) {
        // Ambient
        vec3 ambient = lightAmbient[i] * matAmbient;

        // Diffuse
        vec3 lightDir = normalize(lightPos[i] - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lightDiffuse[i] * (diff * matDiffuse);

        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), matShininess);
        vec3 specular = lightSpecular[i] * (spec * matSpecular);

        result += ambient + diffuse + specular;
    }

    result = clamp(result, 0.0, 1.0);

    fragColor = vec4(result, 1.0);
}