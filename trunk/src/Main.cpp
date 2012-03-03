/**
 * @defgroup type Data Types
 *
 * @defgroup algo Basic Operations
 *
 * @defgroup equiv Equivalence Filters
 *
 * @defgroup strat Strategies
 *
 * @defgroup prog Program
 *
 * @defgroup util Utility Routines
 */

/// @defgroup Obvious Obvious Strategy
/// A strategy that makes an obviously-optimal guess when one exists.
/// @ingroup strat

/// @defgroup Simple Simple Strategy
/// A strategy that makes an arbitrary guess from the set of remaining
/// secrets.
/// @ingroup strat

/// @defgroup Heuristic Heuristic Strategy
/// A strategy that makes the guess that produces the best heuristic
/// score.
/// @ingroup strat

/// @defgroup Optimal Optimal Strategy
/// A strategy that makes the optimal guess through exhaustive search.
/// @ingroup strat

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#ifdef _OPENMP
#include <omp.h>
#endif

#include <iostream>
#include "Rules.hpp"
#include "Codeword.hpp"
#include "Equivalence.hpp"
#include "SimpleStrategy.hpp"
#include "HeuristicStrategy.hpp"
#include "OptimalStrategy.hpp"
#include "CodeBreaker.hpp"
#include "util/io_format.hpp"
#include "util/hr_timer.hpp"

using namespace Mastermind;

#if 0

// c.f. http://www.javaworld.com.tw/jute/post/view?bid=35&id=138372&sty=1&tpg=1&ppg=1&age=0#138372

static int GetMaxBreakableWithin(
	int nsteps,
	int f[],
	CodewordList possibilities,
	CodewordList all,
	unsigned short unguessed_mask,
	int expand_levels)
{
	assert(nsteps >= 0);
	assert(expand_levels >= 0);

	if (nsteps == 0) {
		return 0;
	} else if (nsteps == 1) {
		return 1;
	}
	//else if (nsteps == 2) {
	//	int npegs = possibilities.GetRules().length;
	//	return std::min(possibilities.GetCount(), npegs*(npegs+3)/2);
	//}

	unsigned short impossible_mask = ((1<<all.GetRules().ndigits)-1) & ~possibilities.GetDigitMask();
	CodewordList candidates = all.FilterByEquivalence(unguessed_mask, impossible_mask);
	int best = 0;
	for (int i = 0; i < candidates.GetCount(); i++) {
		Codeword guess = candidates[i];

		int nguessable = 0;
		if (expand_levels == 0) {
			FeedbackFrequencyTable freq(FeedbackList(guess, possibilities));
			for (int fbv = 0; fbv <= freq.GetMaxFeedbackValue(); fbv++) {
				int partition_size = freq[Feedback(fbv)];
				nguessable += std::min(f[nsteps-1], partition_size);
			}
		} else {
			FeedbackFrequencyTable freq;
			possibilities.Partition(guess, freq);
			int partition_start = 0;
			for (int fbv = 0; fbv <= freq.GetMaxFeedbackValue(); fbv++) {
				int partition_size = freq[Feedback(fbv)];
				if (partition_size == 0)
					continue;

				CodewordList filtered(possibilities, partition_start, partition_size);
				int tmpcount = GetMaxBreakableWithin(nsteps-1, f, filtered, all,
					unguessed_mask & ~guess.GetDigitMask(),
					expand_levels-1);
				nguessable += std::min(tmpcount, partition_size);
				partition_start += partition_size;
			}
		}

		best = std::max(best, nguessable);
	}
	return best;
}

