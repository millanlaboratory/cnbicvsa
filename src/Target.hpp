#ifndef CNBICVSA_TARGET_HPP
#define CNBICVSA_TARGET_HPP

#include <algorithm>
#include <array>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <locale>
#include <fstream>
#include <iomanip>
#include <cnbidraw/Gallery.hpp>

namespace cnbi {
    namespace cvsa {

typedef std::vector<unsigned int> Entry;
typedef std::vector<Entry> Sequence;
typedef std::vector<Entry>::iterator SequenceIt;

typedef std::vector<std::shared_ptr<draw::Gallery>> List;
typedef std::vector<std::shared_ptr<draw::Gallery>>::iterator ListIt;

typedef std::array<float, 2> Position;

class Target {

	public:
		Target(const std::string& folder, const std::vector<std::string> ext = {".png"});
		virtual ~Target(void);


		draw::Gallery* Add(float width, float height, float angle, float radius); 	

		void Generate(unsigned int nrepetitions);

		void SetTime(float time);
		void Show(void);
		void Hide(void);
		void Hit(unsigned int id, const float* color);
		bool ToCenter(unsigned int id);	
		void Reset(void);
		float WaitRandom(float maxtime, float mintime = 0.0f);

		bool Export(const std::string& dirname = "./");

		bool Next(void);
		bool Previous(void);

		SequenceIt Begin(void);
		SequenceIt End(void);

	private:
		Sequence nchoosek(unsigned int n, unsigned int k);
		Sequence replicate(Sequence src, unsigned int n);
		Position tocartesian(float angle, float radius);

	private:
		std::string				 folder_;
		std::vector<std::string> extensions_;	
		unsigned int	ntargets_;
		unsigned int	nobjects_;
		unsigned int	nrepetitions_;

		Sequence	sequence_;
		SequenceIt	sequence_it_;

		List		list_;
		ListIt		list_it_;	
		std::vector<Position> positions_;

		bool		ismoving_;
		CcTimeValue timer_;
		float		time_;
		float		thick_;
};

	}
}

#endif
