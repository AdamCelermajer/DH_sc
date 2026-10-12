// Test-only import providers. The frozen corpus supplies the exact original
// graphic output through __wrap_*; export real names to preempt DSO imports.
// No production kernel or native libm accuracy claim belongs in this TU.
extern "C" float __wrap_sinf(float);
extern "C" float __wrap_cosf(float);
extern "C" void __wrap_sincosf(float, float*, float*);
extern "C" float sinf(float x) { return __wrap_sinf(x); }
extern "C" float cosf(float x) { return __wrap_cosf(x); }
extern "C" void sincosf(float x, float* s, float* c) {
  __wrap_sincosf(x, s, c);
}
