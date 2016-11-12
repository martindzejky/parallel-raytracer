#version 410

in vec2 TexCoord;
uniform sampler2D uTexture;
out vec4 oColor;

void main() {
    oColor = texture(uTexture, TexCoord);
}
