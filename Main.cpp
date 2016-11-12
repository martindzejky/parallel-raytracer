#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Input.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include "FullscreenQuad.hpp"


int main() {
    auto window = Window::Create(800, 600, "Raytracer - OpenMP / MPI");
    Shaders::CreateAndLoad();
    Texture::Create();
    auto quad = FullscreenQuad::Create();

    while (true) {
        window->PollEvents();
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            break;
        }

        window->Clear();
        quad->Render();
        window->SwapBuffers();
    }

    return 0;
}