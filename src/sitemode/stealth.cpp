/*
Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
                                                                                      //
This file is part of Liberal Crime Squad.                                             //
                                                                                    //
    Liberal Crime Squad is free software; you can redistribute it and/or modify     //
    it under the terms of the GNU General Public License as published by            //
    the Free Software Foundation; either version 2 of the License, or               //
    (at your option) any later version.                                             //
                                                                                    //
    Liberal Crime Squad is distributed in the hope that it will be useful,          //
    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
    GNU General Public License for more details.                                    //
                                                                                    //
    You should have received a copy of the GNU General Public License               //
    along with Liberal Crime Squad; if not, write to the Free Software              //
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/
/*
        This file was created by Chris Johnson (grundee@users.sourceforge.net)
        by copying code from game.cpp.
        To see descriptions of files and functions, see the list at
        the bottom of includes.h in the top src folder.
*/

#include <includes.h>

#include <cursesAlternative.h>
#include <customMaps.h>
#include <constant_strings.h>
#include <gui_constants.h>
#include <set_color_support.h>
extern Log gamelog;
extern vector<Location *> location;
extern short mode;
extern short sitetype;
 vector<string> blew_stealth_check;
/* checks if your liberal activity is noticed */
void noticecheck(int exclude, int difficulty)
{
	if (sitealarm) return;
	char sneak = 0;
	int topi = 0;
	for (int i = 0; i<6; ++i) if (activesquad->squad[i] && activesquad->squad[i]->get_skill(SKILL_STEALTH)>sneak)
		sneak = activesquad->squad[i]->get_skill(SKILL_STEALTH), topi = i;
	for (int e = 0; e < ENCMAX; e++)
	{  //Prisoners shouldn't shout for help.
		if (!strcmp(encounter[e].name, "Prisoner") || e == exclude || encounter[e].exists == false || activesquad->squad[topi]->skill_check(SKILL_STEALTH, difficulty)) continue;
		else
		{
			clearmessagearea();
			set_color(COLOR_RED, COLOR_BLACK, 1);
			moveSixteenOne();
			addstrAlt(encounter[e].name, gamelog);
			addstrAlt(" observes your Liberal activity ", gamelog);
			moveSeventeenOne();
			if (encounter[e].align == ALIGN_CONSERVATIVE)
				addstrAlt("and lets forth a piercing Conservative alarm cry!", gamelog);
			else addstrAlt("and shouts for help!", gamelog);
			gamelog.newline();
			sitealarm = 1;
			getkey();
			break;
		}
	}
}
/* checks if your liberal behavior/attack alienates anyone */
char alienationcheck(char mistake)
{
	if (location[cursite]->siege.siege)return 0;
	char alienate = 0, alienatebig = 0;
	int oldsitealienate = sitealienate;
	vector<int> noticer;
	for (int e = 0; e < ENCMAX; e++)
	{
		// Prisoners should never be alienated by your crimes, as
		// they're happy to have you attacking their place of holding
		//if(encounter[e].type==CREATURE_PRISONER)continue;
		// ...but Prisoners are now spawned with a variety of creature
		// types, so we'll go by name instead
		if (!strcmp(encounter[e].name, "Prisoner")) continue;
		if (encounter[e].exists&&encounter[e].alive && (encounter[e].align == 0 || (encounter[e].align == 1 && mistake)))
			noticer.push_back(e);
	}
	if (len(noticer))
	{
		int n, an;
		do
		{
			an = LCSrandom(len(noticer));
			n = noticer[an];
			noticer.erase(noticer.begin() + an);
			if (encounter[n].align == 1) alienatebig = 1;
			else alienate = 1;
		} while (len(noticer));
		if (alienatebig) sitealienate = 2;
		if (alienate&&sitealienate != 2) sitealienate = 1;
		if (oldsitealienate < sitealienate)
		{
			set_color(COLOR_YELLOW, COLOR_BLACK, 1);
			moveSixteenOne();
			if (sitealienate == 1)addstrAlt("We've alienated the masses here!              ", gamelog);
			else addstrAlt("We've alienated absolutely everyone here!               ", gamelog);
			gamelog.newline();
			moveSeventeenOne();
			addstrAlt("                                                        ");
			sitealarm = 1;
			for (int i = 0; i < ENCMAX; i++)
				if (encounter[i].exists && encounter[i].align != ALIGN_CONSERVATIVE)
					if (encounter[i].align == ALIGN_MODERATE || alienatebig)
						conservatise(encounter[i]);
			if (mode == GAMEMODE_CHASECAR ||
				mode == GAMEMODE_CHASEFOOT) printchaseencounter();
			else printencounter();
			getkey();
		}
	}
	return alienate;
}
/* checks if conservatives see through your disguise */
void disguisecheck(int timer)
{
	int weapon = 0, partysize = squadsize(activesquad), blew_it = -1;
	bool forcecheck = false, spotted = false;
	//int weaponar[6]={0};
	// Only start to penalize the player's disguise/stealth checks after the first turn.
	timer--;
	for (int i = 0; i < 6; i++)
	{
		if (activesquad->squad[i] == NULL)break;
		if (activesquad->squad[i]->is_naked() &&
			activesquad->squad[i]->animalgloss != ANIMALGLOSS_ANIMAL)forcecheck = true;
		int thisweapon = weaponcheck(*activesquad->squad[i]);
		if (thisweapon > weapon)weapon = thisweapon;
		//if(thisweapon==2)weaponar[i]=1;
	}
	// Nothing suspicious going on here
	if (sitealarmtimer == -1 && weapon < 1 && !forcecheck)
	{
		if (!disguisesite(location[cursite]->type) &&
			!(levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED))return;
	}
	char noticed = 0;
	vector<int> noticer;
	for (int e = 0; e < ENCMAX; e++)
	{
		if (encounter[e].type == CREATURE_PRISONER)continue;
		if (encounter[e].exists&&encounter[e].alive&&
			encounter[e].enemy())
		{
			noticer.push_back(e);
		}
	}
	if (len(noticer))
	{
		int n, an;
		do
		{
			an = LCSrandom(len(noticer));
			n = noticer[an];
			noticer.erase(noticer.begin() + an);
			int stealth_difficulty;
			int disguise_difficulty;
			//// TODO Move to XML
			// Determine difficulty based on enemy type
			switch (encounter[n].type)
			{
			default:
				stealth_difficulty = DIFFICULTY_VERYEASY;
				disguise_difficulty = DIFFICULTY_VERYEASY;
				break;
			case CREATURE_SWAT:
			case CREATURE_COP:
			case CREATURE_GANGUNIT:
			case CREATURE_DEATHSQUAD:
				stealth_difficulty = DIFFICULTY_EASY;
				disguise_difficulty = DIFFICULTY_EASY;
				break;
			case CREATURE_PRISONGUARD:
			case CREATURE_BOUNCER:
			case CREATURE_SECURITYGUARD:
				stealth_difficulty = DIFFICULTY_AVERAGE;
				disguise_difficulty = DIFFICULTY_EASY;
				break;
			case CREATURE_AGENT:
				stealth_difficulty = DIFFICULTY_AVERAGE;
				disguise_difficulty = DIFFICULTY_AVERAGE;
				break;
			case CREATURE_NEWSANCHOR:
			case CREATURE_RADIOPERSONALITY:
			case CREATURE_CORPORATE_CEO:
			case CREATURE_JUDGE_CONSERVATIVE:
			case CREATURE_CCS_ARCHCONSERVATIVE:
			case CREATURE_SCIENTIST_EMINENT:
				stealth_difficulty = DIFFICULTY_EASY;
				disguise_difficulty = DIFFICULTY_HARD;
				break;
			case CREATURE_GUARDDOG:
				stealth_difficulty = DIFFICULTY_HEROIC;
				disguise_difficulty = DIFFICULTY_AVERAGE;
				break;
			case CREATURE_SECRET_SERVICE:
				stealth_difficulty = DIFFICULTY_FORMIDABLE;
				disguise_difficulty = DIFFICULTY_FORMIDABLE;
				break;
			}
			// Increase difficulty if Conservatives suspicious...
			if (sitealarmtimer == 1)
			{
				stealth_difficulty += 6;
				disguise_difficulty += 6;
			}
			else if (sitealarmtimer > 1)
			{
				stealth_difficulty += 3;
				disguise_difficulty += 3;
			}
			// Sneaking with a party is hard
			stealth_difficulty += (partysize - 1) * 3;
			// Make the attempt!
			for (int i = 0; i < 6; i++)
			{
				if (activesquad->squad[i] == NULL)break;
				// Try to sneak.
				if (!spotted)
				{
					int result = activesquad->squad[i]->skill_roll(SKILL_STEALTH);
					result -= timer;
					if (fieldskillrate == FIELDSKILLRATE_HARD && result + 1 == stealth_difficulty)
					{// Hard more = You only learn if you just missed, and realize what you did wrong.
						activesquad->squad[i]->train(SKILL_STEALTH, 10);
					}
					if (result < stealth_difficulty)
						spotted = true;
				}
				// Spotted! Act casual.
				if (spotted)
				{
					// Scary weapons are not very casual.
					if (weaponcheck(*activesquad->squad[i]) == 2)
					{
						noticed = true;
						break;
					}
					else
					{
						int result = activesquad->squad[i]->skill_roll(SKILL_DISGUISE);
						result -= timer;
						if (fieldskillrate == FIELDSKILLRATE_HARD && result + 1 == disguise_difficulty)
						{// Hard more = You only learn if you just missed, and realize what you did wrong.
							activesquad->squad[i]->train(SKILL_DISGUISE, 10);
						}
						if (result < disguise_difficulty)
						{
							// That was not very casual, dude.
							if (result < 0)blew_it = i;
							noticed = true;
							break;
						}
					}
				}
			}
			if (noticed) break;
		} while (len(noticer));
		// Give feedback on the Liberal Performance
		if (!spotted)
		{
			for (int i = 0; i < 6; i++)
			{
				if (activesquad->squad[i] == NULL) break;
				switch (fieldskillrate)
				{
				case FIELDSKILLRATE_FAST:
					activesquad->squad[i]->train(SKILL_STEALTH, 40); break;
				case FIELDSKILLRATE_CLASSIC:
					activesquad->squad[i]->train(SKILL_STEALTH, 10); break;
				case FIELDSKILLRATE_HARD:
					activesquad->squad[i]->train(SKILL_STEALTH, 0); break;
				}
			}
			if (timer == 0)
			{
				set_color(COLOR_CYAN, COLOR_BLACK, 1);
				moveSixteenOne();
				if (partysize > 1)
					addstrAlt("The squad", gamelog);
				else
					addstrAlt(activesquad->squad[0]->name, gamelog);
				addstrAlt(" fades into the shadows.", gamelog);
				gamelog.newline();
				getkey();
			}
		}
		else
		{
			if (blew_it == -1)
			{
				int i;
				for (i = 0; i < 6; i++)
				{
					if (activesquad->squad[i] == NULL) break;
					if (hasdisguise(*(activesquad->squad[i])))
					{
						switch (fieldskillrate)
						{
							//// TODO Move to XML
						case FIELDSKILLRATE_FAST:
							activesquad->squad[i]->train(SKILL_DISGUISE, 50); break;
						case FIELDSKILLRATE_CLASSIC:
							activesquad->squad[i]->train(SKILL_DISGUISE, 10); break;
						case FIELDSKILLRATE_HARD:
							activesquad->squad[i]->train(SKILL_DISGUISE, 0); break;
						}
					}
				}
			}
			if (blew_it != -1 && LCSrandom(2))
			{
				set_color(COLOR_YELLOW, COLOR_BLACK, 1);
				moveSixteenOne();
				addstrAlt(activesquad->squad[blew_it]->name, gamelog);
				addstrAlt(pickrandom(blew_stealth_check), gamelog);
				gamelog.newline();
				getkey();
			}
			else if (!noticed)
			{
				set_color(COLOR_CYAN, COLOR_BLACK, 1);
				moveSixteenOne();
				if (partysize > 1)
					addstrAlt("The squad", gamelog);
				else
					addstrAlt(activesquad->squad[0]->name, gamelog);
				addstrAlt(" acts natural.", gamelog);
				gamelog.newline();
				getkey();
			}
		}
		if (!noticed)return;
		clearmessagearea();
		set_color(COLOR_RED, COLOR_BLACK, 1);
		moveSixteenOne();
		addstrAlt(encounter[n].name, gamelog);
		if (sitealarmtimer != 0 && weapon < 1 && encounter[n].type != CREATURE_GUARDDOG)
		{
			if ((sitetype == SITE_RESIDENTIAL_TENEMENT ||
				sitetype == SITE_RESIDENTIAL_APARTMENT ||
				sitetype == SITE_RESIDENTIAL_APARTMENT_UPSCALE) &&
				levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				sitealarm = 1;
				addstrAlt(" shouts in alarm at the squad's Liberal Trespassing!", gamelog);
			}
			else
			{
				addstrAlt(" looks at the Squad suspiciously.", gamelog);
				int time;
				time = 20 + LCSrandom(10) - encounter[n].get_attribute(ATTRIBUTE_INTELLIGENCE, true)
					- encounter[n].get_attribute(ATTRIBUTE_WISDOM, true);
				if (time < 1)time = 1;
				if (sitealarmtimer > time || sitealarmtimer == -1)sitealarmtimer = time;
				else
				{
					if (sitealarmtimer > 5) sitealarmtimer -= 5;
					if (sitealarmtimer <= 5)sitealarmtimer = 0;
				}
			}
		}
		else
		{
			if (weapon&&encounter[n].type != CREATURE_GUARDDOG)
			{
				addstrAlt(" sees the Squad's Liberal Weapons ", gamelog);
				moveSeventeenOne();
				if (encounter[n].align == ALIGN_CONSERVATIVE)
					addstrAlt("and lets forth a piercing Conservative alarm cry!", gamelog);
				else
					addstrAlt("and shouts for help!", gamelog);
				for (int i = 0; i < 6; i++)
				{
					if (activesquad->squad[i] == NULL)break;
					//if(weaponar[i])
					//{
					// Illegal weapon crimes disabled
					//criminalize(*activesquad->squad[i],LAWFLAG_GUNCARRY);
					//}
				}
			}
			else
			{
				addstrAlt(" looks at the Squad with Intolerance ", gamelog);
				moveSeventeenOne();
				if (encounter[n].align == ALIGN_CONSERVATIVE)
				{
					if (encounter[n].type == CREATURE_GUARDDOG)
						addstrAlt("and launches into angry Conservative barking!", gamelog);
					else
						addstrAlt("and lets forth a piercing Conservative alarm cry!", gamelog);
				}
				else
					addstrAlt("and shouts for help!", gamelog);
			}
			gamelog.newline();
			sitealarm = 1;
		}
		getkey();
	}
}
char weapon_in_character(const string& wtype, const string& atype)
{
	//// TODO Move to XML
	if (atype == tag_ARMOR_LABCOAT && wtype == tag_WEAPON_SYRINGE)
		return CREATURE_SCIENTIST_LABTECH;
	if (atype == tag_ARMOR_BLACKROBE && wtype == tag_WEAPON_GAVEL)
		return CREATURE_JUDGE_LIBERAL;
	if (atype == tag_ARMOR_SECURITYUNIFORM && (wtype == tag_WEAPON_REVOLVER_38 ||
		wtype == tag_WEAPON_REVOLVER_44 || wtype == tag_WEAPON_DESERT_EAGLE ||
		wtype == tag_WEAPON_SEMIPISTOL_9MM || wtype == tag_WEAPON_SEMIPISTOL_45 ||
		wtype == tag_WEAPON_NIGHTSTICK || wtype == tag_WEAPON_MP5_SMG))
		return CREATURE_SECURITYGUARD;
	if ((atype == tag_ARMOR_POLICEUNIFORM || atype == tag_ARMOR_POLICEARMOR) &&
		(wtype == tag_WEAPON_REVOLVER_38 || wtype == tag_WEAPON_REVOLVER_44 ||
			wtype == tag_WEAPON_DESERT_EAGLE || wtype == tag_WEAPON_SEMIPISTOL_9MM ||
			wtype == tag_WEAPON_SEMIPISTOL_45 || wtype == tag_WEAPON_NIGHTSTICK ||
			wtype == tag_WEAPON_SHOTGUN_PUMP))
		return CREATURE_COP;
	if (wtype == tag_WEAPON_SMG_MP5 || wtype == tag_WEAPON_CARBINE_M4)
	{
		if (atype == tag_ARMOR_SWATARMOR)
			return CREATURE_SWAT;
		else if (atype == tag_ARMOR_SECURITYUNIFORM && lawList[LAW_GUNCONTROL] == -2)
			return CREATURE_SECURITYGUARD;
		else if (atype == tag_ARMOR_MILITARY || atype == tag_ARMOR_ARMYARMOR)
			return CREATURE_SOLDIER;
	}
	if (wtype == tag_WEAPON_AUTORIFLE_M16 || wtype == tag_WEAPON_SEMIRIFLE_AR15)
	{
		// Military
		if (atype == tag_ARMOR_MILITARY || atype == tag_ARMOR_ARMYARMOR)
			return CREATURE_SOLDIER;
		if (atype == tag_ARMOR_DEATHSQUADUNIFORM)
			return CREATURE_DEATHSQUAD;
	}
	if (wtype == tag_WEAPON_AXE && atype == tag_ARMOR_BUNKERGEAR)
		return CREATURE_FIREFIGHTER;
	if (wtype == tag_WEAPON_FLAMETHROWER && atype == tag_ARMOR_BUNKERGEAR && lawList[LAW_FREESPEECH] == -2)
		return CREATURE_FIREFIGHTER;
	if (atype == tag_ARMOR_PRISONGUARD && (wtype == tag_WEAPON_SMG_MP5 ||
		wtype == tag_WEAPON_SHOTGUN_PUMP || wtype == tag_WEAPON_NIGHTSTICK))
		return CREATURE_PRISONGUARD;
	if ((atype == tag_ARMOR_OVERALLS || atype == tag_ARMOR_WIFEBEATER) &&
		(wtype == tag_WEAPON_TORCH || wtype == tag_WEAPON_PITCHFORK ||
			(lawList[LAW_GUNCONTROL] == -2 && wtype == tag_WEAPON_SHOTGUN_PUMP)))
		return CREATURE_HICK;
	if (wtype == tag_WEAPON_SHANK && atype == tag_ARMOR_PRISONER)
		return CREATURE_PRISONER;
	if (wtype == tag_WEAPON_CHAIN &&
		(atype == tag_ARMOR_WORKCLOTHES || atype == tag_ARMOR_HARDHAT))
		return CREATURE_WORKER_FACTORY_UNION;
	if (wtype == tag_WEAPON_CARBINE_M4 && atype == tag_ARMOR_SEALSUIT)
		return CREATURE_SEAL;
	return -1;
}
/* checks if a creature's weapon is suspicious */
char weaponcheck(const Creature &cr, bool metaldetect)
{
	bool suspicious = cr.get_weapon().is_suspicious();
	bool concealed = cr.weapon_is_concealed();
	char incharacter = weapon_in_character(cr.get_weapon().get_itemtypename(), cr.get_armor().get_itemtypename());
	//bool illegal = cr.get_weapon().get_legality() < lawList[LAW_GUNCONTROL];
	// If your disguise is inappropriate to the current location,
	// then being in character isn't sufficient
	if (hasdisguise(cr) == false)
		incharacter = -1;
	if (suspicious)
	{
		if (concealed && !metaldetect)
			return 0; // Hidden weapon, nothing to see
		else if (incharacter >= 0)
			return 1; // You look like you're supposed to have that weapon
		else
			return 2; // Looks like trouble
	}
	return 0; // Nothing to see here
}
/* checks if a creature's uniform is appropriate to the location */
char hasdisguise(const Creature &cr)
{
	short type = -1;
	if (cursite >= 0)type = location[cursite]->type;
	char uniformed = 0;
	// Never uniformed in battle colors
	//if(activesquad->stance==SQUADSTANCE_BATTLECOLORS)
	//   return 0;
	if (location[cursite]->siege.siege)
	{
		switch (location[cursite]->siege.siegetype)
		{
		case SIEGE_CIA:
		{
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKSUIT)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKDRESS)uniformed = 1;
			break;
		}
		case SIEGE_CORPORATE:
		{
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_MILITARY)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_ARMYARMOR)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_SEALSUIT)uniformed = 1;
			break;
		}
		case SIEGE_HICKS:
		{
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_CLOTHES)uniformed = 2;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_OVERALLS)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_WIFEBEATER)uniformed = 1;
			break;
		}
		case SIEGE_CCS:
		{
			// CCS has trained in anticipation of this tactic
			// There is no fooling them
			// (They pull this shit all the time in their own sieges)
			uniformed = 0;
			break;
		}
		case SIEGE_POLICE:
		{
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_SWATARMOR&&
				location[cursite]->siege.escalationstate == 0)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_MILITARY&&
				location[cursite]->siege.escalationstate > 0)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_ARMYARMOR&&
				location[cursite]->siege.escalationstate > 0)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_SEALSUIT&&
				location[cursite]->siege.escalationstate > 0)uniformed = 1;
			break;
		}
		case SIEGE_FIREMEN:
		{
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_BUNKERGEAR)uniformed = 1;
			break;
		}
		}
	}
	else
	{
		//// TODO Move to XML
		if ((!cr.is_naked() || cr.animalgloss == ANIMALGLOSS_ANIMAL)
			&& cr.get_armor().get_itemtypename() != tag_ARMOR_HEAVYARMOR)uniformed = 1;
		switch (type)
		{
		case SITE_INDUSTRY_WAREHOUSE:
		case SITE_RESIDENTIAL_SHELTER:
			uniformed = 1;
			break;
		case SITE_LABORATORY_COSMETICS:
		case SITE_LABORATORY_GENETIC:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_LABCOAT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = (location[cursite]->highsecurity ? 1 : 2);
			}
			break;
		case SITE_GOVERNMENT_POLICESTATION:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEARMOR)uniformed = 1;
				if (lawList[LAW_POLICEBEHAVIOR] == -2 && lawList[LAW_DEATHPENALTY] == -2 &&
					cr.get_armor().get_itemtypename() == tag_ARMOR_DEATHSQUADUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SWATARMOR)uniformed = (location[cursite]->highsecurity ? 1 : 2);
			}
			break;
		case SITE_GOVERNMENT_WHITE_HOUSE:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKSUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPSUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVESUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVEDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_MILITARY)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_ARMYARMOR)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SEALSUIT)uniformed = 1;
			}
			break;
		case SITE_GOVERNMENT_COURTHOUSE:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKROBE)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKSUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPSUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVESUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVEDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEARMOR)uniformed = 1;
				if (lawList[LAW_POLICEBEHAVIOR] == -2 && lawList[LAW_DEATHPENALTY] == -2 &&
					cr.get_armor().get_itemtypename() == tag_ARMOR_DEATHSQUADUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SWATARMOR)uniformed = (location[cursite]->highsecurity ? 1 : 2);
			}
			break;
		case SITE_GOVERNMENT_PRISON:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (lawList[LAW_POLICEBEHAVIOR] == -2 && lawList[LAW_DEATHPENALTY] == -2)
				{
					if (cr.get_armor().get_itemtypename() == tag_ARMOR_LABCOAT)uniformed = 1;
				}
				else if (cr.get_armor().get_itemtypename() == tag_ARMOR_PRISONGUARD)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_PRISONER)uniformed = 1;
			}
			break;
		case SITE_GOVERNMENT_ARMYBASE:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_MILITARY)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_ARMYARMOR)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SEALSUIT)uniformed = 1;
			}
			break;
		case SITE_GOVERNMENT_INTELLIGENCEHQ:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKSUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKDRESS)uniformed = 1;
			}
			break;
		case SITE_GOVERNMENT_FIRESTATION:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_BUNKERGEAR)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_WORKCLOTHES)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_OVERALLS)uniformed = 1;
				if (location[cursite]->highsecurity)
				{
					if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEUNIFORM)uniformed = 1;
					if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEARMOR)uniformed = 1;
					if (lawList[LAW_POLICEBEHAVIOR] == -2 && lawList[LAW_DEATHPENALTY] == -2 &&
						cr.get_armor().get_itemtypename() == tag_ARMOR_DEATHSQUADUNIFORM)uniformed = 1;
					if (cr.get_armor().get_itemtypename() == tag_ARMOR_SWATARMOR)uniformed = 1;
				}
			}
			break;
		case SITE_BUSINESS_BANK:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPSUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVESUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVEDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEARMOR)uniformed = 1;
				if (lawList[LAW_POLICEBEHAVIOR] == -2 && lawList[LAW_DEATHPENALTY] == -2 &&
					cr.get_armor().get_itemtypename() == tag_ARMOR_DEATHSQUADUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SWATARMOR)uniformed = (location[cursite]->highsecurity ? 1 : 2);
				if (location[cursite]->highsecurity)
				{
					if (cr.get_armor().get_itemtypename() == tag_ARMOR_CIVILLIANARMOR)uniformed = 1;
				}
			}
			break;
		case SITE_BUSINESS_CIGARBAR:
			uniformed = 0;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVESUIT)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPSUIT)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVEDRESS)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPDRESS)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKSUIT)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_BLACKDRESS)uniformed = 1;
			break;
		case SITE_INDUSTRY_SWEATSHOP:
			uniformed = 0;
			if (cr.is_naked())uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = 1;
			break;
		case SITE_INDUSTRY_POLLUTER:
			uniformed = 0;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_WORKCLOTHES)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_HARDHAT)uniformed = 1;
			if (location[cursite]->highsecurity)
			{
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = 1;
			}
			break;
		case SITE_INDUSTRY_NUCLEAR:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_LABCOAT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CIVILLIANARMOR)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_HARDHAT)uniformed = 1;
			}
			break;
		case SITE_CORPORATE_HEADQUARTERS:
			uniformed = 0;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVESUIT)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPSUIT)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVEDRESS)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPDRESS)uniformed = 1;
			break;
		case SITE_CORPORATE_HOUSE:
			uniformed = 0;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVESUIT)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVEDRESS)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = 1;
			if (cr.get_armor().get_itemtypename() == tag_ARMOR_SERVANTUNIFORM)uniformed = 1;
			if (location[cursite]->highsecurity)
			{
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_MILITARY)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_ARMYARMOR)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SEALSUIT)uniformed = 1;
			}
			break;
		case SITE_MEDIA_AMRADIO:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVESUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPSUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVEDRESS)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_CHEAPDRESS)uniformed = 1;
			}
			break;
		case SITE_MEDIA_CABLENEWS:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)
			{
				uniformed = 0;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_SECURITYUNIFORM)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVESUIT)uniformed = 1;
				if (cr.get_armor().get_itemtypename() == tag_ARMOR_EXPENSIVEDRESS)uniformed = 1;
			}
			break;
		case SITE_RESIDENTIAL_TENEMENT:
		case SITE_RESIDENTIAL_APARTMENT:
		case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
			if (levelmap[locx][locy][locz].flag & SITEBLOCK_RESTRICTED)uniformed = 0;
			break;
		default:
			break;
		}
	}
	if (!uniformed)
	{
		if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEUNIFORM)uniformed = 2;
		if (cr.get_armor().get_itemtypename() == tag_ARMOR_POLICEARMOR)uniformed = 2;
		if (lawList[LAW_POLICEBEHAVIOR] == -2 && lawList[LAW_DEATHPENALTY] == -2 &&
			cr.get_armor().get_itemtypename() == tag_ARMOR_DEATHSQUADUNIFORM)uniformed = 2;
		if (location[cursite]->highsecurity &&
			cr.get_armor().get_itemtypename() == tag_ARMOR_SWATARMOR)uniformed = 2;
		// Loop over adjacent locations to check if fire is anywhere in sight?
		// Or perhaps have a site fire alarm? - Nick
		if ((levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_START ||
			levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_END ||
			levelmap[locx][locy][locz].flag & SITEBLOCK_FIRE_PEAK) &&
			cr.get_armor().get_itemtypename() == tag_ARMOR_BUNKERGEAR)uniformed = 1;
	}
	if (uniformed)
	{
		int qlmax = cr.get_armor().get_quality_levels();
		int ql = cr.get_armor().get_quality() + cr.get_armor().is_damaged();
		if (ql > qlmax)  // Shredded clothes are obvious
		{
			uniformed = 0;
		}
		else if ((ql - 1) * 2 > qlmax) // poor clothes make a poor disguise
		{
			uniformed++;
		}
		if (uniformed > 2)
			uniformed = 0;
	}
	return uniformed;
}
/* returns true if the entire site is not open to public */
char disguisesite(long type)
{
	switch (type)
	{
	case SITE_LABORATORY_COSMETICS:
	case SITE_LABORATORY_GENETIC:
	case SITE_GOVERNMENT_PRISON:
	case SITE_GOVERNMENT_INTELLIGENCEHQ:
	case SITE_INDUSTRY_SWEATSHOP:
	case SITE_INDUSTRY_POLLUTER:
	case SITE_CORPORATE_HEADQUARTERS:
	case SITE_CORPORATE_HOUSE:
	case SITE_BUSINESS_CIGARBAR:
		return 1;
	}
	return 0;
}