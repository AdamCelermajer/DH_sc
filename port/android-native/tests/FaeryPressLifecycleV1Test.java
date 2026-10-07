package com.example.dh2;
import java.util.ArrayList;
import java.util.Arrays;
public final class FaeryPressLifecycleV1Test {
 public static void main(String[] args) {
  ArrayList<Integer> calls = new ArrayList<>();
  FaeryPressLifecycleV1 owner = new FaeryPressLifecycleV1(calls::add);
  owner.release(); // Cancel before a press must not release an unrelated cast.
  owner.press(); owner.press();
  if (!owner.isPressed()) throw new AssertionError("press lost");
  owner.release(); owner.release();
  owner.press(); owner.release();
  if (owner.isPressed() || !calls.equals(Arrays.asList(1,3,1,3)))
   throw new AssertionError(calls);
  System.out.println("Faery press/release ownership PASS");
 }
}
