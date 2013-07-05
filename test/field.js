/*
Unit test for class FieldWrap

Author: Wenbin He
*/

var nof = require('../build/Release/nof');

var osuflow = new nof.OSUFlow();
osuflow.loadData("../../data/vec/tornado.vec", true);

var field = osuflow.getFlowField();

console.log(field.unitTangent([25, 25, 25]));

osuflow.deleteData();
