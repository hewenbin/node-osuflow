/*
OSUFlow wrap class source

Author: Wenbin He
*/

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <list>
#include <iterator>
#include "field.h"
#include "osuflow.h"

using namespace v8;

Persistent<Function> OSUFlowWrap::constructor;

OSUFlowWrap::OSUFlowWrap() {
	osuflow = new OSUFlow();
}

OSUFlowWrap::~OSUFlowWrap() {
	delete osuflow;
}

void OSUFlowWrap::Init(Handle<Object> exports) {
	// prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("OSUFlow"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// prototype
	// load and delete data
	tpl->PrototypeTemplate()->Set(String::NewSymbol("loadData"),
		FunctionTemplate::New(LoadData)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("loadDataCurvilinear"),
		FunctionTemplate::New(LoadDataCurvilinear)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("deleteData"),
		FunctionTemplate::New(DeleteData)->GetFunction());

	// set and get flow field
	tpl->PrototypeTemplate()->Set(String::NewSymbol("getFlowField"),
		FunctionTemplate::New(GetFlowField)->GetFunction());

	// set and get value of the properties
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setBoundary"),
		FunctionTemplate::New(SetBoundary)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("boundary"),
		FunctionTemplate::New(Boundary)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("getGlobalBounds"),
		FunctionTemplate::New(GetGlobalBounds)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("setSeedPoints"),
		FunctionTemplate::New(SetSeedPoints)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setRandomSeedPoints"),
		FunctionTemplate::New(SetRandomSeedPoints)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setRegularSeedPoints"),
		FunctionTemplate::New(SetRegularSeedPoints)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("getSeeds"),
		FunctionTemplate::New(GetSeeds)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("setIntegrationParams"),
		FunctionTemplate::New(SetIntegrationParams)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setMaxError"),
		FunctionTemplate::New(SetMaxError)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setInitialStepSize"),
		FunctionTemplate::New(SetInitialStepSize)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setMinStepSize"),
		FunctionTemplate::New(SetMinStepSize)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setMaxStepSize"),
		FunctionTemplate::New(SetMaxStepSize)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setLowerAngleAccuracy"),
		FunctionTemplate::New(SetLowerAngleAccuracy)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setUpperAngleAccuracy"),
		FunctionTemplate::New(SetUpperAngleAccuracy)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setIntegrationOrder"),
		FunctionTemplate::New(SetIntegrationOrder)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("setUseAdaptiveStepSize"),
		FunctionTemplate::New(SetUseAdaptiveStepSize)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("normalizeField"),
		FunctionTemplate::New(NormalizeField)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("scaleField"),
		FunctionTemplate::New(ScaleField)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("translateField"),
		FunctionTemplate::New(TranslateField)->GetFunction());

	tpl->PrototypeTemplate()->Set(String::NewSymbol("numTimeSteps"),
		FunctionTemplate::New(NumTimeSteps)->GetFunction());
	// tpl->PrototypeTemplate()->Set(String::NewSymbol("getMinMaxTime"),
	// 	FunctionTemplate::New(GetMinMaxTime)->GetFunction());

	// generate streamlines
	tpl->PrototypeTemplate()->Set(String::NewSymbol("genStreamLines"),
		FunctionTemplate::New(GenStreamLines)->GetFunction());

	// generate pathlines
	tpl->PrototypeTemplate()->Set(String::NewSymbol("genPathLines"),
		FunctionTemplate::New(GenPathLines)->GetFunction());

	// // write flow lines into a file
	// tpl->PrototypeTemplate()->Set(String::NewSymbol("writeFlowlines"),
	// 	FunctionTemplate::New(WriteFlowlines)->GetFunction());

	constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("OSUFlow"), constructor);
}

Handle<Value> OSUFlowWrap::New(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = new OSUFlowWrap();
	w->Wrap(args.This());

	return args.This();
}

