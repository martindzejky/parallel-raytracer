#pragma once

#include "Singleton.hpp"


/**
 * Runs on master machine. Renders the final view, controls the slaves.
 */


class Master : public Singleton<Master> {

public:

    static std::shared_ptr<Master> Create();

    void Run(unsigned int machineCount);


private:

    void BroadcastSlaveParts(unsigned int machineCount, int size, int width, int height);
    void BroadcastExit(unsigned int machineCount);
    void BroadcastTime(float time, unsigned int machineCount);

};


