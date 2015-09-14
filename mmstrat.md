# NAME #

mmstrat - build Mastermind strategies

# SYNOPSIS #

mmstrat `[`**-r** _rules_`]` **-s** _strategy_ `[`options`]`

# DESCRIPTION #

**mmstrat** builds a strategy tree for the given _rules_ using the given _strategy_. After the tree is built, it is output to STDOUT in Irving format.

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

Additional options can be specified to control features of the strategies. In general you don't need to specify any of them, as the default values offer the best performance.

  * **-e** _filter_
> Specifies the equivalence filter to use when checking for canonical guesses. This option does not affect the output; it only affects the speed. It may be used to compare the performance of different filters. Available filters are:
    * **default**
> > composite filter (color + constraint)
    * **color**
> > filter by color equivalence
    * **constraint**
> > filter by constraint equivalence
    * **none**
> > do not apply any filter

  * **-nc**

> Do not apply a correction to the heuristic score for guesses from remaining possibilities. By default, a correction term is applied which effectively favors guesses from remaining possibilities.

  * **-no**
> Do not attempt to make an obvious guess before applying the heuristic function. By default, an obvious guess is attempted and used if found. Specifying this option may be useful if the heuristic function may yield a guess that is different than an obvious guess.

## Optimal Strategy ##

An optimal strategy can be selected by **-s optimal**. It performs an exhaustive search to minimize the total number of steps required to reveal all secrets. Additional options include:

  * **-O** _level_
> Specify the level of optimization to perform. A higher level of optimization produces a more ideal solution, but takes more time to run. Available values are:
    * **1**
> > Minimize the total number of guesses required to reveal all secrets. This is the default if no **-O** option is given.

# OPTIONS #

  * **-h**

> Display help screen and exit.

  * **-mt** _n_
> Enable parallel execution with _n_ threads. If _n_ is not specified, it defaults to the number of logical processors (CPUs or cores) on the host machine. If this option is not specified or if multithreading is not supported on the host machine, the program will execute in a single thread.

  * **-po**
> Make a guess from remaining possibilities only. By default, the program will make a guess from the entire set of valid codewords. Specifying this option will drastically reduce the running time of the program, at the cost of yielding only a sub-optimal strategy.

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

> Strategy  Total   Avg   1    2    3    4    5    6    7    8   >8
> optimal    5625 4.340   1    8   95  644  541    7    -    -    -

where `Strategy` is the strategy name, `Total` is the total number of guesses required to reveal all secrets, `Avg` is the average number of guesses required to reveal a secret, and `1`, `2`, ... are the number of guesses revealed by each given number of guesses.

If both the **-S** option and the **-q** option is present, the program outputs a brief summary of the strategy tree, in the following format:

> Total:Depth:Worst

where `Total` is the total number of guesses required to reveal all secrets, `Depth` is the maximum number of guesses needed to reveal any secret, and `Worst` is the number of secrets revealed using the maximum number of guesses.

# EXAMPLES #

The following command finds an optimal strategy for the standard Mastermind rules. It is optimal in the sense that it takes the lowest average number of guesses to reveal each secret. This should produce a result within one second.
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

We should implement the **-md** _depth_ switch for an optimal strategy to limit the maximum number of guesses allowed to reveal any single secret.

The **-O** _level_ switch should be extended to include more optimization levels.

The output format should be extended to Knuth, Extended, and XML.

The program should be able to read a strategy tree output by itself, and then output it identically.

The performance of finding an optimal strategy should be greatly improved.

# AUTHOR #

fancidev@gmail.com