#include "Raytracer.hpp"

#include <glm/glm.hpp>

#include "Window.hpp"
#include "Texture.hpp"


std::shared_ptr<Raytracer> Raytracer::Create() {
    auto raytracer = std::make_shared<Raytracer>();
    SetSingleton(raytracer);
    return raytracer;
}

void Raytracer::RenderOnTexture() const {
    int width, height;
    Window::GetSingleton()->GetSize(width, height);

    auto &data = Texture::GetSingleton()->GetData();

    #pragma omp parallel for shared(data)
    for (auto i = 0; i < width * height; ++i) {
        glm::vec2 uv((i % width) / (float) width, (i / width) / (float) height);
        glm::vec3 color = TracePixel(uv, (float) width / height, .2);

        data[i * 3 + 0] = color.r;
        data[i * 3 + 1] = color.g;
        data[i * 3 + 2] = color.b;
    }

    Texture::GetSingleton()->UploadData();

}

glm::vec3 Raytracer::TracePixel(const glm::vec2 &uv, float aspectRatio, float time) const {
    glm::vec2 uvNorm((uv.x * 2 - 1) * aspectRatio, uv.y * 2 - 1);

    float cameraOffset = 2;
    float cameraHeight = 1.5f;
    glm::vec3 cameraTarget(0, .5, 0);
    glm::vec3 rayOrigin;
    glm::vec3 rayDirection;

    MakeRay(time, uvNorm, cameraOffset, cameraHeight, cameraTarget, rayOrigin, rayDirection);
    glm::vec3 color = ShootRay(rayOrigin, rayDirection);

    return glm::vec3(glm::sqrt(color.r), glm::sqrt(color.g), glm::sqrt(color.b));
}

void Raytracer::MakeRay(float time, const glm::vec2 &uvNorm, float cameraOffset, float cameraHeight,
                        const glm::vec3 &cameraTarget, glm::vec3 &rayOrigin, glm::vec3 &rayDirection) const {
    rayOrigin = glm::vec3(std::cos(time) * cameraOffset, cameraHeight, std::sin(time) * cameraOffset);

    glm::vec3 ww = normalize(cameraTarget - rayOrigin);
    glm::vec3 uu = normalize(cross(ww, glm::vec3(0, 1, 0)));
    glm::vec3 vv = normalize(cross(uu, ww));

    rayDirection = normalize(uvNorm.x * uu + uvNorm.y * vv + 2.f * ww);
}

glm::vec3 Raytracer::ShootRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) const {
    return glm::vec3(.01);
}
