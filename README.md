This project demonstrates several state-of-the-art strategies to solve the Mastermind puzzle and its variant, Bulls and Cows.
Detailed analysis is provided along with an optimized program solver. Me (icare4mm) copied this software collection from code.google.com in
September 2015 to offer people the possibility to contribute to this project.

Introduction
-------------
Mastermind is a two-player game in which one player, the code-maker thinks of a secret codeword, and the other player, the code-breaker makes successive guesses in an attempt to reveal the secret. For each guess, the code-maker responds with a hint of how close the guess is to the secret.

The standard Mastermind game is played using four pegs and six colors. A codeword can contain the same color more than once. A popular variant is Bulls and Cows, where ten colors are used but no repetition of color is allowed in a codeword.

The objective of this project is to demonstrate state-of-the-art strategies for the code-breaker, i.e. how to find out the secret as quickly as possible. Both Mastermind rules and Bulls and Cows rules are covered.

Analysis
---------
A detailed analysis (30+ pages) of the strategies for the code-breaker is under progress. The LaTeX source for this document is under the doc/ directory. Four main types of strategies are covered:

Simple strategy. Makes whatever guess that makes sense.

Heuristic strategy. Makes the heuristically best guess.

Optimal strategy. Makes the guess that are proven to be best.

Randomized strategy. Makes a random but "good" guess.

Results
--------
If you just want to know what the strategies are (rather than how to find them), you can browse the strats/ directory which contains instructions for each strategy analyzed.

Program
--------
A console program is implemented in C++ to carry out the analysis explained above. The source code is under the src/ directory.

The source code conforms to the C++11 standard and makes use of Intel SSE2 instructions (through compiler intrinsics) to optimize performance. It does not rely on any external libraries (except the STL). To build the program, you need a C++11 compliant compiler with Intel intrinsics support. Currently this means either gcc 4.6 under Linux, or Visual C++ 11 under Windows (Visual C++ 2010 also works if you comment out the chrono header). A Code::Blocks project file and a VCX project file is provided.
