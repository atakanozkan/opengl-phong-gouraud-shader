#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec4 chosenColor;
uniform bool useChosenColor;
uniform bool useTexture;
uniform float specularStrength;
uniform float shininess;
uniform bool useAmbient;
uniform bool useDiffuse;
uniform bool useSpecular;

void main() {
    vec3 textureColor = texture(texture1, TexCoord).rgb;
    float ambientStrength = 0.3;
    vec3 ambient = useAmbient ? ambientStrength * lightColor : vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = useDiffuse ? diff * lightColor * (useTexture ? textureColor : vec3(1.0)) : vec3(0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = useSpecular ? specularStrength * spec * lightColor : vec3(0.0);

    vec3 lighting = ambient + diffuse + specular;
    if (useTexture) {
        lighting *= textureColor;
    }
    if (useChosenColor) {
        lighting *= chosenColor.rgb;
    }

    FragColor = vec4(lighting, 1.0);
}
