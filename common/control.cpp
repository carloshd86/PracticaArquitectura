#include "stdafx.h"
#include "control.h"



Control::Control(float x, float y, float width, float height, Container * parent) :
	mX       (x),
	mY       (y),
	mWidth   (width),
	mHeight  (height),
	mParent  (parent),
	mFocused (false),
	mVisible (true)
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

// *************************************************
//
// *************************************************

bool Control::GetVisible() const
{
	return mVisible;
}

// *************************************************
//
// *************************************************

void Control::SetVisible(bool visible)
{
	mVisible = visible;
}