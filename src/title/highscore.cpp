
#include "../includes.h"
const string CONST_highscoreB003 = "wb";
const string CONST_highscoreB002 = "rb";
const string CONST_highscore002 = "score.dat";

const string blankString = "";
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
#include "../common/ledgerEnums.h"
#include "../common/ledger.h"
#include "../title/titlescreen.h"
#include "../common/getnames.h"
// for getmonth
#include "../cursesAlternative.h"
#include "../set_color_support.h"
#include "../common/musicClass.h"
extern MusicClass music;
extern int ustat_recruits;
extern int ustat_kidnappings;
extern int ustat_dead;
extern int ustat_kills;
extern int ustat_funds;
extern int ustat_spent;
extern int ustat_buys;
extern int ustat_burns;
extern int stat_buys;
extern int stat_burns;
extern int stat_kidnappings;
extern int year;
extern int stat_dead;
extern int stat_kills;
extern int stat_recruits;
string univer;
string numRecruit;
string numMartyr;
string numKills;
string numKidnap;
string cashTaxed;
string cashSpent;
string flagsBought;
string flagsBurned;
string noValid;
string heLiElite;
string heLiLiber;
string heLiBrought;
string heLiBlot;
string heLiMob;
string heLiDownsized;
string heLiKIA;
string hecoReag;
string heLiDie;
string heLiExec;
string heLiVaca;
string heLiHide;
string heLiHunted;
string heLiScattered;
string heLiOutCrime;
string heLiBurned;
string hecoStalinized;
string dotSpace;
struct highscorest
{
	char valid, endtype, slogan[SLOGAN_LEN];
	int month, year, stat_recruits, stat_kidnappings, stat_dead, stat_kills, stat_funds, stat_spent, stat_buys, stat_burns;
};
extern highscorest score[SCORENUM];
extern string singleSpace;
extern int yourscore;
extern class Ledger ledger;
extern int month;
extern char slogan[SLOGAN_LEN];
MusicModes getEndingMusic(EndTypes e);
string getEndingString(EndTypes e);
struct  saveLoadChunk {
	void * Buffer;
	size_t      ElementSize;
	size_t      ElementCount;
	saveLoadChunk(void * _Buffer, int _ElementSize, int _ElementCount) :Buffer(_Buffer), ElementSize(_ElementSize), ElementCount(_ElementCount) {}
};
vector<saveLoadChunk> highScoreSaveLoad = {
	saveLoadChunk(&ustat_recruits, sizeof(int), 1),
	saveLoadChunk(&ustat_dead, sizeof(int), 1),
	saveLoadChunk(&ustat_kills, sizeof(int), 1),
	saveLoadChunk(&ustat_kidnappings, sizeof(int), 1),
	saveLoadChunk(&ustat_funds, sizeof(int), 1),
	saveLoadChunk(&ustat_spent, sizeof(int), 1),
	saveLoadChunk(&ustat_buys, sizeof(int), 1),
	saveLoadChunk(&ustat_burns, sizeof(int), 1),
	saveLoadChunk(score, sizeof(highscorest), SCORENUM)
};
/* loads the high scores file */
void loadhighscores()
{
	for (int s = 0; s < SCORENUM; s++)score[s].valid = 0;
	//LOAD FILE
	int loadversion;
	FILE *h = LCSOpenFile(CONST_highscore002.c_str(), CONST_highscoreB002.c_str(), LCSIO_PRE_HOME);
	if (h != NULL)
	{
		fread(&loadversion, sizeof(int), 1, h);
		if (loadversion < lowestloadscoreversion)
		{
			LCSCloseFile(h);
			return;
		}
		for (saveLoadChunk s : highScoreSaveLoad) {
			fread(s.Buffer, s.ElementSize, s.ElementCount, h);
		}
		LCSCloseFile(h);
	}
}
/* displays the high score board */
void viewhighscores(int musicoverride)
{
	if (musicoverride != MUSIC_OFF) music.play(musicoverride);
	int s;
	loadhighscores();
	short validsum = 0;
	for (s = 0; s < SCORENUM; s++)
	{
		if (score[s].valid)validsum++;
	}
	eraseAlt();
	set_color_easy(WHITE_ON_BLACK_BRIGHT);
	mvaddstrAlt(0, 0, validsum ? heLiElite : noValid);
	if (!validsum)
	{
		return;
	}
	int y = 2;
	for (s = 0; s < SCORENUM; s++)
	{
		if (score[s].valid)
		{
			if (yourscore == s&&score[s].endtype == END_WON)set_color_easy(GREEN_ON_BLACK_BRIGHT);
			else if (yourscore == s)set_color_easy(RED_ON_BLACK_BRIGHT);
			else set_color_easy(WHITE_ON_BLACK_BRIGHT);
			mvaddstrAlt(y,  0, score[s].slogan);
			if (yourscore == s&&score[s].endtype == END_WON)set_color_easy(GREEN_ON_BLACK);
			else if (yourscore == s)set_color_easy(RED_ON_BLACK);
			else set_color_easy(WHITE_ON_BLACK);
			mvaddstrAlt(y + 1, 0, getEndingString((EndTypes) score[s].endtype));
			if (yourscore == s&&musicoverride == MUSIC_OFF) { music.play(getEndingMusic((EndTypes) score[s].endtype)); }
			addstrAlt(getmonth(score[s].month));
			addstrAlt(singleSpace);
			addstrAlt(score[s].year);
			addstrAlt(dotSpace);
			mvaddstrAlt(y + 2,  0, numRecruit);
			addstrAlt(score[s].stat_recruits);
			mvaddstrAlt(y + 3,  0, numMartyr);
			addstrAlt(score[s].stat_dead);
			mvaddstrAlt(y + 2,  20, numKills);
			addstrAlt(score[s].stat_kills);
			mvaddstrAlt(y + 3,  20, numKidnap);
			addstrAlt(score[s].stat_kidnappings);
			mvaddstrAlt(y + 2,  40, cashTaxed);
			addstrAlt(score[s].stat_funds);
			mvaddstrAlt(y + 3,  40, cashSpent);
			addstrAlt(score[s].stat_spent);
			mvaddstrAlt(y + 2,  60, flagsBought);
			addstrAlt(score[s].stat_buys);
			mvaddstrAlt(y + 3,  60, flagsBurned);
			addstrAlt(score[s].stat_burns);
			y += 4;
		}
	}
	set_color_easy(GREEN_ON_BLACK_BRIGHT);
	//UNIVERSAL STATS
	mvaddstrAlt(22,  0, univer);
	mvaddstrAlt(23,  0, numRecruit);
	addstrAlt(ustat_recruits);
	mvaddstrAlt(24,  0, numMartyr);
	addstrAlt(ustat_dead);
	mvaddstrAlt(23,  20, numKills);
	addstrAlt(ustat_kills);
	mvaddstrAlt(24,  20, numKidnap);
	addstrAlt(ustat_kidnappings);
	mvaddstrAlt(23,  40, cashTaxed);
	addstrAlt(ustat_funds);
	mvaddstrAlt(24,  40, cashSpent);
	addstrAlt(ustat_spent);
	mvaddstrAlt(23,  60, flagsBought);
	addstrAlt(ustat_buys);
	mvaddstrAlt(24,  60, flagsBurned);
	addstrAlt(ustat_burns);
 	pressAnyKey();
}
MusicModes getEndingMusic(EndTypes e) {
	switch (e)
	{
	case END_WON:
		return (MUSIC_VICTORY);
	case END_POLICE:
		return (MUSIC_DEFEAT);
	case END_CIA:
		return (MUSIC_DEFEAT);
	case END_HICKS:
		return (MUSIC_DEFEAT);
	case END_CORP:
		return (MUSIC_DEFEAT);
	case END_DEAD:
		return (MUSIC_DEFEAT);
	case END_REAGAN:
		return (MUSIC_REAGANIFIED);
	case END_PRISON:
		return (MUSIC_DEFEAT);
	case END_EXECUTED:
		return (MUSIC_DEFEAT);
	case END_DATING:
		return (MUSIC_DEFEAT);
	case END_HIDING:
		return (MUSIC_DEFEAT);
	case END_DISBANDLOSS:
		return (MUSIC_DEFEAT);
	case END_DISPERSED:
		return (MUSIC_DEFEAT);
	case END_CCS:
		return (MUSIC_DEFEAT);
	case END_FIREMEN:
		return (MUSIC_DEFEAT);
	case END_STALIN:
		return (MUSIC_STALINIZED);
	default:
		return MUSIC_RANDOM;
	}
}
string getEndingString(EndTypes e) {
	switch (e)
	{
	case END_WON:
		return(heLiLiber);
		break;
	case END_POLICE:
		return(heLiBrought);
		break;
	case END_CIA:
		return(heLiBlot);
		break;
	case END_HICKS:
		return(heLiMob);
		break;
	case END_CORP:
		return(heLiDownsized);
		break;
	case END_DEAD:
		return(heLiKIA);
		break;
	case END_REAGAN:
		return(hecoReag);
		break;
	case END_PRISON:
		return(heLiDie);
		break;
	case END_EXECUTED:
		return(heLiExec);
		break;
	case END_DATING:
		return(heLiVaca);
		break;
	case END_HIDING:
		return(heLiHide);
		break;
	case END_DISBANDLOSS:
		return(heLiHunted);
		break;
	case END_DISPERSED:
		return(heLiScattered);
		break;
	case END_CCS:
		return(heLiOutCrime);
		break;
	case END_FIREMEN:
		return(heLiBurned);
		break;
	case END_STALIN:
		return(hecoStalinized);
		break;
	default:
		return blankString;
		break;
	}
}
/* saves a new high score */
void savehighscore(char endtype)
{
	loadhighscores();
	//MERGE THE STATS
	ustat_recruits += stat_recruits;
	ustat_dead += stat_dead;
	ustat_kills += stat_kills;
	ustat_kidnappings += stat_kidnappings;
	ustat_funds += ledger.total_income;
	ustat_spent += ledger.total_expense;
	ustat_buys += stat_buys;
	ustat_burns += stat_burns;
	//PLACE THIS HIGH SCORE BY DATE IF NECESSARY
	yourscore = -1;
	for (int s = 0; s<SCORENUM; s++)
	{
		if ((endtype == END_WON&&score[s].endtype == END_WON&&
			year == score[s].year&&month == score[s].month&&
			ledger.total_expense + ledger.total_expense>score[s].stat_spent + score[s].stat_funds) ||
			(endtype == END_WON&&score[s].endtype == END_WON
				&& (year < score[s].year ||
					(year == score[s].year && month<score[s].month))) ||
			(endtype == END_WON&&score[s].endtype != END_WON) ||
			(endtype != END_WON&&score[s].endtype != END_WON&&
				ledger.total_expense + ledger.total_income>score[s].stat_spent + score[s].stat_funds) ||
			score[s].valid == 0)
		{
			for (int s2 = SCORENUM - 1; s2 >= s + 1; s2--)
			{
				score[s2] = score[s2 - 1];
			}
			strcpy(score[s].slogan, slogan);
			score[s].month = month;
			score[s].year = year;
			score[s].stat_recruits = stat_recruits;
			score[s].stat_dead = stat_dead;
			score[s].stat_kills = stat_kills;
			score[s].stat_kidnappings = stat_kidnappings;
			score[s].stat_funds = ledger.total_income;
			score[s].stat_spent = ledger.total_expense;
			score[s].stat_buys = stat_buys;
			score[s].stat_burns = stat_burns;
			score[s].valid = 1;
			score[s].endtype = endtype;
			yourscore = s;
			break;
		}
	}
	FILE *h = LCSOpenFile(CONST_highscore002.c_str(), CONST_highscoreB003.c_str(), LCSIO_PRE_HOME);
	if (h != NULL)
	{
		int lversion = version;
		fwrite(&lversion, sizeof(int), 1, h);
		for (saveLoadChunk s : highScoreSaveLoad) {
			fwrite(s.Buffer, s.ElementSize, s.ElementCount, h);
		}
		LCSCloseFile(h);
	}
	title_screen::getInstance().reset();
}