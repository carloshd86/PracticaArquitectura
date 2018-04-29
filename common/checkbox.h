#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__


#include "control.h"
#include "properties.h"
#include "sprite.h"
#include <string>


class Checkbox : public Control 
{

public:

	class IListener 
	{
	public:

		virtual void OnClick(Checkbox * checkbox) = 0;
	};

	Checkbox(float x, float y, float width, float height, Container * parent, Properties * properties, const char * textKey, bool checked = false, float rOn = 1.f, float gOn = 1.f, float bOn = 0.f, float rOff = 1.f, float gOff = 1.f, float bOff = 1.f);
	virtual ~Checkbox();

	virtual bool ProcessEvent (IEventManager::EM_Event event);
	virtual void Update       (float deltaTime);
	virtual void Render       ();

	void SetChecked    (bool checked);
	bool IsChecked     () const;
	void SetProperties (Properties * properties);

	void SetListener(Checkbox::IListener * listener);

private:

	static const char * UNCHECKED_IMAGE;
	static const char * CHECKED_IMAGE;
	static const vec2   IMAGE_SIZE;

	float        m_rOn;
	float        m_gOn;
	float        m_bOn;
	float        m_rOff;
	float        m_gOff;
	float        m_bOff;
	bool         mChecked;
	const char  *m_pImage;
	vec2         mCheckBoxPos;
	Properties  *m_pProperties;
	std::string  mTextKey;
	std::string  mText;
	IListener   *m_pListener;
	ISprite     *m_pSprite;
};


#endif