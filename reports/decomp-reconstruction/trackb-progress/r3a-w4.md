# r3a-w4 progress (wave r3a, redo, glitch-scene-collada-gui)
<id> | reconstructed N | partial N | skipped N | upstream N | compiles yes/no | note
r3a-glitch-scene-collada-gui-glitch_scene_CSceneManager-146879-0 | reconstructed 9 | partial 11 | skipped 0 | upstream 0 | compiles yes | CSceneManager virtuals; drawInit/drawAll partial (CAttributes layout and SStats), XML readers/writer partial (reader type mismatch), ctor partial (CBatchDriver); added IShadowReceiverTarget::texture (+0x0c) to scene/SceneFactories.hpp
r3a-glitch-scene-collada-gui-glitch_scene_CSceneManager-146879-1 | reconstructed 0 | partial 10 | skipped 0 | upstream 0 | compiles yes | all blocked: CBatchDriver, render lists, mesh cache, XML reader type (layout requests noted in ledger rows)
r3a-glitch-scene-collada-gui-glitch_gui_CGUITable-c6846a-0 | reconstructed 0 | partial 20 | skipped 0 | upstream 0 | compiles n/a (no source change) | BLOCKED: no canonical glitch::gui CGUITable header; the stand-in in gui_simple_accessors.hpp maps only some fields
r3a-glitch-scene-collada-gui-glitch_gui_CGUITable-c6846a-1 | reconstructed 0 | partial 9 | skipped 0 | upstream 0 | compiles n/a | BLOCKED: same as -0
r3a-glitch-scene-collada-gui-glitch_gui_CGUITTFont-c1dcf5-0 | reconstructed 0 | partial 20 | skipped 0 | upstream 0 | compiles n/a | BLOCKED: no canonical glitch::gui CGUITTFont header
r3a-glitch-scene-collada-gui-glitch_gui_CGUITTFont-c1dcf5-1 | reconstructed 0 | partial 3 | skipped 0 | upstream 0 | compiles n/a | BLOCKED: same as -0
r3a-glitch-scene-collada-gui-glitch_gui_CGUITabControl-d0d8a8-0 | reconstructed 0 | partial 20 | skipped 0 | upstream 0 | compiles n/a | BLOCKED: no canonical glitch::gui CGUITabControl header
r3a-glitch-scene-collada-gui-glitch_gui_CGUITabControl-d0d8a8-1 | reconstructed 0 | partial 2 | skipped 0 | upstream 0 | compiles n/a | BLOCKED: same as -0
r3a-glitch-scene-collada-gui-glitch_gui_CGUIContextMenu-e2ce80-0 | reconstructed 0 | partial 19 | skipped 0 | upstream 0 | compiles n/a | BLOCKED: no canonical glitch::gui CGUIContextMenu header
r3a-glitch-scene-collada-gui-glitch_gui_CGUIButton-0ec56c-0 | reconstructed 0 | partial 16 | skipped 0 | upstream 0 | compiles n/a | BLOCKED: no canonical glitch::gui CGUIButton header (gui_elements.hpp stand-in)
r3a-glitch-scene-collada-gui-glitch_gui_CGUIComboBox-55fdaa-0 | reconstructed 0 | partial 15 | skipped 0 | upstream 0 | compiles n/a | BLOCKED: no canonical glitch::gui CGUIComboBox header
r3a-glitch-scene-collada-gui-pack40-74bc1f-0 | reconstructed 8 | partial 12 | skipped 0 | upstream 0 | compiles yes (src/collada/CResFileReader.cpp) | collada camera stand-in and streaming/texture paths blocked; four CResFileReader forwarders added
