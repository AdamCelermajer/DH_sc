/* Independent manual translations of four routines in the user-supplied
 * libDungeonHunter2.so. This is a porting probe, NOT a game engine.
 * Integer operations intentionally retain the original 32-bit wrapping.
 */
.text
.p2align 2

/* Original luaO_log2 at ARM VA 0x00854854, 56 bytes including literal pool.
 * floor(log2(x)); returns -1 for zero. The original uses a byte lookup table.
 */
.global dh2_log2_u32
.type dh2_log2_u32, %function
dh2_log2_u32:
    clz w1, w0
    mov w0, #31
    sub w0, w0, w1
    ret
.size dh2_log2_u32, .-dh2_log2_u32

/* Original luaO_int2fb at ARM VA 0x00854800.
 * Retain the original wrapping add before the logical shift, including UINT_MAX.
 */
.global dh2_int2fb_u32
.type dh2_int2fb_u32, %function
dh2_int2fb_u32:
    mov w3, #0
    cmp w0, #15
    b.ls .Lint2fb_small
.Lint2fb_loop:
    add w0, w0, #1
    lsr w0, w0, #1
    add w3, w3, #1
    cmp w0, #15
    b.hi .Lint2fb_loop
.Lint2fb_small:
    cmp w0, #7
    b.ls .Lint2fb_return
    sub w0, w0, #8
    add w3, w3, #1
    orr w0, w0, w3, lsl #3
.Lint2fb_return:
    ret
.size dh2_int2fb_u32, .-dh2_int2fb_u32

/* Original luaO_fb2int at ARM VA 0x00854838.
 * The exponent-zero case returns the entire input, matching the original even
 * for inputs outside the normal 8-bit floating-byte domain.
 */
.global dh2_fb2int_u32
.type dh2_fb2int_u32, %function
dh2_fb2int_u32:
    ubfx w3, w0, #3, #5
    cbz w3, .Lfb2int_return
    and w0, w0, #7
    add w0, w0, #8
    sub w3, w3, #1
    lsl w0, w0, w3
.Lfb2int_return:
    ret
.size dh2_fb2int_u32, .-dh2_fb2int_u32

/* Original glitch::gui::IGUIElement::isPointInside at ARM VA 0x00534e48.
 * x0 points to a LEGACY-LAYOUT object: signed 32-bit rectangle coordinates at
 * 0x48,0x4c,0x50,0x54. x1 points to two signed 32-bit coordinates.
 * Both arguments are real 64-bit pointers. This does NOT redefine the full C++
 * class or solve its embedded 32-bit pointers, vtable or object ownership.
 */
.global dh2_ui_contains_point_legacy32
.type dh2_ui_contains_point_legacy32, %function
dh2_ui_contains_point_legacy32:
    ldr w3, [x1]
    ldr w2, [x0, #0x48]
    cmp w2, w3
    b.gt .Lpoint_outside
    ldr w2, [x1, #4]
    ldr w4, [x0, #0x4c]
    cmp w4, w2
    b.gt .Lpoint_outside
    ldr w4, [x0, #0x50]
    cmp w3, w4
    b.gt .Lpoint_outside
    ldr w4, [x0, #0x54]
    cmp w2, w4
    cset w0, le
    ret
.Lpoint_outside:
    mov w0, #0
    ret
.size dh2_ui_contains_point_legacy32, .-dh2_ui_contains_point_legacy32

.section .note.GNU-stack,"",%progbits
