package com.example.dh2;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.media.AudioAttributes;
import android.media.MediaPlayer;
import android.os.Handler;
import android.os.Looper;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.TextView;

import java.io.IOException;
import java.util.Locale;

/** Plays the opening movie stretched across the full native splash/loading stage. */
final class IntroCinematicView extends FrameLayout implements TextureView.SurfaceTextureListener {
    private static final String TAG = "DH2Native";
    private static final String ASSET = "original-media/intro.mp4";
    private String selectedAsset = ASSET;

    private final Runnable onFinished;
    private final TextureView video;
    private final TextView subtitles;
    private final TextView skipButton;
    private final IntroSkipPolicy skipPolicy = new IntroSkipPolicy();
    private final Handler subtitleHandler = new Handler(Looper.getMainLooper());
    private volatile int subtitleLanguage = IntroSubtitleTrack.languageIndex(Locale.getDefault());
    private final Runnable subtitleTick = new Runnable() {
        @Override public void run() {
            if (finished || !prepared || player == null) return;
            try {
                subtitles.setText(IntroSubtitleTrack.textAt(player.getCurrentPosition(), subtitleLanguage));
                subtitles.setVisibility(subtitles.length() == 0 ? View.GONE : View.VISIBLE);
                if (player.isPlaying()) subtitleHandler.postDelayed(this, 100L);
            } catch (IllegalStateException e) {
                Log.w(TAG, "Could not update opening subtitles", e);
            }
        }
    };
    private MediaPlayer player;
    private Surface videoSurface;
    private int videoWidth;
    private int videoHeight;
    private boolean requested;
    // The source selects intro_jp/intro_kr from persisted game language, not
    // the Android device locale. MainActivity resolves it after loading the
    // authored front menu; do not start a potentially wrong movie first.
    private boolean sourceLanguageReady;
    private boolean prepared;
    private boolean seekPending;
    // MainActivity requests playback during onCreate, before Android resumes
    // the window. Hold the prepared decoder until resumePlayback() explicitly
    // opens the gate; otherwise a slow startup can consume the intro off-screen.
    private boolean paused = true;
    private boolean finished;
    private int resumePositionMs;
    private float musicVolume = 1f;
    private boolean audioFocusGained;
    private boolean audioFocusKnown;

    IntroCinematicView(Context context, Runnable onFinished) {
        this(context, null, onFinished);
    }

    /** Replaces the provisional device locale with the game's persisted source setting. */
    void setSourceLanguageIndex(int languageIndex) {
        if (languageIndex >= 0 && languageIndex < 8) {
            subtitleLanguage = languageIndex;
            selectedAsset = IntroMovieAssets.pathForSourceLanguage(languageIndex);
            sourceLanguageReady = true;
            startIfReady();
        }
    }

    private IntroCinematicView(Context context, AttributeSet attrs, Runnable onFinished) {
        super(context, attrs);
        this.onFinished = onFinished;
        // Keep the video surface opaque while it fills the native viewport.
        setBackgroundColor(Color.BLACK);
        View matte = new View(context);
        matte.setBackgroundColor(Color.BLACK);
        addView(matte, new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        video = new TextureView(context);
        video.setOpaque(true);
        video.setSurfaceTextureListener(this);
        addView(video, new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        subtitles = new TextView(context);
        subtitles.setTextColor(Color.WHITE);
        subtitles.setTextSize(16);
        subtitles.setTypeface(Typeface.SERIF, Typeface.BOLD);
        subtitles.setGravity(Gravity.CENTER);
        subtitles.setMaxLines(2);
        subtitles.setShadowLayer(2f, 1f, 1f, Color.BLACK);
        subtitles.setPadding(dp(12), dp(8), dp(12), dp(8));
        GradientDrawable captionBackground = new GradientDrawable();
        captionBackground.setColor(Color.argb(145, 0, 0, 0));
        captionBackground.setCornerRadius(dp(6));
        subtitles.setBackground(captionBackground);
        subtitles.setVisibility(View.GONE);
        FrameLayout.LayoutParams captionLayout = new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT,
                Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);
        captionLayout.setMargins(dp(28), 0, dp(28), dp(28));
        addView(subtitles, captionLayout);
        skipButton = new TextView(context);
        skipButton.setText("Skip");
        skipButton.setTextColor(Color.WHITE);
        skipButton.setTextSize(14);
        skipButton.setTypeface(Typeface.SERIF, Typeface.BOLD);
        skipButton.setGravity(Gravity.CENTER);
        skipButton.setPadding(dp(16), dp(8), dp(16), dp(8));
        GradientDrawable skipBackground = new GradientDrawable();
        skipBackground.setColor(Color.argb(190, 20, 16, 12));
        skipBackground.setCornerRadius(dp(5));
        skipBackground.setStroke(dp(1), Color.rgb(190, 157, 83));
        skipButton.setBackground(skipBackground);
        skipButton.setContentDescription("Skip opening cinematic");
        skipButton.setVisibility(View.GONE);
        FrameLayout.LayoutParams skipLayout = new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT,
                Gravity.TOP | Gravity.END);
        skipLayout.setMargins(0, dp(24), dp(24), 0);
        addView(skipButton, skipLayout);
        skipButton.setOnClickListener(v -> finish(true));
        // Keep the native splash/loading stage visible while MediaPlayer
        // prepares the movie. The menu is loaded only after this sequence.
        setAlpha(0f);
        setContentDescription("Opening cinematic. Tap to show or hide skip control.");
        setOnClickListener(v -> {
            if (player == null || !prepared) return;
            boolean visible = skipPolicy.onMovieTap(currentPositionMs());
            skipButton.setVisibility(visible ? View.VISIBLE : View.GONE);
            if (visible) Log.i(TAG, "Opening cinematic skip control shown");
            else if (currentPositionMs() > 7300) Log.i(TAG, "Opening cinematic skip control hidden");
        });
        setClickable(true);
    }

