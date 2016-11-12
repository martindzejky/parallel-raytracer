#include "FullscreenQuad.hpp"

#include <GL/glew.h>

#include "Window.hpp"
#include "Shaders.hpp"


std::shared_ptr<FullscreenQuad> FullscreenQuad::Create() {
    auto quad = std::make_shared<FullscreenQuad>();

    quad->Load();

    SetSingleton(quad);
    return quad;
}

void FullscreenQuad::Render() {
    glBindVertexArray(mArrayObject);
    Window::GetSingleton()->DrawElements(6);
}

FullscreenQuad::~FullscreenQuad() {
    if (mIndices) {
        glDeleteBuffers(1, &mIndices);
    }
    if (mVertexData) {
        glDeleteBuffers(1, &mVertexData);
    }
    if (mArrayObject) {
        glDeleteVertexArrays(1, &mArrayObject);
    }
}

void FullscreenQuad::Load() {
    glGenVertexArrays(1, &mArrayObject);
    glGenBuffers(1, &mVertexData);
    glGenBuffers(1, &mIndices);

    glBindVertexArray(mArrayObject);

    float vertexData[] = {
        -1, -1, 0, 0,
        1, -1, 1, 0,
        1, 1, 1, 1,
        -1, 1, 0, 1
    };
    glBindBuffer(GL_ARRAY_BUFFER, mVertexData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    Shaders::GetSingleton()->Attribute("iPosition", 4 * sizeof(float), 0);
    Shaders::GetSingleton()->Attribute("iTexCoord", 4 * sizeof(float), 2 * sizeof(float));

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
