/*
MPI wrap class source

Author: Wenbin He
*/

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "node_mpi.h"

using namespace v8;

// MPI_Init don't receive arguments from the command line
Handle<Value> mpi_init(const Arguments& args) {
	HandleScope scope;

	MPI_Init(NULL, NULL);

	return scope.Close(Undefined());
}

Handle<Value> mpi_finalize(const Arguments& args) {
	HandleScope scope;

	MPI_Finalize();

	return scope.Close(Undefined());
}

// MPI_Comm_size, MPI_Comm_rank use MPI_COMM_WORLD as the unique communicator
Handle<Value> mpi_comm_size(const Arguments& args) {
	HandleScope scope;

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	return scope.Close(Number::New(size));
}

Handle<Value> mpi_comm_rank(const Arguments& args) {
	HandleScope scope;

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	return scope.Close(Number::New(rank));
}

// init mpi functions
void MPIWrap_Init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("mpi_init"),
		FunctionTemplate::New(mpi_init)->GetFunction());

	exports->Set(String::NewSymbol("mpi_finalize"),
		FunctionTemplate::New(mpi_finalize)->GetFunction());

	exports->Set(String::NewSymbol("mpi_comm_size"),
		FunctionTemplate::New(mpi_comm_size)->GetFunction());

	exports->Set(String::NewSymbol("mpi_comm_rank"),
		FunctionTemplate::New(mpi_comm_rank)->GetFunction());
}
