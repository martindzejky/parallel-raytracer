#pragma once

#include <glm/vec3.hpp>
#include "Singleton.hpp"


/**
 * The most important part. Takes a render texture and shoots a ray
 * into the world to get the color of each pixel.
 */


class Raytracer : public Singleton<Raytracer> {

public:

    static std::shared_ptr<Raytracer> Create();

    void RenderOnTexture() const;


private:

    glm::vec3 TracePixel(const glm::vec2 &uv, float aspectRatio, float time) const;
    void MakeRay(float time, const glm::vec2 &uvNorm, float cameraOffset, float cameraHeight,
                 const glm::vec3 &cameraTarget, glm::vec3 &rayOrigin, glm::vec3 &rayDirection) const;
    glm::vec3 ShootRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) const;
};


