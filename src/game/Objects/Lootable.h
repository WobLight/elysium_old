/************************************************************************
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/
#ifndef LOOTABLE_H
#define LOOTABLE_H

#include "Object.h"
#include "LootMgr.h"

class Lootable
{
public:
    virtual bool prepareLoot(Player *reciever, LootType loot_type, Player *pVictim, PermissionTypes &permission) = 0;
    Lootable(WorldObject const* lootTarget, uint32 gold = 0);
    Loot loot;
};

#endif // LOOTABLE_H
