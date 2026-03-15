/**
  @file shmem_and_reduce.h
  @brief Header file for shmem_and_reduce benchmarks.
  @author Michael Beebe (Texas Tech University)
 */

#ifndef SHMEM_AND_REDUCE_H
#define SHMEM_AND_REDUCE_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shmembench.h"
#include "parse_opts.h"

/**
  @brief Run the bandwidth benchmark for shmem_and_reduce
  @param opts Benchmark options given by the user 
 */
void bench_shmem_and_reduce_bw(options * opts);

#endif /* SHMEM_AND_REDUCE_H */
