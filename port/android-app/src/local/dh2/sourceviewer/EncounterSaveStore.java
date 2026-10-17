package local.dh2.sourceviewer;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.zip.CRC32;

/** Authored encounter saves, separate from every original game save/cache file.
 * One process-wide writer, two atomic replacement slots, immutable checkpoints.
 * Native code independently validates the payload's version/content/state. */
final class EncounterSaveStore {
    private static final int HEADER = 32, MAX_PAYLOAD = 8192;
    private static final Map<String, EncounterSaveStore> STORES = new HashMap<>();
    private static final ExecutorService WRITER = Executors.newSingleThreadExecutor(r -> {
        Thread t = new Thread(r, "DH2 encounter saves"); t.setDaemon(true); return t;
    });
    private final File directory;
    private final File[] slots;
    private final byte[][] loaded = new byte[2][];
    private final long[] sequences = new long[2];
    private final Set<File> rejected = new HashSet<>();
    private byte[] latest;
    private long owner, epoch, revision, committed, nextSequence, lastQueued;
    private boolean queued, urgent;
    private volatile String message = "";
    private boolean filesPresent;

    static synchronized EncounterSaveStore get(File directory) throws IOException {
        String key = directory.getCanonicalPath();
        EncounterSaveStore store = STORES.get(key);
        if (store == null) { store = new EncounterSaveStore(directory); STORES.put(key, store); }
        return store;
    }
    EncounterSaveStore(File directory) throws IOException {
        this.directory = directory;
        if (!directory.isDirectory() && !directory.mkdirs()) throw new IOException("Cannot create save directory");
        slots = new File[]{new File(directory, "encounter-a.dh2s"), new File(directory, "encounter-b.dh2s")};
        for (int i = 0; i < 2; i++) if (slots[i].exists()) {
            filesPresent = true;
            try {
                long size = Files.size(slots[i].toPath());
                if (size < HEADER || size > HEADER + MAX_PAYLOAD) throw new IOException("Save length is invalid");
                byte[] file = Files.readAllBytes(slots[i].toPath());
                sequences[i] = sequence(file); loaded[i] = decode(file);
                nextSequence = Math.max(nextSequence, sequences[i]);
            } catch (IOException | IllegalArgumentException error) {
                sequences[i] = 0; rejected.add(slots[i]); message = "A damaged save was retained.";
            }
        }
    }
    synchronized byte[][] candidates() {
        List<byte[]> result = new ArrayList<>();
        if (latest != null) result.add(latest.clone());
        int first = sequences[0] >= sequences[1] ? 0 : 1;
        for (int i : new int[]{first, 1 - first}) if (loaded[i] != null) {
            boolean duplicate = false;
            for (byte[] value : result) if (Arrays.equals(value, loaded[i])) duplicate = true;
            if (!duplicate) result.add(loaded[i].clone());
        }
        return result.toArray(new byte[0][]);
    }
    synchronized boolean hasFiles() { return filesPresent || latest != null; }
    synchronized void rejected(byte[] payload) {
        for (int i = 0; i < 2; i++) if (Arrays.equals(payload, loaded[i])) rejected.add(slots[i]);
    }
    synchronized void attach(long session) { owner = session; epoch++; }
    synchronized void checkpoint(byte[] payload, long session, boolean force, boolean reset) {
        if (session != owner || payload == null || payload.length < 64 || payload.length > MAX_PAYLOAD) return;
        if (reset) epoch++;
        if (!Arrays.equals(latest, payload) || reset) { latest = payload.clone(); revision++; }
        urgent |= force;
        if (!queued && revision > committed && (urgent || System.nanoTime() - lastQueued >= 1000000000L)) schedule();
    }
    private void schedule() {
        queued = true; lastQueued = System.nanoTime(); WRITER.execute(this::writeLatest);
    }
    private void writeLatest() {
        byte[] payload; long savedRevision, savedEpoch, seq; int target; boolean protect;
        synchronized (this) {
            payload = latest.clone(); savedRevision = revision; savedEpoch = epoch; urgent = false;
            seq = ++nextSequence; target = sequences[0] <= sequences[1] ? 0 : 1;
            protect = rejected.contains(slots[target]);
        }
        File temporary = new File(directory, slots[target].getName() + ".new");
        try {
            if (seq <= 0) throw new IOException("Save sequence exhausted");
            byte[] file = encode(payload, seq);
            try (FileOutputStream out = new FileOutputStream(temporary)) { out.write(file); out.getFD().sync(); }
            if (protect && slots[target].isFile()) {
                Files.copy(slots[target].toPath(), new File(directory,
                    slots[target].getName() + ".rejected-" + UUID.randomUUID()).toPath());
            }
            synchronized (this) {
                if (savedEpoch == epoch) {
                    // Same-directory atomic rename. If unsupported, fail safely;
                    // the other valid slot remains intact and no success is shown.
                    Files.move(temporary.toPath(), slots[target].toPath(),
                        StandardCopyOption.ATOMIC_MOVE, StandardCopyOption.REPLACE_EXISTING);
                    sequences[target] = seq; loaded[target] = payload; rejected.remove(slots[target]);
                    committed = savedRevision; filesPresent = true; message = "";
                }
            }
        } catch (IOException | RuntimeException error) {
            message = "Progress not saved: " + error.getMessage();
        } finally {
            try { Files.deleteIfExists(temporary.toPath()); } catch (IOException ignored) { }
            synchronized (this) {
                queued = false;
                if (urgent && revision > committed) schedule();
                notifyAll();
            }
        }
    }
    synchronized String status() {
        if (!message.isEmpty()) return message;
        return latest == null ? "" : committed >= revision ? "Progress saved." : "Saving progress…";
    }
    synchronized boolean awaitIdle(long milliseconds) throws InterruptedException {
        long end = System.nanoTime() + milliseconds * 1000000L;
        while (queued) {
            long remaining = end - System.nanoTime(); if (remaining <= 0) return false;
            wait(Math.max(1, remaining / 1000000L));
        }
        return true;
    }
    private static int crc(byte[] bytes, int start, int length) {
        CRC32 crc = new CRC32(); crc.update(bytes, start, length); return (int)crc.getValue();
    }
    static byte[] encode(byte[] payload, long sequence) {
        if (payload == null || payload.length < 64 || payload.length > MAX_PAYLOAD || sequence <= 0)
            throw new IllegalArgumentException("Invalid save envelope");
        byte[] file = new byte[HEADER + payload.length];
        ByteBuffer b = ByteBuffer.wrap(file).order(ByteOrder.LITTLE_ENDIAN);
        b.put(new byte[]{'D','H','F','1'}).putInt(1).putLong(sequence).putInt(payload.length)
            .putInt(crc(payload, 0, payload.length)).putInt(0).putInt(0).put(payload);
        b.putInt(28, crc(file, 0, 28)); return file;
    }
    static byte[] decode(byte[] file) {
        sequence(file);
        return Arrays.copyOfRange(file, HEADER, file.length);
    }
    private static long sequence(byte[] file) {
        if (file == null || file.length < HEADER || file.length > HEADER + MAX_PAYLOAD)
            throw new IllegalArgumentException("Invalid save envelope length");
        ByteBuffer b = ByteBuffer.wrap(file).order(ByteOrder.LITTLE_ENDIAN);
        if (b.get() != 'D' || b.get() != 'H' || b.get() != 'F' || b.get() != '1' || b.getInt() != 1)
            throw new IllegalArgumentException("Unsupported save envelope");
        long seq = b.getLong(); int size = b.getInt(), checksum = b.getInt(), reserved = b.getInt(), headerCrc = b.getInt();
        if (seq <= 0 || size < 64 || size != file.length - HEADER || reserved != 0 ||
            headerCrc != crc(file, 0, 28) || checksum != crc(file, HEADER, size))
            throw new IllegalArgumentException("Invalid save envelope checksum");
        return seq;
    }
}
