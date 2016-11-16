#include <chrono>

#include "Window.hpp"
#include "Input.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include "FullscreenQuad.hpp"
#include "Raytracer.hpp"


int main() {
    auto window = Window::Create(500, 400, "Raytracer - OpenMP / MPI");
    Shaders::CreateAndLoad();
    Texture::Create();
    auto quad = FullscreenQuad::Create();
    auto raytracer = Raytracer::Create();

    auto start = std::chrono::high_resolution_clock::now();
    while (!window->IsClosed()) {
        window->PollEvents();
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            window->Close();
            break;
        }

        auto timestamp = std::chrono::high_resolution_clock::now();
        raytracer->RenderOnTexture(
            std::chrono::duration_cast<std::chrono::milliseconds>(timestamp - start).count() / 1000.f);

        window->Clear();
        quad->Render();
        window->SwapBuffers();
    }

    return 0;
}