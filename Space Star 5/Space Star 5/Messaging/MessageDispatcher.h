#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H
//------------------------------------------------------------------------
//
//  Name:   MessageDispatcher.h
//
//  Desc:   A message dispatcher. Manages messages of the type Telegram.
//          Instantiated as a singleton.
//------------------------------------------------------------------------

#pragma warning (disable:4786)

#include <set>

#include "ConsoleUtils.h"
#include "Telegram.h"

#define Dispatch MessageDispatcher::Instance()


class MessageDispatcher
{
private:
	//a set used to contain the delayed messages benefits from auto sorting
	//and avoiding of duplication.  Messages are sorted by their dispatch time
	std::set<Telegram> PriorityQ;

	//calls the message handling member function of the receiving entity, pReceiver
	//with the newly created telegram
	void Discharge(Entity* pReceiver, const Telegram& msg);

	MessageDispatcher(){}

public:
	//make the class a singleton
	static MessageDispatcher* Instance();
	
	//send a message to another agent
	void Dispatchmessage(double delay, int sender, int receiver, int msg, void* ExtraInfo);
	
	//send out any delayed messages. should be called in main loop
	void DispatchDelayedMessages();	
};

