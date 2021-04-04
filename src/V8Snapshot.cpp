//
// Created by Peng,Wei(BAMRD) on 2021/4/2.
//

#include <v8.h>
#include "V8Snapshot.h"
#include "V8Engine.h"

using namespace v8;

v8::StartupData V8Snapshot::makeSnapshot() {
    V8Engine v8Engine;
    v8::StartupData data{nullptr, 0};
    v8::SnapshotCreator creator(v8Engine.isolate);
//    {
        v8::HandleScope scope(v8Engine.isolate);
        v8::Local<v8::Context> context = v8::Context::New(v8Engine.isolate);
        v8::Context::Scope context_scope(context);
        {
            v8::Local<v8::String> source =
                    v8::String::NewFromUtf8(v8Engine.isolate, "0.1+0.2").ToLocalChecked();
            v8::Local<v8::Script> script =
                    v8::Script::Compile(context, source).ToLocalChecked();
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
//            v8::String::Utf8Value value(v8Engine.isolate, result);
//            printf("result = %s\n", *value);
            printf("aa");
            creator.SetDefaultContext(context);
        }
//    }
    data = creator.CreateBlob(v8::SnapshotCreator::FunctionCodeHandling::kKeep);
    printf("end!!!");

//    printf("raw_size =%d, IsValid=%d, CanBeRehashed=%d", data.raw_size, data.IsValid(), data.CanBeRehashed());
    return data;
//    auto data = result;

//    return result;
}

void V8Snapshot::writeFile(v8::StartupData data) {
    FILE *file = fopen("./out/a.blob", "w");
    fseek(file, 0, SEEK_END);
    rewind(file);
    int writeSize = fwrite(data.data, data.raw_size, 1, file);
    printf("write size=%d", writeSize);
}
