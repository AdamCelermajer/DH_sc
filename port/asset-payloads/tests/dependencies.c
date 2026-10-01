// Host model of imported compiler arithmetic helpers, not engine algorithms.
#include <stdint.h>
float oracle_i2f(int32_t a) { return (float)a; }
double oracle_ui2d(uint32_t a) { return (double)a; }
double oracle_i2d(int32_t a) { return (double)a; }
int32_t oracle_f2iz(float a) { return (int32_t)a; }
int32_t oracle_d2iz(double a) { return (int32_t)a; }
float oracle_fmul(float a, float b) { return a * b; }
float oracle_fdiv(float a, float b) { return a / b; }
double oracle_dmul(double a, double b) { return a * b; }
