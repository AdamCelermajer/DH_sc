// Slice 33 reconstruction (see include/glitch-scene-collada-gui/recon/slice_33.hpp).
// Each function keeps the original address and mangled symbol in its comment.
// Bodies marked "not reconstructed" keep the signature and return a neutral value; the
// original body is a larger routine whose parts depend on unmapped classes.

#include "glitch-scene-collada-gui/recon/slice_33.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstring>

namespace glitch::collada::recon_33 {

namespace {

// Attribute constants of the particle affector and emitter index deserializers.
// The original compares the attribute name with the expected key and stops at the
// first mismatch; each helper below does the same and returns the next index.
bool nameIs(IAttributesStandIn& attrs, std::int32_t index, const char* expected) {
    const char* name = attrs.attributeNameAt(index);
    return name != nullptr && std::strcmp(name, expected) == 0;
}

}  // namespace

// ---------------------------------------------------------------------------
// glitch::scene::CColladaBinaryFileLoader
// ---------------------------------------------------------------------------

// 0x6b9310 _ZN6glitch5scene24CColladaBinaryFileLoaderC2EPNS0_13CSceneManagerERKN5boost13intrusive_ptrINS_2io11IFileSystemEEE
void constructColladaBinaryFileLoaderC2(ColladaBinaryFileLoaderStandIn& self,
                                        CSceneManagerStandIn* sceneManager,
                                        IFileSystemStandIn* fileSystem) {
    self.refCount = 1;                 // +0x04
    self.sceneManager = sceneManager;  // +0x08
    self.fileSystem = fileSystem;      // +0x0c
    if (fileSystem != nullptr) {
        ++fileSystem->refCount;
    }
}

// 0x6b9360 _ZN6glitch5scene24CColladaBinaryFileLoaderC1EPNS0_13CSceneManagerERKN5boost13intrusive_ptrINS_2io11IFileSystemEEE
// Same body as the C2 constructor in the original.
void constructColladaBinaryFileLoaderC1(ColladaBinaryFileLoaderStandIn& self,
                                        CSceneManagerStandIn* sceneManager,
                                        IFileSystemStandIn* fileSystem) {
    constructColladaBinaryFileLoaderC2(self, sceneManager, fileSystem);
}

// ---------------------------------------------------------------------------
// glitch::collada::detail skin techniques
// ---------------------------------------------------------------------------

// 0x66b948 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
int prepareMatrixSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                            IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                            MeshBufferStandIn* meshBuffer) {
    (void)driver;
    if (step == kPrepareBufferStepUpload) {
        self.prepareSkinBuffer(skinBuffer, meshBuffer);  // vtable +0x18
    }
    return kPrepareBufferReturnHandled;
}

// 0x66d088 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
int prepareQuatSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                          IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                          MeshBufferStandIn* meshBuffer) {
    (void)driver;
    if (step == kPrepareBufferStepUpload) {
        self.prepareSkinBuffer(skinBuffer, meshBuffer);  // vtable +0x18
    }
    return kPrepareBufferReturnHandled;
}

// 0x66e780 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
int prepareTextureSkinBuffer(HardwareSkinTechniqueStandIn& self, int step,
                             IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                             MeshBufferStandIn* meshBuffer) {
    (void)driver;
    if (step == kPrepareBufferStepUpload) {
        self.prepareSkinBuffer(skinBuffer, meshBuffer);  // vtable +0x18
    }
    return kPrepareBufferReturnHandled;
}

// 0x66f7a8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
// Partial: the material technique lookup that yields the flag mask is passed in as
// `technique`; the original reads it through the material chain. The skin buffer
// reference count that the original takes and drops around the driver call is not
// modelled.
int prepareSoftwareSkinBuffer(SoftwareSkinTechniqueStandIn& self, int step,
                              IVideoDriverStandIn* driver, SkinBufferStandIn& skinBuffer,
                              MeshBufferStandIn* meshBuffer, std::uint32_t technique) {
    // The original uses 0x60001 when the byte at +0x18 is set and 0x20001 otherwise.
    const std::uint32_t flags = self.hasSkinningFlag ? 393217u : 131073u;
    const std::uint32_t processBuffer =
        self.getProcessBuffer(driver, step == 0, flags & technique);
    if ((processBuffer & 4u) != 0) {
        self.processSkinBuffer(skinBuffer, meshBuffer);  // vtable +0x18
    }
    return static_cast<int>(processBuffer);
}

// ---------------------------------------------------------------------------
// glitch::collada::SAnimationAccessor
// ---------------------------------------------------------------------------

