#include "projectile_table_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>

namespace dh2::data {
namespace {
constexpr std::size_t kSerializedRowBytes = 59;
constexpr std::uint32_t kMaximumRows = 4096;
constexpr std::size_t kMaximumInputBytes = 4u * 1024u * 1024u;
constexpr const char* kSchema[] = {
    "Dedicated", "DisappearOnHit", "ExpireFX", "ExpireSFX",
    "FriendlyFire", "ImpactFX", "ImpactSFX", "IsLaser", "IsMagic",
    "MaxDistance", "MaxRotation", "Model", "ObjectImpactFX",
    "ObjectImpactSFX", "StayOnFloor", "TargetLock", "TimeBetweenHit",
    "Timer", "Velocity", "VelocityDamping"};

struct Reader {
    Bytes bytes;
    std::size_t offset{};

    explicit Reader(Bytes input) : bytes(input) {
        if (input.data == nullptr || input.size < 4 ||
            input.size > kMaximumInputBytes)
            throw std::runtime_error("Projectile table input outside size limit");
    }

    void require(std::size_t count) const {
        if (offset > bytes.size || count > bytes.size - offset)
            throw std::runtime_error("Truncated ProjectileTable input");
    }

    std::uint32_t word() {
        require(4);
        const auto* value = bytes.data + offset;
        offset += 4;
        return std::uint32_t(value[0]) |
               (std::uint32_t(value[1]) << 8) |
               (std::uint32_t(value[2]) << 16) |
               (std::uint32_t(value[3]) << 24);
    }

    std::int32_t integer() {
        const auto bits = word();
        std::int32_t value{};
        std::memcpy(&value, &bits, sizeof(value));
        return value;
    }

    float real() {
        const auto bits = word();
        float value{};
        std::memcpy(&value, &bits, sizeof(value));
        if (!std::isfinite(value))
            throw std::runtime_error("Non-finite ProjectileTable float");
        return value;
    }

    bool boolean() {
        require(1);
        const auto value = bytes.data[offset++];
        if (value > 1)
            throw std::runtime_error("Invalid ProjectileTable boolean");
        return value != 0;
    }

    std::vector<std::string> strings() {
        const auto count = word();
        if (count == 0 || count > kMaximumRows)
            throw std::runtime_error("Projectile string-table count outside limit");
        std::vector<std::string> result;
        result.reserve(count);
        for (std::uint32_t i = 0; i < count; ++i) {
            const auto length = word();
            if (length == 0 || length > 4096)
                throw std::runtime_error("Projectile string length outside limit");
            require(length);
            std::string value(reinterpret_cast<const char*>(bytes.data + offset),
                              length);
            offset += length;
            if (std::any_of(value.begin(), value.end(), [](unsigned char ch) {
                    return ch < 0x20 || ch > 0x7e;
                }))
                throw std::runtime_error("Non-ASCII Projectile string");
            result.push_back(std::move(value));
        }
        return result;
    }

    void finish() const {
        if (offset != bytes.size)
            throw std::runtime_error("Unexpected ProjectileTable input suffix");
    }
};

std::vector<std::string> read_names(Bytes bytes, bool unique) {
    Reader reader(bytes);
    auto result = reader.strings();
    reader.finish();
    if (unique) {
        auto sorted = result;
        std::sort(sorted.begin(), sorted.end());
        if (std::adjacent_find(sorted.begin(), sorted.end()) != sorted.end())
            throw std::runtime_error("Duplicate ProjectileTable name");
    }
    return result;
}
}  // namespace

bool load_projectile_table_v1(Bytes records, Bytes names, Bytes schema,
                              Bytes dictionary_names, Bytes dictionary_values,
                              ProjectileTableV1& output, std::string& error) {
    error.clear();
    try {
        const auto row_names = read_names(names, true);
        const auto fields = read_names(schema, true);
        if (fields.size() != sizeof(kSchema) / sizeof(kSchema[0]) ||
            !std::equal(fields.begin(), fields.end(), std::begin(kSchema)))
            throw std::runtime_error("ProjectileTable schema differs from source");

        Dictionary dictionary{};
        if (!load_dictionary(dictionary_names, dictionary_values, dictionary,
                             error))
            throw std::runtime_error(error.empty()
                ? "ProjectileDict rejected" : error);

        Reader reader(records);
        const auto count = reader.word();
        if (count == 0 || count > kMaximumRows || count != row_names.size() ||
            std::uint64_t(count) * kSerializedRowBytes !=
                records.size - reader.offset)
            throw std::runtime_error("ProjectileTable row dimensions differ");

        ProjectileTableV1 next{};
        next.model_dictionary = std::move(dictionary);
        next.rows.reserve(count);
        for (std::uint32_t index = 0; index < count; ++index) {
            ProjectileRowV1 row{};
            row.row_index = index;
            row.table_name = row_names[index];
            row.dedicated = reader.boolean();
            row.disappear_on_hit = reader.boolean();
            row.expire_fx = reader.integer();
            row.expire_sfx = reader.integer();
            row.friendly_fire = reader.boolean();
            row.impact_fx = reader.integer();
            row.impact_sfx = reader.integer();
            row.is_laser = reader.boolean();
            row.is_magic = reader.boolean();
            row.max_distance = reader.real();
            row.max_rotation = reader.real();
            row.model_index = reader.integer();
            row.object_impact_fx = reader.integer();
            row.object_impact_sfx = reader.integer();
            row.stay_on_floor = reader.boolean();
            row.target_lock = reader.boolean();
            row.time_between_hit = reader.integer();
            row.timer = reader.integer();
            row.velocity = reader.real();
            row.velocity_damping = reader.real();
            if (row.model_index < -1 ||
                (row.model_index >= 0 &&
                 static_cast<std::size_t>(row.model_index) >=
                     next.model_dictionary.names.size()))
                throw std::runtime_error("Projectile model index outside ProjectileDict");
            if (row.model_index >= 0) {
                const auto model = static_cast<std::size_t>(row.model_index);
                row.model_dictionary_name = next.model_dictionary.names[model];
                row.model_path = next.model_dictionary.values[model];
                if (row.model_path.rfind("data/3D/projectiles/", 0) != 0 ||
                    row.model_path.size() < 6 ||
                    row.model_path.substr(row.model_path.size() - 5) != ".bdae")
                    throw std::runtime_error("Projectile model path outside source asset family");
            }
            next.rows.push_back(std::move(row));
        }
        reader.finish();
        output = std::move(next);
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

const ProjectileRowV1* projectile_row_v1(const ProjectileTableV1& table,
                                         std::int32_t row) noexcept {
    if (row < 0 || static_cast<std::size_t>(row) >= table.rows.size())
        return nullptr;
    return &table.rows[static_cast<std::size_t>(row)];
}

const ProjectileRowV1* projectile_row_v1(const ProjectileTableV1& table,
                                         const std::string& name) noexcept {
    const auto found = std::find_if(table.rows.begin(), table.rows.end(),
        [&name](const ProjectileRowV1& row) { return row.table_name == name; });
    return found == table.rows.end() ? nullptr : &*found;
}

}  // namespace dh2::data
