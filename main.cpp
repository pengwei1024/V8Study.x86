#include "hello.hpp"
#include "V8Engine.h"

int main(int argc, char *argv[]) {
//    say_hello(argv);
    printf("###\n");
    V8Engine v8Engine;

    v8::Local<v8::Value> result;
    v8::Isolate::Scope isolate_scope(v8Engine.isolate);
    v8::HandleScope handle_scope(v8Engine.isolate);
    v8Engine.runScript("'----hello'", result);
    printf("IsEmpty=%d, IsString=%d\n", result.IsEmpty(), result->IsString());
    v8::String::Utf8Value utf8(v8Engine.isolate, result);
    printf("runScript result = %s\n", *utf8);
    return 0;
}
