#define	SHOP_CPP
#include "../includes.h"
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
// select CONST_shop020, choose CONST_shopB056,
// then choose CONST_shop021.
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
 ShopOption::ShopOption() : description_(undefined), letter_(0), letter_defined_(false)
 { }
 Shop::Shop(MCD_STR xmlstring)
	 : allow_selling_(false), only_sell_legal_(true), increase_prices_with_illegality_(false),
	 fullscreen_(false), sell_masks_(false)
 {
	 init(xmlstring);
 }
 Shop::Shop(MCD_STR xmlstring, bool fullscreen, bool only_sell_legal,
	 bool increase_prices_with_illegality)
	 : allow_selling_(false), only_sell_legal_(only_sell_legal),
	 increase_prices_with_illegality_(increase_prices_with_illegality),
	 fullscreen_(fullscreen), sell_masks_(false)
 {
	 init(xmlstring);
 }

 void Shop::init(const MCD_STR &xmlstring)
 {
	 CMarkup xml;
	 xml.SetDoc(xmlstring);
	 xml.FindElem();
	 xml.IntoElem();
	 while (xml.FindElem())
	 {
		 std::string tag = xml.GetTagName();
		 if (shopInitTags.count(tag)) {
			 switch (shopInitTags[tag]) {
				 int b;
			 case ENUM_tag_only_sell_legal_items:

				 b = stringtobool(xml.GetData());
				 if (b == 1 || b == 0) only_sell_legal_ = b;
				 break;
			 case ENUM_tag_fullscreen:

				 b = stringtobool(xml.GetData());
				 if (b == 1 || b == 0) fullscreen_ = b;
				 break;
			 case ENUM_tag_allow_selling:

				 b = stringtobool(xml.GetData());
				 if (b == 1 || b == 0) allow_selling_ = b;
				 break;
			 case ENUM_tag_increase_prices_with_illegality:

				 b = stringtobool(xml.GetData());
				 if (b == 1 || b == 0) increase_prices_with_illegality_ = b;
				 break;
			 case ENUM_tag_department:

				 options_.push_back(new Shop(xml.GetSubDoc(), fullscreen_, only_sell_legal_,
					 increase_prices_with_illegality_));
				 break;
			 case ENUM_tag_entry:
				 description_ = xml.GetData();
				 break;
			 case ENUM_tag_exit:
				 exit_ = xml.GetData();
				 break;
			 case ENUM_tag_sell_masks:

				 b = stringtobool(xml.GetData());
				 if (b == 1 || b == 0) sell_masks_ = b;
				 break;
			 case ENUM_tag_letter:
				 letter_ = xml.GetData()[0];
				 if (97 <= letter_ && letter_ <= 122) //Check it is a letter.
					 letter_defined_ = true;
				 else if (65 <= letter_ && letter_ <= 90)
				 {
					 letter_ += 32;
					 letter_defined_ = true;
				 }
				 else if (letter_ == '!') //Allow special character.
					 letter_defined_ = true;
				 break;
			 case ENUM_tag_item:

				 options_.push_back(new ShopItem(xml.GetSubDoc(), only_sell_legal_,
					 increase_prices_with_illegality_));
				 break;

			 }
		 }
	 }
 }
 Shop::~Shop()
 {
	 delete_and_clear(options_);
 }
 void Shop::enter(Deprecatedsquadst& customers) const
 {
	 music.play(MUSIC_SHOPPING);
	 int buyer = 0;
	 choose(customers, buyer);
 }
 void Shop::choose(Deprecatedsquadst& customers, int& buyer) const
 {
	 if (fullscreen_) browse_fullscreen(customers, buyer);
	 else browse_halfscreen(customers, buyer);
 }
 void Shop::browse_fullscreen(Deprecatedsquadst& customers, int& buyer) const
 {
	 int page = 0;
	 std::vector<ShopOption*> available_options = options_;
	 available_options.erase(remove_if(available_options.begin(),
		 available_options.end(),
		 not1(mem_fun(&ShopOption::display))),
		 available_options.end());
	 while (true)
	 {
		 eraseAlt();
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(0, 0, CONST_shop026);
		 addstrAlt(customers.squad[buyer]->getNameAndAlignment().name);
		 addstrAlt(CONST_shop051);
		 mvaddstrAlt(1, 0, CONST_shop028);
		 //Write wares and prices
		 for (int p = page * 19, y = 2; p < len(available_options) && p < page * 19 + 19; p++, y++)
		 {
			 if (available_options[p]->is_available())
				 set_color_easy(WHITE_ON_BLACK);
			 else set_color_easy(BLACK_ON_BLACK_BRIGHT);

			 mvaddcharAlt(y, 0, 'A' + y - 2);
			 addstrAlt(spaceDashSpace);
			 addstrAlt(available_options[p]->get_description_fullscreen());
		 }
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(22, 0, CONST_shop029); //allow customize CONST_shopB061? -XML
		 mvaddstrAlt(23, 0, addpagestr());
		 mvaddstrAlt(24, 0, CONST_shop061 + std::string(customers.squad[buyer]->getNameAndAlignment().name) + singleSpace + exit_);
		 int c = getkeyAlt();
		 //PAGE UP
		 if (is_page_up(c) && page > 0) page--;
		 //PAGE DOWN
		 if (is_page_down(c) && (page + 1) * 19 < len(available_options)) page++;
		 if (c >= 'a'&&c <= 's')
		 {
			 int p = page * 19 + c - 'a';
			 if (p < len(available_options) && available_options[p]->is_available())
				 available_options[p]->choose(customers, buyer);
			 break;
		 }
		 if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
	 }
 }
 int fenceselect(Deprecatedsquadst& customers)
 {
	 int ret = 0, page = 0;
	 consolidateLoot(customers.squad[0]->base);

	 vector<int> selected(LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base), 0);
	 while (true)
	 {
		 eraseAlt();
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(0, 0, CONST_shop031);
		 if (ret)
		 {
			 mvaddstrAlt(0, 30, CONST_shop032);
			 addstrAlt(ret);
		 }
		 printparty();
		 int x = 1, y = 10;
		 for (int l = page * 18; l < LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base) && l < page * 18 + 18; l++)
		 {
			 if (selected[l])
				 set_color_easy(GREEN_ON_BLACK_BRIGHT);
			 else set_color_easy(WHITE_ON_BLACK);
			 string itemstr = getLootTitle(customers.squad[0]->base, l);
			 if (getLootNumber(customers.squad[0]->base, l) > 1)
			 {
				 if (selected[l])
					 itemstr += singleSpace + tostring(selected[l]) + CONST_shop033;
				 else itemstr += CONST_shop034;
				 itemstr += tostring(getLootNumber(customers.squad[0]->base, l));
			 }
			 string outstr;
			 outstr = static_cast<char>(l - page * 18 + 'A');
			 outstr += spaceDashSpace + itemstr;
			 mvaddstrAlt(y, x, outstr);
			 x += 26;
			 if (x > 53) x = 1, y++;
		 }
		 //PAGE UP
		 set_color_easy(WHITE_ON_BLACK);
		 if (page > 0)
		 {
			 mvaddstrAlt(17, 1, addprevpagestr());
		 }
		 //PAGE DOWN
		 if ((page + 1) * 18 < LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base))
		 {
			 mvaddstrAlt(17, 53, addnextpagestr());
		 }
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(23, 1, CONST_shop035);
		 mvaddstrAlt(24, 1, enter_done);
		 int c = getkeyAlt();
		 if (c >= 'a' && c <= 'r')
		 {
			 int slot = c - 'a' + page * 18;
			 if (slot >= 0 && slot < LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base))
			 {
				 if (selected[slot])
				 {
					 ret -= getFenceValueLocation(customers.squad[0]->base, slot) * selected[slot];
					 selected[slot] = 0;
				 }
				 else
				 {
					 if (!getCanBeSoldLocation(customers.squad[0]->base, slot))
					 {
						 printparty();
						 set_color_easy(WHITE_ON_BLACK_BRIGHT);
						 mvaddstrAlt(8, 15, CONST_shop036);
						 pressAnyKey();
					 }
					 else
					 {
						 if (getLocationLootNumber(customers.squad[0]->base, slot) > 1)
							 selected[slot] = prompt_amount(0, getLocationLootNumber(customers.squad[0]->base, slot));
						 else selected[slot] = 1;
						 ret += getFenceValueLocation(customers.squad[0]->base, slot) * selected[slot];
					 }
				 }
			 }
		 }
		 if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
		 //PAGE UP
		 if (is_page_up(c) && page > 0)
			 page--;
		 //PAGE DOWN
		 if (is_page_down(c) &&
			 (page + 1) * 18 < LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base))
			 page++;
	 }
	 for (int l = LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base) - 1; l >= 0; l--)
	 {
		 if (selected[l])
		 {
			 decreateLocationLoot(customers.squad[0]->base, l, selected[l]);

		 }
	 }
	 return ret;
 }
 void sell_loot(Deprecatedsquadst& customers)
 {
	 int partysize = customers.squadsize();
	 while (true)
	 {
		 eraseAlt();
		 locheader();
		 printparty();
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(10, 1, CONST_shop058);
		 if (LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base))
			 set_color_easy(WHITE_ON_BLACK);
		 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		 mvaddstrAlt(10, 40, CONST_shop038);
		 mvaddstrAlt(11, 1, CONST_shop039);
		 mvaddstrAlt(11, 40, CONST_shop040);
		 mvaddstrAlt(12, 1, CONST_shop041);
		 mvaddstrAlt(12, 40, CONST_shop042);
		 if (party_status != -1)
			 set_color_easy(WHITE_ON_BLACK);
		 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		 mvaddstrAlt(15, 1, show_squad_liberal_status);
		 if (partysize && (party_status == -1 || partysize > 1))
			 set_color_easy(WHITE_ON_BLACK);
		 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		 mvaddstrAlt(15, 40, check_status_of_squad_liberal);
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(16, 40, CONST_shop043);
		 int c = getkeyAlt();
		 if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
		 if (c == 'e'&&customers.squad[0]->base != -1)
			 equipLoot(customers.squad[0]->base, -1);
		 if (c == 'w' || c == 'a' || c == 'c')
		 {
			 set_color_easy(WHITE_ON_BLACK_BRIGHT);
			 switch (c)
			 {
			 case 'w': mvaddstrAlt(18, 1, CONST_shop044); break;
			 case 'a': mvaddstrAlt(18, 1, CONST_shop045); break;
			 case 'c': mvaddstrAlt(18, 1, CONST_shop046); break;
			 }
			 if (getkeyAlt() != 'y') c = 0; //no sale
		 }
		 if ((c == 'w' || c == 'c' || c == 'l' || c == 'a' || c == 'f') &&
			 LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base))
		 {
			 int fenceamount = 0;
			 if (c == 'f') fenceamount = fenceselect(customers);
			 else
			 {
				 for (int l = LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base) - 1; l >= 0; l--)
				 {
					 if (c == 'w' && whatIsThisItemInLocation(customers.squad[0]->base, l) == THIS_IS_WEAPON
						 && getCanBeSoldLocation(customers.squad[0]->base, l))
					 {
						 fenceamount += getFenceValueLocation(customers.squad[0]->base, l)
							 * getLocationLootNumber(customers.squad[0]->base, l);
						 deleteLocationLoot(customers.squad[0]->base, l);
					 }
					 else if (c == 'c' && whatIsThisItemInLocation(customers.squad[0]->base, l) == THIS_IS_ARMOR
						 && getCanBeSoldLocation(customers.squad[0]->base, l))
					 {
						 fenceamount += getFenceValueLocation(customers.squad[0]->base, l)
							 * getLocationLootNumber(customers.squad[0]->base, l);
						 deleteLocationLoot(customers.squad[0]->base, l);
					 }
					 else if (c == 'a' && whatIsThisItemInLocation(customers.squad[0]->base, l) == THIS_IS_CLIP
						 && getCanBeSoldLocation(customers.squad[0]->base, l))
					 {
						 fenceamount += getFenceValueLocation(customers.squad[0]->base, l)
							 * getLocationLootNumber(customers.squad[0]->base, l);
						 deleteLocationLoot(customers.squad[0]->base, l);
					 }
					 else if (c == 'l' && whatIsThisItemInLocation(customers.squad[0]->base, l) == THIS_IS_LOOT
						 && getCanBeSoldLocation(customers.squad[0]->base, l))
					 {
						 if (!noQuickFenceLocation(customers.squad[0]->base, l))
						 {
							 fenceamount += getFenceValueLocation(customers.squad[0]->base, l)
								 * getLocationLootNumber(customers.squad[0]->base, l);
							 deleteLocationLoot(customers.squad[0]->base, l);
						 }
					 }
				 }
			 }
			 if (fenceamount)
			 {
				 set_color_easy(WHITE_ON_BLACK_BRIGHT);
				 mvaddstrAlt(8, 1, CONST_shop047);
				 addstrAlt(fenceamount);
				 addstrAlt(CONST_shop048);
				 pressAnyKey();
				 ledger.add_funds(fenceamount, INCOME_PAWN);
			 }
		 }
	 }
 }
 void choose_buyer(Deprecatedsquadst& customers, int& buyer)
 {
	 party_status = -1;
	 int partysize = customers.squadsize();
	 if (partysize <= 1) return;
	 while (true)
	 {
		 printparty();
		 set_color_easy(WHITE_ON_BLACK_BRIGHT);
		 mvaddstrAlt(8, 20, chooseALiberalTo + CONST_shop049);
		 int c = getkeyAlt();
		 if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) return;
		 if (c >= '1'&&c <= partysize + '1' - 1)
		 {
			 buyer = c - '1';
			 return;
		 }
	 }
 }

 // Removing the subsequent references to location will be difficult.
 void maskselect(DeprecatedCreature &buyer)
 {
	 short maskindex = -1;
	 std::vector<int> masktype;
	 for (int a = 0; a < len(armortype); a++)
	 {
		 if (armortype[a]->is_mask() && !armortype[a]->is_surprise_mask())
			 masktype.push_back(a);
	 }
	 int page = 0;
	 while (true)
	 {
		 eraseAlt();
		 set_color_easy(WHITE_ON_BLACK_BRIGHT);
		 mvaddstrAlt(0, 0, CONST_shop050);
		 addstrAlt(buyer.getNameAndAlignment().name);
		 addstrAlt(CONST_shop051);
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(1, 0, CONST_shop052);
		 for (int p = page * 19, y = 2; p < len(masktype) && p < page * 19 + 19; p++, y++)
		 {
			 set_color_easy(WHITE_ON_BLACK);

			 mvaddcharAlt(y, 0, y + 'A' - 2);
			 addstrAlt(spaceDashSpace);
			 addstrAlt(armortype[masktype[p]]->get_name());
			 set_color_easy(WHITE_ON_BLACK);
			 mvaddstrAlt(y, 39, armortype[masktype[p]]->get_description());
		 }
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(22, 0, CONST_shop053);
		 mvaddstrAlt(23, 0, addpagestr());
		 mvaddstrAlt(24, 0, CONST_shop054);
		 addstrAlt(buyer.getNameAndAlignment().name);
		 addstrAlt(CONST_shop055);
		 int c = getkeyAlt();
		 //PAGE UP
		 if (is_page_up(c) && page > 0) page--;
		 //PAGE DOWN
		 if (is_page_down(c) && (page + 1) * 19 < len(masktype)) page++;
		 if (c >= 'a'&&c <= 's')
		 {
			 int p = page * 19 + c - 'a';
			 if (p < len(masktype))
			 {
				 maskindex = masktype[p];
				 break;
			 }
		 }
		 if (c == 'z')
		 {
			 for (int i = 0; i < len(armortype); i++)
				 if (armortype[i]->is_mask() && armortype[i]->is_surprise_mask())
					 masktype.push_back(i);
			 maskindex = pickrandom(masktype);
			 break;
		 }
		 if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
	 }
	 if (maskindex != -1 && ledger.get_funds() >= 15)
	 {
		 Armor a = Armor(maskindex);
		 buyer.give_armor(a, &location[buyer.base]->loot);
		 ledger.subtract_funds(15, EXPENSE_SHOPPING);
	 }
 }
 void Shop::browse_halfscreen(Deprecatedsquadst& customers, int& buyer) const
 {
	 int page = 0, partysize = customers.squadsize();
	 std::vector<ShopOption*> available_options = options_;
	 available_options.erase(remove_if(available_options.begin(),
		 available_options.end(),
		 not1(mem_fun(&ShopOption::display))),
		 available_options.end());
	 while (true)
	 {
		 eraseAlt();
		 set_color_easy(WHITE_ON_BLACK);
		 locheader();
		 printparty();
		 mvaddstrAlt(8, 45, CONST_shop056);
		 addstrAlt(customers.squad[buyer]->getNameAndAlignment().name);
		 //Write wares and prices
		 int y = 10, x = 1, taken_letters = 0;
		 for (int p = page * 19; p < len(available_options) && p < page * 19 + 20; p++)
		 {
			 if (available_options[p]->is_available())
				 set_color_easy(WHITE_ON_BLACK);
			 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			 int xNew;
			 if (x == 1)xNew = 1;
			 else xNew = 40;
			 if (available_options[p]->letter_defined_)
				 mvaddcharAlt(y, xNew, available_options[p]->showletter());
			 else
			 {
				 // Find an available letter to use for this ware.
				 bool done = false;
				 while (taken_letters < 27 && !done)
				 {
					 done = true;
					 if ('a' + taken_letters == 'b' || // Letters used by the shop UI are disallowed.
						 'a' + taken_letters == 'e' ||
						 ('a' + taken_letters == 's'&&allow_selling_) ||
						 ('a' + taken_letters == 'm'&&sell_masks_))
					 {
						 taken_letters++;
						 done = false;
						 continue;
					 }
					 for (int i = 0; i < len(available_options); i++)
					 {
						 if (available_options[i]->letter_defined_&&
							 'a' + taken_letters == available_options[i]->letter_)
						 {
							 taken_letters++;
							 done = false;
							 break;
						 }
					 }
				 }
				 available_options[p]->letter_ = 'a' + taken_letters;
				 mvaddcharAlt(y, xNew, 'A' + taken_letters++);
			 }
			 addstrAlt(spaceDashSpace);
			 addstrAlt(available_options[p]->get_description_halfscreen());
			 if (x == 1) x = 2;
			 else y++, x = 1;
		 }
		 if (sell_masks_)
		 {
			 if (ledger.get_funds() >= 15) set_color_easy(WHITE_ON_BLACK);
			 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			 mvaddstrAlt(y, 1 + (x - 1) * 39, CONST_shop057);
		 }
		 if (x == 2) y++;
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(y++, 1, CONST_shop058);
		 if (allow_selling_)
		 {
			 if (LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base))
				 set_color_easy(WHITE_ON_BLACK);
			 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			 mvaddstrAlt(y++, 1, CONST_shop059);
		 }
		 if (party_status != -1) set_color_easy(WHITE_ON_BLACK);
		 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		 mvaddstrAlt(++y, 1, show_squad_liberal_status);
		 if (partysize > 0 && (party_status == -1 || partysize > 1))
			 set_color_easy(WHITE_ON_BLACK);
		 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		 mvaddstrAlt(y++, 40, check_status_of_squad_liberal);
		 if (partysize >= 2) set_color_easy(WHITE_ON_BLACK);
		 else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		 mvaddstrAlt(y, 1, CONST_shop060);
		 set_color_easy(WHITE_ON_BLACK);
		 mvaddstrAlt(y, 40, CONST_shop061);
		 addstrAlt(exit_);
		 int c = getkeyAlt();
		 for (int i = 0; i < len(available_options); i++)
			 if (c == available_options[i]->letter_)
			 {
				 available_options[i]->choose(customers, buyer); break;
			 }
		 if (c == 'e' && customers.squad[0]->base != -1)
			 equipLoot(customers.squad[0]->base, -1);
		 else if (c == 's'&&allow_selling_&&LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, customers.squad[0]->base))
			 sell_loot(customers);
		 else if (c == 'm'&&sell_masks_&&ledger.get_funds() >= 15)
			 maskselect(*customers.squad[buyer]);
		 else if (c == 'b') choose_buyer(customers, buyer);
		 else if (c == '0') party_status = -1;
		 else if (c >= '1'&&c <= '6')
		 {
			 if (customers.squad[c - '1'] != NULL)
			 {
				 if (party_status == c - '1')
					 fullstatus(party_status);
				 else party_status = c - '1';
			 }
		 }
		 else if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
	 }
 }
 bool Shop::is_available() const
 {  //Disable shop (department) if it has nothing to sell.
	 bool r = false;
	 for (int i = 0; i < len(options_) && !(r = options_[i]->display()); i++);
	 return r;
 }

 Shop::ShopItem::ShopItem(MCD_STR xmlstring, bool only_sell_legal,
	 bool increase_price_with_illegality)
	 : price_(0), only_sell_legal_(only_sell_legal),
	 increase_price_with_illegality_(increase_price_with_illegality),
	 description_defined_(false)
 {
	 CMarkup xml;
	 xml.SetDoc(xmlstring);
	 xml.FindElem();
	 xml.IntoElem();
	 while (xml.FindElem())
	 {
		 std::string tag = xml.GetTagName();
		 if (shopItemTags.count(tag)) {
			 switch (shopItemTags[tag]) {

			 case ENUM_tag_class:
				 if (xml.GetData() == tag_WEAPON)
					 itemclass_ = WEAPON;
				 else if (xml.GetData() == tag_CLIP)
					 itemclass_ = CLIP;
				 else if (xml.GetData() == tag_ARMOR)
					 itemclass_ = ARMOR;
				 else if (xml.GetData() == tag_LOOT)
					 itemclass_ = LOOT;
				 break;
			 case ENUM_tag_type:
				 itemtypename_ = xml.GetData();
				 break;
			 case ENUM_tag_description:
				 description_ = xml.GetData();
				 description_defined_ = true;
				 break;
			 case ENUM_tag_price:
				 price_ = atoi(xml.GetData());
				 break;
			 case ENUM_tag_sleeperprice:
				 sleeperprice_ = atoi(xml.GetData());
				 break;
			 case ENUM_tag_letter:
				 letter_ = xml.GetData()[0];
				 if (97 <= letter_ && letter_ <= 122) //Check it is a letter.
					 letter_defined_ = true;
				 else if (65 <= letter_ && letter_ <= 90)
				 {
					 letter_ += 32;
					 letter_defined_ = true;
				 }
				 else if (letter_ == '!') //Allow special character.
					 letter_defined_ = true;
				 break;


			 }
		 }
	 }
 }

 bool Shop::ShopItem::is_available() const
 {
	 return valid_item() && (!only_sell_legal_ || legal()) && can_afford();
 }
 const std::string Shop::ShopItem::get_description_halfscreen() const
 {
	 std::string r = get_description();
	 r.resize(26, ' ');
	 r += paranthesisDollar + tostring(adjusted_price()) + closeParenthesis;
	 return r;
 }
 const std::string Shop::ShopItem::get_description_fullscreen() const
 {
	 std::string r = get_description();
	 r.resize(35, ' ');
	 r += CONST_shop062 + tostring(adjusted_price());
	 return r;
 }
 bool Shop::ShopItem::display() const
 {
	 return(valid_item() && (!only_sell_legal_ || legal()));
 }
 bool Shop::ShopItem::can_afford() const
 {
	 return(adjusted_price() <= ledger.get_funds());
 }
 bool Shop::ShopItem::legal() const
 {
	 bool r = true;
	 switch (itemclass_)
	 {
	 case WEAPON: r = weapontype[getweapontype(itemtypename_)]->is_legal(); break;
	 case CLIP:
		 // Decide if clip is legal by looping through all weapons and
		 // testing if there exists a weapon such that it is legal and
		 // it can take this clip. If no legal weapon can take this type
		 // of clip, the clip is implicitly illegal as well.
		 r = false;
		 for (int i = 0; i < len(weapontype) && !r; i++)
			 if (weapontype[i]->acceptable_ammo(itemtypename_) && weapontype[i]->is_legal())
				 r = true;
		 break;
	 case ARMOR:       //Can't be illegal.
	 case LOOT: break; //Can't be illegal.
	 }
	 return r;
 }
 bool Shop::ShopItem::valid_item() const
 {
	 int i = -1;
	 switch (itemclass_)
	 {
	 case WEAPON: i = getweapontype(itemtypename_); break;
	 case CLIP:   i = getcliptype(itemtypename_); break;
	 case ARMOR:  i = getarmortype(itemtypename_); break;
	 case LOOT:   i = getloottype(itemtypename_); break;
	 }
	 return(i != -1);
 }
 int Shop::ShopItem::adjusted_price() const
 {
	 int p = price_;
	 if (increase_price_with_illegality_&&itemclass_ == WEAPON && valid_item())
		 for (int i = weapontype[getweapontype(itemtypename_)]->get_legality(); i < lawList[LAW_GUNCONTROL]; i++)
			 p *= 2;
	 return p;
 }
 const std::string Shop::ShopItem::get_description() const
 {
	 if (description_defined_) return description_;
	 else switch (itemclass_)
	 {
	 case WEAPON: return weapontype[getweapontype(itemtypename_)]->get_name();
	 case CLIP:   return cliptype[getcliptype(itemtypename_)]->get_name();
	 case ARMOR:  return armortype[getarmortype(itemtypename_)]->get_name();
	 case LOOT:   return LootTypePool::getInstance().getName(itemtypename_); // loottype[getloottype(itemtypename_)]->get_name();
	 default:     return description_; // Will be undefined
	 }
 }

 void Shop::ShopItem::choose(Deprecatedsquadst& customers, int& buyer) const
 {
	 if (!is_available()) return;
	 ledger.subtract_funds(adjusted_price(), EXPENSE_SHOPPING);
	 switch (itemclass_)
	 {
	 case WEAPON: {
		 Weapon* i = new Weapon(*weapontype[getweapontype(itemtypename_)]);
		 customers.squad[buyer]->give_weapon(*i, &location[customers.squad[0]->base]->loot);
		 if (i->empty()) delete i;
		 else location[customers.squad[0]->base]->loot.push_back(i);
		 break; }
	 case CLIP: {
		 Clip* i = new Clip(*cliptype[getcliptype(itemtypename_)]);
		 customers.squad[buyer]->take_clips(*i, 1);
		 if (i->empty()) delete i;
		 else location[customers.squad[0]->base]->loot.push_back(i);
		 break; }
	 case ARMOR: {
		 Armor* i = new Armor(getarmortype(itemtypename_));
		 customers.squad[buyer]->give_armor(*i, &location[customers.squad[0]->base]->loot);
		 if (i->empty()) delete i;
		 else location[customers.squad[0]->base]->loot.push_back(i);
		 break; }
	 case LOOT: {
		 location[customers.squad[0]->base]->loot.push_back(getNewLoot(itemtypename_));
		 break; }
	 }
 }
