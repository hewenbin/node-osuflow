/*
Unit test for basic mpi functions

Author: Wenbin He
*/

var nof = require('../build/Release/nof');

nof.mpi_init();
var size = nof.mpi_comm_size();
var rank = nof.mpi_comm_rank();
console.log('I\'m ' + rank + ' of ' + size + '!');
nof.mpi_finalize();
