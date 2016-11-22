#include "Slave.hpp"

#include <iostream>


std::shared_ptr<Slave> Slave::Create() {
    auto master = std::make_shared<Slave>();
    SetSingleton(master);
    return master;
}

void Slave::Run() {
    std::cout << "Slave is empty for now...\n";
}
