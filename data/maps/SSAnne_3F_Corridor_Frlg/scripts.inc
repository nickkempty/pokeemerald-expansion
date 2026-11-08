SSAnne_3F_Corridor_Frlg_MapScripts::
	.byte 0

SSAnne_3F_Corridor_EventScript_Sailor::
	msgbox SSAnne_3F_Corridor_Text_CaptainTeachesCutToMons, MSGBOX_NPC
	end

SSAnne_3F_Corridor_EventScript_ZkGrunt::
	trainerbattle_single TRAINER_ZK_SSANNE_MAGMA_2, SSAnne_3F_Corridor_Text_ZkGruntIntro, SSAnne_3F_Corridor_Text_ZkGruntDefeat
	msgbox SSAnne_3F_Corridor_Text_ZkGruntAfter, MSGBOX_AUTOCLOSE
	end

SSAnne_3F_Corridor_Text_CaptainTeachesCutToMons::
	.string "Our CAPTAIN is a sword master.\n"
	.string "He's awesome at using CUT.\p"
	.string "They say he even teaches CUT\n"
	.string "to POKéMON!$"


SSAnne_3F_Corridor_Text_ZkGruntIntro::
    .string "TABITHA said nobody gets onto\n"
    .string "the deck. Nobody!$"

SSAnne_3F_Corridor_Text_ZkGruntDefeat::
    .string "Ugh! The boss is gonna be mad…$"

SSAnne_3F_Corridor_Text_ZkGruntAfter::
    .string "MAXIE himself came to pick up\n"
    .string "the RED ORB. You can't stop him!$"
