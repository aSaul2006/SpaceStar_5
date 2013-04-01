#include <string>

#include "Messaging\Telegram.h"

class BaseGameEntity
{
private:
	//every entity should have it's own unique identifier in game
	int				m_ID;
	static int m_iNextValidID;
	
	void SetID(int val);
	
public:
	BaseGameEntity(int id)
	{
		SetID(id);
	}
	
	virtual ~BaseGameEntity(){}
	
	//all entities must implement an update function
	virtual void Update()=0;
	
	//All entities can communicate using messages. they are sent
	//using the messageDispatcher singleton class
	virtual bool HandleMessage(const Telegram& msg)=0;
	
	int ID()const{return m_ID;}
};