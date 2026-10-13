// Android output for the gameplay SFX engine: OpenSL ES PCM16 stereo buffer
// queue (available from the app's minSdk 24; AAudio starts at API 26) fed by the
// platform-free mixer, plus the JNI entry points FrontAudio uses. The shared
// queue consumer (gameplay_sfx_engine_v1.hpp) owns every playback decision.
#include "gameplay_sfx_engine_v1.hpp"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <jni.h>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace {

constexpr const char* kTag = "DH2Sfx";
constexpr unsigned kBuffers = 3;
constexpr std::size_t kMaxAssetBytes = 16u * 1024u * 1024u;

using dh2::gameplay_sfx::Engine;
using dh2::gameplay_sfx::Outcome;

#define SFX_LOGI(...) __android_log_print(ANDROID_LOG_INFO, kTag, __VA_ARGS__)
#define SFX_LOGW(...) __android_log_print(ANDROID_LOG_WARN, kTag, __VA_ARGS__)
#define SFX_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, kTag, __VA_ARGS__)

class Output {
public:
    ~Output() { close(); }

    bool open(Engine& engine, std::uint32_t rate, std::uint32_t frames) {
        close();
        engine_ = &engine;
        frames_ = frames;
        next_ = 0;
        buffers_.assign(std::size_t(kBuffers) * frames * 2, 0);
        SLresult result = slCreateEngine(&engine_obj_, 0, nullptr, 0, nullptr, nullptr);
        if (!ok(result, "slCreateEngine")) return abort_open();
        if (!ok((*engine_obj_)->Realize(engine_obj_, SL_BOOLEAN_FALSE), "engine Realize"))
            return abort_open();
        SLEngineItf engine_itf = nullptr;
        if (!ok((*engine_obj_)->GetInterface(engine_obj_, SL_IID_ENGINE, &engine_itf),
                "engine interface"))
            return abort_open();
        if (!ok((*engine_itf)->CreateOutputMix(engine_itf, &mix_obj_, 0, nullptr, nullptr),
                "CreateOutputMix") ||
            !ok((*mix_obj_)->Realize(mix_obj_, SL_BOOLEAN_FALSE), "output mix Realize"))
            return abort_open();

        SLDataLocator_AndroidSimpleBufferQueue queue_locator{
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, kBuffers};
        SLDataFormat_PCM format{};
        format.formatType = SL_DATAFORMAT_PCM;
        format.numChannels = 2;
        format.samplesPerSec = rate * 1000u; // milliHertz
        format.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
        format.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
        format.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
        format.endianness = SL_BYTEORDER_LITTLEENDIAN;
        SLDataSource source{&queue_locator, &format};
        SLDataLocator_OutputMix mix_locator{SL_DATALOCATOR_OUTPUTMIX, mix_obj_};
        SLDataSink sink{&mix_locator, nullptr};
        const SLInterfaceID ids[2] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION};
        const SLboolean required[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_FALSE};
        if (!ok((*engine_itf)->CreateAudioPlayer(engine_itf, &player_obj_, &source, &sink, 2, ids,
                                                 required),
                "CreateAudioPlayer"))
            return abort_open();
        SLAndroidConfigurationItf configuration = nullptr;
        if ((*player_obj_)->GetInterface(player_obj_, SL_IID_ANDROIDCONFIGURATION,
                                         &configuration) == SL_RESULT_SUCCESS &&
            configuration) {
            // Same routing as the Java music/effect tracks (USAGE_GAME -> music stream).
            SLint32 stream = SL_ANDROID_STREAM_MEDIA;
            (*configuration)->SetConfiguration(configuration, SL_ANDROID_KEY_STREAM_TYPE, &stream,
                                               sizeof stream);
#ifdef SL_ANDROID_KEY_PERFORMANCE_MODE
            SLuint32 mode = SL_ANDROID_PERFORMANCE_LATENCY;
            (*configuration)->SetConfiguration(configuration, SL_ANDROID_KEY_PERFORMANCE_MODE,
                                               &mode, sizeof mode);
#endif
        }
        if (!ok((*player_obj_)->Realize(player_obj_, SL_BOOLEAN_FALSE), "player Realize") ||
            !ok((*player_obj_)->GetInterface(player_obj_, SL_IID_PLAY, &play_), "play interface") ||
            !ok((*player_obj_)->GetInterface(player_obj_, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &queue_),
                "buffer queue interface") ||
            !ok((*queue_)->RegisterCallback(queue_, &Output::callback, this), "RegisterCallback"))
            return abort_open();
        prime();
        if (!ok((*play_)->SetPlayState(play_, SL_PLAYSTATE_PLAYING), "SetPlayState(PLAYING)"))
            return abort_open();
        playing_ = true;
        SFX_LOGI("OpenSL ES output open | rate=%u | buffer_frames=%u x %u", rate, frames, kBuffers);
        return true;
    }

    // Pausing stops the callbacks (no mixer time passes); resuming re-primes
    // the queue so stale audio from before the pause is never replayed.
    void set_active(bool active) {
        if (!play_ || active == playing_) return;
        if (active) {
            (*queue_)->Clear(queue_);
            next_ = 0;
            prime();
            if (ok((*play_)->SetPlayState(play_, SL_PLAYSTATE_PLAYING), "resume")) playing_ = true;
        } else if (ok((*play_)->SetPlayState(play_, SL_PLAYSTATE_PAUSED), "pause")) {
            playing_ = false;
        }
    }

    void close() {
        if (play_) (*play_)->SetPlayState(play_, SL_PLAYSTATE_STOPPED);
        if (player_obj_) (*player_obj_)->Destroy(player_obj_); // joins the callback thread
        if (mix_obj_) (*mix_obj_)->Destroy(mix_obj_);
        if (engine_obj_) (*engine_obj_)->Destroy(engine_obj_);
        player_obj_ = mix_obj_ = engine_obj_ = nullptr;
        play_ = nullptr;
        queue_ = nullptr;
        engine_ = nullptr;
        playing_ = false;
    }

    bool is_open() const noexcept { return play_ != nullptr; }
    bool is_playing() const noexcept { return playing_; }

