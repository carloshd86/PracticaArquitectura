#include "stdafx.h"
#include "globals.h"
#include "checkbox.h"
#include "sys.h"
#include "font.h"
#include <assert.h>


const char * Checkbox::UNCHECKED_IMAGE = "../data/checkbox-unchecked.png";
const char * Checkbox::CHECKED_IMAGE   = "../data/checkbox-checked.png";
const vec2   Checkbox::IMAGE_SIZE      { 25.f, 25.f};

// *************************************************
//
// *************************************************

Checkbox::Checkbox(float x, float y, float width, float height, Container * parent, Properties * properties, const char * textKey, bool checked, float rOn, float gOn, float bOn, float rOff, float gOff, float bOff) :
	Control       (x, y, width, height, parent),
	m_pProperties (properties),
	mTextKey      (textKey),
	m_rOn         (rOn),
	m_gOn         (gOn),
	m_bOn         (bOn),
	m_rOff        (rOff),
	m_gOff        (gOff),
	m_bOff        (bOff),
	mChecked      (checked),
	mCheckBoxPos  { x, y },
	m_pImage      (nullptr),
	m_pListener   (nullptr)
{
	assert(g_pGraphicsEngine);
	mText =  m_pProperties->GetProperty(mTextKey);
	SetChecked(checked);
	mCheckBoxPos = FONT_GetStringNextPos(vmake(mX, mY), mText.c_str());
	mCheckBoxPos = vadd(mCheckBoxPos, vmake(16.f, 0.f));
	m_pSprite = g_pGraphicsEngine->RequireSprite(mCheckBoxPos, IMAGE_SIZE, m_pImage);
}

// *************************************************
//
// *************************************************

Checkbox::~Checkbox()
{
	if (m_pSprite) g_pGraphicsEngine->ReleaseSprite(m_pSprite);
}
		   
// *************************************************
//
// *************************************************

bool Checkbox::ProcessEvent(IEventManager::EM_Event event)
{
	if(!mFocused) return true;

	switch (event) {
		case IEventManager::EM_Event::SinglePressEnter: { if (mFocused) m_pListener->OnClick(this); break; }
	}

	return true;
}

// *************************************************
//
// *************************************************

void Checkbox::Update(float deltaTime)
{
}

// *************************************************
//
// *************************************************

void Checkbox::Render()
{
	if (!mFocused)
		glColor3f(m_rOff, m_gOff, m_bOff);
	else
		glColor3f(m_rOn, m_gOn, m_bOn);

	FONT_DrawString(vmake(mX, mY), mText.c_str());
	vec2 spritePos = m_pSprite->GetPos();
	vec2 spriteSize = m_pSprite->GetSize();
	CORE_RenderSprite(spritePos, vmake(spritePos.x + spriteSize.x, spritePos.y + spriteSize.y), m_pSprite->GetTex());
}

// *************************************************
//
// *************************************************

void Checkbox::SetChecked(bool checked)
{
	mChecked = checked;
	const char * NEXT_IMAGE = mChecked ? CHECKED_IMAGE : UNCHECKED_IMAGE;

	bool changeImage = !m_pImage;
	if (m_pImage && strcmp(NEXT_IMAGE, m_pImage)) 
	{
		g_pGraphicsEngine->ReleaseSprite(m_pSprite);
		changeImage = true;
	}

	if (changeImage)
	{
		m_pImage = NEXT_IMAGE;
		m_pSprite = g_pGraphicsEngine->RequireSprite(mCheckBoxPos, IMAGE_SIZE, m_pImage);
	}
}

// *************************************************
//
// *************************************************

bool Checkbox::IsChecked() const
{
	return mChecked;
}

// *************************************************
//
// *************************************************

void Checkbox::SetProperties(Properties * properties)
{
	m_pProperties = properties;
	mText =  m_pProperties->GetProperty(mTextKey);
	mCheckBoxPos = FONT_GetStringNextPos(vmake(mX, mY), mText.c_str());
	mCheckBoxPos = vadd(mCheckBoxPos, vmake(16.f, 0.f));
	m_pSprite->SetPos(mCheckBoxPos.x, mCheckBoxPos.y);
}

// *************************************************
//
// *************************************************

void Checkbox::SetListener(Checkbox::IListener * listener)
{
	m_pListener = listener;
}

