#pragma once

#include "Singleton.hpp"


/**
 * Runs on master machine. Renders the final view, controls the slaves.
 */


class Master : public Singleton<Master> {

public:

    static std::shared_ptr<Master> Create();

    void Run();

};


