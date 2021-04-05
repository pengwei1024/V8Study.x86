//
// Created by Peng,Wei(BAMRD) on 2021/4/5.
//

#include "GlobalObject.h"

int GlobalObject::x = -2;

void GlobalObject::XGetter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value> &info) {
    printf("XGetter=%d\n", x);
    info.GetReturnValue().Set(x);
}

void GlobalObject::XSetter(v8::Local<v8::Name> property, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info) {
    x = value->Int32Value(info.GetIsolate()->GetCurrentContext()).FromJust();
    printf("XSetter=%d\n", x);
}

void GlobalObject::Version(const v8::FunctionCallbackInfo<v8::Value> &args) {
    args.GetReturnValue().Set(
            v8::String::NewFromUtf8(args.GetIsolate(), v8::V8::GetVersion())
                    .ToLocalChecked());
}
