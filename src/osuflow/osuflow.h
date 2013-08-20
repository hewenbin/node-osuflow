/*
OSUFlow wrap class header

Author: Wenbin He
*/

#define BUILDING_NODE_EXTENSION
#ifndef OSUFLOWWRAP_H
#define OSUFLOWWRAP_H

#include <node.h>
#include "OSUFlow.h"

using namespace v8;

class OSUFlowWrap : public node::ObjectWrap {
public:
	static void Init(Handle<Object> exports);

private:
	OSUFlowWrap();
	~OSUFlowWrap();
	static Persistent<Function> constructor;
	static Handle<Value> New(const Arguments& args);

	// wrapped function
	// load and delete data
	static Handle<Value> LoadData(const Arguments& args);
	static Handle<Value> LoadDataCurvilinear(const Arguments& args);
	static Handle<Value> DeleteData(const Arguments& args);

	// set and get flow field
	static Handle<Value> GetFlowField(const Arguments& args);

	// set and get value of the properties
	static Handle<Value> SetBoundary(const Arguments& args);
	static Handle<Value> Boundary(const Arguments& args);
	static Handle<Value> GetGlobalBounds(const Arguments& args);

	static Handle<Value> SetSeedPoints(const Arguments& args);
	static Handle<Value> SetRandomSeedPoints(const Arguments& args);
	static Handle<Value> SetRegularSeedPoints(const Arguments& args);
	static Handle<Value> GetSeeds(const Arguments& args);

	static Handle<Value> SetIntegrationParams(const Arguments& args);
	static Handle<Value> SetMaxError(const Arguments& args);
	static Handle<Value> SetInitialStepSize(const Arguments& args);
	static Handle<Value> SetMinStepSize(const Arguments& args);
	static Handle<Value> SetMaxStepSize(const Arguments& args);
	static Handle<Value> SetLowerAngleAccuracy(const Arguments& args);
	static Handle<Value> SetUpperAngleAccuracy(const Arguments& args);
	static Handle<Value> SetIntegrationOrder(const Arguments& args);
	static Handle<Value> SetUseAdaptiveStepSize(const Arguments& args);

	static Handle<Value> NormalizeField(const Arguments& args);
	static Handle<Value> ScaleField(const Arguments& args);
	static Handle<Value> TranslateField(const Arguments& args);

	static Handle<Value> NumTimeSteps(const Arguments& args);
	// static Handle<Value> GetMinMaxTime(const Arguments& args);

	// generate streamlines
	static Handle<Value> GenStreamLines(const Arguments& args);

	// generate pathlines
	static Handle<Value> GenPathLines(const Arguments& args);

	// // write flow lines into a file
	// static Handle<Value> WriteFlowlines(const Arguments& args);

	// wrapped object
	OSUFlow *osuflow;
};

#endif
