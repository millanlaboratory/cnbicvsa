#ifndef CNBICVSA_TARGETCONTROL_HPP
#define CNBICVSA_TARGETCONTROL_HPP

#include <algorithm>
#include <array>
#include <random>
#include <string>
#include <vector>
#include <cmath>

namespace cnbi {
    namespace cvsa {

typedef std::vector<unsigned int> Entry;
typedef std::vector<Entry> List;


class TargetControl {

	public:
		TargetControl(unsigned int ntargets, unsigned int nobjects, unsigned int nrepetitions);
		virtual ~TargetControl(void);


	public:
		List nchoosek(unsigned int n, unsigned int k);
		List replicate(List src, unsigned int n);

	private:
		size_t	ntargets_;
		size_t	nobjects_;
		size_t	nrepetitions_;

		List list_;

};

	}
}

#endif
