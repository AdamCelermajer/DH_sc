# Additive source-built matching-version backend; include from a caller CMake.
set(DH2_FT237 "${CMAKE_CURRENT_LIST_DIR}/vendor/freetype-2.3.7")
add_library(dh2_freetype237 STATIC
 "${DH2_FT237}/src/base/ftbase.c" "${DH2_FT237}/src/base/ftinit.c"
 "${DH2_FT237}/src/base/ftsystem.c" "${DH2_FT237}/src/base/ftdebug.c"
 "${DH2_FT237}/src/base/ftbbox.c" "${DH2_FT237}/src/base/ftglyph.c"
 "${DH2_FT237}/src/base/ftmm.c" "${DH2_FT237}/src/base/ftstroke.c"
 "${DH2_FT237}/src/base/ftsynth.c" "${DH2_FT237}/src/base/fttype1.c"
 "${DH2_FT237}/src/base/ftpatent.c" "${DH2_FT237}/src/base/ftxf86.c"
 "${DH2_FT237}/src/autofit/autofit.c" "${DH2_FT237}/src/truetype/truetype.c"
 "${DH2_FT237}/src/type1/type1.c" "${DH2_FT237}/src/cff/cff.c"
 "${DH2_FT237}/src/cid/type1cid.c" "${DH2_FT237}/src/pfr/pfr.c"
 "${DH2_FT237}/src/type42/type42.c" "${DH2_FT237}/src/winfonts/winfnt.c"
 "${DH2_FT237}/src/pcf/pcf.c" "${DH2_FT237}/src/psaux/psaux.c"
 "${DH2_FT237}/src/psnames/psnames.c" "${DH2_FT237}/src/pshinter/pshinter.c"
 "${DH2_FT237}/src/raster/raster.c" "${DH2_FT237}/src/sfnt/sfnt.c"
 "${DH2_FT237}/src/smooth/smooth.c" "${DH2_FT237}/src/bdf/bdf.c"
 "${DH2_FT237}/src/gzip/ftgzip.c" "${DH2_FT237}/src/lzw/ftlzw.c")
target_include_directories(dh2_freetype237 SYSTEM PUBLIC "${DH2_FT237}/include")
target_compile_definitions(dh2_freetype237 PRIVATE FT2_BUILD_LIBRARY)
set_target_properties(dh2_freetype237 PROPERTIES POSITION_INDEPENDENT_CODE ON)
