/*
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

#include "propvar.h"
#include "helper.h"
#include "logging.h"

#ifdef WIN32
#include <stdexcept>
#endif

CPropertyVarBase::CPropertyVarBase(): m_type()
{
	m_initialized = false;
}

CPropertyVarBase::~CPropertyVarBase()
{
	m_initialized = false;
}

void CPropertyVarBase::Init(const char *propname, PropType type, int entity)
{
	CBaseEntity* baseentity = bot_helper->GetEntity(entity);

	if (!baseentity)
	{
		logger->Log(LogLevel::ERROR, "Initialization failed for PropertyVar \"%s\"! Entity of index %i is NULL!", propname, entity);
#ifdef WIN32 // TO-DO: verify of runtime_error works fine under linux
		throw std::runtime_error("Initialization failed for PropertyVar!");
#endif
		//return;
	}

	m_propname = std::string(propname);
	m_type = type;
	m_entity.Set(reinterpret_cast<IHandleEntity*>(baseentity));
	m_initialized = true;
}

void CPropertyVarBase::Term()
{
	m_initialized = false;
}

int CPropertyVarInt::Get() const
{
	return entprops->GetEntProp(m_entity.GetEntryIndex(), m_type, const_cast<char*>(m_propname.c_str()));
}

bool CPropertyVarBool::Get() const
{
	return entprops->GetEntPropBool(m_entity.GetEntryIndex(), m_type, const_cast<char*>(m_propname.c_str()));
}

float CPropertyVarFloat::Get() const
{
	return entprops->GetEntPropFloat(m_entity.GetEntryIndex(), m_type, const_cast<char*>(m_propname.c_str()));
}

Vector CPropertyVarVector::Get() const
{
    return entprops->GetEntPropVector(m_entity.GetEntryIndex(), m_type, const_cast<char*>(m_propname.c_str()));
}

void CPropertyVarVector::Get(Vector &dest) const
{
	const Vector source = entprops->GetEntPropVector(m_entity.GetEntryIndex(), m_type, const_cast<char*>(m_propname.c_str()));
	dest.x = source.x;
	dest.y = source.y;
	dest.z = source.z;
}