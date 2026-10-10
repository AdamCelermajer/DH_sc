package com.example.dh2;

/** Integer segment gain/mix operations observed in VoxNativeSubDecoder. */
final class VxnSegmentMixMath {
    static final int Q30_ONE = 0x40000000;
    static final int SLOT_COUNT = 3;

    private VxnSegmentMixMath() {}

    static int slope(int fromQ30, int toQ30, int frames) {
        if (frames <= 0) throw new IllegalArgumentException("fade frames must be positive");
        return (toQ30 - fromQ30) / frames;
    }

    /** Mixes one PCM span and advances each linear gain once per stereo frame. */
    static void mix(short[][] source, int frames, int[] gainQ30, int[] stepQ30,
                    int[] fadeRemaining, int[] fadeTargetQ30, short[] output) {
        if (source.length != SLOT_COUNT || gainQ30.length != SLOT_COUNT ||
                stepQ30.length != SLOT_COUNT || fadeRemaining.length != SLOT_COUNT ||
                fadeTargetQ30.length != SLOT_COUNT || output.length < frames * 2)
            throw new IllegalArgumentException("invalid VXN mixer buffers");
        for (short[] track : source)
            if (track != null && track.length < frames * 2)
                throw new IllegalArgumentException("short source span");
        for (int frame = 0; frame < frames; frame++) {
            int left = 0, right = 0;
            for (int slot = 0; slot < SLOT_COUNT; slot++) {
                short[] track = source[slot];
                if (track != null) {
                    int highGain = gainQ30[slot] >> 15;
                    left += (track[frame * 2] * highGain) >> 15;
                    right += (track[frame * 2 + 1] * highGain) >> 15;
                }
                if (fadeRemaining[slot] > 0 && --fadeRemaining[slot] == 0) {
                    gainQ30[slot] = fadeTargetQ30[slot];
                    stepQ30[slot] = 0;
                } else if (fadeRemaining[slot] > 0) {
                    gainQ30[slot] += stepQ30[slot];
                }
            }
            output[frame * 2] = saturate(left);
            output[frame * 2 + 1] = saturate(right);
        }
    }

    static short saturate(int sample) {
        return (short) Math.max(Short.MIN_VALUE, Math.min(Short.MAX_VALUE, sample));
    }
}
