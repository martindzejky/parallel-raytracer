#include "Texture.hpp"

#include <GL/glew.h>

#include "Window.hpp"


std::shared_ptr<Texture> Texture::Create() {
    auto texture = std::make_shared<Texture>();

    texture->Load();

    SetSingleton(texture);
    return texture;
}

void Texture::UploadData() {
    int width, height;
    Window::GetSingleton()->GetSize(width, height);

    glBindTexture(GL_TEXTURE_2D, mId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, mData.data());
}

std::vector<float> &Texture::GetData() {
    return mData;
}

Texture::~Texture() {
    if (mId) {
        glDeleteTextures(1, &mId);
    }
}

void Texture::Load() {
    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height;
    Window::GetSingleton()->GetSize(width, height);
    mData.resize((unsigned long) (width * height * 3));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, mData.data());
}
