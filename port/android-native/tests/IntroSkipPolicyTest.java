package com.example.dh2;

public final class IntroSkipPolicyTest {
    public static void main(String[] args) {
        IntroSkipPolicy policy = new IntroSkipPolicy();
        if (policy.onMovieTap(7300) || policy.isSkipVisible())
            throw new AssertionError("source ignores taps at and before the 7300 ms threshold");
        if (!policy.onMovieTap(7301) || !policy.isSkipVisible())
            throw new AssertionError("first eligible tap reveals skip control");
        if (policy.onMovieTap(8000) || policy.isSkipVisible())
            throw new AssertionError("second eligible tap hides skip control");
        System.out.println("PASS: original intro skip-control tap threshold and toggle");
    }
}
