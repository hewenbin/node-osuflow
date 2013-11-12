{
	"targets" : [{
		"target_name" : "nof",
		"sources" : [
			"src/nof.cc",
			"src/mpi/node_mpi.cc",
			"src/osuflow/osuflow.cc",
			"src/osuflow/field.cc"
		],
		"include_dirs" : ["deps/osuflow/include"],
		"libraries" : [
			"../deps/osuflow/lib/libOSUFlow.so",
			"../deps/osuflow/lib/libdiy.so"
		]
	}]
}
