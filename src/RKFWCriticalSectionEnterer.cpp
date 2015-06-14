#include "RKFWCriticalSectionEnterer.h"

#include <assert.h>

namespace RKFW
{

CriticalSectionEnterer::CriticalSectionEnterer( CRITICAL_SECTION& criticalSection )
	: mCriticalSection( criticalSection )
{
	EnterCriticalSection( &mCriticalSection );
}	

CriticalSectionEnterer::~CriticalSectionEnterer()
{
	LeaveCriticalSection( &mCriticalSection );
}

}
