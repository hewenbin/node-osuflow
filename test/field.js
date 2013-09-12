/*
Unit test for class FieldWrap

Author: Wenbin He
*/

var nof = require('../build/Release/nof');

var osuflow = new nof.OSUFlow();
osuflow.loadData("../../data/vec/tornado.vec", true);

var field = osuflow.getFlowField();

var pos = [23.5, 23.5, 23.5], vec = [], lqdg = [];
var streamline = [1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0],
	curvature = [];

field.at_phys(pos, 0, vec);
console.log(vec);

field.lqdg(pos, lqdg);
console.log(lqdg);

field.curvature(streamline, curvature);
console.log(curvature);

osuflow.deleteData();
