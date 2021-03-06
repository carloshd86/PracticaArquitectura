#ifndef __BUTTON_H__
#define __BUTTON_H__


#include "control.h"
#include "properties.h"
#include <string>


class Button : public Control 
{

public:

	class IListener 
	{
	public:

		virtual void OnClick(Button * button) = 0;
	};

	Button(float x, float y, float width, float height, Container * parent, Properties * properties, const char * textKey, float rOn = 1.f, float gOn = 1.f, float bOn = 0.f, float rOff = 1.f, float gOff = 1.f, float bOff = 1.f);
	virtual ~Button();

	virtual bool ProcessEvent (IEventManager::EM_Event event);
	virtual void Update       (float deltaTime);
	virtual void Render       ();

	void SetProperties(Properties * properties);

	void SetListener(Button::IListener * listener);

private:

	float       m_rOn;
	float       m_gOn;
	float       m_bOn;
	float       m_rOff;
	float       m_gOff;
	float       m_bOff;
	Properties *m_pProperties;
	std::string mTextKey;
	std::string mText;
	IListener  *m_pListener;
};


#endif