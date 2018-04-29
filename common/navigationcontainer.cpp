#include "stdafx.h"
#include "navigationcontainer.h"
#include "control.h"


NavigationContainer::NavigationContainer(bool allowNoFocused) :
	Container       (),
	mFocusedIndex   (-1),
    mAllowNoFocused (allowNoFocused) {}

// *************************************************
//
// *************************************************

NavigationContainer::~NavigationContainer()
{
}

// *************************************************
//
// *************************************************

void NavigationContainer::Update(float deltaTime)
{
	Container::Update(deltaTime);
	if (!mAllowNoFocused && !mControls.empty() && mFocusedIndex < 0) FocusNextControl();
}

// *************************************************
//
// *************************************************

Control * NavigationContainer::GetFocusedControl()
{
	if (mControls.empty() || mFocusedIndex < 0) return nullptr;

	return mControls[mFocusedIndex];
}

// *************************************************
//
// *************************************************

void NavigationContainer::FocusNextControl() 
{
	if (!mControls.empty()) 
	{
		mFocusedIndex++;
		int numControls = static_cast<int>(mControls.size());
		if (mFocusedIndex < 0 || mFocusedIndex >= numControls) mFocusedIndex = 0;
	
		for (int i = 0; i < numControls; ++i)
		{
			if (i == mFocusedIndex) 
				mControls[i]->SetFocused(true);
			else                    
				mControls[i]->SetFocused(false);
		}
	}
}

// *************************************************
//
// *************************************************

void NavigationContainer::FocusPreviousControl() 
{
	if (!mControls.empty()) 
	{
		mFocusedIndex--;
		int numControls = static_cast<int>(mControls.size());
		if (mFocusedIndex < 0) mFocusedIndex = numControls - 1;

		for (int i = 0; i < numControls; ++i)
		{
			if (i == mFocusedIndex) mControls[i]->SetFocused(true);
			else                    mControls[i]->SetFocused(false);
		}
	}
}

// *************************************************
//
// *************************************************

void NavigationContainer::ResetFocus() 
{
	mFocusedIndex = -1;
}