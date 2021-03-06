/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2014 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/
#include "ZoneServer\Services\ham\ham_service.h"

#include "Zoneserver/ObjectController/ObjectController.h"
#include "ZoneServer/ObjectController/ObjectControllerOpcodes.h"
#include "ZoneServer/ObjectController/ObjectControllerCommandMap.h"
#include "ZoneServer/Objects/Player Object/PlayerObject.h"
#include "ZoneServer/GameSystemManagers/Spawn Manager/SpawnPoint.h"
#include "MessageLib/MessageLib.h"

#include "DatabaseManager/Database.h"
#include "DatabaseManager/DatabaseResult.h"
#include "DatabaseManager/DataBinding.h"

//#include "NetworkManager/Message.h"
//#include "NetworkManager/MessageFactory.h"

//=============================================================================
//
// database callback
//
void ObjectController::handleDatabaseJobComplete(void* ref,swganh::database::DatabaseResult* result)
{
    ObjControllerAsyncContainer* asyncContainer = reinterpret_cast<ObjControllerAsyncContainer*>(ref);

    switch(asyncContainer->mQueryType)
    {
    case OCQuery_StatRead:
    {
        if(!asyncContainer->playerObject)
            break;
/*
        Ham* ourHam = asyncContainer->playerObject->getHam();

        StatTargets theTargets;

        swganh::database::DataBinding* binding = mDatabase->createDataBinding(9);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetHealth),4,0);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetStrength),4,1);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetConstitution),4,2);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetAction),4,3);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetQuickness),4,4);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetStamina),4,5);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetMind),4,6);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetFocus),4,7);
        binding->addField(swganh::database::DFT_uint32,offsetof(StatTargets,TargetWillpower),4,8);

        uint64	count	= result->getRowCount();
        //make sure we have some values set in case the statmigration table wont hold anything on us
        if(count != 1)
        {
            ourHam->setTargetStatValue(HamBar_Health,ourHam->getPropertyValue(HamBar_Health,HamProperty_BaseHitpoints));
            ourHam->setTargetStatValue(HamBar_Strength,ourHam->getPropertyValue(HamBar_Strength,HamProperty_BaseHitpoints));
            ourHam->setTargetStatValue(HamBar_Constitution,ourHam->getPropertyValue(HamBar_Constitution,HamProperty_BaseHitpoints));

            ourHam->setTargetStatValue(HamBar_Action,ourHam->getPropertyValue(HamBar_Action,HamProperty_BaseHitpoints));
            ourHam->setTargetStatValue(HamBar_Quickness,ourHam->getPropertyValue(HamBar_Quickness,HamProperty_BaseHitpoints));
            ourHam->setTargetStatValue(HamBar_Stamina,ourHam->getPropertyValue(HamBar_Stamina,HamProperty_BaseHitpoints));

            ourHam->setTargetStatValue(HamBar_Mind,ourHam->getPropertyValue(HamBar_Mind,HamProperty_BaseHitpoints));
            ourHam->setTargetStatValue(HamBar_Focus,ourHam->getPropertyValue(HamBar_Focus,HamProperty_BaseHitpoints));
            ourHam->setTargetStatValue(HamBar_Willpower,ourHam->getPropertyValue(HamBar_Willpower,HamProperty_BaseHitpoints));

        }
        else
        {
            result->getNextRow(binding,&theTargets);
            ourHam->setTargetStatValue(HamBar_Health,theTargets.TargetHealth);
            ourHam->setTargetStatValue(HamBar_Strength,theTargets.TargetStrength);
            ourHam->setTargetStatValue(HamBar_Constitution,theTargets.TargetConstitution);

            ourHam->setTargetStatValue(HamBar_Action,theTargets.TargetAction);
            ourHam->setTargetStatValue(HamBar_Quickness,theTargets.TargetQuickness);
            ourHam->setTargetStatValue(HamBar_Stamina,theTargets.TargetStamina);

            ourHam->setTargetStatValue(HamBar_Mind,theTargets.TargetMind);
            ourHam->setTargetStatValue(HamBar_Focus,theTargets.TargetFocus);
            ourHam->setTargetStatValue(HamBar_Willpower,theTargets.TargetWillpower);
        }

        mDatabase->destroyDataBinding(binding);
        gMessageLib->sendStatMigrationStartMessage(asyncContainer->playerObject);
		*/
    }
    break;

    case OCQuery_FindFriend:
    {
        swganh::database::DataBinding* binding = mDatabase->createDataBinding(1);
        binding->addField(swganh::database::DFT_uint64,0,8);

        uint64	ret		= 0;
        uint64	count	= result->getRowCount();

        result->getNextRow(binding,&ret);

        mDatabase->destroyDataBinding(binding);
        if(count == 0)
            ret = 0;

        _handleFindFriendDBReply(ret,asyncContainer->mString);

    }
    break;

    case OCQuery_AddFriend:
    {
        swganh::database::DataBinding* binding = mDatabase->createDataBinding(1);
        binding->addField(swganh::database::DFT_uint32,0,4);

        uint32	ret		= 0;
        //uint64	count	= result->getRowCount();

        result->getNextRow(binding,&ret);

        mDatabase->destroyDataBinding(binding);

        _handleAddFriendDBReply(ret,asyncContainer->mString);
    }
    break;

    case OCQuery_RemoveFriend:
    {
        swganh::database::DataBinding* binding = mDatabase->createDataBinding(1);
        binding->addField(swganh::database::DFT_uint32,0,4);

        uint32	ret		= 0;
        //uint64	count	= result->getRowCount();

        result->getNextRow(binding,&ret);

        mDatabase->destroyDataBinding(binding);

        _handleRemoveFriendDBReply(ret,asyncContainer->mString);
    }
    break;

    case OCQuery_AddIgnore:
    {
        swganh::database::DataBinding* binding = mDatabase->createDataBinding(1);
        binding->addField(swganh::database::DFT_uint32,0,4);

        uint32	ret		= 0;
        //uint64	count	= result->getRowCount();

        result->getNextRow(binding,&ret);

        mDatabase->destroyDataBinding(binding);

        _handleAddIgnoreDBReply(ret,asyncContainer->mString);
    }
    break;

    case OCQuery_RemoveIgnore:
    {
        swganh::database::DataBinding* binding = mDatabase->createDataBinding(1);
        binding->addField(swganh::database::DFT_uint32,0,4);

        uint32	ret		= 0;
        //uint64	count	= result->getRowCount();

        result->getNextRow(binding,&ret);

        mDatabase->destroyDataBinding(binding);

        _handleRemoveIgnoreDBReply(ret,asyncContainer->mString);
    }
    break;

   

    default:
    {
    }
    break;
    }

    mDBAsyncContainerPool.free(asyncContainer);
}



//=============================================================================





