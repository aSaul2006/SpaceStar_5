#ifndef STATE_H
#define STATE_H
//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   abstract base class to define an interface for a state
//------------------------------------------------------------------------

template<class entity_type>
class State
{
public:
	virtual ~State(){}
	
	//Execute when a state is entered
	virtual void Enter(entity_type*)=0;
	
	//called by the inherited Update() function each update step, unless defined otherwise
	virtual void Execute(entity_type*)=0;
	
	//Executed upon the exit of the state referred to
	virtual void Exit(entity_type*)=0;
	
};

#endif