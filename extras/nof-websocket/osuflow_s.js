/*
Websocket server for osuflow wrap class

Author: Wenbin He
*/

var socketio = require('socket.io');	// install socket.io first
var nof = require('../../build/Release/nof');	// put your nof filepath here

var io;
var clientNumber = 0;

exports.listen = function (server) {
	io = socketio.listen(server);
	io.set("log level", 1);

	io.sockets.on("connection", function (socket) {
		clientNumber++;

		// objects maintained for each connection
		var osuflow = new nof.OSUFlow();
		var min = [], max = [];
		var streamlines = [];
		var pathlines = [];

		// handle requests
		handleLoadData(socket, osuflow, min, max);
		handleLoadDataCurvilinear(socket, osuflow, min, max);
		handleSetSeedPoints(socket, osuflow);
		handleSetIntegrationParams(socket, osuflow);
		handleSetMaxError(socket, osuflow);
		handleSetLowerAngleAccuracy(socket, osuflow);
		handleSetUpperAngleAccuracy(socket, osuflow);
		handleSetUseAdaptiveStepSize(socket, osuflow);
		handelTranslateField(socket, osuflow);
		handleGenStreamLines(socket, osuflow, streamlines);
		handleGenPathLines(socket, osuflow, pathlines);
		handleDisconnection(socket, osuflow);
	});
};

function handleLoadData(socket, osuflow, min, max) {
	socket.on("loadData", function (data) {
		osuflow.loadData("data/vec/" + data.filepath, data.steady);	// put your vec data path here
		osuflow.boundary(min, max);

		socket.emit("boundary", {
			min : min,
			max : max
		});
	});
}

function handleLoadDataCurvilinear(socket, osuflow, min, max) {
	socket.on("loadDataCurvilinear", function (data) {
		osuflow.loadDataCurvilinear("data/plot3d/" + data.filepath, data.steady, data.min, data.max);	// put your plot3d data path here
		osuflow.boundary(min, max);

		socket.emit("boundary", {
			min : min,
			max : max
		});
	});
}

function handleSetSeedPoints(socket, osuflow) {
	socket.on("setSeedPoints", function (seeds) {
		osuflow.setSeedPoints(seeds);
	});
}

function handleSetIntegrationParams(socket, osuflow) {
	socket.on("setIntegrationParams", function (params) {
		osuflow.setIntegrationParams(params.init, params.min, params.max);
	});
}

function handleSetMaxError(socket, osuflow) {
	socket.on("setMaxError", function (maxError) {
		osuflow.setMaxError(maxError);
	});
}

function handleSetLowerAngleAccuracy(socket, osuflow) {
	socket.on("setLowerAngleAccuracy", function (accuracy) {
		osuflow.setLowerAngleAccuracy(accuracy);
	});
}

function handleSetUpperAngleAccuracy(socket, osuflow) {
	socket.on("setUpperAngleAccuracy", function (accuracy) {
		osuflow.setUpperAngleAccuracy(accuracy);
	});
}

function handleSetUseAdaptiveStepSize(socket, osuflow) {
	socket.on("setUseAdaptiveStepSize", function (use) {
		osuflow.setUseAdaptiveStepSize(use);
	});
}

function handelTranslateField(socket, osuflow) {
	socket.on("translateField", function (translate) {
		osuflow.translateField(translate);
	});
}

function handleGenStreamLines(socket, osuflow, streamlines) {
	socket.on("genStreamLines", function (params) {
		streamlines.length = 0;
		osuflow.genStreamLines(streamlines, params.direction, params.maxpoints, 0);

		socket.emit("streamlines", streamlines);
	});
}

function handleGenPathLines(socket, osuflow, pathlines) {
	socket.on("genPathLines", function (params) {
		pathlines.length = 0;
		osuflow.genPathLines(pathlines, params.direction, params.maxpoints, params.ctime);

		socket.emit("pathlines", pathlines);
	});
}

function handleDisconnection(socket, osuflow) {
	socket.on("disconnect", function () {
		clientNumber--;
		osuflow.deleteData();
		delete osuflow;
	});
}
