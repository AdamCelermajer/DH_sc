# Original Lua numeric callbacks

Source versions of nine registered callbacks are implemented in `numeric.c`:
`ToFixed`, `FromFixed`, `MulFixed`, `DivFixed`, `BitNot`, `BitXOr`, `BitAnd`,
`BitOr` and `Trace`. `bridge.c` installs them in the source Lua runtime.
This module supplies numeric operations; it does not implement gameplay.

## Recovered behavior

`ToFixed` truncates the float to int32 **before** shifting left by eight with
32-bit wrap. `FromFixed` returns **two** values: the signed integer shifted
right eight bits, then a float scaled by 1/256. `MulFixed` wraps the product
before its signed right shift. `DivFixed` divides the first integer by the
second integer shifted right eight bits, truncating toward zero.

`BitNot` needs exactly one argument; `BitXOr` needs exactly two. `BitAnd` and
`BitOr` need at least two and fold every argument. Missing required arguments
return no values; unused trailing arguments are ignored. The original `Trace`
body immediately returns, so its source counterpart produces no output.

Consumed operands here are numbers only. The bridge preserves explicit
non-number rejection with no results for `BitNot`/`BitXOr`; other non-number
operands raise an authored error. String, boolean and object coercion through
original `Value` objects is not reconstructed. Nonfinite/out-of-int32 values,
zero divisors and INT_MIN divided by -1 raise errors. These guards avoid
undefined behavior; their error behavior is not claimed as an original match.

## Evidence

[Differential evidence](arm-differential-validation.json) records **2,375**
comparisons of actual original ARM32 callbacks against compiled ARM64 and host
source. Output counts, integer bits and float bits match. Actual original
`Arguments` indexing and numeric `Value` getters execute. The `ReturnValues`
integer/number push boundaries are explicit capture stubs; return allocation
and engine object ownership are not exercised. Arithmetic imports use the
existing host dependency model. Unsafe divisions are excluded from original
execution. Inputs, output guards and restored stacks are checked.

[Host evidence](host-build-validation.json) includes 6,000 operand lists and
54,000 sanitizer calls, including damaged arguments and rejected conversions.
Checks stop on AddressSanitizer, undefined behavior or float-cast overflow.
[ARM64 build evidence](arm64-build-validation.json) records 16 KiB load alignment.

The [integrated runtime](../lua-runtime/README.md) executes authored bridge
assertions and compiles the original script corpus. Original game scripts are
not executed. Integer PyData constants are now supplied by the separate
[table bridge](../pydata-constants/README.md). Engine objects, includes,
structured PyData, AI and skill behavior remain unfinished; this component
is not packaged in the source Android APK yet.

## Reproduce

```sh
python3 port/lua-numeric/build.py --host --report /path/to/host.json
python3 port/lua-numeric/build.py --ndk /path/to/windows-ndk --report /path/to/arm64.json
python3 port/lua-numeric/tests/differential.py --help
```

The differential test needs Unicorn, pyelftools, the owner's original library
and the previously built host arithmetic oracle. Proprietary binaries are not
inputs to ordinary source builds.
