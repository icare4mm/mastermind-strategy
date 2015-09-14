# NAME #

mmstrat, mmserve, mmbreak, mmdebug - build Mastermind strategies

# SYNOPSIS #

mmstrat `[`options`]` `[`**-r** _rules_`]` **-s** _strategy_

mmserve `[`options`]` `[`**-r** _rules_`]` `[`_secret_`]`

mmbreak `[`options`]` `[`**-r** _rules_`]` **-s** _strategy_

mmdebug `[`options`]`

# DESCRIPTION #

These programs demonstrate state-of-the-art strategies for playing the Mastermind game.

**mmstrat** builds a strategy tree for the given _rules_ using the given _strategy_ and outputs the strategy tree to STDOUT.

**mmserve** works as a codemaker. It reads from STDIN one guess at a time, and prints a response. The program exits when the user reveals the secret. If _secret_ is specified, it is used by the program as the secret. This is only useful for debugging purpose.

**mmbreak** works as a codebreaker.

**mmdebug** is diagnostics mode. This will launch an interactive debugger which provides a set of commands for examining the codeword space.

# RULES #

The rules of the game can be specified by the **-r** _rules_ option. A set of rules consists of three parts:

  1. number of pegs
  1. number of colors
  1. whether repetition of color is allowed

The rules are specified in the form "**p** _pegs_ **c** _colors_ **r**" to allow color repetition, or "**p** _pegs_ **c** _colors_ **n**" to disallow color repetition, without quotes or spaces in between. In addition, it can take one of the following predefined values:

  * **mm** or `p4c6r`
> Standard Mastermind rules (4 pegs, 6 colors, with repetition). This is the default if no **-r** option is given.
  * **bc** or `p4c10n`
> Bulls and Cows rules (4 pegs, 10 colors, no repetition).
  * **lg** or `p5c8r`
> Logik rules (5 pegs, 8 colors, with repetition).

For performance reasons, there are built-in limits on the number of pegs and colors. The maximum number of pegs supported is 6, and the maximum number of colors supported is 10.

# STRATEGIES #

## Simple Strategy ##

A simple strategy can be selected by **-s simple**. It makes a guess using the first remaining possibility.

## Heuristic Strategy ##

A heuristic strategy can be selected by **-s** _heuristic_. It makes a guess based on a heuristic score. Supported heuristics are:

  * **minmax**
> min-max heuristic strategy
  * **minavg**
> min-average heuristic strategy
  * **entropy**
> max-entropy heuristic strategy
  * **parts**
> max-parts heuristic strategy

Additional options can be specified to amend the behavior of the strategies. In general you don't need to specify any of these options, as the default values offer the best performance. The options are provided for testing purpose only.

  * **-e** _filter_
> Specifies the equivalence filter to use when checking for canonical guesses. Available filters are:
    * **default**
> > composite filter (color + constraint)
    * **color**
> > filter by color equivalence
    * **constraint**
> > filter by constraint equivalence
    * **none**
> > do not apply any filter

  * **-nc**

> Do not apply a correction to the heuristic score for guesses from remaining possibilities. By default, a correction is applied in this case which effectively favors guesses from remaining possibilities.

  * **-no**
> Do not attempt to make an obvious guess before applying the heuristic function. By default, an obvious guess is attempted and used if found. Specifying this option may be useful if the heuristic function may yield a guess that is different than an obvious guess.

## Optimal Strategy ##

An optimal strategy can be selected by **-s optimal**. It performs an exhaustive search to minimize the total number of steps required to reveal all secrets. Additional options include:

  * **-md** _depth_
> Specify the maximum number of guesses allowed to reveal any single secret. This option is only supported for an optimal strategy. If not all secrets can be revealed within _depth_ guesses, no strategy will be generated.

# OPTIONS #

  * **-h**
> Display help screen and exit.

  * **-mt** _n_
> Enable parallel execution with _n_ threads. If _n_ is not specified, it defaults to the number of logical processors (CPUs or cores) on the host machine. If this option is not specified or if multithreading is not supported on the host machine, the program will execute in a single thread.

  * **-po**
> Make guesses from remaining possibilities only. If this option is not specified, the program will make guesses from the entire set of valid codewords. Specifying this option will drastically reduce the execution time of the program, at the cost of finding only a sub-optimal strategy.

  * **-prof**
> Collect and display profiling details. The program has built-in support for intrumental profiling on critical routines, but the profiling is not turned on by default. Specifying this option will collect profiling data at run-time, and display them before the program exits. There is a slight performance panelty (around 5%) to collect profiling data.

  * **-q**
> Quiet mode; display minimal information.

  * **-S**
> Output strategy summary instead of strategy tree. See the OUTPUT section below for details.

  * **-v**
> Display version and exit.

# OUTPUT #

## Strategy Tree ##

If the **-S** option is not specified, the program outputs the strategy tree in Irving format. The format is described as follows. (Describe the strategy tree output format here.)

## Strategy Summary ##

If the **-S** option is specified and the **-q** option is not specified, the program outputs summary statistics of the strategy tree, in the following format:

> Strategy  Total   Avg 1    2    3    4    5    6    7    8   >8
> > optimal   5625 4.340 1    8   95  644  541    7    -    -    -

where `Strategy` is the strategy name, `Total` is the total number of guesses required to reveal all secrets, `Avg` is the average number of guesses required to reveal a secret, and `1`, `2`, ... are the number of guesses revealed by each given number of guesses.

If both the **-S** option and the **-q** option is present, the program outputs a brief summary of the strategy tree, in the following format:


> Total:Depth:Worst

where `Total` is the total number of guesses required to reveal all secrets, `Depth` is the maximum number of guesses needed to reveal any secret, and `Worst` is the number of secrets revealed using the maximum number of guesses.

# EXAMPLES #

The following command finds an optimal strategy for the standard Mastermind rules. It is optimal in the sense that it takes the lowest average number of guesses to reveal each secret. This should produce a result below one second.
```
  mmstrat -s optimal
```

The following command finds an optimal strategy for Bulls and Cows rules. This takes about 5-10 minutes to run on a modern computer.
```
  mmstrat -s optimal -r bc
```

The following command finds a heuristic strategy for Logik rules using the `entropy` heuristic. In addition, on machines with multiple CPUs or cores, it will execute with two threads. This takes about 10 seconds to run.
```
  mmstrat -s entropy -r p5c8r -mt 2
```

# SEE ALSO #

Visit http://code.google.com/p/mastermind-strategy/ for updates.

# BUGS #

List known bugs.

# AUTHOR #

fancidev@gmail.com

# COPYRIGHT #

This program and its source code is distributed under MIT License.