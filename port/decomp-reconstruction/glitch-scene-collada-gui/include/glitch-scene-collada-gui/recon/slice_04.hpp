// Slice 04 declarations: glitch::gui text/window/toolbar classes and glitch::scene::CBatchMesh.
// Layouts for these classes are not mapped yet (IGUIElement, CGUITable, CGUITTFace,
// CBatchMesh member block before +0x20), so the types below are empty stand-ins that only
// carry the member signatures. Bodies stay unwritten until the layouts are known.
// Namespace recon_04 avoids collisions with the existing glitch::gui and glitch::scene types.
#pragma once

#include <cstdint>

namespace glitch::collada::recon_04 {

class IGUIElement {};

class CGUITable {
public:
    // 0x55a914 _ZN6glitch3gui10CGUITable9addColumnEPKwi
    void addColumn(const wchar_t* name, int index);
    // 0x55adfc _ZN6glitch3gui10CGUITable6addRowEj
    void addRow(unsigned int index);
};

class IGUIToolBar : public IGUIElement {
public:
    // 0x55b190 _ZN6glitch3gui11IGUIToolBarC2EPNS0_14IGUIEnvironmentEPNS0_11IGUIElementEiNS_4core4rectIiEE
    IGUIToolBar();
    // 0x55b908 _ZN6glitch3gui11IGUIToolBarD0Ev (deleting destructor) is skipped
    ~IGUIToolBar();
};

class CGUIToolBar : public IGUIToolBar {
public:
    // 0x55afcc _ZN6glitch3gui11CGUIToolBar22updateAbsolutePositionEv
    void updateAbsolutePosition();
    // 0x55b014 _ZN6glitch3gui11CGUIToolBar9addButtonEiPKwS3_RKN5boost13intrusive_ptrINS_5video8ITextureEEESA_bb
    void* addButton(int id, const wchar_t* text, const wchar_t* tooltip, void* image, void* pressedImage, bool isPushButton, bool useAlphaChannel);
    // 0x55b450 _ZN6glitch3gui11CGUIToolBarC2EPNS0_14IGUIEnvironmentEPNS0_11IGUIElementEiNS_4core4rectIiEE
    CGUIToolBar();
    // 0x55b740 _ZN6glitch3gui11CGUIToolBar7onEventERKNS_6SEventE
    bool onEvent(const void* event);
    // 0x55b83c _ZN6glitch3gui11CGUIToolBarD1Ev
    ~CGUIToolBar();
    // 0x55b984 _ZN6glitch3gui11CGUIToolBar4drawEv
    void draw();
};

class CGUITTGlyph {
public:
    // 0x55ba34 _ZN6glitch3gui11CGUITTGlyphC2Ev
    CGUITTGlyph();
    // 0x55c6e8 _ZN6glitch3gui11CGUITTGlyphD1Ev
    ~CGUITTGlyph();
    // 0x55c7a4 _ZN6glitch3gui11CGUITTGlyph4FreeEPNS_5video12IVideoDriverE
    void Free(void* driver);
    // 0x55c7ec _ZN6glitch3gui11CGUITTGlyphaSERKS1_
    CGUITTGlyph& operator=(const CGUITTGlyph& other);
    // 0x55cbfc _ZN6glitch3gui11CGUITTGlyph5cacheEjPNS0_9CGUITTFaceEPNS_5video12IVideoDriverEb
    void* cache(unsigned int glyphIndex, void* face, void* driver, bool bold);
};

class CGUITTFace {
public:
    // 0x55c564 _ZN6glitch3gui10CGUITTFaceC2Ev
    CGUITTFace();
    // 0x55c2f8 _ZN6glitch3gui10CGUITTFaceD1Ev
    ~CGUITTFace();
    // 0x55c3cc _ZN6glitch3gui10CGUITTFace4loadEPNS_2io9IReadFileE
    bool load(void* readFile);
    // 0x55c450 _ZN6glitch3gui10CGUITTFace4loadEPKc
    bool load(const char* path);
};

class CGUITTLibrary {
public:
    // 0x55c518 _ZN6glitch3gui13CGUITTLibraryC2Ev
    CGUITTLibrary();
    // 0x55c48c _ZN6glitch3gui13CGUITTLibraryD1Ev
    ~CGUITTLibrary();
};

class IGUITTFont {};

class CGUITTFont : public IGUITTFont {
public:
    // 0x55bb30 _ZN6glitch3gui11CGUITTFontC2EPNS_5video12IVideoDriverE
    explicit CGUITTFont(void* driver);
    // 0x55cae0 _ZN6glitch3gui11CGUITTFontD1Ev
    ~CGUITTFont();
    // 0x55bc38 _ZN6glitch3gui11CGUITTFont9setBorderEjNS_5video6SColorE
    void setBorder(unsigned int width, std::uint32_t color);
    // 0x55bfec _ZNK6glitch3gui11CGUITTFont9drawGlyphEPKNS0_11CGUITTGlyphENS_4core11position2dIiEEPKNS4_4rectIiEENS_5video6SColorE
    void drawGlyph(const void* glyph, const void* pos, const void* clip, std::uint32_t color) const;
    // 0x55c110 _ZNK6glitch3gui11CGUITTFont17drawGlyphInTextureEPKNS0_11CGUITTGlyphERKN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core11position2dIiEEPKNS8_4rectIiEENS_5video6SColorE
    void drawGlyphInTexture(const void* glyph, const void* texture, const void* pos, const void* clip, std::uint32_t color) const;
    // 0x55c240 _ZNK6glitch3gui11CGUITTFont19getVertBearingFactorEv
    float getVertBearingFactor() const;
    // 0x55c97c _ZN6glitch3gui11CGUITTFont11clearGlyphsEv
    void clearGlyphs();
    // 0x55d218 _ZNK6glitch3gui11CGUITTFont14getGlyphByValueEj
    unsigned int getGlyphByValue(unsigned int value) const;
    // 0x55d2b0 _ZNK6glitch3gui11CGUITTFont18getWidthFromCharacterEj
    int getWidthFromCharacter(unsigned int value) const;
    // 0x55d340 _ZNK6glitch3gui11CGUITTFont16getCharacterFromPosEPKci
    int getCharacterFromPos(const char* text, int pos) const;
    // 0x55d3b8 _ZNK6glitch3gui11CGUITTFont16getCharacterFromPosEPKwi
    int getCharacterFromPos(const wchar_t* text, int pos) const;
    // 0x55d418 _ZNK6glitch3gui11CGUITTFont12getDimensionEPKc
    void getDimension(const char* text);
    // 0x55d488 _ZNK6glitch3gui11CGUITTFont12getDimensionEPKw
    void getDimension(const wchar_t* text);
    // 0x55d4e0 _ZNK6glitch3gui11CGUITTFont13drawInTextureEPKcRKN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core4rectIiEENS5_6SColorEbb
    void drawInTexture(const char* text, const void* texture, const void* area, std::uint32_t color, bool hcenter, bool vcenter);
    // 0x55d854 _ZNK6glitch3gui11CGUITTFont4drawEPKcRKNS_4core4rectIiEENS_5video6SColorEbbPKS5_
    void draw(const char* text, const void* area, std::uint32_t color, bool hcenter, bool vcenter, const void* clip);
    // 0x55dab8 _ZNK6glitch3gui11CGUITTFont9getHeightEPKc
    int getHeight(const char* text) const;
    // 0x55db50 _ZNK6glitch3gui11CGUITTFont9getHeightEPKw
    int getHeight(const wchar_t* text) const;
    // 0x55dbd8 _ZNK6glitch3gui11CGUITTFont12getGlyphByCharEw
    unsigned int getGlyphByChar(wchar_t ch) const;
    // 0x55dc70 _ZNK6glitch3gui11CGUITTFont13drawInTextureEPKwRKN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core4rectIiEENS5_6SColorEbb
    void drawInTexture(const wchar_t* text, const void* texture, const void* area, std::uint32_t color, bool hcenter, bool vcenter);
    // 0x55e020 _ZNK6glitch3gui11CGUITTFont4drawEPKwRKNS_4core4rectIiEENS_5video6SColorEbbPKS5_
    void draw(const wchar_t* text, const void* area, std::uint32_t color, bool hcenter, bool vcenter, const void* clip);
    // 0x55e9f0 _ZN6glitch3gui11CGUITTFont6attachEPNS0_9CGUITTFaceEjjNS_5video6SColorE
    bool attach(void* face, unsigned int size, unsigned int dpi, std::uint32_t color);
};

class IGUIWindow : public IGUIElement {
public:
    // 0x55ed80 _ZN6glitch3gui10IGUIWindowC2EPNS0_14IGUIEnvironmentEPNS0_11IGUIElementEiNS_4core4rectIiEE
    IGUIWindow();
};

class CGUIWindow : public IGUIWindow {
public:
    // 0x55f040 _ZN6glitch3gui10CGUIWindowC2EPNS0_14IGUIEnvironmentEPNS0_11IGUIElementEiNS_4core4rectIiEE
    CGUIWindow();
    // 0x55fc70 _ZN6glitch3gui10CGUIWindowD1Ev
    ~CGUIWindow();
    // 0x55fd5c _ZN6glitch3gui10CGUIWindowD2Ev (VTT-style variant with base vtable argument)
    void destroyWithVtt(void* vtt);
    // 0x55fe1c _ZN6glitch3gui10CGUIWindow7onEventERKNS_6SEventE
    bool onEvent(const void* event);
    // 0x56008c _ZN6glitch3gui10CGUIWindow4drawEv
    void draw();
};

class CBatchMesh {
public:
    // 0x578924 _ZN6glitch5scene10CBatchMeshC2Ev
    CBatchMesh();
    // 0x578aa4 _ZN6glitch5scene10CBatchMeshC2Ej
    explicit CBatchMesh(unsigned int segmentCount);
    // 0x57a59c _ZN6glitch5scene10CBatchMeshD1Ev
    ~CBatchMesh();
    // 0x578c50 _ZNK6glitch5scene10CBatchMesh13getMeshBufferEj
    void* getMeshBuffer(unsigned int batchIndex) const;
    // 0x578ccc _ZNK6glitch5scene10CBatchMesh11getMaterialEj
    void* getMaterial(unsigned int batchIndex) const;
    // 0x578cf4 _ZNK6glitch5scene10CBatchMesh30getMaterialVertexAttributeMapEj
    void* getMaterialVertexAttributeMap(unsigned int batchIndex) const;
    // 0x57a4a8 _ZN6glitch5scene10CBatchMesh5stripEv
    void strip();
    // 0x57a6d4 _ZNK6glitch5scene10CBatchMesh17updateBoundingBoxEv
    void updateBoundingBox() const;
    // 0x57b174 _ZN6glitch5scene10CBatchMesh28initStaticSegmentBoundingBoxesEv
    void initStaticSegmentBoundingBoxes();
    // 0x57b900 _ZN6glitch5scene10CBatchMesh8addBatchEv
    unsigned int addBatch();
    // 0x57b96c _ZN6glitch5scene10CBatchMesh18quantizeComponentsEbbPNS_5video12IVideoDriverE
    void quantizeComponents(bool a, bool b, void* driver);
    // 0x57bc78 _ZN6glitch5scene10CBatchMesh11sortBatchesEPKNS_5video12IVideoDriverEPj
    unsigned int sortBatches(const void* driver, unsigned int* order);
    // 0x57bd44 _ZN6glitch5scene10CBatchMesh5clearEv
    void clear();
    // 0x57be34 _ZN6glitch5scene10CBatchMesh9setBufferEjRKN5boost13intrusive_ptrINS1_11CMeshBufferEEERKNS2_INS_5video9CMaterialEEE
    void setBuffer(unsigned int batchIndex, const void* buffer, const void* material);
    // 0x57c19c _ZN6glitch5scene10CBatchMesh4sortEPKNS_5video12IVideoDriverE
    unsigned int sort(const void* driver);
    // 0x57d98c _ZN6glitch5scene10CBatchMesh10addSegmentEtjs
    unsigned int addSegment(unsigned short a, unsigned int b, short c);
    // 0x57a368 _ZN6glitch5scene10CBatchMesh6SBatchD1Ev
    // 0x57ba8c _ZN6glitch5scene10CBatchMesh6SBatchaSERKS2_
    class SBatch {
    public:
        ~SBatch();
        SBatch& operator=(const SBatch& other);
    };
    class SSegment {
    public:
        // 0x578ee0 _ZNK6glitch5scene10CBatchMesh8SSegment4saveEPNS_2io10IWriteFileEb
        bool save(void* writeFile, bool flag) const;
        // 0x57b3d8 _ZN6glitch5scene10CBatchMesh8SSegment4loadEPNS_2io9IReadFileEb
        bool load(void* readFile, bool flag);
        // 0x57b67c _ZN6glitch5scene10CBatchMesh8SSegmentC2ERKS1_
        SSegment(const SSegment& other);
    };
};

class CBatchSceneNode {
public:
    // 0x57e524 _ZNK6glitch5scene15CBatchSceneNode23getTransformedBoundingBoxEv (thunk, skipped)
};

// 0x578d9c _ZN6glitch5scene30getNbVertexAttributeMapsDataSizeERKN5boost13intrusive_ptrINS_5video24CMaterialVertexAttributeMapEEE
unsigned int getNbVertexAttributeMapsDataSize(const void* mapRef);

}  // namespace glitch::collada::recon_04
