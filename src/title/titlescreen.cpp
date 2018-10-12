
#include "../includes.h"
const string CONST_titlescreen003 = "real_quote.txt";

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
// Note: this file is encoded in the PC-8 / Code Page 437 / OEM-US character set
// (The same character set used by Liberal Crime Squad when it is running)
// Certain special characters won't display correctly unless your text editor is
// set to use that character set, such as this e with an accent: �
// In Windows Notepad with the Terminal font, OEM/DOS encoding it should work fine.
// You can set this in Notepad by going to Format->Font and choosing the Terminal font,
// then choosing OEM/DOS in the Script dropdown box.
// In Notepad++ go to the Encoding menu, Character sets, Western European, OEM-US... easy!
// In Code::Blocks's editor go to Settings->Editor->the Other Settings tab and
// then pick WINDOWS-437 from the dropdown box and then choose the radio button
// to make this the default encoding and disable auto-detection of the encoding.
// Then close the file and reopen it (since Code::Blocks detects the encoding
// when it opens the file and it can't be changed after that; what we changed was
// how it detects encoding for files it opens in the future, not files already open).
// In Microsoft Visual C++, right-click the file in the Solution Explorer,
// select CONST_titlescreen001, choose CONST_titlescreenB004,
// then choose CONST_titlescreen002.
// In MS-DOS Editor (included with Windows as EDIT.COM in your system32 directory),
// the codepage will be correct already since it's running in a console window just
// like Liberal Crime Squad. Well OK, the encoding might be wrong, but then it's wrong
// in Liberal Crime Squad TOO, and to fix it, go to Control Panel, Regional and Language Settings,
// Advanced tab, and choose English (United States) from the dropdown box as the encoding
// for non-Unicode applications, then press OK.
// If you have a Linux or other UNIX-based system you are obviously smart enough
// to figure out for yourself how to open a file in OEM-US PC-8 codepage 437 in
// your favorite text editor. If you're on Mac OS X, well that's UNIX-based, figure
// it out for yourself.
#include "../title/titlescreen.h"
void mode_base();
//#include "../common/consolesupport.h"
// for void getkeyAlt()
#include "../common/commondisplay.h"
// for addstr
#include "../common/getnames.h"
// for enter_name
#include "../title/highscore.h"
//for void viewhighscores
#include "../title/newgame.h"
//for void setup_newgame();
//#include "../title/saveload.h"
bool file_exists(const std::string& filename);
char load(const string& filename);
void savegame(const string& filename);
string title_screen::savefile_name;
vector<string> title_screen::s_savefiles;
#include "../cursesAlternative.h"
#include "../cursesAlternativeConstants.h"
#include "../set_color_support.h"
/* end the game and clean up */
void end_game(int err = EXIT_SUCCESS);
#include "../common/musicClass.h"
#include "../creature/creatureEnums.h"
#include "../customMaps.h"
const string titley = "titlescreen\\";
const int REAL_QUOTE_SIZE = 4;
vector<vector<string> > real_quote;
vector<file_and_text_collection> title_screen_text_files = {
	customText(&real_quote, titley + CONST_titlescreen003, REAL_QUOTE_SIZE),
};
const string spaceDashSpace = " - ";
string deleteSave;
string chooseSave;
string titleScreenLine;
string newGame;
string pressToSelectSave;
string pressToDeleteSave;
string vToSwitchXToQuit;
string areYouSureDelte;
string questionYSlashN;
string pressMtoTurnOffMusic;
string pressMtoTurnOnMusic;
string inWhatWorld;
string enterNameForSave;
string pleaseEnterName;
string prettyPlease;
string justEnterName;
string liberalCrimeSquad;
string inspiredByOubliette;
string copyrightTarn;
string bayTwelveProductions;
string lcsHyperlink;
string vChar;
string maintainedByOpenSource;
string kingDrakeHyperlink;
string lcsForumHyperlink;
string lcsWikiHyperlink;
string pressESCToQuit;
string pressAnyKeyToPursue;
string plusChar;
string dotDat;
// private
void title_screen::title() {
	extern string PACKAGE_VERSION_STR;
	//title screen
	eraseAlt();
	set_color_easy(GREEN_ON_BLACK_BRIGHT);
	mvaddstrCenter(2, liberalCrimeSquad);
	set_color_easy(WHITE_ON_BLACK_BRIGHT);
	mvaddstrCenter(4, inspiredByOubliette);
	vector<string> quote = pickrandom(real_quote);
	mvaddstrCenter(6, quote[0]);
	mvaddstrCenter(7, quote[1]);
	mvaddstrCenter(8, quote[2]);
	mvaddstrCenter(9, quote[3]);
	mvaddstrCenter(11, copyrightTarn);
	mvaddstrCenter(12, bayTwelveProductions);
	mvaddstrCenter(13, lcsHyperlink);
	mvaddstrCenter(15, vChar + PACKAGE_VERSION_STR + maintainedByOpenSource);
	mvaddstrCenter(16, kingDrakeHyperlink);
	mvaddstrCenter(17, lcsForumHyperlink);
	mvaddstrCenter(18, lcsWikiHyperlink);
	mvaddstrCenter(20, pressESCToQuit);
	mvaddstrCenter(22, pressAnyKeyToPursue);
	mvaddstrAlt(24, 79, plusChar);
}
void title_screen::choose_savefile_name()
{
	const int SAVE_FILE_NAMELEN = 21;
	eraseAlt();
	set_color_easy(WHITE_ON_BLACK_BRIGHT);
	mvaddstrAlt(0, 0, inWhatWorld);
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(1, 0, enterNameForSave);
	char savefile_temp[SAVE_FILE_NAMELEN];
	enter_name(2, 0, savefile_temp, SAVE_FILE_NAMELEN, blankString.c_str());
	bool justEnter = false;
	bool enterDamn = false;
	do {
		if (strcmp(savefile_temp, blankString.c_str()) == 0) {
			eraseAlt();
			set_color_easy(WHITE_ON_BLACK_BRIGHT);
			mvaddstrAlt(0, 0, inWhatWorld);
			string enterTheName;
			if (enterDamn) {
				enterTheName = (justEnterName);
			}
			else if (justEnter) {
				enterTheName = (prettyPlease);
				enterDamn = true;
			}
			else {
				enterTheName = (pleaseEnterName);
			}
			set_color_easy(WHITE_ON_BLACK);
			mvaddstrAlt(1, 0, enterTheName);
			enter_name(2, 0, savefile_temp, SAVE_FILE_NAMELEN, blankString.c_str());
			justEnter = true;
		}
		else {
			savefile_name = string(savefile_temp) + dotDat;
			justEnter = false;
		}
	} while (justEnter);
	setup_newgame();
	makecharacter();
}
void title_screen::selectAndLoadSaveFile() {
	extern short interface_pgup;
	extern short interface_pgdn;
	s_savefiles = move(LCSSaveFiles());
	char loaded = s_savefiles.size() > 0;
	if (!loaded)
	{
		choose_savefile_name();
	}
	else {
		bool to_delete = false;
		int page = 0;
		// IsaacG This almost has to be redone
		while (!len(savefile_name))
		{
			eraseAlt();
			set_color_easy(to_delete ? YELLOW_ON_RED : WHITE_ON_BLACK_BRIGHT);
			mvaddstrAlt(0, 0, to_delete ? deleteSave : chooseSave);
			set_color_easy(WHITE_ON_BLACK);
			mvaddstrAlt(1, 0, titleScreenLine);
			int y = 2;
			for (int p = page * 19; p < s_savefiles.size() && p < page * 19 + 19; p++, y++)
			{
				mvaddchAlt(y, 0, y + 'A' - 2);
				addstrAlt(spaceDashSpace);
				const string &strtemp = s_savefiles[y - 2];
				addstrAlt(strtemp.substr(0, strtemp.find(dotDat)));
			}
			mvaddstrAlt(y, 0, (char(y + 'A' - 2)) + spaceDashSpace + newGame);
			mvaddstrAlt(22, 0, (to_delete ? pressToDeleteSave : pressToSelectSave) + vToSwitchXToQuit);
			mvaddstrAlt(23, 0, addpagestr());
			int c = getkeyAlt();
			//PAGE UP
			if ((c == interface_pgup || c == KEY_UP || c == KEY_LEFT) && page > 0)page--;
			//PAGE DOWN
			if ((c == interface_pgdn || c == KEY_DOWN || c == KEY_RIGHT) && (page + 1) * 19 < s_savefiles.size())page++;
			if (c >= 'a'&&c <= 's')
			{
				const int p = page * 19 + c - 'a';
				if (p < s_savefiles.size()) {
					if (!to_delete) {
						savefile_name = s_savefiles[p];
					}
					else {
						set_color_easy(WHITE_ON_BLACK_BRIGHT);
						mvaddstrCenter(10, areYouSureDelte + s_savefiles[p] + questionYSlashN);
						c = getkeyAlt();
						if (c == 'y')
						{
							LCSDeleteFile(s_savefiles[p].c_str(), LCSIO_PRE_HOME);
							s_savefiles = move(LCSSaveFiles());
						}
						continue;
					}
				}
				else if (p == s_savefiles.size()) {
					choose_savefile_name();
				}
			}
			else if (c == 'v') to_delete = !to_delete;
			if (c == ESC || c == 'x') end_game();
		}
		load(savefile_name);
	}
}
title_screen title_screen::s_title_singleton;
bool title_screen::titleInitiated = false;
title_screen title_screen::getInstance()
{
	if (!titleInitiated) {
		titleInitiated = true;
		s_title_singleton = title_screen();
	}
	return s_title_singleton;
}
void automatedDataTests();
// public
void title_screen::mode_title()
{
	extern short mode;
	extern MusicClass music;
	title();
	int c = 0;
	do {
		if (c == 'h') {
			viewhighscores();
			pressAnyKey();
			title();
		}
		if (c == 'm') music.enableIf(!music.isEnabled());
		string str;
		if (music.isEnabled()) {
			(str = pressMtoTurnOffMusic);
		}
		else {
			(str = pressMtoTurnOnMusic);
		}
		if (c == 'd') {
			automatedDataTests();
			title();
		}
		mvaddstrCenter(22, str);
		if (c == ESC || c == 'x') end_game();
		c = getkeyAlt();
	} while (c == 'm' || c == 'h' || c == 'x' || c == ESC || c == 'd');
	selectAndLoadSaveFile();
	mode = GAMEMODE_BASE;
	// Main Loop
	mode_base();
	savegame(savefile_name);
}
bool title_screen::autosave = true;
void title_screen::reset()
{
	//LCSDeleteFile(savefile_name.c_str(), LCSIO_PRE_HOME);
	LCSDeleteFile(savefile_name.c_str(), LCSIO_PRE_HOME);

	//deleteVerbose(savefile_name);
}
void title_screen::setautosaveoption(bool shouldautosave) {
	autosave = shouldautosave;
}
void title_screen::autosavegame() {
	if (autosave) {
		savegame(savefile_name);
	}
}