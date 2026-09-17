#pragma once

#include <jni.h>
#include <stdint.h>

namespace zygisk {

enum Option : int {
    DLCLOSE_MODULE_LIBRARY = 0,
    FORCE_DENYLIST_UNMOUNT = 1,
};

enum StateFlag : uint32_t {
    PROCESS_ON_DENYLIST = 1 << 0,
    PROCESS_ROOT_GRANTED = 1 << 1,
};

struct AppSpecializeArgs {
    jint &uid;
    jint &gid;
    jintArray &gids;
    jint &runtime_flags;
    jint &mount_external;
    jstring &se_info;
    jstring &nice_name;
    jboolean &is_child_zygote;
    jstring &instruction_set;
    jstring &app_data_dir;
};

struct ServerSpecializeArgs {
    jint &uid;
    jint &gid;
    jintArray &gids;
    jint &runtime_flags;
    jlong &permitted_capabilities;
    jlong &effective_capabilities;
};

class Api {
public:
    virtual void setOption(Option opt) = 0;
    virtual uint32_t getFlags() = 0;
    virtual void exemptFd(int fd) = 0;
    virtual void hookJniNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int numMethods) = 0;
    virtual void pltHookRegister(const char *regex, const char *symbol, void *newFunc, void **oldFunc) = 0;
    virtual bool pltHookCommit() = 0;
    virtual int connectCompanion() = 0;
};

class ModuleBase {
public:
    virtual void onLoad(Api *api, JNIEnv *env) {}
    virtual void preAppSpecialize(AppSpecializeArgs *args) {}
    virtual void postAppSpecialize(const AppSpecializeArgs *args) {}
    virtual void preServerSpecialize(ServerSpecializeArgs *args) {}
    virtual void postServerSpecialize(const ServerSpecializeArgs *args) {}
};

struct ModuleAbi {
    long api_version;
    ModuleBase *(*impl_factory)();
};

} // namespace zygisk

#define REGISTER_ZYGISK_MODULE(clazz) \
extern "C" [[gnu::visibility("default")]] \
void zygisk_module_entry(zygisk::ModuleAbi *abi) { \
    abi->api_version = 4; \
    abi->impl_factory = []() -> zygisk::ModuleBase* { \
        return new clazz(); \
    }; \
}
