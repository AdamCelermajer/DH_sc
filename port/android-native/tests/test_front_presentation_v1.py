"""Run production Front/asset/viewport functions against original splash data.

Only this host harness is compiled. No Android build, APK or emulator is used.
The GPU recorder proves submitted viewport/texture/UV/transform contracts;
integrated pixels and frame-to-frame breathing still require runtime capture.
"""
from pathlib import Path
import hashlib
import os
import shutil
import struct
import subprocess
import tempfile
import unittest
import zipfile
import zlib

ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"
ASSETS = ROOT / "port/android-native/app/src/main/assets"
ARCHIVE = Path(os.environ.get("DH2_ORIGINAL_CACHE", "C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip"))
PREFIX = "com.gameloft.android.GAND.GloftD2SS/files/"


class Bits:
    def __init__(self, data, offset):
        self.data, self.bit = data, offset * 8

    def read(self, count):
        value = 0
        for _ in range(count):
            value = value * 2 + ((self.data[self.bit // 8] >> (7 - self.bit % 8)) & 1)
            self.bit += 1
        return value

    def signed(self, count):
        value = self.read(count)
        return value - (1 << count) if count and value & (1 << (count - 1)) else value

    def offset(self):
        return (self.bit + 7) // 8


def background_matrix(raw):
    data = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b"CWS" else raw
    bits = Bits(data, 8)
    bits.read(bits.read(5) * 4)
    pos = bits.offset() + 4
    while pos + 2 <= len(data):
        header = struct.unpack_from("<H", data, pos)[0]
        pos += 2
        kind, size = header >> 6, header & 63
        if size == 63:
            size = struct.unpack_from("<I", data, pos)[0]
            pos += 4
        if kind in (2, 22, 32):
            bits = Bits(data, pos + 2)
            count = bits.read(5)
            bounds = [bits.signed(count) for _ in range(4)]
            q = bits.offset()
            if bounds == [-4838, 4780, -3284, 3141]:
                assert data[q] == 1 and data[q + 1] in (64, 65, 66, 67)
                assert struct.unpack_from("<H", data, q + 2)[0] == 1
                bits = Bits(data, q + 4)
                scale, skew = [1., 1.], [0., 0.]
                if bits.read(1):
                    count = bits.read(5)
                    scale = [bits.signed(count) / 65536 for _ in range(2)]
                if bits.read(1):
                    count = bits.read(5)
                    skew = [bits.signed(count) / 65536 for _ in range(2)]
                count = bits.read(5)
                tx, ty = [bits.signed(count) for _ in range(2)]
                a, d = scale
                c, b = skew
                determinant = a * d - b * c
                return [d / determinant, -b / determinant, (b * ty - d * tx) / determinant,
                        -c / determinant, a / determinant, (c * tx - a * ty) / determinant]
        pos += size
    raise AssertionError("Original droid/i9000 splash background absent")


def function(text, signature):
    start = text.index(signature)
    brace = text.index("{", start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (text[end] == "{") - (text[end] == "}")
        end += 1
    return text[start:end] + "\n"


class FrontPresentationTest(unittest.TestCase):
    def test_menu_scene_surface_and_camera(self):
        # Compile the production viewport owner and camera setters verbatim.
        # Original CreateAvatarCamera (0x42bf3c) uses FOV 0x3f3579c8 and
        # aspect 0x3fd578e9. The actual matrix kernel below makes screen-space
        # isotropy independent of the presentation adapter's implementation.
        harness = r'''
#include "gameplay_camera_matrix_v8.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
namespace dh2::application {struct ApplicationServicesOwnerV5 {};}
namespace dh2::camera {
using PointV2=std::array<float,3>;
struct CameraViewV11 {
 MatrixV8 view,projection;PointV2 eye{},target{},up{0,0,1};
 float fov{},aspect{},near_plane{},far_plane{};
};
struct CameraProceduralNodeV16 {
 bool alive_=true;CameraViewV11 view_;
 bool set_data(float,float,float,float,std::string&);
 bool set_up(const PointV2&,std::string&);
 bool view(CameraViewV11& out,std::string&){
  const float input[]{view_.fov,view_.aspect,view_.near_plane,view_.far_plane};
  dh2_camera_perspective_v8(&view_.projection,input);out=view_;return true;
 }
};
#include "camera_setters_under_test.inc"
}
namespace model_renderer {
using GLuint=unsigned;
bool menu_background=true,menu_background_character_visible_v137=true,throw_draw=false;
bool preview_camera_available=true;
auto retained_camera=std::make_shared<dh2::camera::CameraProceduralNodeV16>();
std::array<int,4> viewport{};std::array<int,2> surface{},scene_dimensions{};
std::vector<unsigned char> pixels;dh2::camera::CameraViewV11 submitted_camera;
bool submitted_avatar{},scissor_enabled{};int scene_draws{};std::vector<std::string> diagnostics;
constexpr int ANDROID_LOG_WARN=5;
int __android_log_print(int,const char*,const char* format,...){diagnostics.emplace_back(format);return 0;}
bool active(){return true;}
bool borrow_actual_application_services_v5(std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string&){
 app=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();return true;
}
bool native_menu_preview_camera_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::shared_ptr<dh2::camera::CameraProceduralNodeV16>& camera,std::string&){camera=preview_camera_available?retained_camera:nullptr;return true;}
bool native_menu_preview_camera_view_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 dh2::camera::CameraViewV11& view,bool& present,std::string& error){present=preview_camera_available;if(!present){error.clear();return true;}return retained_camera->view(view,error);}
void glViewport(int x,int y,int w,int h){viewport={x,y,w,h};}
constexpr int GL_SCISSOR_TEST=0x0C11;
void glDisable(int capability){if(capability==GL_SCISSOR_TEST)scissor_enabled=false;}
void glEnable(int capability){if(capability==GL_SCISSOR_TEST)scissor_enabled=true;}
void draw(int width,int height){
 ++scene_draws;
 scene_dimensions={width,height};submitted_avatar=menu_background_character_visible_v137;
 assert(!scissor_enabled);
 std::string error;retained_camera->view(submitted_camera,error);
 // Rasterized coverage of the actual submitted viewport; these are layout
 // pixels, not a claim about final Android scene/shader output.
 for(int y=std::max(0,viewport[1]);y<std::min(surface[1],viewport[1]+viewport[3]);++y)
  for(int x=std::max(0,viewport[0]);x<std::min(surface[0],viewport[0]+viewport[2]);++x)
   pixels[std::size_t(y)*surface[0]+x]=1;
 // A nested scene pass may change GL state before either returning or
 // failing. The Front overlay must still get its full physical surface.
 glEnable(GL_SCISSOR_TEST);glViewport(10,20,30,40);
 if(throw_draw)throw std::runtime_error("GPU endpoint failure");
}
#include "menu_viewport_under_test.inc"
}
int main(){
 using namespace model_renderer;
 const std::array<int,2> dimensions[]{{800,480},{854,480},{1920,1080},{2400,1080},{1080,1920}};
 for(const auto size:dimensions)for(const auto visible:{false,true})for(const auto fail:{false,true}){
  surface=size;pixels.assign(std::size_t(size[0])*size[1],0);viewport={123,0,480,320};glEnable(GL_SCISSOR_TEST);
  const dh2::camera::CameraViewV11 original{{},{},{0,-440,150},{0,0,240},{0,1,0},.7088894844055176f,1.6677523851394653f,10.f,2000.f};
  retained_camera->view_=original;throw_draw=fail;bool threw=false;
  try{draw_menu_background(size[0],size[1],visible);}catch(const std::runtime_error&){threw=true;}
  assert(threw==fail);
  assert((viewport==std::array<int,4>{0,0,size[0],size[1]}));
  assert(!scissor_enabled);
  assert(scene_dimensions==size);
  assert(std::all_of(pixels.begin(),pixels.end(),[](auto pixel){return pixel==1;}));
  // Equal camera-space X/Y segments at the same depth must occupy equal
  // physical pixel lengths. A full viewport with the old fixed aspect fails.
  const auto x_scale=std::abs(submitted_camera.projection.values[0])*size[0];
  const auto y_scale=std::abs(submitted_camera.projection.values[5])*size[1];
  assert(std::abs(x_scale/y_scale-1.f)<1e-6f);
  assert(submitted_camera.fov==original.fov&&submitted_camera.near_plane==original.near_plane&&submitted_camera.far_plane==original.far_plane);
  assert(submitted_camera.eye==original.eye&&submitted_camera.target==original.target&&submitted_camera.up==original.up);
  const auto& restored=retained_camera->view_;
  assert(restored.fov==original.fov&&restored.aspect==original.aspect&&restored.near_plane==original.near_plane&&restored.far_plane==original.far_plane);
  assert(restored.eye==original.eye&&restored.target==original.target&&restored.up==original.up);
  assert(submitted_avatar==visible&&menu_background_character_visible_v137);
 }
 // SelectClass.Show can retire Main's camera before a failed authored
 // transition leaves the underlying Main/EnterName clips in Front's draw list.
 preview_camera_available=false;const int draws_before=scene_draws;bool threw=false;
 glEnable(GL_SCISSOR_TEST);glViewport(10,20,30,40);
 try{draw_menu_background(2400,1080,false);}catch(const std::runtime_error&){threw=true;}
 assert(!threw&&scene_draws==draws_before);
 assert((viewport==std::array<int,4>{0,0,2400,1080}));
 assert(!scissor_enabled);
 assert(!diagnostics.empty()&&diagnostics.back().find("continuing SWF layers")!=std::string::npos);
 std::puts("PASS production menu full-surface pixel coverage, camera isotropy, pose preservation and failure cleanup");
}
'''
        with tempfile.TemporaryDirectory(prefix="dh2-menu-presentation-") as directory:
            temp = Path(directory)
            (temp / "menu_viewport_under_test.inc").write_text(function((CPP / "renderer_front_exports_v87.inc").read_text(), "void draw_menu_background("))
            camera_source = (ROOT / "port/level-world/gameplay_camera_factory_v16.cpp").read_text()
            (temp / "camera_setters_under_test.inc").write_text("\n".join(function(camera_source, "bool CameraProceduralNodeV16::" + method + "(") for method in ("set_data", "set_up")))
            source = temp / "menu-presentation.cpp"
            source.write_text(harness)
            prefix = [] if shutil.which("g++") else ["wsl", "--exec"]

            def convert(path):
                return str(path) if not prefix else subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()

            executable = temp / "menu-presentation"
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-fno-fast-math", "-ffp-contract=off", "-I" + convert(temp), "-I" + convert(ROOT / "port/level-world"), convert(source), convert(ROOT / "port/level-world/gameplay_camera_matrix_v8.cpp"), "-o", convert(executable)]
            result = subprocess.run(command, capture_output=True, text=True, timeout=60)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            result = subprocess.run(prefix + [convert(executable)], capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS production menu", result.stdout)
            print(result.stdout, end="")

    def test_production_presentation(self):
        self.assertTrue(ARCHIVE.is_file(), str(ARCHIVE))
        front = (CPP / "front_ui_session_v87.cpp").read_text()
        prepare = function(front, "bool FrontUiSessionV87::prepare_process_front_v119(")
        self.assertIn("impl_->splash_source_uri_v1=ui::splash_source_uri_v1(width,impl_->settings->language());", prepare)
        self.assertLess(prepare.index("impl_->splash_source_uri_v1=ui::splash_source_uri_v1"), prepare.index('load_front_screen(directory,"main",e)'))
        reset = function(front, "    bool reset_failed(std::string& error)")
        self.assertIn("process_splash_v119={};process_splash_uri_v119.clear()", reset)
        self.assertIn("splash_source_uri_v1.clear();splash_texture_identity_v1=0;rendering_splash_clip_v1=false", reset)
        source_render = function(front, "bool FrontUiSessionV87::render_source_movies_v93(")
        self.assertIn('self.rendering_splash_clip_v1=q.actual_clip_path=="_root.menu_splash";', source_render)
        self.assertIn("splash_scope{self.rendering_splash_clip_v1,previous_splash}", source_render)
        self.assertIn("display_source_stage_clip_v5", source_render)
        self.assertNotIn("advance_frames(", source_render)
        self.assertNotIn("input_advance(", source_render)
        with tempfile.TemporaryDirectory(prefix="dh2-front-presentation-") as directory:
            temp = Path(directory)
            fixtures = temp / "fixtures"
            fixtures.mkdir()
            with zipfile.ZipFile(ARCHIVE) as archive:
                for filename in ("splash_final.tga", "splash_final_droid.tga", "splash_final_i9000.tga", "splash_final_jp.tga", "splash_final_kor.tga"):
                    uri = "data/3d/textures/" + filename
                    file = fixtures / uri
                    file.parent.mkdir(parents=True, exist_ok=True)
                    payload = archive.read(PREFIX + uri)
                    file.write_bytes(payload)
                    print("fixture", uri, len(payload), hashlib.sha256(payload).hexdigest())
                matrices = [background_matrix(archive.read(PREFIX + "data/menus/" + name)) for name in ("dqmenus_droid.swf", "dqmenus_i9000.swf")]
                self.assertEqual(matrices[0], matrices[1])
            (fixtures / "background-uv.txt").write_text(" ".join(format(value, ".12g") for value in matrices[0]))
            cases = []
            for width in (800, 854, 2400):
                for language in (0, 4, 5):
                    suffix = "_jp" if language == 4 else "_kor" if language == 5 else "_i9000" if width == 800 else "_droid" if width == 854 else ""
                    cases.append(f"{width} {language} data/3d/textures/splash_final{suffix}.tga")
            case_file = temp / "cases.txt"
            case_file.write_text("\n".join(cases) + "\n")
            (temp / "front_texture_under_test.inc").write_text(function(front, "    static bool texture("))
            (temp / "front_draw_under_test.inc").write_text(function(front, "    static bool draw("))
            scope_start = source_render.index("  const bool previous_splash=")
            scope_end = source_render.index("  if(!movie->display_source_stage_clip_v5", scope_start)
            scope_end = source_render.index("return false;", scope_end) + len("return false;")
            (temp / "front_splash_scope_under_test.inc").write_text(source_render[scope_start:scope_end])
            (temp / "front_splash_under_test.inc").write_text("\n".join(function(front, "bool FrontUiSessionV87::" + method + "(") for method in ("load_process_splash_v119", "render_process_splash_v119", "clear_process_splash_v119")))
            (temp / "assets_read_under_test.inc").write_text(function((CPP / "original_ui_assets.cpp").read_text(), "bool OriginalUiAssets::read("))
            shutil.copyfile(CPP / "original_ui_asset_catalog.inc", temp / "catalog_under_test.inc")
            prefix = [] if shutil.which("g++") else ["wsl", "--exec"]

            def convert(path):
                if not prefix:
                    return str(path)
                return subprocess.check_output(prefix + ["wslpath", "-a", path.as_posix()], text=True).strip()

            # Menu rendering has its own production-body regression above.
            # Keep the existing fixture's authored splash/SWF coverage intact.
            harness = (ROOT / "port/android-native/tests/front_presentation_v1.cpp").read_text()
            start = harness.index("// Execute the existing scene viewport owner")
            end = harness.index("// The source camera math", start)
            harness = harness[:start] + harness[end:]
            start = harness.index("  const auto w=dimensions[0],h=dimensions[1];model_renderer::viewport=")
            end = harness.index("  for(const auto authored:", start)
            harness = harness[:start] + "  const auto w=dimensions[0],h=dimensions[1];\n" + harness[end:]
            harness = harness.replace("centered 3:2 menu scene and ", "")
            source = temp / "front-presentation.cpp"
            source.write_text(harness)
            includes = [temp] + [ROOT / "port" / name for name in ("engine-ui", "engine-textures", "scene-materials", "asset-payloads")]
            sources = [source] + [ROOT / "port" / name for name in ("engine-ui/viewport.cpp", "engine-textures/textures.cpp", "engine-textures/pvrtc.cpp", "scene-materials/swf_texture.cpp", "asset-payloads/sha256.cpp")]
            executable = temp / "front-presentation"
            command = prefix + ["g++", "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-Wno-misleading-indentation", "-fno-fast-math", "-ffp-contract=off"]
            command += ["-I" + convert(path) for path in includes]
            command += [convert(path) for path in sources] + ["-o", convert(executable)]
            print("compile", subprocess.list2cmdline(command))
            result = subprocess.run(command, capture_output=True, text=True, timeout=60)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            command = prefix + [convert(executable), convert(ASSETS), convert(fixtures), convert(case_file)]
            print("run", subprocess.list2cmdline(command))
            result = subprocess.run(command, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn("PASS production Front", result.stdout)
            print(result.stdout, end="")


if __name__ == "__main__":
    unittest.main()
