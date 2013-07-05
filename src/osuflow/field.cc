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
	delete field;
}

void FieldWrap::Init(Handle<Object> exports) {
	// prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Field"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// prototype
	// feature computation
	tpl->PrototypeTemplate()->Set(String::NewSymbol("unitTangent"),
		FunctionTemplate::New(UnitTangent)->GetFunction());

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

// feature computation
Handle<Value> FieldWrap::UnitTangent(const Arguments& args) {
	HandleScope scope;

	FieldWrap *w = ObjectWrap::Unwrap<FieldWrap>(args.This());
	CVectorField *f = w->field;

	Handle<Array> a_pos = Handle<Array>::Cast(args[0]);
	VECTOR3 pos, tangent;
	pos.Set(a_pos->Get(0)->NumberValue(), a_pos->Get(1)->NumberValue(), a_pos->Get(2)->NumberValue());
	tangent = f->UnitTangent(pos);

	Handle<Array> a_tangent = Array::New(3);
	a_tangent->Set(0, Number::New(tangent[0]));
	a_tangent->Set(1, Number::New(tangent[1]));
	a_tangent->Set(2, Number::New(tangent[2]));

	return scope.Close(a_tangent);
}
