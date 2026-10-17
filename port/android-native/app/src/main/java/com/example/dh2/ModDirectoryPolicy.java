package com.example.dh2;

import java.io.File;

/** Chooses a writable app-scoped mod directory with a private-storage fallback. */
final class ModDirectoryPolicy {
    private ModDirectoryPolicy() { }

    static File prepare(File externalFilesRoot, File privateFilesRoot) {
        File external = externalFilesRoot == null ? null : new File(externalFilesRoot, "mods");
        if (ensureDirectory(external)) return external;

        File internal = privateFilesRoot == null ? null : new File(privateFilesRoot, "mods");
        return ensureDirectory(internal) ? internal : null;
    }

    private static boolean ensureDirectory(File directory) {
        return directory != null && (directory.isDirectory() || directory.mkdirs() && directory.isDirectory());
    }
}
