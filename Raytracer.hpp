#pragma once

#include "Singleton.hpp"


/**
 * The most important part. Takes a render texture and shoots a ray
 * into the world to get the color of each pixel.
 */


class Raytracer : public Singleton<Raytracer> {

public:

    static std::shared_ptr<Raytracer> Create();

    void RenderOnTexture();

};


