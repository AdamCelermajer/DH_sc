#include "../pydata-scripts/native/pydata_scripts.h"

#include <jni.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace {
struct InputBytes {
    std::uint8_t* data;
    std::uint32_t size;
};

bool copy_java(JNIEnv* env, jbyteArray input, InputBytes* output) {
    if (!input || !output) return false;
    const auto length = env->GetArrayLength(input);
    if (length <= 0 || static_cast<std::uint32_t>(length) > DH2_SCRIPT_MAX_TABLE_BYTES)
        return false;
    auto* bytes = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(length)));
    if (!bytes) return false;
    env->GetByteArrayRegion(input, 0, length, reinterpret_cast<jbyte*>(bytes));
    if (env->ExceptionCheck()) {
        std::free(bytes);
        return false;
    }
    output->data = bytes;
    output->size = static_cast<std::uint32_t>(length);
    return true;
}

jstring text(JNIEnv* env, const char* message) {
    return env->NewStringUTF(message);
}
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_validateScriptTables(
        JNIEnv* env, jclass, jbyteArray common_names_array,
        jbyteArray common_programs_array, jbyteArray swamp_names_array,
        jbyteArray swamp_programs_array) {
    InputBytes common_names{}, common_programs{}, swamp_names{}, swamp_programs{};
    if (!copy_java(env, common_names_array, &common_names) ||
        !copy_java(env, common_programs_array, &common_programs) ||
        !copy_java(env, swamp_names_array, &swamp_names) ||
        !copy_java(env, swamp_programs_array, &swamp_programs)) {
        std::free(common_names.data);
        std::free(common_programs.data);
        std::free(swamp_names.data);
        std::free(swamp_programs.data);
        return text(env, "PyData rejected: source tables could not be read within limits.");
    }

    dh2_script_table common{}, swamp{};
    dh2_script_error error{};
    const auto common_result = dh2_script_table_decode(
        common_names.data, common_names.size,
        common_programs.data, common_programs.size, &common, &error);
    const auto common_error = error;
    auto swamp_result = DH2_SCRIPT_OK;
    if (common_result == DH2_SCRIPT_OK) {
        swamp_result = dh2_script_table_decode(
            swamp_names.data, swamp_names.size,
            swamp_programs.data, swamp_programs.size, &swamp, &error);
    }
    const auto swamp_error = error;
    std::free(common_names.data);
    std::free(common_programs.data);
    std::free(swamp_names.data);
    std::free(swamp_programs.data);

    if (common_result != DH2_SCRIPT_OK || swamp_result != DH2_SCRIPT_OK) {
        const auto& failure = common_result != DH2_SCRIPT_OK ? common_error : swamp_error;
        const auto code = common_result != DH2_SCRIPT_OK ? common_result : swamp_result;
        char message[192]{};
        std::snprintf(message, sizeof(message), "PyData rejected: %s at %u (error %u).",
                      dh2_script_error_name(code), failure.offset,
                      static_cast<unsigned>(code));
        dh2_script_table_destroy(&common);
        dh2_script_table_destroy(&swamp);
        return text(env, message);
    }

    constexpr std::uint8_t intro_name[] = "LizardMan_Intro";
    const auto intro_local = dh2_script_name_lookup(
        &swamp, intro_name, sizeof(intro_name) - 1, 0);
    const auto intro_id = dh2_script_resolve_id(
        &common, &swamp, intro_name, sizeof(intro_name) - 1, 1);
    const auto intro_commands = intro_local >= 0
        ? swamp.scripts[static_cast<std::uint32_t>(intro_local)].command_count : 0U;

    char message[224]{};
    std::snprintf(message, sizeof(message),
        "Scripts parsed · common %u/%u, SWAMP %u/%u; Intro ID %d, %u commands. Run its trace manually below; collision is not wired.",
        common.script_count, common.total_command_count,
        swamp.script_count, swamp.total_command_count,
        intro_id, intro_commands);
    dh2_script_table_destroy(&common);
    dh2_script_table_destroy(&swamp);
    return text(env, message);
}