// 0x66b374 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiRKNS_3res6vectorIiEEiRii
// Not reconstructed. The original caches the last key frame in the accessor (value at
// +0x08, index at +0x0c, dirty flag at +0x0d). It dispatches on getTimeInternalType to
// findKeyFrameNoEx<unsigned short,30>, <int,1000> or <unsigned char,30>; those templates
// are not mapped. Returns 0 here.
std::uint8_t findKeyFrameNo(SAnimationAccessorStandIn& self, int a2, const std::vector<int>& times,
                            int a4, int& keyFrame, int a6) {
    (void)self;
    (void)a2;
    (void)times;
    (void)a4;
    (void)keyFrame;
    (void)a6;
    return 0;
}

// ---------------------------------------------------------------------------
// glitch::gui attribute serialization
// Base class IGUIElement serialize/deserialize calls are omitted: that base is not mapped.
// ---------------------------------------------------------------------------

// 0x6a76bc _ZNK6glitch3gui12CGUICheckBox19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeCheckBox(const CGUICheckBoxStandIn& self, IAttributesStandIn& attrs) {
    attrs.addBool("Checked", self.checked);  // +0x159
}

// 0x6a7e94 _ZN6glitch3gui12CGUICheckBox21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void deserializeCheckBox(CGUICheckBoxStandIn& self, IAttributesStandIn& attrs) {
    self.checked = attrs.getBool("Checked");  // +0x159
}

// 0x6a6630 _ZNK6glitch3gui10CGUIButton19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeButton(const CGUIButtonStandIn& self, IAttributesStandIn& attrs) {
    attrs.addBool("PushButton", self.pressedMode);  // +0x159
    if (self.pressedMode) {
        attrs.addBool("Pressed", self.pressed);     // +0x158
    }
    attrs.addTexture("Image", self.image);          // +0x1b0
    attrs.addRect("ImageRect", self.imageRect);     // +0x1b8
    attrs.addTexture("PressedImage", self.pressedImage);  // +0x1b4
    attrs.addRect("PressedImageRect", self.pressedImageRect);  // +0x1c8
    attrs.addBool("Border", self.border);           // +0x15b
    attrs.addBool("UseAlphaChannel", self.useAlphaChannel);  // +0x15a
}

// 0x6a6bb0 _ZN6glitch3gui10CGUIButton21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the original calls the virtual setters (slots +0x80, +0x84, +0x88, +0x8c,
// +0xb0) for the images and the border; here they are direct assignments. The final
// virtual call on the base (slot +0x0c) is omitted.
void deserializeButton(CGUIButtonStandIn& self, IAttributesStandIn& attrs) {
    self.pressedMode = attrs.getBool("PushButton");
    self.pressed = self.pressedMode ? attrs.getBool("Pressed") : false;

    const core::rect<std::int32_t> imageRect = attrs.getRect("ImageRect");
    ITextureStandIn* image = attrs.getTexture("Image");
    // A rectangle whose lower right corner is left of or above the upper left one means
    // "no rectangle": the original then calls the overload without a rectangle.
    const bool imageRectDegenerate = imageRect.LowerRightCorner.X < imageRect.UpperLeftCorner.X ||
                                     imageRect.LowerRightCorner.Y < imageRect.UpperLeftCorner.Y;
    self.image = image;
    if (!imageRectDegenerate) {
        self.imageRect = imageRect;
    }

    const core::rect<std::int32_t> pressedRect = attrs.getRect("PressedImageRect");
    ITextureStandIn* pressedImage = attrs.getTexture("PressedImage");
    const bool pressedRectDegenerate = pressedRect.UpperLeftCorner.X > pressedRect.LowerRightCorner.X ||
                                       pressedRect.UpperLeftCorner.Y > pressedRect.LowerRightCorner.Y;
    self.pressedImage = pressedImage;
    if (!pressedRectDegenerate) {
        self.pressedImageRect = pressedRect;
    }

    self.border = attrs.getBool("Border");
    self.useAlphaChannel = attrs.getBool("UseAlphaChannel");
}

// 0x6b0d9c _ZNK6glitch3gui11CGUIEditBox19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeEditBox(const CGUIEditBoxStandIn& self, IAttributesStandIn& attrs,
                      const char* const* alignNames) {
    attrs.addBool("OverrideColorEnabled", self.overrideColorEnabled);  // +0x15a
    attrs.addColor("OverrideColor", self.overrideColor);               // +0x164
    attrs.addInt("MaxChars", self.maxChars);                           // +0x184
    attrs.addBool("WordWrap", self.wordWrap);                          // +0x188
    attrs.addBool("MultiLine", self.multiLine);                        // +0x189
    attrs.addBool("AutoScroll", self.autoScroll);                      // +0x18a
    attrs.addBool("PasswordBox", self.passwordBox);                    // +0x18b
    // The original builds a one character wide string. The stand-in has only narrow
    // strings, so the character is narrowed here.
    attrs.addString("PasswordChar", std::string(1, static_cast<char>(self.passwordChar)));  // +0x18c
    attrs.addEnum("HTextAlign", self.horizontalAlign, alignNames);     // +0x190
    attrs.addEnum("VTextAlign", self.verticalAlign, alignNames);       // +0x194
}

