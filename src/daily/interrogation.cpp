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

#include <includes.h>
#include "creature/creature.h"

#include "common/ledgerEnums.h"
#include "common/ledger.h"

#include "log/log.h"
#include "common/commondisplayCreature.h"
// for void printhealthstat(Creature &,int,int,char);

 #include "common/getnames.h"
// for std::string getview(short ,bool );

#include "common/commonactionsCreature.h"
// for  void sleeperize_prompt(Creature &,Creature &,int);


#include <cursesAlternative.h>
#include <customMaps.h>
#include <set_color_support.h>
#include "common/creaturePoolCreature.h"
#include "locations/locationsPool.h"
#include "common/musicClass.h"
extern Log gamelog;
extern MusicClass music;
extern int stat_recruits;
extern int stat_kills;
extern class Ledger ledger;
extern short lawList[LAWNUM];
 vector<string> execution;
 vector<string> feels_sick;
 vector<string> low_heart_torture_props;
 vector<string> screaming;
 vector<string> beat_with_props;
 vector<string> words_meaning_screaming;
 vector<string> words_to_scream;
 vector<string> prays;
 vector<string> prays_on_drugs;
 vector<string> use_props;
 vector<string> resist_drugs;
 vector<string> partial_conversion;
 vector<string> broods_over_death;
 vector<string> self_wounding;
 vector<string> good_trip;
 vector<vector<string>> fall_in_love;
 vector<vector<string>> bad_trip;
 vector<vector<string>> good_trip_companion;
 vector<string> smarter_than_you_one_line;
 vector<vector<string>> smarter_than_you;
 vector<string> develops_hatred_one_line;
 vector<vector<string>> develops_hatred;
 vector<string> interrogater_shows_compassion_one_line;
 vector<vector<string>> interrogater_shows_compassion;
 vector<vector<string>> cling_to_interrogater;
 vector<string> clinging_one_line;
 vector<vector<string>> cling_to_religion;
 vector<string> cling_to_religion_one_line;
 vector<vector<string>> discuss;
 vector<string> vanilla_recruit;
 vector<vector<string>> cling_to_business;
 vector<vector<string>> cling_to_science;
 vector<string> cling_to_business_one_line;
 vector<string> cling_to_science_one_line;


 const string interrogate = "interrogation\\";
 const int DOUBLE_LINE = 2;
 vector<file_and_text_collection> interrogate_text_file_collection = {

	 /*interrogation.cpp*/
	 customText(&execution, interrogate + "execution.txt"),
	 customText(&feels_sick, interrogate + "feels_sick.txt"),
	 customText(&low_heart_torture_props, interrogate + "low_heart_torture_props.txt"),
	 customText(&screaming, interrogate + "screaming.txt"),
	 customText(&beat_with_props, interrogate + "beat_with_props.txt"),
	 customText(&words_meaning_screaming, interrogate + "words_meaning_screaming.txt"),
	 customText(&words_to_scream, interrogate + "words_to_scream.txt"),
	 customText(&prays, interrogate + "prays.txt"),
	 customText(&prays_on_drugs, interrogate + "prays_on_drugs.txt"),
	 customText(&use_props, interrogate + "use_props.txt"),
	 customText(&resist_drugs, interrogate + "resist_drugs.txt"),
	 customText(&partial_conversion, interrogate + "partial_conversion.txt"),
	 customText(&broods_over_death, interrogate + "broods_over_death.txt"),
	 customText(&self_wounding, interrogate + "self_wounding.txt"),
	 customText(&good_trip, interrogate + "good_trip.txt"),
	 customText(&fall_in_love, interrogate + "fall_in_love.txt", DOUBLE_LINE),
	 customText(&bad_trip, interrogate + "bad_trip.txt", DOUBLE_LINE),
	 customText(&good_trip_companion, interrogate + "good_trip_companion.txt", DOUBLE_LINE),
	 customText(&smarter_than_you_one_line, interrogate + "smarter_than_you_one_line.txt"),
	 customText(&smarter_than_you, interrogate + "smarter_than_you.txt", DOUBLE_LINE),
	 customText(&develops_hatred_one_line, interrogate + "develops_hatred_one_line.txt"),
	 customText(&develops_hatred, interrogate + "develops_hatred.txt", DOUBLE_LINE),
	 customText(&interrogater_shows_compassion_one_line, interrogate + "interrogater_shows_compassion_one_line.txt"),
	 customText(&interrogater_shows_compassion, interrogate + "interrogater_shows_compassion.txt", DOUBLE_LINE),
	 customText(&cling_to_interrogater, interrogate + "cling_to_interrogater.txt", DOUBLE_LINE),
	 customText(&clinging_one_line, interrogate + "clinging_one_line.txt"),
	 customText(&cling_to_religion, interrogate + "cling_to_religion.txt", DOUBLE_LINE),
	 customText(&cling_to_religion_one_line, interrogate + "cling_to_religion_one_line.txt"),
	 customText(&discuss, interrogate + "discuss.txt", DOUBLE_LINE),
	 customText(&vanilla_recruit, interrogate + "vanilla_recruit.txt"),
	 customText(&cling_to_business_one_line, interrogate + "cling_to_business_one_line.txt"),
	 customText(&cling_to_science_one_line, interrogate + "cling_to_science_one_line.txt"),
	 customText(&cling_to_business, interrogate + "cling_to_business.txt", DOUBLE_LINE),
	 customText(&cling_to_science, interrogate + "cling_to_science.txt", DOUBLE_LINE),
 };

 string feels_sick_and;

extern string singleDot;
extern string AND;
extern string singleSpace;
extern string commaSpace;
enum InterrogationTechnqiues
{
   TECHNIQUE_TALK,
   TECHNIQUE_RESTRAIN,
   TECHNIQUE_BEAT,
   TECHNIQUE_PROPS,
   TECHNIQUE_DRUGS,
   TECHNIQUE_KILL
};


