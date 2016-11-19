#include <chrono>
#include <iostream>
#include <omp.h>

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

    #pragma omp parallel
    #pragma omp master
    {
        std::cout << "Number of threads: " << omp_get_num_threads() << std::endl;
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto last = std::chrono::high_resolution_clock::now();
    std::cout << "Time it takes to render a frame:\n";

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

        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp - last).count();
        last = timestamp;
        std::cout << delta << "ms\n";
    }

    return 0;
}