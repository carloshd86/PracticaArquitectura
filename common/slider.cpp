#include "stdafx.h"
#include "globals.h"
#include "slider.h"
#include "sys.h"
#include "font.h"
#include "asserts.h"


const char * Slider::LINE_IMAGE              = "../data/sliderLine.png";
const vec2   Slider::LINE_IMAGE_SIZE         { 250.f, 25.f };
const char * Slider::MARKER_IMAGE            = "../data/sliderMarker.png";
const vec2   Slider::MARKER_IMAGE_SIZE       { 25.f, 25.f };
const float  Slider::PERCENTAGE_CHANGE_SPEED = 50;

// *************************************************
//
// *************************************************

Slider::Slider(float x, float y, float width, float height, Container * parent, Properties * properties, const char * textKey, float percentage, float rOn, float gOn, float bOn, float rOff, float gOff, float bOff) :
	Control         (x, y, width, height, parent),
	m_pProperties   (properties),
	mTextKey        (textKey),
	m_rOn           (rOn),
	m_gOn           (gOn),
	m_bOn           (bOn),
	m_rOff          (rOff),
	m_gOff          (gOff),
	m_bOff          (bOff),
	mPercentage     (percentage),
	mSliderPos      { x, y },
	m_pLineImage    (LINE_IMAGE),
	m_pMarkerImage  (MARKER_IMAGE),
	m_pListener     (nullptr),
	m_pLineSprite   (nullptr),
	m_pMarkerSprite (nullptr)
{
	GAME_ASSERT(g_pGraphicsEngine);
	mText =  m_pProperties->GetProperty(mTextKey);
	mSliderPos = FONT_GetStringNextPos(vmake(mX, mY), mText.c_str());
	mSliderPos = vadd(mSliderPos, vmake(16.f, 0.f));
	m_pLineSprite   = g_pGraphicsEngine->RequireSprite(mSliderPos, LINE_IMAGE_SIZE, m_pLineImage, false);
	m_pMarkerSprite = g_pGraphicsEngine->RequireSprite(vec2 { mSliderPos.x + (LINE_IMAGE_SIZE.x - MARKER_IMAGE_SIZE.x) * mPercentage / 100.f, mSliderPos.y }, MARKER_IMAGE_SIZE, m_pMarkerImage, false);
}

// *************************************************
//
// *************************************************

Slider::~Slider()
{
	if (m_pLineSprite)   g_pGraphicsEngine->ReleaseSprite(m_pLineSprite);
	if (m_pMarkerSprite) g_pGraphicsEngine->ReleaseSprite(m_pMarkerSprite);
}

// *************************************************
//
// *************************************************

bool Slider::ProcessEvent(IEventManager::EM_Event event)
{
	if(!mFocused) return true;

	switch (event) {
		case IEventManager::EM_Event::MoveRight   : { if (mFocused) { IncreasePercentage(true);  break; } }
		case IEventManager::EM_Event::MoveLeft    : { if (mFocused) { IncreasePercentage(false); break; } }
		case IEventManager::EM_Event::ReleaseRight: 
		case IEventManager::EM_Event::ReleaseLeft : { if (mFocused) { StopIncreasing();  break; } }
	}

	return true;
}

// *************************************************
//
// *************************************************

void Slider::Update(float deltaTime)
{
	if (0 != mPercentageIncreaseRate)
	{
		SetPercentage(mPercentage + static_cast<float>(mPercentageIncreaseRate) * Slider::PERCENTAGE_CHANGE_SPEED * deltaTime);
	}
}

// *************************************************
//
// *************************************************

void Slider::Render()
{
	if (!mFocused)
		glColor3f(m_rOff, m_gOff, m_bOff);
	else
		glColor3f(m_rOn, m_gOn, m_bOn);

	FONT_DrawString(vmake(mX, mY), mText.c_str());
	g_pGraphicsEngine->RenderSprite(m_pLineSprite);
	g_pGraphicsEngine->RenderSprite(m_pMarkerSprite);
}

// *************************************************
//
// *************************************************

void Slider::SetPercentage(float percentage)
{
	percentage = max(min(percentage, 100.f), 0.f);

	if (mPercentage != percentage)
	{
		mPercentage = percentage;
		m_pMarkerSprite->SetPos(mSliderPos.x + (LINE_IMAGE_SIZE.x - MARKER_IMAGE_SIZE.x) * mPercentage / 100.f, m_pMarkerSprite->GetPos().y);
		m_pListener->OnPercentageChanged(this);
	}
}

// *************************************************
//
// *************************************************

float Slider::GetPercentage() const
{
	return mPercentage;
}

// *************************************************
//
// *************************************************

void Slider::SetProperties(Properties * properties)
{
	m_pProperties = properties;
	mText =  m_pProperties->GetProperty(mTextKey);
	mSliderPos = FONT_GetStringNextPos(vmake(mX, mY), mText.c_str());
	mSliderPos = vadd(mSliderPos, vmake(16.f, 0.f));
	m_pLineSprite->SetPos(mSliderPos.x, mSliderPos.y);
	m_pMarkerSprite->SetPos(mSliderPos.x + (LINE_IMAGE_SIZE.x - MARKER_IMAGE_SIZE.x) * mPercentage / 100.f, m_pMarkerSprite->GetPos().y);
	m_pListener->OnPercentageChanged(this);
}

// *************************************************
//
// *************************************************

void Slider::SetListener(Slider::IListener * listener)
{
	m_pListener = listener;
}

// *************************************************
//
// *************************************************

void Slider::IncreasePercentage(bool positive)
{
	mPercentageIncreaseRate = (positive ? 1 : -1);
}

// *************************************************
//
// *************************************************

void Slider::StopIncreasing()
{
	mPercentageIncreaseRate = 0;
}
