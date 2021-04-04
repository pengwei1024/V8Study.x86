//
// Created by Peng,Wei(BAMRD) on 2021/4/2.
//

#include <v8.h>
#include "V8Snapshot.h"
#include "V8Engine.h"

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
    V8Engine v8Engine;
    v8::StartupData data{nullptr, 0};
    v8::SnapshotCreator creator(v8Engine.isolate);
    {
        v8::HandleScope scope(v8Engine.isolate);
        v8::Local<v8::Context> context = v8::Context::New(v8Engine.isolate);
        v8::Context::Scope context_scope(context);
        {
            v8::Local<v8::String> source =
                    v8::String::NewFromUtf8(v8Engine.isolate,
                                            "function add(a,b) {return a+b;}add(0.1,0.3);").ToLocalChecked();
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

void V8Snapshot::restoreSnapshot(v8::StartupData data, const bool createPlatform) {
    if (createPlatform) {
        v8::V8::InitializeICU();
        std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(platform.get());
        v8::V8::Initialize();
        printf("version=%s\n", v8::V8::GetVersion());
    }
    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    create_params.snapshot_blob = &data;
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    {
        v8::HandleScope handle_scope(isolate);
        // Create a new context.
//        v8::Local<v8::Context> context = v8::Context::New(isolate);

        v8::Local<v8::Context> context = v8::Context::FromSnapshot(isolate, 0).ToLocalChecked();
        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);
        {
            v8::Local<v8::String> source =
                    v8::String::NewFromUtf8Literal(isolate, "add(1,11)");
            // Compile the source code.
            v8::Local<v8::Script> script =
                    v8::Script::Compile(context, source).ToLocalChecked();
            // Run the script to get the result.
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(isolate, result);
            printf("restore add() = %s\n", *utf8);
        }
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
}

v8::StartupData V8Snapshot::readFile() {
    v8::StartupData data{nullptr, 0};
    char currentPath[MAX_PATH_LEN];
    getcwd(currentPath, MAX_PATH_LEN);
    printf("current path =%s\n", currentPath);
    std::string path = currentPath;
    std::ifstream in(path + "/a.blob");
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    data.raw_size = contents.size();
    data.data = contents.c_str();
    printf("readFile ## raw_size =%d, IsValid=%d, CanBeRehashed=%d\n",
           data.raw_size, data.IsValid(), data.CanBeRehashed());
    return data;
}
