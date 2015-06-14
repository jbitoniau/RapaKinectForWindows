#pragma once

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>

namespace RKFW
{

/*
	CriticalSectionEnterer

	A RAII-style object (Resource Acquisition Is Initialization) that facilitates
	the use of CriticalSections

	Information about Windows CriticalSection can be found here:
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms682530(v=vs.85).aspx
*/
class CriticalSectionEnterer
{
public:
	CriticalSectionEnterer( CRITICAL_SECTION& criticalSection );
	~CriticalSectionEnterer();

private:
	CriticalSectionEnterer( CriticalSectionEnterer& other );
	CriticalSectionEnterer& operator=( CriticalSectionEnterer& other );
	
	CRITICAL_SECTION& mCriticalSection;
};

}