    private int dp(float value) {
        return Math.round(value * getResources().getDisplayMetrics().density);
    }

    void startPlayback() {
        startPlayback(0);
    }

    void startPlayback(int positionMs) {
        requested = true;
        resumePositionMs = Math.max(0, positionMs);
        startIfReady();
    }

    void pausePlayback() {
        paused = true;
        subtitleHandler.removeCallbacks(subtitleTick);
        if (player != null) {
            try {
                if (player.isPlaying()) player.pause();
            } catch (IllegalStateException ignored) {
                // The asynchronous prepare callback observes paused and won't start it.
            }
        }
    }

    void resumePlayback() {
        paused = false;
        if (player == null) {
            startIfReady();
            return;
        }
        // prepareAsync may still be in flight when Activity resumes. Only the
        // prepared state is allowed to call MediaPlayer.start().
        if (prepared) startPreparedPlayback(player);
    }

    void dispose() {
        finished = true;
        releasePlayer();
    }

    boolean isPending() { return !finished; }

    void setMusicVolume(float volume) {
        musicVolume = Math.max(0f, Math.min(1f, volume));
        applyMusicVolume();
    }

    void setAudioFocusGained(boolean gained) {
        audioFocusKnown = true;
        audioFocusGained = gained;
        applyMusicVolume();
    }

    private void applyMusicVolume() {
        if (player != null && prepared) {
            try {
                float output = OriginalMusicVolume.outputGain(musicVolume, audioFocusKnown, audioFocusGained);
                player.setVolume(output, output);
            } catch (IllegalStateException e) {
                Log.w(TAG, "Could not apply opening movie music volume", e);
            }
        }
    }

    int currentPositionMs() {
        if (player != null && prepared && !finished) {
            try {
                return Math.max(0, player.getCurrentPosition());
            } catch (IllegalStateException ignored) {
                // Use the last retained seek position while the decoder changes state.
            }
        }
        return resumePositionMs;
    }

    private void startIfReady() {
        if (!requested || !sourceLanguageReady || finished || player != null || !video.isAvailable()) return;
        // Surface recreation can leave the old opaque frame in the view while
        // a new decoder is preparing. Re-hide it until the new first frame.
        setAlpha(0f);
        try {
            AssetFileDescriptor source = getContext().getAssets().openFd(selectedAsset);
            MediaPlayer candidate = new MediaPlayer();
            player = candidate;
            videoWidth = videoHeight = 0;
            videoSurface = new Surface(video.getSurfaceTexture());
            candidate.setSurface(videoSurface);
            candidate.setAudioAttributes(new AudioAttributes.Builder()
                    .setUsage(AudioAttributes.USAGE_MEDIA)
                    .setContentType(AudioAttributes.CONTENT_TYPE_MOVIE)
                    .build());
            float outputVolume = OriginalMusicVolume.outputGain(musicVolume, audioFocusKnown, audioFocusGained);
            candidate.setVolume(outputVolume, outputVolume);
            candidate.setOnVideoSizeChangedListener((mp, width, height) -> {
                videoWidth = width;
                videoHeight = height;
                fitVideo();
            });
            candidate.setOnInfoListener((mp, what, extra) -> {
                if (what == MediaPlayer.MEDIA_INFO_VIDEO_RENDERING_START && !finished && player == mp) {
                    // Reveal the overlay on the first rendered video frame,
                    // not merely when prepareAsync reports the track size.
                    post(() -> {
                        if (!finished && player == mp) setAlpha(1f);
                    });
                }
                return false;
            });
            candidate.setOnPreparedListener(mp -> {
                if (finished || player != mp) return;
                prepared = true;
                float output = OriginalMusicVolume.outputGain(musicVolume, audioFocusKnown, audioFocusGained);
                mp.setVolume(output, output);
                boolean hasAudioTrack = false;
                try {
                    for (MediaPlayer.TrackInfo track : mp.getTrackInfo()) {
                        if (track.getTrackType() == MediaPlayer.TrackInfo.MEDIA_TRACK_TYPE_AUDIO) {
                            hasAudioTrack = true;
                            break;
                        }
                    }
                } catch (IllegalStateException ignored) {
                    // Preparation is complete; track diagnostics are best effort.
                }
                Log.i(TAG, "Opening movie prepared | asset=" + selectedAsset + " | audioTrack=" + hasAudioTrack
                        + " | musicGain=" + musicVolume + " | focusKnown=" + audioFocusKnown
                        + " | focusGained=" + audioFocusGained + " | outputGain=" + output);
                videoWidth = mp.getVideoWidth();
                videoHeight = mp.getVideoHeight();
                fitVideo();
                if (resumePositionMs > 0) {
                    final int position = resumePositionMs;
                    seekPending = true;
                    mp.setOnSeekCompleteListener(seekPlayer -> {
                        if (finished || player != seekPlayer) return;
                        seekPending = false;
                        resumePositionMs = 0;
                        startPreparedPlayback(seekPlayer);
                    });
                    try {
                        mp.seekTo(position);
                    } catch (IllegalStateException e) {
                        Log.w(TAG, "Could not restore opening movie position", e);
                        seekPending = false;
                        resumePositionMs = 0;
                        startPreparedPlayback(mp);
                    }
                } else {
                    startPreparedPlayback(mp);
                }
            });
            candidate.setOnCompletionListener(mp -> finish());
            candidate.setOnErrorListener((mp, what, extra) -> {
                Log.e(TAG, "Opening movie playback failed: " + what + "/" + extra);
                finish();
                return true;
            });
            try (source) {
                candidate.setDataSource(source.getFileDescriptor(), source.getStartOffset(), source.getLength());
            }
            candidate.prepareAsync();
        } catch (IOException | IllegalArgumentException | IllegalStateException e) {
            Log.e(TAG, "Could not open bundled opening movie: " + selectedAsset, e);
            finish();
        }
    }

