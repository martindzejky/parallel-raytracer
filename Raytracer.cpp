#include "Raytracer.hpp"

#include "Window.hpp"
#include "Texture.hpp"


std::shared_ptr<Raytracer> Raytracer::Create() {
    auto raytracer = std::make_shared<Raytracer>();
    SetSingleton(raytracer);
    return raytracer;
}

void Raytracer::RenderOnTexture() {
    int width, height;
    Window::GetSingleton()->GetSize(width, height);

    auto &data = Texture::GetSingleton()->GetData();

    #pragma omp parallel for shared(data)
    for (auto i = 0; i < width * height; ++i) {
        float u = (i % width) / (float) width;
        float v = (i / width) / (float) height;

        float r = 1 - v;
        float g = u;
        float b = v;

        data[i * 3 + 0] = r;
        data[i * 3 + 1] = g;
        data[i * 3 + 2] = b;
    }

    Texture::GetSingleton()->UploadData();

}
