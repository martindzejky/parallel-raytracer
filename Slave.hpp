#pragma once

#include "Singleton.hpp"


/**
 * Runs on master machine. Renders the final view, controls the slaves.
 */


class Slave : public Singleton<Slave> {

public:

    static std::shared_ptr<Slave> Create();

    void Run();


private:

    bool ShouldExit() const;

};