    private void fitVideo() {
        IntroVideoFit.Result fitted = IntroVideoFit.fullSurface(
                videoWidth, videoHeight, video.getWidth(), video.getHeight());
        if (fitted == null) return;
        // Match the source startup presentation: scale each axis to the full
        // TextureView bounds so wide and portrait sizes receive no mattes.
        Matrix transform = new Matrix();
        transform.setScale(fitted.scaleX, fitted.scaleY,
                video.getWidth() * 0.5f, video.getHeight() * 0.5f);
        video.setTransform(transform);
    }

    private void startPreparedPlayback(MediaPlayer target) {
        if (finished || paused || seekPending || !prepared || target == null || player != target) return;
        try {
            if (!target.isPlaying()) target.start();
            subtitleHandler.removeCallbacks(subtitleTick);
            subtitleHandler.post(subtitleTick);
        } catch (IllegalStateException e) {
            Log.w(TAG, "Could not start opening movie", e);
            finish();
        }
    }

    @Override protected void onSizeChanged(int width, int height, int oldWidth, int oldHeight) {
        super.onSizeChanged(width, height, oldWidth, oldHeight);
        // A window resize can change the TextureView without recreating its
        // SurfaceTexture. Refit here as well as on texture/video callbacks so
        // rotation, cutout policy, and resizable-window changes keep the movie
        // stretched across the complete live surface.
        video.post(this::fitVideo);
    }

    private void finish() {
        finish(false);
    }

    private void finish(boolean skippedByUser) {
        if (finished) return;
        finished = true;
        subtitleHandler.removeCallbacks(subtitleTick);
        subtitles.setText("");
        subtitles.setVisibility(View.GONE);
        skipButton.setVisibility(View.GONE);
        if (skippedByUser) Log.i(TAG, "Opening cinematic skipped by user");
        releasePlayer();
        post(onFinished);
    }

    private void releasePlayer() {
        subtitleHandler.removeCallbacks(subtitleTick);
        MediaPlayer old = player;
        if (!finished && old != null && prepared && !seekPending) {
            try {
                resumePositionMs = Math.max(0, old.getCurrentPosition());
            } catch (IllegalStateException ignored) {
                // Keep the most recently saved position during decoder teardown.
            }
        }
        player = null;
        prepared = false;
        seekPending = false;
        if (old != null) {
            try {
                old.setOnCompletionListener(null);
                old.setOnErrorListener(null);
                old.setOnInfoListener(null);
                old.setOnPreparedListener(null);
                old.setOnVideoSizeChangedListener(null);
                old.setOnSeekCompleteListener(null);
                old.release();
            } catch (IllegalStateException ignored) {
                // Releasing an already-failed decoder is safe to ignore.
            }
        }
        Surface oldSurface = videoSurface;
        videoSurface = null;
        if (oldSurface != null) oldSurface.release();
    }

    @Override public void onSurfaceTextureAvailable(SurfaceTexture texture, int width, int height) {
        fitVideo();
        startIfReady();
    }

    @Override public void onSurfaceTextureSizeChanged(SurfaceTexture texture, int width, int height) {
        fitVideo();
    }

    @Override public boolean onSurfaceTextureDestroyed(SurfaceTexture texture) {
        releasePlayer();
        return true;
    }

    @Override public void onSurfaceTextureUpdated(SurfaceTexture texture) { }
}
