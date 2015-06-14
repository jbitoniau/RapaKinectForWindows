#include "RKFWSensorManager.h"

#include "RKFWSensorManagerInternals.h"

namespace RKFW
{

SensorManager::SensorManager()
	: mInternals(NULL)
{
	mInternals = new SensorManagerInternals(this);
}

SensorManager::~SensorManager()
{
	delete mInternals;
	mInternals = NULL;
}

void SensorManager::update()
{
	mInternals->update();
}

const Sensors& SensorManager::getSensors() const
{
	return mInternals->getSensors();
}

void SensorManager::addListener( Listener* listener )
{
	mInternals->addListener(listener);
}

bool SensorManager::removeListener( Listener* listener )
{
	return mInternals->removeListener(listener);
}

}