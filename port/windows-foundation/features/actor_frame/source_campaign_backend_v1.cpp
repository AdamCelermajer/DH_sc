#include "source_campaign_backend_v1.hpp"
#include "../../../android-native/app/src/main/cpp/renderer_character_campaign_v62.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_ai_queue_v105.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_death_rewards_v84.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_fx_v77.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_script_ai_v118.hpp"
#include "../../../level-world/canonical_gameobject_graph_v68.hpp"
#include "../../../level-world/character_controller_commands.hpp"
#include "../../../level-world/character_path_commands.hpp"
namespace dh::foundation::actor_frame {
namespace {
thread_local std::weak_ptr<SourceCampaignBackendContextV1> current;
template <class A, class B>
bool same(const std::shared_ptr<A> &a, const std::shared_ptr<B> &b) {
  return a && b && a.get() == b.get() && !a.owner_before(b) &&
         !b.owner_before(a);
}
} // namespace
bool SourceCampaignBackendContextV1::validate(std::string &e) const {
  const auto &g = graph_;
  const auto &c = g.candidate;
  if (!g.world || !g.characters || !g.current_level || !c.actual_world ||
      (g.mode != SourceCampaignBackendModeV1::menu &&
       g.mode != SourceCampaignBackendModeV1::campaign) ||
      !c.application || !c.objects || !c.roots || !c.floors ||
      !same(g.world->owner, c.actual_world) ||
      !same(g.world->application, c.application) || !g.world->canonical_world ||
      !same(g.world->canonical_world->manager_lease, c.objects) ||
      !same(g.current_level->graph().world, c.actual_world) ||
      !same(g.current_level->graph().application, c.application) ||
      !same(g.current_level->graph().objects, c.objects) ||
      !same(g.current_level->graph().floors, c.floors) ||
      !same(g.world->canonical_world->scene_roots_v20, c.roots) ||
      c.properties != &g.world->canonical_world->properties ||
      !g.current_level->graph().gs_globals ||
      !g.current_level->graph().gs_runtime ||
      !g.current_level->graph().gs_runtime->owns_globals_v50(
          g.current_level->graph().gs_globals) ||
      (c.navigation_registry &&
       !same(g.current_level->graph().navigation, c.navigation_registry))) {
    e = "Required SAME backend World/Application/manager/roots/floors owners";
    return false;
  }
  const auto &gs = g.current_level->graph();
  if (g.mode == SourceCampaignBackendModeV1::menu) {
    // Menu mode is admitted only for the same, pristine GS global/runtime.
    // Never infer emptiness from a failed current() or a failed construction.
    if (g.current_level->construction_attempted() || gs.gs_runtime->connection() ||
        gs.gs_runtime->fields().level34 || gs.gs_globals->s_level) {
      e = "Menu backend requires the actual fresh empty GS s_level slot";
      return false;
    }
    e.clear();
    return true;
  }
  if (!c.level || !g.services.level || !same(g.services.level, c.level)) {
    e = "Campaign backend requires the exact nonnull candidate/current Level";
    return false;
  }
  SourceCurrentLevelBorrowV1 level;
  if (!g.current_level->current(level, e)) return false;
  if (!level || !same(level.level(), c.level) ||
      !g.current_level->still_current(level, e)) {
    if (e.empty()) e = "Required exact live campaign GS s_level lease";
    return false;
  }
  e.clear();
  return true;
}
bool SourceCampaignBackendContextV1::register_current(
    const std::shared_ptr<SourceCampaignBackendContextV1> &c, std::string &e) {
  if (!c || !c->validate(e))
    return false;
  if (auto old = current.lock(); old && old != c) {
    e = "Backend current context already registered";
    return false;
  }
  current = c;
  return true;
}
void SourceCampaignBackendContextV1::unregister_current(
    const std::shared_ptr<SourceCampaignBackendContextV1> &c) noexcept {
  if (current.lock() == c)
    current.reset();
}
std::shared_ptr<SourceCampaignBackendContextV1>
SourceCampaignBackendContextV1::borrow_current(std::string &e) {
  auto c = current.lock();
  if (!c) {
    e = "Required registered SourceCampaignBackendContextV1";
    return {};
  }
  return c->validate(e) ? c : nullptr;
}
} // namespace dh::foundation::actor_frame
namespace model_renderer {
namespace {
using Context = dh::foundation::actor_frame::SourceCampaignBackendContextV1;
template <class A, class B>
bool same(const std::shared_ptr<A> &a, const std::shared_ptr<B> &b) {
  return a && b && a.get() == b.get() && !a.owner_before(b) &&
         !b.owner_before(a);
}
bool need(const char *leaf, std::string &e) {
  e = std::string("Required typed backend service: ") + leaf;
  return false;
}
std::shared_ptr<Context> context(const std::shared_ptr<void> &w,
                                 std::string &e) {
  auto c = Context::borrow_current(e);
  if (!c)
    return {};
  const auto &actual = c->graph().candidate.actual_world;
  if (actual.get() != w.get() || actual.owner_before(w) ||
      w.owner_before(actual)) {
    need("same World epoch", e);
    return {};
  }
  return c;
}
const dh2::world::CanonicalObjectBorrowV1 *object(const Context &c,
                                                  std::uintptr_t id) {
  const auto &m = c.graph().candidate.objects;
  std::int32_t k{};
  const dh2::world::CanonicalObjectBorrowV1 *o{};
  bool next = m->source_ordered_begin_v38(k, o);
  while (next) {
    if (o && o->identity == id)
      return o;
    next = m->source_ordered_next_v38(k, k, o);
  }
  return nullptr;
}
bool require_campaign_level(const Context &c, std::string &e) {
  const auto &g = c.graph();
  if (g.mode != dh::foundation::actor_frame::SourceCampaignBackendModeV1::campaign ||
      !g.candidate.level || !g.services.level ||
      !same(g.services.level, g.candidate.level))
    return need("actual nonnull campaign Level", e);
  dh::foundation::actor_frame::SourceCurrentLevelBorrowV1 level;
  if (!g.current_level->current(level, e)) return false;
  if (!level || !same(level.level(), g.candidate.level) ||
      !g.current_level->still_current(level, e)) {
    if (e.empty()) e = "Required live campaign Level at reached level-dependent callback";
    return false;
  }
  return true;
}
} // namespace
bool borrow_source_campaign_candidate_v55(SourceCampaignCandidateBorrowV55 &out,
                                          std::string &e) {
  auto c = Context::borrow_current(e);
  if (!c)
    return false;
  out = c->graph().candidate;
  return true;
}
bool borrow_source_campaign_condition_world_v70(
    const SourceCampaignCandidateBorrowV55 &in,
    std::shared_ptr<SourceWorldBorrowV61> &out, std::string &e) {
  out.reset();
  auto c = context(in.actual_world, e);
  if (!c)
    return false;
  const auto &actual = c->graph().candidate;
  if (in.application != actual.application || in.objects != actual.objects ||
      in.level != actual.level || in.floors != actual.floors ||
      in.roots != actual.roots)
    return need("candidate owner identities", e);
  out = c->graph().world;
  return true;
}
bool borrow_source_campaign_character_v62(const std::shared_ptr<void> &w,
                                          std::uintptr_t id,
                                          SourceCampaignCharacterBorrowV62 &out,
                                          std::string &e) {
  out = {};
  auto c = context(w, e);
  if (!c)
    return false;
  auto r = c->graph().characters->find(id);
  auto o = object(*c, id);
  if (!r || !r->actor || !r->actor->object || !r->actor->machine ||
      r->actor->object->identity != id ||
      r->actor->machine->native_fsm().character != id ||
      r->services.world != w ||
      r->services.canonical_objects != c->graph().candidate.objects || !o ||
      !o->as_character)
    return need("published same canonical Character record", e);
  std::uintptr_t selected{};
  if (!o->as_character(o->context, selected, e) || selected != id)
    return need("same ObjectManager Character receiver", e);
  out = {w, std::move(r)};
  e.clear();
  return true;
}
bool source_campaign_object_as_character_v114(const std::shared_ptr<void> &w,
                                              std::uintptr_t id,
                                              std::uintptr_t &out,
                                              std::string &e) {
  out = 0;
  // Source interaction_v114: a NULL Handle requires no World/current context.
  if (!id) {
    e.clear();
    return true;
  }
  auto c = context(w, e);
  if (!c)
    return false;
  auto o = object(*c, id);
  if (!o || !o->lease || !o->as_character)
    return need("published object IsCharacter", e);
  // The selected virtual may unpublish itself. Retain its exact receiver lease
  // and callback fields before entering it, as in the recovered source adapter.
  const auto captured = *o;
  return captured.as_character(captured.context, out, e);
}
bool borrow_source_campaign_object_base_v77(
    const SourceCampaignCandidateBorrowV55 &candidate, std::uintptr_t id,
    std::shared_ptr<void> &pin,
    dh2::world::CanonicalGameObjectBaseOwnerV1 *&base, std::string &e) {
  pin.reset();
  base = nullptr;
  std::shared_ptr<SourceWorldBorrowV61> w;
  if (!borrow_source_campaign_condition_world_v70(candidate, w, e))
    return false;
  if (!w->gameobject_graph_v68)
    return need("canonical GameObject graph", e);
  return w->gameobject_graph_v68->borrow_base_v77(id, pin, base, e);
}
bool source_campaign_character_sync_visibility_v86(
    const std::shared_ptr<void> &w, std::uintptr_t id, std::string &e) {
  SourceCampaignCharacterBorrowV62 b;
  if (!borrow_source_campaign_character_v62(w, id, b, e))
    return false;
  auto &r = *b.character;
  const auto *stored = r.actor->source_bool_field(0x80);
  if (!stored)
    return need("visible80", e);
  const auto visual_id = r.actor->source_visual();
  if (!visual_id) {
    e.clear();
    return true;
  }
  auto v = r.visual ? r.visual->visual() : nullptr;
  if (!v || reinterpret_cast<std::uintptr_t>(v.get()) != visual_id)
    return need("retained actual VisualObject2d8", e);
  bool local = *stored != 0;
  if (local) {
    bool player{};
    if (!r.is_player(player, e))
      return false;
    if (!player) {
      std::int32_t ai{};
      auto rows = r.design.ai();
      if (!rows || !r.properties ||
          dh2_character_target_ai_id(
              &ai, r.properties->resolved.data(),
              static_cast<std::uint32_t>(rows->rows.size())) ||
          ai < 0 || std::size_t(ai) >= rows->rows.size())
        return need("authored AI row", e);
      if (rows->rows[ai].type != 3) {
        auto zoned = r.actor->source_bool_field(0x2ee);
        if (!zoned)
          return need("zoned2ee", e);
        if (*zoned) {
          auto inside = r.actor->source_bool_field(0x2f0);
          if (!inside)
            return need("zone2f0", e);
          if (!*inside)
            local = false;
        }
      }
    }
  }
  return v->set_root_local_visibility_v3(local, e);
}
bool source_campaign_character_set_visible_v96(const std::shared_ptr<void> &w,
                                               std::uintptr_t id, bool visible,
                                               std::string &e) {
  SourceCampaignCharacterBorrowV62 b;
  if (!borrow_source_campaign_character_v62(w, id, b, e))
    return false;
  std::uint8_t stored{};
  return b.character->actor->source_set_visible80_v96(visible, stored, e) &&
         source_campaign_character_sync_visibility_v86(w, id, e);
}
bool source_campaign_character_command_look_v114(const std::shared_ptr<void> &w,
                                                 std::uintptr_t id,
                                                 std::uintptr_t target,
                                                 std::string &e) {
  SourceCampaignCharacterBorrowV62 b;
  if (!borrow_source_campaign_character_v62(w, id, b, e))
    return false;
  auto c = context(w, e);
  auto &r = *b.character;
  dh2::character::ControllerCommandState32 source;
  if (!r.actor->controller || !r.services.controller ||
      !r.services.controller(source, r, e))
    return need("active source controller", e);
  auto active = r.actor->controller->command_state(source.global_blocked);
  if (!active || active->owner != id ||
      active->controller != r.actor->controller->identity())
    return need("same controller378", e);
  struct Dispatch {
    std::shared_ptr<Context> c;
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> r;
    std::string *e;
  };
  Dispatch d{c, b.character, &e};
  const dh2::character::CharacterControlServices16 services{
      &d, [](void *raw, const auto *q, auto *out) {
        auto &d = *static_cast<Dispatch *>(raw);
        if (!q || !out || q->reserved)
          return -1;
        if (q->service == dh2::character::control_target_position) {
          auto o = object(*d.c, q->subject);
          std::uintptr_t id{};
          if (!o || !o->as_character || !o->as_character(o->context, id, *d.e))
            return -1;
          std::shared_ptr<void> pin;
          const float *p{};
          if (id) {
            SourceCampaignCharacterBorrowV62 b;
            if (!borrow_source_campaign_character_v62(
                    d.c->graph().candidate.actual_world, id, b, *d.e))
              return -1;
            auto &a = *b.character->actor;
            dh2::world::GameObjectInitializationFieldsV62 f;
            if (!a.inherited_initialization_fields_v62(b.character->actor, f,
                                                       *d.e))
              return -1;
            auto n = f.pointer(0x180);
            if (!n)
              return -1;
            p = a.source_position160_v7();
            if (*n) {
              auto v = a.source_bool_field(0x80);
              if (!v)
                return -1;
              if (*v)
                p = a.runtime.target_position;
            }
            pin = b.character;
          } else {
            dh2::world::CanonicalGameObjectBaseOwnerV1 *base{};
            if (!borrow_source_campaign_object_base_v77(
                    d.c->graph().candidate, q->subject, pin, base, *d.e))
              return -1;
            auto n = base->pointer(0x180);
            if (!n)
              return -1;
            p = base->vector3(0x160);
            if (*n) {
              auto v = base->byte(0x80);
              if (!v)
                return -1;
              if (*v)
                p = base->runtime().target_position;
            }
          }
          if (!p)
            return -1;
          for (unsigned i = 0; i < 3; ++i)
            out->position[i] = p[i];
          return 1;
        }
        if (q->service == dh2::character::control_look_at_point) {
          auto &a = *d.r->actor;
          if (q->subject != a.object->identity)
            return -1;
          auto p = a.source_position160_v7();
          if (!p)
            return -1;
          dh2::character::LookAtState16 state{{p[0], p[1], p[2]},
                                              a.runtime.rotation.heading_angle};
          if (dh2_character_look_at_point(&state, q->position))
            return -1;
          a.runtime.rotation.heading_angle = state.heading_angle;
          return 1;
        }
        return -1;
      }};
  return dh2_character_controller_character(
             active, dh2::character::controller_look_object, target,
             &services) == 1
             ? true
             : need("native Cmd_LookAt delivery", e);
}
bool borrow_source_campaign_fx_v77(const std::shared_ptr<void> &w,
                                   std::shared_ptr<void> &pin,
                                   dh2::fx::CharacterMeshFxOwnerV4 *&out,
                                   std::string &e) {
  pin.reset();
  out = nullptr;
  auto c = context(w, e);
  if (!c)
    return false;
  auto fx = c->graph().services.fx;
  if (!fx)
    return need("actual CharacterMeshFxOwnerV4", e);
  pin = fx;
  out = fx.get();
  return true;
}
bool borrow_actual_application_audio_v42(
    dh2::audio::AudioApplicationBorrowV42 &out, std::string &e) {
  auto c = Context::borrow_current(e);
  if (!c)
    return false;
  out = c->graph().services.audio;
  e.clear();
  return true;
}
bool submit_campaign_audio_v46(
    dh2::audio::AudioCategoryV46 category,
    const dh2::audio::AudioApplicationBorrowV42 &captured,
    const std::shared_ptr<void> &w, const dh2::character::CombatSoundPlayV1 &q,
    std::string &e) {
  auto c = context(w, e);
  if (!c)
    return false;
  if (!require_campaign_level(*c, e)) return false;
  auto bridge = c->graph().services.audio_bridge;
  if (!bridge || bridge->world() != w ||
      bridge->manager().manager != captured.manager)
    return need("same AudioCampaignBridgeV46/captured manager", e);
  auto timestamp = dh2::audio::AudioAuthoredEventScopeV46::current();
  return timestamp > 0 ? bridge->submit(category, q, timestamp, e)
                       : bridge->source_prefix_without_clock(q, e);
}
bool source_campaign_character_zoning_v108(const std::shared_ptr<void> &w,
                                           std::uintptr_t id, bool enabled,
                                           std::string &e) {
  auto c = context(w, e);
  if (!c)
    return false;
  auto &s = c->graph().services;
  if (!require_campaign_level(*c, e)) return false;
  if (!s.zoning)
    return need("current Level zoning", e);
  SourceCampaignCharacterBorrowV62 b;
  return borrow_source_campaign_character_v62(w, id, b, e) &&
         s.zoning(id, enabled, e);
}
bool source_campaign_character_interact_v114(const std::shared_ptr<void> &w,
                                             std::uintptr_t id,
                                             std::uintptr_t other,
                                             std::string &e) {
  auto c = context(w, e);
  if (!c)
    return false;
  auto &s = c->graph().services;
  if (!require_campaign_level(*c, e)) return false;
  if (!s.interact)
    return need("current Level Character interact", e);
  SourceCampaignCharacterBorrowV62 b;
  return borrow_source_campaign_character_v62(w, id, b, e) &&
         s.interact(id, other, e);
}
bool source_campaign_noncharacter_interact_v114(const std::shared_ptr<void> &w,
                                                std::uintptr_t id,
                                                std::uintptr_t other,
                                                std::string &e) {
  auto c = context(w, e);
  if (!c)
    return false;
  auto &s = c->graph().services;
  if (!require_campaign_level(*c, e)) return false;
  if (!s.noncharacter_interact)
    return need("current Level noncharacter interact", e);
  std::uintptr_t character{};
  if (!source_campaign_object_as_character_v114(w, id, character, e))
    return false;
  if (character)
    return need("noncharacter receiver", e);
  return s.noncharacter_interact(id, other, e);
}
bool source_campaign_ai_is_my_turn_v105(const std::shared_ptr<void> &w,
                                        std::uintptr_t ai, bool &out,
                                        std::string &e) {
  out = false;
  auto c = context(w, e);
  if (!c)
    return false;
  auto &s = c->graph().services;
  if (!s.targets || s.targets != c->graph().world->character_targets_v116 ||
      !s.ai_turn)
    return need("same World AI queue turn", e);
  return s.ai_turn(ai, out, e);
}
int source_campaign_limbus_body_v118(
    dh2::world::CanonicalCharacterCandidateRecordV60 &r,
    const dh2::character::StateOwnerRequest48 &q, std::string &e) {
  auto c = context(r.services.world, e);
  if (!c)
    return -1;
  auto &s = c->graph().services;
  if (!require_campaign_level(*c, e)) return -1;
  if (!s.limbus) {
    need("current Level Limbus", e);
    return -1;
  }
  if (!r.actor || !r.actor->object ||
      c->graph().characters->find(r.actor->object->identity).get() != &r) {
    need("same Limbus record", e);
    return -1;
  }
  return s.limbus(r, q, e);
}
bool source_campaign_character_aggro_event_v84(
    const std::shared_ptr<void> &w, std::uint32_t bit, std::uintptr_t owner,
    std::uintptr_t target, const dh2_script_callback_scope *scope,
    std::string &e) {
  auto c = context(w, e);
  if (!c)
    return false;
  auto &s = c->graph().services;
  if (!s.targets || s.targets != c->graph().world->character_targets_v116 ||
      !s.aggro)
    return need("same World aggro AI owner", e);
  return s.aggro(bit, owner, target, scope, e);
}
bool source_campaign_character_clear_aggro_v84(
    const std::shared_ptr<void> &w, std::uintptr_t owner, std::uintptr_t other,
    const dh2_script_callback_scope *scope, std::string &e) {
  auto c = context(w, e);
  if (!c)
    return false;
  auto &s = c->graph().services;
  if (!s.targets || s.targets != c->graph().world->character_targets_v116 ||
      !s.clear_aggro)
    return need("same World reciprocal aggro owner", e);
  return s.clear_aggro(owner, other, scope, e);
}
} // namespace model_renderer

