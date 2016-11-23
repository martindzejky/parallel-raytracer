#include "Raytracer.hpp"


std::shared_ptr<Raytracer> Raytracer::Create() {
    auto raytracer = std::make_shared<Raytracer>();
    SetSingleton(raytracer);
    return raytracer;
}

glm::vec3 Raytracer::TracePixel(const glm::vec2 &uv, float time) const {
    glm::vec3 cameraTarget(0, .5f, 0);
    glm::vec3 rayOrigin;
    glm::vec3 rayDirection;

    MakeRay(time, uv, cameraTarget, rayOrigin, rayDirection);
    glm::vec3 color = ShootRay(time, rayOrigin, rayDirection);

    return glm::vec3(glm::sqrt(color.r), glm::sqrt(color.g), glm::sqrt(color.b));
}

void Raytracer::MakeRay(float time, const glm::vec2 &uvNorm, const glm::vec3 &cameraTarget, glm::vec3 &rayOrigin,
                        glm::vec3 &rayDirection) const {
    float cameraOffset = 2 + .8f * glm::sin(time / 12);
    float cameraHeight = 1.6f + .7f * glm::sin(time / 12);

    rayOrigin = glm::vec3(std::cos(time / 10) * cameraOffset, cameraHeight, std::sin(time / 10) * cameraOffset);

    glm::vec3 ww = glm::normalize(cameraTarget - rayOrigin);
    glm::vec3 uu = glm::normalize(cross(ww, glm::vec3(0, 1, 0)));
    glm::vec3 vv = glm::normalize(cross(uu, ww));

    rayDirection = glm::normalize(uvNorm.x * uu + uvNorm.y * vv + 2.f * ww);
}

glm::vec3 Raytracer::ShootRay(float time, const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) const {
    glm::vec3 hitPoint;
    Object hitObject = Object::None;

    float hitDistance = 0;
    for (auto i = 0; i < 256; ++i) {
        glm::vec3 point = rayOrigin + rayDirection * hitDistance;
        Object object = Object::None;
        float distance = Map(time, point, object);

        if (distance < .00001f) {
            hitPoint = point;
            hitObject = object;
            break;
        }

        hitDistance += distance;
    }

    auto specular = .0f;

    glm::vec3 hitColor;
    switch (hitObject) {
        default:
            hitColor = mBackgroundColor;
        case Object::Ground:
            hitColor = glm::vec3(.45f, .44f, .4f);
            specular = .2f;
            break;
        case Object::Battery:
            hitColor = glm::vec3(1, .1f, .1f);
            specular = 1.f;
            break;
        case Object::BatteryCore:
            hitColor = glm::vec3(.2f, 1, .25f);
            break;
    }

    if (hitObject != Object::None) {
        auto normal = MapNormal(time, hitPoint);
        auto reflected = glm::reflect(mSun, normal);

        auto diff = glm::max(0.f, glm::dot(normal, mSun));
        auto spec = specular * glm::max(0.f, glm::pow(glm::max(0.f, glm::dot(reflected, rayDirection)), 16.f));
        auto shad = .0f;

        if (diff > .00001f) {
            shad = Shadow(time, hitPoint, normal);
        }

        hitColor = glm::clamp((spec + diff * hitColor) * shad +
                              mAmbient * hitColor * glm::smoothstep(-1.5f, 1.f, normal.y), 0.f, 1.f);
    }

    float f = glm::exp(-hitDistance * hitDistance * .02f);
    glm::vec3 colorWithFog = glm::mix(mBackgroundColor, hitColor, f);

    return colorWithFog;
}

float Raytracer::Map(float time, const glm::vec3 &point, Raytracer::Object &object) const {
    float minDistance = std::numeric_limits<float>::max();

    // battery
    auto batteryBoxVector = glm::max(glm::abs(point) - glm::vec3(.5f, 1, .5f), 0.f);
    auto batteryBox = glm::sqrt(batteryBoxVector.x * batteryBoxVector.x + batteryBoxVector.y * batteryBoxVector.y +
                                batteryBoxVector.z * batteryBoxVector.z);
    auto batterySphere = glm::sqrt(glm::pow(point.x, 2) + glm::pow(point.y - .5f, 2) + glm::pow(point.z, 2)) - .65f;
    float battery = glm::max(-batterySphere, (double) batteryBox);
    if (battery < minDistance) {
        minDistance = battery;
        object = Object::Battery;
    }

    // battery core
    auto coreBoxVector = glm::max(glm::abs(point) - glm::vec3(.35f, 1.15f + .3f * glm::sin(time / 14 + 2), .35f), 0.f);
    auto coreBox = glm::sqrt(coreBoxVector.x * coreBoxVector.x + coreBoxVector.y * coreBoxVector.y +
                             coreBoxVector.z * coreBoxVector.z);
    auto coreSphere = glm::sqrt(glm::pow(point.x, 2) +
                                glm::pow(point.y - .8f - .3f * glm::sin(time / 14 + 2), 2) +
                                glm::pow(point.z, 2)) - .4f;
    float core = glm::max(coreSphere, (double) coreBox);
    if (core < minDistance) {
        minDistance = core;
        object = Object::BatteryCore;
    }

    // ground
    auto ground = point.y;
    if (ground < minDistance) {
        minDistance = ground;
        object = Object::Ground;
    }

    return minDistance;
}

glm::vec3 Raytracer::MapNormal(float time, const glm::vec3 &point) const {
    float e = .0001f;
    glm::vec3 n;

    Object object;
    n.x = Map(time, glm::vec3(point.x + e, point.y, point.z), object) -
          Map(time, glm::vec3(point.x - e, point.y, point.z), object);
    n.y = Map(time, glm::vec3(point.x, point.y + e, point.z), object) -
          Map(time, glm::vec3(point.x, point.y - e, point.z), object);
    n.z = Map(time, glm::vec3(point.x, point.y, point.z + e), object) -
          Map(time, glm::vec3(point.x, point.y, point.z - e), object);

    return glm::normalize(n);
}

float Raytracer::Shadow(float time, const glm::vec3 &point, const glm::vec3 normal)const {
    glm::vec3 origin = point + normal * .001f;
    const glm::vec3 &dir = mSun;

    auto result = 1.f;
    auto t = .002f;
    for (auto i = 0; i < 64; ++i) {
        glm::vec3 mapPoint = origin + dir * t;
        Object object;
        float distance = Map(time, mapPoint, object);
        result = glm::min(result, 10 * distance / t);

        if (distance < .00001f) {
            break;
        }

        t += glm::clamp(distance, .01f, .02f);
    }

    return glm::clamp(result, 0.f, 1.f);
}
