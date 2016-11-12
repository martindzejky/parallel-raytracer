#pragma once

#include "Singleton.hpp"


/**
 * Encloses the shader program. Loads shaders, compiles them, and links
 * into a program.
 */


class Shaders : public Singleton<Shaders> {

public:

    typedef unsigned int Id;


public:

    static std::shared_ptr<Shaders> CreateAndLoad();

    ~Shaders();


private:

    void Load();


private:

    Id mVertexShader = 0;
    Id mFragmentShader = 0;
    Id mProgram = 0;

};


