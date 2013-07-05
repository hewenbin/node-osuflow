/*
Unit test for class OSUFlowWrap

Author: Wenbin He
*/

var nof = require('../build/Release/nof');

var osuflow = new nof.OSUFlow();
var min = [], max = [], numSeeds = [2, 2, 2];
var num_seeds = 0, seeds = [];
var streamlines = [];
var pathlines = [];

osuflow.loadData("../../data/vec/tornado/50.vec", true);

osuflow.boundary(min, max);
console.log(min);
console.log(max);

// osuflow.setRandomSeedPoints(min, max, 5);
// seeds = osuflow.getSeeds();
// num_seeds = seeds.length / 3;
// console.log(num_seeds);
// console.log(seeds);
// osuflow.setRegularSeedPoints(min, max, numSeeds);
// seeds = osuflow.getSeeds();
// num_seeds = seeds.length / 3;
// console.log(num_seeds);
// console.log(seeds);
seeds = [1, 2, 3];
osuflow.setSeedPoints(seeds);
seeds = osuflow.getSeeds();
num_seeds = seeds.length / 3;
console.log(num_seeds);
console.log(seeds);

osuflow.setIntegrationParams(1, 0.01, 5);
osuflow.scaleField(20.0);
osuflow.setMaxError(0.0001);

console.log(osuflow.numTimeSteps());
// var time = [];
// osuflow.getMinMaxTime(time);
// console.log(time);

osuflow.genStreamLines(streamlines, 2, 500, 0);
console.log(streamlines[0]);
// osuflow.genPathLines(pathlines, 1, 50, 0.0);
// console.log(pathlines[0]);

osuflow.deleteData();
