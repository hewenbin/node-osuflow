/*
Unit test for class FieldWrap

Author: Wenbin He
*/

var nof = require('../build/Release/nof');

var osuflow = new nof.OSUFlow();
osuflow.loadData("../../data/vec/tornado.vec", true);

var field = osuflow.getFlowField();
var pos = [1, 2, 3], vec = [];
field.at_phys(pos, 0, vec);

console.log(vec);

osuflow.deleteData();
