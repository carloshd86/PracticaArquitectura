#include "control.h"


Control::Control(float x, float y, float width, float height, Container * parent) :
	mX       (x),
	mY       (y),
	mWidth   (width),
	mHeight  (height),
	mParent  (parent),
	mFocused (false)
{
	if (parent)
		parent->AddControl(this);
}

// *************************************************
//
// *************************************************

Control::~Control()
{
}

// *************************************************
//
// *************************************************

const Container * Control::GetParent() const
{
	return mParent;
}

// *************************************************
//
// *************************************************

void Control::SetFocused(bool focused)
{
	mFocused = focused;
}