#include "../game_object_position_owner_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace pos = dh2::game_object_position_owner_v1;
namespace {
enum class Op { translate, aabb, physical, visual_sync, destination, force };
struct Call { Op op; pos::Identity owner, component; pos::Point3 point; };
struct Fixture { std::vector<Call> calls; };
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
int translate(void* p, pos::Identity o, pos::Identity c, pos::Point3 d,
              std::string&) {
    static_cast<Fixture*>(p)->calls.push_back({Op::translate,o,c,d}); return 0;
}
int aabb(void* p, pos::Identity o, std::string&) {
    static_cast<Fixture*>(p)->calls.push_back({Op::aabb,o,0,{}}); return 0;
}
int physical(void* p, pos::Identity o, pos::Identity c, float x, float y,
             std::string&) {
    static_cast<Fixture*>(p)->calls.push_back({Op::physical,o,c,{x,y,0}}); return 0;
}
int sync(void* p, pos::Identity o, pos::Identity c, std::string&) {
    static_cast<Fixture*>(p)->calls.push_back({Op::visual_sync,o,c,{}}); return 0;
}
int destination(void* p, pos::Identity o, pos::Point3 v, std::string&) {
    static_cast<Fixture*>(p)->calls.push_back({Op::destination,o,0,v}); return 0;
}
int force(void* p, pos::Identity o, pos::Identity c, std::string&) {
    static_cast<Fixture*>(p)->calls.push_back({Op::force,o,c,{}}); return 0;
}
}

int main() {
    try {
        Fixture f;
        pos::Services services{&f, translate, aabb, physical, sync,
                               destination, force};
        pos::Owner owner{0x123456789ULL, 0x2001, 0x3001, 0x4001,
                         {1.0f, 2.0f, 3.0f}};
        std::string error;
        const pos::Point3 target{11.0f, -4.0f, 8.0f};
        require(pos::set_position(&owner, target, true, &services, error) ==
                    pos::Status::complete, "SetPosition failed");
        require(owner.position.x == 11.0f && owner.position.y == -4.0f &&
                    owner.position.z == 8.0f, "cached world position not updated");
        require(f.calls.size() == 5 &&
                    f.calls[0].op == Op::translate &&
                    f.calls[1].op == Op::aabb &&
                    f.calls[2].op == Op::physical &&
                    f.calls[3].op == Op::visual_sync &&
                    f.calls[4].op == Op::destination,
                "SetPosition service order changed");
        require(f.calls[0].owner == owner.identity &&
                    f.calls[0].component == owner.instance_transform &&
                    f.calls[0].point.x == 10.0f &&
                    f.calls[0].point.y == -6.0f &&
                    f.calls[0].point.z == 5.0f,
                "instance transform delta or identity mismatch");
        require(f.calls[2].owner == owner.identity &&
                    f.calls[2].component == owner.physical_object &&
                    f.calls[2].point.x == target.x &&
                    f.calls[2].point.y == target.y &&
                    f.calls[2].point.z == 0.0f,
                "physical adapter must receive x/y only");
        require(f.calls[4].point.x == target.x &&
                    f.calls[4].point.y == target.y &&
                    f.calls[4].point.z == target.z,
                "destination target mismatch");
        require(pos::force_update_position(&owner, &services, error) ==
                    pos::Status::complete && f.calls.size() == 6 &&
                    f.calls.back().op == Op::force &&
                    f.calls.back().owner == owner.identity &&
                    f.calls.back().component == owner.visual_object,
                "ForceUpdatePosition identity/order mismatch");
        std::cout << "{\"set_position_order\":true,\"force_update_identity\":true}\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
