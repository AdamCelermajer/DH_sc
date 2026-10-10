# Lane 19 — dialogue and tutorials handoff

## Source delivered

- `MenuDialogMessagesV97` now models the original `DialogMsg` name parsing for localized and pre-resolved title/message strings, plus actor names. It requires a parser callback instead of silently publishing untranslated `$player` tokens.
- `SourceScriptUiWorldV97::parse_player_name` resolves the current local character through the same Application/PlayerManager and validates the active Character/Save association before applying the existing `$player` transform. A missing character preserves the source input.
- Existing tutorial command handling already routes kinds 71–76 through the retained tutorial/CharacterMenu queues; control kinds 77–78 use the actual settings cells, local player, difficulty gate, script manager and settings persistence path. Those handlers were inspected and left intact.

## Required shared wiring

`OriginalUiSession::dialog_owner_v97()` constructs `MenuDialogServicesV97`; it must set the new `parse_player_name` callback there. Capture the same weak `Impl` owner, require a live `script_ui_world_v97`, and call `world->parse_player_name(input, ui->localization.pack() >= 4 && ui->localization.pack() <= 6, parsed, error)`. This binds DialogMsg to the actual selected Save name and active language spacing rule. Without this callback, dialogue construction now fails explicitly; no authored dialogue is claimed integrated yet.

The source `send_script_message` path still has no positive `CMsgScriptCmd` network transport, and `before_stop_dialog` still lacks the real online hosting selector/message sender. These are honest blockers for script choice/online branches; do not bypass or synthesize success. The existing dialog queue supports the real enqueue/start/read/skip flow once the callback is wired.

## Offline script-message gate

`SourceScriptUiWorldV97::send_script_message` now checks the same live Application `GetOnline` owner. `COnline.byte5 == 0` returns success without constructing or sending a message; an online call still reports the missing positive transport. IDA confirms this no-op branch: `Script_ExitCutSceneMode::Execute` at `0x4599ac` tests byte5 at `0x459a18–0x459a28` and returns before `CMessage::CreateMessage`/`CMessaging::SendMsg`; its positive branch creates type-1 `CMsgScriptCmd` with words `(-2,-1)` at `0x459a34–0x459a64`. `ScriptManager::SkipScript` at `0x4604c0` sends only when byte5 is nonzero and its received flag is false; `StartScript` at `0x4605c0` also branches around message creation when byte5 is zero. Thus the offline success is specifically suppression of an online-only network message, while online delivery remains unimplemented and explicit.

## IDA basis and validation boundary

- `ParsePlayerName`: `0x433d00`; original calls `GetLocalPlayer(0, true)`, gets the actual Character name and replaces `$player` through `StringManager::parse`.
- `DialogMsg::SetActorName`: `0x433dd8`; actor text is localized then passed through `ParsePlayerName`.
- `DialogMsg(std::string const&, std::string const&, int, int)`: `0x433ea8`; both title and body use `ParsePlayerName`, then actor-name handling.
- `Script_ExitCutSceneMode::Execute`: `0x4599ac`; `GetOnline()->byte5` gates message creation and player cutscene updates.
- `ScriptManager::SkipScript` / `StartScript`: `0x4604c0` / `0x4605c0`; their network messages are gated by byte5 and `SendMsg` has no result value.
- No build, test, emulator, or runtime claim was made; the coordinator owns integrated validation.
