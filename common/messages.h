#ifndef __MESSAGES_H__
#define __MESSAGES_H__


#include "messagereceiver.h"


class GameMessage
{
public:

	virtual ~GameMessage() {};
};

/***************************/

class EnemyCollisionMessage : public GameMessage
{
};

/***************************/

class RigidBodyCollisionMessage : public GameMessage
{
public:

	enum Type 
	{
		CollisionX,
		CollisionY
	};

	RigidBodyCollisionMessage(RigidBodyCollisionMessage::Type type) : 
		m_type(type) {}

	RigidBodyCollisionMessage::Type GetType() const { return m_type; }

private:

	RigidBodyCollisionMessage::Type m_type;
};

/***************************/

class GoalReachedMessage : public GameMessage
{
};

/***************************/

class SetPositionMessage : public GameMessage
{
public:

	SetPositionMessage(float x, float y) : 
		mX(x),
		mY(y) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }

private:

	float mX;
	float mY;
};

/***************************/

class AddPositionMessage : public GameMessage
{
public:

	AddPositionMessage(float x, float y) : 
		mX(x),
		mY(y) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }

private:

	float mX;
	float mY;
};

/***************************/

class SetSizeMessage : public GameMessage
{
public:

	SetSizeMessage(float x, float y) : 
		mX(x),
		mY(y) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }

private:

	float mX;
	float mY;
};

/***************************/

class RequirePositionMessage : public GameMessage
{
public:

	RequirePositionMessage() : 
		mProcessed (false),
		mX         (0.f), 
		mY         (0.f) {}

	bool  GetProcessed()         const { return mProcessed; }
	void  SetProcessed(bool processed) { mProcessed = processed; }
	float GetX()                 const { return mX; }
	void  SetX(float x)                { mX = x; }
	float GetY()                 const { return mY; }
	void  SetY(float y)                { mY = y; }

private:

	bool  mProcessed;
	float mX;
	float mY;
};

/***************************/

class RequireSizeMessage : public GameMessage
{
public:

	RequireSizeMessage() : 
		mProcessed (false),
		mX         (0.f), 
		mY         (0.f) {}

	bool  GetProcessed()         const { return mProcessed; }
	void  SetProcessed(bool processed) { mProcessed = processed; }
	float GetX()                 const { return mX; }
	void  SetX(float x)                { mX = x; }
	float GetY()                 const { return mY; }
	void  SetY(float y)                { mY = y; }

private:

	bool  mProcessed;
	float mX;
	float mY;
};

/***************************/

class RequireMovementMessage : public GameMessage
{
public:

	RequireMovementMessage() : 
		mProcessed (false),
		mX         (0.f), 
		mY         (0.f),
		mSpeed     (0.f){}

	bool  GetProcessed()                  const { return mProcessed; }
	void  SetProcessed(bool processed)          { mProcessed = processed; }
	float GetX()                          const { return mX; }
	void  SetX(float x)                         { mX = x; }
	float GetY()                          const { return mY; }
	void  SetY(float y)                         { mY = y; }
	float GetSpeed()                      const { return mSpeed; }
	void  SetSpeed(float speed)                 { mSpeed = speed; }

private:

	bool  mProcessed;
	float mX;
	float mY;
	float mSpeed;
};

/***************************/

class RequirePursuingSpeedMessage : public GameMessage
{
public:

	RequirePursuingSpeedMessage() : 
		mPursuingSpeed (0.f) {}

	bool  GetProcessed()                  const { return mProcessed; }
	void  SetProcessed(bool processed)          { mProcessed = processed; }
	float GetPursuingSpeed()              const { return mPursuingSpeed; }
	void  SetPursuingSpeed(float pursuingSpeed) { mPursuingSpeed = pursuingSpeed; }

private:

	bool  mProcessed;
	float mPursuingSpeed;
};

/***************************/

class RequireRouteMessage : public GameMessage
{
public:

	RequireRouteMessage() : 
		m_pRoutePoints(nullptr),
		mCurrentRoutePoint(0) {}

	const std::vector<vec2> * GetRoutePoints()                                 const { return m_pRoutePoints; }
	void                      SetRoutePoints(const std::vector<vec2> * pRoutePoints) { m_pRoutePoints = pRoutePoints; }
	uint8_t                   GetCurrentRoutePoint()                           const { return mCurrentRoutePoint; }
	void                      SetCurrentRoutePoint(uint8_t currentRoutePoint)        { mCurrentRoutePoint = currentRoutePoint; }

private:

	const std::vector<vec2> *m_pRoutePoints;
	uint8_t                  mCurrentRoutePoint;
};

/***************************/

class IncreaseCurrentRoutePointMessage : public GameMessage
{
public:

	IncreaseCurrentRoutePointMessage(float currentX, float currentY) : 
		mCurrentX(currentX),
		mCurrentY(currentY) {}

	float GetCurrentX() const { return mCurrentX; }
	float GetCurrentY() const { return mCurrentY; }

private:

	float mCurrentX;
	float mCurrentY;
};


/***************************/

class SetMovementMessage : public GameMessage
{
public:

	SetMovementMessage(float x, float y) : 
		mX(x),
		mY(y) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }

private:

	float mX;
	float mY;
};

/***************************/

class MoveUpMessage : public GameMessage
{
};

/***************************/

class MoveDownMessage : public GameMessage
{
};

/***************************/

class MoveLeftMessage : public GameMessage
{
};

/***************************/

class MoveRightMessage : public GameMessage
{
};

/***************************/

class ChangeSpriteMessage : public GameMessage
{
public:

	ChangeSpriteMessage(const char * image) : 
		mImage(image) {}

	const char * GetImage() const { return mImage; }

private:

	const char * mImage;
};

#endif