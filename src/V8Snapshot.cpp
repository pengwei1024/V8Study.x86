//
// Created by Peng,Wei(BAMRD) on 2021/4/2.
// https://github.com/tosone/bundleNode/blob/aaf04fd200a56f3f5b60c1c691877c192c6448af/deps/v8/src/startup-data-util.cc
//

#include <v8.h>
#include "V8Snapshot.h"
#include "V8Engine.h"
#include "GlobalObject.h"
#include "Utils.hpp"

#ifdef WIN32
#include <io.h>
#include <direct.h>
#else

#include <unistd.h>
#include <sys/stat.h>

#endif

#include <stdint.h>
#include <string>

#define MAX_PATH_LEN 256

#ifdef WIN32
#define ACCESS(fileName,accessMode) _access(fileName,accessMode)
#define MKDIR(path) _mkdir(path)
#else
#define ACCESS(fileName, accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

#define MAX_PATH_LEN 256

using namespace v8;

v8::StartupData V8Snapshot::makeSnapshot() {
    auto external_references = createExtRef();
    V8Engine v8Engine(external_references);
    v8::StartupData data{nullptr, 0};
    v8::SnapshotCreator creator(v8Engine.isolate, external_references.data(), nullptr);
    {
        v8::HandleScope scope(v8Engine.isolate);
        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(v8Engine.isolate);
        global->SetAccessor(String::NewFromUtf8(v8Engine.isolate, "x").ToLocalChecked(), GlobalObject::XGetter,
                            GlobalObject::XSetter);
        printf("GlobalObject::Version point=%p\n", &GlobalObject::Version);
        Local<FunctionTemplate> fun = v8::FunctionTemplate::New(v8Engine.isolate, GlobalObject::Version);
        global->Set(v8Engine.isolate, "version", fun);
        v8::Local<v8::Context> context = v8::Context::New(v8Engine.isolate, nullptr, global);
        v8::Context::Scope context_scope(context);
        {
            v8::Local<v8::String> source =
                    v8::String::NewFromUtf8(v8Engine.isolate,
                                            "if(typeof x == 'undefined'){x = 0}x++;function add(a,b) {return a+b;}var kk='##@';(typeof version === 'function'? version(): 'None')+ add(0.1,x);").ToLocalChecked();
            v8::Local<v8::Script> script =
                    v8::Script::Compile(context, source).ToLocalChecked();
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
            v8::String::Utf8Value value(v8Engine.isolate, result);
            printf("result = %s\n", *value);
            // 必须要调用 AddContext
            creator.AddContext(context);
            creator.SetDefaultContext(context);
        }
    }
    data = creator.CreateBlob(v8::SnapshotCreator::FunctionCodeHandling::kKeep);
    // 立即从 snapshot 恢复 context
    restoreSnapshot(data, false);
    return data;
}

// 从左到右依次判断文件夹是否存在,不存在就创建
// example: /home/root/mkdir/1/2/3/4/
// 注意:最后一个如果是文件夹的话,需要加上 '\' 或者 '/'
int32_t createDirectory(const std::string &directoryPath) {
    uint32_t dirPathLen = directoryPath.length();
    if (dirPathLen > MAX_PATH_LEN) {
        return -1;
    }
    char tmpDirPath[MAX_PATH_LEN] = {0};
    for (uint32_t i = 0; i < dirPathLen; ++i) {
        tmpDirPath[i] = directoryPath[i];
        if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/') {
            if (ACCESS(tmpDirPath, 0) != 0) {
                int32_t ret = MKDIR(tmpDirPath);
                if (ret != 0) {
                    return ret;
                }
            }
        }
    }
    return 0;
}

void V8Snapshot::restoreSnapshot(v8::StartupData &data, const bool createPlatform) {
    printf("## restoreSnapshot ##\n");
    if (createPlatform) {
        v8::V8::InitializeICU();
        v8::V8::InitializeExternalStartupDataFromFile(__FILE__);
        platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(platform.get());
        v8::V8::Initialize();
        printf("version=%s\n", v8::V8::GetVersion());
    }
    // Create a new Isolate and make it the current one.
    createParams = v8::Isolate::CreateParams();
    createParams.array_buffer_allocator =
            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    createParams.snapshot_blob = &data;
    createParams.external_references = createExtRef().data();
    v8::Isolate *isolate = v8::Isolate::New(createParams);
    {
        v8::HandleScope handle_scope(isolate);
        v8::TryCatch tryCatch(isolate);
        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate);
//        v8::Local<v8::Context> context = v8::Context::FromSnapshot(isolate, 0).ToLocalChecked();
        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);
        {
            v8::Local<v8::String> source =
                    v8::String::NewFromUtf8Literal(isolate,
                                                   "(typeof version === 'function')? x + version():'not found version()'");
            // Compile the source code.
            v8::Local<v8::Script> script =
                    v8::Script::Compile(context, source).ToLocalChecked();
            v8::MaybeLocal<v8::Value> localResult = script->Run(context);
            if (tryCatch.HasCaught() || localResult.IsEmpty()) {
                Local<String> msg = tryCatch.Message()->Get();
                v8::String::Utf8Value msgVal(isolate, msg);
                printf("HasCaught => %s\n", *msgVal);
            } else {
                v8::Local<v8::Value> result = localResult.ToLocalChecked();
                // Convert the result to an UTF8 string and print it.
                v8::String::Utf8Value utf8(isolate, result);
                printf("restore add() = %s\n", *utf8);
            }
        }
    }
    if (createPlatform) {
        isolate->Dispose();
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        delete createParams.array_buffer_allocator;
    }
}


void V8Snapshot::writeFile(v8::StartupData data) {
    char currentPath[MAX_PATH_LEN];
    getcwd(currentPath, MAX_PATH_LEN);
    printf("current path =%s\n", currentPath);
    std::string path = currentPath;
    FILE *file = fopen((path + "/a.blob").c_str(), "w");
    fseek(file, 0, SEEK_END);
    rewind(file);
    int writeSize = fwrite(data.data, data.raw_size, 1, file);
    printf("write size=%d\n", writeSize);
    fclose(file);
}

void V8Snapshot::readFile(v8::StartupData &data) {
    char currentPath[MAX_PATH_LEN];
    getcwd(currentPath, MAX_PATH_LEN);
    printf("current path =%s\n", currentPath);
    std::string path = currentPath;
    FILE *file = fopen((path + "/a.blob").c_str(), "rb");
    fseek(file, 0, SEEK_END);
    data.raw_size = static_cast<int>(ftell(file));
    rewind(file);
    data.data = new char[data.raw_size];
    int read_size = static_cast<int>(fread(const_cast<char *>(data.data),
                                           1, data.raw_size, file));
    fclose(file);
    printf("readFile ## raw_size =%d, IsValid=%d, CanBeRehashed=%d, read_size=%d\n",
           data.raw_size, data.IsValid(), data.CanBeRehashed(), read_size);
}

std::vector<intptr_t> V8Snapshot::createExtRef() {
    std::vector<intptr_t> external_references;
    external_references.reserve(3);
    external_references.push_back((intptr_t) &GlobalObject::Version);
    external_references.push_back((intptr_t) &GlobalObject::XGetter);
    external_references.push_back((intptr_t) &GlobalObject::XSetter);
    return external_references;
}
