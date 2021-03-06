/*
Field wrap class source

Author: Wenbin He
*/

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "field.h"

using namespace v8;

Persistent<Function> FieldWrap::constructor;

FieldWrap::FieldWrap() {}

FieldWrap::~FieldWrap() {
	field = NULL;
}

void FieldWrap::Init(Handle<Object> exports) {
	// prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Field"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// prototype
	// to obtain vector data
	tpl->PrototypeTemplate()->Set(String::NewSymbol("at_phys"),
		FunctionTemplate::New(at_phys)->GetFunction());
	// feature computation
	tpl->PrototypeTemplate()->Set(String::NewSymbol("generateVortexMetrics"),
		FunctionTemplate::New(GenerateVortexMetrics)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("generateVortexMetricsLine"),
		FunctionTemplate::New(GenerateVortexMetricsLine)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("curvature"),
		FunctionTemplate::New(Curvature)->GetFunction());

	constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol("Field"), constructor);
}

Handle<Value> FieldWrap::New(const Arguments& args) {
	HandleScope scope;

	FieldWrap *w = new FieldWrap();
	w->Wrap(args.This());

	return args.This();
}

Handle<Value> FieldWrap::NewInstance(CVectorField *field) {
	HandleScope scope;

	Local<Object> instance = constructor->NewInstance(0, NULL);
	FieldWrap *w = ObjectWrap::Unwrap<FieldWrap>(instance);
	w->field = field;

	return scope.Close(instance);
}

// to obtain vector data
Handle<Value> FieldWrap::at_phys(const Arguments& args) {
	HandleScope scope;

	FieldWrap *w = ObjectWrap::Unwrap<FieldWrap>(args.This());
	CVectorField *f = w->field;

	Handle<Array> a_pos = Handle<Array>::Cast(args[0]);
	Handle<Array> a_vec = Handle<Array>::Cast(args[2]);
	VECTOR3 pos, vec;
	pos.Set(a_pos->Get(0)->NumberValue(), a_pos->Get(1)->NumberValue(), a_pos->Get(2)->NumberValue());
	f->at_phys(pos, args[1]->NumberValue(), vec);
	a_vec->Set(0, Number::New(vec[0]));
	a_vec->Set(1, Number::New(vec[1]));
	a_vec->Set(2, Number::New(vec[2]));

	return scope.Close(Undefined());
}

// feature computation
Handle<Value> FieldWrap::GenerateVortexMetrics(const Arguments& args) {
	HandleScope scope;

	FieldWrap *w = ObjectWrap::Unwrap<FieldWrap>(args.This());
	CVectorField *f = w->field;

	Handle<Array> a_pos = Handle<Array>::Cast(args[0]);
	Handle<Array> a_lqdg = Handle<Array>::Cast(args[1]);
	VECTOR3 pos;
	pos.Set(a_pos->Get(0)->NumberValue(), a_pos->Get(1)->NumberValue(), a_pos->Get(2)->NumberValue());
	float lambda2, q, delta, gamma2;
	f->GenerateVortexMetrics(pos, lambda2, q, delta, gamma2);
	a_lqdg->Set(0, Number::New(lambda2));
	a_lqdg->Set(1, Number::New(q));
	a_lqdg->Set(2, Number::New(delta));
	a_lqdg->Set(3, Number::New(gamma2));

	return scope.Close(Undefined());
}

Handle<Value> FieldWrap::GenerateVortexMetricsLine(const Arguments& args) {
	HandleScope scope;

	FieldWrap *w = ObjectWrap::Unwrap<FieldWrap>(args.This());
	CVectorField *f = w->field;

	Handle<Array> a_fieldline = Handle<Array>::Cast(args[0]);
	Handle<Array> a_lambda2 = Handle<Array>::Cast(args[1]);
	Handle<Array> a_q = Handle<Array>::Cast(args[2]);
	Handle<Array> a_delta = Handle<Array>::Cast(args[3]);
	Handle<Array> a_gamma2 = Handle<Array>::Cast(args[4]);

	int num = a_fieldline->Length();
	VECTOR3* fieldline = new VECTOR3[num / 3];
	float* lambda2 = new float[num / 3];
	float* q = new float[num / 3];
	float* delta = new float[num / 3];
	float* gamma2 = new float[num / 3];

	for (int i = 0; i < num; i += 3) {
		fieldline[i / 3].Set(a_fieldline->Get(i)->NumberValue(), a_fieldline->Get(i + 1)->NumberValue(), a_fieldline->Get(i + 2)->NumberValue());
	}

	f->GenerateVortexMetricsLine(fieldline, num / 3, lambda2, q, delta, gamma2);

	for (int i = 0; i < num / 3; i++) {
		a_lambda2->Set(i, Number::New(lambda2[i]));
		a_q->Set(i, Number::New(q[i]));
		a_delta->Set(i, Number::New(delta[i]));
		a_gamma2->Set(i, Number::New(gamma2[i]));
	}

	delete[] fieldline;
	delete[] lambda2; delete[] q; delete[] delta; delete[] gamma2;

	return scope.Close(Undefined());
}

Handle<Value> FieldWrap::Curvature(const Arguments& args) {
	HandleScope scope;

	FieldWrap *w = ObjectWrap::Unwrap<FieldWrap>(args.This());
	CVectorField *f = w->field;

	Handle<Array> a_fieldline = Handle<Array>::Cast(args[0]);
	Handle<Array> a_curvature = Handle<Array>::Cast(args[1]);

	int num = a_fieldline->Length();
	VECTOR3* fieldline = new VECTOR3[num / 3];
	float* curvature = new float[num / 3];

	for (int i = 0; i < num; i += 3) {
		fieldline[i / 3].Set(a_fieldline->Get(i)->NumberValue(), a_fieldline->Get(i + 1)->NumberValue(), a_fieldline->Get(i + 2)->NumberValue());
	}

	f->Curvature(fieldline, num / 3, curvature);

	for (int i = 0; i < num / 3; i++) {
		a_curvature->Set(i, Number::New(curvature[i]));
	}

	delete[] fieldline;
	delete[] curvature;

	return scope.Close(Undefined());
}
