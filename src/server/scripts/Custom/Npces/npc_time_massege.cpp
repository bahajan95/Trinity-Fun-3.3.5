/*
*******************************
*		Overhack_dev 	  	  *
*							  *
* fixed By overhack_Dev       *
*							  *
*YID : overhack_Dev@yahoo.com *
*							  *
*Tell : 0930 408 5657         *
*******************************
*/
#include "ScriptPCH.h"

#define DELAY   120 // seconds

class npc_Messenger : public CreatureScript
{
public:
    npc_Messenger() : CreatureScript("npc_Messenger") {}

    struct npc_MessengerAI : public ScriptedAI
    {
        uint32 timer;

        npc_MessengerAI(Creature* creature) : ScriptedAI(creature)
        {
            timer = 0;
        }

        void Reset()
        {
            timer = DELAY*IN_MILLISECONDS;
        }

        void UpdateAI(uint32 diff)
        {
            if(diff > timer)
            {
                me->MonsterSay("Welcome To Server Ghost Wolf", LANG_UNIVERSAL, 0);
                Reset();
            }
            else
                timer -= diff;	
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_MessengerAI(creature);
    }
};

void AddSC_npc_Messenger()
{
    new npc_Messenger();
}  