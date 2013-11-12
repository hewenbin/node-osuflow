/*
MPI wrap class header

Author: Wenbin He
*/

#define BUILDING_NODE_EXTENSION
#ifndef MPIWRAP_H
#define MPIWRAP_H

#include <node.h>
#include <mpi.h>

using namespace v8;

// basic mpi functions
Handle<Value> mpi_init(const Arguments& args);
Handle<Value> mpi_finalize(const Arguments& args);
Handle<Value> mpi_comm_size(const Arguments& args);
Handle<Value> mpi_comm_rank(const Arguments& args);

// init mpi functions
void MPIWrap_Init(Handle<Object> exports);

#endif
