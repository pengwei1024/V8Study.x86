//
// Created by Peng,Wei(BAMRD) on 2021/4/2.
//

#ifndef V8STUDY_X86__V8SNAPSHOT_H
#define V8STUDY_X86__V8SNAPSHOT_H


class V8Snapshot {
public:
    v8::StartupData makeSnapshot();
    void writeFile(v8::StartupData data);
};


#endif //V8STUDY_X86__V8SNAPSHOT_H