static int TestBound(Rules rules)
{
	// Find out the maximum size of any partition
	CodewordList all = CodewordList::Enumerate(rules);
	FeedbackFrequencyTable freq(FeedbackList(all[0], all));

	int f[100];
	f[0] = 0;
	f[1] = 1;
	int min_rounds = 0;
	int expand_levels = 1;
	for (int n = 2; n < 100; n++) {
		unsigned short unguessed_mask = ((1<<all.GetRules().ndigits)-1);
		f[n] = GetMaxBreakableWithin(n, f, all.Copy(), all, unguessed_mask, expand_levels);
		if (f[n] >= all.GetCount()) {
			min_rounds = n;
			break;
		}
	}
	for (int n = 1; n <= min_rounds; n++) {
		printf("Maximum number of secrets that can be guessed in %2d rounds: <= %d\n",
			n, f[n]);
	}
	printf("**** Minimum number of rounds necessary to guess all secrets: >= %d\n",
		min_rounds);

	int min_steps = all.GetCount()*min_rounds;
	for (int n = 1; n < min_rounds; n++) {
		min_steps -= f[n];
	}
	printf("**** Minimum total steps necessary to guess all secrets: >= %d\n",
		min_steps);
	printf("**** Minimum average steps necessary to guess all secrets: >= %5.3f\n",
		(double)min_steps/all.GetCount());

	int *S = Mastermind::Heuristics::MinimizeSteps::partition_score;
	S[0] = 0;
	for (int i = 1; i <= all.GetCount(); i++) {
		int nr = 0;
		for (nr = 1; nr <= 100; nr++) {
			if (f[nr] >= i)
				break;
		}
		int score = i*nr;
		for (int j = 1; j < nr; j++) {
			score -= f[j];
		}
		S[i] = score;
		//if (i % 40==0)
		//printf("S[%d] = %d\n", i, score);
	}

#if 0
	int m=14;

	const int nmax=20;
	int S[nmax+1];
	S[0]=0;
	int n;
	for (n = 1; n <= nmax; n++) {
		if (n <= m) {
			S[n] = 1+(n-1)*2;
		} else {
			S[n]=n;
			int nper = (n-1)/(m-1);
			S[n] += S[nper]*(m-2);
			S[n] += S[(n-1)-(nper*(m-2))];
		}
		printf("S[%4d] = %d\n", n, S[n]);
	}
#endif

	//system("PAUSE");
	return 0;
}
#endif

static void usage()
{
	std::cerr <<
		"Mastermind [options] [-r rules] [-e equiv-filter] mode\n"
		"Build a Mastermind strategy and output the strategy tree.\n"
		"Version 0.5 (2012). Configured with max " << MM_MAX_PEGS << " pegs and "
		<< MM_MAX_COLORS << " colors.\n"
		"Modes:\n"
		"    -d          interactive debugger\n"
		"    -p [secret] interactive player (optionally) using the given secret\n"
		"    -s strat    build strategy 'strat' and output strategy tree\n"
		"    -t          run tests\n"
		"Options:\n"
		"    -h          display this help screen and exit\n"
		"    -md [depth] limit the maximum number of guesses to reveal any secret\n"
#ifdef _OPENMP
		"    -mt [n]     enable parallel execution with n threads [default="
		<< omp_get_max_threads() << "]\n"
#endif
		"    -po         make guess from remaining possibilities only\n"
		//"    -q          display minimal information\n"
		"    -v,-vv      verbose mode; extra v for even more verbose\n"
		"Rules: 'p' pegs 'c' colors ['r'|'n']\n"
		"    mm,p4c6r    [default] Mastermind (4 pegs, 6 colors, with repetition)\n"
		"    bc,p4c10n   Bulls and Cows (4 pegs, 10 colors, no repetition)\n"
		"    lg,p5c8r    Logik (5 pegs, 8 colors, with repetition)\n"
		// @todo descriptions for heuristic strategies 
		// @todo -file switch to process a strategy tree file directly
		"Strategies: (~ indicates no favor of remaining possibility as guess)\n"
		"    file path   read strategy from 'path'; use - for STDIN\n"
		"    simple      simple strategy\n"
		"    minmax      min-max heuristic strategy\n"
		"    minavg[~]   min-average heuristic strategy\n"
		"    entropy[~]  max-entropy heuristic strategy\n"
		"    parts[~]    max-parts heuristic strategy\n"
		"    minlb       min-lowerbound heuristic strategy\n"
		"    optimal[+]  optimal strategy that minimizes the total number of steps\n"
		"                + : in case of a tie, minimizes the worst-case depth\n"
		//"    optimal++   in addition to 'optimal+', minimizes the number of secrets\n"
		//"                revealed using the worst-case number of steps\n"
		"Equivalence filters:\n"
		"    default     composite filter (color + constraint)\n"
		"    color       filter by color equivalence\n"
		"    constraint  filter by constraint equivalence\n"
		"    none        do not apply any filter\n"
		"";
}

// TODO: Refactor MakeGuess() code to make each call longer and fewer calls
//       to take advantage of OpenMP.
// TODO: Add progress display to OptimalCodeBreaker
// TODO: Output strategy tree after finishing a run

// extern int strategy(std::string strat);
extern int interactive_player(Engine & e, int verbose, Codeword secret);
extern int interactive_analyst(Engine & e, int verbose);
extern int test(const Rules &rules, bool verbose);

extern void pause_output();

#define USAGE_ERROR(msg) do { \
		std::cerr << "Error: " << msg << ". Type -h for help." << std::endl; \
		return 1; \
	} while (0)

#define USAGE_REQUIRE(cond,msg) do { \
		if (!(cond)) USAGE_ERROR(msg); \
	} while (0)

