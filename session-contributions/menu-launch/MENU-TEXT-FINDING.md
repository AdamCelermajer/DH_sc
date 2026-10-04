# Original authored button text does not reach the linked display setter

Update: the property/layout connection is now implemented in the isolated snapshot. `swf_text_layout_connection.cpp`, the generated `text-property-v1` overlay, and the real provider face-metric methods connect the original HTML parser and layout to retained display records. All four main button labels, including styled MORE GAMES!, passed screenshot/provider checks at three sizes (`text-tests-v17-recheck/`). Inline images and underlines still reject a reached missing display owner. Every original text-format setter and the complete original display body remain unproven. The text below records the diagnosis that led to this change.

Build v9 log `native-main-v9.log` inspects `_root.menu_MainMenu.btn_MENU_SINGLE_PLAYER.text` after actual authored `onShow`. Its htmlText exists as a stored AS string; its visible text still shows BtnText. This child is the actual edit-text field, not a ButtonLabeled class wrapper. Earlier parent-button prototype diagnostics were not evidence of a missing constructor for this field.

The pinned vendor `gameswf_text.cpp` edit_text_character::set_member handles M_TEXT and calls set_text_value, but has no htmlText handling. It falls through to character::set_member, storing an ordinary string without formatting the displayed field. That explains why the original localization provider reports Start game but the screenshot still displays BtnText.

Read-only original ARM evidence is captured in `original-edit-text-set-member.asm`, from .local-inputs/libDungeonHunter2.so, symbol `_ZN7gameswf19edit_text_character10set_memberERKNS_10tu_stringiERKNS_8as_valueE`, address 0x790ca4, size 0x2b4. The original standard-member switch contains adjacent cases calling set_text_value(tu_string const&, bool), passing false at 0x790dec and true at 0x790e50, before delegating to character::set_member. The added bridge reaches original `text_layout_v1` for HTML; the untouched plain upstream path and full setter composition still need original-domain parity work.

The snapshot already contains a separately proven `text_layout_v1` subsystem and its original format_text(bool)/html_reader evidence (`port/engine-ui/reference/text-layout-v1/NOTES.md`). It explicitly says retained core setters and Android drawing integration remain prerequisites. Do not claim full HTML parity by simply renaming htmlText to text or adopting the upstream HTML parser.

This finding narrows the text issue; it does not resolve save/settings/menu callbacks or authored input/frame ownership.