// 0x6b08ac _ZN6glitch3gui11CGUIEditBox21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the setters of the original (virtual slots +0x80, +0x84, +0xb4, +0x90, +0x98,
// +0xa0, +0xa8, +0xb0, +0x8c) are direct assignments here.
void deserializeEditBox(CGUIEditBoxStandIn& self, IAttributesStandIn& attrs,
                        const char* const* alignNames) {
    self.overrideColor = attrs.getColor("OverrideColor");
    self.overrideColorEnabled = attrs.getBool("OverrideColorEnabled");
    self.maxChars = attrs.getInt("MaxChars");
    self.wordWrap = attrs.getBool("WordWrap");
    self.multiLine = attrs.getBool("MultiLine");
    self.autoScroll = attrs.getBool("AutoScroll");
    // An empty password string keeps the default mask character '*' (42).
    const std::string passwordChar = attrs.getString("PasswordChar");
    self.passwordChar = passwordChar.empty() ? 42u : static_cast<std::uint8_t>(passwordChar[0]);
    self.passwordBox = attrs.getBool("PasswordBox");
    self.horizontalAlign = attrs.getEnum("HTextAlign", alignNames);
    self.verticalAlign = attrs.getEnum("VTextAlign", alignNames);
}

// 0x6ac12c _ZNK6glitch3gui12CGUIComboBox19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed. The item text slot (+0x94 call with the 68 byte item field) is
// ambiguous: the original passes the item field to a string slot. Only the attribute
// order is recorded here: HTextAlign, VTextAlign, Selected, ItemCount, then Text<i>.
void serializeComboBox(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6ac9bc _ZN6glitch3gui12CGUIComboBox21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed. Order as in the original: alignment, ItemCount items read from
// Text<i>, then Selected. Item storage is not mapped.
void deserializeComboBox(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6af2bc _ZNK6glitch3gui15CGUIContextMenu19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed. Items are 96 byte records; per item the original writes
// IsSeparator, and for non separators Text, CommandID and Enabled. The parent index is
// searched for when the parent is a menu or menu item. The record layout is not mapped.
void serializeContextMenu(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6af62c _ZN6glitch3gui15CGUIContextMenu21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed. Same attribute names as the serializer; the item records and the
// add/separator calls are not mapped.
void deserializeContextMenu(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// ---------------------------------------------------------------------------
// glitch::scene::CSceneCollisionManager
// ---------------------------------------------------------------------------

// 0x6c3bec _ZN6glitch5scene22CSceneCollisionManagerC2EPNS0_13CSceneManagerEPNS_5video12IVideoDriverE
void constructCollisionManagerC2(CollisionManagerStandIn& self, CSceneManagerStandIn* sceneManager,
                                 IVideoDriverStandIn* driver) {
    self.refCount = 1;                 // +0x04
    self.sceneManager = sceneManager;  // +0x08
    self.driver = driver;              // +0x0c
    self.reserved10 = 0;               // +0x10
    self.reserved14 = 0;               // +0x14
    self.reserved18 = 0;               // +0x18
    if (driver != nullptr) {
        ++driver->refCount;
    }
}

// 0x6c3c48 _ZN6glitch5scene22CSceneCollisionManagerC1EPNS0_13CSceneManagerEPNS_5video12IVideoDriverE
void constructCollisionManagerC1(CollisionManagerStandIn& self, CSceneManagerStandIn* sceneManager,
                                 IVideoDriverStandIn* driver) {
    constructCollisionManagerC2(self, sceneManager, driver);
}

// 0x6c42a8 _ZN6glitch5scene22CSceneCollisionManager24testTriangleIntersectionEPNS1_14SCollisionDataERKNS_4core10triangle3dIfEE
// Not reconstructed (14228 byte body): plane setup, ellipsoid root solving and the
// collision data update. Returns 0 here.
float testTriangleIntersection(CollisionManagerStandIn& self, void* collisionData,
                               const TriangleStandIn& triangle) {
    (void)self;
    (void)collisionData;
    (void)triangle;
    return 0.0f;
}

// 0x6c5d40 _ZN6glitch5scene22CSceneCollisionManager15getPickedNodeBBEPNS0_10ISceneNodeERKNS_4core6line3dIfEEibRfRS3_
// Not reconstructed (5440 byte body). Returns nullptr here.
void* getPickedNodeBB(CollisionManagerStandIn& self, void* selector, const LineStandIn& ray,
                      std::int32_t bitMask, bool useBoundingBox, float& distance) {
    (void)self;
    (void)selector;
    (void)ray;
    (void)bitMask;
    (void)useBoundingBox;
    distance = 0.0f;
    return nullptr;
}

// 0x6c72f4 _ZN6glitch5scene22CSceneCollisionManager25collideEllipsoidWithWorldEPNS0_17ITriangleSelectorERKNS_4core8vector3dIfEES8_S8_fS8_RNS4_10triangle3dIfEERb
// Not reconstructed (5733 byte body). Returns the position unchanged here.
core::vector3d<float> collideEllipsoidWithWorld(CollisionManagerStandIn& self, void* selector,
                                               const core::vector3d<float>& radius,
                                               const core::vector3d<float>& position,
                                               const core::vector3d<float>& velocity, float slidingSpeed,
                                               const core::vector3d<float>& gravity,
                                               TriangleStandIn& triangle, bool& hit) {
    (void)self;
    (void)selector;
    (void)radius;
    (void)velocity;
    (void)slidingSpeed;
    (void)gravity;
    (void)triangle;
    hit = false;
    return position;
}

// 0x6c7720 _ZN6glitch5scene22CSceneCollisionManager26getCollisionResultPositionEPNS0_17ITriangleSelectorERKNS_4core8vector3dIfEES8_S8_RNS4_10triangle3dIfEERbfS8_
// Without a selector or with a zero radius component the source position is returned;
// otherwise the ellipsoid collision is used. The collision call is the stub above.
core::vector3d<float> getCollisionResultPosition(CollisionManagerStandIn& self, void* selector,
                                                 const core::vector3d<float>& radius,
                                                 const core::vector3d<float>& position,
                                                 const core::vector3d<float>& velocity,
                                                 TriangleStandIn& triangle, bool& hit,
                                                 float slidingSpeed,
                                                 const core::vector3d<float>& gravity) {
    if (selector == nullptr || radius.X == 0.0f || radius.Y == 0.0f || radius.Z == 0.0f) {
        return position;
    }
    return collideEllipsoidWithWorld(self, selector, radius, position, velocity, slidingSpeed,
                                     gravity, triangle, hit);
}

// ---------------------------------------------------------------------------
// glitch::scene animators
// ---------------------------------------------------------------------------

// 0x6ca764 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeCollisionResponse(const CollisionResponseAnimatorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addVector3d("Radius", self.radius);                 // +0x18
    attrs.addVector3d("Gravity", self.gravity);               // +0x24
    attrs.addVector3d("Translation", self.translation);       // +0x30
    attrs.addBool("AnimateCameraTarget", self.animateCameraTarget);  // +0x56
}

// 0x6ca804 _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The first vector is read under the name "Radius": the pseudocode omits that argument,
// and the name is taken from the serializer order.
void deserializeCollisionResponse(CollisionResponseAnimatorStandIn& self, IAttributesStandIn& attrs) {
    self.radius = attrs.getVector3d("Radius");
    self.gravity = attrs.getVector3d("Gravity");
    self.translation = attrs.getVector3d("Translation");
    self.animateCameraTarget = attrs.getBool("AnimateCameraTarget");
}

// 0x6cb738 _ZNK6glitch5scene27CSceneNodeAnimatorFlyCircle19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeFlyCircle(const FlyCircleAnimatorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addVector3d("Center", self.center);       // +0x0c
    attrs.addFloat("Radius", self.radius);          // +0x3c
    attrs.addFloat("Speed", self.speed);            // +0x40
    attrs.addVector3d("Direction", self.direction); // +0x18
}

// 0x6cba9c _ZN6glitch5scene27CSceneNodeAnimatorFlyCircle21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The final init() call of the original is omitted (not reconstructed).
void deserializeFlyCircle(FlyCircleAnimatorStandIn& self, IAttributesStandIn& attrs) {
    self.center = attrs.getVector3d("Center");
    self.radius = attrs.getFloat("Radius");
    self.speed = attrs.getFloat("Speed");
    self.direction = attrs.getVector3d("Direction");
    self.state44 = 0;  // +0x44

    const core::vector3d<float>& d = self.direction;
    const bool nonZero = std::fabs(d.X) > 0.000001f || std::fabs(d.Y) > 0.000001f ||
                         std::fabs(d.Z) > 0.000001f;
    if (nonZero) {
        const float length = std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
        self.direction.X = d.X / length;
        self.direction.Y = d.Y / length;
        self.direction.Z = d.Z / length;
    } else {
        self.direction = core::vector3d<float>{0.0f, 1.0f, 0.0f};
    }
}

// 0x6cc078 _ZNK6glitch5scene29CSceneNodeAnimatorFlyStraight19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeFlyStraight(const FlyStraightAnimatorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addVector3d("Start", self.start);         // +0x0c
    attrs.addVector3d("End", self.end);             // +0x18
    attrs.addInt("TimeForWay", self.timeForWay);    // +0x3c
    attrs.addBool("Loop", self.loop);               // +0x40
}

// 0x6cc3ec _ZN6glitch5scene29CSceneNodeAnimatorFlyStraight21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The recalculateImidiateValues() call of the original is omitted (not reconstructed).
void deserializeFlyStraight(FlyStraightAnimatorStandIn& self, IAttributesStandIn& attrs) {
    self.start = attrs.getVector3d("Start");
    self.end = attrs.getVector3d("End");
    self.timeForWay = attrs.getInt("TimeForWay");
    self.loop = attrs.getBool("Loop");
}

// 0x6cd678 _ZNK6glitch5scene26CSceneNodeAnimatorRotation19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeRotation(const RotationAnimatorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addQuaternion("Rotation", self.rotation);  // +0x0c..+0x18
}

