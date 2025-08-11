//========= Copyright © 2008-2025, Team Sandpit, All rights reserved. ============
//
// Purpose: Entity and edict utilities
//
// $NoKeywords: $
//================================================================================

#ifndef __ENTITY_UTILS_H_
#define __ENTITY_UTILS_H_

#include <edict.h>

class CEntityUtils
{
public:
	static edict_t *BaseEntityToEdict(CBaseEntity *pEntity);
};

#endif // __ENTITY_UTILS_H_