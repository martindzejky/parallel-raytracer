#include "Slave.hpp"

#include <iostream>
#include <mpi.h>


std::shared_ptr<Slave> Slave::Create() {
    auto master = std::make_shared<Slave>();
    SetSingleton(master);
    return master;
}

void Slave::Run() {
    std::cout << "Waiting for the data part\n";
    int dataPart[2];
    MPI_Recv(dataPart, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::cout << "This slave will compute the data part " << dataPart[0] << ":" << dataPart[1] << std::endl;
}
