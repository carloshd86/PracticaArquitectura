#ifndef _ENTITY_H_
#define _ENTITY_H_


#include "core.h"
#include "messagereceiver.h"


class Component;


class Entity : public IMessageReceiver
{
public:

	~Entity();

	virtual void Run          (float deltaTime);
	void         AddComponent (Component * component);

	virtual void ReceiveMessage(GameMessage &message);

private:

	std::vector<Component *> m_components;
};

#endif