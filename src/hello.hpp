#include <iostream>
#include <v8pp/module.hpp>
#include "v8.h"
#include "libplatform/libplatform.h"

/**
 * 官方简单的 V8 例子，执行字符串想加，并输出结果
 * @param argv
 */
extern void say_hello(char* argv[]) {
    // Initialize V8.
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(__FILE__);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
    printf("version=%s\n", v8::V8::GetVersion());
    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    {
        v8::Isolate::Scope isolate_scope(isolate);
        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);


        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
        v8pp::module myLib(isolate);
        myLib.set_const("PI", 3.1415);
        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);

        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);
        {
            v8::Maybe<bool> success = context->Global()->Set(context, v8::String::NewFromUtf8Literal(isolate, "myLib"),
                                   myLib.new_instance());
            printf("context->Global()->Set() success=%d\n", success.FromJust());
            // Create a string containing the JavaScript source code.
            v8::Local<v8::String> source =
                    v8::String::NewFromUtf8Literal(isolate, "myLib.PI + '#Hello' + ', World!'");

            // Compile the source code.
            v8::Local<v8::Script> script =
                    v8::Script::Compile(context, source).ToLocalChecked();

            // Run the script to get the result.
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(isolate, result);
            printf("result = %s\n", *utf8);
        }
        // Dispose the isolate and tear down V8.
        isolate->Dispose();
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        delete create_params.array_buffer_allocator;
    }
}