HxRoute104_MrBrineysHouse_EventScript_Briney::
	lock
	faceplayer
	goto_if_set FLAG_HX_DEFEATED_RUSTBORO_GYM, HxRoute104_MrBrineysHouse_EventScript_BrineyOffer
	msgbox HxRoute104_MrBrineysHouse_Text_BrineyNotYet, MSGBOX_DEFAULT
	release
	end

HxRoute104_MrBrineysHouse_EventScript_BrineyOffer::
	msgbox HxRoute104_MrBrineysHouse_Text_BrineyOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HxRoute104_MrBrineysHouse_EventScript_BrineyDecline
	msgbox HxRoute104_MrBrineysHouse_Text_BrineySail, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	warp MAP_HX_DEWFORD_TOWN, 12, 10
	waitstate
	release
	end

HxRoute104_MrBrineysHouse_EventScript_BrineyDecline::
	msgbox HxRoute104_MrBrineysHouse_Text_BrineyDecline, MSGBOX_DEFAULT
	release
	end

HxRoute104_MrBrineysHouse_Text_BrineyNotYet::
	.string "MR. BRINEY: Ahoy! The sea is calm,\n"
	.string "but my boat's not going anywhere\l"
	.string "until the tide turns.\p"
	.string "Come see me again once you've dealt\n"
	.string "with the RUSTBORO GYM.$"

HxRoute104_MrBrineysHouse_Text_BrineyOffer::
	.string "MR. BRINEY: Ahoy, young sailor!\p"
	.string "I heard the commotion at DEVON CORP.\n"
	.string "A TEAM AQUA boat slipped out of port\l"
	.string "heading for DEWFORD!\p"
	.string "Want me to take you across?$"

HxRoute104_MrBrineysHouse_Text_BrineySail::
	.string "MR. BRINEY: Then hold on tight!\n"
	.string "Full sail to DEWFORD!$"

HxRoute104_MrBrineysHouse_Text_BrineyDecline::
	.string "MR. BRINEY: Take your time.\n"
	.string "The sea isn't going anywhere!$"