// Clear sidebar
void clear_interrogation_sidebar()
{
	for (int i = 4; i < 23; i++)
		mvaddstrAlt(i, 40, "                                  "); // 34 spaces
}
// Shows the interrogation data at the right side of the screen
void show_interrogation_sidebar(Creature * cr, Creature * a)
{
	clear_interrogation_sidebar();
	int y = 4;
	map<long, struct float_zero>& rapport = cr->activity.intr()->rapport;
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(y,  40, "Prisoner: ");
	set_color_easy(RED_ON_BLACK_BRIGHT);
	addstrAlt(cr->name);
	moveAlt(y += 2, 40);
	set_color_easy(WHITE_ON_BLACK);
	addstrAlt("Health: ");
	printhealthstat(*cr, y, 48, 0);
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(++y,  40, "Heart: ");
	addstrAlt(cr->get_attribute(ATTRIBUTE_HEART, true));
	mvaddstrAlt(++y,  40, "Wisdom: ");
	addstrAlt(cr->get_attribute(ATTRIBUTE_WISDOM, true));
	mvaddstrAlt(++y,  40, "Health: ");
	addstrAlt(cr->get_attribute(ATTRIBUTE_HEALTH, true));
	moveAlt(y = 13, 40);
	set_color_easy(WHITE_ON_BLACK);
	addstrAlt("Lead Interrogator: ");
	set_color_easy(GREEN_ON_BLACK_BRIGHT);
	addstrAlt(a->name);
	moveAlt(y += 2, 40);
	set_color_easy(WHITE_ON_BLACK);
	addstrAlt("Health: ");
	printhealthstat(*a, y, 48, 0);
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(++y,  40, "Psychology Skill: ");
	addstrAlt(a->get_skill(SKILL_PSYCHOLOGY));
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(++y,  40, "Heart: ");
	addstrAlt(a->get_attribute(ATTRIBUTE_HEART, true));
	mvaddstrAlt(++y,  40, "Wisdom: ");
	addstrAlt(a->get_attribute(ATTRIBUTE_WISDOM, true));
	mvaddstrAlt(++y,  40, "Outfit: ");
	addstrAlt(a->get_armor().get_name());
	moveAlt(y += 2, 40);
	// What would 4, 2, 0, -2, -3, and/or -5 mean? (Some of these may not exist) -- LK
	// These are greater than and less than comparisons, so they are testing ranges -Fox
	string line_one;
	string line_two = "";
	if (rapport[a->id] > 3)
	{
		line_one = "The Conservative clings helplessly ";
		line_two = "to ";
		line_two += string(a->name);
		line_two += " as its only friend.";
	}
	else if (rapport[a->id] > 1)
	{
		line_one = "The Conservative likes ";
		line_two += string(a->name);
		line_one += singleDot;
	}
	else if (rapport[a->id] > -1)
	{
		line_one = "The Conservative is uncooperative ";
		line_two = "toward ";
		line_two += string(a->name);
		line_two += singleDot;
	}
	else if (rapport[a->id] > -4)
	{
		line_one = "The Conservative hates ";
		line_two += string(a->name);
		line_one += singleDot;
	}
	else
	{
		line_one = "The Conservative would like to ";
		line_two, "murder ";
		line_two += string(a->name);
		line_two += singleDot;
	}
	addstrAlt(line_one);
	if (len(line_two)) {
		mvaddstrAlt(++y,  40, line_two);
	}
}
/* hostage tending */
void tendhostage(Creature *cr, char &clearformess)
{
	music.play(MUSIC_INTERROGATION);
	vector<Creature *> temppool;
	int p;
	Creature *a = NULL;
	interrogation* &intr = cr->activity.intr();
	bool(&techniques)[6] = intr->techniques;
	int& druguse = intr->druguse;
	map<long, struct float_zero>& rapport = intr->rapport;
	findAllTendersToThisHostage(cr, temppool);
	if (cr->location == -1)
	{
		delete cr;
		return;
	}
	//possible hostage escape attempt if unattended or unrestrained
	if (!len(temppool) || !techniques[TECHNIQUE_RESTRAIN])
	{
		//CHECK FOR HOSTAGE ESCAPE
		if (LCSrandom(200) + 25 * len(temppool) <
			cr->get_attribute(ATTRIBUTE_INTELLIGENCE, true) +
			cr->get_attribute(ATTRIBUTE_AGILITY, true) +
			cr->get_attribute(ATTRIBUTE_STRENGTH, true) &&
			cr->joindays >= 5)
		{
			hostageEscapes(cr, clearformess);
			//delete interrogation data
			delete intr;

			return;
		}
		if (!len(temppool)) return;
	}
	clearformess = 1;
	eraseAlt();
	set_color_easy(WHITE_ON_BLACK_BRIGHT);
	mvaddstrAlt(0,  0, "The Education of ");
	addstrAlt(cr->name);
	addstrAlt(": Day ");
	addstrAlt(cr->joindays);
	getkeyAlt();
	set_color_easy(WHITE_ON_BLACK);
	char turned = 0;
	int y = 3;
	{
		//each day, the attack roll is initialized to the number of days of the stay with
		//the LCS -- they will eventually break, but also eventually become too traumatized
		//to continue
		int p, business = 0, religion = 0, science = 0, attack = 0;
		int* _attack = new int[len(temppool)];
		for (p = 0; p<len(temppool); p++)
		{
			_attack[p] = 0;
			if (temppool[p])
				if (temppool[p]->alive)
				{
					if (temppool[p]->get_skill(SKILL_BUSINESS)>business)
						business = temppool[p]->get_skill(SKILL_BUSINESS);
					if (temppool[p]->get_skill(SKILL_RELIGION) > religion)
						religion = temppool[p]->get_skill(SKILL_RELIGION);
					if (temppool[p]->get_skill(SKILL_SCIENCE) > science)
						science = temppool[p]->get_skill(SKILL_SCIENCE);
					_attack[p] = (temppool[p]->get_attribute(ATTRIBUTE_HEART, true) -
						temppool[p]->get_attribute(ATTRIBUTE_WISDOM, true) +
						temppool[p]->get_skill(SKILL_PSYCHOLOGY) * 2);
					_attack[p] += temppool[p]->get_armor().get_interrogation_basepower();
					if (_attack[p] < 0) _attack[p] = 0;
					if (_attack[p] > attack) attack = _attack[p];
				}
		}
		vector<int> goodp;
		for (p = 0; p < len(temppool); p++)
			if (temppool[p])
				if (temppool[p]->alive&&_attack[p] == attack)
					goodp.push_back(p);
		a = temppool[pickrandom(goodp)];
		attack += len(temppool);
		attack += cr->joindays;
		attack += business - cr->get_skill(SKILL_BUSINESS);
		attack += religion - cr->get_skill(SKILL_RELIGION);
		attack += science - cr->get_skill(SKILL_SCIENCE);
		attack += a->skill_roll(SKILL_PSYCHOLOGY) - cr->skill_roll(SKILL_PSYCHOLOGY);
		attack += cr->attribute_roll(ATTRIBUTE_HEART);
		attack -= cr->attribute_roll(ATTRIBUTE_WISDOM) * 2;
		while (true)
		{
			y = 2;
			if (techniques[TECHNIQUE_KILL])
			{
				set_color_easy(RED_ON_BLACK_BRIGHT);
				moveAlt(y, 0); y += 2; addstrAlt("The Execution of an Automaton         ");
			}
			else
			{
				set_color_easy(YELLOW_ON_BLACK_BRIGHT);
				moveAlt(y, 0); y += 2; addstrAlt("Selecting a Liberal Interrogation Plan");
			}
			if (techniques[TECHNIQUE_KILL])set_color_easy(BLACK_ON_BLACK_BRIGHT);
			else set_color_easy(techniques[TECHNIQUE_TALK] ? WHITE_ON_BLACK_BRIGHT : WHITE_ON_BLACK);
			mvaddstrAlt(y++,  0, "A - ");
			if (!techniques[TECHNIQUE_TALK]) addstrAlt("No Verbal Contact     ");
			else addstrAlt("Attempt to Convert");
			if (!techniques[TECHNIQUE_KILL])set_color_easy(techniques[TECHNIQUE_RESTRAIN] ? WHITE_ON_BLACK_BRIGHT : WHITE_ON_BLACK);
			mvaddstrAlt(y++,  0, "B - ");
			if (!techniques[TECHNIQUE_RESTRAIN]) addstrAlt("No ");
			addstrAlt("Physical Restraints   ");
			if (!techniques[TECHNIQUE_KILL])set_color_easy(techniques[TECHNIQUE_BEAT] ? WHITE_ON_BLACK_BRIGHT : WHITE_ON_BLACK);
			mvaddstrAlt(y++,  0, "C - ");
			if (!techniques[TECHNIQUE_BEAT]) addstrAlt("Not ");
			addstrAlt("Violently Beaten    ");
			if (!techniques[TECHNIQUE_KILL])set_color_easy(techniques[TECHNIQUE_PROPS] ? WHITE_ON_BLACK_BRIGHT : WHITE_ON_BLACK);
			mvaddstrAlt(y,  0, "D - ");
			if (!techniques[TECHNIQUE_PROPS])addstrAlt("No ");
			addstrAlt("Expensive Props     ");
			mvaddstrAlt(y++,  27, "($250)");
			if (!techniques[TECHNIQUE_KILL])set_color_easy(techniques[TECHNIQUE_DRUGS] ? WHITE_ON_BLACK_BRIGHT : WHITE_ON_BLACK);
			mvaddstrAlt(y,  0, "E - ");
			if (!techniques[TECHNIQUE_DRUGS])addstrAlt("No ");
			addstrAlt("Hallucinogenic Drugs    ");
			moveAlt(y, 28); y += 2;
			addstrAlt("($50)");
			if (techniques[TECHNIQUE_KILL])set_color_easy(RED_ON_BLACK_BRIGHT);
			else set_color_easy(WHITE_ON_BLACK);
			moveAlt(y, 0); y += 2; addstrAlt("K - Kill the Hostage");
			set_color_easy(WHITE_ON_BLACK);
			mvaddstrAlt(y++,  0, "Press Enter to Confirm the Plan");
			show_interrogation_sidebar(cr, a);
			int c = getkeyAlt();
			if (c >= 'a'&&c <= 'e') techniques[c - 'a'] = !techniques[c - 'a'];
			if (c == 'k') techniques[TECHNIQUE_KILL] = !techniques[TECHNIQUE_KILL];
			if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
		}
		if (techniques[TECHNIQUE_PROPS] && ledger.get_funds() >= 250)
			ledger.subtract_funds(250, EXPENSE_HOSTAGE);
		else techniques[TECHNIQUE_PROPS] = 0;
		if (techniques[TECHNIQUE_DRUGS] && ledger.get_funds() >= 50)
			ledger.subtract_funds(50, EXPENSE_HOSTAGE);
		else techniques[TECHNIQUE_DRUGS] = 0;
		if (techniques[TECHNIQUE_KILL]) // Kill the Hostage
		{
			eraseAlt();
			set_color_easy(WHITE_ON_BLACK_BRIGHT);
			mvaddstrAlt(0,  0, "The Final Education of ", gamelog);
			addstrAlt(cr->name, gamelog);
			addstrAlt(": Day ", gamelog);
			addstrAlt(cr->joindays, gamelog);
			gamelog.newline();
			a = NULL;
			for (int i = 0; i < len(temppool); i++)
				if (LCSrandom(50) < temppool[i]->juice ||
					LCSrandom(9) + 1 >= temppool[i]->get_attribute(ATTRIBUTE_HEART, 0))
				{
					a = temppool[i]; break;
				}
			if (a)
			{
				//delete interrogation information
				delete intr;
				set_color_easy(MAGENTA_ON_BLACK);
				cr->die();
				stat_kills++;
				mvaddstrAlt(y++,  0, a->name, gamelog);
				addstrAlt(" executes ", gamelog);
				addstrAlt(cr->name, gamelog);
				addstrAlt(" by ", gamelog);
				addstrAlt(pickrandom(execution), gamelog);
				//show_interrogation_sidebar(cr,a);
				getkeyAlt();
				if (LCSrandom(a->get_attribute(ATTRIBUTE_HEART, false)) > LCSrandom(3))
				{
					gamelog.newline();
					set_color_easy(GREEN_ON_BLACK_BRIGHT);
					mvaddstrAlt(y++,  0, a->name, gamelog);
					addstrAlt(feels_sick_and, gamelog);
					a->adjust_attribute(ATTRIBUTE_HEART, -1);
					mvaddstrAlt(y++,  0, pickrandom(feels_sick), gamelog);
				}
				else if (!LCSrandom(3))
				{
					gamelog.newline();
					set_color_easy(CYAN_ON_BLACK_BRIGHT);
					mvaddstrAlt(y++,  0, a->name, gamelog);
					addstrAlt(" grows colder.", gamelog);
					a->adjust_attribute(ATTRIBUTE_WISDOM, +1);
				}
				gamelog.nextMessage();
			}
			else
			{
				set_color_easy(YELLOW_ON_BLACK);
				mvaddstrAlt(y++,  0, "There is no one able to get up the nerve to ", gamelog);
				mvaddstrAlt(y++,  0, "execute ", gamelog);
				addstrAlt(cr->name, gamelog);
				addstrAlt(" in cold blood.", gamelog);
				gamelog.nextMessage();
				//Interrogation will continue as planned, with
				//these restrictions:
				techniques[TECHNIQUE_TALK] = 0; //don't talk to them today
				techniques[TECHNIQUE_BEAT] = 0; //don't beat them today
				techniques[TECHNIQUE_DRUGS] = 0; //don't administer drugs
												 //Food and restraint settings will be applied as normal
			}
			//show_interrogation_sidebar(cr,a);
			getkeyAlt();
			set_color_easy(WHITE_ON_BLACK);
			mvaddstrAlt(24,  0, "Press any key to reflect on this.");
			getkeyAlt();
			if (!cr->alive)
			{

				setAllCreatureActivities(ACTIVITY_NONE, temppool);
				delete[] _attack;
				return;
			}
		}
		eraseAlt();
		set_color_easy(WHITE_ON_BLACK_BRIGHT);
		mvaddstrAlt(0,  0, "The Education of ", gamelog);
		addstrAlt(cr->name, gamelog);
		addstrAlt(": Day ", gamelog);
		addstrAlt(cr->joindays, gamelog);
		gamelog.newline();
		moveAlt((y = 2)++, 0);
		addstrAlt("The Automaton", gamelog);
		if (techniques[TECHNIQUE_RESTRAIN]) // Restraint
		{
			addstrAlt(" is tied hands and feet to a metal chair", gamelog);
			mvaddstrAlt(y++,  0, "in the middle of a back room.", gamelog);
			gamelog.newline();
			attack += 5;
		}
		else
		{
			addstrAlt(" is locked in a back room ", gamelog);
			mvaddstrAlt(y++,  0, "converted into a makeshift cell.", gamelog);
			gamelog.newline();
		}
		//show_interrogation_sidebar(cr,a);
		getkeyAlt();
		if (techniques[TECHNIQUE_DRUGS]) // Hallucinogenic drugs
		{
			mvaddstrAlt(++y,  0, "It is subjected to dangerous hallucinogens.", gamelog);
			gamelog.newline();
			int drugbonus = 10 + a->get_armor().get_interrogation_drugbonus(); // we won't apply this JUST yet
																			   //Possible permanent health damage
			if (LCSrandom(50) < ++druguse)
			{
				cr->adjust_attribute(ATTRIBUTE_HEALTH, -1);
				moveAlt(++y, 0);
				getkeyAlt();
				addstrAlt(cr->name, gamelog);
				addstrAlt(" foams at the mouth and its eyes roll back in its skull.", gamelog);
				gamelog.newline();
				moveAlt(++y, 0);
				getkeyAlt();
				Creature* doctor = a; // the lead interrogator is doctor by default
				int maxskill = doctor->get_skill(SKILL_FIRSTAID);
				for (int i = 0; i<len(temppool); i++) // search for the best doctor
					if (temppool[i]->get_skill(SKILL_FIRSTAID)>maxskill)
						maxskill = (doctor = temppool[i])->get_skill(SKILL_FIRSTAID); // we found a doctor
				if (cr->get_attribute(ATTRIBUTE_HEALTH, false) <= 0 || !maxskill) // he's dead, Jim
				{
					if (maxskill)
					{ // we have a real doctor but the patient is still dead anyway
						addstrAlt(doctor->name, gamelog);
						addstrAlt(" uses a defibrillator repeatedly but ", gamelog);
						addstrAlt(cr->name, gamelog);
						addstrAlt(" flatlines.", gamelog);
					}
					else
					{
						addstrAlt(doctor->name, gamelog);
						if (lawList[LAW_FREESPEECH] == -2)
							addstrAlt(" has a panic attack and [makes a stinky].", gamelog);
						else
						{
							addstrAlt(" has a panic attack and shits ", gamelog);
							addstrAlt(doctor->hisher(), gamelog);
							addstrAlt(" pants.", gamelog);
						}
					}
					gamelog.newline();
					moveAlt(++y, 0);
					getkeyAlt();
					set_color_easy(YELLOW_ON_BLACK_BRIGHT);
					if (maxskill)
					{
						addstrAlt("It is a lethal overdose in ", gamelog);
						addstrAlt(cr->name, gamelog);
						addstrAlt("'s weakened state.", gamelog);
					}
					else
					{
						addstrAlt(cr->name, gamelog);
						addstrAlt(" dies due to ", gamelog);
						addstrAlt(doctor->name, gamelog);
						addstrAlt("'s incompetence at first aid.", gamelog);
					}
					gamelog.newline();
					cr->die();
				}
				else
				{
					addstrAlt(doctor->name, gamelog);
					if (doctor->skill_check(SKILL_FIRSTAID, DIFFICULTY_CHALLENGING)) // is the doctor AWESOME?
					{
						doctor->train(SKILL_FIRSTAID, 5 * max(10 - doctor->get_skill(SKILL_FIRSTAID), 0), 10); // can train up to 10
						addstrAlt(" deftly rescues it from cardiac arrest with a defibrillator.", gamelog); // not long enough for near-death experience
						gamelog.newline();
						moveAlt(++y, 0);
						getkeyAlt();
						addstrAlt(doctor->name, gamelog);
						addstrAlt(" skillfully saves ", gamelog);
						addstrAlt(cr->name, gamelog);
						addstrAlt(" from any health damage.", gamelog);
						cr->adjust_attribute(ATTRIBUTE_HEALTH, +1); // no permanent health damage from a skilled doctor
						techniques[TECHNIQUE_DRUGS] = (druguse = drugbonus = 0); // drugs eliminated from the system (zeroing out 3 variables with 1 line of code)
					}
					else
					{
						doctor->train(SKILL_FIRSTAID, 5 * max(5 - doctor->get_skill(SKILL_FIRSTAID), 0), 5); // can train up to 5
						addstrAlt(" clumsily rescues it from cardiac arrest with a defibrillator.", gamelog);
						gamelog.newline();
						moveAlt(++y, 0);
						getkeyAlt();
						addstrAlt(cr->name, gamelog);
						if (cr->get_skill(SKILL_RELIGION)) // the patient was out long enough to have a near-death experience
							addstrAlt(" had a near-death experience and met God in heaven.", gamelog);
						else addstrAlt(" had a near-death experience and met John Lennon.", gamelog);
						drugbonus *= 2; // the near-death experience doubles the drug bonus, since the hostage is spaced out afterwards
					}
					rapport[doctor->id] += 0.5f; // rapport bonus for having life saved by doctor
					gamelog.newline();
				}
			}
			attack += drugbonus; // now we finally apply the drug bonus
			moveAlt(++y, 0);
			//show_interrogation_sidebar(cr,a);
			getkeyAlt();
		}
		if (techniques[TECHNIQUE_BEAT] && !turned&&cr->alive) // Beating
		{
			moveAlt(++y, 0);
			int forceroll = 0;
			bool tortured = 0;
			for (int i = 0; i < len(temppool); i++)
			{
				//add interrogator's strength to beating strength
				forceroll += temppool[i]->attribute_roll(ATTRIBUTE_STRENGTH);
				//reduce rapport with each interrogator
				rapport[temppool[i]->id] -= 0.4f;
			}
			//Torture captive if lead interrogator has low heart
			//and you funded using extra supplies
			//
			//Yeah, you kinda don't want this to happen
			if (!(a->attribute_check(ATTRIBUTE_HEART, DIFFICULTY_EASY)) && techniques[TECHNIQUE_PROPS])
			{
				tortured = true;
				//Torture more devastating than normal beating
				forceroll *= 5;
				//Extremely bad for rapport with lead interrogator
				rapport[a->id] -= 3;
				addstrAlt(a->name, gamelog);
				addstrAlt(singleSpace, gamelog);
				addstrAlt(pickrandom(low_heart_torture_props), gamelog);
				addstrAlt(commaSpace, gamelog);
				mvaddstrAlt(++y,  0, "screaming \"", gamelog);
				addstrAlt(pickrandom(screaming), gamelog);
				addstrAlt("! ", gamelog);
				addstrAlt(pickrandom(screaming), gamelog);
				addstrAlt("!\" in its face.", gamelog);
				gamelog.newline();
				if (cr->get_attribute(ATTRIBUTE_HEART, true) > 1) cr->adjust_attribute(ATTRIBUTE_HEART, -1);
				if (cr->get_attribute(ATTRIBUTE_WISDOM, true) > 1) cr->adjust_attribute(ATTRIBUTE_WISDOM, -1);
			}
			else
			{
				if (len(temppool) == 1)
				{
					addstrAlt(temppool[0]->name, gamelog);
					addstrAlt(" beats", gamelog);
				}
				else if (len(temppool) == 2)
				{
					addstrAlt(temppool[0]->name, gamelog);
					addstrAlt(AND, gamelog);
					addstrAlt(temppool[1]->name, gamelog);
					addstrAlt(" beat", gamelog);
				}
				else
				{
					addstrAlt(cr->name);
					addstrAlt("'s guards beat", gamelog);
				}
				addstrAlt(" the Automaton", gamelog);
				if (techniques[TECHNIQUE_PROPS])
				{
					addstrAlt(singleSpace, gamelog);
					addstrAlt(pickrandom(beat_with_props), gamelog);
				}
				addstrAlt(commaSpace, gamelog);
				mvaddstrAlt(++y,  0, pickrandom(words_meaning_screaming), gamelog);
				for (int i = 0; i < 3; i++)
				{
					addstrAlt(pickrandom(words_to_scream), gamelog);
					if (i < 2) addstrAlt("! ", gamelog);
				}
				addstrAlt("!\" in its face.", gamelog);
				gamelog.newline();
			}
			y++;
			cr->blood -= (5 + LCSrandom(5)) * (1 + techniques[TECHNIQUE_PROPS]);
			//show_interrogation_sidebar(cr,a);
			getkeyAlt();
			if (!(cr->attribute_check(ATTRIBUTE_HEALTH, forceroll)))
			{
				if (cr->skill_check(SKILL_RELIGION, forceroll))
				{
					mvaddstrAlt(y++,  0, cr->name, gamelog);
					if (!techniques[TECHNIQUE_DRUGS])
					{
						addstrAlt(singleSpace, gamelog);
						addstrAlt(pickrandom(prays), gamelog);
						gamelog.newline();
					}
					else
					{
						addstrAlt(singleSpace, gamelog);
						addstrAlt(pickrandom(prays_on_drugs), gamelog);
						gamelog.newline();
					}
				}
				else if (forceroll >
					cr->get_attribute(ATTRIBUTE_WISDOM, true) * 3 +
					cr->get_attribute(ATTRIBUTE_HEART, true) * 3 +
					cr->get_attribute(ATTRIBUTE_HEALTH, true) * 3)
				{
					mvaddstrAlt(y++,  0, cr->name, gamelog);
					switch (LCSrandom(4))
					{
					case 0:addstrAlt(" screams helplessly for ", gamelog);
						if (techniques[TECHNIQUE_DRUGS])addstrAlt("John Lennon's mercy.", gamelog);
						else if (cr->get_skill(SKILL_RELIGION))addstrAlt("God's mercy.", gamelog);
						else addstrAlt("mommy.", gamelog);
						break;
					case 1:
						if (techniques[TECHNIQUE_RESTRAIN]) addstrAlt(" goes limp in the restraints.", gamelog);
						else addstrAlt(" curls up in the corner and doesn't move.", gamelog); break;
					case 2:
						if (techniques[TECHNIQUE_DRUGS] && !LCSrandom(5)) addstrAlt(" barks helplessly.", gamelog);
						else addstrAlt(" cries helplessly.", gamelog); break;
					case 3:
						if (techniques[TECHNIQUE_DRUGS] && !LCSrandom(3)) addstrAlt(" wonders about apples.", gamelog);
						else addstrAlt(" wonders about death.", gamelog);
						break;
					}
					gamelog.newline();
					if (cr->get_attribute(ATTRIBUTE_HEART, false) > 1) cr->adjust_attribute(ATTRIBUTE_HEART, -1);
					if (LCSrandom(2) && cr->juice > 0) { if ((cr->juice -= forceroll) < 0) cr->juice = 0; }
					else if (cr->get_attribute(ATTRIBUTE_WISDOM, false) > 1)
					{
						cr->set_attribute(ATTRIBUTE_WISDOM, cr->get_attribute(ATTRIBUTE_WISDOM, false) - (forceroll / 10));
						if (cr->get_attribute(ATTRIBUTE_WISDOM, false) < 1) cr->set_attribute(ATTRIBUTE_WISDOM, 1);
					}
					if (LocationsPool::getInstance().isLocationMapped(cr->worklocation) == 0 && !LCSrandom(5))
					{
						//show_interrogation_sidebar(cr,a);
						getkeyAlt();
						mvaddstrAlt(y++,  0, a->name, gamelog);
						addstrAlt(" beats information out of the pathetic thing.", gamelog);
						gamelog.newline();
						moveAlt(y++, 0);
						getkeyAlt();
						if (LocationsPool::getInstance().getLocationType(cr->worklocation) <= SITE_RESIDENTIAL_SHELTER)
							addstrAlt("Unfortunately, none of it is useful to the LCS.", gamelog);
						else
						{
							addstrAlt("A detailed map has been created of ", gamelog);
							addstrAlt(LocationsPool::getInstance().getLocationName(cr->worklocation), gamelog);
							addstrAlt(singleDot, gamelog);
						}
						gamelog.newline();
						LocationsPool::getInstance().setLocationMappedAndUnhidden(cr->worklocation);
					}
				}
				else
				{
					mvaddstrAlt(y++,  0, cr->name, gamelog);
					addstrAlt(" seems to be getting the message.", gamelog);
					gamelog.newline();
					if (cr->juice > 0) if ((cr->juice -= forceroll) < 0) cr->juice = 0;
					if (cr->get_attribute(ATTRIBUTE_WISDOM, false) > 1)
					{
						cr->set_attribute(ATTRIBUTE_WISDOM, cr->get_attribute(ATTRIBUTE_WISDOM, false) - (forceroll / 10 + 1));
						if (cr->get_attribute(ATTRIBUTE_WISDOM, false) < 1) cr->set_attribute(ATTRIBUTE_WISDOM, 1);
					}
				}
				if (!(cr->attribute_check(ATTRIBUTE_HEALTH, forceroll / 3)))
				{
					//show_interrogation_sidebar(cr,a);
					getkeyAlt();
					moveAlt(y++, 0);
					if (cr->get_attribute(ATTRIBUTE_HEALTH, false) > 1)
					{
						cr->adjust_attribute(ATTRIBUTE_HEALTH, -1);
						addstrAlt(cr->name, gamelog);
						addstrAlt(" is badly hurt.", gamelog);
						gamelog.newline();
					}
					else
					{
						cr->set_attribute(ATTRIBUTE_HEALTH, 0);
						addstrAlt(cr->name, gamelog);
						addstrAlt("'s weakened body crumbles under the brutal assault.", gamelog);
						gamelog.newline();
						cr->die();
					}
				}
			}
			else
			{
				mvaddstrAlt(y++,  0, cr->name, gamelog);
				addstrAlt(" takes it well.", gamelog);
				gamelog.newline();
			}
			//show_interrogation_sidebar(cr,a);
			getkeyAlt();
			if (tortured && cr->alive)
			{
				if (LCSrandom(a->get_attribute(ATTRIBUTE_HEART, false)) > LCSrandom(3))
				{
					set_color_easy(GREEN_ON_BLACK_BRIGHT);
					mvaddstrAlt(y++,  0, a->name, gamelog);
					addstrAlt(feels_sick_and, gamelog);
					a->adjust_attribute(ATTRIBUTE_HEART, -1);
					mvaddstrAlt(y++,  0, pickrandom(feels_sick), gamelog);
					gamelog.newline();
				}
				else if (!LCSrandom(3))
				{
					set_color_easy(CYAN_ON_BLACK_BRIGHT);
					mvaddstrAlt(y++,  0, a->name, gamelog);
					addstrAlt(" grows colder.", gamelog);
					gamelog.newline();
					a->adjust_attribute(ATTRIBUTE_WISDOM, +1);
				}
				set_color_easy(WHITE_ON_BLACK_BRIGHT);
			}
		}
		// Verbal Interrogation
		if (techniques[TECHNIQUE_TALK] && cr->alive)
		{
			float rapport_temp = rapport[a->id];
			if (!techniques[TECHNIQUE_RESTRAIN])attack += 5;
			attack += int(rapport[a->id] * 3);
			mvaddstrAlt((++y)++,  0, a->name, gamelog);
			if (techniques[TECHNIQUE_PROPS])//props
			{
				attack += 10;
				addstrAlt(singleSpace, gamelog);
				addstrAlt(pickrandom(use_props), gamelog);
				gamelog.newline();
			}
			else
			{
				int which_discuss = LCSrandom(discuss.size() + vanilla_recruit.size());
				if (which_discuss < discuss.size()) {
					addstrAlt(discuss[which_discuss][0], gamelog);
					addstrAlt(getview(LCSrandom(VIEWNUM - 3), true), gamelog);
					addstrAlt(discuss[which_discuss][1], gamelog);
				}
				else {
					addstrAlt(pickrandom(vanilla_recruit), gamelog);
				}
			}
			addstrAlt(cr->name, gamelog);
			addstrAlt(singleDot, gamelog);
			gamelog.newline();
			//Hallucinogenic drugs:
			//Re-interprets lead interrogator
			if (techniques[TECHNIQUE_DRUGS])
			{
				//show_interrogation_sidebar(cr,a);
				getkeyAlt();
				moveAlt(y++, 0);
				if (cr->skill_check(SKILL_PSYCHOLOGY, DIFFICULTY_CHALLENGING))
				{
					addstrAlt(cr->name);
					addstrAlt(singleSpace, gamelog);
					addstrAlt(pickrandom(resist_drugs), gamelog);
					gamelog.newline();
				}
				else if ((rapport[a->id] > 1 && !LCSrandom(3)) || !LCSrandom(10))
				{
					rapport_temp = 10;
					switch (LCSrandom(fall_in_love.size() + 1))
					{
					case 0:
						addstrAlt(cr->name, gamelog);
						addstrAlt(" stammers and ", gamelog);
						techniques[TECHNIQUE_RESTRAIN] ? addstrAlt("talks about hugging ", gamelog) : addstrAlt("hugs ", gamelog);
						addstrAlt(a->name, gamelog);
						addstrAlt(singleDot, gamelog);
						break;
					default:
						int which_love = LCSrandom(fall_in_love.size());
						addstrAlt(cr->name, gamelog);
						addstrAlt(fall_in_love[which_love][0], gamelog);
						addstrAlt(a->name, gamelog);
						addstrAlt(fall_in_love[which_love][1], gamelog);
						break;
					}
					gamelog.newline();
				}
				else if ((rapport[a->id] < -1 && LCSrandom(3)) || !LCSrandom(5))
				{
					attack = 0;
					switch (LCSrandom(bad_trip.size() + 2))
					{
					case 0:
						if (rapport[a->id] < -3)
						{
							addstrAlt(cr->name, gamelog);
							addstrAlt(" begs Hitler to stay and kill ", gamelog);
							addstrAlt(a->name, gamelog);
							addstrAlt(singleDot, gamelog);
						}
						else
						{
							addstrAlt(cr->name, gamelog);
							addstrAlt(" screams for ", gamelog);
							addstrAlt(a->name, gamelog);
							addstrAlt(" to stop looking like Hitler.", gamelog);
						}
						break;
					case 1:
						addstrAlt(cr->name, gamelog);
						if (!techniques[TECHNIQUE_RESTRAIN]) addstrAlt(" curls up and", gamelog);
						addstrAlt(" begs for the nightmare to end.", gamelog);
						break;
					default:
						int which_trip = LCSrandom(bad_trip.size());
						addstrAlt(cr->name, gamelog);
						addstrAlt((bad_trip[which_trip][0]), gamelog);
						addstrAlt(a->name, gamelog);
						addstrAlt((bad_trip[which_trip][1]), gamelog);
						break;
					}
					gamelog.newline();
				}
				else
				{
					int which_good_trip = LCSrandom(good_trip.size() + good_trip_companion.size());
					addstrAlt(cr->name, gamelog);
					if (which_good_trip < good_trip_companion.size()) {
						addstrAlt(good_trip_companion[which_good_trip][0], gamelog);
						addstrAlt(a->name, gamelog);
						addstrAlt(good_trip_companion[which_good_trip][1], gamelog);
					}
					else {
						addstrAlt(pickrandom(good_trip), gamelog);
					}
					gamelog.newline();
				}
			}
			//show_interrogation_sidebar(cr,a);
			getkeyAlt();
			if (cr->get_skill(SKILL_PSYCHOLOGY)>a->get_skill(SKILL_PSYCHOLOGY))
			{
				mvaddstrAlt(y++,  0, cr->name, gamelog);
				int which_smarter = LCSrandom(smarter_than_you.size() + smarter_than_you_one_line.size());
				if (which_smarter < smarter_than_you.size()) {
					addstrAlt(smarter_than_you[which_smarter][0], gamelog);
					addstrAlt(a->name, gamelog);
					addstrAlt(smarter_than_you[which_smarter][1], gamelog);
				}
				else {
					addstrAlt(pickrandom(smarter_than_you_one_line), gamelog);
				}
			}
			//Attempt to convert when the target is brutally treated will
			//just alienate them and make them cynical
			else if (techniques[TECHNIQUE_BEAT] || rapport_temp < -2)
			{
				mvaddstrAlt(y++,  0, cr->name, gamelog);
				int which_hatred = LCSrandom(develops_hatred.size() + develops_hatred_one_line.size());
				if (which_hatred < develops_hatred.size())
				{
					addstrAlt(develops_hatred[which_hatred][0], gamelog);
					addstrAlt(a->name);
					addstrAlt(develops_hatred[which_hatred][1], gamelog);
				}
				else {
					addstrAlt(pickrandom(develops_hatred_one_line), gamelog);
				}
				gamelog.newline();
				if (a->skill_check(SKILL_SEDUCTION, DIFFICULTY_CHALLENGING))
				{
					//show_interrogation_sidebar(cr,a);
					getkeyAlt();
					mvaddstrAlt(y++,  0, a->name, gamelog);
					int which_compassion = LCSrandom(interrogater_shows_compassion.size() + interrogater_shows_compassion_one_line.size());
					if (which_compassion < interrogater_shows_compassion.size()) {
						addstrAlt(interrogater_shows_compassion[which_compassion][0], gamelog);
						addstrAlt(cr->name, gamelog);
						addstrAlt(interrogater_shows_compassion[which_compassion][1], gamelog);
					}
					else {
						addstrAlt(pickrandom(interrogater_shows_compassion_one_line), gamelog);
					}
					gamelog.newline();
					rapport[a->id] += 0.7f;
					if (rapport[a->id]>3)
					{
						//show_interrogation_sidebar(cr,a);
						getkeyAlt();
						mvaddstrAlt(y++,  0, cr->name, gamelog);
						int which_cling = LCSrandom(cling_to_interrogater.size() + clinging_one_line.size());
						if (which_cling < cling_to_interrogater.size()) {
							addstrAlt(cling_to_interrogater[which_cling][0], gamelog);
							addstrAlt(a->name, gamelog);
							addstrAlt(cling_to_interrogater[which_cling][1], gamelog);
						}
						else {
							addstrAlt(pickrandom(clinging_one_line), gamelog);
						}
						gamelog.newline();
						if (rapport[a->id]>5) turned = 1;
					}
				}
				if (cr->get_attribute(ATTRIBUTE_HEART, false) > 1) cr->adjust_attribute(ATTRIBUTE_HEART, -1);
			}
			//Failure to break religious convictions
			else if (cr->get_skill(SKILL_RELIGION) > a->get_skill(SKILL_RELIGION) + a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
			{
				moveAlt(y++, 0);
				int which_cling = LCSrandom(cling_to_religion.size() + cling_to_religion_one_line.size());
				if (which_cling < cling_to_religion.size()) {
					addstrAlt(a->name, gamelog);
					addstrAlt(cling_to_religion[which_cling][0], gamelog);
					addstrAlt(cr->name, gamelog);
					addstrAlt(cling_to_religion[which_cling][1], gamelog);
				}
				else {
					addstrAlt(cr->name, gamelog);
					addstrAlt(pickrandom(cling_to_religion_one_line), gamelog);
				}
				gamelog.newline();
				a->train(SKILL_RELIGION, cr->get_skill(SKILL_RELIGION) * 4);
			}
			//Failure to persuade entrenched capitalists
			else if (cr->get_skill(SKILL_BUSINESS)>a->get_skill(SKILL_BUSINESS) + a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
			{
				moveAlt(y++, 0);
				int which_cling = LCSrandom(cling_to_business.size() + cling_to_business_one_line.size());
				if (which_cling < cling_to_business.size()) {
					addstrAlt(cr->name, gamelog);
					addstrAlt(cling_to_business[which_cling][0], gamelog);
					addstrAlt(a->name, gamelog);
					addstrAlt(cling_to_business[which_cling][1], gamelog);
				}
				else {
					addstrAlt(cr->name, gamelog);
					addstrAlt(pickrandom(cling_to_business_one_line), gamelog);
				}
				gamelog.newline();
				a->train(SKILL_BUSINESS, cr->get_skill(SKILL_BUSINESS) * 4);
			}
			//Failure to persuade scientific minds
			else if (cr->get_skill(SKILL_SCIENCE)>a->get_skill(SKILL_SCIENCE) + a->get_skill(SKILL_PSYCHOLOGY) && !techniques[TECHNIQUE_DRUGS])
			{
				moveAlt(y++, 0);
				int which_cling = LCSrandom(cling_to_science.size() + cling_to_science_one_line.size());
				if (which_cling < cling_to_science.size()) {
					addstrAlt(cr->name, gamelog);
					addstrAlt(cling_to_science[which_cling][0], gamelog);
					addstrAlt(a->name, gamelog);
					addstrAlt(cling_to_science[which_cling][1], gamelog);
				}
				else {
					addstrAlt(cr->name, gamelog);
					addstrAlt(pickrandom(cling_to_science_one_line), gamelog);
				}
				gamelog.newline();
				a->train(SKILL_SCIENCE, cr->get_skill(SKILL_SCIENCE) * 4);
			}
			//Target is swayed by Liberal Reason -- skilled interrogators, time held,
			//and rapport contribute to the likelihood of this
			else if (!(cr->attribute_check(ATTRIBUTE_WISDOM, attack / 6)))
			{
				if (cr->juice>0)
				{
					cr->juice -= attack;
					if (cr->juice < 0) cr->juice = 0;
				}
				if (cr->get_attribute(ATTRIBUTE_HEART, false) < 10)
					cr->adjust_attribute(ATTRIBUTE_HEART, +1);
				//Improve rapport with interrogator
				rapport[a->id] += 1.5;
				//Join LCS??
				//1) They were liberalized
				if (cr->get_attribute(ATTRIBUTE_HEART, true) > cr->get_attribute(ATTRIBUTE_WISDOM, true) + 4) turned = 1;
				//2) They were befriended
				if (rapport[a->id]>4) turned = 1;
				mvaddstrAlt(y++,  0, cr->name, gamelog);
				addstrAlt(pickrandom(partial_conversion), gamelog);
				gamelog.newline();
				if (LocationsPool::getInstance().isLocationMapped(cr->worklocation) == 0 && !LCSrandom(5))
				{
					mvaddstrAlt((++y)++,  0, cr->name, gamelog);
					addstrAlt(" reveals details about the ", gamelog);
					addstrAlt(LocationsPool::getInstance().getLocationName(cr->worklocation), gamelog);
					addstrAlt(singleDot, gamelog);
					gamelog.newline();
					moveAlt(y++, 0);
					if (LocationsPool::getInstance().getLocationType(cr->worklocation) <= SITE_RESIDENTIAL_SHELTER)
					{
						addstrAlt("Unfortunately, none of it is useful to the LCS.", gamelog);
					}
					else
					{
						addstrAlt(a->name, gamelog);
						addstrAlt(" was able to create a map of the site with this information.", gamelog);
					}
					gamelog.newline(); 
					LocationsPool::getInstance().setLocationMappedAndUnhidden(cr->worklocation);
				}
			}
			//Target is not sold on the LCS arguments and holds firm
			//This is the worst possible outcome if you use props
			else if (!(cr->skill_check(SKILL_PERSUASION, a->get_attribute(ATTRIBUTE_HEART, true))) || techniques[TECHNIQUE_PROPS])
			{
				//Not completely unproductive; builds rapport
				rapport[a->id] += 0.2f;
				mvaddstrAlt(y++,  0, cr->name, gamelog);
				addstrAlt(" holds firm.", gamelog);
				gamelog.newline();
			}
			//Target actually wins the argument so successfully that the Liberal
			//interrogator's convictions are the ones that are shaken
			else
			{
				//Consolation prize is that they end up liking the
				//liberal more
				rapport[a->id] += 0.5f;
				a->adjust_attribute(ATTRIBUTE_WISDOM, +1);
				mvaddstrAlt(y++,  0, cr->name, gamelog);
				addstrAlt(" turns the tables on ", gamelog);
				addstrAlt(a->name, gamelog);
				addstrAlt("!", gamelog);
				gamelog.newline();
				//show_interrogation_sidebar(cr,a);
				getkeyAlt();
				mvaddstrAlt(y++,  0, a->name, gamelog);
				addstrAlt(" has been tainted with wisdom!", gamelog);
				gamelog.newline();
			}
			//show_interrogation_sidebar(cr,a);
			getkeyAlt();
		}
		//Lead interrogator gets bonus experience
		if (!techniques[TECHNIQUE_KILL])
		{
			a->train(SKILL_PSYCHOLOGY, attack / 2 + 1);
			//Others also get experience
			for (int i = 0; i < len(temppool); i++) temppool[i]->train(SKILL_PSYCHOLOGY, attack / 4 + 1);
		}
		//Possibly suicidal when heart is down to 1 and prisoner has already been
		//captive for a week without rescue
		if (!turned&&cr->alive&&cr->get_attribute(ATTRIBUTE_HEART, false) <= 1 && LCSrandom(3) && cr->joindays>6)
		{
			moveAlt(++y, 0);
			//can't commit suicide if restrained
			if (LCSrandom(6) || techniques[TECHNIQUE_RESTRAIN])
			{
				set_color_easy(MAGENTA_ON_BLACK);
				addstrAlt(cr->name, gamelog);
				//can't cut self if restrained
				if (!techniques[TECHNIQUE_RESTRAIN] && !LCSrandom(5)) {
					addstrAlt(singleSpace + pickrandom(self_wounding), gamelog); cr->blood -= LCSrandom(15) + 10;
				}
				else {
					addstrAlt(singleSpace + pickrandom(broods_over_death), gamelog);
				}
				gamelog.newline();
			}
			else
			{
				set_color_easy(RED_ON_BLACK_BRIGHT);
				addstrAlt(cr->name, gamelog);
				addstrAlt(" has committed suicide.", gamelog);
				gamelog.newline(); //TODO: Next message?
				cr->die();
			}
			y++;
			//show_interrogation_sidebar(cr,a);
			getkeyAlt();
		}
		//Death
		if (cr->alive == 0 || cr->blood < 1)
		{
			//delete interrogation information
			delete intr;
			cr->die();
			stat_kills++;
			set_color_easy(RED_ON_BLACK_BRIGHT);
			mvaddstrAlt(++y,  0, cr->name, gamelog);
			addstrAlt(" is dead", gamelog);
			if (a)
			{
				addstrAlt(" under ", gamelog);
				addstrAlt(a->name, gamelog);
				addstrAlt("'s interrogation.", gamelog);
			}
			else addstrAlt(singleDot, gamelog);
			set_color_easy(WHITE_ON_BLACK);
			y++;
			//show_interrogation_sidebar(cr,a);
			getkeyAlt();
			if (a)
			{
				if (LCSrandom(a->get_attribute(ATTRIBUTE_HEART, false)))
				{
					gamelog.newline();
					set_color_easy(GREEN_ON_BLACK_BRIGHT);
					mvaddstrAlt(++y,  0, a->name, gamelog);
					addstrAlt(feels_sick_and, gamelog);
					a->adjust_attribute(ATTRIBUTE_HEART, -1);
					mvaddstrAlt(++y,  0, pickrandom(feels_sick), gamelog);
				}
				else if (!LCSrandom(3))
				{
					gamelog.newline();
					set_color_easy(CYAN_ON_BLACK_BRIGHT);
					mvaddstrAlt(++y,  0, a->name, gamelog);
					addstrAlt(" grows colder.", gamelog);
					a->adjust_attribute(ATTRIBUTE_WISDOM, +1);
				}
			}
		}
		gamelog.nextMessage();
		delete[] _attack;
	}
	if (AUTOENLIGHTEN) {
		turned = 1;// Lucky!
	}
	if (turned&&cr->alive)
	{
		//clear_interrogation_sidebar();
		//delete interrogation information
		delete intr;
		set_color_easy(WHITE_ON_BLACK_BRIGHT);
		mvaddstrAlt(++y,  0, "The Automaton has been Enlightened!   Your Liberal ranks are swelling!", gamelog);
		if (cr->get_attribute(ATTRIBUTE_HEART, true) > 7 &&
			cr->get_attribute(ATTRIBUTE_WISDOM, true) > 2 &&
			!LCSrandom(4) && (cr->flag & CREATUREFLAG_KIDNAPPED))
		{
			moveAlt(++y, 0);
			gamelog.newline();
			addstrAlt("The conversion is convincing enough that the police no longer consider it a kidnapping.", gamelog);
			//Actually liberalized -- they'll clean up the kidnapping story
			cr->flag &= ~CREATUREFLAG_MISSING;
			cr->flag &= ~CREATUREFLAG_KIDNAPPED;
		}
		cr->flag |= CREATUREFLAG_BRAINWASHED;

		setAllCreatureActivities(ACTIVITY_NONE, temppool);

		y += 2;
		liberalize(*cr, false);
		cr->hireid = a->id;
		stat_recruits++;
		if (LocationsPool::getInstance().isLocationMapped(cr->worklocation) == 0 || LocationsPool::getInstance().isLocationHidden(cr->worklocation) == 1)
		{
			gamelog.newline();
			mvaddstrAlt(y,  0, cr->name, gamelog);
			addstrAlt(" reveals details about the ", gamelog);
			addstrAlt(LocationsPool::getInstance().getLocationName(cr->worklocation), gamelog);
			addstrAlt(singleDot, gamelog);
			gamelog.newline();
			moveAlt(++y, 0);
			if (LocationsPool::getInstance().getLocationType(cr->worklocation) <= SITE_RESIDENTIAL_SHELTER)
				addstrAlt("Unfortunately, none of it is useful to the LCS.", gamelog);
			else
			{
				addstrAlt(a->name, gamelog);
				addstrAlt(" was able to create a map of the site with this information.", gamelog);
			}
			LocationsPool::getInstance().setLocationMappedAndUnhidden(cr->worklocation);
			y += 2;
		}
		if (cr->flag & CREATUREFLAG_MISSING && !(cr->flag & CREATUREFLAG_KIDNAPPED))
		{
			getkeyAlt();
			eraseAlt();
			set_color_easy(WHITE_ON_BLACK_BRIGHT);
			moveAlt(y = 1, 0);
			gamelog.newline();
			addstrAlt(cr->name, gamelog);
			addstrAlt("'s disappearance has not yet been reported.", gamelog);
			sleeperize_prompt(*cr, *a, y += 2);
			cr->flag &= ~CREATUREFLAG_MISSING;
			gamelog.nextMessage();
			return;
		}
	}
	if (cr->align == 1 || !cr->alive) {
		setAllCreatureActivities(ACTIVITY_NONE, temppool);
	}
	gamelog.nextMessage();
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(24,  0, "Press any key to reflect on this.");
	getkeyAlt();
}