# 1 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 2 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
.set LOCALID_DAYCARE_WOMAN, 1
# 3 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 4 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_MapScripts::
# 5 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	.byte 0
# 6 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 7 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_EventScript_DaycareWoman::
# 8 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_questlog EventScript_ReleaseEnd
# 9 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	lock
# 10 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	faceplayer
# 11 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, GetDaycareState
# 12 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, DAYCARE_EGG_WAITING, FourIsland_PokemonDayCare_EggWaiting
# 13 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, FourIsland_PokemonDayCare_OneMonInDaycare
# 14 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, DAYCARE_TWO_MONS, FourIsland_PokemonDayCare_TwoMonsInDaycare
# 15 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_WouldYouLikeUsToRaiseMon, MSGBOX_YESNO
# 16 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, YES, FourIsland_PokemonDayCare_GiveMonToRaise
# 17 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_FineThenComeAgain
# 18 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 19 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 20 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 21 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_GiveMonToRaise::
# 22 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, CountPartyNonEggMons
# 23 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, 1, FourIsland_PokemonDayCare_OnlyOneMonInParty
# 24 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_WhichMonShouldWeRaise
# 25 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	fadescreen FADE_TO_BLACK
# 26 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	special ChooseSendDaycareMon
# 27 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	waitstate
# 28 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_ge VAR_0x8004, PARTY_SIZE, FourIsland_PokemonDayCare_ComeAgain
# 29 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, CountPartyAliveNonEggMons_IgnoreVar0x8004Slot
# 30 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, 0, FourIsland_PokemonDayCare_OnlyOneAliveMonInParty
# 31 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_0x8005, GetSelectedMonNicknameAndSpecies
# 32 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	waitse
# 33 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	playmoncry VAR_0x8005, CRY_MODE_NORMAL
# 34 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_WellRaiseYourMon
# 35 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	waitmoncry
# 36 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	special StoreSelectedPokemonInDaycare
# 37 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	callnative UpdateFollowingPokemon
# 38 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	incrementgamestat GAME_STAT_USED_DAYCARE
# 39 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, GetDaycareState
# 40 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, FourIsland_PokemonDayCare_CanRaiseOneMore
# 41 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 42 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 43 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 44 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_ComeAgain::
# 45 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_ComeAgain
# 46 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 47 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 48 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 49 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_CanRaiseOneMore::
# 50 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_WeCanRaiseOneMore, MSGBOX_YESNO
# 51 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, YES, FourIsland_PokemonDayCare_GiveMonToRaise
# 52 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto FourIsland_PokemonDayCare_ComeAgain
# 53 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 54 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 55 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_OnlyOneMonInParty::
# 56 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_YouHaveJustOneMon
# 57 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 58 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 59 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 60 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_OnlyOneAliveMonInParty::
# 61 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_WhatWillYouBattleWith
# 62 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 63 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 64 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 65 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_EggWaiting::
# 66 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_HusbandWasLookingForYou
# 67 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 68 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 69 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 70 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_MonHasGrownXLevels::
# 71 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_YourMonHasGrownXLevels
# 72 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	return
# 73 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 74 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_DisplayLevelsGained::
# 75 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, GetNumLevelsGainedFromDaycare
# 76 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	call_if_ne VAR_RESULT, 0, FourIsland_PokemonDayCare_MonHasGrownXLevels
# 77 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	return
# 78 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 79 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_OneMonInDaycare::
# 80 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_GoodToSeeYou
# 81 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	setvar VAR_0x8004, 0
# 82 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	call FourIsland_PokemonDayCare_DisplayLevelsGained
# 83 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_WeCanRaiseOneMore, MSGBOX_YESNO
# 84 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, YES, FourIsland_PokemonDayCare_GiveMonToRaise
# 85 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_TakeYourMonBack, MSGBOX_YESNO
# 86 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, YES, FourIsland_PokemonDayCare_TryRetrieveMon
# 87 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto FourIsland_PokemonDayCare_ComeAgain
# 88 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 89 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 90 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_TryRetrieveMon::
# 91 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, CalculatePlayerPartyCount
# 92 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, PARTY_SIZE, FourIsland_PokemonDayCare_NoRoomInParty
# 93 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, GetDaycareState
# 94 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	setvar VAR_0x8004, 0
# 95 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, FourIsland_PokemonDayCare_CostPrompt
# 96 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	special ShowDaycareLevelMenu
# 97 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	waitstate
# 98 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	copyvar VAR_0x8004, VAR_RESULT
# 99 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, DAYCARE_EXITED_LEVEL_MENU, FourIsland_PokemonDayCare_ComeAgain
# 100 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto FourIsland_PokemonDayCare_CostPrompt
# 101 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 102 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 103 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_CostPrompt::
# 104 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	special GetDaycareCost
# 105 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_ItWillCostX, MSGBOX_YESNO
# 106 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, YES, FourIsland_PokemonDayCare_CheckEnoughMoney
# 107 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto FourIsland_PokemonDayCare_ComeAgain
# 108 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 109 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 110 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_CheckEnoughMoney::
# 111 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, IsEnoughForCostInVar0x8005
# 112 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, FourIsland_PokemonDayCare_RetrieveMon
# 113 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_NotEnoughMoney
# 114 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 115 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 116 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 117 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_RetrieveMon::
# 118 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	applymovement LOCALID_DAYCARE_WOMAN, FourIsland_PokemonDayCare_Movement_RetrieveMon
# 119 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	waitmovement 0
# 120 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, TakePokemonFromDaycare
# 121 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	special SubtractMoneyFromVar0x8005
# 122 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	playse SE_RS_SHOP
# 123 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_HeresYourMon
# 124 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	waitse
# 125 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	playmoncry VAR_RESULT, CRY_MODE_NORMAL
# 126 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	textcolor NPC_TEXT_COLOR_NEUTRAL
# 127 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_TookBackMon
# 128 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	call EventScript_RestorePrevTextColor
# 129 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	waitmoncry
# 130 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, GetDaycareState
# 131 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, DAYCARE_ONE_MON, FourIsland_PokemonDayCare_AskRetrieveOtherMon
# 132 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto FourIsland_PokemonDayCare_ComeAgain
# 133 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 134 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 135 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_AskRetrieveOtherMon::
# 136 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_TakeOtherOneBackToo, MSGBOX_YESNO
# 137 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, YES, FourIsland_PokemonDayCare_TryRetrieveMon
# 138 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto FourIsland_PokemonDayCare_ComeAgain
# 139 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 140 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 141 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_NoRoomInParty::
# 142 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_YourPartyIsFull
# 143 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 144 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 145 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 146 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_Movement_RetrieveMon::
# 147 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 148 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 149 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	face_right
# 150 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 151 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 152 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	face_left
# 153 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 154 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 155 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	face_up
# 156 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	walk_slower_up
# 157 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	set_invisible
# 158 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 159 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 160 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 161 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 162 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	delay_16
# 163 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	face_down
# 164 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	set_visible
# 165 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	walk_slower_down
# 166 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	step_end
# 167 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 168 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
@ Unused. Possibly a commented script, or a typo end added to the above Movement script
# 169 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_EventScript_UnusedEnd::
# 170 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 171 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 172 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_TwoMonsInDaycare::
# 173 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_GoodToSeeYou
# 174 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	setvar VAR_0x8004, 0
# 175 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	call FourIsland_PokemonDayCare_DisplayLevelsGained
# 176 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	setvar VAR_0x8004, 1
# 177 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	call FourIsland_PokemonDayCare_DisplayLevelsGained
# 178 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_TakeYourMonBack, MSGBOX_YESNO
# 179 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, YES, FourIsland_PokemonDayCare_TryRetrieveMon
# 180 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_ComeAgain
# 181 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 182 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
# 183 "data/maps/FourIsland_PokemonDayCare/scripts.pory"

# 184 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
@ Unused
# 185 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
FourIsland_PokemonDayCare_EventScript_UnusedRetrieveMon::
# 186 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	special ShowDaycareLevelMenu
# 187 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	waitstate
# 188 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	goto_if_eq VAR_RESULT, 2, FourIsland_PokemonDayCare_ComeAgain
# 189 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	copyvar VAR_0x8004, VAR_RESULT
# 190 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	specialvar VAR_RESULT, TakePokemonFromDaycare
# 191 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_HeresYourMon
# 192 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	msgbox DayCare_Text_ComeAgain
# 193 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	release
# 194 "data/maps/FourIsland_PokemonDayCare/scripts.pory"
	end
