/*
Node-osuflow provides native bindings to the osuflow library
as a node.js addon

Author: Wenbin He
*/

#define BUILDING_NODE_EXTENSION
#include <node.h>

#include "osuflow/field.h"
#include "osuflow/osuflow.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
	FieldWrap::Init(exports);
	OSUFlowWrap::Init(exports);
}

NODE_MODULE(nof, InitAll);