// load and delete data
Handle<Value> OSUFlowWrap::LoadData(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	String::AsciiValue filename(args[0]->ToString());
	o->LoadData(*filename, args[1]->BooleanValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::LoadDataCurvilinear(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	String::AsciiValue filename(args[0]->ToString());
	Handle<Array> a_min = Handle<Array>::Cast(args[2]);
	Handle<Array> a_max = Handle<Array>::Cast(args[3]);
	VECTOR3 min, max;
	min.Set(a_min->Get(0)->NumberValue(), a_min->Get(1)->NumberValue(), a_min->Get(2)->NumberValue());
	max.Set(a_max->Get(0)->NumberValue(), a_max->Get(1)->NumberValue(), a_max->Get(2)->NumberValue());
	o->LoadDataCurvilinear(*filename, args[1]->BooleanValue(), min, max);

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::DeleteData(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->DeleteData();

	return scope.Close(Undefined());
}

// set and get flow field
Handle<Value> OSUFlowWrap::GetFlowField(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	return scope.Close(FieldWrap::NewInstance(o->GetFlowField()));
}

// set and get value of the properties
Handle<Value> OSUFlowWrap::SetBoundary(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	Handle<Array> a_min = Handle<Array>::Cast(args[0]);
	Handle<Array> a_max = Handle<Array>::Cast(args[1]);
	VECTOR3 min, max;
	min.Set(a_min->Get(0)->NumberValue(), a_min->Get(1)->NumberValue(), a_min->Get(2)->NumberValue());
	max.Set(a_max->Get(0)->NumberValue(), a_max->Get(1)->NumberValue(), a_max->Get(2)->NumberValue());
	o->SetBoundary(min, max);

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::Boundary(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	VECTOR3 min, max;
	o->Boundary(min, max);

	Handle<Array> a_min = Handle<Array>::Cast(args[0]);
	Handle<Array> a_max = Handle<Array>::Cast(args[1]);

	a_min->Set(0, Number::New(min.x()));
	a_min->Set(1, Number::New(min.y()));
	a_min->Set(2, Number::New(min.z()));
	a_max->Set(0, Number::New(max.x()));
	a_max->Set(1, Number::New(max.y()));
	a_max->Set(2, Number::New(max.z()));

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::GetGlobalBounds(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	VECTOR3 min, max;
	o->GetGlobalBounds(min, max);

	Handle<Array> a_min = Handle<Array>::Cast(args[0]);
	Handle<Array> a_max = Handle<Array>::Cast(args[1]);

	a_min->Set(0, Number::New(min.x()));
	a_min->Set(1, Number::New(min.y()));
	a_min->Set(2, Number::New(min.z()));
	a_max->Set(0, Number::New(max.x()));
	a_max->Set(1, Number::New(max.y()));
	a_max->Set(2, Number::New(max.z()));

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetSeedPoints(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	Handle<Array> a_seeds = Handle<Array>::Cast(args[0]);
	int num_elements = a_seeds->Length();
	VECTOR3 *seeds = new VECTOR3[num_elements / 3];
	for (int i = 0; i < num_elements; i += 3) {
		seeds[i / 3].Set(a_seeds->Get(i)->NumberValue(), a_seeds->Get(i + 1)->NumberValue(), a_seeds->Get(i + 2)->NumberValue());
	}
	o->SetSeedPoints(seeds, num_elements / 3);

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetRandomSeedPoints(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	float min[3], max[3];
	Handle<Array> a_min = Handle<Array>::Cast(args[0]);
	Handle<Array> a_max = Handle<Array>::Cast(args[1]);
	min[0] = a_min->Get(0)->NumberValue();
	min[1] = a_min->Get(1)->NumberValue();
	min[2] = a_min->Get(2)->NumberValue();
	max[0] = a_max->Get(0)->NumberValue();
	max[1] = a_max->Get(1)->NumberValue();
	max[2] = a_max->Get(2)->NumberValue();
	o->SetRandomSeedPoints(min, max, args[2]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetRegularSeedPoints(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	float min[3], max[3];
	size_t numSeeds[3];
	Handle<Array> a_min = Handle<Array>::Cast(args[0]);
	Handle<Array> a_max = Handle<Array>::Cast(args[1]);
	Handle<Array> a_numSeeds = Handle<Array>::Cast(args[2]);
	min[0] = a_min->Get(0)->NumberValue();
	min[1] = a_min->Get(1)->NumberValue();
	min[2] = a_min->Get(2)->NumberValue();
	max[0] = a_max->Get(0)->NumberValue();
	max[1] = a_max->Get(1)->NumberValue();
	max[2] = a_max->Get(2)->NumberValue();
	numSeeds[0] = a_numSeeds->Get(0)->NumberValue();
	numSeeds[1] = a_numSeeds->Get(1)->NumberValue();
	numSeeds[2] = a_numSeeds->Get(2)->NumberValue();
	o->SetRegularSeedPoints(min, max, numSeeds);

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::GetSeeds(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	int nSeeds;
	VECTOR3* seeds = o->GetSeeds(nSeeds);
	Handle<Array> a_seeds = Array::New(nSeeds * 3);
	for (int i = 0; i < nSeeds; i++) {
		a_seeds->Set(i * 3, Number::New(seeds[i].x()));
		a_seeds->Set(i * 3 + 1, Number::New(seeds[i].y()));
		a_seeds->Set(i * 3 + 2, Number::New(seeds[i].z()));
	}

	return scope.Close(a_seeds);
}

Handle<Value> OSUFlowWrap::SetIntegrationParams(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	if (args.Length() == 2)
		o->SetIntegrationParams(args[0]->NumberValue(), args[1]->NumberValue());
	else if (args.Length() == 3)
		o->SetIntegrationParams(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetMaxError(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->SetMaxError(args[0]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetInitialStepSize(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->SetInitialStepSize(args[0]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetMinStepSize(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->SetMinStepSize(args[0]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetMaxStepSize(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->SetMaxStepSize(args[0]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetLowerAngleAccuracy(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->SetLowerAngleAccuracy(args[0]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetUpperAngleAccuracy(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->SetUpperAngleAccuracy(args[0]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetIntegrationOrder(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->SetIntegrationOrder(INTEG_ORD(args[0]->NumberValue()));

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::SetUseAdaptiveStepSize(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->SetUseAdaptiveStepSize(args[0]->BooleanValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::NormalizeField(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->NormalizeField(args[0]->BooleanValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::ScaleField(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	o->ScaleField(args[0]->NumberValue());

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::TranslateField(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	Handle<Array> a_translate = Handle<Array>::Cast(args[0]);
	VECTOR3 translate;
	translate.Set(a_translate->Get(0)->NumberValue(), a_translate->Get(1)->NumberValue(), a_translate->Get(2)->NumberValue());
	o->TranslateField(translate);

	return scope.Close(Undefined());
}

Handle<Value> OSUFlowWrap::NumTimeSteps(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	return scope.Close(Number::New(o->NumTimeSteps()));
}

// Handle<Value> OSUFlowWrap::GetMinMaxTime(const Arguments& args) {
// 	HandleScope scope;

// 	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
// 	OSUFlow *o = w->osuflow;

// 	int min, max;
// 	o->GetMinMaxTime(min, max);
// 	Handle<Array> a_time = Handle<Array>::Cast(args[0]);
// 	a_time->Set(0, Number::New(min));
// 	a_time->Set(1, Number::New(max));

// 	return scope.Close(Undefined());
// }

// generate streamlines
Handle<Value> OSUFlowWrap::GenStreamLines(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	list<vtListSeedTrace*> list;
	bool s = o->GenStreamLines(list, TRACE_DIR(args[1]->NumberValue()), args[2]->NumberValue(), args[3]->NumberValue());

	Handle<Array> streamlines = Handle<Array>::Cast(args[0]);
	std::list<vtListSeedTrace*>::iterator pIter;
	int i, j;
	for (pIter = list.begin(), i = 0; pIter != list.end(); pIter++, i++) {
		vtListSeedTrace *trace = *pIter;
		Handle<Array> streamline = Array::New(trace->size() * 3);
		std::list<VECTOR3*>::iterator pnIter;
		for (pnIter = trace->begin(), j = 0; pnIter != trace->end(); pnIter++, j += 3) {
			VECTOR3 p = **pnIter;
			streamline->Set(j, Number::New(p.x()));
			streamline->Set(j + 1, Number::New(p.y()));
			streamline->Set(j + 2, Number::New(p.z()));
		}
		streamlines->Set(i, streamline);
	}

	return scope.Close(Boolean::New(s));
}

// generate pathlines
Handle<Value> OSUFlowWrap::GenPathLines(const Arguments& args) {
	HandleScope scope;

	OSUFlowWrap *w = ObjectWrap::Unwrap<OSUFlowWrap>(args.This());
	OSUFlow *o = w->osuflow;

	list<vtListTimeSeedTrace*> list;
	bool s = o->GenPathLines(list, TIME_DIR(args[1]->NumberValue()), args[2]->NumberValue(), args[3]->NumberValue());

	Handle<Array> pathlines = Handle<Array>::Cast(args[0]);
	std::list<vtListTimeSeedTrace*>::iterator pIter;
	int i, j;
	for (pIter = list.begin(), i = 0; pIter != list.end(); pIter++, i++) {
		vtListTimeSeedTrace *trace = *pIter;
		Handle<Array> pathline = Array::New(trace->size() * 4);
		std::list<VECTOR4*>::iterator pnIter;
		for (pnIter = trace->begin(), j = 0; pnIter != trace->end(); pnIter++, j += 4) {
			VECTOR4 p = **pnIter;
			pathline->Set(j, Number::New(p[0]));
			pathline->Set(j + 1, Number::New(p[1]));
			pathline->Set(j + 2, Number::New(p[2]));
			pathline->Set(j + 3, Number::New(p[3]));
		}
		pathlines->Set(i, pathline);
	}

	return scope.Close(Boolean::New(s));
}
