package com.example.dh2;

/** Original intro movie variants selected by SavegameManager::getLanguage(). */
final class IntroMovieAssets {
    static String pathForSourceLanguage(int language) {
        if (language == 4) return "original-media/intro_jp.mp4";
        if (language == 5) return "original-media/intro_kr.mp4";
        return "original-media/intro.mp4";
    }

    private IntroMovieAssets() { }
}
