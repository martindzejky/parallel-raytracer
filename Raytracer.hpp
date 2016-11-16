#pragma once

#include <glm/vec3.hpp>

#include "Singleton.hpp"


/**
 * The most important part. Takes a render texture and shoots a ray
 * into the world to get the color of each pixel.
 */


class Raytracer : public Singleton<Raytracer> {

public:

    enum class Object {
        None,
        Ground,
        Battery
    };

public:

    static std::shared_ptr<Raytracer> Create();

    void RenderOnTexture(float time) const;


private:

    glm::vec3 TracePixel(const glm::vec2 &uv, float time) const;
    void MakeRay(float time, const glm::vec2 &uvNorm, const glm::vec3 &cameraTarget, glm::vec3 &rayOrigin,
                     glm::vec3 &rayDirection) const;
    glm::vec3 ShootRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) const;
    void Map(const glm::vec3 &point, float &distance, Object &object) const;


private:

    const glm::vec3 mBackgroundColor = glm::vec3(.01f, .01f, .015f);

};


