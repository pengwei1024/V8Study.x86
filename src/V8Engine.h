//
// Created by Peng,Wei(BAMRD) on 2021/4/2.
//

#ifndef V8STUDY_X86__V8ENGINE_H
#define V8STUDY_X86__V8ENGINE_H

#include <v8.h>
#include "libplatform/libplatform.h"


class V8Engine {
public:
    V8Engine();
    V8Engine(std::vector<intptr_t> external_references);

    ~V8Engine();

    v8::Isolate *isolate;

    v8::Isolate::CreateParams create_params;

    /**
     * 执行 js 脚本
     * @param js js 脚本
     * @return 执行结果 v8::Value
     */
    bool runScript(const std::string &js, v8::Local<v8::Value>& result) const;

private:
    std::unique_ptr<v8::Platform> platform;
};


#endif //V8STUDY_X86__V8ENGINE_H
