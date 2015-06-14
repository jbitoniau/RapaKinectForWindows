#pragma once

#include <Unknwn.h>

namespace RKFW
{

/*
	COMObjectSharedPtr

	A RAII-style object (Resource Acquisition Is Initialization) that facilitates
	the add-ref'ing and release of COM IUnknown-based objects. 
*/
template<class T>
class COMObjectSharedPtr
{
public:
	COMObjectSharedPtr()
		: mUnknown( NULL )
	{
	}
	
	COMObjectSharedPtr( T* unknown )
		: mUnknown( unknown )
	{
	}

	~COMObjectSharedPtr()
	{
		reset();
	}

	void reset()
	{
		if ( mUnknown )
			mUnknown->Release();
		mUnknown = NULL;
	}

	T* get()
	{
		return mUnknown;
	}

	const T* get() const
	{
		return mUnknown;
	}

	T* operator->()
	{
		return mUnknown;
	}

	T* operator->() const
	{
		return mUnknown;
	}

	// Copy-constructor
	COMObjectSharedPtr( const COMObjectSharedPtr& other ) 
	{	
		// Increment the other's IUnknown object reference counter
		if ( other.mUnknown )
			other.mUnknown->AddRef();
			
		// Assign the other into this one
		mUnknown = other.mUnknown;
	}
	
	// Assignment operator
	COMObjectSharedPtr& operator=( const COMObjectSharedPtr& other ) 
	{
		if ( &other==this )
			return *this;

		// Increment the other's IUnknown object reference counter
		if ( other.mUnknown )
			other.mUnknown->AddRef();
			
		// Decrement this IUnknown object reference counter
		if ( mUnknown )
			mUnknown->Release();
					
		// Assign the other into this one
		mUnknown = other.mUnknown;

		return *this;
	}
	
private:
	T* mUnknown;
};

}