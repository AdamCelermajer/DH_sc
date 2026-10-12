// vox::VoxSoundPackXML constructor, destructor and LoadXML. The loader reads the sound-pack
// file into the loader records (SoundXMLDef, GroupXMLDef, BankXMLDef, EventXMLDef), then
// fills the pack's record tables from them. The record tables keep the port's accessor
// layout, and their string pointers refer to the loader records (kept alive by loaded_).
//
// Deviations from the binary, all in the loader only:
//  - an element without a uid (or with one out of range) is skipped. The binary reuses the
//    uid of the previous element, which is a stale variable.
//  - a custom parameter count that is not positive gives no strings, where the binary
//    allocates and writes past the array.
//  - the sound label is not stored in the record (its +0x08 word is an int in the accessor
//    layout). It is used for the uid map only.
//  - the priority the binary writes at +0x04 is kept in the record's emitter_uid word, as
//    the record's +0x04 is already read as the emitter uid by the accessors (see NOTES.md).
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "vox-audio/deps/vox_file_system.hpp"
#include "vox-audio/vendor/tixml_dh.hpp"
#include "vox-audio/vox/sound_pack_xml.hpp"

namespace vox {

namespace {

using tixml::TiXmlDocument;
using tixml::TiXmlElement;
using tixml::TiXmlHandle;

bool QueryInt(const TiXmlElement* e, const char* name, int& out) {
    return e != nullptr && e->QueryIntAttribute(name, &out) == 0;
}

bool QueryDouble(const TiXmlElement* e, const char* name, double& out) {
    return e != nullptr && e->QueryDoubleAttribute(name, &out) == 0;
}

const char* Attr(const TiXmlElement* e, const char* name) {
    return e != nullptr ? e->Attribute(name) : nullptr;
}

// The "size" attribute of a section, 0 when absent or negative.
int SizeAttr(const TiXmlElement* e) {
    int n = 0;
    if (!QueryInt(e, "size", n) || n < 0) {
        return 0;
    }
    return n;
}

// Reads a uid attribute and returns true when it names a slot below `limit`.
bool ReadIndex(const TiXmlElement* e, std::size_t limit, std::size_t& index) {
    int uid = 0;
    if (!QueryInt(e, "uid", uid) || uid < 0 || static_cast<std::size_t>(uid) >= limit) {
        return false;
    }
    index = static_cast<std::size_t>(uid);
    return true;
}

// Splits a custom parameter string at ';' into `count` pieces: the last piece keeps the rest
// of the text, and missing pieces are empty. count 1 keeps the whole string.
std::vector<std::string> SplitCustom(const std::string& text, int count) {
    std::vector<std::string> out;
    std::size_t pos = 0;
    for (int i = 0; i < count; ++i) {
        if (pos == std::string::npos) {
            out.emplace_back();
            continue;
        }
        if (i == count - 1) {
            out.push_back(text.substr(pos));
            continue;
        }
        const std::size_t semi = text.find(';', pos);
        out.push_back(text.substr(pos, semi == std::string::npos ? std::string::npos : semi - pos));
        pos = semi == std::string::npos ? std::string::npos : semi + 1;
    }
    return out;
}

// Keeps a pointer array for a record in the loaded tables and returns its first element.
const char* const* CustomArray(XmlLoadedTables& tables, const std::vector<std::string>& strings) {
    if (strings.empty()) {
        return nullptr;
    }
    tables.custom_pointers.emplace_back();
    std::vector<const char*>& pointers = tables.custom_pointers.back();
    for (const std::string& s : strings) {
        pointers.push_back(s.c_str());
    }
    return pointers.data();
}

// Splits a string on any character of `delims`, skipping empty pieces (strtok semantics).
std::vector<std::string> Tokens(const char* text, const char* delims) {
    std::vector<std::string> out;
    std::string current;
    for (const char* p = text; *p != '\0'; ++p) {
        if (std::strchr(delims, *p) != nullptr) {
            if (!current.empty()) {
                out.push_back(current);
                current.clear();
            }
        } else {
            current.push_back(*p);
        }
    }
    if (!current.empty()) {
        out.push_back(current);
    }
    return out;
}

// Sound format attribute (0x88d344, the format branch).
void ReadSoundFormat(const char* f, SoundXMLDef& s) {
    if (std::strcmp(f, "pcm") == 0 || std::strcmp(f, "adpcm") == 0) {
        s.format_ = 1;
    } else if (std::strcmp(f, "mpc8") == 0 || std::strcmp(f, "mpc") == 0) {
        s.format_ = 3;
    } else if (std::strcmp(f, "ogg") == 0) {
        s.format_ = 2;
    } else if (std::strcmp(f, "vxn") == 0) {
        s.format_ = 4;
    }
}

// Loading flags attribute: none 0, load to ram 1, load and decode 2.
void ReadLoadingFlags(const char* f, SoundXMLDef& s) {
    if (std::strcmp(f, "none") == 0) {
        s.loading_flags_ = 0;
    } else if (std::strcmp(f, "load to ram") == 0) {
        s.loading_flags_ = 1;
    } else if (std::strcmp(f, "load and decode") == 0) {
        s.loading_flags_ = 2;
    }
}

// Extension appended to a label when the sound has no file name (1 wav, 2 ogg, 3 mpc, 4 vxn).
const char* FormatExtension(int format) {
    switch (format) {
        case 1: return ".wav";
        case 2: return ".ogg";
        case 3: return ".mpc";
        case 4: return ".vxn";
        default: return "";
    }
}

}  // namespace

// 0x88ec4c _ZN3vox15VoxSoundPackXMLC1EPKc and 0x88ecc4 _ZN3vox15VoxSoundPackXMLC2EPKc
VoxSoundPackXML::VoxSoundPackXML(const char* path) { LoadXML(path); }

// 0x88c2cc _ZN3vox15VoxSoundPackXMLD1Ev and 0x88c36c _ZN3vox15VoxSoundPackXMLD2Ev. The
// record vectors, the uid map, the group masks and the loaded tables are freed by their
// own destructors.
VoxSoundPackXML::~VoxSoundPackXML() = default;

// 0x88d344 _ZN3vox15VoxSoundPackXML7LoadXMLEPKc (partial, see the file comment)
int VoxSoundPackXML::LoadXML(const char* path) {
    sounds_.clear();
    groups_.clear();
    banks_.clear();
    events_.clear();
    sound_uids_.clear();
    loaded_.reset();

    FileSystemInterface* fs = FileSystemInterface::GetInstance();
    if (fs == nullptr) {
        return 0;
    }
    FileInterface* file = fs->OpenFile(path, kVoxFileRead);
    if (file == nullptr) {
        return 0;
    }
    file->Seek(0, 2);
    const int size = file->Tell();
    file->Seek(0, 0);
    std::vector<char> buffer(static_cast<std::size_t>(size > 0 ? size : 0) + 1, '\0');
    const bool read_ok =
        size >= 0 && file->Read(buffer.data(), static_cast<std::size_t>(size), 1) == 1;
    fs->CloseFile(file);
    if (!read_ok) {
        return 0;
    }

    TiXmlDocument doc;
    doc.Parse(buffer.data());
    if (doc.Error()) {
        return 1;  // the binary keeps the tables cleared and still reports the read as done
    }

    auto tables = std::make_shared<XmlLoadedTables>();

    // Sounds (records of 0x44 bytes in the binary). uid map: label -> uid, in document order.
    tables->sounds.resize(static_cast<std::size_t>(SizeAttr(
        TiXmlHandle(&doc).FirstChild("soundpack").FirstChild("sounds").ToElement())));
    for (const TiXmlElement* e = TiXmlHandle(&doc)
                                     .FirstChild("soundpack")
                                     .FirstChild("sounds")
                                     .FirstChild("sound")
                                     .ToElement();
         e != nullptr; e = e->NextSiblingElement()) {
        std::size_t index = 0;
        if (!ReadIndex(e, tables->sounds.size(), index)) {
            continue;
        }
        SoundXMLDef& s = tables->sounds[index];
        s.uid_ = static_cast<std::int32_t>(index);
        int v = 0;
        if (QueryInt(e, "bank", v)) s.bank_ = static_cast<std::int8_t>(v);
        if (QueryInt(e, "group", v)) s.group_ = static_cast<std::int8_t>(v);
        if (QueryInt(e, "priority", v)) s.priority_ = v;
        if (const char* loop = Attr(e, "loop")) s.loop_ = loop[0] == 'y' ? 1 : 0;
        s.label_ = Attr(e, "label") != nullptr ? Attr(e, "label") : "";
        if (const char* format = Attr(e, "format")) ReadSoundFormat(format, s);
        if (const char* flags = Attr(e, "loadingflags")) ReadLoadingFlags(flags, s);
        if (const char* file_name = Attr(e, "filename")) {
            s.data_name_ = file_name;
        } else if (s.format_ != -1) {
            s.data_name_ = s.label_ + FormatExtension(s.format_);
        }
        static const char* const kValueNames[9] = {
            "refdistance", "maxdistance", "rolloff", "basegain", "mingainmod",
            "maxgainmod", "basepitch", "minpitchmod", "maxpitchmod"};
        for (int i = 0; i < 9; ++i) {
            double d = 0.0;
            if (QueryDouble(e, kValueNames[i], d)) s.values_[i] = static_cast<float>(d);
        }
        if (Attr(e, "customparam") != nullptr) {
            int qty = 0;
            if (QueryInt(e, "customparamqty", qty)) s.custom_count_ = qty;
            if (s.custom_count_ > 0) {
                s.custom_ = SplitCustom(Attr(e, "customparam"), s.custom_count_);
            }
        }
        sound_uids_[s.label_.c_str()] = static_cast<std::int32_t>(index);
    }

    // Groups: size + 1 records, the attributes name the bus (+0x04) and the name (+0x1c).
    tables->groups.resize(static_cast<std::size_t>(SizeAttr(
                              TiXmlHandle(&doc).FirstChild("soundpack").FirstChild("groups").ToElement())) +
                          1);
    for (const TiXmlElement* e = TiXmlHandle(&doc)
                                     .FirstChild("soundpack")
                                     .FirstChild("groups")
                                     .FirstChild("group")
                                     .ToElement();
         e != nullptr; e = e->NextSiblingElement()) {
        std::size_t index = 0;
        if (!ReadIndex(e, tables->groups.size(), index)) {
            continue;
        }
        GroupXMLDef& g = tables->groups[index];
        g.uid_ = static_cast<std::int32_t>(index);
        if (const char* pos = Attr(e, "pos3d")) {
            g.type_ = pos[0] == 'y' ? 1 : (pos[0] == 'r' ? 2 : 0);
        }
        g.name_ = Attr(e, "bus") != nullptr ? Attr(e, "bus") : "";
        g.label_ = Attr(e, "name") != nullptr ? Attr(e, "name") : "";
    }

    // Group masks: label -> mask, added to the existing map (the binary does not clear it).
    for (const TiXmlElement* e = TiXmlHandle(&doc)
                                     .FirstChild("soundpack")
                                     .FirstChild("groupmasks")
                                     .FirstChild("groupmask")
                                     .ToElement();
         e != nullptr; e = e->NextSiblingElement()) {
        int mask = 0;
        const char* label = Attr(e, "label");
        if (QueryInt(e, "mask", mask) && label != nullptr) {
            group_masks_[std::string(label)] = mask;
        }
    }

    // Banks: size + 1 records.
    tables->banks.resize(static_cast<std::size_t>(SizeAttr(
                             TiXmlHandle(&doc).FirstChild("soundpack").FirstChild("banks").ToElement())) +
                         1);
    for (const TiXmlElement* e = TiXmlHandle(&doc)
                                     .FirstChild("soundpack")
                                     .FirstChild("banks")
                                     .FirstChild("bank")
                                     .ToElement();
         e != nullptr; e = e->NextSiblingElement()) {
        std::size_t index = 0;
        if (!ReadIndex(e, tables->banks.size(), index)) {
            continue;
        }
        BankXMLDef& b = tables->banks[index];
        b.uid_ = static_cast<std::int32_t>(index);
        int v = 0;
        if (QueryInt(e, "maxplaybacks", v)) b.max_playbacks_ = v;
        if (QueryInt(e, "threshold", v)) b.threshold_ = v;
        b.name_ = Attr(e, "name") != nullptr ? Attr(e, "name") : "";
        if (const char* behaviour = Attr(e, "behaviour")) {
            if (std::strcmp(behaviour, "steal oldest") == 0) {
                b.behavior_ = 0;
            } else if (std::strcmp(behaviour, "steal lowest priority") == 0) {
                b.behavior_ = 1;
            } else if (std::strcmp(behaviour, "steal low. prio. or old. same prio") == 0) {
                b.behavior_ = 2;
            }
        }
    }

    // Events: size records (no +1), uid, label, type, the sound uid list, the two parameters
    // and the custom parameters.
    tables->events.resize(static_cast<std::size_t>(SizeAttr(
        TiXmlHandle(&doc).FirstChild("soundpack").FirstChild("events").ToElement())));
    for (const TiXmlElement* e = TiXmlHandle(&doc)
                                     .FirstChild("soundpack")
                                     .FirstChild("events")
                                     .FirstChild("event")
                                     .ToElement();
         e != nullptr; e = e->NextSiblingElement()) {
        std::size_t index = 0;
        if (!ReadIndex(e, tables->events.size(), index)) {
            continue;
        }
        EventXMLDef& ev = tables->events[index];
        ev.uid_ = static_cast<std::int32_t>(index);
        ev.name_ = Attr(e, "label") != nullptr ? Attr(e, "label") : "";
        if (const char* type = Attr(e, "type")) {
            if (std::strcmp(type, "random") == 0) {
                ev.mode_ = 0;
            } else if (std::strcmp(type, "playlist") == 0) {
                ev.mode_ = 1;
            }
        }
        if (const char* value = Attr(e, "value")) {
            for (const std::string& token : Tokens(value, " ,")) {
                ev.sound_uids_.push_back(static_cast<std::int32_t>(std::atoi(token.c_str())));
            }
        }
        if (const char* params = Attr(e, "params")) {
            const std::vector<std::string> pieces = Tokens(params, " ;");
            if (!pieces.empty()) {
                const int first = std::atoi(pieces[0].c_str());
                const int count = static_cast<int>(ev.sound_uids_.size());
                ev.max_used_ = static_cast<std::int16_t>(first < count ? first : count);
                if (pieces.size() > 1) {
                    int second = std::atoi(pieces[1].c_str());
                    if (second >= 100) second = 100;
                    ev.no_sound_percent_ = static_cast<std::int16_t>(second);
                }
            }
        }
        if (Attr(e, "customparam") != nullptr) {
            int qty = 0;
            if (QueryInt(e, "customparamqty", qty)) ev.custom_count_ = qty;
            if (ev.custom_count_ > 0) {
                ev.custom_ = SplitCustom(Attr(e, "customparam"), ev.custom_count_);
            }
        }
        ev.sequence_count_ = static_cast<std::int16_t>(ev.sound_uids_.size());
    }

    // Record tables: the accessor layout, with string pointers into the loaded tables.
    sounds_.resize(tables->sounds.size());
    for (std::size_t i = 0; i < tables->sounds.size(); ++i) {
        const SoundXMLDef& s = tables->sounds[i];
        SoundRecordXML& r = sounds_[i];
        r.uid = s.uid_;
        r.emitter_uid = s.priority_;
        r.data_name = s.data_name_.c_str();
        r.field_10 = s.loading_flags_;
        r.format = s.format_;
        r.field_15 = s.bank_;
        r.field_16 = s.group_;
        r.emitter_flag = s.loop_;
        for (int k = 0; k < 9; ++k) r.emitter_values_[k] = s.values_[k];
        r.custom_count = s.custom_count_ > 0 ? s.custom_count_ : 0;
        r.custom_values = CustomArray(*tables, s.custom_);
    }
    groups_.resize(tables->groups.size());
    for (std::size_t i = 0; i < tables->groups.size(); ++i) {
        const GroupXMLDef& g = tables->groups[i];
        GroupRecordXML& r = groups_[i];
        r.uid = g.uid_;
        r.name_ptr = g.label_.c_str();
        r.name = g.label_;
        r.type = g.type_;
    }
    banks_.resize(tables->banks.size());
    for (std::size_t i = 0; i < tables->banks.size(); ++i) {
        const BankXMLDef& b = tables->banks[i];
        BankRecordXML& r = banks_[i];
        r.uid = b.uid_;
        r.behavior = b.behavior_;
        r.field_08 = b.max_playbacks_;
        r.field_0c = b.threshold_;
        r.name = b.name_;
    }
    events_.resize(tables->events.size());
    for (std::size_t i = 0; i < tables->events.size(); ++i) {
        const EventXMLDef& ev = tables->events[i];
        EventRecordXML& r = events_[i];
        r.uid = ev.uid_;
        r.name = ev.name_.c_str();
        r.used_sound_uids = ev.used_;
        r.sound_uids = ev.sound_uids_;
        r.field_1c = ev.mode_;
        r.field_1e = ev.max_used_;
        r.field_20 = ev.no_sound_percent_;
        r.sequence_index = ev.sequence_count_;
        r.custom_count = ev.custom_count_ > 0 ? ev.custom_count_ : 0;
        r.custom_values = CustomArray(*tables, ev.custom_);
    }

    loaded_ = tables;
    return 1;
}

}  // namespace vox
