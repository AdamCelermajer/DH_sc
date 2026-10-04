# Loading atlas coordinates

v21 adds a one-shot log of the actual loading bitmap draw. audit_loading_bitmap.py independently decodes the supplied SWF shape bounds/fill MATRIX and compares its inverse with this native log. loading-draw-v21/authored-matrix-audit.json records PASS for this correspondence, not appearance.

The SWF shape has bounds [173,753,112,697] twips, bitmap matrix [20,0,-6750,0,20,-6601]. Its actual texture pixel bounds are [346.15,375.15,335.65,364.90]. The logged draw uses 2048x1024 texture pixels and the matching inverse matrix [0.05,0,337.5,0,0.05,330.05]. original-splash-atlas.png renders the exact bundled TGA for inspection. This sampled rectangle falls on artwork; the visible spinner graphics are elsewhere. Thus a display resize alone cannot resolve this appearance.

Original SwfTextureLoader 0x416c14 matches menus/splash_final_droid.tga explicitly. Default/non-Korean/non-Japanese language selects data/3d/textures/splash_final_droid.tga. Korean source enum5 and Japanese enum4 select their named variants. original-swf-texture-loader.asm preserves the complete original body. The English texture-selection path already agrees with the current provider.

Correction to an early diagnostic hypothesis: the original tag35 loader 0x75f6fc also reads the id/u32 and creates an empty bitmap placeholder. The stock no-JPEG path likewise supplies an empty bitmap, followed by external substitution. A different JPEG decoder is not demonstrated as the cause. Native loading-screen setup, the SWF chosen by that setup, and the supplied resource set still require investigation. No guessed atlas coordinate patch was made.

The first v21 live capture was intercepted by Android PackageUpdateActivity and timed out observing the default menu. The same emulator remained live. After the package updater settled, the loading activity was explicitly selected, the draw captured, and the main menu restored. Both emulator operations were confined to ADB server5038/emulator5580.
