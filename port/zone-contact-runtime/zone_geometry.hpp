#ifndef DH2_ZONE_CONTACT_RUNTIME_GEOMETRY_HPP
#define DH2_ZONE_CONTACT_RUNTIME_GEOMETRY_HPP

namespace dh2_zone_contact {

struct Vec3 {
    float x;
    float y;
    float z;
};

/* Initialized by the original native library's Point3D translation-unit
 * constructor. Keep this constant fixed to the recovered runtime value. */
extern const Vec3 kNativeGlobalDirection;

struct Aabb {
    Vec3 min;
    Vec3 max;
};

struct Segment {
    Vec3 start;
    Vec3 end;
};

enum ContactPath {
    kNoPhysicalObject,
    kAabbFallback,
    kSelectorLine
};

/* `radius` is the value returned by PhysicalObject::getRadius(). The native
 * Zone fallback computes a radius-scaled temporary vector, but the result is
 * dead before the return predicate; this field is retained so host tests can
 * make that non-dependence explicit. */
struct PhysicalObjectView {
    bool present;
    float radius;
};

struct ContactResult {
    bool inside;
    ContactPath path;
    bool selector_query_performed;
    Segment segment;
    Vec3 intersection_point;
};

/* The caller supplies the scene/collision-manager query for the recovered
 * selector path. `false` means the line did not hit the selector. */
typedef bool (*SelectorLineQuery)(void *context,
                                  const Segment *segment,
                                  Vec3 *intersection_point);

/* Reproduces the recovered Zone::InitPost local-box arithmetic only:
 * scaled_dimensions = dimensions * object_scale, then min/max = +/- half.
 * It does not apply the unresolved GameObject virtual bounds update. */
bool make_zone_local_aabb(const Vec3 *dimensions,
                          const Vec3 *object_scale,
                          Aabb *local_bounds);

/* Project Zone::IsInside(GameObject*) after the object has a physical object.
 * `zone_world_bounds` must be the already-updated six bounds at Zone+0x12c..140.
 * If `selector_node_present` is false, the native predicate is inclusive
 * position-in-AABB; actor bounds and PhysicalObject::getRadius() do not affect
 * it. If true, the selector query receives
 * (actor_position-zone_position)+/-100*kNativeGlobalDirection. A missing
 * triangle selector or callback produces a false selector-path result.
 */
ContactResult project_is_inside(
    const PhysicalObjectView *physical_object,
    bool selector_node_present,
    bool triangle_selector_present,
    const Aabb *zone_world_bounds,
    const Vec3 *zone_position,
    const Vec3 *actor_position,
    SelectorLineQuery selector_query,
    void *selector_query_context);

}  // namespace dh2_zone_contact

#endif
