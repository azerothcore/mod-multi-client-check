/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/***
 *  @project: Firestorm Freelance
 *  @author: Meltie2013 (github) aka Lilcrazy
 *  @copyright: 2017
 */

#include "ScriptMgr.h"
#include "Config.h"
#include "World.h"
#include "WorldSession.h"
#include "Chat.h"
#include "Player.h"
#include "StringFormat.h"

// Check to see if the player is attempting to multi-box
class multi_login_check : public PlayerScript
{
public:
    multi_login_check() : PlayerScript("multi_login_check") { }

    void OnLogin(Player* player) override
    {
        if (AccountMgr::IsGMAccount(player->GetSession()->GetSecurity()))
            return;

        uint32 CountLimit = sConfigMgr->GetOption<uint32>("Disallow.Multiple.Client", 0);

        if (CountLimit)
        {
            // There is a Limit of Client
            if (sConfigMgr->GetOption<bool>("Disallow.Multiple.Client.Announce", true))
            {
                ChatHandler(player->GetSession()).SendSysMessage(Acore::StringFormatFmt("This Server Max Account of Same IP Is: {}", CountLimit));
            }

            uint32 count = 1;

            for (auto const& [accID, session] : sWorld->GetAllSessions())
            {
                Player* _player = session->GetPlayer();
                if (!_player || _player == player)
                {
                    continue;
                }

                // If Remote Address matches, remove the player from the world
                if (player->GetSession()->GetRemoteAddress() == _player->GetSession()->GetRemoteAddress() && ++count > CountLimit)
                {
                    player->GetSession()->KickPlayer();
                }
            }
        }
    }
};

void AddMultiLoginCheckScripts()
{
    new multi_login_check;
}
