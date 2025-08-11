//========= Copyright © 2008-2025, Team Sandpit, All rights reserved. ============
//
// Purpose: Entity and edict utilities
//
// $NoKeywords: $
//================================================================================

#include "entity_utils.h"

edict_t *CEntityUtils::BaseEntityToEdict(CBaseEntity *pEntity)
{
	IServerUnknown *pUnknown = reinterpret_cast<IServerUnknown*>(pEntity);
	IServerNetworkable *pNetworkable = pUnknown->GetNetworkable();

	if (!pNetworkable)
	{
		return nullptr;
	}

	return pNetworkable->GetEdict();
}