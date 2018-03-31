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
		m_x(x),
		m_y(y) {}

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

private:

	float m_x;
	float m_y;
};

/***************************/

class AddPositionMessage : public GameMessage
{
public:

	AddPositionMessage(float x, float y) : 
		m_x(x),
		m_y(y) {}

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

private:

	float m_x;
	float m_y;
};

/***************************/

class SetSizeMessage : public GameMessage
{
public:

	SetSizeMessage(float x, float y) : 
		m_x(x),
		m_y(y) {}

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

private:

	float m_x;
	float m_y;
};

/***************************/

class RequirePositionMessage : public GameMessage
{
public:

	RequirePositionMessage() : 
		m_processed (false),
		m_x         (0.f), 
		m_y         (0.f) {}

	bool  GetProcessed()         const { return m_processed; }
	void  SetProcessed(bool processed) { m_processed = processed; }
	float GetX()                 const { return m_x; }
	void  SetX(float x)                { m_x = x; }
	float GetY()                 const { return m_y; }
	void  SetY(float y)                { m_y = y; }

private:

	bool  m_processed;
	float m_x;
	float m_y;
};

/***************************/

class RequireSizeMessage : public GameMessage
{
public:

	RequireSizeMessage() : 
		m_processed (false),
		m_x         (0.f), 
		m_y         (0.f) {}

	bool  GetProcessed()         const { return m_processed; }
	void  SetProcessed(bool processed) { m_processed = processed; }
	float GetX()                 const { return m_x; }
	void  SetX(float x)                { m_x = x; }
	float GetY()                 const { return m_y; }
	void  SetY(float y)                { m_y = y; }

private:

	bool  m_processed;
	float m_x;
	float m_y;
};

/***************************/

class RequireMovementMessage : public GameMessage
{
public:

	RequireMovementMessage() : 
		m_processed (false),
		m_x         (0.f), 
		m_y         (0.f),
		m_speed     (0.f){}

	bool  GetProcessed()                  const { return m_processed; }
	void  SetProcessed(bool processed)          { m_processed = processed; }
	float GetX()                          const { return m_x; }
	void  SetX(float x)                         { m_x = x; }
	float GetY()                          const { return m_y; }
	void  SetY(float y)                         { m_y = y; }
	float GetSpeed()                      const { return m_speed; }
	void  SetSpeed(float speed)                 { m_speed = speed; }

private:

	bool  m_processed;
	float m_x;
	float m_y;
	float m_speed;
};

/***************************/

class RequirePursuingSpeedMessage : public GameMessage
{
public:

	RequirePursuingSpeedMessage() : 
		m_pursuingSpeed (0.f) {}

	bool  GetProcessed()                  const { return m_processed; }
	void  SetProcessed(bool processed)          { m_processed = processed; }
	float GetPursuingSpeed()              const { return m_pursuingSpeed; }
	void  SetPursuingSpeed(float pursuingSpeed) { m_pursuingSpeed = pursuingSpeed; }

private:

	bool  m_processed;
	float m_pursuingSpeed;
};

/***************************/

class RequireRouteMessage : public GameMessage
{
public:

	RequireRouteMessage() : 
		m_pRoutePoints(nullptr),
		m_currentRoutePoint(0) {}

	const std::vector<vec2> * GetRoutePoints()                                 const { return m_pRoutePoints; }
	void                      SetRoutePoints(const std::vector<vec2> * pRoutePoints) { m_pRoutePoints = pRoutePoints; }
	uint8_t                   GetCurrentRoutePoint()                           const { return m_currentRoutePoint; }
	void                      SetCurrentRoutePoint(uint8_t currentRoutePoint)        { m_currentRoutePoint = currentRoutePoint; }

private:

	const std::vector<vec2> *m_pRoutePoints;
	uint8_t                  m_currentRoutePoint;
};

/***************************/

class IncreaseCurrentRoutePointMessage : public GameMessage
{
public:

	IncreaseCurrentRoutePointMessage(float currentX, float currentY) : 
		m_currentX(currentX),
		m_currentY(currentY) {}

	float GetCurrentX() const { return m_currentX; }
	float GetCurrentY() const { return m_currentY; }

private:

	float m_currentX;
	float m_currentY;
};


/***************************/

class SetMovementMessage : public GameMessage
{
public:

	SetMovementMessage(float x, float y) : 
		m_x(x),
		m_y(y) {}

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

private:

	float m_x;
	float m_y;
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
		m_image(image) {}

	const char * GetImage() const { return m_image; }

private:

	const char * m_image;
};

#endif