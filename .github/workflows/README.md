# MLBB OnePlus 15 Zygisk Spoofer

A native C++ Zygisk module engineered specifically for **KernelSU Next + ReZygisk** (and APatch / Magisk).

### What it does
* **Strict Process Isolation**: In `preAppSpecialize`, the module checks if the launching process is `com.mobile.legends`.
* **Zero System-Wide Footprint**: For any other process (banking apps, Google Play Services, system UI), it immediately tells Zygisk to `dlclose()` and unload itself.
* **Property Spoofing**: Only inside `com.mobile.legends`, it updates Java `android.os.Build` fields (`BRAND`, `MANUFACTURER`, `MODEL`, `DEVICE`, `PRODUCT`) to report as **OnePlus CPH2747**.
* **Play Integrity Safe**: Because it never touches global system properties or attestation props, your device retains its genuine identity everywhere else.

---

### Project Structure
* `module.prop`: Module metadata for KernelSU / Magisk / APatch.
* `customize.sh`: Module installation banner.
* `jni/main.cpp`: Native C++ Zygisk implementation using JNI reflection.
* `jni/zygisk.hpp`: Official Zygisk API definitions.
* `jni/Android.mk` & `Application.mk`: NDK build definitions.
* `.github/workflows/build.yml`: Automated CI workflow to compile into flashable `.zip`.

---

### How to Build the Flashable Zip

#### Method 1: Using GitHub Actions (Easiest - 1 Click, No Tools Needed)
1. Push this folder to a GitHub repository (private or public).
2. Go to the **Actions** tab in your repository.
3. The workflow `Build Zygisk Module` will automatically run.
4. When finished, download `MLBB_OnePlus15_Zygisk_v1.0.zip` from the workflow Artifacts.

#### Method 2: Local Compilation (Android NDK)
If you have Android NDK installed on your machine:
```bash
cd jni
ndk-build -B
mkdir -p package/zygisk
cp ../libs/arm64-v8a/libmlbb_spoofer.so package/zygisk/arm64-v8a.so
cp ../module.prop package/
cp ../customize.sh package/
cd package
zip -r ../MLBB_OnePlus15_Zygisk_v1.0.zip ./*
```

---

### Installation on Phone
1. Install **KernelSU Next**.
2. Install the **ReZygisk** module from GitHub and reboot.
3. Flash the compiled `MLBB_OnePlus15_Zygisk_v1.0.zip` in KernelSU Next Manager.
4. Reboot your device.
5. Launch Mobile Legends: Bang Bang.
