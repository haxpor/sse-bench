/// Experiment 1: with SIMD via SSE to initialize all element of float array of size SIZE.
/// Experiment 2: do math operations initialized buff with float[4] (with arbitrary values) then store back results into buff.
///               math opeations are 'buff[i] = sqrtf(buff[i] * MUL_VAL) + ADD_VAL' in which buff is our buffer, i is index, MUL_VAL is multiplier value
///               and ADD_VAL is additional value. The latter twos will be in vector for case of SSE operation.  
///
/// Notice malloc16() and free16() to help in allocate aligned memory, and free.
///
/// Output from program has 4 columns in fixed-point number
/// <initialize without SSE> <initialize with SSE> <math operations without SSE> <math operation with SSE>
///
/// Result
/// Interesting to see that if compile without optimization flag, SSE code perform better relatively!
/// But if compile with -O2 or more, comparatively both version executes in similar performance.
/// 
/// Comment or suggest, tweet to @haxpor
///
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#if defined( __SSE__ ) || defined( __SSE2__ )
#define HAS_SSE
#include <xmmintrin.h>
#endif

#include <time.h>

#define SIZE 10000004
#define BENCH_ITERATION 100

// values used to set for benchmark 1
#define SET_VAL 1.f
#define OPTIMIZED_SET_VAL {1.f, 1.f, 1.f, 1.f}
// values used as multiplier for benchmark 2
#define MUL_VAL 6.5f
#define OPTIMIZED_MUL_VAL {6.5f, 6.5f, 6.5f, 6.5f}
// values used as additional for benchmark 2
#define ADD_VAL 100.f
#define OPTIMIZED_ADD_VAL {100.f, 100.f, 100.f, 100.f}

// concept for allocating aligned memory space
// overflow it, then turncate the lower bits
// those two techniques will make sure we have aligned memory
//
// return NULL in case it cannot allocate memory due to out of memory
void* malloc16(size_t s)
{
  // allocate memory 16 bytes (15 bytes for alignment size + 1 byte more to store offset size)
  // this means at worst case, we can possibly waste memory for up to 16 bytes unused to
  // satisfy the memory alignment
  unsigned char* p_original = malloc(s + 0x10);

  // if not enough memory
  if (p_original == NULL)
    return NULL;

  unsigned char* p = p_original;

  // overflow (1.) then truncate (2.) the memory to guaruntee aligned allocation
  //
  // 1.) 0x10 means we offset upwards by 0x0F bytes for actual target byte alignment + 0x01 byte 
  // allocated to store our offset size just before the pointer. Offset upwards like this will 
  // make sure we will pass the boundary location in which we will truncate it next.
  //
  // 2.) & ~0x0F means we truncate lower 4 bits, 0x0F comes from 0x10 - 1 in order for its result 
  // to be negated with ~ thus ~(00001111) => 11110000, so the memory address always at boundary
  // note: for N byte alignment, we only need to offset the pointer from 0 to (N-1) bytes
  p = (unsigned char*)((size_t)(p + 0x10) & (~0x0F));
  // store offset size at the byte before the pointer
  *(p-1) = p - p_original;

  return p;
}

// it will find the original pointer that point to whole memory space before applying with offset
// then free it, thus it needs to grab such offset size at the -1 offset location of input pointer
void free16(void* p)
{
  // convert to byte pointer
  unsigned char* ptr = (unsigned char*)p;

  // grab offset size
  size_t offset = *(ptr-1);
  // free (original) whole memory space
  free(ptr - offset);
}

void benchmark(float* buff, size_t size)
{
  clock_t begin;

  // benchmark 1 - initialization
  // - manual
  // initialize value for input
  begin = clock();
  for (int i=0; i<size; ++i)
  {
    buff[i] = SET_VAL;
  }
  printf("%.8lf ", (double)(clock() - begin) / CLOCKS_PER_SEC * 1000.0);
  
  // - sse
  // this need to be aligned 16-bytes for SSE
  __attribute((aligned(16))) const float aligned_set_vals[4] = OPTIMIZED_SET_VAL;
  __m128 set_values = _mm_load_ps(aligned_set_vals);
  begin = clock();
  for (int i=0; i<size; i+=4)
  {
    //_mm_store_ps(&buff[i], set_values);
    // or possible as well with the following line
    _mm_store_ps(buff + i, set_values);
    //_mm_store_ps((float*)((size_t)buff + i*sizeof(float)), set_values);
  }
  printf("%.8lf ", (double)(clock() - begin) / CLOCKS_PER_SEC * 1000.0);

  // benchmark 2 - multiply and store back
  // - manual
  begin = clock();
  for (int i=0; i<size; ++i)
  {
    buff[i] = sqrtf(buff[i] * MUL_VAL) + ADD_VAL;   
  }
  printf("%.8lf ", (double)(clock() - begin) / CLOCKS_PER_SEC * 1000.0);

  // clear buff to initial value that set
  for (int i=0; i<size; ++i)
  {
    buff[i] = SET_VAL;
  }

  // - sse
  // load and cache multiplier values
  __attribute((aligned(16))) const float aligned_mul_vals[4] = OPTIMIZED_MUL_VAL;
  __attribute((aligned(16))) __m128 mul_values = _mm_load_ps(aligned_mul_vals);

  // load and cache addition values
  __attribute((aligned(16))) const float aligned_add_vals[4] = OPTIMIZED_ADD_VAL;
  __attribute((aligned(16))) __m128 add_values = _mm_load_ps(aligned_add_vals);

  __m128 load_vals, temp_vals;
  begin = clock();
  for (int i=0; i<size; i+=4)
  {
    // load 4 elements each time
    load_vals = _mm_load_ps(&buff[i]);
    // do operations then store back to buff
    _mm_store_ps(&buff[i], _mm_add_ps(_mm_sqrt_ps(_mm_mul_ps(load_vals, mul_values)), add_values));
  }
  printf("%.8lf\n", (double)(clock() - begin) / CLOCKS_PER_SEC * 1000.0);
}

int main (int argc, char* argv[])
{
#ifndef HAS_SSE
  fprintf(stderr, "Your machine doesn't have SSE/SSE2 support\n");
  return -1;
#else
  // allocate 16-byte aligned memory space
  float* buff = malloc16(sizeof(float) * SIZE);

  // do it this way to user can pipe the result externally
  for (int i=0; i<BENCH_ITERATION; ++i)
  {
    benchmark(buff, SIZE);
  }

  // remember to call proper free16()
  free16(buff);
  buff = NULL;

	return 0;
#endif
}
