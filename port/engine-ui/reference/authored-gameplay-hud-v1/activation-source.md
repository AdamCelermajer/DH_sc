# Actual root activation and Android capability

Owner activation now executes actual root DisplayRightHud2f9d, not its selected
menu's onPush/onShow alone. Source calls NativeGetOptionParameters(HUDStyle,
ObjHud); selects menu_HUD_+ObjHud.CurrentOption; invokes onPush/onShow; stores
root.CurrentHud at3091; restores IconNumber; queries NativeUseIpodPlayer;
localizes RespawnText. Owner then validates CurrentHud matches its borrowed
selected menu. No injected global is used in the current host regression.

Original APK SHA32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200,
classes.dex SHA650888ef70256b273f1745fe803fbb03ba970ac09ee3e88020a4acc48d297883.
DungeonHunter2.isSupportMM()I atDEX code offset209776 starts const/4 v3,-1;
all normal device comparisons converge to the only return v3, with no later
writer to that register. Native wrapper533570 uses JNI table+204, i.e.
CallStaticIntMethod (NOT Boolean), cached method+28. NativeUseIpodPlayer
43a974 exposes bool only if result==1. Thus actual original Android normal
producer is−1 and authored iPod controls are hidden. Class static Build
manufacturer/model must exist; absent Java producer/null is not success.

NativeGetOptionParameters44a364/368 language_override reads byte global
isKOREAN_BUILD at9f640b, GOTslot998a44, initial ELF/BSS byte0. Both reached
reads must use the same retained source-build field; original Language
override adjusts NumOptions5 and publishes the current language to its
separate source global. Generic locale/device capability is not this flag.
The publication symbol is exact signed int lang_kor at9a5c0c, initial0;
source44a500/504 GOT load then44a508 STR current option.

authored_hud_options_bridge_v1 executes existing whole source entry3/4 inside
CharacterMenuCallV1 using its actual object writes/result. It does not reenter
SwfMovie or directly interpret AS pointers. Settings queries use the same
OwnedHudSettings; remaining backend supplies integer localization,
isKOREAN_BUILD, language publication and Android nativeIsSupportMM result.
Text remains pinned through synchronous callback/member-write reentry.
