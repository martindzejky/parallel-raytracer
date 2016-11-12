#version 410

in vec2 TexCoord;
uniform sampler2D uTexture;
out vec4 oColor;

void main() {
//    oColor = texture(uTexture, TexCoord);
    oColor = vec4(1.0, 0.0, 0.0, 1.0);
}
