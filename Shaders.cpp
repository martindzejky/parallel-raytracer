#include "Shaders.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include "Error.hpp"


std::shared_ptr<Shaders> Shaders::CreateAndLoad() {
    auto shaders = std::make_shared<Shaders>();

    shaders->Load();

    SetSingleton(shaders);
    return shaders;
}

Shaders::~Shaders() {
    if (mProgram) {
        glDeleteProgram(mProgram);
    }
    if (mFragmentShader) {
        glDeleteShader(mFragmentShader);
    }
    if (mVertexShader) {
        glDeleteShader(mVertexShader);
    }
}

void Shaders::Load() {
    mVertexShader = glCreateShader(GL_VERTEX_SHADER);
    mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    mProgram = glCreateProgram();

    std::ifstream vertexFile("Vertex.glsl");
    if (!vertexFile.is_open()) {
        THROW_ERROR("Cannot open vertex shader file");
    }

    std::ifstream fragmentFile("Fragment.glsl");
    if (!fragmentFile.is_open()) {
        THROW_ERROR("Cannot open fragment shader file");
    }

    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();
    std::string vertexSource = vertexStream.str();
    const char* vertexSourcePtr = vertexSource.c_str();
    glShaderSource(mVertexShader, 1, &vertexSourcePtr, nullptr);

    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();
    std::string fragmentSource = fragmentStream.str();
    const char* fragmentSourcePtr = fragmentSource.c_str();
    glShaderSource(mFragmentShader, 1, &fragmentSourcePtr, nullptr);

    glCompileShader(mVertexShader);
    glCompileShader(mFragmentShader);
    glAttachShader(mProgram, mVertexShader);
    glAttachShader(mProgram, mFragmentShader);
    glBindFragDataLocation(mProgram, 0, "oColor");
    glLinkProgram(mProgram);
    glUseProgram(mProgram);
}
