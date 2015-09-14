# Release 1.0 #

This is the first release of the `mmstrat` utility. It is released in March 2012.

## Features ##

This program provides a rich set of features that covers major areas of interest in Mastermind strategies. Key features are:

  * Finds optimal strategy that minimizes the expected number of guesses required to reveal a secret.
  * Finds heuristic strategies using the most popular heuristics, namely min-max, min-average, entropy, and max-parts.
  * Outputs strategy tree in standard Irving format.
  * Built-in multi-threading support to take advantage of multi-core CPUs if available.
  * Various options to fine-tune specific features of the strategies.

## Performance Highlights ##

The program is designed with the objective to optimize speed. Below are the running results for building an entire strategy tree. They are run using a single thread on an Intel 2.4G processor with SSE 2 support.

  * Standard Mastermind rules, optimal strategy: 0.5 seconds
  * Bulls and Cows rules, optimal strategy: 6 minutes
  * Bulls and Cows rules, entropy heuristic strategy: 0.2 seconds
  * Logik rules, entropy heuristic strategy: 14 seconds
  * Logik rules, entropy heuristic strategy (2 threads): 8 seconds

To achieve this performance, several key optimizations techniques are employed:

  * **SIMD instructions**. It uses highly-optimized SSE 2 instructions in critical routines, which provides significant performance enhancement over a straightforward implementation.
  * **OpenMP support**. It has built-in OpenMP support that parallels the execution. For heuristic strategies, using two threads can almost double the performance (halves the run-time) over using a single thread.
  * **Branch pruning**. For optimal strategies, it employs various techniques to prune branches in the search space.

## Portability ##

The source code is written in portable C++, and should readily build under both Windows and Linux. This is the recommended way to obtain the software. Instructions for building from source can be found in [Building](Building.md).

Pre-built binaries are provided for a limited number of platforms for the convenience of users. Details can be found in the Downloads page.