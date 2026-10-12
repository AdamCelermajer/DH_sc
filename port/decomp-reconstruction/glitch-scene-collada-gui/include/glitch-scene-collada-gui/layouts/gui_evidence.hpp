#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_GUI_EVIDENCE_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_GUI_EVIDENCE_HPP

// Evidence layouts (generated from Hex-Rays bodies in recovered/native/hexrays-all).
// Member offsets are from the start of each object (including base-class bytes).
// Each named member cites the body that proves its width and use. Ctor/dtor first, then accessors.
// Accessor names come from one-line setter/getter pairs that read or write the same byte.
// Pads are gaps no cited body touches. Not confirmed.
// Vtable slot numbers are byte offset / 4 (e.g. +0x88 is slot 34). Slot calls are noted in the member comments.

#include <cstddef>
#include <cstdint>

namespace glitch::gui::evidence {

// CGUIButton (ctor 0x6a63fc, 0x6a653c; dtor 0x6a6a2c). Setters 0x6a5df0..0x6a6958.
struct CGUIButton_ev {
  std::uint32_t vptr; // +0x00 width=4 src=0x6a63fc primary vptr (ctor writes off_985C68)
  std::uint8_t pad_0x04[151]; // not confirmed
  std::uint8_t ctor_bool_0x9b; // +0x9B width=1 src=0x6a63fc ctor arg stored; Irrlicht NoClip not checked
  std::uint8_t pad_0x9C[152]; // not confirmed
  std::uint8_t flag_0x134; // +0x134 width=1 src=0x6a63fc ctor writes 1, no accessor seen
  std::uint8_t pad_0x135[35]; // not confirmed
  std::uint8_t pressed; // +0x158 width=1 src=0x6a5f14 read by isPressed 6a5ecc; setPressed writes arg
  std::uint8_t push_button; // +0x159 width=1 src=0x6a5ec4 read by isPushButton 6a5ed4
  std::uint8_t use_alpha; // +0x15A width=1 src=0x6a5edc read by isAlphaChannelUsed 6a5ee4
  std::uint8_t draw_border; // +0x15B width=1 src=0x6a5df0 read by isDrawingBorder 6a5eec; ctor writes 1
  std::uint32_t pressed_time; // +0x15C width=4 src=0x6a5f14 setPressed stores os::Timer::getTime() result
  std::uint32_t sprite_bank; // +0x160 width=4 src=0x6a5df8 setSpriteBank stores ref; dtor 6a6a2c drops it
  std::uint32_t override_font; // +0x164 width=4 src=0x6a5e90 setOverrideFont stores ref; dtor 6a6a2c drops it
  std::int32_t i32_0x168; // +0x168 width=4 src=0x6a63fc ctor writes -1 (no accessor seen)
  std::uint8_t pad_0x16C[8]; // not confirmed
  std::int32_t i32_0x174; // +0x174 width=4 src=0x6a63fc ctor writes -1
  std::uint8_t pad_0x178[8]; // not confirmed
  std::int32_t i32_0x180; // +0x180 width=4 src=0x6a63fc ctor writes -1
  std::uint8_t pad_0x184[8]; // not confirmed
  std::int32_t i32_0x18c; // +0x18C width=4 src=0x6a63fc ctor writes -1
  std::uint8_t pad_0x190[8]; // not confirmed
  std::int32_t i32_0x198; // +0x198 width=4 src=0x6a63fc ctor writes -1
  std::uint8_t pad_0x19C[8]; // not confirmed
  std::int32_t i32_0x1a4; // +0x1A4 width=4 src=0x6a63fc ctor writes -1
  std::uint8_t pad_0x1A8[8]; // not confirmed
  std::uint32_t image; // +0x1B0 width=4 src=0x6a68d4 setImage stores ref; dtor 6a6a2c drops +0x1B0; vslot #34 (+0x88) called on this when no pressed image
  std::uint32_t pressed_image; // +0x1B4 width=4 src=0x6a6958 setPressedImage stores ref; dtor 6a6a2c drops +0x1B4
  std::uint32_t u32_0x1b8; // +0x1B8 width=4 src=0x6a68d4 setImage zeroes it
  std::uint32_t u32_0x1bc; // +0x1BC width=4 src=0x6a68d4 setImage zeroes it
  std::uint32_t image_tex_0x20; // +0x1C0 width=4 src=0x6a68d4 copies texture word at +0x20 (src ITexture, not in this body)
  std::uint32_t image_tex_0x24; // +0x1C4 width=4 src=0x6a68d4 copies texture word at +0x24
  std::uint32_t pressed_zero_0x1c8; // +0x1C8 width=4 src=0x6a6958 setPressedImage zeroes it
  std::uint32_t pressed_zero_0x1cc; // +0x1CC width=4 src=0x6a6958 setPressedImage zeroes it
  std::uint32_t pressed_tex_0x20; // +0x1D0 width=4 src=0x6a6958 copies texture word at +0x20
  std::uint32_t pressed_tex_0x24; // +0x1D4 width=4 src=0x6a6958 copies texture word at +0x24
};
static_assert(offsetof(CGUIButton_ev, vptr) == 0x00, "vptr");
static_assert(offsetof(CGUIButton_ev, ctor_bool_0x9b) == 0x9B, "ctor_bool_0x9b");
static_assert(offsetof(CGUIButton_ev, flag_0x134) == 0x134, "flag_0x134");
static_assert(offsetof(CGUIButton_ev, pressed) == 0x158, "pressed");
static_assert(offsetof(CGUIButton_ev, push_button) == 0x159, "push_button");
static_assert(offsetof(CGUIButton_ev, use_alpha) == 0x15A, "use_alpha");
static_assert(offsetof(CGUIButton_ev, draw_border) == 0x15B, "draw_border");
static_assert(offsetof(CGUIButton_ev, pressed_time) == 0x15C, "pressed_time");
static_assert(offsetof(CGUIButton_ev, sprite_bank) == 0x160, "sprite_bank");
static_assert(offsetof(CGUIButton_ev, override_font) == 0x164, "override_font");
static_assert(offsetof(CGUIButton_ev, i32_0x168) == 0x168, "i32_0x168");
static_assert(offsetof(CGUIButton_ev, i32_0x174) == 0x174, "i32_0x174");
static_assert(offsetof(CGUIButton_ev, i32_0x180) == 0x180, "i32_0x180");
static_assert(offsetof(CGUIButton_ev, i32_0x18c) == 0x18C, "i32_0x18c");
static_assert(offsetof(CGUIButton_ev, i32_0x198) == 0x198, "i32_0x198");
static_assert(offsetof(CGUIButton_ev, i32_0x1a4) == 0x1A4, "i32_0x1a4");
static_assert(offsetof(CGUIButton_ev, image) == 0x1B0, "image");
static_assert(offsetof(CGUIButton_ev, pressed_image) == 0x1B4, "pressed_image");
static_assert(offsetof(CGUIButton_ev, u32_0x1b8) == 0x1B8, "u32_0x1b8");
static_assert(offsetof(CGUIButton_ev, u32_0x1bc) == 0x1BC, "u32_0x1bc");
static_assert(offsetof(CGUIButton_ev, image_tex_0x20) == 0x1C0, "image_tex_0x20");
static_assert(offsetof(CGUIButton_ev, image_tex_0x24) == 0x1C4, "image_tex_0x24");
static_assert(offsetof(CGUIButton_ev, pressed_zero_0x1c8) == 0x1C8, "pressed_zero_0x1c8");
static_assert(offsetof(CGUIButton_ev, pressed_zero_0x1cc) == 0x1CC, "pressed_zero_0x1cc");
static_assert(offsetof(CGUIButton_ev, pressed_tex_0x20) == 0x1D0, "pressed_tex_0x20");
static_assert(offsetof(CGUIButton_ev, pressed_tex_0x24) == 0x1D4, "pressed_tex_0x24");

// CGUICheckBox (ctor 0x6a7bd4, 0x6a7cb0; dtor 0x6a7dc8). Accessors 0x6a76ac, 0x6a76b4.
struct CGUICheckBox_ev {
  std::uint32_t vptr; // +0x00 width=4 src=0x6a7bd4 primary vptr
  std::uint8_t pad_0x04[304]; // not confirmed
  std::uint8_t flag_0x134; // +0x134 width=1 src=0x6a7bd4 ctor writes 1
  std::uint8_t pad_0x135[35]; // not confirmed
  std::uint8_t u8_0x158; // +0x158 width=1 src=0x6a7bd4 ctor writes 0
  std::uint8_t checked; // +0x159 width=1 src=0x6a76ac setChecked writes arg 6a76ac; isChecked reads 6a76b4; ctor writes ctor bool arg a2 (6a7bd4)
  std::uint8_t pad_0x15A[2]; // not confirmed
  std::uint32_t u32_0x15c; // +0x15C width=4 src=0x6a7bd4 ctor writes 0
};
static_assert(offsetof(CGUICheckBox_ev, vptr) == 0x00, "vptr");
static_assert(offsetof(CGUICheckBox_ev, flag_0x134) == 0x134, "flag_0x134");
static_assert(offsetof(CGUICheckBox_ev, u8_0x158) == 0x158, "u8_0x158");
static_assert(offsetof(CGUICheckBox_ev, checked) == 0x159, "checked");
static_assert(offsetof(CGUICheckBox_ev, u32_0x15c) == 0x15C, "u32_0x15c");

// CGUIComboBox (ctor 0x6ab38c, 0x6ab740; dtor 0x6acc30). Accessors 0x6aaa88..0x6ac7a4.
struct CGUIComboBox_ev {
  std::uint32_t vptr; // +0x00 width=4 src=0x6ab38c primary vptr
  std::uint8_t pad_0x04[151]; // not confirmed
  std::uint8_t flag_0x9b; // +0x9B width=1 src=0x6ab38c ctor writes 1
  std::uint8_t pad_0x9C[188]; // not confirmed
  std::uint32_t ptr_or_float_0x158; // +0x158 width=4 src=0x6ab38c ctor stores vslot #30 (+0x78) result as float, then reads the same slot as an object (+0x134 write); not resolved
  std::uint32_t child; // +0x15C width=4 src=0x6ab38c child created in ctor; setAlignment and setTabOrder on it; setSelected 6aab1c calls vslot #17 (+0x44) on it
  std::uint32_t u32_0x160; // +0x160 width=4 src=0x6ab38c ctor writes 0
  std::uint32_t items_begin; // +0x164 width=4 src=0x6aaa88 getItemCount reads +0x164/+0x168; dtor 6acc30 destroys vector at +0x164
  std::uint32_t items_end; // +0x168 width=4 src=0x6aaa88 getItemCount reads
  std::uint32_t items_cap; // +0x16C width=4 src=0x6ab38c ctor writes 0
  std::int32_t selected; // +0x170 width=4 src=0x6aab1c setSelected writes (6aab1c), getSelected reads (6aab14); ctor writes -1
  std::uint8_t u8_0x174; // +0x174 width=1 src=0x6ab38c ctor writes 0
  std::uint8_t pad_0x175[3]; // not confirmed
  std::uint32_t u32_0x178; // +0x178 width=4 src=0x6ab38c ctor writes 0
  std::uint32_t u32_0x17c; // +0x17C width=4 src=0x6ab38c ctor writes 0
  std::uint32_t u32_0x180; // +0x180 width=4 src=0x6ab38c ctor writes 2
};
static_assert(offsetof(CGUIComboBox_ev, vptr) == 0x00, "vptr");
static_assert(offsetof(CGUIComboBox_ev, flag_0x9b) == 0x9B, "flag_0x9b");
static_assert(offsetof(CGUIComboBox_ev, ptr_or_float_0x158) == 0x158, "ptr_or_float_0x158");
static_assert(offsetof(CGUIComboBox_ev, child) == 0x15C, "child");
static_assert(offsetof(CGUIComboBox_ev, u32_0x160) == 0x160, "u32_0x160");
static_assert(offsetof(CGUIComboBox_ev, items_begin) == 0x164, "items_begin");
static_assert(offsetof(CGUIComboBox_ev, items_end) == 0x168, "items_end");
static_assert(offsetof(CGUIComboBox_ev, items_cap) == 0x16C, "items_cap");
static_assert(offsetof(CGUIComboBox_ev, selected) == 0x170, "selected");
static_assert(offsetof(CGUIComboBox_ev, u8_0x174) == 0x174, "u8_0x174");
static_assert(offsetof(CGUIComboBox_ev, u32_0x178) == 0x178, "u32_0x178");
static_assert(offsetof(CGUIComboBox_ev, u32_0x17c) == 0x17C, "u32_0x17c");
static_assert(offsetof(CGUIComboBox_ev, u32_0x180) == 0x180, "u32_0x180");

// CGUIComboBox item (stride 72). Only the text pointer seen in setSelected 0x6aab1c.
struct CGUIComboBoxItem_ev {
  std::uint8_t pad_0x00[68]; // not confirmed
  std::uint32_t text_cstr; // +0x44 width=4 src=0x6aab1c passed to vslot #17 (+0x44) in setSelected; item stride 72 from getItemCount 6aaa88
};
static_assert(offsetof(CGUIComboBoxItem_ev, text_cstr) == 0x44, "text_cstr");

// CGUIContextMenu (ctor 0x6adfb8, 0x6ae100; dtor 0x6ae858, 0x6ae990). Accessors 0x6acdac..0x6adc3c.
struct CGUIContextMenu_ev {
  std::uint32_t vptr; // +0x00 width=4 src=0x6adfb8 primary vptr
  std::uint8_t pad_0x04[148]; // not confirmed
  std::uint8_t flag_0x98; // +0x98 width=1 src=0x6adc3c setVisible writes arg
  std::uint8_t pad_0x99[2]; // not confirmed
  std::uint8_t flag_0x9b; // +0x9B width=1 src=0x6adfb8 ctor writes 1
  std::uint8_t pad_0x9C[188]; // not confirmed
  std::int32_t selected_item; // +0x158 width=4 src=0x6ad4d4 getSelectedItem reads; ctor writes -1
  std::uint32_t items_begin; // +0x15C width=4 src=0x6acdac getItemCount reads +0x15C/+0x160; dtor 6ae858 destroys vector at +0x15C; stride 96
  std::uint32_t items_end; // +0x160 width=4 src=0x6acdac getItemCount reads
  std::uint32_t items_cap; // +0x164 width=4 src=0x6adfb8 ctor writes 0
  std::int32_t pos_x; // +0x168 width=4 src=0x6adfb8 ctor copies rect arg word 0 (a5[0])
  std::int32_t pos_y; // +0x16C width=4 src=0x6adfb8 ctor copies rect arg word 1 (a5[1])
  std::uint32_t timestamp; // +0x170 width=4 src=0x6adc3c setVisible stores os::Timer::getTime() result
  std::uint32_t event_parent; // +0x174 width=4 src=0x6ad58c setEventParent writes arg
  std::uint8_t ctor_flag_0x178; // +0x178 width=1 src=0x6adfb8 ctor writes 7th arg (a7); 6ae100 writes its 8th arg
  std::uint8_t pad_0x179[3]; // not confirmed
  std::uint32_t ref_0x17c; // +0x17C width=4 src=0x6ae858 dtor drops it (this+95); ctor writes 0
};
static_assert(offsetof(CGUIContextMenu_ev, vptr) == 0x00, "vptr");
static_assert(offsetof(CGUIContextMenu_ev, flag_0x98) == 0x98, "flag_0x98");
static_assert(offsetof(CGUIContextMenu_ev, flag_0x9b) == 0x9B, "flag_0x9b");
static_assert(offsetof(CGUIContextMenu_ev, selected_item) == 0x158, "selected_item");
static_assert(offsetof(CGUIContextMenu_ev, items_begin) == 0x15C, "items_begin");
static_assert(offsetof(CGUIContextMenu_ev, items_end) == 0x160, "items_end");
static_assert(offsetof(CGUIContextMenu_ev, items_cap) == 0x164, "items_cap");
static_assert(offsetof(CGUIContextMenu_ev, pos_x) == 0x168, "pos_x");
static_assert(offsetof(CGUIContextMenu_ev, pos_y) == 0x16C, "pos_y");
static_assert(offsetof(CGUIContextMenu_ev, timestamp) == 0x170, "timestamp");
static_assert(offsetof(CGUIContextMenu_ev, event_parent) == 0x174, "event_parent");
static_assert(offsetof(CGUIContextMenu_ev, ctor_flag_0x178) == 0x178, "ctor_flag_0x178");
static_assert(offsetof(CGUIContextMenu_ev, ref_0x17c) == 0x17C, "ref_0x17c");

// CGUIContextMenu::SItem (stride 96). Only the submenu pointer is evidenced.
struct CGUIContextMenuItem_ev {
  std::uint8_t pad_0x00[88]; // not confirmed
  std::uint32_t submenu; // +0x58 width=4 src=0x6ae858 dtor drops it; vslot #11 (+0x2C) hasOpenSubMenu 6ad620; vslot #12 (+0x30) setVisible 6adc3c; stride 96 from 6acdac
};
static_assert(offsetof(CGUIContextMenuItem_ev, submenu) == 0x58, "submenu");

// CGUIEditBox (ctor 0x6b2608, 0x6b28c8; dtor 0x6b0bac, 0x6b0c7c). Accessors 0x6afa70..0x6b185c.
struct CGUIEditBox_ev {
  std::uint32_t vptr; // +0x00 width=4 src=0x6b2608 primary vptr (off_987450)
  std::uint8_t pad_0x04[341]; // not confirmed
  std::uint8_t draw_border; // +0x159 width=1 src=0x6afaa8 setDrawBorder writes arg; ctor writes arg a3 (6b2608)
  std::uint8_t override_color_enabled; // +0x15A width=1 src=0x6afab0 enableOverrideColor writes arg; setOverrideColor 6afa70 writes 1; ctor writes 0
  std::uint8_t pad_0x15B[9]; // not confirmed
  std::uint32_t override_color; // +0x164 width=4 src=0x6afa70 setOverrideColor stores 4-byte colour; ctor writes bytes ff,ff,ff,65 at +0x164..+0x167
  std::uint32_t override_font; // +0x168 width=4 src=0x6b162c setOverrideFont stores ref; dtor 6b0bac drops it
  std::uint8_t pad_0x16C[4]; // not confirmed
  std::uint32_t skin_ref; // +0x170 width=4 src=0x6b2608 ctor takes vslot #10 (+0x28) result of env, increments refcount; dtor 6b0bac drops it
  std::uint8_t pad_0x174[16]; // not confirmed
  std::uint32_t max_len; // +0x184 width=4 src=0x6b185c setMax writes; getMax 6afaf4 reads; ctor writes 0
  std::uint8_t word_wrap; // +0x188 width=1 src=0x6b1624 setWordWrap writes; isWordWrapEnabled 6afab8 reads; ctor writes 0
  std::uint8_t multi_line; // +0x189 width=1 src=0x6afac0 setMultiLine writes; isMultiLineEnabled 6afac8 reads; ctor writes 0
  std::uint8_t auto_scroll; // +0x18A width=1 src=0x6afae4 setAutoScroll writes; isAutoScrollEnabled 6afaec reads; ctor writes 1
  std::uint8_t password; // +0x18B width=1 src=0x6afad0 isPasswordBox reads 6afad0; ctor writes 0
  std::int32_t i32_0x18c; // +0x18C width=4 src=0x6b2608 ctor writes 42
  std::uint8_t pad_0x190[4]; // not confirmed
  std::int32_t i32_0x194; // +0x194 width=4 src=0x6b2608 ctor writes 2
  std::uint32_t line_texts_begin; // +0x198 width=4 src=0x6b0bac dtor destroys std::vector of wstring at +0x198; ctor writes 0
  std::uint32_t line_texts_end; // +0x19C width=4 src=0x6b2608 ctor writes 0
  std::uint32_t line_texts_cap; // +0x1A0 width=4 src=0x6b2608 ctor writes 0
  std::uint32_t buffer; // +0x1A4 width=4 src=0x6b0bac dtor calls GlitchFree on it; ctor writes 0
  std::uint8_t pad_0x1A8[16]; // not confirmed
  std::uint32_t one_0x1b8; // +0x1B8 width=4 src=0x6b2608 ctor writes 1
  std::uint32_t one_0x1bc; // +0x1BC width=4 src=0x6b2608 ctor writes 1
  std::int32_t text_rect[4]; // +0x1C0 width=4 x4 src=0x6b2608 ctor copies rect arg words 0..3 (a7) and adds border sizes when draw_border set
};
static_assert(offsetof(CGUIEditBox_ev, vptr) == 0x00, "vptr");
static_assert(offsetof(CGUIEditBox_ev, draw_border) == 0x159, "draw_border");
static_assert(offsetof(CGUIEditBox_ev, override_color_enabled) == 0x15A, "override_color_enabled");
static_assert(offsetof(CGUIEditBox_ev, override_color) == 0x164, "override_color");
static_assert(offsetof(CGUIEditBox_ev, override_font) == 0x168, "override_font");
static_assert(offsetof(CGUIEditBox_ev, skin_ref) == 0x170, "skin_ref");
static_assert(offsetof(CGUIEditBox_ev, max_len) == 0x184, "max_len");
static_assert(offsetof(CGUIEditBox_ev, word_wrap) == 0x188, "word_wrap");
static_assert(offsetof(CGUIEditBox_ev, multi_line) == 0x189, "multi_line");
static_assert(offsetof(CGUIEditBox_ev, auto_scroll) == 0x18A, "auto_scroll");
static_assert(offsetof(CGUIEditBox_ev, password) == 0x18B, "password");
static_assert(offsetof(CGUIEditBox_ev, i32_0x18c) == 0x18C, "i32_0x18c");
static_assert(offsetof(CGUIEditBox_ev, i32_0x194) == 0x194, "i32_0x194");
static_assert(offsetof(CGUIEditBox_ev, line_texts_begin) == 0x198, "line_texts_begin");
static_assert(offsetof(CGUIEditBox_ev, line_texts_end) == 0x19C, "line_texts_end");
static_assert(offsetof(CGUIEditBox_ev, line_texts_cap) == 0x1A0, "line_texts_cap");
static_assert(offsetof(CGUIEditBox_ev, buffer) == 0x1A4, "buffer");
static_assert(offsetof(CGUIEditBox_ev, one_0x1b8) == 0x1B8, "one_0x1b8");
static_assert(offsetof(CGUIEditBox_ev, one_0x1bc) == 0x1BC, "one_0x1bc");
static_assert(offsetof(CGUIEditBox_ev, text_rect) == 0x1C0, "text_rect");

}  // namespace glitch::gui::evidence

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_GUI_EVIDENCE_HPP
