#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   EntityManager.h
//
//  Desc:   Singleton class to handle the  management of Entities.          
//
//------------------------------------------------------------------------
#include <map>
#include <cassert>
#include "..\BaseGameEntity.h"


class EntityManager
{
private:
	//create a map for ease of access
	typedef std::map<int, BaseGameEntity*> EntityMap;
	
private:
	//to facilitate quick lookup the entities are stored in a map,
	//in which pointers to entities are cross referenced by their identifying number
	EntityMap		m_EntityMap;
	
	EntityManager(){}
	
	//copy ctor and assign should be private
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);
	
public:
	static EntityManager* Instance();
	
	//This method stores a pointer to the entity in the std::vector
	//m_Entities at the index pos indicated by the entity's ID
	void	RegisterEntity(BaseGameEntity* NewEntity);
	
	//returns apointer to the entity with the ID given as a param
	BaseGameEntity* GetEntityFromID(int id)const;
	
	//method removes the entity from the list
	void			RemoveEntity(BaseGameEntity* pEntity);
};

//provides easy access to the instance of the EntityManager
#define EntityMgr EntityManager::Instance()
#endif