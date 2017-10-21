#include "ImgMatchAlgorithm.h"



void ImgMatchAlgorithm::init ( void* )
{
}

void ImgMatchAlgorithm::destroy ()
{
}



AlgorithmResult ImgMatchAlgorithm::process ( void* param )
{
	bool exec_status = true;
	exec_status &= preprocess ( param );
	if ( exec_status) exec_status &= detect ();
	if ( exec_status ) exec_status &= removeOutliers ();
	if ( exec_status ) exec_status &= match ();
	if ( exec_status ) exec_status &= drawMatch ();
	if ( exec_status )
		return AlgorithmResult::AR_DONE;
	else
		return AlgorithmResult::AR_INTERNAL;
}
