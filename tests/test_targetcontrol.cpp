#include "TargetControl.hpp"

using namespace cnbi;

int main(int argc, char** argv) {


	unsigned int ntargets = 2;
	unsigned int nobjects = 20;
	unsigned int nrepetitions = 30;

	cvsa::TargetControl tcontrol(ntargets, nobjects, nrepetitions);

	printf("[test_tcontrol] - Combinations:\n");
	for(auto i = 0; i < tcontrol.Size(); i++) {
		printf("[%d] %u %u\n", i, tcontrol.At(0), tcontrol.At(1));	
		tcontrol.Next();
	}


	return 0;
}