private:
    static bool ok(SLresult result, const char* what) {
        if (result == SL_RESULT_SUCCESS) return true;
        SFX_LOGE("OpenSL ES %s failed: 0x%x", what, static_cast<unsigned>(result));
        return false;
    }
    bool abort_open() {
        close();
        return false;
    }
    void prime() {
        for (unsigned i = 0; i < kBuffers; ++i) enqueue_next();
    }
    void enqueue_next() {
        std::int16_t* buffer = &buffers_[std::size_t(next_) * frames_ * 2];
        engine_->render(buffer, frames_);
        (*queue_)->Enqueue(queue_, buffer, frames_ * 2u * sizeof(std::int16_t));
        next_ = (next_ + 1) % kBuffers;
    }
    static void callback(SLAndroidSimpleBufferQueueItf, void* context) {
        static_cast<Output*>(context)->enqueue_next();
    }

    SLObjectItf engine_obj_ = nullptr, mix_obj_ = nullptr, player_obj_ = nullptr;
    SLPlayItf play_ = nullptr;
    SLAndroidSimpleBufferQueueItf queue_ = nullptr;
    Engine* engine_ = nullptr;
    std::vector<std::int16_t> buffers_;
    std::uint32_t frames_ = 0;
    unsigned next_ = 0;
    bool playing_ = false;
};

struct Backend {
    std::mutex mutex;
    std::unique_ptr<Engine> engine;
    Output output;
    jobject assets_ref = nullptr;
    AAssetManager* assets = nullptr;
    bool active = true;
};
Backend& backend() {
    // Intentionally never destroyed: process teardown must not call into OpenSL ES.
    static Backend* value = new Backend();
    return *value;
}

bool read_asset(AAssetManager* manager, const std::string& path,
                std::vector<std::uint8_t>& bytes, std::string& error) {
    if (!manager) {
        error = "no asset manager";
        return false;
    }
    std::unique_ptr<AAsset, decltype(&AAsset_close)> asset(
        AAssetManager_open(manager, path.c_str(), AASSET_MODE_BUFFER), &AAsset_close);
    if (!asset) {
        error = "asset not found: " + path;
        return false;
    }
    const auto length = AAsset_getLength64(asset.get());
    if (length <= 0 || static_cast<std::uint64_t>(length) > kMaxAssetBytes) {
        error = "asset has an invalid size: " + path;
        return false;
    }
    bytes.resize(static_cast<std::size_t>(length));
    std::size_t offset = 0;
    while (offset < bytes.size()) {
        const int count = AAsset_read(asset.get(), bytes.data() + offset, bytes.size() - offset);
        if (count <= 0) {
            error = "asset read was truncated: " + path;
            return false;
        }
        offset += static_cast<std::size_t>(count);
    }
    return true;
}

std::string copy_jstring(JNIEnv* env, jstring text) {
    if (!text) return {};
    const char* raw = env->GetStringUTFChars(text, nullptr);
    if (!raw) return {};
    std::string value(raw);
    env->ReleaseStringUTFChars(text, raw);
    return value;
}

