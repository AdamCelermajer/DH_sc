package com.example.dh2;

public final class IntroMovieAssetsTest {
    private static void check(String actual, String expected, String message) {
        if (!expected.equals(actual)) throw new AssertionError(message + ": " + actual);
    }

    public static void main(String[] args) {
        check(IntroMovieAssets.pathForSourceLanguage(0), "original-media/intro.mp4", "English source video");
        check(IntroMovieAssets.pathForSourceLanguage(3), "original-media/intro.mp4", "Italian source video");
        check(IntroMovieAssets.pathForSourceLanguage(4), "original-media/intro_jp.mp4", "Japanese source video");
        check(IntroMovieAssets.pathForSourceLanguage(5), "original-media/intro_kr.mp4", "Korean source video");
        check(IntroMovieAssets.pathForSourceLanguage(6), "original-media/intro.mp4", "Chinese source video fallback");
        check(IntroMovieAssets.pathForSourceLanguage(7), "original-media/intro.mp4", "Spanish source video fallback");
        System.out.println("PASS source language 4/5 selects original Japanese/Korean intro; other source languages use intro.mp4");
    }
}