static int build_heuristic_strategy_tree(
	Engine &e, const EquivalenceFilter *filter, int /* verbose */,
	const std::string &name, bool pos_only, StrategyTree &tree)
{
	using namespace Mastermind::Heuristics;

	Strategy *strat = NULL;
	if (name == "simple")
		strat = new SimpleStrategy(e);
	else if (name == "minmax")
		strat = new HeuristicStrategy<MinimizeWorstCase<1>>(e);
	else if (name == "minavg~")
		strat = new HeuristicStrategy<MinimizeAverage>(e, MinimizeAverage(false));
	else if (name == "minavg")
		strat = new HeuristicStrategy<MinimizeAverage>(e, MinimizeAverage(true));
	else if (name == "entropy~")
		strat = new HeuristicStrategy<MaximizeEntropy>(e, MaximizeEntropy(false));
	else if (name == "entropy")
		strat = new HeuristicStrategy<MaximizeEntropy>(e, MaximizeEntropy(true));
	else if (name == "parts~")
		strat = new HeuristicStrategy<MaximizePartitions>(e, MaximizePartitions(false));
	else if (name == "parts")
		strat = new HeuristicStrategy<MaximizePartitions>(e, MaximizePartitions(true));
	else if (name == "minlb")
		strat = new HeuristicStrategy<MinimizeLowerBound>(e, MinimizeLowerBound(e));
	else
		USAGE_ERROR("unknown strategy: " << name);

	CodeBreakerOptions options;
	options.optimize_obvious = true;
	options.possibility_only = pos_only;
	std::unique_ptr<EquivalenceFilter> copy(filter->clone());
	tree = BuildStrategyTree(e, strat, copy.get(), options);
	return 0;
}

extern StrategyTree build_optimal_strategy_tree(
	Engine &e, bool min_depth = false, int max_depth = 100);

