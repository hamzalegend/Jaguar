#include "ScriptGlue.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include <string>
#include <fstream>
#include <iostream>

#include <math/math.h>
#define JR_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Jaguar.InternalCalls::"#Name, Name)

static void NativeLog(MonoString* text, int par)
{
    char* cStr = mono_string_to_utf8(text);
    std::cout << cStr << ", " << par << "\n";
    mono_free(cStr);
}

static void NativeLogv(glm::vec3* v, glm::vec3* outResult)
{
    std::cout << v->x << ", " << v->y << ", " << v->z << "\n";
    *outResult = glm::cross(*v, glm::vec3(v->x, v->y, -v->z));
}



void ScriptGLue::RegisterFunctions()
{
	JR_ADD_INTERNAL_CALL(NativeLog);
    JR_ADD_INTERNAL_CALL(NativeLogv);
}