// 0x6cd6c8 _ZN6glitch5scene26CSceneNodeAnimatorRotation21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void deserializeRotation(RotationAnimatorStandIn& self, IAttributesStandIn& attrs) {
    attrs.getQuaternion(self.rotation, "Rotation");
}

// 0x6ccef8 _ZNK6glitch5scene30CSceneNodeAnimatorFollowSpline19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed. The points are written under one character keys built from the
// running index (the original builds std::string of a single char). Recorded here:
// Speed and Tightness as floats, one point per entry of the 12 byte vector array, plus
// one extra zero point when the options word has bit 1 set.
void serializeFollowSpline(void* self, IAttributesStandIn& attrs, const std::uint32_t* options) {
    (void)self;
    (void)attrs;
    (void)options;
}

// 0x6cd0cc _ZN6glitch5scene30CSceneNodeAnimatorFollowSpline21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed (5771 byte body).
void deserializeFollowSpline(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6cdee4 _ZNK6glitch5scene25CSceneNodeAnimatorTexture19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the texture list is written under one character keys (index + 1), as in the
// original. The option bit 1 adds one extra entry.
void serializeTextureAnimator(void* self, IAttributesStandIn& attrs, const std::uint32_t* options) {
    (void)self;
    (void)attrs;
    (void)options;
}

// 0x6ce1f8 _ZN6glitch5scene25CSceneNodeAnimatorTexture21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed.
void deserializeTextureAnimator(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6cdcd8 _ZN6glitch5scene25CSceneNodeAnimatorTextureC1ERKSt6vectorIN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core10SAllocatorIS7_LNS_6memory13E_MEMORY_HINTE0EEEEibj
// Not reconstructed: the base object setup (vtables and the IObject base) is omitted.
void* constructTextureAnimatorC1(void* self, const std::vector<ITextureStandIn*>& textures,
                                 std::int32_t timePerFrame, bool loop, std::uint32_t startTime) {
    (void)textures;
    (void)timePerFrame;
    (void)loop;
    (void)startTime;
    return self;
}

// 0x6ce084 _ZN6glitch5scene25CSceneNodeAnimatorTextureC2ERKSt6vectorIN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core10SAllocatorIS7_LNS_6memory13E_MEMORY_HINTE0EEEEibj
// Not reconstructed: the base object setup is omitted.
void* constructTextureAnimatorC2(void* self, void* base, const std::vector<ITextureStandIn*>& textures,
                                 std::int32_t timePerFrame, bool loop, std::uint32_t startTime) {
    (void)base;
    (void)textures;
    (void)timePerFrame;
    (void)loop;
    (void)startTime;
    return self;
}

// ---------------------------------------------------------------------------
// glitch::scene skybox, terrain, text, shadow volume
// ---------------------------------------------------------------------------

// 0x6cebc8 _ZN6glitch5scene16CSkyBoxSceneNodeC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS2_8ITextureEEESA_SA_SA_SA_SA_i
// Not reconstructed (14012 byte body).
void constructSkyBoxC2(void* self, void* driver) {
    (void)self;
    (void)driver;
}

// 0x6cf564 _ZN6glitch5scene16CSkyBoxSceneNodeC1EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS2_8ITextureEEESA_SA_SA_SA_SA_i
// Not reconstructed (14086 byte body).
void constructSkyBoxC1(void* self, void* driver) {
    (void)self;
    (void)driver;
}

// 0x6d042c _ZN6glitch5scene17CTerrainSceneNode22setCurrentLODOfPatchesERKSt6vectorIiNS_4core10SAllocatorIiLNS_6memory13E_MEMORY_HINTE0EEEE
// The patch count is the square of the patch dimension; each patch takes one LOD value.
// The stand-in keeps the patch table as a vector, so the loop is also bounded by its
// size and by the size of the input; the original trusts the table to be large enough.
void setCurrentLODOfPatches(TerrainStandIn& self, const std::vector<std::int32_t>& lods) {
    const std::int64_t dim = self.patchDimension;
    const std::int64_t count = dim * dim;
    if (count <= 0) {
        return;
    }
    const std::size_t n = std::min({static_cast<std::size_t>(count), self.patches.size(), lods.size()});
    for (std::size_t i = 0; i != n; ++i) {
        self.patches[i].lod = lods[i];
    }
}

// 0x6d083c _ZNK6glitch5scene17CTerrainSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Base ISceneNode serialization is omitted (not mapped).
void serializeTerrain(void* self, IAttributesStandIn& attrs, const std::uint32_t* options) {
    (void)self;
    (void)attrs;
    (void)options;
}

// 0x6d5268 _ZN6glitch5scene17CTerrainSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed.
void deserializeTerrain(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6d21ac _ZNK6glitch5scene17CTerrainSceneNode22getCurrentLODOfPatchesERSt6vectorIiNS_4core10SAllocatorIiLNS_6memory13E_MEMORY_HINTE0EEEE
// Not reconstructed (2033 byte body).
void getCurrentLODOfPatches(void* self, std::vector<std::int32_t>& lods) {
    (void)self;
    (void)lods;
}

// 0x6d4454 _ZN6glitch5scene17CTerrainSceneNode13loadHeightMapEPNS_2io9IReadFileENS_5video6SColorEi
// Not reconstructed (17835 byte body).
void loadHeightMap(void* self, void* readFile, std::uint32_t colorKey, std::int32_t mode) {
    (void)self;
    (void)readFile;
    (void)colorKey;
    (void)mode;
}

// 0x6d5a14 _ZNK6glitch5scene23CBillboardTextSceneNode8getColorERNS_5video6SColorES4_
// Not reconstructed. The original copies the 4 byte colors at +0x1a8 and +0x1ac into the
// two outputs (memcpy). The stand-in does not carry those fields, so the outputs are
// zeroed here and the return value is the first color.
std::uint32_t getBillboardTextColors(const void* self, std::uint32_t& top, std::uint32_t& bottom) {
    (void)self;
    top = 0;
    bottom = 0;
    return top;
}

// 0x6d5b5c _ZN6glitch5scene14ITextSceneNodeC2EiRKNS_4core8vector3dIfEE
// Not reconstructed: the ISceneNode base constructor and the vtable writes are omitted.
void constructTextSceneNode(void* self, void* parent, std::int32_t id, const core::vector3d<float>& pos) {
    (void)self;
    (void)parent;
    (void)id;
    (void)pos;
}

// 0x6ffb1c _ZN6glitch5scene22CShadowVolumeSceneNodeC1EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrIKNS0_5IMeshEEEiNS0_20E_SHADOW_VOLUME_TYPEEf
// Not reconstructed (2001 byte body).
void constructShadowVolumeC1(void* self, void* driver, void* mesh, std::int32_t id, int type, float f) {
    (void)self;
    (void)driver;
    (void)mesh;
    (void)id;
    (void)type;
    (void)f;
}

// 0x6ffc70 _ZN6glitch5scene22CShadowVolumeSceneNodeC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrIKNS0_5IMeshEEEiNS0_20E_SHADOW_VOLUME_TYPEEf
// Not reconstructed (1930 byte body).
void constructShadowVolumeC2(void* self, void* driver, void* mesh, std::int32_t id, int type, float f) {
    (void)self;
    (void)driver;
    (void)mesh;
    (void)id;
    (void)type;
    (void)f;
}

// 0x701358 _ZN6glitch5scene22CShadowVolumeSceneNode11SSilhouette16createSilhouetteENS_4core8vector3dIfEENS_5video12E_LIGHT_TYPEERKN5boost13intrusive_ptrIKNS0_11CMeshBufferEEE
// Not reconstructed (8712 byte body).
void createSilhouette(void* self, const core::vector3d<float>& light, int lightType, void* mesh) {
    (void)self;
    (void)light;
    (void)lightType;
    (void)mesh;
}

// ---------------------------------------------------------------------------
// glitch::collada coronas and glitch::scene animated mesh
// ---------------------------------------------------------------------------

// 0x6e5a90 _ZNK6glitch7collada17CCoronasSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeCoronas(const CoronasSceneNodeStandIn& self, IAttributesStandIn& attrs) {
    attrs.addFloat("Width", self.width);     // +0x148
    attrs.addFloat("Height", self.height);   // +0x14c
    attrs.addColor("VColor", self.vColor);   // +0x18c
}

// 0x6e5b0c _ZN6glitch7collada17CCoronasSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The final virtual call (slot +0xf4 on the node, with the size) is omitted.
void deserializeCoronas(CoronasSceneNodeStandIn& self, IAttributesStandIn& attrs) {
    self.width = attrs.getFloat("Width");
    self.height = attrs.getFloat("Height");
    self.vColor = attrs.getColor("VColor");
}

// 0x6f6478 _ZNK6glitch5scene22CAnimatedMeshSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The mesh name is passed in by the caller; the original resolves it through the scene
// manager chain. The base ISceneNode serialization is omitted.
void serializeAnimatedMesh(const AnimatedMeshStandIn& self, const std::string& meshName,
                           IAttributesStandIn& attrs) {
    attrs.addString("Mesh", meshName);                     // name from the mesh object
    attrs.addBool("Looping", self.looping);                // +0x154
    attrs.addInt("FramesPerSecond", self.framesPerSecond); // +0x140
}

// 0x6f6d98 _ZN6glitch5scene22CAnimatedMeshSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the mesh lookup and the setMesh virtual call (slot +0x118 on the node) are
// omitted. Looping and FramesPerSecond are read.
void deserializeAnimatedMesh(AnimatedMeshStandIn& self, IAttributesStandIn& attrs) {
    (void)attrs.getString("Mesh");
    self.looping = attrs.getBool("Looping");
    self.framesPerSecond = attrs.getInt("FramesPerSecond");
}

// ---------------------------------------------------------------------------
// glitch::scene particle system node, emitters and affectors
// ---------------------------------------------------------------------------

// 0x6c1554 _ZNK6glitch5scene24CParticleSystemSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed: the emitter and affector type enums depend on the emitter vtable
// and the type name tables (ParticleEmitterTypeNames, ParticleAffectorTypeNames).
void serializeParticleSystemNode(void* self, IAttributesStandIn& attrs, const std::uint32_t* options) {
    (void)self;
    (void)attrs;
    (void)options;
}

// 0x6c1790 _ZN6glitch5scene24CParticleSystemSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed (6187 byte body).
void deserializeParticleSystemNode(void* self, IAttributesStandIn& attrs) {
    (void)self;
    (void)attrs;
}

// 0x6c23f4 _ZN6glitch5scene24IParticleSystemSceneNodeC2EiRKNS_4core8vector3dIfEES6_S6_
// The rotation vector is converted to a quaternion; the base ISceneNode constructor and
// the vtable writes are omitted.
void constructIParticleSystemSceneNode(void* self, void* parent, std::int32_t id,
                                       const core::vector3d<float>& rotation) {
    (void)self;
    (void)parent;
    (void)id;
    (void)rotation;
}

// 0x6f76d0 _ZNK6glitch5scene16IParticleEmitter19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void serializeParticleEmitterBase() {}

// 0x6f76e4 _ZN6glitch5scene16IParticleEmitter21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original: returns 0 and leaves the index alone.
std::int32_t deserializeParticleEmitterBase(IAttributesStandIn& attrs, std::int32_t index) {
    (void)attrs;
    (void)index;
    return 0;
}

// 0x6f837c _ZNK6glitch5scene17IParticleAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void serializeParticleAffectorBase() {}

// 0x6f8390 _ZN6glitch5scene17IParticleAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original: returns 0 and leaves the index alone.
std::int32_t deserializeParticleAffectorBase(IAttributesStandIn& attrs, std::int32_t index) {
    (void)attrs;
    (void)index;
    return 0;
}

// 0x6f8d5c _ZNK6glitch5scene19CParticleBoxEmitter19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// The box attribute is the half extent of the box: (max - min) * 0.5 per axis.
void serializeBoxEmitter(const ParticleBoxEmitterStandIn& self, IAttributesStandIn& attrs) {
    const core::vector3d<float> halfExtent{
        (self.maxEdge.X - self.minEdge.X) * 0.5f,
        (self.maxEdge.Y - self.minEdge.Y) * 0.5f,
        (self.maxEdge.Z - self.minEdge.Z) * 0.5f,
    };
    attrs.addVector3d("Box", halfExtent);
    attrs.addVector3d("Direction", self.direction);                              // +0x28
    attrs.addInt("MinParticlesPerSecond", self.minParticlesPerSecond);           // +0x34
    attrs.addInt("MaxParticlesPerSecond", self.maxParticlesPerSecond);           // +0x38
    attrs.addColor("MinStartColor", self.minStartColor);                         // +0x3c
    attrs.addColor("MaxStartColor", self.maxStartColor);                         // +0x40
    attrs.addInt("MinLifeTime", self.minLifeTime);                               // +0x44
    attrs.addInt("MaxLifeTime", self.maxLifeTime);                               // +0x48
    attrs.addInt("MaxAngleDegrees", self.maxAngleDegrees);                       // +0x54
}

// 0x6f8fd4 _ZN6glitch5scene19CParticleBoxEmitter21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed (3463 byte body).
void deserializeBoxEmitter(ParticleBoxEmitterStandIn& self, std::int32_t index, IAttributesStandIn& attrs) {
    (void)self;
    (void)index;
    (void)attrs;
}

// 0x6fbcd8 _ZNK6glitch5scene21CParticlePointEmitter19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializePointEmitter(const ParticlePointEmitterStandIn& self, IAttributesStandIn& attrs) {
    attrs.addVector3d("Direction", self.direction);                          // +0x48
    attrs.addInt("MinParticlesPerSecond", self.minParticlesPerSecond);       // +0x54
    attrs.addInt("MaxParticlesPerSecond", self.maxParticlesPerSecond);       // +0x58
    attrs.addColor("MinStartColor", self.minStartColor);                     // +0x5c
    attrs.addColor("MaxStartColor", self.maxStartColor);                     // +0x60
    attrs.addInt("MinLifeTime", self.minLifeTime);                           // +0x64
    attrs.addInt("MaxLifeTime", self.maxLifeTime);                           // +0x68
    attrs.addInt("MaxAngleDegrees", self.maxAngleDegrees);                   // +0x6c
}

// 0x6fbe4c _ZN6glitch5scene21CParticlePointEmitter21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Not reconstructed (2739 byte body). Recorded: the direction is normalized (a zero
// vector becomes the default), the per second counts are clamped to 1..200, and the
// final call on the attribute object is omitted.
void deserializePointEmitter(ParticlePointEmitterStandIn& self, std::int32_t index, IAttributesStandIn& attrs) {
    (void)self;
    (void)index;
    (void)attrs;
}

// 0x6fa750 _ZNK6glitch5scene24CParticleFadeOutAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeFadeOutAffector(const ParticleFadeOutAffectorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addColor("TargetColor", self.targetColor);   // +0x05
    attrs.addFloat("FadeOutTime", self.fadeOutTime);   // +0x0c
}

