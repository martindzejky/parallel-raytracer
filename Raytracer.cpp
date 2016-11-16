#include "Raytracer.hpp"

#include <limits>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "Window.hpp"
#include "Texture.hpp"


std::shared_ptr<Raytracer> Raytracer::Create() {
    auto raytracer = std::make_shared<Raytracer>();
    SetSingleton(raytracer);
    return raytracer;
}

void Raytracer::RenderOnTexture(float time) const {
    int width, height;
    Window::GetSingleton()->GetSize(width, height);

    auto &data = Texture::GetSingleton()->GetData();

    #pragma omp parallel for shared(data)
    for (auto i = 0; i < width * height; ++i) {
        glm::vec2 uv((i % width) / (float) width, (i / width) / (float) height);
        glm::vec2 uvNorm((uv.x * 2 - 1) * width / height, uv.y * 2 - 1);

        glm::vec3 color = TracePixel(uvNorm, time);

        data[i * 3 + 0] = color.r;
        data[i * 3 + 1] = color.g;
        data[i * 3 + 2] = color.b;
    }

    Texture::GetSingleton()->UploadData();
}

glm::vec3 Raytracer::TracePixel(const glm::vec2 &uv, float time) const {
    glm::vec3 cameraTarget(0, .5f, 0);
    glm::vec3 rayOrigin;
    glm::vec3 rayDirection;

    MakeRay(time, uv, cameraTarget, rayOrigin, rayDirection);
    glm::vec3 color = ShootRay(rayOrigin, rayDirection);

    return glm::vec3(glm::sqrt(color.r), glm::sqrt(color.g), glm::sqrt(color.b));
}

void Raytracer::MakeRay(float time, const glm::vec2 &uvNorm, const glm::vec3 &cameraTarget, glm::vec3 &rayOrigin,
                        glm::vec3 &rayDirection) const {
    float cameraOffset = 2;
    float cameraHeight = 1.2f;

    rayOrigin = glm::vec3(std::cos(time) * cameraOffset, cameraHeight, std::sin(time) * cameraOffset);

    glm::vec3 ww = glm::normalize(cameraTarget - rayOrigin);
    glm::vec3 uu = glm::normalize(cross(ww, glm::vec3(0, 1, 0)));
    glm::vec3 vv = glm::normalize(cross(uu, ww));

    rayDirection = glm::normalize(uvNorm.x * uu + uvNorm.y * vv + 2.f * ww);
}

glm::vec3 Raytracer::ShootRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) const {
    glm::vec3 hitPoint;
    Object hitObject = Object::None;

    float hitDistance = 0;
    for (auto i = 0; i < 256; ++i) {
        glm::vec3 point = rayOrigin + rayDirection * hitDistance;
        float distance;
        Object object = Object::None;

        Map(point, distance, object);

        if (distance < .00001f) {
            hitPoint = point;
            hitObject = object;
            break;
        }

        hitDistance += distance;
    }

    glm::vec3 hitColor;
    switch (hitObject) {
        default:
            hitColor = mBackgroundColor;
        case Object::Ground:
            hitColor = glm::vec3(.45f, .44f, .4f);
            break;
        case Object::Battery:
            hitColor = glm::vec3(1, .2f, .2f);
            break;
    }

    float f = glm::exp(-hitDistance * hitDistance * .1f);
    glm::vec3 colorWithFog = glm::mix(mBackgroundColor, hitColor, f);

    return colorWithFog;
}

void Raytracer::Map(const glm::vec3 &point, float &distance, Raytracer::Object &object) const {
    float minDistance = std::numeric_limits<float>::max();

    // battery
    auto boxPoint = glm::max(glm::abs(point) - glm::vec3(.5f, 1, .5f), 0.f);
    auto boxBody = glm::sqrt(boxPoint.x * boxPoint.x + boxPoint.y * boxPoint.y + boxPoint.z * boxPoint.z);
    auto sphere = glm::sqrt(glm::pow(point.x, 2) + glm::pow(point.y - .5f, 2) + glm::pow(point.z, 2)) - .6f;
    float battery = glm::max(-sphere, (double) boxBody);
    if (battery < minDistance) {
        minDistance = battery;
        object = Object::Battery;
    }

    // ground
    auto ground = point.y;
    if (ground < minDistance) {
        minDistance = ground;
        object = Object::Ground;
    }

    distance = minDistance;
}
