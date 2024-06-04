#version 330 core
in vec4 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform bool useTexture;

void main() {
    if (useTexture) {
        vec3 textureColor = texture(texture1, TexCoord).rgb;
        FragColor = vec4(textureColor, 1.0) * vertexColor;
    } else {
        FragColor = vertexColor;
    }
}
