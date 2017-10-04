#ifndef CNBICVSA_TARGETCONTROL_HPP
#define CNBICVSA_TARGETCONTROL_HPP

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

namespace cnbi {
    namespace cvsa {

typedef std::vector<unsigned int>	Trials;
typedef Trials::iterator		TrialsIt;

class TrialControl {
	
	public:
		TrialControl(void);
		virtual ~TrialControl(void);

		bool Add(unsigned int id, unsigned int nrepetitions);
		
		void Generate(void);
		unsigned int GetId(void);
		TrialsIt GetIt(void);

		TrialsIt Begin(void);
		TrialsIt End(void);
		bool Next(void);
		bool Previous(void);

	private:
		std::unordered_map<unsigned int, unsigned int>	map_;
		Trials		sequence_;
		TrialsIt	sequence_it_;

		unsigned int nrepetitions_;
};

	}
}

#endif
