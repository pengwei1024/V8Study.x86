#include "hello.hpp"
#include "V8Snapshot.h"
#include "V8Engine.h"
#include <v8pp/context.hpp>

int main(int argc, char *argv[]) {
//    say_hello(argv);

    V8Engine v8Engine;
    v8::Local<v8::Value> result;
    v8::Isolate::Scope isolate_scope(v8Engine.isolate);
    v8::HandleScope handle_scope(v8Engine.isolate);
    v8pp::context context(v8Engine.isolate, v8Engine.create_params.array_buffer_allocator);
    v8::TryCatch tryCatch(v8Engine.isolate);
    result = context.run_file("../test/a.js");
    if (tryCatch.HasCaught()) {
        v8::Local<v8::String> msg = tryCatch.Message()->Get();
        v8::String::Utf8Value msgVal(v8Engine.isolate, msg);
        printf("HasCaught => %s\n", *msgVal);
        return 0;
    }
    v8Engine.printValue(result);

//    bool write = true;
//    V8Snapshot v8Snapshot;
//    v8::StartupData data{nullptr, 0};
//    if (write) {
//        data = v8Snapshot.makeSnapshot();
//    } else {
//        v8Snapshot.readFile(data);
//    }
//    printf("raw_size =%d, IsValid=%d, CanBeRehashed=%d\n", data.raw_size, data.IsValid(), data.CanBeRehashed());
//    if (write) {
//        v8Snapshot.writeFile(data);
//    } else {
//        v8Snapshot.restoreSnapshot(data);
//    }
    return 0;
}
