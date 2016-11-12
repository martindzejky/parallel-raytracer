#pragma once

#include <vector>

#include "Singleton.hpp"


/**
 * The procedural render target. This is where the magic happens.
 */


class Texture : public Singleton<Texture> {

public:

    typedef unsigned int Id;


public:

    static std::shared_ptr<Texture> Create();

    std::vector<float> &GetData();

    ~Texture();


private:

    void Load();


private:

    Id mId = 0;
    std::vector<float> mData;

};


