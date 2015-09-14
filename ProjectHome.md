This project demonstrates several state-of-the-art strategies to solve the Mastermind puzzle and its variant, Bulls and Cows. Detailed analysis is provided along with an optimized program solver.

## Introduction ##

Mastermind is a two-player game in which one player, the code-maker thinks of a secret codeword, and the other player, the code-breaker makes successive guesses in an attempt to reveal the secret. For each guess, the code-maker responds with a hint of how close the guess is to the secret. Details of the rules can be found [here](http://en.wikipedia.org/wiki/Mastermind_(board_game)).

The standard Mastermind game is played using four pegs and six colors. A codeword can contain the same color more than once. A popular variant is Bulls and Cows, where ten colors are used but no repetition of color is allowed in a codeword. Details of Bulls and Cows can be found [here](http://en.wikipedia.org/wiki/Bulls_and_cows).

The objective of this project is to demonstrate state-of-the-art strategies for the code-breaker, i.e. how to find out the secret as quickly as possible. Both Mastermind rules and Bulls and Cows rules are covered.

## Analysis ##

A detailed analysis (30+ pages) of the strategies for the code-breaker is under progress. The LaTeX source for this document is under the `doc/` directory and can be browsed [here](http://code.google.com/p/mastermind-strategy/source/browse/#svn/trunk/doc). Four main types of strategies are covered:

_Simple strategy_. Makes whatever guess that makes sense.

_Heuristic strategy_. Makes the heuristically best guess.

_Optimal strategy_. Makes the guess that are proven to be best.

_Randomized strategy_. Makes a random but "good" guess.

## Results ##

If you just want to know what the strategies are (rather than how to find them), you can browse the [strats](http://code.google.com/p/mastermind-strategy/source/browse/#svn/trunk/strats) directory which contains instructions for each strategy analyzed.

## Program ##

A console program is implemented in C++ to carry out the analysis explained above. The source code is under the `src/` directory and can be browsed [here](http://code.google.com/p/mastermind-strategy/source/browse/#svn/trunk/src).

Detailed instructions for building the program from source is available [here](Building.md).

## License ##

The source code and documentation is distributed under MIT License.

The nice icon is adapted from [here](http://commons.wikimedia.org/wiki/File:Aufgabensammlung_-_Symbol_-_Beweis_(2_von_4).svg) and is distributed under the [Creative Commons Attribution-Share Alike 3.0 Unported](http://creativecommons.org/licenses/by-sa/3.0/deed.en) license.