This project explores a number strategies to solve the Mastermind game
and its variations. The objective is a program that solves the game both in real-time and offline.

## Introduction ##

Mastermind is a game played by two players, the <i>code maker</i> and the <i>code breaker</i>. At the beginning of the game, the code maker thinks of a <i>secret</i> <i>codeword</i> with `p` pegs and `c` colors. Then the code breaker begins making <i>guesses</i>.

For each guess, the code maker responds with a <i>feedback</i> in the form of `xAyB`, where `x` is the number of correct colors in the correct pegs, and `y` is the number of correct colors in the wrong pegs. The game continues until the code breaker finds the secret.

The standard Mastermind game is played on a board with four pegs and using six colors. A codeword can contain the same color more than once. A popular variation of the rules is called _GuessNumber_, also known as Bulls-n-Cows. Under these rules, the codeword is not allowed to contain repetitive colors.

The program in this project plays the role of the code breaker. The goal is to find out the secret using as few guesses as possible. The program supports both the Mastermind rules and the GuessNumber rules, subject to the following limits:

  * Maximum number of colors (defined by `MM_MAX_COLORS`): 10.
  * Maximum number of pegs (defined by `MM_MAX_PEGS`): 6.

## Strategies of the Code Breaker ##

There are three types of strategies for the code breaker:

**Simple strategy**. The code breaker just makes a random guess, as long as the guess will bring some information. A natural choice could be the first codeword from the remaining possibilities. These strategies obviously perform poorly in that it usually takes many steps to find the secret, but they are simple and fast and thus can be used as a benchmark for other more sophisticated strategies.

**Heuristic strategy**. The code breaker evaluates each potential guess according to some scoring criteria, and picks the one that gets the highest score. These strategies are fast enough for real-time games, and performs quite well. Most research in this field focuses on finding a good heuristic criteria, and a number of intuitive and well-performing heuristics have been proposed. For more details, see [heuristic strategies](HeuristicStrategies.md).

**Optimal strategy**. Since the number of possible secrets as well as sequence of guesses are finite, the code breaker can employ a depth-first search to find out the _optimal_ guessing strategy. The optimal strategy minimizes the expected number of guesses, optionally subject to a maximum-guesses constraint. Finding such a strategy involves exhaustive search, and therefore is too slow to be suitable for real-time application. For more details on the implementation, see [optimal strategies](OptimalStrategies.md).

## Program Optimization Techniques ##

While the code-breaking algorithms are quite straightforward, much effort of this project has been put to optimize the performance of a real-time code breaker. Some of the hot-spots are identified by the profiler. The major points of optimization are described below. Most of the optimized routines are implemented in standalone source files for clarity.

<i>Search space pruning</i>. While all codewords are candidates for making a guess, some of them are _equivalent_ in terms of bringing new information. For example, in the first round of a Number Guessing game, any guess works the same. Aware of this, we implement pruning by classifying digits into three classes: <i>impossible</i>, <i>unguessed</i>, and the rest. After each round of feedback, we update the list of <i>distinct</i> guesses (in terms of bringing information) and search within this list only. This reduces the search space significantly.

<i>Codeword comparison</i>. This is the most intensive operation in the program, accounting for 40% of all CPU time. The program uses SSE2 instructions (implemented via compiler intrinsics) to compare each pair of codewords in four instructions.

<i>Feedback frequency counting</i>. The heuristic code breaker relies heavily on these routines to count statistics on partitions. This is an intensive operation which accounts for about 20% of all CPU time. The program uses an ASM implementation to maximize performance. See `Frequency.cpp`.

## Reference ##

[Knuth (1976)](http://www.dcc.fc.up.pt/~sssousa/RM09101.pdf)
published the first paper on Mastermind, where he introduced a heuristic strategy that aimed at minimizing the worst-case number of remaining possibilities.

[Kooi (2005)](http://www.philos.rug.nl/~barteld/master.pdf)
provided a concise yet thorough overview of the heuristic
algorithms to solve the %Mastermind game. He also proposed the %MaxParts
heuristic, which proved to work well for a %Mastermind game with 4 pegs
and 6 colors, but didn't work well for one with 5 pegs and 8 colors.

[Heeffer (2007)](http://logica.ugent.be/albrecht/thesis/Logik.pdf)
tested various heuristic algorithms on the %Mastermind game with 5 pegs
and 8 colors, and found the Entropy method to perform the best.

Not much results seen out there for the Number Guesser game.