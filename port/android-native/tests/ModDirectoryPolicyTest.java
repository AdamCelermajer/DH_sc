package com.example.dh2;

import java.io.File;
import java.io.IOException;

public final class ModDirectoryPolicyTest {
    private static void check(boolean condition, String message) {
        if (!condition) throw new AssertionError(message);
    }

    private static void delete(File file) {
        if (file.isDirectory()) {
            File[] children = file.listFiles();
            if (children != null) for (File child : children) delete(child);
        }
        if (file.exists() && !file.delete()) throw new AssertionError("Could not clean test path: " + file);
    }

    public static void main(String[] args) throws IOException {
        File root = new File(System.getProperty("java.io.tmpdir"), "dh2-mod-policy-" + System.nanoTime());
        File privateRoot = new File(root, "private");
        File externalRoot = new File(root, "external");
        check(externalRoot.mkdirs(), "test external root setup");
        File preferred = ModDirectoryPolicy.prepare(externalRoot, privateRoot);
        check(preferred != null && preferred.equals(new File(externalRoot, "mods")),
                "writable app-scoped external storage is preferred");
        delete(root);

        root = new File(System.getProperty("java.io.tmpdir"), "dh2-mod-policy-" + System.nanoTime());
        privateRoot = new File(root, "private");
        externalRoot = new File(root, "external");
        check(root.mkdirs(), "test fallback root setup");
        check(externalRoot.createNewFile(), "test unwritable external root setup");
        File fallback = ModDirectoryPolicy.prepare(externalRoot, privateRoot);
        check(fallback != null && fallback.equals(new File(privateRoot, "mods")),
                "unavailable external storage falls back to private app storage");
        delete(root);
        System.out.println("PASS: app-scoped mod directory selection and storage fallback");
    }
}
