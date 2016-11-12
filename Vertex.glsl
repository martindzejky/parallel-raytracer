#version 410

in vec2 iPosition;
in vec2 iTexCoord;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(iPosition, 0.0, 1.0);
    TexCoord = iTexCoord;
}
