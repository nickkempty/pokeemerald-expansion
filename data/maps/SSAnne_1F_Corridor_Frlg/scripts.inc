SSAnne_1F_Corridor_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Corridor_EventScript_WorkerM::
	msgbox SSAnne_1F_Corridor_Text_LeStrongSilentType, MSGBOX_NPC
	end

SSAnne_1F_Corridor_EventScript_Sailor::
	msgbox SSAnne_1F_Corridor_Text_PassengersAreRestless, MSGBOX_NPC
	end

SSAnne_1F_Corridor_EventScript_ZkGrunt::
	trainerbattle_single TRAINER_ZK_SSANNE_MAGMA_1, SSAnne_1F_Corridor_Text_ZkGruntIntro, SSAnne_1F_Corridor_Text_ZkGruntDefeat
	msgbox SSAnne_1F_Corridor_Text_ZkGruntAfter, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Corridor_Text_LeStrongSilentType::
	.string "Bonjour!\n"
	.string "I am le waiter on this ship!\p"
	.string "I will be happy to serve you\n"
	.string "anything you please!\p"
	.string "… … …\n"
	.string "Ah! Le strong silent type!$"

SSAnne_1F_Corridor_Text_PassengersAreRestless::
	.string "The passengers are restless from\n"
	.string "this long voyage.\p"
	.string "You might be challenged by the\n"
	.string "more bored ones!$"


SSAnne_1F_Corridor_Text_ZkGruntIntro::
    .string "Huh? This sailor suit is real!\n"
    .string "…OK, it's not. Out of my way!$"

SSAnne_1F_Corridor_Text_ZkGruntDefeat::
    .string "My disguise was perfect…$"

SSAnne_1F_Corridor_Text_ZkGruntAfter::
    .string "Too late! The RED ORB is already\n"
    .string "out of the CAPTAIN's safe!$"
