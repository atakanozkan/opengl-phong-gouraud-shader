#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
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
    vec3 pos = vec3(model * vec4(aPos, 1.0));
    vec3 norm = normalize(mat3(transpose(inverse(model))) * aNormal);
    vec3 lightDir = normalize(lightPos - pos);
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float ambientStrength = 0.1;
    vec3 ambient = useAmbient ? ambientStrength * lightColor : vec3(0.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = useDiffuse ? diff * lightColor : vec3(0.0);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = useSpecular ? specularStrength * spec * lightColor : vec3(0.0);

    vec3 result = ambient + diffuse + specular;
    if (useChosenColor) {
        result *= chosenColor.rgb;
    }

    vertexColor = vec4(result, 1.0);
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
