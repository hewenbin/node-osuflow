/*
Websocket server for node-osuflow, an improvement of osuflow_s.js

Author: Wenbin He
*/

var socketio = require('socket.io');	// install socket.io first
var nof = require('../../build/Release/nof');	// put your nof filepath here
var io;

// osuflow objects
var osuflow = new nof.OSUFlow();
osuflow.loadDataCurvilinear("../../data/plot3d/channel", true);	// put your vec/plot3d data path here
var field = osuflow.getFlowField();
var min = [], max = [];
osuflow.boundary(min, max);
osuflow.setIntegrationParams(1, 1, 2);

// clients' information
var clientNumber = 0;
var seeds = {};

exports.listen = function (server) {
	io = socketio.listen(server);
	io.set("log level", 1);

	io.sockets.on("connection", function (socket) {
		clientNumber++;
		console.log("Client: " + socket.id + " connect in.");

		// send boundary to the client
		socket.emit("boundary", {
			min : min,
			max : max
		});

		// handle requests
		handleAt_phys(socket, field);
		handleSetSeedPoints(socket, seeds);
		handleGenStreamLines(socket, osuflow, seeds);
		handleDisconnection(socket, seeds);
	});
};

function handleAt_phys(socket, field) {
	socket.on("velocity", function (pos) {
		var velocity = [];
		field.at_phys(pos, 0, velocity);

		socket.emit("velocity", velocity);
		velocity.length = 0;
	});
}

function handleSetSeedPoints(socket, seeds) {
	socket.on("setSeedPoints", function (seedsIn) {
		seeds[socket.id] = seedsIn.slice(0);
	});
}

function handleGenStreamLines(socket, osuflow, seeds) {
	socket.on("genStreamLines", function (params) {
		// set seeds
		osuflow.setSeedPoints(seeds[socket.id]);

		// generate streamlines
		var streamlines = [];
		osuflow.genStreamLines(streamlines, params.direction, params.maxpoints, 0);

		socket.emit("streamlines", streamlines);

		// clean streamlines
		streamlines.length = 0;
	});
}

function handleDisconnection(socket, seeds) {
	socket.on("disconnect", function () {
		delete seeds[socket.id];
		clientNumber--;
		console.log("Client: " + socket.id + " disconnect.");
	});
}
