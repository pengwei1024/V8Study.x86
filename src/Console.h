//
// Created by Peng,Wei(BAMRD) on 2021/4/8.
//

#ifndef V8STUDY_X86__CONSOLE_H
#define V8STUDY_X86__CONSOLE_H


#include <v8.h>
#include <v8pp/module.hpp>

class Console {
public:
    static void Assert(v8::FunctionCallbackInfo<v8::Value> const& args);
    static void Log(v8::FunctionCallbackInfo<v8::Value> const& args);
    static void Error(v8::FunctionCallbackInfo<v8::Value> const& args);
    static void Warn(v8::FunctionCallbackInfo<v8::Value> const& args);
    static void Info(v8::FunctionCallbackInfo<v8::Value> const& args);
    static void Debug(v8::FunctionCallbackInfo<v8::Value> const& args);

    static v8pp::module createModule(v8::Isolate *isolate) {
        v8pp::module consoleLModule(isolate);
        consoleLModule.set("log", Console::Log);
        consoleLModule.set("debug", Console::Debug);
        consoleLModule.set("error", Console::Error);
        consoleLModule.set("info", Console::Info);
        consoleLModule.set("warn", Console::Warn);
        return consoleLModule;
    }

private:
    static void print(const char* type, v8::FunctionCallbackInfo<v8::Value> const& args);
};


#endif //V8STUDY_X86__CONSOLE_H
