#include "navigationcontainer.h"
#include "control.h"


NavigationContainer::NavigationContainer() :
	Container     (),
	mFocusedIndex (-1) {}

// *************************************************
//
// *************************************************

NavigationContainer::~NavigationContainer()
{
}

// *************************************************
//
// *************************************************

Control * NavigationContainer::GetFocusedControl()
{
	if (mControls.empty()) return nullptr;

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
			if (i == mFocusedIndex) mControls[i]->SetFocused(true);
			else                    mControls[i]->SetFocused(false);
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