// verbose: 0 = quiet, 1 = verbose, 2 = very verbose
static int build_strategy(
	Engine &e, const EquivalenceFilter *filter, int verbose,
	const std::string &name, const std::string &file,
	int max_depth, bool pos_only)
{
	using namespace Mastermind::Heuristics;

	StrategyTree tree(e.rules());
	util::hr_timer timer;

	timer.start();
	if (name == "file")
	{
		USAGE_ERROR("Not implemented");
	}
	else if (name == "optimal")
	{
		tree = build_optimal_strategy_tree(e, false, max_depth);
	}
	else if (name == "optimal+")
	{
		tree = build_optimal_strategy_tree(e, true, max_depth);
	}
	else
	{
		int ret = build_heuristic_strategy_tree(e, filter, verbose, name, pos_only, tree);
		if (ret)
			return ret;
	}
	double t = timer.stop();

	StrategyTreeInfo info(name, tree, t);
	if (verbose)
	{
		std::cout << util::header;
	}
	std::cout << info;

	// Display debug info if required.
	if (verbose >= 2)
	{
		std::cout << "Call statistics for EvaluateHeuristic_Possibilities:" << std::endl;
		std::cout << util::call_counter::get("EvaluateHeuristic_Possibilities") << std::endl;
		std::cout << "Call statistics for EvaluateHeuristic_Candidates:" << std::endl;
		std::cout << util::call_counter::get("EvaluateHeuristic_Candidates") << std::endl;
		std::cout << "Call statistics for ComputeLowerBound_Steps:" << std::endl;
		std::cout << util::call_counter::get("ComputeLowerBound_Steps") << std::endl;
		std::cout << "Call statistics for ComputeLowerBound_Depth:" << std::endl;
		std::cout << util::call_counter::get("ComputeLowerBound_Depth") << std::endl;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	using namespace Mastermind;

#if 0
	Rules rules(4, 3, true);
#elif 0
	Rules rules(4, 10, false);
#else
	Rules rules(4, 6, true);
#endif

	int verbose = 0;
	enum Mode
	{
		DefaultMode = 0,
		StrategyMode = 1,
		PlayerMode = 2,
		DebugMode = 3,
		TestMode = 4,
	} mode = DefaultMode;
	std::string strat_name, strat_file, filter_name;
	Codeword secret;
#ifdef _OPENMP
	int mt = 1;
#endif
	int max_depth = 1000;
	bool pos_only = false;

	// Parse command line arguments.
	for (int i = 1; i < argc; i++)
	{
		std::string s = argv[i];
		if (s == "-d")
		{
			USAGE_REQUIRE(mode == DefaultMode, "only one mode may be specified");
			mode = DebugMode;
		}
		else if (s == "-p")
		{
			USAGE_REQUIRE(mode == DefaultMode, "only one mode may be specified");
			mode = PlayerMode;
			if (i+1 < argc && argv[i+1][0] != '-')
			{
				std::string arg(argv[++i]);
				USAGE_REQUIRE((std::istringstream(arg) >> secret) && 
					secret.valid(rules), "expecting secret after -p");
			}
		}
		else if (s == "-s")
		{
			USAGE_REQUIRE(mode == DefaultMode, "only one mode may be specified");
			USAGE_REQUIRE(++i < argc, "missing argument for option -s");
			mode = StrategyMode;
			strat_name = argv[i];
			if (strat_name == "file")
			{
				USAGE_REQUIRE(++i < argc, "missing input filename for file strategy");
				strat_file = argv[i];
			}
		}
		else if (s == "-t")
		{
			USAGE_REQUIRE(mode == DefaultMode, "only one mode may be specified");
			mode = TestMode;
		}
		else if (s == "-e")
		{
			USAGE_REQUIRE(filter_name.empty(), "only one equivalence filter may be specified");
			USAGE_REQUIRE(++i < argc, "missing argument for option -f");
			filter_name = argv[i];
		}
		else if (s == "-h")
		{
			usage();
			return 0;
		}
		else if (s == "-r")
		{
			USAGE_REQUIRE(++i < argc, "missing argument for option -r");
			USAGE_REQUIRE(secret.empty(), "-r rules must be specified before -p secret");
			std::string name = argv[i];
			if (name == "mm")
				rules = Rules(4, 6, true);
			else if (name == "bc")
				rules = Rules(4, 10, false);
			else if (name == "lg")
				rules = Rules(5, 8, true);
			else
				rules = Rules(name.c_str());
			USAGE_REQUIRE(rules.valid(), "invalid rules: " << argv[i]);
		}
		else if (s == "-md")
		{
			USAGE_REQUIRE(++i < argc, "missing argument for option -md");
			std::string cnt(argv[i]);
			USAGE_REQUIRE((std::istringstream(cnt) >> max_depth) && (max_depth > 0),
				"positive integer argument expected for option -md");
		}
		else if (s == "-po")
		{
			pos_only = true;
		}
		else if (s == "-mt")
		{
			int n = -1;
			if (i+1 < argc && argv[i+1][0] != '-')
			{
				std::string cnt(argv[++i]);
				USAGE_REQUIRE((std::istringstream(cnt) >> n) && (n > 0),
					"positive integer argument expected for option -mt");
			}
#ifdef _OPENMP
			if (n < 0)
			{
				mt = omp_get_max_threads();
			}
			else
			{
				if (n > omp_get_max_threads())
				{
					std::cerr << "Warning: number of threads set to maximum value "
						<< omp_get_max_threads() << std::endl;
					n = omp_get_max_threads();
				}
				mt = n;
			}
#else
			std::cerr << "Warning: option -mt is not supported by this build"
				" and is ignored." << std::endl;
#endif
		}
		else if (s == "-v")
		{
			verbose = 1;
		}
		else if (s == "-vv")
		{
			verbose = 2;
		}
		else
		{
			USAGE_REQUIRE(false, "unknown option: " << s);
		}
	}

	// Set number of threads.
#ifdef _OPENMP
	omp_set_num_threads(mt);
#endif

	// Create an algorithm engine.
	Engine e(rules);

	// Create the specified equivalence filter.
	EquivalenceFilter *filter = NULL;
	if (filter_name == "default" || filter_name == "")
	{
		filter = new CompositeEquivalenceFilter(
			RoutineRegistry<CreateEquivalenceFilterRoutine>::get("Color")(e),
			RoutineRegistry<CreateEquivalenceFilterRoutine>::get("Constraint")(e));
	}
	else if (filter_name == "color")
	{
		filter = RoutineRegistry<CreateEquivalenceFilterRoutine>::get("Color")(e);
	}
	else if (filter_name == "constraint")
	{
		filter = RoutineRegistry<CreateEquivalenceFilterRoutine>::get("Constraint")(e);
	}
	else if (filter_name == "none")
	{
		filter = RoutineRegistry<CreateEquivalenceFilterRoutine>::get("Dummy")(e);
	}
	else
	{
		USAGE_ERROR("unknown equivalence filter: " << filter_name);
	}
	std::unique_ptr<EquivalenceFilter> filter_obj(std::move(filter));

	// Execute the specified action.
	switch (mode)
	{
	case StrategyMode:
		return build_strategy(e, filter, verbose, strat_name, strat_file,
			max_depth, pos_only);
	case PlayerMode:
		return interactive_player(e, verbose, secret);
	case DebugMode:
		return interactive_analyst(e, verbose);
	case TestMode:
		return test(rules, verbose);
	default:
		USAGE_ERROR("missing mode");
	}
}
