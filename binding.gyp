{
	"targets" : [{
		"target_name" : "nof",
		"sources" : [
			"src/nof.cc",
			"src/osuflow/osuflow.cc",
			"src/osuflow/field.cc"
		],
		"include_dirs" : ["deps/osuflow/include"],
		"libraries" : [
			"../deps/osuflow/lib/libOSUFlow.so",
			"../deps/osuflow/lib/liblibgcb.so",
			"../deps/osuflow/lib/librenderer.so",
			"../deps/osuflow/lib/libdiy.so"
		]
	}]
}
