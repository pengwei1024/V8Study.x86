//
// Created by Peng,Wei(BAMRD) on 2021/4/5.
//

#ifndef V8STUDY_X86__GLOBALOBJECT_H
#define V8STUDY_X86__GLOBALOBJECT_H

#include <v8.h>

class GlobalObject {
    static int x;
public:
    // 读取 x 的值
    static void XGetter(v8::Local<v8::Name> property,
                        const v8::PropertyCallbackInfo<v8::Value> &info);

    // 设置 x 的 值
    static void XSetter(v8::Local<v8::Name> property, v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void> &info);

    // 获取 V8 版本号
    static void Version(const v8::FunctionCallbackInfo<v8::Value> &args);
};

#endif //V8STUDY_X86__GLOBALOBJECT_H
