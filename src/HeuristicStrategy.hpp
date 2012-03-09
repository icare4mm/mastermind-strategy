#ifndef MASTERMIND_HEURISTIC_STRATEGY_HPP
#define MASTERMIND_HEURISTIC_STRATEGY_HPP

#include "Strategy.hpp"
#include "util/call_counter.hpp"

/**
 * Define FAVOR_POSSIBILITY to 1 to make a heuristic strategy favor
 * a guess that is among the remaining possibilities when multiple
 * guesses have the same heuristic score.
 *
 * Normally, this flag should be set to 0 because we expect the
 * heuristic score to fully capture the available information from
 * the partition. However, setting this to 1 does improve the result
 * marginally in certain cases, e.g. "-r bc -s entropy". Also it
 * may help in compatilibity with legacy implementations that favor
 * a remaining possibility as guess in case of a tie.
 *
 * @ingroup Heuristic
 */
#define FAVOR_POSSIBILITY 0

namespace Mastermind {

/**
 * Strategy that makes the guess that produces the optimal score for
 * a heuristic function.
 *
 * The heuristic strategy makes a guess by computing a score for each
 * candidate guess using a <i>heuristic function</i>. The heuristic
 * function takes as input the partitioning of the possibility set by
 * the guess, and outputs a scalar score. The guess that produces the
 * optimal (lowest) score is chosen. If more than one guesses have
 * the same score, the guess that is in the possibility set is chosen.
 * If multiple choices still exist, the first one is chosen.
 *
 * The heuristic function used by the strategy is specified in the
 * template parameter <code>Heuristic</code>. It takes as input the
 * frequency table of the partition, and returns a heuristic score.
 * The partition is defined as below.
 *
 * Suppose there are <code>N</code> remaining possibilities. Given
 * a guess, these possibilities can be partitioned into <code>m</code>
 * cells, where the possibilities in each cell have the same feedback
 * when compared to the guess. That is, given that guess, there is
 * no way to distinguish one possibility from another in the same
 * cell.
 *
 * Note that when computing the heuristic score, we always assume that
 * each codeword in the possibility set is equally-likely to be the
 * secret.
 *
 * @ingroup Heuristic
 */
template <class Heuristic>
class HeuristicStrategy : public Strategy
{
	Engine &e;
	Heuristic h;

	struct choice_t
	{
		typedef typename Heuristic::score_t score_t;

		int i; // index to the choice, -1 = undefined
		typename Heuristic::score_t score; // score of the choice
#if FAVOR_POSSIBILITY
		bool ispos; // whether the choice is a remaining possibility
		choice_t() : i(-1), score(), ispos(false) { }
		choice_t(int _i, const score_t _score, bool _ispos)
			: i(_i), score(_score), ispos(_ispos) { }
#else
		choice_t() : i(-1), score() { }
		choice_t(int _i, const score_t &_score)
			: i(_i), score(_score) { }
#endif

		bool operator < (const choice_t &other) const
		{
			if (i < 0)
				return false;
			if (other.i < 0)
				return true;
			if (score < other.score)
				return true;
			if (other.score < score)
				return false;
#if FAVOR_POSSIBILITY
			if (!ispos && other.ispos)
				return true;
			if (ispos && !other.ispos)
				return false;
#endif
			return i < other.i;
		}
	};

public:

	typedef typename Heuristic::score_t score_type;

	/// Constructs the strategy.
	HeuristicStrategy(Engine &engine, const Heuristic &heuristic = Heuristic())
		: e(engine), h(heuristic) { }

	Heuristic& heuristic() { return h; }

	/// Returns the name of the strategy.
	virtual std::string name() const
	{
		 return h.name();
	}

#if 0
	/// Evaluates an array of candidates, and stores the heuristic score
	/// of each candidate.
	/// Makes the guess that produces the lowest heuristic score.
	/// Optionally stores the score of all candidates in an array.
	void evaluate(
		CodewordConstRange possibilities,
		CodewordConstRange candidates,
		score_type *scores) const
	{
		REGISTER_CALL_COUNTER(EvaluateHeuristic_Possibilities);
		REGISTER_CALL_COUNTER(EvaluateHeuristic_Candidates);
		UPDATE_CALL_COUNTER(EvaluateHeuristic_Possibilities, (unsigned int)possibilities.size());
		UPDATE_CALL_COUNTER(EvaluateHeuristic_Candidates, (unsigned int)candidates.size());

		if (candidates.empty() || !scores)
			return;

		int n = (int)candidates.size();

		// OpenMP index variable (i) must have signed integer type.
#if _OPENMP
		#pragma omp parallel for schedule(static)
#endif
		for (int i = 0; i < n; ++i)
		{
			// Partition the possibilities.
			// @todo Make the size of freq smaller (redundant elements unnecessary).
			Codeword guess = candidates[i];
			FeedbackFrequencyTable freq = e.frequencies(guess, possibilities);

			// Compute a score of the partition.
			score_type score = h.compute(freq);

			// Store the score.
			scores[i] = score;
		}
	}
#endif