// 0x6fa804 _ZN6glitch5scene24CParticleFadeOutAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
std::int32_t deserializeFadeOutAffector(ParticleFadeOutAffectorStandIn& self, std::int32_t index,
                                        IAttributesStandIn& attrs) {
    if (!nameIs(attrs, index, "TargetColor")) {
        return index;
    }
    self.targetColor = attrs.colorAt(index);
    ++index;
    if (!nameIs(attrs, index, "FadeOutTime")) {
        return index;
    }
    self.fadeOutTime = attrs.floatAt(index);
    return index + 1;
}

// 0x6fad3c _ZNK6glitch5scene24CParticleGravityAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeGravityAffector(const ParticleGravityAffectorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addVector3d("Gravity", self.gravity);            // +0x0c
    attrs.addFloat("TimeForceLost", self.timeForceLost);   // +0x08
}

// 0x6fadd8 _ZN6glitch5scene24CParticleGravityAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
std::int32_t deserializeGravityAffector(ParticleGravityAffectorStandIn& self, std::int32_t index,
                                        IAttributesStandIn& attrs) {
    if (!nameIs(attrs, index, "Gravity")) {
        return index;
    }
    self.gravity = attrs.vector3dAt(index);
    ++index;
    if (!nameIs(attrs, index, "TimeForceLost")) {
        return index;
    }
    self.timeForceLost = attrs.floatAt(index);
    return index + 1;
}

