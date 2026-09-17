#include <jni.h>
#include <string.h>
#include <android/log.h>
#include "zygisk.hpp"

#define LOG_TAG "MLBB_Zygisk_Spoofer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using zygisk::Api;
using zygisk::AppSpecializeArgs;

class MLBBModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        const char *process = env->GetStringUTFChars(args->nice_name, nullptr);
        
        // Target com.mobile.legends specifically
        if (process && (strncmp(process, "com.mobile.legends", 18) == 0)) {
            is_target = true;
            LOGI("Target detected: %s. Module will stay loaded.", process);
        } else {
            // For all other apps, banking apps, and system processes:
            // Instruct Zygisk to immediately dlclose and unload our module.
            api->setOption(zygisk::Option::DLCLOSE_MODULE_LIBRARY);
        }

        if (process) {
            env->ReleaseStringUTFChars(args->nice_name, process);
        }
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {
        if (!is_target) {
            return;
        }

        LOGI("Applying OnePlus CPH2747 spoof inside MLBB process...");

        // Hook Android Java Build properties in this process
        jclass build_class = env->FindClass("android/os/Build");
        if (build_class) {
            setField(build_class, "BRAND", "OnePlus");
            setField(build_class, "MANUFACTURER", "OnePlus");
            setField(build_class, "MODEL", "CPH2747");
            setField(build_class, "DEVICE", "CPH2747");
            setField(build_class, "PRODUCT", "CPH2747");
            env->DeleteLocalRef(build_class);
            LOGI("Build properties spoofed successfully.");
        } else {
            LOGE("Failed to find android/os/Build class.");
        }
    }

private:
    Api *api = nullptr;
    JNIEnv *env = nullptr;
    bool is_target = false;

    void setField(jclass clazz, const char *field_name, const char *value) {
        jfieldID fid = env->GetStaticFieldID(clazz, field_name, "Ljava/lang/String;");
        if (fid) {
            jstring jval = env->NewStringUTF(value);
            env->SetStaticObjectField(clazz, fid, jval);
            env->DeleteLocalRef(jval);
            LOGI("Overrode Build.%s = %s", field_name, value);
        } else {
            LOGE("Could not locate field Build.%s", field_name);
        }
    }
};

REGISTER_ZYGISK_MODULE(MLBBModule)
