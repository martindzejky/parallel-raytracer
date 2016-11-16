#include "Window.hpp"
#include "Input.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include "FullscreenQuad.hpp"
#include "Raytracer.hpp"


int main() {
    auto window = Window::Create(800, 600, "Raytracer - OpenMP / MPI");
    Shaders::CreateAndLoad();
    Texture::Create();
    auto quad = FullscreenQuad::Create();

    auto raytracer = Raytracer::Create();
    raytracer->RenderOnTexture();

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