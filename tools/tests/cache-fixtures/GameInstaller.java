/* TEST FIXTURE ONLY. Never include this class in any Android build or APK.
 * It supplies host paths for cache tests because the real GameInstaller's
 * static initialization calls Android Environment. All checksum, metadata,
 * marker and DownloadComponent implementations under test are production code.
 */
package com.gameloft.android.GAND.GloftD2SS.installer;
public final class GameInstaller {
    public static String DATA_PATH;
    public static String marketPath;
    public static String sd_folder;
    public static void addErrorNumber(int value) {
        throw new AssertionError("Unexpected GameInstaller error path in cache-only test: " + value);
    }
}
