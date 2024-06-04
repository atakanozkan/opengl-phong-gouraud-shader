#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform bool useAmbient;
uniform bool useDiffuse;
uniform bool useSpecular;
uniform float specularStrength;
uniform float shininess;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float ambientStrength = 0.1;
    vec3 ambient = useAmbient ? ambientStrength * lightColor : vec3(0.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = useDiffuse ? diff * lightColor : vec3(0.0);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = useSpecular ? specularStrength * spec * lightColor : vec3(0.0);

    vec3 result = ambient + diffuse + specular;
    result *= Color;

    FragColor = vec4(result, 1.0);
}
