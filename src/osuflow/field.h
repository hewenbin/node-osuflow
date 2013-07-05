/*
Field wrap class header

Author: Wenbin He
*/

#ifndef FIELDWRAP_H
#define FIELDWRAP_H

#include <node.h>
#include "Field.h"

using namespace v8;

class FieldWrap : public node::ObjectWrap {
public:
	static void Init(Handle<Object> exports);
	static Handle<Value> NewInstance(CVectorField *field);

private:
	FieldWrap();
	~FieldWrap();
	static Persistent<Function> constructor;
	static Handle<Value> New(const Arguments& args);

	// wrapped function
	// feature computation
	static Handle<Value> UnitTangent(const Arguments& args);

	// wrapped object
	CVectorField *field;
};

#endif
