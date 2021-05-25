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

// Check to see if the player is attempting to multi-box
class multi_login_check : public PlayerScript
{
public:
    multi_login_check() : PlayerScript("multi_login_check") { }

	void OnLogin(Player* player)
    {
        uint32 CountLimit = sConfigMgr->GetIntDefault("Disallow.Multiple.Client", 0);
	if (CountLimit>0)
        {
	// There is a Limit of Client
	    if (sConfigMgr->GetBoolDefault("Disallow.Multiple.Client.Announce", true))
            {	
               	std::ostringstream stream;
		stream << "This Server Max Account of Same IP Is:" << CountLimit;
		ChatHandler(player->GetSession()).SendSysMessage(stream.str().c_str());
            }	
            SessionMap sessions = sWorld->GetAllSessions();
            uint32 count = 1;
	    for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
            {
                if (Player* login = itr->second->GetPlayer())
                {
                    if (player != login)
                    {
                        // If Remote Address matches, remove the player from the world
                        if (player->GetSession()->GetRemoteAddress() == login->GetSession()->GetRemoteAddress() && !AccountMgr::IsGMAccount(player->GetSession()->GetSecurity()))
                            {
				if (++count > CountLimit)
				{
				    player->GetSession()->KickPlayer();
				}
			    }
                    }
                }
            }
        }
	}
};

void AddMultiLoginCheckScripts()
{
    new multi_login_check;
}
