#include "Master.hpp"

#include <chrono>
#include <iostream>
#include <mpi.h>

#include "Window.hpp"
#include "Input.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include "FullscreenQuad.hpp"
#include "Raytracer.hpp"


std::shared_ptr<Master> Master::Create() {
    auto master = std::make_shared<Master>();
    SetSingleton(master);
    return master;
}

void Master::Run(unsigned int machineCount) {
    auto window = Window::Create(500, 400, "Raytracer - OpenMP / MPI");
    Shaders::CreateAndLoad();
    Texture::Create();
    auto quad = FullscreenQuad::Create();
    auto raytracer = Raytracer::Create();

    auto start = std::chrono::high_resolution_clock::now();
    auto last = std::chrono::high_resolution_clock::now();

    int width, height;
    Window::GetSingleton()->GetSize(width, height);

    int maxI = width * height / machineCount;
    BroadcastSlaveParts(machineCount, width * height);

    std::cout << "Time it takes to render a frame:\n";
    while (!window->IsClosed()) {
        window->PollEvents();
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            window->Close();
            break;
        }

        auto timestamp = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp - last).count();
        last = timestamp;
        std::cout << delta << "ms\n";

        auto &data = Texture::GetSingleton()->GetData();

        #pragma omp parallel for default(shared)
        for (auto i = 0; i < maxI; ++i) {
            glm::vec2 uv((i % width) / (float) width, (i / width) / (float) height);
            glm::vec2 uvNorm((uv.x * 2 - 1) * width / height, uv.y * 2 - 1);

            float time = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp - start).count() / 1000.f;
            glm::vec3 color = raytracer->TracePixel(uvNorm, time);

            data[i * 3 + 0] = color.r;
            data[i * 3 + 1] = color.g;
            data[i * 3 + 2] = color.b;
        }

        Texture::GetSingleton()->UploadData();

        window->Clear();
        quad->Render();
        window->SwapBuffers();
    }
}

void Master::BroadcastSlaveParts(unsigned int machineCount, int size) {
    std::cout << "Sending the slave data parts\n";
    for (auto i = 1; i < machineCount; ++i) {
        int slavePart[] = { size / machineCount * i, size / machineCount * (i + 1) };
        MPI_Send(slavePart, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
}
