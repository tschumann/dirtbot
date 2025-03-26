/*
 *    part of https://rcbot2.svn.sourceforge.net/svnroot/rcbot2
 *
 *    This file is part of RCBot.
 *
 *    RCBot by Paul Murphy adapted from Botman's HPB Bot 2 template.
 *
 *    RCBot is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    RCBot is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RCBot; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#ifndef __RCBOT2_UTIL_FUNCTIONS_H_
#define __RCBOT2_UTIL_FUNCTIONS_H_

/**
 * @brief General RCBot2 utility functions.
 */
namespace rcbot2utils
{
	// TO-DO: Migrate functions from CBotGlobals to here.

	/**
	 * @brief Checks if the given edict is valid.
	 * @param edict Edict to check.
	 * @return True if valid, false otherwise.
	 */
	bool IsValidEdict(const edict_t* edict);
	/**
	 * @brief Gets the index of an edict.
	 * @param edict Edict ptr.
	 * @return Index of the edict.
	 */
	int IndexOfEdict(const edict_t* edict);
	/**
	 * @brief Gets an edict from an index.
	 * @param index Entity index.
	 * @return Edict pointer or NULL on failure.
	 */
	edict_t* EdictOfIndex(int index);
	// Converts a CBaseEntity into an edict_t
	edict_t* BaseEntityToEdict(CBaseEntity* entity);
	// Converts an edict_t into a CBaseEntity
	CBaseEntity* EdictToBaseEntity(edict_t* edict);
	/**
	 * @brief Converts an IHandleEntity to edict_t. Used to get entities in the trace filter callback function.
	 * @param pHandleEntity Entity to convert.
	 * @return edict_t* pointer. May be NULL.
	 */
	edict_t* GetEdictFromHandleEntity(const IHandleEntity* pHandleEntity);
	/**
	 * @brief Converts an IHandleEntity to CBaseEntity. Used to get entities in the trace filter callback function.
	 * @param pHandleEntity Entity to convert.
	 * @return CBaseEntity pointer. May be NULL.
	 */
	CBaseEntity* GetEntityFromHandleEntity(const IHandleEntity* pHandleEntity);
	/**
	 * @brief Dereferences the given handle and converts to an edict_t.
	 * @param handle Handle to dereference.
	 * @return Edict pointer or NULL if the entity stored is no longer valid.
	 */
	edict_t* GetHandleEdict(CBaseHandle& handle);
	/**
	 * @brief Stores an edict on a handle.
	 * @param handle Handle to store the entity.
	 * @param edict Edict to store.
	 */
	void SetHandleEdict(CBaseHandle& handle, const edict_t* edict);
	/**
	 * @brief Returns the entity origin (generally this calls GetAbsOrigin).
	 * 
	 * For brush entities, this will generally always return 0,0,0. Use WorldSpaceCenter for brush entities.
	 * @param entity Entity to get the origin from.
	 * @return Entity position.
	 */
	const Vector& GetEntityOrigin(edict_t* entity);
	/**
	 * @brief Returns the entity origin (generally this calls GetAbsOrigin).
	 *
	 * For brush entities, this will generally always return 0,0,0. Use WorldSpaceCenter for brush entities.
	 * @param entity Entity to get the origin from.
	 * @return Entity position.
	 */
	const Vector& GetEntityOrigin(CBaseEntity* entity);
	/**
	 * @brief Returns the entity angles (generally this calls GetAbsAngles).
	 * @param entity Entity to get the angles from.
	 * @return Entity Angles.
	 */
	const QAngle& GetEntityAngles(edict_t* entity);
	/**
	 * @brief Returns the entity angles (generally this calls GetAbsAngles).
	 * @param entity Entity to get the angles from.
	 * @return Entity Angles.
	 */
	const QAngle& GetEntityAngles(CBaseEntity* entity);
	/**
	 * @brief Gets the entity World Space Center.
	 * @param entity Entity to get the WSC from.
	 * @return Entity's world space center position.
	 */
	Vector GetWorldSpaceCenter(edict_t* entity);
	/**
	 * @brief Gets the entity World Space Center.
	 * @param entity Entity to get the WSC from.
	 * @return Entity's world space center position.
	 */
	Vector GetWorldSpaceCenter(CBaseEntity* entity);
	/**
	 * @brief Checks if a given point is within the trigger bounds of an entity.
	 * @param pEntity Entity to test.
	 * @param vPoint Point to test.
	 * @return True if the given point is within, false otherwise.
	 */
	bool PointIsWithinTrigger(edict_t* pEntity, const Vector& vPoint);
}

#endif // !__RCBOT2_UTIL_FUNCTIONS_H_
