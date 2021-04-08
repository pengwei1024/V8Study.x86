//
// Created by Peng,Wei(BAMRD) on 2021/4/2.
//


#include "V8Engine.h"

V8Engine::V8Engine(std::vector<intptr_t> external_references) {
    v8::V8::InitializeICU();
    v8::V8::InitializeExternalStartupData(__FILE__);
    platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
    this->create_params = v8::Isolate::CreateParams();
    this->create_params.external_references = external_references.data();
    this->create_params.array_buffer_allocator =
            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    this->isolate = v8::Isolate::New(this->create_params);
}

V8Engine::V8Engine(): V8Engine({
                                       reinterpret_cast<intptr_t>(nullptr)}) {
}

V8Engine::~V8Engine() {
//    isolate->Dispose();
//    v8::V8::Dispose();
//    v8::V8::ShutdownPlatform();
//    delete this->create_params.array_buffer_allocator;
    printf("~V8Engine\n");
}

bool V8Engine::runScript(v8::Local<v8::Context> context,
                         const std::string &js, v8::Local<v8::Value>& result) const {
    v8::TryCatch tryCatch(isolate);
    v8::Context::Scope context_scope(context);
    {
        v8::Local<v8::String> source =
                v8::String::NewFromUtf8(isolate, js.c_str()).ToLocalChecked();
        v8::Local<v8::Script> script =
                v8::Script::Compile(context, source).ToLocalChecked();
        auto localResult = script->Run(context);
        if (tryCatch.HasCaught() || localResult.IsEmpty()) {
            v8::Local<v8::String> msg = tryCatch.Message()->Get();
            v8::String::Utf8Value msgVal(isolate, msg);
            printf("HasCaught => %s\n", *msgVal);
            return false;
        }
        result = localResult.ToLocalChecked();
    }
    return true;
}

void V8Engine::printValue(const v8::Local<v8::Value> &value) {
    printf("IsEmpty=%d, IsString=%d, \n", value.IsEmpty(), value->IsString());
    v8::String::Utf8Value utf8(isolate, value);
    printf("runScript result = %s\n", *utf8);
}

