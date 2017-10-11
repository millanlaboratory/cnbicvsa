#ifndef CNBICVSA_COPILOT_HPP
#define CNBICVSA_COPILOT_HPP

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>
#include <cmath>

namespace cnbi {
    namespace cvsa {

typedef std::vector<unsigned int>	Trials;
typedef std::vector<unsigned int>	Catch;
typedef Trials::iterator			TrialsIt;
typedef Catch::iterator				CatchIt;

class Copilot {
	
	public:
		Copilot(void);
		virtual ~Copilot(void);

		bool Add(unsigned int id, unsigned int ntrials, float ncatch = 0.0f);
		
		void Generate(void);
		unsigned int GetId(void);
		TrialsIt GetIt(void);
		bool IsCatch(void);
		unsigned int GetCatchId(void);

		TrialsIt Begin(void);
		TrialsIt End(void);
		bool Next(void);
		bool Previous(void);
		void Dump(void);

		unsigned int GetPosition(void);
		unsigned int GetSize(void);

	private:
		std::unordered_map<unsigned int, unsigned int>	map_trial_;
		std::unordered_map<unsigned int, float>  map_catch_;

		Trials		sequence_trial_;
		Catch		sequence_catch_;
		TrialsIt	sequence_trial_it_;
		CatchIt		sequence_catch_it_;

		unsigned int ntrials_;
};

	}
}

#endif
