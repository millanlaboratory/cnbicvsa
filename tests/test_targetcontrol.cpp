#include "TargetControl.hpp"

using namespace cnbi;

int main(int argc, char** argv) {


	unsigned int ntargets = 2;
	unsigned int nobjects = 20;
	unsigned int nrepetitions = 30;

	cvsa::TargetControl tcontrol(ntargets, nobjects, nrepetitions);
	//cvsa::List l;

	//l = tcontrol.nchoosek(nobjects, ntargets);

	//for(auto it1 = l.begin(); it1 != l.end(); ++it1) {
	//	for(auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
	//		printf(" %u", (*it2));
	//	}
	//	printf("\n");
	//}



	return 0;
}
