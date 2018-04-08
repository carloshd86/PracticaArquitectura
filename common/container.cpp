#include "container.h"
#include "control.h"


Container::Container() :
	mVisible(true) {}

// *************************************************
//
// *************************************************

const std::vector<Control *>& Container::GetControls() const
{
	return mControls;
}

// *************************************************
//
// *************************************************

Container::~Container()
{
	for (auto control : mControls) delete control;
}

// *************************************************
//
// *************************************************

void Container::AddControl(Control * control)
{
	mControls.push_back(control);
}

// *************************************************
//
// *************************************************

void Container::RemoveControl(Control *& control)
{
	auto it = mControls.begin();
	while (it != mControls.end()) 
	{
		if ((*it) == control)
		{
			it = mControls.erase(it);
			break;
		}
	}

	delete control;
	control = nullptr;
}

// *************************************************
//
// *************************************************

void Container::Clear()
{
	auto it = mControls.begin();
	while (it != mControls.end()) 
	{
		delete (*it);
		it = mControls.erase(it);
	}
}

// *************************************************
//
// *************************************************

void Container::Update(float deltaTime)
{
	for (auto it = mControls.begin(); it != mControls.end(); ++it) (*it)->Update(deltaTime);
}

// *************************************************
//
// *************************************************

void Container::Render()
{
	for (auto it = mControls.begin(); it != mControls.end(); ++it) (*it)->Render();
}

// *************************************************
//
// *************************************************

bool Container::GetVisible() const
{
	return mVisible;
}

// *************************************************
//
// *************************************************

void Container::SetVisible(bool visible)
{
	mVisible = visible;
	for (auto it = mControls.begin(); it != mControls.end(); ++it)
	{
		(*it)->SetVisible(visible);
		if (!visible) (*it)->SetFocused(false);
	}
}