void release_assets(JNIEnv* env, Backend& b) {
    if (b.assets_ref) env->DeleteGlobalRef(b.assets_ref);
    b.assets_ref = nullptr;
    b.assets = nullptr;
}

} // namespace

extern "C" {

// Opens (or re-binds) the effect engine. Returns false when the platform has no
// usable output; the caller then keeps its legacy Java effect path.
JNIEXPORT jboolean JNICALL Java_com_example_dh2_NativeBridge_sfxOpen(
    JNIEnv* env, jclass, jint sample_rate, jint buffer_frames, jobject assets) {
    Backend& b = backend();
    std::lock_guard<std::mutex> lock(b.mutex);
    if (!assets) return JNI_FALSE;
    release_assets(env, b);
    b.assets_ref = env->NewGlobalRef(assets);
    b.assets = b.assets_ref ? AAssetManager_fromJava(env, b.assets_ref) : nullptr;
    if (!b.assets) {
        release_assets(env, b);
        return JNI_FALSE;
    }
    if (b.engine && b.output.is_open()) return JNI_TRUE;
    const auto rate = static_cast<std::uint32_t>(std::clamp<jint>(sample_rate, 8000, 192000));
    const auto frames = static_cast<std::uint32_t>(std::clamp<jint>(buffer_frames, 128, 4096));
    b.output.close();
    b.engine = std::make_unique<Engine>(
        rate, [](const std::string& path, std::vector<std::uint8_t>& bytes, std::string& error) {
            return read_asset(backend().assets, path, bytes, error);
        });
    if (!b.output.open(*b.engine, rate, frames)) {
        b.engine.reset();
        return JNI_FALSE;
    }
    if (!b.active) b.output.set_active(false);
    return JNI_TRUE;
}

// One `dh2fx,...` wire command from the shared gameplay queue. Returns the
// Outcome ordinal, or -1 when the native engine is not open.
JNIEXPORT jint JNICALL Java_com_example_dh2_NativeBridge_sfxCommand(JNIEnv* env, jclass,
                                                                    jstring command) {
    Backend& b = backend();
    std::lock_guard<std::mutex> lock(b.mutex);
    if (!b.engine || !b.output.is_open()) return -1;
    const std::string text = copy_jstring(env, command);
    const auto result = b.engine->command(text);
    switch (result.outcome) {
    case Outcome::started:
        SFX_LOGI("Effect started | uid=%u | voices=%zu | cached=%zu KiB", result.uid,
                 b.engine->mixer().active_voices(), b.engine->cached_bytes() / 1024);
        break;
    case Outcome::refused_by_bank:
        SFX_LOGI("Effect refused by priority bank | uid=%u", result.uid);
        break;
    case Outcome::stopped:
        SFX_LOGI("Effect stop | %s", text.c_str());
        break;
    case Outcome::voice_limit:
        SFX_LOGW("Effect dropped, voice limit | uid=%u", result.uid);
        break;
    case Outcome::load_failed:
        SFX_LOGE("Effect failed | uid=%u | %s", result.uid, result.detail.c_str());
        break;
    case Outcome::invalid:
        SFX_LOGE("Effect command rejected | %s | %s", result.detail.c_str(), text.c_str());
        break;
    case Outcome::control:
    case Outcome::ignored:
        break;
    }
    return static_cast<jint>(result.outcome);
}

// Authored audio controls: pause-all, resume-all, resume, volume,<music>,<fx>.
JNIEXPORT jint JNICALL Java_com_example_dh2_NativeBridge_sfxControl(JNIEnv* env, jclass,
                                                                    jstring control) {
    Backend& b = backend();
    std::lock_guard<std::mutex> lock(b.mutex);
    if (!b.engine || !b.output.is_open()) return -1;
    const std::string text = copy_jstring(env, control);
    const auto result = b.engine->control(text);
    if (result.outcome == Outcome::invalid)
        SFX_LOGE("Audio control rejected | %s | %s", result.detail.c_str(), text.c_str());
    else if (result.outcome == Outcome::control)
        SFX_LOGI("Audio control applied | %s", text.c_str());
    return static_cast<jint>(result.outcome);
}

// Activity pause / audio-focus loss silences and freezes the mixer.
JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_sfxSetActive(JNIEnv*, jclass,
                                                                      jboolean active) {
    Backend& b = backend();
    std::lock_guard<std::mutex> lock(b.mutex);
    b.active = active == JNI_TRUE;
    if (b.engine) b.output.set_active(b.active);
}

JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_sfxClose(JNIEnv* env, jclass) {
    Backend& b = backend();
    std::lock_guard<std::mutex> lock(b.mutex);
    b.output.close();
    b.engine.reset();
    release_assets(env, b);
    b.active = true;
}

} // extern "C"
