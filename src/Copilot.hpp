#ifndef CNBICVSA_COPILOT_HPP
#define CNBICVSA_COPILOT_HPP

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

namespace cnbi {
    namespace cvsa {

typedef std::vector<unsigned int>	Trials;
typedef std::vector<unsigned int>	Catch;
typedef Trials::iterator			TrialsIt;

class Copilot {
	
	public:
		Copilot(void);
		virtual ~Copilot(void);

		bool Add(unsigned int id, unsigned int nrepetitions);
		
		void Generate(float ncatch = 0.0);
		unsigned int GetId(void);
		TrialsIt GetIt(void);
		bool IsCatch(void) {};

		TrialsIt Begin(void);
		TrialsIt End(void);
		bool Next(void);
		bool Previous(void);
		void Dump(void);

		unsigned int GetPosition(void);
		unsigned int GetSize(void);

	private:
		std::unordered_map<unsigned int, unsigned int>	map_;
		Trials		sequence_trial_;
		Catch		sequence_catch_;
		TrialsIt	sequence_it_;

		unsigned int nrepetitions_;
		float ncatch_;
};

	}
}

#endif
