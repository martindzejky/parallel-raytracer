#include <chrono>
#include <iostream>
#include <omp.h>
#include <mpi.h>

#include "Window.hpp"
#include "Input.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include "FullscreenQuad.hpp"
#include "Raytracer.hpp"


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int machineCount;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &machineCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::cout << "Number of machines in the cluster: " << machineCount << std::endl;
    std::cout << "This machine's rank: " << rank << std::endl;

    #pragma omp parallel
    #pragma omp master
    {
        std::cout << "Number of threads: " << omp_get_num_threads() << std::endl;
    }

    auto window = Window::Create(500, 400, "Raytracer - OpenMP / MPI");
    Shaders::CreateAndLoad();
    Texture::Create();
    auto quad = FullscreenQuad::Create();
    auto raytracer = Raytracer::Create();

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

    MPI_Finalize();

    return 0;
}