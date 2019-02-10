# sse-bench
Benchmark for code with/without SSE for initialization and math operations

# How?

You can look at the benchmark implementation inside `ssebench.c`.
Basically, it operates on float array with 10M+4 elements (intended to make it multiple of 4 perfectly for SSE).

Preprocessor check added in case your machine has no SSE support, thus it will print relevant message then quit.

Benchmark spans into 2 categories

1. Initialization
2. Math operations

Each test against code with / without SSE to compare performance.

It executes each categories in sequence for 100 iterations, then I sum up execution time for each round for each of type then divide by 100. Result is in `ms` (millsecond).

As well, variables are made sure to be aligned in memory allocation (16-bytes alignment) to satisfy SSE 128-bit instruction.

# Collect Result

As from test, I compiled and built the program with and without optimization flags.

with `gcc ssebench.c -o ssebench`, and another with `gcc ssebench.c -O2 -o ssebench-optimized`

Then execute it one by one

`./ssebench > ssebench.txt`

then

`./ssebench-optimized > ssebench-optimized.txt`

Result has 4 columns in each line for its execution time for

<initialization **without** SSE> <initialization **with** SSE> <math operations **without** SSE> <math operations **with** SSE>

I average the result by using `awk '{s+=$1} END {print s/100}'` to get average of numbers in first column, then change `$1` to `$2` to get average for second column for initialization with SSE, and so on.

# Result

Results are as following

Without compiler's optimization flags
* Initialization **without** SSE => 21.7788 ms
* Initialization **with** SSE => 8.05029 ms
* Math operations **without** SSE => 26.9069 ms
* Math operations **with** SSE => 21.1805 ms

With compiler's optimization flags (`-O2`)
* Initialization **without** SSE => 4.42866 ms
* Initialization **with** SSE => 4.27777 ms
* Math operations **without** SSE => 6.56853 ms
* Math operations **with** SSE => 6.62412 ms

# Testing Machine

MBP mid-2012 

macOS mojave 10.14 (18A391) 

RAM 8 GB 1600 MHz DDR3 

Processor 2.5 GHz Intel Core i5

Note: gcc is in fact clang with gcc frontend

# Reference

- [Modern compiler will auto-vectorize code as per optimization applied](https://stackoverflow.com/a/50786881/571227) - this might answer the result from above that code with / without SSE perform similar the same at `-O2` optimization level.

# License
MIT, Wasin Thonkaew
