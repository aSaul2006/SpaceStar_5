//ItemActor Class
#include "ItemActor.h"

ItemActor::ItemActor(void)
{

}
ItemActor::~ItemActor()
{
	Shutdown();
}

void ItemActor::Shutdown()
{
	if(itemSFX!=NULL)
	{
		itemSFX->release();
		itemSFX = NULL;
	}
}
