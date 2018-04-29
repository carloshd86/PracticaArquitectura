#ifndef __COMPONENT_ROUTE_PATH_H__
#define __COMPONENT_ROUTE_PATH_H__


#include "component.h"


class C_RoutePath : public Component, public IMessageReceiver
{
public:

	C_RoutePath(Entity * owner) :
		Component           (owner),
		mCurrentRoutePoint (0) {}

	virtual ~C_RoutePath()
	{
		End();
	}

	virtual void Init()
	{
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireRoute              , std::bind(&C_RoutePath::OnRequireRoute              , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::IncreaseCurrentRoutePoint , std::bind(&C_RoutePath::OnIncreaseCurrentRoutePoint , this, std::placeholders::_1)));

		mInitialized = true;
	}

	virtual void End()
	{
		mMessageCallbacks.clear();

		mInitialized = false;
	}

	virtual void Run(float deltaTime)
	{

	}

	void AddRoutePoint(float x, float y)
	{
		mRoutePoints.push_back(vmake(x, y));
	}

	void RemoveRoutePoint(float x, float y)
	{
		for (auto it = mRoutePoints.begin(); it != mRoutePoints.end(); ) 
		{
			if ((*it).x == x && (*it).y == y) 
			{
				it = mRoutePoints.erase(it);
				break;
			} 
			else 
			{
				++it;
			}
		}
	}

	void ClearRoutePoints()
	{
		mRoutePoints.clear();
	}

	void ReceiveMessage(GameMessage &message)
	{
		GameMessage::GM_Type messageType = message.GetType();
		if (mMessageCallbacks.end() != mMessageCallbacks.find(messageType))
			mMessageCallbacks[messageType](message);
	}

private:

	std::vector<vec2>  mRoutePoints;
	uint8_t            mCurrentRoutePoint;
	bool               mInitialized;
	MessageCallbackMap mMessageCallbacks;

	void OnRequireRoute(GameMessage& message)
	{
		RequireRouteMessage * rrm = dynamic_cast<RequireRouteMessage *>(&message);
		if (rrm)
		{
			rrm->SetCurrentRoutePoint(mCurrentRoutePoint);
			rrm->SetRoutePoints(&mRoutePoints);
		}
	}

	void OnIncreaseCurrentRoutePoint(GameMessage& message)
	{
		IncreaseCurrentRoutePointMessage * icrpm = dynamic_cast<IncreaseCurrentRoutePointMessage *>(&message);
		if (icrpm)
		{ 
			if (abs(mRoutePoints[mCurrentRoutePoint].x - icrpm->GetCurrentX()) < 2 && abs(mRoutePoints[mCurrentRoutePoint].y - icrpm->GetCurrentY()) < 2)
				++mCurrentRoutePoint;
			if(mCurrentRoutePoint >= mRoutePoints.size())
				mCurrentRoutePoint = 0;
		}
	}
};

#endif
