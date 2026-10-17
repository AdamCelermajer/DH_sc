package local.dh2.sourceviewer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

/** Host checks actual checksums, atomic slot replacement, fallback and failure. */
public final class EncounterSaveStoreTest {
    private static void check(boolean value, String message) { if (!value) throw new AssertionError(message); }
    private static byte[] payload(int seed) {
        byte[] value = new byte[96]; for (int i = 0; i < value.length; i++) value[i] = (byte)(seed + i); return value;
    }
    private static void idle(EncounterSaveStore store) throws Exception { check(store.awaitIdle(10000), "writer did not finish"); }
    public static void main(String[] args) throws Exception {
        Path root = Paths.get(args[0]); Files.createDirectories(root);
        byte[] a = payload(1), b = payload(2), c = payload(3), d = payload(4);
        byte[] file = EncounterSaveStore.encode(a, 0x102030405060708L);
        check(Arrays.equals(a, EncounterSaveStore.decode(file)), "exact envelope round trip");
        for (int i = 0; i < file.length; i++) {
            byte[] changed = file.clone(); changed[i] ^= 1;
            try { EncounterSaveStore.decode(changed); throw new AssertionError("accepted corrupt byte " + i); }
            catch (IllegalArgumentException expected) { }
        }
        for (int i = 0; i < file.length; i++) {
            try { EncounterSaveStore.decode(Arrays.copyOf(file, i)); throw new AssertionError("accepted truncation " + i); }
            catch (IllegalArgumentException expected) { }
        }
        Path directory = Files.createTempDirectory(root, "slots-");
        EncounterSaveStore store = new EncounterSaveStore(directory.toFile()); store.attach(1);
        store.checkpoint(a, 1, true, false); idle(store);
        store.checkpoint(b, 1, true, false); idle(store);
        check("Progress saved.".equals(store.status()), "durability acknowledgement");
        Path newest = directory.resolve("encounter-b.dh2s");
        byte[] damaged = Files.readAllBytes(newest); damaged[50] ^= 1; Files.write(newest, damaged);
        EncounterSaveStore fallback = new EncounterSaveStore(directory.toFile());
        check(fallback.candidates().length == 1 && Arrays.equals(fallback.candidates()[0], a), "damaged newest fallback");
        fallback.attach(2); fallback.checkpoint(c, 2, true, false); idle(fallback);
        check(Arrays.equals(EncounterSaveStore.decode(Files.readAllBytes(newest)), c), "damaged slot repaired");
        try (java.util.stream.Stream<Path> paths = Files.list(directory)) {
            check(paths.anyMatch(p -> p.getFileName().toString().contains(".rejected-")), "damaged save retained");
        }
        fallback.attach(3); fallback.checkpoint(b, 2, true, false); // stale Activity is ignored
        fallback.checkpoint(d, 3, true, true); idle(fallback);
        check(Arrays.equals(new EncounterSaveStore(directory.toFile()).candidates()[0], d), "reset/stale-owner protection");
        for (int i = 0; i < 100; i++) fallback.checkpoint(payload(i), 3, true, false);
        fallback.checkpoint(a, 3, true, true); idle(fallback);
        check(Arrays.equals(new EncounterSaveStore(directory.toFile()).candidates()[0], a), "queued old writes cannot replace reset");

        Path brokenDirectory = Files.createTempDirectory(root, "failure-");
        EncounterSaveStore broken = new EncounterSaveStore(brokenDirectory.toFile()); broken.attach(4);
        broken.checkpoint(a, 4, true, false); idle(broken);
        byte[] preceding = Files.readAllBytes(brokenDirectory.resolve("encounter-a.dh2s"));
        Files.createDirectory(brokenDirectory.resolve("encounter-b.dh2s.new"));
        broken.checkpoint(b, 4, true, false); idle(broken);
        check(broken.status().startsWith("Progress not saved:"), "write failure visible");
        check(Arrays.equals(preceding, Files.readAllBytes(brokenDirectory.resolve("encounter-a.dh2s"))), "previous valid save survives failure");
        check(Arrays.equals(new EncounterSaveStore(brokenDirectory.toFile()).candidates()[0], a), "failure reload preserves prior save");
        System.out.println("PASS: 128 corruption + 128 truncation rejections; exact round trip; atomic slots; corrupt fallback/retention; owner/reset queue; write failure rollback.");
    }
}
