/**
  @file shmem_and_reduce.c
  @brief Implementation of the shmem_and_reduce benchmarks.
  @author Michael Beebe (Texas Tech University)
 */

#include "shmem_and_reduce.h"



/**
  @brief Run the bandwidth benchmark for shmem_and_reduce
  @param opts Benchmark options given by the user 
 */

#define BENCH_SHMEM_REDUCE_DEFINITION(_op) \
void bench_shmem_##_op##_reduce_bw(options * opts) { \
  /* Ensure there are at least 2 PEs available to run the benchmark */ \
  if (!check_if_atleast_2_pes()) { \
    return; \
  } \
  /* Variables for message sizes, times, and bandwidths */  \
  int *msg_sizes; \
  double *times, *bandwidths; \
  int num_sizes = 0; \
  /* Setup benchmark */ \
  setup_bench(opts->min_msg_size, opts->max_msg_size, &num_sizes, &msg_sizes, &times, \
              &bandwidths); \
  /* Run the benchmark */ \
  for (int i = 0, size = opts->min_msg_size; size <= opts->max_msg_size; size *= 2, i++) { \
    /* Validate the message size for the long datatype */ \
    int valid_size = validate_typed_size(size, sizeof(long), "long"); \
    msg_sizes[i] = valid_size; \
    /* Calculate the number of elements based on the validated size */ \
    int elem_count = calculate_elem_count(valid_size, sizeof(long)); \
    /* Allocate memory for source and destination arrays */ \
    long *source = (long *)shmem_malloc(elem_count * sizeof(long)); \
    long *dest = (long *)shmem_malloc(elem_count * sizeof(long)); \
    /* Initialize the source buffer with data */ \
    for (int j = 0; j < elem_count; j++) { \
      source[j] = j; \
    } \
    double start_time, end_time; \
    /* Sync PEs */ \
    shmem_barrier_all(); \
    /* Do warmup runs */ \
    for (int j = 0; j < opts->warmups; j++) { \
      shmem_##_op##_reduce(SHMEM_TEAM_WORLD, dest, source, elem_count); \
    } \
    shmem_barrier_all(); \
    /* Start timer */ \
    start_time = mysecond(); \
    /* Perform the shmem_and_reduce operation for the specified number of times */ \
    for (int j = 0; j < opts->ntimes; j++) { \
      shmem_##_op##_reduce(SHMEM_TEAM_WORLD, dest, source, elem_count); \
    } \
    shmem_quiet(); \
    /* Stop timer */ \
    end_time = mysecond(); \
    /* Calculate average time per operation in useconds */ \
    times[i] = (end_time - start_time) * 1e6 / opts->ntimes; \
    /* Calculate bandwidth*/ \
    bandwidths[i] = calculate_bw(valid_size, times[i]); \
    /* Free the allocated memory for source and destination arrays */ \
    shmem_free(source); \
    shmem_free(dest); \
  } \
 \
  /* Display results */ \
  shmem_barrier_all(); \
  if (shmem_my_pe() == 0) { \
    display_results(times, msg_sizes, bandwidths, "bw", num_sizes); \
  } \
  shmem_barrier_all(); \
  /* Free the memory allocated for message sizes, times, and bandwidths */ \
  free(msg_sizes); \
  free(times); \
  free(bandwidths); \
} 



#define BENCH_SHMEM_REDUCE(_op) \
  BENCH_SHMEM_REDUCE_DEFINITION(_op)

BENCH_SHMEM_REDUCE_OPERATIONS(BENCH_SHMEM_REDUCE)
#undef BENCH_SHMEM_REDUCE


/**
  @brief Run the bandwidth benchmark for shmem_and_reduce
  @param opts Benchmark options given by the user 
 */
// void bench_shmem_and_reduce_bw(options * opts) {
//   /* Ensure there are at least 2 PEs available to run the benchmark */
//   if (!check_if_atleast_2_pes()) {
//     return;
//   }
//
//   /* Variables for message sizes, times, and bandwidths */
//   int *msg_sizes;
//   double *times, *bandwidths;
//   int num_sizes = 0;
//
//   /* Setup benchmark */
//   setup_bench(opts->min_msg_size, opts->max_msg_size, &num_sizes, &msg_sizes, &times,
//               &bandwidths);
//
//   /* Get the number of processing elements (PEs) and PE number */
//   int npes = shmem_n_pes();
//
//   /* Run the benchmark */
//   for (int i = 0, size = opts->min_msg_size; size <= opts->max_msg_size; size *= 2, i++) {
//     /* Validate the message size for the long datatype */
//     int valid_size = validate_typed_size(size, sizeof(long), "long");
//     msg_sizes[i] = valid_size;
//
//     /* Calculate the number of elements based on the validated size */
//     int elem_count = calculate_elem_count(valid_size, sizeof(long));
//
//     /* Allocate memory for source and destination arrays */
//     long *source = (long *)shmem_malloc(elem_count * sizeof(long));
//     long *dest = (long *)shmem_malloc(elem_count * sizeof(long));
//
//     /* Initialize the source buffer with data */
//     for (int j = 0; j < elem_count; j++) {
//       source[j] = j;
//     }
//
//     double start_time, end_time;
//
//     /* Sync PEs */
//     shmem_barrier_all();
//
//     /* Do warmup runs */
//     for (int j = 0; j < opts->warmups; j++) {
//       shmem_and_reduce(SHMEM_TEAM_WORLD, dest, source, elem_count);
//     }
//
//     shmem_barrier_all();
//
//     /* Start timer */
//     start_time = mysecond();
//
//     /* Perform the shmem_and_reduce operation for the specified number of times */
//     for (int j = 0; j < opts->ntimes; j++) {
//       shmem_and_reduce(SHMEM_TEAM_WORLD, dest, source, elem_count);
//     }
//     shmem_quiet();
//
//     /* Stop timer */
//     end_time = mysecond();
//
//     /* Calculate average time per operation in useconds */
//     times[i] = (end_time - start_time) * 1e6 / opts->ntimes;
//
//     /* Calculate bandwidth*/
//     bandwidths[i] = calculate_bw(valid_size, times[i]);
//
//     /* Free the allocated memory for source and destination arrays */
//     shmem_free(source);
//     shmem_free(dest);
//   }
//
//   /* Display results */
//   shmem_barrier_all();
//   if (shmem_my_pe() == 0) {
//     display_results(times, msg_sizes, bandwidths, "bw", num_sizes);
//   }
//   shmem_barrier_all();
//
//   /* Free the memory allocated for message sizes, times, and bandwidths */
//   free(msg_sizes);
//   free(times);
//   free(bandwidths);
// }