// 0x6fd400 _ZNK6glitch5scene21CParticleSizeAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeSizeAffector(const ParticleSizeAffectorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addFloat("TargetSize", self.targetSize);       // +0x08
    attrs.addFloat("Variation", self.variation);         // +0x0c
    attrs.addFloat("GrowForTime", self.growForTime);     // +0x10
    attrs.addFloat("FadeForTime", self.fadeForTime);     // +0x14
}

// 0x6fd4e4 _ZN6glitch5scene21CParticleSizeAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
std::int32_t deserializeSizeAffector(ParticleSizeAffectorStandIn& self, std::int32_t index,
                                     IAttributesStandIn& attrs) {
    if (!nameIs(attrs, index, "TargetSize")) {
        return index;
    }
    self.targetSize = attrs.floatAt(index);
    ++index;
    if (!nameIs(attrs, index, "Variation")) {
        return index;
    }
    self.variation = attrs.floatAt(index);
    ++index;
    if (!nameIs(attrs, index, "GrowForTime")) {
        return index;
    }
    self.growForTime = attrs.floatAt(index);
    ++index;
    if (!nameIs(attrs, index, "FadeForTime")) {
        return index;
    }
    self.fadeForTime = attrs.floatAt(index);
    return index + 1;
}

// 0x6fe498 _ZNK6glitch5scene21CParticleSpinAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void serializeSpinAffector(const ParticleSpinAffectorStandIn& self, IAttributesStandIn& attrs) {
    attrs.addFloat("SpinTime", static_cast<float>(self.spinTime));  // +0x08 (unsigned to float)
    attrs.addFloat("Variation", self.variation);                    // +0x0c
}

// 0x6fe540 _ZN6glitch5scene21CParticleSpinAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
std::int32_t deserializeSpinAffector(ParticleSpinAffectorStandIn& self, std::int32_t index,
                                     IAttributesStandIn& attrs) {
    if (!nameIs(attrs, index, "SpinTime")) {
        return index;
    }
    self.spinTime = static_cast<std::uint32_t>(attrs.floatAt(index));
    ++index;
    if (!nameIs(attrs, index, "Variation")) {
        return index;
    }
    self.variation = attrs.floatAt(index);
    return index + 1;
}

}  // namespace glitch::collada::recon_33
