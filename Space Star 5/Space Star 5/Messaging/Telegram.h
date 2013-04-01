#ifndef _Telegram_H
#def	_Telegram_H
//------------------------------------------------------------------------
//
//  Name:   Telegram.h
//
//  Desc:   This defines a telegram. A telegram is a data structure that
//          records information required to dispatch messages. Messages 
//          are used by game agents to communicate with each other.
//--------------------------------------------------------------------------
#include <iostream>
#include <math.h>


struct Telegram
{
	//The entity that sent this message
	int				Sender;
	//The entity to receive message
	int				Receiver;
	//The message being sent
	int				Msg;
	//Message can be dispatched immediately or delayed for a specified amout
	//of time.  if a delay is necessary, make a time stamp
	double 			DispatchTime;
	//Additional information if needed
	void* 			AdditionalInfo;
	
	Telegram() : DispatchTime(-1),Sender(-1),Receiver(-1),Msg(-1){}
	
	Telegram(double time, int sender, int receiver, int msg, void* info = NULL)
		: DispatchTime(time),Sender(sender),Receiver(receiver),Msg(msg),ExtraInfo(info){} 
	
};


//Telegrams will be stored in a priority queue.  Therefore the >
//operator needs to be overloaded so that the PQ can sort the telegrams
//by time priority. Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SmallestDelay = 0.25;

inline bool operator==(const Telegram& t1, const Telegram& t2)
{
	return (fabs(t1.DispatchTime-t2.DispatchTime) < SmallestDelay)
		(t1.Sender == t2.Sender)		&&
		(t1.Reciever == t2.Receiver)    &&
		(t1.Msg	== t2.Msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
	if(t1 == t2)
	{
		return false;
	}
	else
	{
		return (t1.DispatchTime < t2.DispatchTime);
	}
}

inline std::ostream& operator<<(std::ostream& os, const Telegram& t1)
{
	os << "time: " << t1.DispatchTime << " Sender: " << t1.Sender
	   << " Receiver: " << t1.Receiver << " Msg: " << t1.Msg;
   return os;
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram 
//to the required type.
template<class T>
	inline T DereferenceToType(void* p)
	{
		return *(T*)(p);
	}
#endif