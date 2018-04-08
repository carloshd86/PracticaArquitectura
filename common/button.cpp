#include "stdafx.h"
#include "globals.h"
#include "button.h"
#include "sys.h"
#include "font.h"



Button::Button(float x, float y, float width, float height, Container * parent, Properties * properties, const char * textKey, float rOn, float gOn, float bOn, float rOff, float gOff, float bOff) :
	Control       (x, y, width, height, parent),
	m_pProperties (properties),
	mTextKey      (textKey),
	m_rOn         (rOn),
	m_gOn         (gOn),
	m_bOn         (bOn),
	m_rOff        (rOff),
	m_gOff        (gOff),
	m_bOff        (bOff),
	mListener     (nullptr) 
{
	mText =  m_pProperties->GetProperty(mTextKey);
}

// *************************************************
//
// *************************************************

Button::~Button()
{
}
		   
// *************************************************
//
// *************************************************

bool Button::ProcessEvent(IEventManager::EM_Event event)
{
	if(!mFocused) return true;

	switch (event) {
		case IEventManager::EM_Event::SinglePressEnter: { if (mFocused) mListener->OnClick(this); break; }
	}

	return true;
}

// *************************************************
//
// *************************************************

void Button::Update(float deltaTime)
{
}

// *************************************************
//
// *************************************************

void Button::Render()
{
	if (!mFocused)
		glColor3f(m_rOff, m_gOff, m_bOff);
	else
		glColor3f(m_rOn, m_gOn, m_bOn);

	FONT_DrawString(vmake(mX, mY), mText.c_str());
}

// *************************************************
//
// *************************************************

void Button::SetProperties(Properties * properties)
{
	m_pProperties = properties;
	mText =  m_pProperties->GetProperty(mTextKey);
}

// *************************************************
//
// *************************************************

void Button::SetListener(Button::IListener * listener)
{
	mListener = listener;
}

