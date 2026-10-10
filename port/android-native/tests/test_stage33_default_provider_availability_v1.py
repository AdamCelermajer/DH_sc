"""Static provider-availability contract for the default Stage33 composer.

The composer lives in model_renderer's large native TU, so this test checks its
default wiring contract without constructing unrelated campaign owners. In
particular, the two native values with no typed source producer must stay
unavailable instead of becoming guessed defaults.
"""
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
COMPOSER = ROOT / "port/android-native/app/src/main/cpp/renderer_source_stage34_v80.inc"
STAGE = ROOT / "port/level-loader/stage_loader_save_restore_v1.hpp"


def test_stage33_default_provider_contract():
    composer = COMPOSER.read_text(encoding="utf-8")
    stage = STAGE.read_text(encoding="utf-8")

    # Floor height is backed by the SAME candidate-owned PF floor lease and
    # native world-height primitive; the query's original includeSpecial is 0.
    assert "if(!native33.floor_height)native33.floor_height=" in composer
    assert "auto floors=c&&c->rooms?c->rooms->world():nullptr;" in composer
    assert "w->source_pf_floors_v115!=floors" in composer
    assert "dh2_nav_world_height(&hit,&floors->collision_world,point.data(),0)" in composer

    # No default IsLocalPlayerHosting callback exists: online host identity
    # has no producer in the composed PM/CNet owner.
    assert "std::function<bool(const RestorePlayerManagerBorrowV1&,bool&,std::string&)> local_player_hosting;" in stage
    assert "native33.local_player_hosting" not in composer

    # The actual PM frame projection is pointer-width; the composer must not
    # expose it as float3 position6d4.
    assert "out={pm,reinterpret_cast<std::uintptr_t>(pm->manager()),&fields->byte6d0,nullptr};" in composer
    assert "native33.position6d4" not in composer


if __name__ == "__main__":
    test_stage33_default_provider_contract()
    print("Stage33 default provider contract: PASS")
