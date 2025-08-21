#version 410 core
in vec3 FragPos;
in vec3 Normal;
uniform vec3 lightPos[2];
uniform vec3 matDiffuse;
out vec4 FragColor;
void main() {
    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);

   for (int i = 0; i < 2; i++) {
   vec3 ambient = 0.05 * matDiffuse;
        // Diffuse
   vec3 lightDir = normalize(lightPos[i] - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
      
   // Quantize the diffuse intensity into cartoon-like steps
   
   if (diff > 0.8)
        diff = 0.8;
    else if (diff > 0.6)
        diff = 0.6;
    else if (diff > 0.4)
        diff = 0.4;
    else if (diff > 0.2)
        diff = 0.2;
    else
        diff = 0.01;

     vec3 diffuse = (diff * matDiffuse);

     result += (ambient + diffuse);
    }


    result = clamp(result, 0.0, 1.0);

    FragColor = vec4(result, 1.0);
}