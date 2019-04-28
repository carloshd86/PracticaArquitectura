#ifndef __SLIDER_H__
#define __SLIDER_H__


#include "control.h"
#include "properties.h"
#include "sprite.h"
#include <string>


class Slider : public Control 
{

public:

	class IListener 
	{
	public:

		virtual void OnPercentageChanged(Slider* slider) = 0;
	};

	Slider(float x, float y, float width, float height, Container * parent, Properties * properties, const char * textKey, float percentage, float rOn = 1.f, float gOn = 1.f, float bOn = 0.f, float rOff = 1.f, float gOff = 1.f, float bOff = 1.f);
	virtual ~Slider();

	virtual bool ProcessEvent (IEventManager::EM_Event event);
	virtual void Update       (float deltaTime);
	virtual void Render       ();

	void  SetPercentage      (float percentage);
	float GetPercentage      () const;
	void  SetProperties      (Properties * properties);

	void SetListener(Slider::IListener * listener);

private:

	static const char * LINE_IMAGE;
	static const vec2   LINE_IMAGE_SIZE;
	static const char * MARKER_IMAGE;
	static const vec2   MARKER_IMAGE_SIZE;
	static const float  PERCENTAGE_CHANGE_SPEED;

	float        m_rOn;
	float        m_gOn;
	float        m_bOn;
	float        m_rOff;
	float        m_gOff;
	float        m_bOff;
	float        mPercentage;
	const char  *m_pLineImage;
	const char  *m_pMarkerImage;
	vec2         mSliderPos;
	Properties  *m_pProperties;
	std::string  mTextKey;
	std::string  mText;
	IListener   *m_pListener;
	ISprite     *m_pLineSprite;
	ISprite     *m_pMarkerSprite;
	int          mPercentageIncreaseRate;

	void  IncreasePercentage (bool positive = true);
	void  StopIncreasing     ();
};


#endif