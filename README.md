# sse-bench
Benchmark for code with/without SSE for initialization and math operations

# How?

You can look at the benchmark implementation inside `ssebench.c`.
Basically, it operates on float array with 10M+8 elements (intended to make it multiple of 4 perfectly for SSE and AVX whichever one is available but preferred AVX).

Preprocessor check added in case your machine has no SSE support, thus it will print relevant message then quit.

Benchmark spans into 2 categories

1. Initialization
2. Math operations

Each test against code with / without SSE/AVX to compare performance.

It executes each categories in sequence for 100 iterations, then I sum up execution time for each round for each of type then divide by 100. Result is in `ms` (millsecond).

As well, variables are made sure to be aligned in memory allocation (16-bytes alignment) to satisfy SSE 128-bit instruction.

# Build & Collect Result

As from test, I compiled and built the program with and without optimization flags.
Also we need to produce result from SSE, and AVX separately.

Do it like this

1. Compile to get result from SSE via `gcc ssebench.c` with / without optimization flags.
2. Compile to get result from AVX via `gcc ssebench.c -mavx` with / without optimization flags.

> To be able to have AVX support, not just processor needs to support, but also we need to supply compilation flag of `-mavx`. So we will have `__AVX__` defined as our preprocessor in code will have a chance to detect it.

So normal flow in compilation is `gcc ssebench.c -o ssebench`, and another with `gcc ssebench.c -O2 -o ssebench-optimized`  

Then execute it one by one

`./ssebench > ssebench.txt`

then

`./ssebench-optimized > ssebench-optimized.txt`

then repeat with AVX via `gcc ssebench.c -o ssebench-avx -mavx`, and anothe with `gcc ssebench.c -O2 -o ssebench-avx-optimized -mavx`, and collect the result with the same steps but different executable file and output to relevant .txt files to collect result later.

Result has 4 columns in each line for its execution time for

<initialization **without** SSE/AVX> <initialization **with** SSE/AVX> <math operations **without** SSE/AVX> <math operations **with** SSE/AVX>

I average the result by using `awk '{s+=$1} END {print s/100}'` to get average of numbers in first column, then change `$1` to `$2` to get average for second column for initialization with SSE, and so on.

# Automated Script

Above in section _Build & Collect Result_ is automated in provided bash script `collect.sh`. The script will build relevant program, execute it and collect result.

Execute the script with `bash collect.sh`.

It will finally summarize the results for test cases.

# Result (as collected on my testing)

Results as collected via `bash collect.sh`.

* Result as tested on macOS (see spec on _Testing Machine_)

![result on macOS](https://github.com/haxpor/sse-bench/blob/master/result.png)

* Result as tested on Linux Ubuntu 18.04 (see spec on _Testing Machine_)

![result on linux](https://github.com/haxpor/sse-bench/blob/master/result-linux.png)

# Testing Machine

## macOS
MBP mid-2012 

macOS mojave 10.14 (18A391) 

RAM 8 GB 1600 MHz DDR3 

Processor 2.5 GHz Intel Core i5

Note: gcc is in fact clang with gcc frontend

## Linux Ubuntu 18.04

CPU Xeon E3-12xx v2 (ivy bridge) 2.2 GHz with 1 GB of RAM.
1 core with hyperthreading. VM.

# Reference

- [Modern compiler will auto-vectorize code as per optimization applied](https://stackoverflow.com/a/50786881/571227) - this might answer the result from above that code with / without SSE perform similar the same at `-O2` optimization level.

# License
MIT, Wasin Thonkaew
