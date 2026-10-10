#include "audio_source_cue_submission_v1.hpp"

#include <utility>

namespace dh::foundation::audio {

bool resolve_audio_source_cue_v1(
    const dh2::audio::AudioSourceBindingsV38& bindings,
    const dh2::audio::AudioCatalogV34& catalog,
    std::int32_t source_id,
    std::int64_t event_ns,
    AudioSourceCueSubmissionV1& output,
    std::string& error) {
    if (source_id < 0 || event_ns <= 0) {
        error = "Required actual source cue ordinal and caller event time";
        return false;
    }
    const auto* row = bindings.row(source_id);
    const auto* name = static_cast<std::size_t>(source_id) < bindings.names().size()
        ? &bindings.names()[static_cast<std::size_t>(source_id)] : nullptr;
    if (!row || !name) {
        error = "Required exact generated source cue row and name";
        return false;
    }
    if (row->event != 0) {
        error = "Source event cue needs its original selection/RNG branch";
        return false;
    }
    const auto* sound = catalog.sound(row->uid);
    if (!sound || !catalog.group(sound->group)) {
        error = "Required exact selected soundpack cue row/group";
        return false;
    }
    AudioSourceCueSubmissionV1 next;
    next.source_id = source_id;
    next.xml_sound_uid = sound->uid;
    next.event_monotonic_ns = event_ns;
    next.source_name = *name;
    next.exact_resource_uri = "data/sounds/" + sound->filename;
    output = std::move(next);
    error.clear();
    return true;
}

bool submit_audio_source_cue_v1(
    const AudioSourceCueSubmissionV1& submission,
    dh2::audio::AudioGameplayRuntimeV42& runtime,
    const dh2::audio::AudioGameplayRuntimeV42::PlainCommand& make_command,
    std::string& error) {
    if (submission.source_id < 0 || submission.xml_sound_uid < 0 ||
        submission.event_monotonic_ns <= 0 || submission.source_name.empty() ||
        submission.exact_resource_uri.empty()) {
        error = "Required complete resolved source cue submission record";
        return false;
    }
    return runtime.submit_plain_source(submission.source_id,
        submission.event_monotonic_ns, make_command, error);
}

} // namespace dh::foundation::audio
