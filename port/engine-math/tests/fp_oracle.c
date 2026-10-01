// Test dependency model: these helpers implement the external arithmetic/libm
// contracts. They do not implement any vector/quaternion engine algorithm.
#include <math.h>
float oracle_fadd(float a, float b) { return a + b; }
float oracle_fsub(float a, float b) { return a - b; }
float oracle_fmul(float a, float b) { return a * b; }
float oracle_fdiv(float a, float b) { return a / b; }
float oracle_sqrtf(float a) { return sqrtf(a); }
float oracle_sinf(float a) { return sinf(a); }
float oracle_cosf(float a) { return cosf(a); }
float oracle_asinf(float a) { return asinf(a); }
float oracle_acosf(float a) { return acosf(a); }
float oracle_atan2f(float a, float b) { return atan2f(a, b); }
double oracle_sin(double a) { return sin(a); }
double oracle_cos(double a) { return cos(a); }
double oracle_atan2(double a, double b) { return atan2(a, b); }
double oracle_f2d(float a) { return (double)a; }
float oracle_d2f(double a) { return (float)a; }
double oracle_dadd(double a, double b) { return a + b; }
double oracle_dsub(double a, double b) { return a - b; }
double oracle_dmul(double a, double b) { return a * b; }
double oracle_ddiv(double a, double b) { return a / b; }
