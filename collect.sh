#!/bin/bash

# check in source code how many ITERATION the test will run
ITERATION=100

# measure total time elapsed
STARTTIME=`date +%s`

# SSE
# compile (without optimization)
echo "building SSE wo/ optimization"
gcc ssebench.c -o sse

# compile (with optimization)
echo "building SSE w/ optimization"
gcc ssebench.c -O2 -o sse-optimized

# AVX
# compile (without optimization)
echo "building AVX wo/ optimization"
gcc ssebench.c -mavx -o avx

# compile (with AVX)
echo "building AVX w/ optimization"
gcc ssebench.c -mavx -O2 -o avx-optimized

# execute and write into txt files
echo "executing SSE"
./sse > sse.txt
echo "executing SSE-optimized"
./sse-optimized > sse-optimized.txt
echo "executing AVX"
./avx > avx.txt
echo "executing AVX-optimized"
./avx-optimized > avx-optimized.txt

# collect results
## init normal
echo "collecting results for SSE"
SSE_1=`awk '{s+=$1} END { print s/'"$ITERATION"' }' sse.txt`
SSE_2=`awk '{s+=$2} END { print s/'"$ITERATION"' }' sse.txt`
SSE_3=`awk '{s+=$3} END { print s/'"$ITERATION"' }' sse.txt`
SSE_4=`awk '{s+=$4} END { print s/'"$ITERATION"' }' sse.txt`

echo "collecting results for SSE-optimized"
SSE_OPT_1=`awk '{s+=$1} END { print s/'"$ITERATION"' }' sse-optimized.txt`
SSE_OPT_2=`awk '{s+=$2} END { print s/'"$ITERATION"' }' sse-optimized.txt`
SSE_OPT_3=`awk '{s+=$3} END { print s/'"$ITERATION"' }' sse-optimized.txt`
SSE_OPT_4=`awk '{s+=$4} END { print s/'"$ITERATION"' }' sse-optimized.txt`

echo "collecting results for AVX"
AVX_1=`awk '{s+=$1} END { print s/'"$ITERATION"' }' avx.txt`
AVX_2=`awk '{s+=$2} END { print s/'"$ITERATION"' }' avx.txt`
AVX_3=`awk '{s+=$3} END { print s/'"$ITERATION"' }' avx.txt`
AVX_4=`awk '{s+=$4} END { print s/'"$ITERATION"' }' avx.txt`

echo "collecting results for AVX-optimized"
AVX_OPT_1=`awk '{s+=$1} END { print s/'"$ITERATION"' }' avx-optimized.txt`
AVX_OPT_2=`awk '{s+=$2} END { print s/'"$ITERATION"' }' avx-optimized.txt`
AVX_OPT_3=`awk '{s+=$3} END { print s/'"$ITERATION"' }' avx-optimized.txt`
AVX_OPT_4=`awk '{s+=$4} END { print s/'"$ITERATION"' }' avx-optimized.txt`

printf "\n"
printf "Results in ms (milliseconds)\n\n"
printf "%9s %9s %9s %9s\n" "Init-Norm" "Init-SSE " "Math-Norm" "Math-SSE "
printf "%9.6f %9.6f %9.6f %9.6f\n\n" $SSE_1 $SSE_2 $SSE_3 $SSE_4
printf "%9s %9s %9s %9s\n" "Init-Norm" "Init-SSEO" "Math-Norm" "Math-SSEO"
printf "%9.6f %9.6f %9.6f %9.6f\n\n" $SSE_OPT_1 $SSE_OPT_2 $SSE_OPT_3 $SSE_OPT_4
printf "%9s %9s %9s %9s\n" "Init-Norm" "Init-AVX " "Math-Norm" "Math-AVX "
printf "%9.6f %9.6f %9.6f %9.6f\n\n" $AVX_1 $AVX_2 $AVX_3 $AVX_4
printf "%9s %9s %9s %9s\n" "Init-Norm" "Init-AVXO" "Math-Norm" "Math-AVXO"
printf "%9.6f %9.6f %9.6f %9.6f\n\n" $AVX_OPT_1 $AVX_OPT_2 $AVX_OPT_3 $AVX_OPT_4

# measure total time elapsed
ENDTIME=`date +%s`
echo "Total time $(($ENDTIME - $STARTTIME)) seconds"
