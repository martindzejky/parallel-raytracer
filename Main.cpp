#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Input.hpp"
#include "Shaders.hpp"


int main() {
    auto window = Window::Create(800, 600, "Raytracer - OpenMP / MPI");
    Shaders::CreateAndLoad();

    while (true) {
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            break;
        }

        window->PollEvents();
    }

    return 0;
}