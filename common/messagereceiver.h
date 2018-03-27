#ifndef _MESSAGE_RECEIVER_H_
#define _MESSAGE_RECEIVER_H_


class GameMessage;

class IMessageReceiver 
{
public:

	virtual void ReceiveMessage(GameMessage &message) = 0;
};

#endif