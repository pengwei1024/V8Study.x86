//
// Created by Peng,Wei(BAMRD) on 2021/4/8.
//

#include <iostream>
#include "Console.h"

void Console::Assert(const v8::FunctionCallbackInfo<v8::Value> &args) {
    print(__FUNCTION__, args);
}

void Console::Log(const v8::FunctionCallbackInfo<v8::Value> &args) {
    print(__FUNCTION__, args);
}

void Console::Error(const v8::FunctionCallbackInfo<v8::Value> &args) {
    print(__FUNCTION__, args);
}

void Console::Warn(const v8::FunctionCallbackInfo<v8::Value> &args) {
    print(__FUNCTION__, args);
}

void Console::Info(const v8::FunctionCallbackInfo<v8::Value> &args) {
    print(__FUNCTION__, args);
}

void Console::Debug(const v8::FunctionCallbackInfo<v8::Value> &args) {
    print(__FUNCTION__, args);
}

void Console::print(const char* type, const v8::FunctionCallbackInfo<v8::Value> &args) {
    v8::HandleScope handle_scope(args.GetIsolate());

    for (int i = 0; i < args.Length(); ++i)
    {
        if (i == 0) {
            std::cout << "[" << type << "]";
        } else {
            std::cout << ' ';
        }
        v8::String::Utf8Value str(args.GetIsolate(), args[i]);
        std::cout <<  *str;
    }
    std::cout << std::endl;
}
