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