	/// Makes the guess that produces the lowest heuristic score.
	/// Optionally stores the score of all candidates in an array.
	Codeword make_guess(
		CodewordConstRange possibilities,
		CodewordConstRange candidates,
		score_type *scores) const
	{
		REGISTER_CALL_COUNTER(EvaluateHeuristic_Possibilities);
		REGISTER_CALL_COUNTER(EvaluateHeuristic_Candidates);
		UPDATE_CALL_COUNTER(EvaluateHeuristic_Possibilities, (unsigned int)possibilities.size());
		UPDATE_CALL_COUNTER(EvaluateHeuristic_Candidates, (unsigned int)candidates.size());

		if (candidates.empty())
			return Codeword();

#if 0
		static int debug_i = 0;
		extern int estimate_obvious_lowerbound(
			const Rules &rules,
			CodewordConstRange possibilities);

		// Pure debug output: when there are a few possibilities left,
		// what are the typical structure of these remaining possibilities?
		if (possibilities.size() < 14 && rand() % 750 == 0)
		{
			std::cout << '[' << (++debug_i) << "] ";
			for (size_t j = 0; j < possibilities.size(); ++j)
			{
				std::cout << possibilities[j] << ' ';
			}

			// Find the best possibility.
			score_type best = score_type();
			for (size_t i = 0; i < possibilities.size(); ++i)
			{
				Codeword guess = possibilities[i];
				FeedbackFrequencyTable freq = e.frequencies(guess, possibilities);
				score_type score = h.compute(freq);
				if (i == 0 || score < best)
				{
					best = score;
				}
			}
			std::cout << "(" << best << " v ";

			// Find the best in all.
			for (size_t i = 0; i < candidates.size(); ++i)
			{
				Codeword guess = candidates[i];
				FeedbackFrequencyTable freq = e.frequencies(guess, possibilities);
				score_type score = h.compute(freq);
				if (i == 0 || score < best)
				{
					best = score;
				}
			}
			std::cout << best;
			std::cout << " : " << estimate_obvious_lowerbound(e.rules(), possibilities);
			std::cout << ")";
			std::cout << std::endl;
		}
#endif

#if _OPENMP
		choice_t global_choice;
#else
		choice_t choice;
#endif

#if FAVOR_POSSIBILITY
		size_t target = Feedback::perfectValue(e.rules()).value();
#endif

		// Evaluate each candidate guess and find the one that
		// produces the lowest score.
		int n = (int)candidates.size();

		// OpenMP index variable (i) must have signed integer type.
#if _OPENMP
		#pragma omp parallel if (0)
		{
			choice_t choice;

			#pragma omp for schedule(static)
#endif
			for (int i = 0; i < n; ++i)
			{
				Codeword guess = candidates[i];
				FeedbackFrequencyTable freq = e.compare(guess, possibilities);

				// Compute a score of the partition.
				score_type score = h.compute(freq);

				// Store the score if requested.
				if (scores)
					scores[i] = score;

				// Keep track of the guess that produces the lowest score.
#if FAVOR_POSSIBILITY
				choice_t current(i, score, freq[target] > 0);
#else
				choice_t current(i, score);
#endif
				choice = std::min(choice, current);
			}
#if _OPENMP
			#pragma omp critical
			{
				global_choice = std::min(global_choice, choice);
			}
		}
		return candidates[global_choice.i];
#else
		return candidates[choice.i];
#endif
	}

	virtual Codeword make_guess(
		CodewordConstRange possibilities,
		CodewordConstRange candidates) const
	{
		return make_guess(possibilities, candidates, NULL);
	}
};

} // namespace Mastermind

#endif // MASTERMIND_HEURISTIC_STRATEGY_HPP