//
// Created by Peng,Wei(BAMRD) on 2021/4/2.
//

#ifndef V8STUDY_X86__V8SNAPSHOT_H
#define V8STUDY_X86__V8SNAPSHOT_H


class V8Snapshot {
    std::unique_ptr<v8::Platform> platform;
    v8::Isolate::CreateParams createParams;
public:
    /**
     * V8 生成 snapshot
     * @return
     */
    v8::StartupData makeSnapshot();

    /**
     * 恢复 snapshot
     * @param data snapshot data
     * @param createPlatform 是否创建 Platform
     */
    void restoreSnapshot(v8::StartupData &data, bool createPlatform = true);

    /**
     * 将 StartupData 写入文件
     * @param data
     */
    void writeFile(v8::StartupData data);

    void readFile(v8::StartupData & data);
};


#endif //V8STUDY_X86__V8SNAPSHOT_H
