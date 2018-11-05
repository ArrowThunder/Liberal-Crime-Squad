
#include "../includes.h"

const string tag_LOOT = "LOOT";
const string tag_LOOT_SECRETDOCUMENTS = "LOOT_SECRETDOCUMENTS";
const string tag_LOOT_INTHQDISK = "LOOT_INTHQDISK";
const string tag_The_Bronx = "The Bronx";
const string tag_Long_Island = "Long Island";
const string tag_Brooklyn_ampersand_Queens = "Brooklyn & Queens";
const string tag_B = "B";
const string tag_Manhattan = "Manhattan";
const string tag_The = "The ";
const string tag_Manhattan_Island = "Manhattan Island";
const string tag_Greater_Hollywood = "Greater Hollywood";
const string tag_Hollywood = "Hollywood";
const string tag_Arlington = "Arlington";
const string tag_City_Outskirts = "City Outskirts";
const string tag_Seaport_Area = "Seaport Area";
const string tag_Shopping = "Shopping";
const string tag_Seaport = "Seaport";
const string tag_Outskirts_amp_Orange_County = "Outskirts & Orange County";
const string tag_Outskirts = "Outskirts";
const string tag_University_District = "University District";
const string tag_i_District = "I-District";
const string tag_Industrial_District = "Industrial District";
const string tag_u_District = "U-District";
const string tag_D = "D";
const string tag_Downtown = "Downtown";
const string tag_National_Mall = "National Mall";
const string tag_ARMOR = "ARMOR";
const string tag_ARMOR_CLOTHES = "ARMOR_CLOTHES";
const string tag_CLIP = "CLIP";
const string tag_CLIP_9 = "CLIP_9";
const string tag_WEAPON = "WEAPON";
const string tag_WEAPON_SEMIPISTOL_9MM = "WEAPON_SEMIPISTOL_9MM";
const string tag_WEAPON_SMG_MP5 = "WEAPON_SMG_MP5";
const string tag_WEAPON_AUTORIFLE_AK47 = "WEAPON_AUTORIFLE_AK47";
const string blankString = "";
#include "../vehicle/vehicletype.h"
#include "../vehicle/vehicle.h"
#include "../creature/creature.h"
////

//#include "../creature/deprecatedCreatureD.h"

////
#include "../locations/locations.h"
//#include "../items/armortype.h"
#include "../common/ledgerEnums.h"
#include "../common/ledger.h"
#include "../common/commondisplay.h"
// for void printfunds(int,int,char*)
#include "../common/stringconversion.h"
//for string attribute_enum_to_string(int)
#include "../common/getnames.h"
// for cityname
#include "../common/translateid.h"
// for  getarmortype
#include "../cursesAlternative.h"
#include "../cursesAlternativeConstants.h"
#include "../set_color_support.h"
#include "locationsPool.h"
#include "../common/creaturePoolCreature.h"
#include "../items/money.h"
#include "../recruits.h"
vector<Location *> listOfLocations;

Vehicle* newSportsCar();
void newVehicle(Vehicle *startcar);
void DeprecatedCreature::initiateNewgameLocations(const newGameArguments ngm) {
	// Gives you bloody armor
	extern bool GIVEBLOODYARMOR;
	// Start with lots of money
	extern bool HIGHFUNDS;
	Deprecatedsquadst *newsq = new Deprecatedsquadst;
	extern class Ledger ledger;
	extern long cursquadid;
	newsq->id = 0; cursquadid++;
	newsq->squad[0] = this;
	squadid = 0;
	extern string theLCS;
	strcpy(newsq->name, theLCS);
	extern vector<ClipType *> cliptype;
	extern vector<WeaponType *> weapontype;
	extern vector<Deprecatedsquadst *> squad;
	// This is not a proper for loop, it only iterates to find a single instance of a valid location
	// then executes its code once, and then break;
	for (int l = 0; l < len(listOfLocations); l++)
	{
		if (listOfLocations[l]->type == ngm.base)
		{
			base = l;
			this->location = l;
			if (ngm.sports_car) {

				Vehicle * startcar = newSportsCar();
				startcar->add_heat(10);
				newVehicle(startcar);
				pref_carid = startcar->id();
				
				startcar->set_location(l);
				
			}
			switch (ngm.base)
			{
			case SITE_RESIDENTIAL_APARTMENT_UPSCALE:listOfLocations[l]->renting = 500; break;
			case SITE_RESIDENTIAL_APARTMENT:listOfLocations[l]->renting = 200; break;
			case SITE_RESIDENTIAL_TENEMENT:listOfLocations[l]->renting = 100; break;
			case SITE_BUSINESS_CRACKHOUSE:
				listOfLocations[l]->renting = RENTING_PERMANENT;
				listOfLocations[l]->compound_stores += 100;
				break;
			}
			listOfLocations[l]->newrental = 1;
			switch (ngm.recruits)
			{
			case RECRUITS_GANG:
				for (int i = 0; i < 4; i++)
				{
					DeprecatedCreature* recruit = new DeprecatedCreature;
					recruit->makecreature( CREATURE_GANGMEMBER);
					if (recruit->get_weapon().get_itemtypename() == tag_WEAPON_AUTORIFLE_AK47 ||
						recruit->get_weapon().get_itemtypename() == tag_WEAPON_SMG_MP5 ||
						!recruit->is_armed())
					{
						Weapon w(*weapontype[getweapontype(tag_WEAPON_SEMIPISTOL_9MM)]);
						recruit->give_weapon(w, NULL);
						Clip c(*cliptype[getcliptype(tag_CLIP_9)], 4);
						recruit->take_clips(c, 4);
						recruit->reload(false);
					}
					recruit->align = ALIGN_LIBERAL;
					recruit->set_attribute(ATTRIBUTE_HEART,
						recruit->get_attribute(ATTRIBUTE_HEART, false) +
						recruit->get_attribute(ATTRIBUTE_WISDOM, false) / 2);
					recruit->set_attribute(ATTRIBUTE_WISDOM,
						recruit->get_attribute(ATTRIBUTE_WISDOM, false) / 2);
					recruit->namecreature();
					recruit->rename(recruit->propername);
					recruit->location = l;
					recruit->base = l;
					recruit->hireid = id;
					newsq->squad[i + 1] = recruit;
					recruit->squadid = newsq->id;
					recruit->addCreature();
				}
				break;
			}
			if (GIVEBLOODYARMOR) {
				Armor *newa = new Armor(getarmortype(tag_ARMOR_CLOTHES));
				newa->set_bloody(true);
				listOfLocations[l]->loot.push_back(newa);
			}
			if (HIGHFUNDS) {
				ledger.force_funds(100000);
			}
			break;
		}
	}
	extern Deprecatedsquadst *activesquad;
	//newcr->juice=0;
	squad.push_back(newsq);
	activesquad = newsq;
	if (ngm.makelawyer)
	{
		DeprecatedCreature* lawyer = new DeprecatedCreature;
		lawyer->makecreature(CREATURE_LAWYER);
		// Make sure lawyer is of the appropriate gender for dating the main character;
		// opposite sex by default, same sex if the option was chosen that mentions
		// homosexuality
		if (ngm.gaylawyer)
		{
			lawyer->gender_conservative = lawyer->gender_liberal = gender_conservative;
			// neutral founder gets neutral partner
		}
		else
		{
			if (gender_conservative == GENDER_MALE)
				lawyer->gender_liberal = lawyer->gender_conservative = GENDER_FEMALE;
			if (gender_conservative == GENDER_FEMALE)
				lawyer->gender_liberal = lawyer->gender_conservative = GENDER_MALE;
			// neutral founder gets random partner
		}
		// Ensure the lawyer has good heart/wisdom stats
		if (lawyer->get_attribute(ATTRIBUTE_HEART, false) < get_attribute(ATTRIBUTE_HEART, false) - 2)
			lawyer->adjust_attribute(ATTRIBUTE_HEART, -2);
		lawyer->set_attribute(ATTRIBUTE_WISDOM, 1);
		lawyer->namecreature();
		lawyer->flag |= CREATUREFLAG_SLEEPER;
		lawyer->flag |= CREATUREFLAG_LOVESLAVE;
		lawyer->align = ALIGN_LIBERAL;
		lawyer->infiltration = 0.3f;
		lawyer->set_age(28);
		listOfLocations[lawyer->worklocation]->mapped = 1;
		lawyer->hireid = id;
		lawyer->addCreature();
		lawyer->location = lawyer->base = lawyer->worklocation;
	}
}
bool LocationsPool::locationPoolInitiated = false;
LocationsPool lPool;
LocationsPool LocationsPool::getInstance()
{
	if (!locationPoolInitiated) {
		lPool = LocationsPool();
		locationPoolInitiated = true;
	}
	return lPool;
}
const int LocationsPool::lenpool() const
{
	return len(listOfLocations);
}
const int LocationsPool::getLocationCity(int cursite)const {
	return listOfLocations[cursite]->city;
}
void LocationsPool::hideCCSSafehouses()
{
	// hide ccs safehouses
	for (int l = 0; l < lenpool(); l++)
	{
		if (getRentingType(l) == RENTING_CCS)
		{
			listOfLocations[l]->renting = RENTING_NOCONTROL;
			listOfLocations[l]->hidden = true;
		}
	}
}
void LocationsPool::addHeat(int cursite, int heat)
{
	listOfLocations[cursite]->heat += heat;
}
#include "../common/equipment.h"

const bool LocationsPool::isLocationMapped(int cursite)const
{
	return listOfLocations[cursite]->mapped;
}
const bool LocationsPool::isLocationHidden(int cursite)const
{
	return listOfLocations[cursite]->hidden;
}
void LocationsPool::setLocationMappedAndUnhidden(int cursite)
{
	listOfLocations[cursite]->mapped = 1;
	listOfLocations[cursite]->hidden = 0;
}
const string LocationsPool::getLocationNameWithGetnameMethod(int cursite, signed char a)const {
	return getLocationNameWithGetnameMethod(cursite, a, false);
}
const string LocationsPool::getLocationNameWithGetnameMethod(int cursite, signed char a, bool b)const
{
	return listOfLocations[cursite]->getname(a, b);
}
const int LocationsPool::getCompoundWalls(int cursite)const
{
	return listOfLocations[cursite]->compound_walls;
}
// Redundant, carelessly, worry later TODO
void LocationsPool::setTimeUntilSiege(int cursite, int time)
{
	LocationsPool::getInstance().setSiegetimeuntillocated(cursite, time);
}
const bool LocationsPool::isNewRental(int cursite)const
{
	return listOfLocations[cursite]->newrental;
}
#include "../common/creaturePool.h"
void LocationsPool::evictLCSFrom(int l)
{
	const string CONST_locationsPool036 = ".  Possessions go to the shelter.";
	const string CONST_locationsPool035 = "EVICTION NOTICE: ";

	set_color_easy(WHITE_ON_BLACK_BRIGHT);
	mvaddstrAlt(8, 1, CONST_locationsPool035);
	addstrAlt(LocationsPool::getInstance().getLocationName(l));
	addstrAlt(CONST_locationsPool036);
	pressAnyKey();
	listOfLocations[l]->renting = RENTING_NOCONTROL;
	CreaturePool::getInstance().moveAllSquadMembers(l);
	int hs = find_site_index_in_same_city(SITE_RESIDENTIAL_SHELTER, l);
	listOfLocations[hs]->getloot(listOfLocations[l]->loot);
	listOfLocations[l]->compound_walls = 0;
	listOfLocations[l]->compound_stores = 0;
	listOfLocations[l]->front_business = -1;
}
const int LocationsPool::isThisSiteClosed(int cursite)const
{
	return listOfLocations[cursite]->closed;
}
const char LocationsPool::doesThisPlaceNeedACar(int cursite)const
{
	return listOfLocations[cursite]->needcar;
}
const int LocationsPool::findTravelLocation()const
{
	int travelLocation = -1;
	for (int i = 0; i < len(listOfLocations); i++) {
		if (listOfLocations[i]->type == SITE_TRAVEL)
		{
			travelLocation = i;
			break;
		}
	}
	return travelLocation;
}
const int LocationsPool::getLocationParent(int cursite)const
{
	return listOfLocations[cursite]->parent;
}
int LocationsPool::deleteSpecialItem(int slot, vector<int> loottypeindex)
{
	//DELETE THE ITEM
	for (int loc = 0; loc < LocationsPool::getInstance().lenpool(); loc++)
	{
		if (LocationsPool::getInstance().get_specific_integer(INT_GETRENTINGTYPE,loc) == RENTING_NOCONTROL) continue;
		for (int l = 0; l < len(listOfLocations[loc]->loot); l++)
		{
			if (!(listOfLocations[loc]->loot[l]->whatIsThis() == THIS_IS_LOOT)) continue;
			if (getloottype(listOfLocations[loc]->loot[l]->get_itemtypename()) == loottypeindex[slot])
			{
				listOfLocations[loc]->loot[l]->decrease_number(1);
				if (listOfLocations[loc]->loot[l]->empty())
					delete_and_remove(listOfLocations[loc]->loot, l);
				return loottypeindex[slot];
			}
		}
	}
	return -1;
}
void LocationsPool::getAssetValues(long & weaponValue, long & armorValue, long & clipValue, long & lootValue)const
{
	for (int j = 0; j < LocationsPool::getInstance().lenpool(); j++)
		for (int i = 0; i < len(listOfLocations[j]->loot); i++)
		{
			Item* item = listOfLocations[j]->loot[i];
			if (item->whatIsThis() == THIS_IS_WEAPON) weaponValue += item->get_fencevalue()*item->get_number();
			if (item->whatIsThis() == THIS_IS_ARMOR) armorValue += item->get_fencevalue()*item->get_number();
			if (item->whatIsThis() == THIS_IS_CLIP) clipValue += item->get_fencevalue()*item->get_number();
			if (item->whatIsThis() == THIS_IS_LOOT) lootValue += item->get_fencevalue()*item->get_number();
		}
}
const bool LocationsPool::canBeFortified(int cursite)const
{
	return listOfLocations[cursite]->can_be_fortified();
}
/* daily - seeds and names a site (will re-seed and rename if used after start) */
void initlocation(Location &loc);
void LocationsPool::initLocation(int cursite)
{
	initlocation(*listOfLocations[cursite]);
}
#include "../items/lootTypePoolItem.h"
void LocationsPool::stashThisLootHere(const string& tag, int homes) {
	listOfLocations[homes]->loot.push_back(getNewLoot(tag));
}
void LocationsPool::stashThisWeaponHere(int itemindex, int shelter) {
	extern vector<WeaponType *> weapontype;
	listOfLocations[shelter]->loot.push_back(new Weapon(*weapontype[itemindex]));
}
void LocationsPool::stashThisArmorHere(int itemindex, int shelter) {
	listOfLocations[shelter]->loot.push_back(new Armor(itemindex));
}
void LocationsPool::equipLoc(int loc, int y)
{
	DeprecatedCreature::equip(listOfLocations[loc]->loot, y);
}
void LocationsPool::delete_and_clear_pool()
{
	delete_and_clear(listOfLocations);
}
const char LocationsPool::isThereASiegeHere(int cursite)const
{
	return listOfLocations[cursite]->siege.siege;
}
const int LocationsPool::isThisPlaceHighSecurity(int cursite)const
{
	return listOfLocations[cursite]->highsecurity;
}
void LocationsPool::isThereASiegeHere(int cursite, char newCondition)const
{
	listOfLocations[cursite]->siege.siege = newCondition;
}
void LocationsPool::isThisPlaceHighSecurity(int cursite, int newCondition)const
{
	listOfLocations[cursite]->highsecurity = newCondition;
}
const char LocationsPool::getLocationType(int cursite)const
{
	return listOfLocations[cursite]->type;
}
const short LocationsPool::getSiegeType(int cursite)const
{
	return listOfLocations[cursite]->siege.siegetype;
}
const short LocationsPool::getSiegeEscalationState(int cursite)const
{
	return listOfLocations[cursite]->siege.escalationstate;
}
const int LocationsPool::getRentingType(int cursite)const
{
	return listOfLocations[cursite]->renting;
}
//#include "sitemode/sitemode.h"
void initsite(Location &loc);
void LocationsPool::initSite(int loc)
{
	initsite(*listOfLocations[loc]);
}
void LocationsPool::eraseAndReplaceGraffiti(int cursite, int locx, int locy, int locz)
{
	// Erase any previous semi-permanent graffiti here
	for (int i = 0; i < len(listOfLocations[cursite]->changes); i++)
	{
		if ((listOfLocations[cursite]->changes[i].x == locx) &&
			(listOfLocations[cursite]->changes[i].y == locy) &&
			(listOfLocations[cursite]->changes[i].z == locz) &&
			((listOfLocations[cursite]->changes[i].flag == SITEBLOCK_GRAFFITI) ||
			(listOfLocations[cursite]->changes[i].flag == SITEBLOCK_GRAFFITI_CCS) ||
				(listOfLocations[cursite]->changes[i].flag == SITEBLOCK_GRAFFITI_OTHER)))
		{
			listOfLocations[cursite]->changes.erase(listOfLocations[cursite]->changes.begin() + i);
			break;
		}
	}
	// Add new semi-permanent graffiti
	struct sitechangest change(locx, locy, locz, SITEBLOCK_GRAFFITI);
	listOfLocations[cursite]->changes.push_back(change);
}

const string LocationsPool::getLocationName(int cursite)const
{
	return listOfLocations[cursite]->getname();
}
#include "../common/commonactionsCreature.h"
/* common - purges empty squads from existence */
void cleangonesquads()
{
	extern Deprecatedsquadst *activesquad;
	extern vector<Deprecatedsquadst *> squad;
	for (int sq = len(squad) - 1; sq >= 0; sq--)
	{  //NUKE SQUAD IF IT IS GONE
		bool hasmembers = false;
		for (int p = 0; p < 6; p++)
			if (squad[sq]->squad[p] != NULL)
			{  // Let's do a bit of housekeeping here
			   // And see if we can't gracefully eliminate that
			   // pesky dead liberal in my squad bug
				if (squad[sq]->squad[p]->getNameAndAlignment().alive == false)
				{
					squad[sq]->squad[p]->removesquadinfo();
					p = -1; // restart this for loop
				}
				else hasmembers = true;
			}
		if (!hasmembers)
		{  //SQUAD LOOT WILL BE DESTROYED
			if (activesquad == squad[sq]) activesquad = NULL;
			delete_and_remove(squad, sq);
		}
		//OTHERWISE YOU CAN TAKE ITS MONEY (and other gear)
		else listOfLocations[squad[sq]->squad[0]->base]->getloot(squad[sq]->loot);
	}
}
//#include "pdcurses/curses.h"
extern string singleDot;
// Prompt to turn new recruit into a sleeper
void DeprecatedCreature::sleeperize_prompt(DeprecatedCreature &recruiter, int y)
{
	extern vector<Location *> listOfLocations;
	const string CONST_locationsPool046 = "sleeper agent";
	const string CONST_locationsPool045 = " as a ";
	const string CONST_locationsPool044 = "Stay at ";
	const string CONST_locationsPool043 = "-> ";
	const string CONST_locationsPool042 = "regular member";
	const string CONST_locationsPool040 = "Come to ";
	const string CONST_locationsPool039 = "   ";
	const string CONST_locationsPool038 = " best serve the Liberal cause?";
	const string CONST_locationsPool037 = "In what capacity will ";

	bool selection = false;
	while (true)
	{
		set_color_easy(WHITE_ON_BLACK);
		mvaddstrAlt(y, 0, CONST_locationsPool037);
		addstrAlt(getNameAndAlignment().name);
		addstrAlt(CONST_locationsPool038);
		set_color_easy(selection ? WHITE_ON_BLACK : WHITE_ON_BLACK_BRIGHT);
		mvaddstrAlt(y + 2, 0, selection ? CONST_locationsPool039 : CONST_locationsPool043);
		addstrAlt(CONST_locationsPool040);
		addstrAlt(listOfLocations[recruiter.location]->getname(-1, true));
		addstrAlt(CONST_locationsPool045);
		set_color_easy(selection ? GREEN_ON_BLACK : GREEN_ON_BLACK_BRIGHT);
		addstrAlt(CONST_locationsPool042);
		set_color_easy(selection ? WHITE_ON_BLACK : WHITE_ON_BLACK_BRIGHT);
		addstrAlt(singleDot);
		set_color_easy(selection ? WHITE_ON_BLACK_BRIGHT : WHITE_ON_BLACK);
		mvaddstrAlt(y + 3, 0, selection ? CONST_locationsPool043 : CONST_locationsPool039);
		addstrAlt(CONST_locationsPool044);
		addstrAlt(listOfLocations[worklocation]->getname(-1, true));
		addstrAlt(CONST_locationsPool045);
		set_color_easy(selection ? CYAN_ON_BLACK_BRIGHT : CYAN_ON_BLACK);
		addstrAlt(CONST_locationsPool046);
		set_color_easy(selection ? WHITE_ON_BLACK_BRIGHT : WHITE_ON_BLACK);
		addstrAlt(singleDot);
		int c = getkeyAlt();
		if ((c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) && selection)
		{
			flag |= CREATUREFLAG_SLEEPER;
			this->location = worklocation;
			listOfLocations[worklocation]->mapped = 1;
			listOfLocations[worklocation]->hidden = 0;
			base = worklocation;
			liberalize(false);
			break;
		}
		else if ((c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) && !selection)
		{
			this->location = recruiter.location;
			base = recruiter.base;
			liberalize(false);
			break;
		}
		else if (is_page_up(c) || is_page_down(c)) selection = !selection;
	}
}
Location* find_site_in_city(const int site_type, const int city)
{
	int i = find_site_index_in_city(site_type, city);
	if (i != -1) return listOfLocations[i];
	else return NULL;
}
int find_site_index_in_city(const int site_type, const int city)
{
	extern bool multipleCityMode;
	for (int i = 0; i < len(listOfLocations); i++)
		if (listOfLocations[i]->type == site_type && (!multipleCityMode || city == -1 || listOfLocations[i]->city == city))
			return i;
	return -1;
}
int find_site_index_in_same_city(const int site_type, const int site_index)
{
	int city = -1;
	if (site_index >= 0) city = listOfLocations[site_index]->city;
	return find_site_index_in_city(site_type, city);
}
void addChildren(Location* district, const vector<SiteTypes> subdistricts) {
	for (SiteTypes st : subdistricts) {
		district->addchild(st);
	}
}
void addChildren(Location* district, const vector<SiteTypes> subdistricts, const bool hasmaps) {
	for (SiteTypes st : subdistricts) {
		district->addchild(st)->mapped = hasmaps;
	}
}
vector<SiteTypes> classicDowntown = {
	SITE_RESIDENTIAL_APARTMENT_UPSCALE,
	SITE_GOVERNMENT_POLICESTATION,
	SITE_GOVERNMENT_COURTHOUSE,
	SITE_BUSINESS_BANK,
	SITE_GOVERNMENT_FIRESTATION,
	SITE_MEDIA_AMRADIO,
	SITE_MEDIA_CABLENEWS,
	SITE_BUSINESS_CIGARBAR,
	SITE_BUSINESS_LATTESTAND,
};
vector<SiteTypes> classicCommercial = {

	SITE_BUSINESS_DEPTSTORE,
	SITE_BUSINESS_PAWNSHOP,
	SITE_BUSINESS_HALLOWEEN,
	SITE_BUSINESS_CARDEALERSHIP,
};
vector<SiteTypes> classicUptown = {

	SITE_RESIDENTIAL_APARTMENT,
	SITE_HOSPITAL_UNIVERSITY,
	SITE_HOSPITAL_CLINIC,
	SITE_LABORATORY_GENETIC,
	SITE_LABORATORY_COSMETICS,
	SITE_BUSINESS_VEGANCOOP,
	SITE_BUSINESS_JUICEBAR,
	SITE_BUSINESS_INTERNETCAFE,
	SITE_OUTDOOR_PUBLICPARK,
};
vector<SiteTypes> classicOutOfTown = {

	SITE_GOVERNMENT_PRISON,
	SITE_GOVERNMENT_INTELLIGENCEHQ,
	SITE_INDUSTRY_NUCLEAR,
	SITE_CORPORATE_HEADQUARTERS,
	SITE_CORPORATE_HOUSE,
	SITE_GOVERNMENT_ARMYBASE,
};



void make_classic_world(bool hasmaps)
{
	Location* district = NULL;
	listOfLocations.push_back(district = new Location(SITE_DOWNTOWN));
	district->area = 0;
	district->mapped = hasmaps; // for some reason this property isn't inherited by downtown locations so it's manually added for each one, need to debug why this happens
	addChildren(district, classicDowntown, hasmaps);
	{
		Location* site = district->addchild(SITE_BUSINESS_BARANDGRILL);
		site->renting = RENTING_CCS;
		site->hidden = true;
		site->mapped = false;
	}
	listOfLocations.push_back(district = new Location(SITE_COMMERCIAL));
	district->area = 0;
	addChildren(district, classicCommercial);

	listOfLocations.push_back(district = new Location(SITE_UDISTRICT));
	district->area = 0;
	addChildren(district, classicUptown);

	listOfLocations.push_back(district = new Location(SITE_INDUSTRIAL));
	district->area = 0;
	district->addchild(SITE_RESIDENTIAL_SHELTER)->renting = RENTING_PERMANENT;
	{
		Location* site = district->addchild(SITE_INDUSTRY_WAREHOUSE);
		site->renting = RENTING_PERMANENT;
		site->upgradable = true;
	}
	district->addchild(SITE_RESIDENTIAL_TENEMENT);
	district->addchild(SITE_INDUSTRY_POLLUTER);
	district->addchild(SITE_INDUSTRY_SWEATSHOP);
	district->addchild(SITE_BUSINESS_CRACKHOUSE)->upgradable = true;
	{
		Location* site = district->addchild(SITE_RESIDENTIAL_BOMBSHELTER);
		site->renting = RENTING_CCS;
		site->hidden = true;
	}
	listOfLocations.push_back(district = new Location(SITE_OUTOFTOWN));
	district->area = 1;
	addChildren(district, classicOutOfTown);
	{
		Location* site = district->addchild(SITE_OUTDOOR_BUNKER);
		site->renting = RENTING_CCS;
		site->hidden = true;
	}
	listOfLocations.push_back(district = new Location(SITE_TRAVEL));
	district->area = 1;
	district->addchild(SITE_GOVERNMENT_WHITE_HOUSE);
}


vector<SiteTypes> seattleDowntown = {

	SITE_RESIDENTIAL_APARTMENT_UPSCALE,
	SITE_GOVERNMENT_POLICESTATION,
	SITE_GOVERNMENT_COURTHOUSE,
	SITE_BUSINESS_BANK,
	SITE_GOVERNMENT_FIRESTATION,
	SITE_MEDIA_AMRADIO,
	SITE_BUSINESS_CIGARBAR,
	SITE_BUSINESS_LATTESTAND,
	SITE_BUSINESS_DEPTSTORE,
};

vector<SiteTypes> seattleUptown = {

	SITE_RESIDENTIAL_APARTMENT,
	SITE_HOSPITAL_UNIVERSITY,
	SITE_HOSPITAL_CLINIC,
	SITE_LABORATORY_GENETIC,
	SITE_LABORATORY_COSMETICS,
	SITE_BUSINESS_VEGANCOOP,
	SITE_BUSINESS_JUICEBAR,
	SITE_BUSINESS_INTERNETCAFE,
	SITE_OUTDOOR_PUBLICPARK,
	SITE_BUSINESS_HALLOWEEN,
};
vector<SiteTypes> seattleOutOfTown = {

	SITE_GOVERNMENT_PRISON,
	SITE_GOVERNMENT_INTELLIGENCEHQ,
	SITE_CORPORATE_HEADQUARTERS,
	SITE_GOVERNMENT_ARMYBASE,
};

vector<SiteTypes> newYorkManhattan = {

	SITE_RESIDENTIAL_APARTMENT_UPSCALE,
	SITE_GOVERNMENT_POLICESTATION,
	SITE_GOVERNMENT_COURTHOUSE,
	SITE_BUSINESS_BANK,
	SITE_CORPORATE_HEADQUARTERS,
	SITE_MEDIA_AMRADIO,
	SITE_MEDIA_CABLENEWS,
	SITE_BUSINESS_CIGARBAR,
	SITE_OUTDOOR_PUBLICPARK,
	SITE_BUSINESS_DEPTSTORE,
	SITE_GOVERNMENT_PRISON,
};

vector<SiteTypes> newYorkLongIsland = {
	SITE_RESIDENTIAL_APARTMENT,
	SITE_GOVERNMENT_FIRESTATION,
	SITE_HOSPITAL_UNIVERSITY,
	SITE_HOSPITAL_CLINIC,
	SITE_BUSINESS_JUICEBAR,
	SITE_BUSINESS_INTERNETCAFE,
	SITE_INDUSTRY_POLLUTER,
	SITE_LABORATORY_GENETIC,
	SITE_GOVERNMENT_ARMYBASE,
};


vector<SiteTypes> losAngelesDowntown = {



	SITE_RESIDENTIAL_APARTMENT,
	SITE_GOVERNMENT_POLICESTATION,
	SITE_GOVERNMENT_COURTHOUSE,
	SITE_BUSINESS_BANK,
	SITE_GOVERNMENT_FIRESTATION,
	SITE_CORPORATE_HEADQUARTERS,
	SITE_HOSPITAL_UNIVERSITY,
	SITE_BUSINESS_DEPTSTORE,

};


vector<SiteTypes> hollyWoodUptown = {


	SITE_RESIDENTIAL_APARTMENT_UPSCALE,
	SITE_BUSINESS_VEGANCOOP,
	SITE_BUSINESS_HALLOWEEN,
	SITE_BUSINESS_CIGARBAR,
	SITE_MEDIA_AMRADIO,
	SITE_OUTDOOR_PUBLICPARK,
	SITE_CORPORATE_HOUSE,


};


vector<SiteTypes> seaportArea = {


	SITE_RESIDENTIAL_TENEMENT,
	SITE_HOSPITAL_CLINIC,
	SITE_LABORATORY_GENETIC,
	SITE_LABORATORY_COSMETICS,
	SITE_INDUSTRY_POLLUTER,
	SITE_BUSINESS_PAWNSHOP,
	SITE_INDUSTRY_SWEATSHOP,
	SITE_BUSINESS_CARDEALERSHIP,


};

vector<SiteTypes> washingtonDCDowntown = {

	SITE_GOVERNMENT_POLICESTATION,
	SITE_GOVERNMENT_FIRESTATION,
	SITE_GOVERNMENT_COURTHOUSE,
	SITE_BUSINESS_BANK,
	SITE_BUSINESS_CARDEALERSHIP,
	SITE_HOSPITAL_CLINIC,
	SITE_HOSPITAL_UNIVERSITY,
	SITE_BUSINESS_DEPTSTORE,

};
void make_world(const bool hasmaps)
{
	const string CONST_locationsPool047 = "Mall";

	extern bool multipleCityMode;
	if (!multipleCityMode)
	{
		make_classic_world(hasmaps);
		return;
	}
	//MAKE LOCATIONS
	Location* city = NULL;
	Location* district = NULL;
	//Location* site = NULL;
	listOfLocations.push_back(city = new Location(SITE_CITY_SEATTLE));
	district = city->addchild(SITE_DOWNTOWN);
	district->area = 0;
	district->mapped = hasmaps; // for some reason this property isn't inherited by downtown locations so it's manually added for each one, need to debug why this happens
	addChildren(district, seattleDowntown, hasmaps);
	{
		Location* site = district->addchild(SITE_BUSINESS_BARANDGRILL);
		site->renting = RENTING_CCS;
		site->hidden = true;
		site->mapped = false;
	}
	district = city->addchild(SITE_UDISTRICT);
	district->area = 0;
	addChildren(district, seattleUptown);
	district = city->addchild(SITE_INDUSTRIAL);
	district->area = 0;
	district->addchild(SITE_RESIDENTIAL_SHELTER)->renting = RENTING_PERMANENT;
	{
		Location* site = district->addchild(SITE_INDUSTRY_WAREHOUSE);
		site->renting = RENTING_PERMANENT;
		site->upgradable = true;
	}
	district->addchild(SITE_RESIDENTIAL_TENEMENT);
	district->addchild(SITE_INDUSTRY_POLLUTER);
	district->addchild(SITE_INDUSTRY_SWEATSHOP);
	district->addchild(SITE_BUSINESS_CRACKHOUSE)->upgradable = true;
	district->addchild(SITE_BUSINESS_PAWNSHOP);
	district->addchild(SITE_BUSINESS_CARDEALERSHIP);
	district = city->addchild(SITE_OUTOFTOWN);
	district->area = 1;
	addChildren(district, seattleOutOfTown);
	listOfLocations.push_back(city = new Location(SITE_CITY_NEW_YORK));
	district = city->addchild(SITE_DOWNTOWN);
	district->area = 0;
	strcpy(district->name, tag_Manhattan_Island);
	strcpy(district->shortname, tag_Manhattan);

	addChildren(district, newYorkManhattan);
	district = city->addchild(SITE_UDISTRICT);
	district->area = 0;
	strcpy(district->name, tag_Brooklyn_ampersand_Queens);
	strcpy(district->shortname, tag_Long_Island);
	{
		Location* site = district->addchild(SITE_INDUSTRY_WAREHOUSE);
		site->renting = RENTING_PERMANENT;
		site->upgradable = true;
	}
	addChildren(district, newYorkLongIsland);
	{
		Location* site = district->addchild(SITE_RESIDENTIAL_BOMBSHELTER);
		site->renting = RENTING_CCS;
		site->hidden = true;
	}
	district = city->addchild(SITE_INDUSTRIAL);
	district->area = 0;
	strcpy(district->name, tag_The_Bronx);
	strcpy(district->shortname, tag_The_Bronx);
	district->addchild(SITE_RESIDENTIAL_SHELTER)->renting = RENTING_PERMANENT;
	district->addchild(SITE_RESIDENTIAL_TENEMENT);
	district->addchild(SITE_INDUSTRY_POLLUTER);
	district->addchild(SITE_INDUSTRY_SWEATSHOP);
	district->addchild(SITE_LABORATORY_COSMETICS);
	district->addchild(SITE_BUSINESS_VEGANCOOP);
	district->addchild(SITE_BUSINESS_PAWNSHOP);
	district->addchild(SITE_BUSINESS_CARDEALERSHIP);
	district->addchild(SITE_BUSINESS_CRACKHOUSE)->upgradable = true;
	district->addchild(SITE_OUTDOOR_PUBLICPARK);
	district = city->addchild(SITE_OUTOFTOWN);
	district->area = 1;
	district->addchild(SITE_INDUSTRY_NUCLEAR);
	listOfLocations.push_back(city = new Location(SITE_CITY_LOS_ANGELES));
	district = city->addchild(SITE_DOWNTOWN);
	district->area = 0;
	district->addchild(SITE_RESIDENTIAL_SHELTER)->renting = RENTING_PERMANENT;

	addChildren(district, losAngelesDowntown);

	district = city->addchild(SITE_UDISTRICT);
	district->area = 0;
	strcpy(district->name, tag_Greater_Hollywood);
	strcpy(district->shortname, tag_Hollywood);

	addChildren(district, hollyWoodUptown);

	district = city->addchild(SITE_INDUSTRIAL);
	district->area = 0;
	strcpy(district->name, tag_Seaport_Area);
	strcpy(district->shortname, tag_Seaport);
	{
		Location* site = district->addchild(SITE_INDUSTRY_WAREHOUSE);
		site->renting = RENTING_PERMANENT;
		site->upgradable = true;
	}
	addChildren(district, seaportArea);
	district->addchild(SITE_BUSINESS_CRACKHOUSE)->upgradable = true;
	district = city->addchild(SITE_OUTOFTOWN);
	district->area = 1;
	strcpy(district->name, tag_Outskirts_amp_Orange_County);
	district->addchild(SITE_GOVERNMENT_PRISON);
	district->addchild(SITE_INDUSTRY_NUCLEAR);
	district->addchild(SITE_GOVERNMENT_ARMYBASE);
	{
		Location* site = district->addchild(SITE_OUTDOOR_BUNKER);
		site->renting = RENTING_CCS;
		site->hidden = true;
	}
	listOfLocations.push_back(city = new Location(SITE_CITY_WASHINGTON_DC));
	district = city->addchild(SITE_DOWNTOWN);
	district->area = 0;
	strcpy(district->name, tag_Downtown);
	addChildren(district, washingtonDCDowntown);
	district->addchild(SITE_RESIDENTIAL_SHELTER)->renting = RENTING_PERMANENT;
	district = city->addchild(SITE_UDISTRICT);
	district->area = 0;
	strcpy(district->name, tag_National_Mall);
	strcpy(district->shortname, CONST_locationsPool047);
	district->addchild(SITE_OUTDOOR_PUBLICPARK);
	district->addchild(SITE_GOVERNMENT_WHITE_HOUSE);
	district = city->addchild(SITE_OUTOFTOWN);
	district->area = 1;
	strcpy(district->name, tag_Arlington);
	district->addchild(SITE_GOVERNMENT_PRISON);
	district->addchild(SITE_GOVERNMENT_INTELLIGENCEHQ);
	district->addchild(SITE_GOVERNMENT_ARMYBASE);
}
// Finds a location with the corresponding type and returns
// its index in the location array
int findlocation(int type, int city = -1)
{
	extern bool multipleCityMode;
	if (!multipleCityMode) city = -1;
	for (int i = 0; i < len(listOfLocations); i++)
		if (listOfLocations[i]->type == type && (listOfLocations[i]->city == city || city == -1)) return i;
	return -1;
}
// Locations - Construct a new location with the specified parameters
#include "../daily/daily.h"
Location::Location(const char type_, const int parent_)
	: type(type_), city(-1), parent(parent_), renting(RENTING_NOCONTROL), needcar(false), hidden(false), upgradable(false)
{
	extern bool multipleCityMode;
	if (this->parent != -1)
	{
		this->needcar = listOfLocations[this->parent]->needcar;
		this->mapped = listOfLocations[this->parent]->mapped;
		this->area = listOfLocations[this->parent]->area;
		this->city = listOfLocations[this->parent]->city;
	}
	if (this->city < 0 && multipleCityMode)
		this->city = this->type;
	initlocation(*this);
}
Location* Location::addchild(const char type_)
{
	Location *newloc = new Location(type_, findlocation(this->type, this->city));
	listOfLocations.push_back(newloc);
	return newloc;
}
/* Settings for shortname_ (true is 1 and false is 0, by the way):
* -1: entire name is long, no matter what
*  0: first part of place name is long, and if there's a city at the end it's short
*  1: first part of the name is short unless the place itself is a city in which case it's long, and if there's a city at the end it's short
*  2: entire name is short, no matter what */
extern string singleSpace;
extern string commaSpace;
enum cityLocationTagEnums {
	ENUM_tag_Downtown,
	ENUM_tag_University_District,
	ENUM_tag_u_District,
	ENUM_tag_Industrial_District,
	ENUM_tag_i_District,
	ENUM_tag_Shopping,
	ENUM_tag_Outskirts,
	ENUM_tag_Seaport_Area,
	ENUM_tag_Seaport,
	ENUM_tag_Outskirts_amp_Orange_County,
	ENUM_tag_City_Outskirts,
	ENUM_tag_Arlington,
	ENUM_tag_Hollywood,
	ENUM_tag_Greater_Hollywood,
	ENUM_tag_Manhattan,
	ENUM_tag_Manhattan_Island,
	ENUM_tag_Brooklyn_ampersand_Queens,
	ENUM_tag_Long_Island,
	ENUM_tag_The_Bronx
};
map <string, int> cityLocationTags = {
	map<string, int>::value_type(tag_Downtown, ENUM_tag_Downtown),
	map<string, int>::value_type(tag_University_District, ENUM_tag_University_District),
	map<string, int>::value_type(tag_u_District, ENUM_tag_u_District),
	map<string, int>::value_type(tag_Industrial_District, ENUM_tag_Industrial_District),
	map<string, int>::value_type(tag_i_District, ENUM_tag_i_District),
	map<string, int>::value_type(tag_Shopping, ENUM_tag_Shopping),
	map<string, int>::value_type(tag_Outskirts, ENUM_tag_Outskirts),
	map<string, int>::value_type(tag_Seaport_Area, ENUM_tag_Seaport_Area),
	map<string, int>::value_type(tag_Seaport, ENUM_tag_Seaport),
	map<string, int>::value_type(tag_Outskirts_amp_Orange_County, ENUM_tag_Outskirts_amp_Orange_County),
	map<string, int>::value_type(tag_City_Outskirts, ENUM_tag_City_Outskirts),
	map<string, int>::value_type(tag_Arlington, ENUM_tag_Arlington),
	map<string, int>::value_type(tag_Hollywood, ENUM_tag_Hollywood),
	map<string, int>::value_type(tag_Greater_Hollywood, ENUM_tag_Greater_Hollywood),
	map<string, int>::value_type(tag_Manhattan, ENUM_tag_Manhattan),
	map<string, int>::value_type(tag_Manhattan_Island, ENUM_tag_Manhattan_Island),
	map<string, int>::value_type(tag_Brooklyn_ampersand_Queens, ENUM_tag_Brooklyn_ampersand_Queens),
	map<string, int>::value_type(tag_Long_Island, ENUM_tag_Long_Island),
	map<string, int>::value_type(tag_The_Bronx, ENUM_tag_The_Bronx)
};
string defaultName(const signed char shortname_, Location* thisLocation) {
	string str;
	if ((shortname_ >= 1 && thisLocation->type != thisLocation->city) || shortname_ >= 2) {
		if (thisLocation->front_business != -1)
			str = thisLocation->front_shortname;
		else
			str = thisLocation->shortname;
	}
	else {
		if (thisLocation->front_business != -1)
			str = thisLocation->front_name;
		else
			str = thisLocation->name;
	}
	return str;
}
string Location::getname(const signed char shortname_, const bool include_city)
{
	const string CONST_locationsPool051 = ", New York";
	const string CONST_locationsPool050 = ", California";
	const string CONST_locationsPool049 = ", Virginia";
	const string CONST_locationsPool048 = " Outskirts";

	const string CONST_locationsPoolB146 = ", NY";
	const string CONST_locationsPoolB145 = ", CA";
	const string CONST_locationsPoolB144 = ", VA";
	extern bool multipleCityMode;
	string str = defaultName(shortname_, this);
	if (multipleCityMode&&include_city&&type != city) {
		string cityname = listOfLocations[findlocation(city, city)]->getname(shortname_ + 2);
			switch (cityLocationTags[str]) {
			case ENUM_tag_Downtown:
				str += singleSpace + cityname;
				break;
			case ENUM_tag_University_District:
			case ENUM_tag_u_District:
			case ENUM_tag_Industrial_District:
			case ENUM_tag_i_District:
			case ENUM_tag_Shopping:
			case ENUM_tag_Outskirts:
			case ENUM_tag_Seaport_Area:
			case ENUM_tag_Seaport:
			case ENUM_tag_Outskirts_amp_Orange_County:
				str = cityname + singleSpace + str;
				break;
			case ENUM_tag_City_Outskirts:
				str = cityname + CONST_locationsPool048;
				break;
			case ENUM_tag_Arlington:
				str += (shortname_ < 0 ? CONST_locationsPool049 : CONST_locationsPoolB144);
				break;
			case ENUM_tag_Hollywood:
			case ENUM_tag_Greater_Hollywood:
				str += (shortname_ < 0 ? CONST_locationsPool050 : CONST_locationsPoolB145);
				break;
			case ENUM_tag_Manhattan:
			case ENUM_tag_Manhattan_Island:
			case ENUM_tag_Brooklyn_ampersand_Queens:
			case ENUM_tag_Long_Island:
			case ENUM_tag_The_Bronx:
				str += (shortname_ < 0 ? CONST_locationsPool051 : CONST_locationsPoolB146);
				break;
			default:
				str += commaSpace + cityname;
				break;
			}
		
	}
	return str;
}
bool Location::duplicatelocation()
{
	for (int l = 0; l < len(listOfLocations); l++)
	{
		if (listOfLocations[l] == this)
			continue;
		if (type != SITE_RESIDENTIAL_SHELTER && !strcmp(listOfLocations[l]->name, this->name))
			return true;
		if (listOfLocations[l]->front_business != -1 && this->front_business != -1 &&
			!strcmp(listOfLocations[l]->front_shortname, this->front_shortname))
			return true;
	}
	return 0;
}
void Location::update_heat_protection()
{
	extern short lawList[LAWNUM];
	int l;
	for (l = 0; l < len(listOfLocations); l++)
	{
		if (listOfLocations[l] == this)
			break;
	}
	if (l == len(listOfLocations))
	{
		heat_protection = 0;
		return;
	}
	int numpres = CreaturePool::getInstance().howManyLivingPeopleAreHere(l);
	heat_protection = 0;
	// Determine how effective your current safehouse
	// is at keeping the police confused
	switch (listOfLocations[l]->type)
	{
	case SITE_INDUSTRY_WAREHOUSE:
		if (listOfLocations[l]->front_business != -1)
			heat_protection += 12; // Business front -- high protection
		else
			heat_protection += 0; // Abandoned warehouse -- no protection
		break;
	case SITE_RESIDENTIAL_SHELTER:
		heat_protection += 0; // Homeless shelter -- no protection
		break;
	case SITE_RESIDENTIAL_TENEMENT:
		heat_protection += 4; // Lower class housing -- low protection
		break;
	case SITE_RESIDENTIAL_APARTMENT:
		heat_protection += 8; // Middle class housing -- medium protection
		break;
	case SITE_RESIDENTIAL_BOMBSHELTER:
	case SITE_OUTDOOR_BUNKER:
	case SITE_BUSINESS_BARANDGRILL:
	case SITE_RESIDENTIAL_APARTMENT_UPSCALE:
		heat_protection += 12; // Upper class housing -- high protection
		break;
	}
	if (lawList[LAW_FLAGBURNING] == -2 && listOfLocations[l]->haveflag) heat_protection += 6; // More protection if the flag is sacred
	else if (lawList[LAW_FLAGBURNING] != -2 && listOfLocations[l]->haveflag) heat_protection += 2; // Some if the flag isn't
	else if (lawList[LAW_FLAGBURNING] == -2 && !(listOfLocations[l]->haveflag)) heat_protection -= 2; // Lose some if it is and you have no flag
	else {} // None if it isn't and you have no flag
	if (heat_protection < 0) heat_protection = 0;
	heat_protection *= 5;
	if (heat_protection > 95) heat_protection = 95;
}
map<short, string> getActivityString;
std::string getactivity(ActivityST &act)
{
	const string CONST_locationsPool056 = "Reporting Bugs to the Dev Team";
	const string CONST_locationsPool055 = "Going to ";
	const string CONST_locationsPool054 = "Making ";
	const string CONST_locationsPool053 = "a bug";
	const string CONST_locationsPool052 = "Tending to ";

	extern vector<ArmorType *> armortype;
	if (getActivityString.count(act.type)) {
		return getActivityString[act.type];
	}
	else
		switch (act.type)
		{
		case ACTIVITY_HOSTAGETENDING:
		{
			std::string str = CONST_locationsPool052;
			int pl = getpoolcreature(act.arg);
			if (pl != -1) return str + CreaturePool::getInstance().getName(pl);
			else return str + CONST_locationsPool053;
		}
		case ACTIVITY_MAKE_ARMOR:
			return CONST_locationsPool054 + armortype[act.arg]->get_shortname();
		case ACTIVITY_VISIT:
			return CONST_locationsPool055 + listOfLocations[act.arg]->getname(!listOfLocations[act.arg]->is_city());
		default:
			return CONST_locationsPool056;
		}
}
/* daily - manages too hot timer and when a site map should be re-seeded and renamed */
void advancelocations()
{
	//ADVANCE LOCATIONS
	for (int l = 0; l < len(listOfLocations); l++)
	{
		if (listOfLocations[l]->closed > 0)
		{
			listOfLocations[l]->closed--;
			if (listOfLocations[l]->closed == 0)
			{  //Clean up graffiti, patch up walls, restore fire damage
				listOfLocations[l]->changes.clear();
				//If high security is supported, chance to throw guards everywhere
				if (securityable(listOfLocations[l]->type) && LCSrandom(2))
					listOfLocations[l]->highsecurity = 60;
				//Else remodel the location, invalidate maps
				else initlocation(*listOfLocations[l]);
			}
		}
		else if (listOfLocations[l]->highsecurity > 0)
		{  // Bank will remain on high security much longer
			if (listOfLocations[l]->type != SITE_BUSINESS_BANK)
				listOfLocations[l]->highsecurity--;
			else if (!LCSrandom(5))
				listOfLocations[l]->highsecurity--;
		}
	}
}
bool DeprecatedCreature::is_imprisoned() const
{
	return(alive && clinic == 0 && dating == 0 && hiding == 0 &&
		!(flag & CREATUREFLAG_SLEEPER) &&
		::listOfLocations[this->location]->part_of_justice_system());
}
bool DeprecatedCreature::is_active_liberal() const
{
	return(alive && align == ALIGN_LIBERAL && clinic == 0 && dating == 0 &&
		hiding == 0 && !(flag & CREATUREFLAG_SLEEPER) &&
		!::listOfLocations[this->location]->part_of_justice_system());
}
/* prompt user to enter an amount of items to equip, move, or sell */
long prompt_amount(long min, long max)
{
	const string CONST_locationsPool057 = "     How many?          ";
	DeprecatedCreature::printparty();
	set_color_easy(WHITE_ON_BLACK_BRIGHT);
	mvaddstrAlt(8, 15, CONST_locationsPool057);
	char str[100];
	enter_name(8, 30, str, 100, toCstring(max));
	int amount = atoi(str);
	amount = std::max((long)amount, min);
	amount = std::min((long)amount, max);
	return amount;
}
extern string spaceDashSpace;
extern string enter_done;
extern string chooseALiberalTo;

void printEquipMenu(const int loc, const int page, const string errmsg, const vector<Item *> loot) {
	const string CONST_locationsPool065 = "Z - Stash things at ";
	const string CONST_locationsPool064 = "Y - Get things from ";
	const string CONST_locationsPool063 = "Cursors - Increase or decrease ammo allocation";
	const string CONST_locationsPool062 = "S - Liberally Strip a Squad member";
	const string CONST_locationsPool061 = "Press a number to drop that Squad member's Conservative weapon";
	const string CONST_locationsPool060 = "Press a letter to equip a Liberal item";
	const string CONST_locationsPool059 = " x";
	const string CONST_locationsPool058 = "Equip the Squad";

	eraseAlt();
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(0, 0, CONST_locationsPool058);
	DeprecatedCreature::printparty();
	if (!errmsg.empty()) {
		set_color_easy(CYAN_ON_BLACK_BRIGHT);
		mvaddstrAlt(8, 20, errmsg);
		set_color_easy(WHITE_ON_BLACK);
	}
	int x = 1, y = 10;
	char str[200];
	//char str2[200];
	for (int l = page * 18; l < len(loot) && l < page * 18 + 18; l++)
	{
		string s = loot[l]->equip_title();
		if (loot[l]->get_number() > 1)
			s += CONST_locationsPool059 + tostring(loot[l]->get_number());
		str[0] = l - page * 18 + 'A';
		str[1] = '\x0';
		strcat(str, spaceDashSpace);
		strcat(str, s);
		mvaddstrAlt(y, x, str);
		x += 26;
		if (x > 53) x = 1, y++;
	}
	//PAGE UP
	if (page > 0)
	{
		mvaddstrAlt(17, 1, addprevpagestr());
	}
	//PAGE DOWN
	if ((page + 1) * 18 < len(loot))
	{
		mvaddstrAlt(17, 53, addnextpagestr());
	}
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(19, 1, CONST_locationsPool060);
	mvaddstrAlt(20, 1, CONST_locationsPool061);
	mvaddstrAlt(21, 1, CONST_locationsPool062);
	mvaddstrAlt(22, 1, CONST_locationsPool063);
	if (loc != -1)
	{
		if (len(listOfLocations[loc]->loot)) set_color_easy(WHITE_ON_BLACK);
		else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		mvaddstrAlt(23, 1, CONST_locationsPool064);
		addstrAlt(listOfLocations[loc]->getname(true));
		if (len(loot)) set_color_easy(WHITE_ON_BLACK);
		else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		mvaddstrAlt(23, 40, CONST_locationsPool065);
		addstrAlt(listOfLocations[loc]->getname(true));
	}
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(24, 1, enter_done);
}
int makeChoice(const int increaseammo, const int decreaseammo) {

	const string CONST_locationsPool069 = "receive it.";
	const string CONST_locationsPool068 = "drop a clip.";
	const string CONST_locationsPool067 = "receive a clip.";

	set_color_easy(WHITE_ON_BLACK_BRIGHT);
	moveAlt(8, 20);
	if (increaseammo)
		addstrAlt(chooseALiberalTo + CONST_locationsPool067);
	else if (decreaseammo)
		addstrAlt(chooseALiberalTo + CONST_locationsPool068);
	else addstrAlt(chooseALiberalTo + CONST_locationsPool069);
	return getkeyAlt();

}

// TODO Relocate these to a new file
// to allow removal of creature.h
const string NOT_ERROR_BUT_CONTINUE = "For simplicity this function returns an error message, prompting 'continue;', this is not an error, but it needs to use 'continue;' anyway";
<<<<<<< HEAD
string DeprecatedCreature::transferClipBaseSquad(const bool decreaseammo, const bool increaseammo, int& slot, int& page, const int e, vector<Item *> &loot) {
=======
string transferClipBaseSquad(const bool decreaseammo, const bool increaseammo, int& slot, int& page, const int e, vector<Item *> &loot) {
>>>>>>> acaa55987f7b177b662bdb7f42ebb6850475784a
	extern Deprecatedsquadst *activesquad;

	const string CONST_locationsPool076 = "Can't carry any more ammo.";
	const string CONST_locationsPool075 = "That ammo doesn't fit.";
	const string CONST_locationsPool074 = "Can't carry ammo without a gun.";
	const string CONST_locationsPool073 = "No ammo available!";
	const string CONST_locationsPool072 = "No ammo required!";
	const string CONST_locationsPool071 = "No spare clips!";
	const string CONST_locationsPool070 = "No ammo to drop!";

	DeprecatedCreature *squaddie = activesquad->squad[e - '1'];
	if (squaddie)
	{
		if (decreaseammo)
		{
			if (squaddie->has_clips())
			{
				loot.push_back(squaddie->take_one_clip());
				consolidateloot(loot);
				return NOT_ERROR_BUT_CONTINUE;
			}
			else if (!squaddie->get_weapon().uses_ammo())
			{
				return CONST_locationsPool070;
			}
			else
			{
				return CONST_locationsPool071;
			}
		}
		if (increaseammo)
		{
			if (!squaddie->get_weapon().uses_ammo())
			{
				return CONST_locationsPool072;
			}
			slot = -1;
			for (int sl = 0; sl < len(loot); sl++)
			{
				if (loot[sl]->whatIsThis() == THIS_IS_CLIP && squaddie->get_weapon().acceptable_ammo(*loot[sl]))
				{
					slot = sl;
					break;
				}
				else if (loot[sl]->whatIsThis() == THIS_IS_WEAPON && loot[sl]->is_same_type(squaddie->get_weapon())) //For throwing weapons. -XML
				{
					Weapon* w = static_cast<Weapon*>(loot[sl]); //cast -XML
					if (w->is_throwable())
					{
						slot = sl;
						break;
					}
				}
			}
			if (slot == -1)
			{
				return CONST_locationsPool073;
			}
		}
		int armok = 2;
		if ((squaddie->wound[BODYPART_ARM_RIGHT] & WOUND_NASTYOFF) ||
			(squaddie->wound[BODYPART_ARM_RIGHT] & WOUND_CLEANOFF)) armok--;
		if ((squaddie->wound[BODYPART_ARM_LEFT] & WOUND_NASTYOFF) ||
			(squaddie->wound[BODYPART_ARM_LEFT] & WOUND_CLEANOFF)) armok--;
		if (squaddie->special[SPECIALWOUND_NECK] != 1) armok = 0;
		if (squaddie->special[SPECIALWOUND_UPPERSPINE] != 1) armok = 0;
		if (loot[slot]->whatIsThis() == THIS_IS_WEAPON && armok)
		{
			Weapon* w = static_cast<Weapon*>(loot[slot]); //cast -XML
			squaddie->give_weapon(*w, &loot);
			if (loot[slot]->empty()) delete_and_remove(loot, slot);
			if (page * 18 >= len(loot) && page != 0) page--;
		}
		else if (loot[slot]->whatIsThis() == THIS_IS_ARMOR)
		{
			Armor* a = static_cast<Armor*>(loot[slot]); //cast -XML
			squaddie->give_armor(*a, &loot);
			if (loot[slot]->empty()) delete_and_remove(loot, slot);
			if (page * 18 >= len(loot) && page != 0) page--;
		}
		else if (loot[slot]->whatIsThis() == THIS_IS_CLIP && armok)
		{
			int space = 9 - squaddie->count_clips();
			if (!squaddie->get_weapon().uses_ammo())
			{
				return CONST_locationsPool074;
			}
			else if (!squaddie->get_weapon().acceptable_ammo(*loot[slot]))
			{
				return CONST_locationsPool075;
			}
			else if (space < 1)
			{
				return CONST_locationsPool076;
			}
			else
			{
				int amount = 1;
				if (loot[slot]->get_number() > 1 && !increaseammo)
					amount = prompt_amount(0, min((int)loot[slot]->get_number(), space));
				squaddie->take_clips(*loot[slot], amount);
				if (loot[slot]->empty()) delete_and_remove(loot, slot);
				if (page * 18 >= len(loot) && page != 0) page--;
			}
		}
		consolidateloot(loot);
	}
	return blankString;
}
bool isThereNoActivesquad() {
	extern Deprecatedsquadst *activesquad;
	return activesquad == NULL;
}
// This function is used only once, but it allows another function not to import *activesquad (though, like much of the code, its purpose is unclear)
void clearActiveSquadForceInc() {
	extern Deprecatedsquadst *activesquad;
	for (int p = 0; p < 6; p++)
		if (activesquad->squad[p] != NULL)
			activesquad->squad[p]->forceinc = 0;
}
void DeprecatedCreature::disarmSquadmember(vector<Item *> &loot, const int p) {
	extern Deprecatedsquadst *activesquad;

	if (activesquad->squad[p] != NULL)
	{
		activesquad->squad[p]->drop_weapons_and_clips(&loot);
		consolidateloot(loot);
	}
}

bool notDoesActiveSquadHaveExactlyOneMember() {
	bool choice = true;
	extern Deprecatedsquadst *activesquad;
	if (activesquad->squad[0])
	{
		choice = false;
		for (int i = 1; i < 6; i++)
			if (activesquad->squad[i])
			{
				choice = true; break;
			}
	}
	return choice;
}
void DeprecatedCreature::completelyStripSquadMember(vector<Item *> &loot, int d) {
	extern Deprecatedsquadst *activesquad;
	if (activesquad->squad[d])
	{
		activesquad->squad[d]->strip(&loot);
		consolidateloot(loot);
	}
}
/* review squad equipment */
void DeprecatedCreature::equip(vector<Item *> &loot, int loc)
{
	extern vector<Location *> listOfLocations;
	const string CONST_locationsPool077 = "strip down.";

	const string CONST_locationsPool066 = "You can't equip that.";

	if (isThereNoActivesquad()) return;
	consolidateloot(loot);
	if (loc != -1) consolidateloot(listOfLocations[loc]->loot);
	int page = 0;
	string errmsg = blankString;
	while (true)
	{
		printEquipMenu(loc, page, errmsg, loot);
		errmsg = blankString;

		int c = getkeyAlt();
		bool increaseammo = (c == KEY_UP);
		bool decreaseammo = (c == KEY_DOWN);
		if ((c >= 'a'&&c <= 'r') || increaseammo || decreaseammo)
		{
			int slot = c - 'a' + page * 18;
			if (increaseammo || decreaseammo) slot = -999;
			else
			{
				if (slot < 0 || slot >= len(loot)) continue; // Out of range.
				else if (!(loot[slot]->whatIsThis() == THIS_IS_WEAPON)
					&& !(loot[slot]->whatIsThis() == THIS_IS_ARMOR)
					&& !(loot[slot]->whatIsThis() == THIS_IS_CLIP))
				{
					errmsg = CONST_locationsPool066;
					continue;
				}
			}
			bool choice = notDoesActiveSquadHaveExactlyOneMember();
			int e = '1';
			if (choice)
			{
				e = makeChoice(increaseammo, decreaseammo);
			}
			if (e >= '1'&&e <= '6')
			{
				errmsg = transferClipBaseSquad(decreaseammo, increaseammo, slot, page, e, loot);
				if (!errmsg.empty()) {
					if (errmsg == NOT_ERROR_BUT_CONTINUE) {
						errmsg = blankString;
					}
					continue;
				}
			}
		}
		else
			if (c == 's')
			{
				bool choice = notDoesActiveSquadHaveExactlyOneMember();

				int d = '1';
				if (choice)
				{
					set_color_easy(WHITE_ON_BLACK_BRIGHT);
					mvaddstrAlt(8, 20, chooseALiberalTo + CONST_locationsPool077);
					d = getkeyAlt();
				}
				if (d >= '1'&& d <= '6')
				{
					completelyStripSquadMember(loot, d - '1');
				}

			}
			else
				if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) { break; }
		if (loc != -1)
		{
			if (c == 'y'&&len(listOfLocations[loc]->loot)) moveloot(loot, listOfLocations[loc]->loot);
			if (c == 'z'&&len(loot)) moveloot(listOfLocations[loc]->loot, loot);
		}
		if (c >= '1'&&c <= '6')
		{
			int p = c - '1';
			disarmSquadmember(loot, p);
		}
		//PAGE UP
		if (is_page_up(c) && page > 0) page--;
		//PAGE DOWN
		if (is_page_down(c) && (page + 1) * 18 < len(loot)) page++;
	}
}
/* lets you pick stuff to stash/retrieve from one location to another */
void moveloot(vector<Item *> &dest, vector<Item *> &source)
{
	const string CONST_locationsPool081 = "Press a letter to select an item.";
	const string CONST_locationsPool080 = "x";
	const string CONST_locationsPool079 = "/";
	const string CONST_locationsPool078 = "Select Objects";
	int page = 0;
	vector<int> selected(len(source), 0);
	while (true)
	{
		eraseAlt();
		set_color_easy(WHITE_ON_BLACK);
		mvaddstrAlt(0, 0, CONST_locationsPool078);
		DeprecatedCreature::printparty();
		int x = 1, y = 10;
		char str[200];
		for (int l = page * 18; l < len(source) && l < page * 18 + 18; l++)
		{
			if (selected[l]) set_color_easy(GREEN_ON_BLACK_BRIGHT);
			else set_color_easy(WHITE_ON_BLACK);
			string s = source[l]->equip_title();
			if (source[l]->get_number() > 1)
			{
				s += singleSpace;
				if (selected[l] > 0) s += tostring(selected[l]) + CONST_locationsPool079;
				else s += CONST_locationsPool080;
				s += tostring(source[l]->get_number());
			}
			str[0] = l - page * 18 + 'A';
			str[1] = '\x0';
			strcat(str, spaceDashSpace);
			strcat(str, s);
			mvaddstrAlt(y, x, str);
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
		if ((page + 1) * 18 < len(source))
		{
			mvaddstrAlt(17, 53, addnextpagestr());
		}
		set_color_easy(WHITE_ON_BLACK);
		mvaddstrAlt(23, 1, CONST_locationsPool081);
		mvaddstrAlt(24, 1, enter_done);
		int c = getkeyAlt();
		if (c >= 'a'&&c <= 'r')
		{
			int slot = c - 'a' + page * 18;
			if (slot >= 0 && slot < len(source))
			{
				if (selected[slot]) selected[slot] = 0;
				else if (source[slot]->get_number() > 1)
					selected[slot] = prompt_amount(0, source[slot]->get_number());
				else selected[slot] = 1;
			}
		}
		if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
		//PAGE UP
		if (is_page_up(c) && page > 0) page--;
		//PAGE DOWN
		if (is_page_down(c) && (page + 1) * 18 < len(source)) page++;
	}
	for (int l = len(source) - 1; l >= 0; l--) if (selected[l] > 0)
	{
		if (source[l]->get_number() <= selected[l])
		{
			dest.push_back(source[l]);
			source.erase(source.begin() + l);
		}
		else
		{
			Item* newit = source[l]->split(selected[l]);
			dest.push_back(newit);
		}
	}
	// Avoid stuff jumping around the next time you equip.
	consolidateloot(dest);
}
/* equipment - assign new bases to the equipment */
void equipmentbaseassign()
{
	const string CONST_locationsPool089 = ",. to view other base pages.";
	const string CONST_locationsPool088 = "  Shift and a Number will move ALL items!";
	const string CONST_locationsPool087 = "T to sort by type.";
	const string CONST_locationsPool086 = "T to sort by location.";
	const string CONST_locationsPool085 = "Press a Letter to assign a base.  Press a Number to select a base.";
	const string CONST_locationsPool084 = "NEW LOCATION";
	const string CONST_locationsPool083 = "----ITEM----------------CURRENT LOCATION---------------------------------------";
	const string CONST_locationsPool082 = "Moving Equipment";
	int page_loot = 0, page_loc = 0, selectedbase = 0;
	bool sortbytype = false;
	vector<Item *> temploot;
	map<Item *, Location *> temploot2;
	for (int l = 0; l < len(listOfLocations); l++) for (int l2 = 0; l2 < len(listOfLocations[l]->loot); l2++)
		if (!listOfLocations[l]->siege.siege)
		{
			temploot.push_back(listOfLocations[l]->loot[l2]);
			temploot2[listOfLocations[l]->loot[l2]] = listOfLocations[l];
		}
	if (!len(temploot)) return;
	vector<int> temploc;
	for (int l = 0; l < len(listOfLocations); l++) if (listOfLocations[l]->renting >= 0 && !listOfLocations[l]->siege.siege)
		temploc.push_back(l);
	if (!len(temploc)) return;
	while (true)
	{
		eraseAlt();
		set_color_easy(WHITE_ON_BLACK);
		printfunds();
		mvaddstrAlt(0, 0, CONST_locationsPool082);
		mvaddstrAlt(1, 0, CONST_locationsPool083);
		mvaddstrAlt(1, 51, CONST_locationsPool084);
		for (int p = page_loot * 19, y = 2; p < len(temploot) && p < page_loot * 19 + 19; p++, y++)
		{
			set_color_easy(WHITE_ON_BLACK);
			moveAlt(y, 0);
			addcharAlt(y + 'A' - 2); addstrAlt(spaceDashSpace);
			addstrAlt(temploot[p]->equip_title());
			mvaddstrAlt(y, 25, temploot2[temploot[p]]->getname(true, true));
		}
		for (int p = page_loc * 9, y = 2; p < len(temploc) && p < page_loc * 9 + 9; p++, y++)
		{
			if (p == selectedbase)set_color_easy(WHITE_ON_BLACK_BRIGHT);
			else set_color_easy(WHITE_ON_BLACK);
			moveAlt(y, 51);
			addcharAlt(y + '1' - 2); addstrAlt(spaceDashSpace);
			addstrAlt(listOfLocations[temploc[p]]->getname(true, true));
		}
		set_color_easy(WHITE_ON_BLACK);
		mvaddstrAlt(22, 0, CONST_locationsPool085);
		moveAlt(23, 0);
		if (sortbytype) addstrAlt(CONST_locationsPool086);
		else addstrAlt(CONST_locationsPool087);
		addstrAlt(CONST_locationsPool088);
		moveAlt(24, 0); // location for either viewing other base pages or loot pages
		if (len(temploc) > 9)
		{
			addstrAlt(CONST_locationsPool089);
			moveAlt(24, 34); // we have base pages, so different location for viewing other loot pages
		}
		if (len(temploot) > 19)
			addstrAlt(addpagestr());
		int c = getkeyAlt();
		//PAGE UP (items)
		if (is_page_up(c) && page_loot > 0) page_loot--;
		//PAGE DOWN (items)
		if (is_page_down(c) && (page_loot + 1) * 19 < len(temploot)) page_loot++;
		//PAGE UP (locations)
		if (c == ','&&page_loc > 0) page_loc--;
		//PAGE DOWN (locations)
		if (c == '.' && (page_loc + 1) * 9 < len(temploc)) page_loc++;
		//Toggle sorting method
		if (c == 't')
		{
			sortbytype = !sortbytype;
			if (sortbytype) sort(temploot.begin(), temploot.end(), Item::sort_compare);
			else
			{  //Sort by location
				temploot.clear();
				for (int l = 0; l < len(listOfLocations); l++) for (int l2 = 0; l2 < len(listOfLocations[l]->loot); l2++)
					if (!listOfLocations[l]->siege.siege) temploot.push_back(listOfLocations[l]->loot[l2]);
			}
		}
		if (c >= 'a'&&c <= 's')
		{
			int p = page_loot * 19 + c - 'a';
			if (p < len(temploot))
				// Search through the old base's stuff for this item
				for (int l2 = 0; l2 < len(temploot2[temploot[p]]->loot); l2++)
					// Remove it from that inventory and move it to the new one
					if (temploot2[temploot[p]]->loot[l2] == temploot[p])
					{
						temploot2[temploot[p]]->loot.erase(temploot2[temploot[p]]->loot.begin() + l2);
						listOfLocations[temploc[selectedbase]]->loot.push_back(temploot[p]);
						temploot2[temploot[p]] = listOfLocations[temploc[selectedbase]];
					}
		}
		if (c >= '1'&&c <= '9')
		{
			int p = page_loc * 9 + c - '1';
			if (p < len(temploc)) selectedbase = p;
		}
		// Check if the player wants to move all items to a new location,
		// using Shift + a number key.
		const char upnums[] = "!@#$%^&*(";
		for (int upnumi = 0; upnumi < len(upnums); upnumi++)
		{
			if (c == upnums[upnumi])
			{
				// Set base location
				int basechoice = page_loc * 9 + upnumi;
				if (basechoice < len(temploc))
				{
					selectedbase = basechoice;
					// Search through the old base's stuff for this item
					for (int p = 0; p < len(temploot); p++)
						// Search through the old base's stuff for this item
						for (int l2 = 0; l2 < len(temploot2[temploot[p]]->loot); l2++)
							// Remove it from that inventory and move it to the new one
							if (temploot2[temploot[p]]->loot[l2] == temploot[p])
							{
								temploot2[temploot[p]]->loot.erase(temploot2[temploot[p]]->loot.begin() + l2);
								listOfLocations[temploc[selectedbase]]->loot.push_back(temploot[p]);
								temploot2[temploot[p]] = listOfLocations[temploc[selectedbase]];
							}
				}
			}
		}
		if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
	}
}
/* combines multiple items of the same type into stacks */
void consolidateloot(vector<Item *> &loot)
{
	//PUT THINGS TOGETHER
	for (int l = len(loot) - 1; l >= 1; l--) for (int l2 = l - 1; l2 >= 0; l2--)
	{
		loot[l2]->merge(*loot[l]);
		if (loot[l]->empty())
		{
			delete_and_remove(loot, l); break;
		}
	}
	sort(loot.begin(), loot.end(), Item::sort_compare);
}
/* check if the squad has a certain weapon */
char DeprecatedCreature::squadhasitem(Deprecatedsquadst &sq, const string& type)
{
	if (getweapontype(type) == -1) return 0;
	for (int p = 0; p < 6; p++) if (sq.squad[p])
		if (sq.squad[p]->get_weapon().get_itemtypename() == type) return 1;
	for (int l = 0; l < len(sq.loot); l++)
	{
		if (sq.loot[l]->get_itemtypename() != type) continue;
		if (sq.loot[l]->whatIsThis() == THIS_IS_WEAPON && sq.loot[l]->get_itemtypename() == type) return 1;
	}
	return 0;
}
#include "../daily/siege.h"
/* location info at top of screen */
const string CONST_locationsPool116 = " Eating";
const string CONST_locationsPool115 = "s";
const string CONST_locationsPool114 = " Daily Ration";
const string CONST_locationsPool113 = "Not Enough Food";
const string CONST_locationsPool112 = " of Food Left";
const string CONST_locationsPool111 = " Day";
const string CONST_locationsPool110 = "GENERATOR";
const string CONST_locationsPool109 = "LIGHTS OUT";
const string CONST_locationsPool108 = "TANK TRAPS";
const string CONST_locationsPool107 = "AA GUN";
const string CONST_locationsPool106 = "BOOBY TRAPS";
const string CONST_locationsPool105 = "CAMERAS ON";
const string CONST_locationsPool104 = "CAMERAS OFF";
const string CONST_locationsPool103 = "BUSINESS FRONT";
const string CONST_locationsPool102 = "PRINTING PRESS";
const string CONST_locationsPool101 = "FORTIFIED COMPOUND";
const string CONST_locationsPool100 = "This location has insufficient food stores.";
const string CONST_locationsPool099 = "This location has food for only a few days.";
const string CONST_locationsPool098 = "You are not under siege...  yet.";
const string CONST_locationsPool097 = "Firemen are raiding this location!";
const string CONST_locationsPool096 = "The CCS is raiding this location!";
const string CONST_locationsPool095 = "The Corporations are raiding this location!";
const string CONST_locationsPool094 = "The masses are storming this location!";
const string CONST_locationsPool093 = "The CIA is raiding this location!";
const string CONST_locationsPool092 = "The police are raiding this location!";
const string CONST_locationsPool091 = "The police have surrounded this location.";
void printlocation(long loc)
{

	if (listOfLocations[loc]->siege.siege)
	{
		if (!listOfLocations[loc]->siege.underattack)
		{
			set_color_easy(YELLOW_ON_BLACK_BRIGHT);
			mvaddstrAlt(2, 1, CONST_locationsPool091);
		}
		else
		{
			set_color_easy(RED_ON_BLACK_BRIGHT);
			switch (listOfLocations[loc]->siege.siegetype)
			{
			case SIEGE_POLICE:
				mvaddstrAlt(2, 1, CONST_locationsPool092); break;
			case SIEGE_CIA:
				mvaddstrAlt(2, 1, CONST_locationsPool093); break;
			case SIEGE_HICKS:
				mvaddstrAlt(2, 1, CONST_locationsPool094); break;
			case SIEGE_CORPORATE:
				mvaddstrAlt(2, 1, CONST_locationsPool095); break;
			case SIEGE_CCS:
				mvaddstrAlt(2, 1, CONST_locationsPool096); break;
			case SIEGE_FIREMEN:
				mvaddstrAlt(2, 1, CONST_locationsPool097); break;
			}
		}
	}
	else
	{
		set_color_easy(WHITE_ON_BLACK);
		mvaddstrAlt(2, 1, CONST_locationsPool098);
	}
	if (listOfLocations[loc]->can_be_upgraded())
	{
		if (numbereating(loc) > 0)
		{
			if (fooddaysleft(loc))
			{
				if (fooddaysleft(loc) < 4)
				{
					if (!listOfLocations[loc]->siege.siege)set_color_easy(WHITE_ON_BLACK);
					else set_color_easy(YELLOW_ON_BLACK_BRIGHT);
					mvaddstrAlt(3, 1, CONST_locationsPool099);
				}
			}
			else
			{
				if (!listOfLocations[loc]->siege.siege)set_color_easy(WHITE_ON_BLACK);
				else set_color_easy(RED_ON_BLACK_BRIGHT);
				mvaddstrAlt(3, 1, CONST_locationsPool100);
			}
		}
		if (listOfLocations[loc]->compound_walls & COMPOUND_BASIC)
		{
			set_color_easy(WHITE_ON_BLACK_BRIGHT);
			mvaddstrAlt(4, 1, CONST_locationsPool101);
		}
		if (listOfLocations[loc]->compound_walls & COMPOUND_PRINTINGPRESS)
		{
			set_color_easy(BLUE_ON_BLACK_BRIGHT);
			mvaddstrAlt(4, 31, CONST_locationsPool102);
		}
		if (listOfLocations[loc]->front_business != -1)
		{
			set_color_easy(MAGENTA_ON_BLACK_BRIGHT);
			mvaddstrAlt(4, 54, CONST_locationsPool103);
		}
		if (listOfLocations[loc]->compound_walls & COMPOUND_CAMERAS)
		{
			if (listOfLocations[loc]->siege.siege&&listOfLocations[loc]->siege.cameras_off)
			{
				set_color_easy(RED_ON_BLACK);
				mvaddstrAlt(5, 1, CONST_locationsPool104);
			}
			else
			{
				set_color_easy(GREEN_ON_BLACK_BRIGHT);
				mvaddstrAlt(5, 1, CONST_locationsPool105);
			}
		}
		if (listOfLocations[loc]->compound_walls & COMPOUND_TRAPS)
		{
			set_color_easy(RED_ON_BLACK_BRIGHT);
			mvaddstrAlt(5, 16, CONST_locationsPool106);
		}
		if (listOfLocations[loc]->compound_walls & COMPOUND_AAGUN)
		{
			set_color_easy(CYAN_ON_BLACK_BRIGHT);
			mvaddstrAlt(5, 33, CONST_locationsPool107);
		}
		if (listOfLocations[loc]->compound_walls & COMPOUND_TANKTRAPS)
		{
			set_color_easy(YELLOW_ON_BLACK_BRIGHT);
			mvaddstrAlt(5, 46, CONST_locationsPool108);
		}
		if (listOfLocations[loc]->siege.siege&&listOfLocations[loc]->siege.lights_off)
		{
			set_color_easy(WHITE_ON_BLACK);
			mvaddstrAlt(5, 60, CONST_locationsPool109);
		}
		else if (listOfLocations[loc]->compound_walls & COMPOUND_GENERATOR)
		{
			set_color_easy(WHITE_ON_BLACK_BRIGHT);
			mvaddstrAlt(5, 61, CONST_locationsPool110);
		}
		int eaters = numbereating(loc), days = fooddaysleft(loc);
		if (eaters > 0)
		{
			if (days >= 1)
			{
				set_color_easy(WHITE_ON_BLACK);
				mvaddstrAlt(6, 50, days);
				addstrAlt(CONST_locationsPool111);
				if (days != 1)addcharAlt('s');
				addstrAlt(CONST_locationsPool112);
			}
			else if (days == 0)
			{
				set_color_easy(RED_ON_BLACK);
				mvaddstrAlt(6, 50, CONST_locationsPool113);
			}
		}
		set_color_easy(WHITE_ON_BLACK);
		mvaddstrAlt(6, 1, listOfLocations[loc]->compound_stores);
		addstrAlt(CONST_locationsPool114);
		if (listOfLocations[loc]->compound_stores != 1)addstrAlt(CONST_locationsPool115);
		set_color_easy(WHITE_ON_BLACK);
		mvaddstrAlt(6, 30, eaters);
		addstrAlt(CONST_locationsPool116);
	}
}
void addStringYear();
/* location and squad header */
void printActivesquad() {
	const string CONST_locationsPool121 = "Acting Individually";
	extern Deprecatedsquadst *activesquad;
	string str = getactivity(activesquad->activity);
	set_activity_color(activesquad->activity.type);
	if (activesquad->activity.type == ACTIVITY_NONE)
	{
		bool haveact = false, multipleact = false;
		for (int p = 0; p < 6; p++)
		{
			if (activesquad->squad[p] == NULL) continue;
			const string str2 = getactivity(activesquad->squad[p]->activity);
			set_activity_color(activesquad->squad[p]->activity.type);
			if (haveact&&str != str2) multipleact = true;
			str = str2, haveact = true;
		}
		if (multipleact)
		{
			str = CONST_locationsPool121;
			set_color_easy(WHITE_ON_BLACK_BRIGHT);
		}
	}
	mvaddstrAlt(0, 41, str);
}
/* location and squad header */
void locheader()
{
	const string CONST_locationsPool120 = "2) Press Z to Assemble a New Squad";
	const string CONST_locationsPool119 = "1) R - Review Assets and Form Squads";
	const string CONST_locationsPool118 = "To form a new squad:";
	const string CONST_locationsPool117 = "No Squad Selected";

	extern Deprecatedsquadst *activesquad;
	extern int selectedsiege;
	extern int day;
	extern int month;
	extern int year;
	if (activesquad != NULL && activesquad->squad[0]->location != -1)
	{
		if (listOfLocations[activesquad->squad[0]->location]->siege.siege)
		{
			if (listOfLocations[activesquad->squad[0]->location]->siege.underattack)set_color_easy(RED_ON_BLACK_BRIGHT);
			else set_color_easy(YELLOW_ON_BLACK_BRIGHT);
		}
		else set_color_easy(WHITE_ON_BLACK);
	}
	else if (selectedsiege != -1)
	{
		if (listOfLocations[selectedsiege]->siege.siege)
		{
			if (listOfLocations[selectedsiege]->siege.underattack)set_color_easy(RED_ON_BLACK_BRIGHT);
			else set_color_easy(YELLOW_ON_BLACK_BRIGHT);
		}
		else set_color_easy(WHITE_ON_BLACK);
	}
	else set_color_easy(WHITE_ON_BLACK);
	moveAlt(0, 0);
	if (activesquad != NULL && activesquad->squad[0]->location != -1)
	{
		addstrAlt(listOfLocations[activesquad->squad[0]->location]->getname(false, true));
		addstrAlt(commaSpace);
	}
	else
	{
		if (selectedsiege == -1)
		{
			addstrAlt(CONST_locationsPool117);
			addstrAlt(commaSpace);
		}
		else
		{
			addstrAlt(listOfLocations[selectedsiege]->getname(false, true));
			addstrAlt(commaSpace);
		}
	}
	addstrAlt(getmonth(month, true));
	addstrAlt(singleSpace);
	addstrAlt(day);
	addstrAlt(commaSpace);
	addStringYear();
	if (activesquad == NULL && selectedsiege == -1)
	{
		set_color_easy(BLACK_ON_BLACK_BRIGHT);
		mvaddstrAlt(3, 6, CONST_locationsPool118);
		mvaddstrAlt(4, 6, CONST_locationsPool119);
		mvaddstrAlt(5, 6, CONST_locationsPool120);
		set_color_easy(WHITE_ON_BLACK);
	}
	printfunds();
	if (activesquad != NULL)
	{
		printActivesquad();
	}
}
#include "../common/commonactions.h"
#include "../common/musicClass.h"
string closeParenthesis;
string spaceParanthesisDollar;
string needCar;
string travelDifCity;
string secrecyLevel;
string percentSign;
string heatLevel;
string underSiege;
string currentLocation;
string highSecurity;
string closedDown;
string enemySafeHouse;
string safeHouse;
bool showSafehouseInfo(Location* this_location, Location* squad_location, const bool havecar, const int ticketprice) {
	extern class Ledger ledger;
	extern bool multipleCityMode;

	bool show_safehouse_info = false;
	if (this_location == squad_location) {
		set_color_easy(WHITE_ON_BLACK_BRIGHT);
		addstrAlt(currentLocation);
		show_safehouse_info = true;
	}
	else if (this_location->renting >= 0) {
		set_color_easy(GREEN_ON_BLACK_BRIGHT);
		addstrAlt(safeHouse);
		show_safehouse_info = true;
	}
	else if (this_location->renting == RENTING_CCS) {
		set_color_easy(RED_ON_BLACK_BRIGHT);
		addstrAlt(enemySafeHouse);
	}
	else if (this_location->closed) {
		set_color_easy(RED_ON_BLACK_BRIGHT);
		addstrAlt(closedDown);
	}
	else if (this_location->highsecurity) {
		set_color_easy(MAGENTA_ON_BLACK_BRIGHT);
		addstrAlt(highSecurity);
	}
	else if (multipleCityMode && this_location->type == squad_location->city) {
		set_color_easy(WHITE_ON_BLACK_BRIGHT);
		addstrAlt(currentLocation);
	}
	else if (this_location->area != squad_location->area && !havecar) {
		set_color_easy(YELLOW_ON_BLACK_BRIGHT);
		addstrAlt(needCar);
	}
	else if (this_location->type == SITE_TRAVEL) {
		if (ledger.get_funds() < ticketprice)
			set_color_easy(RED_ON_BLACK_BRIGHT);
		else
			set_color_easy(GREEN_ON_BLACK_BRIGHT);
		addstrAlt(spaceParanthesisDollar + tostring(ticketprice) + closeParenthesis);
	}
	return show_safehouse_info;
}
/* base - go forth to stop evil */
const string CONST_locationsPool124 = "Enter - Back one step.";
const string CONST_locationsPool123 = "Enter - The squad is not yet Liberal enough.";
const string CONST_locationsPool122 = "Where will the Squad go?";
void stopevil()
{
	extern class Ledger ledger;
	extern Deprecatedsquadst *activesquad;
	extern bool multipleCityMode;
	extern MusicClass music;
	//int l = 0, p = 0;
	if (!activesquad) return;
	bool havecar = false;
	for (int p = 0; p < 6; p++) if (activesquad->squad[p]) if (activesquad->squad[p]->pref_carid != -1)
	{
		havecar = true;
		break;
	}
	Location* squad_location = listOfLocations[activesquad->squad[0]->location];
	int page = 0, loc = -1;
	// Start at the city level, rather than the absolute top
	if (multipleCityMode) {
		for (int l = 0; l < len(listOfLocations); l++) {
			if (listOfLocations[l]->type == squad_location->city) {
				loc = l;
				break;
			}
		}
	}
	vector<long> temploc;
	// 1. LCS safe houses
	for (int l = 0; l < len(listOfLocations); l++)
		if (listOfLocations[l]->parent == loc && listOfLocations[l]->renting >= 0 && !listOfLocations[l]->hidden)
			temploc.push_back(l);
	// 2. CCS safe houses
	for (int l = 0; l < len(listOfLocations); l++)
		if (listOfLocations[l]->parent == loc && listOfLocations[l]->renting == RENTING_CCS && !listOfLocations[l]->hidden)
			temploc.push_back(l);
	// 3. Other sites
	for (int l = 0; l < len(listOfLocations); l++)
		if (listOfLocations[l]->parent == loc && listOfLocations[l]->renting == RENTING_NOCONTROL && !listOfLocations[l]->hidden)
			temploc.push_back(l);
	// Determine cost of tickets for travel
	int ticketprice = 100 * activesquadSize();
	while (true)
	{
		music.play(MUSIC_STOPEVIL);
		eraseAlt();
		set_color_easy(WHITE_ON_BLACK);
		mvaddstrAlt(0, 0, CONST_locationsPool122);
		DeprecatedCreature::printparty();
		if (loc != -1)
		{
			set_color_easy(WHITE_ON_BLACK);
			mvaddstrAlt(8, 0, listOfLocations[loc]->getname(-1, true));
		}
		temploc.clear();
		for (int l = 0; l < len(listOfLocations); l++)
			if (listOfLocations[l]->parent == loc && listOfLocations[l]->renting >= 0 && !listOfLocations[l]->hidden)temploc.push_back(l);
		for (int l = 0; l < len(listOfLocations); l++)
			if (listOfLocations[l]->parent == loc && listOfLocations[l]->renting == RENTING_CCS && !listOfLocations[l]->hidden)temploc.push_back(l);
		for (int l = 0; l < len(listOfLocations); l++)
			if (listOfLocations[l]->parent == loc && listOfLocations[l]->renting == RENTING_NOCONTROL && !listOfLocations[l]->hidden)temploc.push_back(l);
		int y = 10;
		for (int p = page * 11; p != -1 && p < len(temploc) && p < page * 11 + 11; p++)
		{
			Location* this_location = listOfLocations[temploc[p]];
			set_color_easy(WHITE_ON_BLACK);
			mvaddcharAlt(y, 0, y - 10 + 'A');
			addstrAlt(spaceDashSpace);
			addstrAlt(listOfLocations[temploc[p]]->getname());

			bool show_safehouse_info = showSafehouseInfo(this_location, squad_location, havecar, ticketprice);

			if (this_location->siege.siege > 0) {
				set_color_easy(RED_ON_BLACK);
				addstrAlt(underSiege);
			}
			if (show_safehouse_info)
			{
				this_location->update_heat_protection();
				set_color_easy(WHITE_ON_BLACK);
				mvaddstrAlt(y, 54, heatLevel);
				if (this_location->heat > this_location->heat_protection)
					set_color_easy(YELLOW_ON_BLACK_BRIGHT);
				else set_color_easy(BLACK_ON_BLACK_BRIGHT);
				addstrAlt(this_location->heat);
				addstrAlt(percentSign);
				set_color_easy(WHITE_ON_BLACK);
				mvaddstrAlt(y, 66, secrecyLevel);
				if (this_location->heat > this_location->heat_protection)
					set_color_easy(YELLOW_ON_BLACK_BRIGHT);
				else set_color_easy(BLACK_ON_BLACK_BRIGHT);
				addstrAlt(this_location->heat_protection);
				addstrAlt(percentSign);
			}
			if (multipleCityMode && this_location->city == this_location->type)
			{
				set_color_easy(BLACK_ON_BLACK_BRIGHT);
				mvaddstrAlt(y, 50, this_location->city_description());
			}
			y++;
		}
		if (multipleCityMode && loc != -1 && LocationsPool::getInstance().getLocationType(loc) == listOfLocations[loc]->city)
		{
			set_color_easy(WHITE_ON_BLACK);
			mvaddcharAlt(y + 1, 0, y - 10 + 'A');
			addstrAlt(travelDifCity);
			if (!havecar) {
				set_color_easy(YELLOW_ON_BLACK_BRIGHT);
				addstrAlt(needCar);
			}
			else {
				if (ledger.get_funds() < ticketprice)
					set_color_easy(RED_ON_BLACK_BRIGHT);
				else set_color_easy(GREEN_ON_BLACK_BRIGHT);
				addstrAlt(spaceParanthesisDollar + tostring(ticketprice) + closeParenthesis);
			}
			temploc.push_back(-1);
		}
		set_color_easy(WHITE_ON_BLACK);
		//PAGE UP
		if (page > 0)
		{
			mvaddstrAlt(10, 60, addprevpagestr());
		}
		//PAGE DOWN
		if ((page + 1) * 11 < len(temploc))
		{
			mvaddstrAlt(20, 60, addnextpagestr());
		}
		set_color_easy(WHITE_ON_BLACK);
		if ((loc == -1) || (multipleCityMode && LocationsPool::getInstance().getLocationType(loc) == squad_location->city)) mvaddstrAlt(24, 1, CONST_locationsPool123);
		else mvaddstrAlt(24, 1, CONST_locationsPool124);
		int c = getkeyAlt();
		//PAGE UP
		if (is_page_up(c) && page > 0) page--;
		//PAGE DOWN
		if (is_page_down(c) && (page + 1) * 11 < len(temploc)) page++;
		if (c >= 'a'&&c <= 'k')
		{
			int sq = page * 11 + c - 'a';
			if (sq < len(temploc) && sq >= 0)
			{
				int oldloc = loc;
				loc = temploc[sq];
				if ((loc == -1 || (multipleCityMode && listOfLocations[loc]->city != squad_location->city)) && !havecar)
					loc = oldloc;
				int subcount = 0;
				for (int l = 0; l < len(listOfLocations); l++)
					if (listOfLocations[l]->parent == loc)
						subcount++;
				if (subcount == 0 || (multipleCityMode && loc >= 0 && listOfLocations[loc]->city != squad_location->city))
				{
					if (!listOfLocations[loc]->closed &&
						((listOfLocations[loc]->area == squad_location->area&&listOfLocations[loc]->city == squad_location->city) || havecar))
					{
						activesquad->activity.type = ACTIVITY_VISIT;
						activesquad->activity.arg = loc;
						return;
					}
					else loc = oldloc;
				}
			}
		}
		/*if(c=='z')
		{
		activesquad->stance++;
		if(activesquad->stance>SQUADSTANCE_STANDARD)
		activesquad->stance=0;
		}*/
		if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR)
		{
			if (loc != -1 && (listOfLocations[loc]->city != LocationsPool::getInstance().getLocationType(loc) || listOfLocations[loc]->city != squad_location->city))
				loc = listOfLocations[loc]->parent;
			else
			{
				activesquad->activity.type = ACTIVITY_NONE; // Clear squad activity
				break;
			}
		}
	}
}
vector<string> caseBUSINESSFRONT_INSURANCE;
vector<string> caseBUSINESSFRONT_TEMPAGENCY;
vector<string> caseBUSINESSFRONT_RESTAURANT;
vector<string> caseBUSINESSFRONT_MISCELLANEOUS;
const string mostlyendings = "mostlyendings\\";
#include "../customMaps.h"
const string CONST_locationsPool128 = "caseBUSINESSFRONT_MISCELLANEOUS.txt";
const string CONST_locationsPool127 = "caseBUSINESSFRONT_RESTAURANT.txt";
const string CONST_locationsPool126 = "caseBUSINESSFRONT_TEMPAGENCY.txt";
const string CONST_locationsPool125 = "caseBUSINESSFRONT_INSURANCE.txt";
vector<file_and_text_collection> locationspool_text_file_collection = {
	customText(&caseBUSINESSFRONT_INSURANCE, mostlyendings + CONST_locationsPool125),
	customText(&caseBUSINESSFRONT_TEMPAGENCY, mostlyendings + CONST_locationsPool126),
	customText(&caseBUSINESSFRONT_RESTAURANT, mostlyendings + CONST_locationsPool127),
	customText(&caseBUSINESSFRONT_MISCELLANEOUS, mostlyendings + CONST_locationsPool128),
};
enum BusinessFronts
{
	BUSINESSFRONT_INSURANCE,
	BUSINESSFRONT_TEMPAGENCY,
	BUSINESSFRONT_RESTAURANT,
	BUSINESSFRONT_MISCELLANEOUS,
	BUSINESSFRONTNUM
};
/* base - invest in this location */
const string CONST_locationsPool140 = "R - Stockpile 20 daily rations of food ($150)";
const string CONST_locationsPool139 = "F - Setup a Business Front to ward off suspicion ($3000)";
const string CONST_locationsPool138 = "P - Buy a Printing Press to start your own newspaper ($3000)";
const string CONST_locationsPool137 = "A - Install and conceal an illegal Anti-Aircraft gun on the roof ($200,000)";
const string CONST_locationsPool136 = "A - Install a perfectly legal Anti-Aircraft gun on the roof ($35,000)";
const string CONST_locationsPool135 = "G - Buy a Generator for emergency electricity ($3000)";
const string CONST_locationsPool134 = "T - Ring the Compound with Tank Traps ($3000)";
const string CONST_locationsPool133 = "B - Place Booby Traps throughout the Compound ($3000)";
const string CONST_locationsPool132 = "C - Place Security Cameras around the Compound ($2000)";
const string CONST_locationsPool131 = "W - Fortify the Compound for a Siege ($2000)";
const string CONST_locationsPool130 = "W - Fortify the Bomb Shelter Entrances ($2000)";
const string CONST_locationsPool129 = "W - Repair the Bunker Fortifications ($2000)";
void investlocation()
{
	extern class Ledger ledger;
	extern int selectedsiege;
	extern short lawList[LAWNUM];
	int loc = selectedsiege;
	while (true)
	{
		eraseAlt();
		locheader();
		printlocation(loc);
		if (LocationsPool::getInstance().canBeFortified(loc))
		{
			if (ledger.get_funds() >= 2000) set_color_easy(WHITE_ON_BLACK);
			else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			if (LocationsPool::getInstance().getLocationType(loc) == SITE_OUTDOOR_BUNKER)
				mvaddstrAlt(8, 1, CONST_locationsPool129);
			else if (LocationsPool::getInstance().getLocationType(loc) == SITE_RESIDENTIAL_BOMBSHELTER)
				mvaddstrAlt(8, 1, CONST_locationsPool130);
			else
				mvaddstrAlt(8, 1, CONST_locationsPool131);
		}
		if (!(LocationsPool::getInstance().get_specific_integer(INT_GETCOMPOUNDWALLS,loc) & COMPOUND_CAMERAS))
		{
			if (ledger.get_funds() >= 2000) set_color_easy(WHITE_ON_BLACK);
			else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			mvaddstrAlt(9, 1, CONST_locationsPool132);
		}
		if (listOfLocations[loc]->can_be_trapped())
		{
			if (ledger.get_funds() >= 3000) set_color_easy(WHITE_ON_BLACK);
			else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			mvaddstrAlt(10, 1, CONST_locationsPool133);
		}
		if (listOfLocations[loc]->can_install_tanktraps())
		{
			if (ledger.get_funds() >= 3000) set_color_easy(WHITE_ON_BLACK);
			else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			mvaddstrAlt(11, 1, CONST_locationsPool134);
		}
		if (!(listOfLocations[loc]->compound_walls & COMPOUND_GENERATOR))
		{
			if (ledger.get_funds() >= 3000) set_color_easy(WHITE_ON_BLACK);
			else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			mvaddstrAlt(12, 1, CONST_locationsPool135);
		}
		if (!(listOfLocations[loc]->compound_walls & COMPOUND_AAGUN))
		{
			if (lawList[LAW_GUNCONTROL] == ALIGN_ARCHCONSERVATIVE)
			{
				if (ledger.get_funds() >= 35000) set_color_easy(WHITE_ON_BLACK);
				mvaddstrAlt(13, 1, CONST_locationsPool136);
			}
			else
			{
				if (ledger.get_funds() >= 200000) set_color_easy(WHITE_ON_BLACK);
				else set_color_easy(BLACK_ON_BLACK_BRIGHT);
				mvaddstrAlt(13, 1, CONST_locationsPool137);
			}
		}
		if (!(listOfLocations[loc]->compound_walls & COMPOUND_PRINTINGPRESS))
		{
			if (ledger.get_funds() >= 3000) set_color_easy(WHITE_ON_BLACK);
			else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			mvaddstrAlt(14, 1, CONST_locationsPool138);
		}
		if (listOfLocations[loc]->can_have_businessfront())
		{
			if (ledger.get_funds() >= 3000) set_color_easy(WHITE_ON_BLACK);
			else set_color_easy(BLACK_ON_BLACK_BRIGHT);
			mvaddstrAlt(15, 1, CONST_locationsPool139);
		}
		if (ledger.get_funds() >= 150) set_color_easy(WHITE_ON_BLACK);
		else set_color_easy(BLACK_ON_BLACK_BRIGHT);
		mvaddstrAlt(16, 1, CONST_locationsPool140);
		mvaddstrAlt(17, 1, enter_done);
		int c = getkeyAlt();
		if (c == 'x' || c == ENTER || c == ESC || c == SPACEBAR) break;
		if (c == 'w')
		{
			if (listOfLocations[loc]->can_be_fortified() && ledger.get_funds() >= 2000)
			{
				ledger.subtract_funds(2000, EXPENSE_COMPOUND);
				listOfLocations[loc]->compound_walls |= COMPOUND_BASIC;
			}
		}
		if (c == 'c')
		{
			if (!(LocationsPool::getInstance().get_specific_integer(INT_GETCOMPOUNDWALLS,loc) & COMPOUND_CAMERAS) && ledger.get_funds() >= 2000)
			{
				ledger.subtract_funds(2000, EXPENSE_COMPOUND);
				listOfLocations[loc]->compound_walls |= COMPOUND_CAMERAS;
			}
		}
		if (c == 'b')
		{
			if (listOfLocations[loc]->can_be_trapped() && ledger.get_funds() >= 3000)
			{
				ledger.subtract_funds(3000, EXPENSE_COMPOUND);
				listOfLocations[loc]->compound_walls |= COMPOUND_TRAPS;
			}
		}
		if (c == 't')
		{
			if (listOfLocations[loc]->can_install_tanktraps() && ledger.get_funds() >= 3000)
			{
				ledger.subtract_funds(3000, EXPENSE_COMPOUND);
				listOfLocations[loc]->compound_walls |= COMPOUND_TANKTRAPS;
			}
		}
		if (c == 'g')
		{
			if (!(LocationsPool::getInstance().get_specific_integer(INT_GETCOMPOUNDWALLS,loc) & COMPOUND_GENERATOR) && ledger.get_funds() >= 3000)
			{
				ledger.subtract_funds(3000, EXPENSE_COMPOUND);
				listOfLocations[loc]->compound_walls |= COMPOUND_GENERATOR;
			}
		}
		if (c == 'a')
		{
			int aagunPrice = 200000;
			if (lawList[LAW_GUNCONTROL] == ALIGN_ARCHCONSERVATIVE)
				aagunPrice = 35000;
			if (!(listOfLocations[loc]->compound_walls & COMPOUND_AAGUN) && ledger.get_funds() >= aagunPrice)
			{
				ledger.subtract_funds(aagunPrice, EXPENSE_COMPOUND);
				listOfLocations[loc]->compound_walls |= COMPOUND_AAGUN;
			}
		}
		if (c == 'p')
		{
			if (!(LocationsPool::getInstance().get_specific_integer(INT_GETCOMPOUNDWALLS,loc) & COMPOUND_PRINTINGPRESS) && ledger.get_funds() >= 3000)
			{
				ledger.subtract_funds(3000, EXPENSE_COMPOUND);
				listOfLocations[loc]->compound_walls |= COMPOUND_PRINTINGPRESS;
			}
		}
		if (c == 'r')
		{
			if (ledger.get_funds() >= 150)
			{
				ledger.subtract_funds(150, EXPENSE_COMPOUND);
				listOfLocations[loc]->compound_stores += 20;
			}
		}
		if (c == 'f')
		{
			if (listOfLocations[loc]->can_have_businessfront() && ledger.get_funds() >= 3000)
			{
				ledger.subtract_funds(3000, EXPENSE_COMPOUND);
				do
				{
					listOfLocations[loc]->front_business = LCSrandom(BUSINESSFRONTNUM);
					strcpy(listOfLocations[loc]->front_name, lastname(true));
					strcat(listOfLocations[loc]->front_name, singleSpace);
					int selection;
					switch (listOfLocations[loc]->front_business)
					{
					case BUSINESSFRONT_INSURANCE:
						selection = LCSrandom(len(caseBUSINESSFRONT_INSURANCE) / 2);
						strcat(listOfLocations[loc]->front_name, caseBUSINESSFRONT_INSURANCE[selection * 2]);
						strcpy(listOfLocations[loc]->front_shortname, caseBUSINESSFRONT_INSURANCE[selection * 2 + 1]);
						break;
					case BUSINESSFRONT_TEMPAGENCY:
						selection = LCSrandom(len(caseBUSINESSFRONT_TEMPAGENCY) / 2);
						strcat(listOfLocations[loc]->front_name, caseBUSINESSFRONT_TEMPAGENCY[selection * 2]);
						strcpy(listOfLocations[loc]->front_shortname, caseBUSINESSFRONT_TEMPAGENCY[selection * 2 + 1]);
						break;
					case BUSINESSFRONT_RESTAURANT:
						selection = LCSrandom(len(caseBUSINESSFRONT_RESTAURANT) / 2);
						strcat(listOfLocations[loc]->front_name, caseBUSINESSFRONT_RESTAURANT[selection * 2]);
						strcpy(listOfLocations[loc]->front_shortname, caseBUSINESSFRONT_RESTAURANT[selection * 2 + 1]);
						break;
					case BUSINESSFRONT_MISCELLANEOUS:
						selection = LCSrandom(len(caseBUSINESSFRONT_MISCELLANEOUS) / 2);
						strcat(listOfLocations[loc]->front_name, caseBUSINESSFRONT_MISCELLANEOUS[selection * 2]);
						strcpy(listOfLocations[loc]->front_shortname, caseBUSINESSFRONT_MISCELLANEOUS[selection * 2 + 1]);
						break;
					}
				} while (listOfLocations[loc]->duplicatelocation());
			}
		}
	}
}
void DeprecatedCreature::makeIntoHostage() {
	extern Deprecatedsquadst *activesquad;
	namecreature();
	location = activesquad->squad[0]->base;
	base = activesquad->squad[0]->base;
	flag |= CREATUREFLAG_MISSING;
	//disarm them and stash their weapon back at the base
	extern vector<Location *> listOfLocations;
	drop_weapons_and_clips(&(listOfLocations[this->location]->loot));
	//Create InterrogationST data
	activity.intr() = new InterrogationST;
}
/* names the new hostage and stashes them in your base */
void DeprecatedCreature::kidnaptransfer()
{
	const string CONST_locationsPool144 = "If you do not enter anything, their real name will be used.";
	const string CONST_locationsPool143 = " in its presence?";
	const string CONST_locationsPool142 = "What name will you use for this ";
	const string CONST_locationsPool141 = "The Education of ";

	extern int stat_kidnappings;
	DeprecatedCreature *newcr = new DeprecatedCreature;
	newcr = this;
	newcr->makeIntoHostage();
	eraseAlt();
	set_color_easy(WHITE_ON_BLACK_BRIGHT);
	mvaddstrAlt(0, 0, CONST_locationsPool141);
	addstrAlt(newcr->propername);
	set_color_easy(WHITE_ON_BLACK);
	mvaddstrAlt(2, 0, CONST_locationsPool142);
	addstrAlt(newcr->get_type_name());
	addstrAlt(CONST_locationsPool143);
	mvaddstrAlt(3, 0, CONST_locationsPool144);
	newcr->new_name_four();
	newcr->addCreature();
	stat_kidnappings++;
}
/* transfer all loot from some source (such as a squad or another location) to a location, and deal with money properly */
void Location::getloot(vector<Item *>& loot)
{
	extern class Ledger ledger;
	for (int l = len(loot) - 1; l >= 0; l--)
		if (loot[l]->whatIsThis() == THIS_IS_MONEY)
		{
			Money* m = static_cast<Money*>(loot[l]); //cast -XML
			ledger.add_funds(m->get_amount(), INCOME_THIEVERY);
			delete loot[l];
		}
		else
		{  // Empty squad inventory into base inventory
			this->loot.push_back(loot[l]);
		}
	loot.clear();
}
void clearRentExemptions() {
	for (int l = 0; l < LocationsPool::getInstance().lenpool(); l++) listOfLocations[l]->newrental = 0;
}
/* monthly - guardian - prints liberal guardian special editions */
void printnews(short l, short newspaper);
#include "../items/itemPool.h"
#include "../items/lootTypePool.h"

void manageGrafiti() {
	extern char endgamestate;
	extern short background_liberal_influence[VIEWNUM];
	//Manage graffiti
	for (int l = 0; l < LocationsPool::getInstance().lenpool(); l++) // Check each location
	{
		for (int c = len(listOfLocations[l]->changes) - 1; c >= 0; c--) // Each change to the map
		{
			if (listOfLocations[l]->changes[c].flag == SITEBLOCK_GRAFFITI ||
				listOfLocations[l]->changes[c].flag == SITEBLOCK_GRAFFITI_CCS ||
				listOfLocations[l]->changes[c].flag == SITEBLOCK_GRAFFITI_OTHER) // Find changes that refer specifically to graffiti
			{
				int power = 0, align = 0;
				if (listOfLocations[l]->changes[c].flag == SITEBLOCK_GRAFFITI) align = 1;
				if (listOfLocations[l]->changes[c].flag == SITEBLOCK_GRAFFITI_CCS) align = -1;
				//Purge graffiti from more secure sites (or from non-secure
				//sites about once every five years), but these will
				//influence people more for the current month
				if (securityable(LocationsPool::getInstance().getLocationType(l)))
				{
					listOfLocations[l]->changes.erase(listOfLocations[l]->changes.begin() + c);
					power = 5;
				}
				else
				{
					if (listOfLocations[l]->renting == RENTING_CCS)
						listOfLocations[l]->changes[c].flag = SITEBLOCK_GRAFFITI_CCS; // Convert to CCS tags
					else if (listOfLocations[l]->renting == RENTING_PERMANENT)
						listOfLocations[l]->changes[c].flag = SITEBLOCK_GRAFFITI; // Convert to LCS tags
					else
					{
						power = 1;
						if (!LCSrandom(10))
							listOfLocations[l]->changes[c].flag = SITEBLOCK_GRAFFITI_OTHER; // Convert to other tags
						if (!LCSrandom(10) && endgamestate < ENDGAME_CCS_DEFEATED&&endgamestate>0)
							listOfLocations[l]->changes[c].flag = SITEBLOCK_GRAFFITI_CCS; // Convert to CCS tags
						if (!LCSrandom(30))
							listOfLocations[l]->changes.erase(listOfLocations[l]->changes.begin() + c); // Clean up
					}
				}
				if (align == 1)
				{
					background_liberal_influence[VIEW_LIBERALCRIMESQUAD] += power;
					background_liberal_influence[VIEW_CONSERVATIVECRIMESQUAD] += power;
				}
				else if (align == -1)
				{
					background_liberal_influence[VIEW_LIBERALCRIMESQUAD] -= power;
					background_liberal_influence[VIEW_CONSERVATIVECRIMESQUAD] -= power;
				}
			}
		}
	}
}
siegest* getseigestFromLocation(int secondaryLocation) {
	extern Deprecatedsquadst *activesquad;
	siegest* siege = NULL;
	int primaryLocation = activesquad ? activesquad->squad[0]->location : -1;
	if (primaryLocation != -1) {
		siege = &listOfLocations[primaryLocation]->siege;
	}
	else if (secondaryLocation != -1) {
		siege = &listOfLocations[secondaryLocation]->siege;
	}
	return siege;
}
void gotoEquipmentScreen(int loc) {
	DeprecatedCreature::equip(listOfLocations[loc]->loot, -1);
}
void DeprecatedCreature::createTempSquadWithJustThisLiberal(int cursquadid) {
	const string CONST_locationsPool145 = "Temporary Squad";

	extern Deprecatedsquadst *activesquad;
	//create a temp squad containing just this liberal
	int oldsquadid = squadid;
	Deprecatedsquadst *oldactivesquad = activesquad;
	activesquad = new Deprecatedsquadst;
	strcpy(activesquad->name, CONST_locationsPool145);
	activesquad->id = cursquadid;
	activesquad->squad[0] = this;
	squadid = activesquad->id;
	//go to equipment screen
	gotoEquipmentScreen(activesquad->squad[0]->location);
	//once you're done, restore original squad status.
	delete activesquad;
	activesquad = oldactivesquad;
	squadid = oldsquadid;
}

void moveEverythingAwayFromSite(int cursite) {
	listOfLocations[cursite]->renting = RENTING_NOCONTROL;
	//MOVE ALL ITEMS AND SQUAD MEMBERS
	const int hs = find_site_index_in_same_city(SITE_RESIDENTIAL_SHELTER, cursite);
	CreaturePool::getInstance().moveEverythingFrom(cursite, hs);
	listOfLocations[hs]->getloot(listOfLocations[cursite]->loot);
	listOfLocations[cursite]->compound_walls = 0;
	listOfLocations[cursite]->compound_stores = 0;
	listOfLocations[cursite]->front_business = -1;
}
void LocationsPool::addSiegeKill(int cursite) {
	listOfLocations[cursite]->siege.kills++;
}
void LocationsPool::removeTank(int cursite) {
	listOfLocations[cursite]->siege.tanks--;
}
void addLocationChange(int cursite, sitechangest change) {
	listOfLocations[cursite]->changes.push_back(change);
}
const int LocationsPool::getHeat(int cursite) const {
	return listOfLocations[cursite]->heat;
}
void LocationsPool::setSiegetimeuntillocated(int cursite, int timer) {
	listOfLocations[cursite]->siege.timeuntillocated = timer;
}

void LocationsPool::clearHeat(int cursite) {
	listOfLocations[cursite]->heat = 0;
}
void setColorBasedOnSiege(const int cursite, const int y, const bool p) {
	Location *loc = listOfLocations[cursite];
	siegest *siege = &loc->siege;
	if (siege ? siege->siege : false) set_color_easy(siege->underattack ? p ? RED_ON_BLACK_BRIGHT : RED_ON_BLACK : p ? YELLOW_ON_BLACK_BRIGHT : YELLOW_ON_BLACK);
	mvaddstrAlt(y, 31, listOfLocations[cursite]->getname(true, true));
}

int consolidateSiegeLoot() {
	int k = 0;
	for (int l = 0; l < len(listOfLocations); l++)
	{
		consolidateloot(listOfLocations[l]->loot);
		if (!listOfLocations[l]->siege.siege)
			k += len(listOfLocations[l]->loot); // Review and Move Equipment
	}
	return k;
}

void nukeAllEmptySquads(const vector<int> squadloc, const int mode) {
	extern Deprecatedsquadst *activesquad;
	extern vector<Deprecatedsquadst *> squad;
	//NUKE ALL EMPTY SQUADS
	for (int sq = len(squad) - 1; sq >= 0; sq--)
	{
		bool hasmembers = false;
		for (int p = 0; p < 6; p++)
			if (squad[sq]->squad[p] != NULL)
			{
				hasmembers = true; break;
			}
		// in the event a new squad is created squadloc[] will be one shorter than squad[]
		// But since a new squad cannot be created without members, this should
		// not ever break
		if (!hasmembers && mode == GAMEMODE_BASE)
		{
			if (squadloc[sq] != -1)
				listOfLocations[squadloc[sq]]->getloot(squad[sq]->loot);
			if (activesquad == squad[sq])activesquad = NULL;
			delete_and_remove(squad, sq);
		}
	}
}

void consolidateLoot(const int l) {
	consolidateloot(listOfLocations[l]->loot);
}
int LocationsPool::lenloot(const int l)const {
	return len(listOfLocations[l]->loot);
}
string getLootTitle(const int base, const int l) {
	return listOfLocations[base]->loot[l]->equip_title();
}

int getLootNumber(const int base, const int l) {
	return listOfLocations[base]->loot[l]->get_number();
}

void LocationsPool::captureSite(int cursite) {
	listOfLocations[cursite]->renting = RENTING_PERMANENT; // Capture safehouse for the glory of the LCS!
	listOfLocations[cursite]->closed = 0;
	listOfLocations[cursite]->heat = 100;
}

void LocationsPool::closeSite(int cursite, int sitecrime) {
	listOfLocations[cursite]->closed = sitecrime / 10; // Close down site
}
bool LocationsPool::siteHasCameras(int cursite)const {
	return (listOfLocations[cursite]->compound_walls & COMPOUND_CAMERAS) && !listOfLocations[cursite]->siege.cameras_off;
}
void giveActiveSquadThisLoot(Item* de);
void getRandomLoot(int cursite) {
	extern Deprecatedsquadst *activesquad;
	int b = LCSrandom(len(listOfLocations[cursite]->loot));
	Item *it = listOfLocations[cursite]->loot[b];
	giveActiveSquadThisLoot(it);
	listOfLocations[cursite]->loot.erase(listOfLocations[cursite]->loot.begin() + b);
}

char LocationsPool::isThisUnderAttack(int cursite)const {
	return listOfLocations[cursite]->siege.underattack;
}

void LocationsPool::setRenting(int cursite, int renting) {
	listOfLocations[cursite]->renting = renting;
}

char LocationsPool::isThisAFront(int cursite)const {
	return listOfLocations[cursite]->front_business;
}
string LocationsPool::getFrontName(int cursite)const {
	return listOfLocations[cursite]->front_name;
}

void LocationsPool::tickAttackTime(int cursite) {
	listOfLocations[cursite]->siege.attacktime++;
}
int LocationsPool::getAttackTime(int cursite)const {
	return listOfLocations[cursite]->siege.attacktime;
}
void LocationsPool::resetAttackTime(int cursite) {
	listOfLocations[cursite]->siege.attacktime = 0;
}
int LocationsPool::doWeHaveTankTraps(int cursite)const {
	return (listOfLocations[cursite]->compound_walls&COMPOUND_TANKTRAPS);
}
void LocationsPool::deleteTankTraps(int l) {
	listOfLocations[l]->compound_walls &= ~COMPOUND_TANKTRAPS;
}
void LocationsPool::spawnATank(int cursite) {
	listOfLocations[cursite]->siege.tanks++;
}

void LocationsPool::spawnATank(int cursite, int num) {
	listOfLocations[cursite]->siege.tanks = num;
}

int LocationsPool::getSiegeKills(int cursite)const {
	return listOfLocations[cursite]->siege.kills;
}
int LocationsPool::getSiegeTanks(int cursite)const {
	return listOfLocations[cursite]->siege.tanks;
}

void LocationsPool::turnOffSiege(int loc) {
	listOfLocations[loc]->siege.attacktime = 0;
	listOfLocations[loc]->siege.kills = 0;
	listOfLocations[loc]->siege.tanks = 0;
}

bool LocationsPool::hasTraps(int loc)const {
	return listOfLocations[loc]->compound_walls&COMPOUND_TRAPS;
}

bool LocationsPool::lightsOff(int loc) const {
	return listOfLocations[loc]->siege.lights_off;
}
void DeprecatedCreature::findArmorToRepair(Armor* armor, Item* pile, int &pileindex, vector<Item* > *pilelist) {
	extern vector<Location *> listOfLocations;
	for (int passnum = 0; passnum < 3 && armor == NULL; passnum++) {

		for (int l = 0; l < LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, this->location); l++) {

			if (listOfLocations[this->location]->loot[l]->whatIsThis() == THIS_IS_ARMOR)
			{
				bool dothis = false;
				Armor* a = static_cast<Armor*>(listOfLocations[this->location]->loot[l]);//cast -XML
				bool blood = a->is_bloody();
				bool damage = a->is_damaged();
				bool hard = armor_makedifficulty(*a) > 4;
				bool easy = armor_makedifficulty(*a) <= 4;

				switch (passnum)
				{
				case 0: // Guaranteed to accomplish something
					dothis = (blood && damage);
					break;
				case 1: // Find something to clean if low skill, repair if high
					dothis = (blood && hard)
						|| (damage && easy);
					break;
				case 2: // Anything that needs work
					dothis = (blood || damage);
					break;
				}
				if (dothis)
				{
					armor = a;
					pile = listOfLocations[this->location]->loot[l];
					pileindex = l;
					pilelist = &listOfLocations[this->location]->loot;
					break;
				}
			}

		}

	}
}
char tryFindCloth(int cursite) {
	for (int l = 0; l < len(listOfLocations[cursite]->loot); l++) {
		if (listOfLocations[cursite]->loot[l]->whatIsThis() == THIS_IS_LOOT &&
			(listOfLocations[cursite]->loot[l])->is_cloth()) //cast -XML
		{
			if (listOfLocations[cursite]->loot[l]->get_number() == 1)
				delete_and_remove(listOfLocations[cursite]->loot, l);
			else listOfLocations[cursite]->loot[l]->decrease_number(1);
			return 1;
		}
	}
	return 0;
}
void addLootToLoc(int loc, Item* it) {
	listOfLocations[loc]->loot.push_back(it);
}
string DeprecatedCreature::gimmeASprayCan() {
	extern vector<Location *> listOfLocations;
	for (int i = 0; i < LocationsPool::getInstance().get_specific_integer(INT_LENLOOT, base); i++)
	{
		if (listOfLocations[base]->loot[i]->whatIsThis() == THIS_IS_WEAPON)
		{
			Weapon *w = static_cast<Weapon*>(listOfLocations[base]->loot[i]); //cast -XML
			if (w->get_specific_bool(BOOL_CAN_GRAFFITI_))
			{
				give_weapon(*w, &(listOfLocations[base]->loot));
				if (listOfLocations[base]->loot[i]->empty())
					delete_and_remove(listOfLocations[base]->loot, i);
				return w->get_name();
			}
		}
	}
	return "";
}
const string tag_WEAPON_SPRAYCAN = "WEAPON_SPRAYCAN";
void DeprecatedCreature::buyMeASprayCan() {

	extern vector<Location *> listOfLocations;
	extern vector<WeaponType *> weapontype;
	Weapon spray(*weapontype[getweapontype(tag_WEAPON_SPRAYCAN)]);
	give_weapon(spray, &listOfLocations[base]->loot);
}
void DeprecatedCreature::lootTheBody(int base) {
	//MAKE BASE LOOT
	extern vector<Location *> listOfLocations;
	makeloot(listOfLocations[base]->loot);
}

int countSafeHouses() {
	int safenumber = 0;
	for (int l = 0; l < len(listOfLocations); l++) if (listOfLocations[l]->is_lcs_safehouse()) safenumber++;
	return safenumber;
}
Location* getLocation() {
	extern Deprecatedsquadst *activesquad;
	extern int selectedsiege;
	Location* loc = NULL;
	if (selectedsiege != -1) loc = listOfLocations[selectedsiege];
	if (activesquad) if (activesquad->squad[0]->location != -1)
		loc = listOfLocations[activesquad->squad[0]->location];
	return loc;
}

bool isPartOfJusticeSystem(int cursite) {
	return listOfLocations[cursite]->part_of_justice_system();
}
bool LocationsPool::canBeUpgraded(int cursite)const {
	return listOfLocations[cursite]->can_be_upgraded();
}

void LocationsPool::clearunderattack(int cursite) {
	listOfLocations[cursite]->siege.underattack = 0;
}
bool isThisSafehouse(int loc) {
	return listOfLocations[loc]->is_lcs_safehouse();
}
void equipLoot(int l, int loc) {
	DeprecatedCreature::equip(listOfLocations[l]->loot, loc);
}
void burnFlagAtLocation(int l) {
	listOfLocations[l]->haveflag = 0;
}
void locationIsNowRented(int l, int rent) {
	listOfLocations[l]->renting = rent;
	listOfLocations[l]->newrental = 1;
}

int getCity(int l) {
	return listOfLocations[l]->city;
}

int getFenceValueLocation(int l, int slot) {
	return listOfLocations[l]->loot[slot]->get_fencevalue();
}

bool getCanBeSoldLocation(int l, int slot) {
	return listOfLocations[l]->loot[slot]->is_good_for_sale();
}

int getLocationLootNumber(int l, int slot) {
	return listOfLocations[l]->loot[slot]->get_number();
}

void decreateLocationLoot(int loc, int loot, int num) {
	listOfLocations[loc]->loot[loot]->decrease_number(num);
	if (listOfLocations[loc]->loot[loot]->empty())
		delete_and_remove(listOfLocations[loc]->loot, loot);
}

int whatIsThisItemInLocation(int loc, int l) {
	return listOfLocations[loc]->loot[l]->whatIsThis();
}

void deleteLocationLoot(int loc, int loot) {
	delete_and_remove(listOfLocations[loc]->loot, loot);
}
void deleteLocationLoot(int loc) {
	delete_and_clear(listOfLocations[loc]->loot);
}

bool noQuickFenceLocation(int loc, int l) {
	return listOfLocations[loc]->loot[l]->no_quick_fencing();
}

void CCSCapturesSite(int loc) {
	listOfLocations[loc]->renting = RENTING_CCS;
}

int LocationsPool::getStoresAmount(int l)const {
	return listOfLocations[l]->compound_stores;
}

void endLocationSiege(int l) {
	listOfLocations[l]->siege.siege = 0;
}

void reduceCompoundStores(int loc, int amount) {
	if (listOfLocations[loc]->compound_stores <= amount) {
		listOfLocations[loc]->compound_stores = 0;
	}
	else {
		listOfLocations[loc]->compound_stores -= amount;
	}
}
void emptyCompoundStores(int l) {
	listOfLocations[l]->compound_stores = 0;
}

bool hasPrintingPress(int l) {
	return listOfLocations[l]->compound_walls&COMPOUND_PRINTINGPRESS;
}

void deletePrintingPress(int loc) {

	listOfLocations[loc]->compound_walls &= ~COMPOUND_PRINTINGPRESS;
}

siegest getWholeSiege(int l) {
	return listOfLocations[l]->siege;
}

void deleteBusinessFront(int l) {

	listOfLocations[l]->front_business = -1;
}

void deleteCompoundWalls(int loc) {

	listOfLocations[loc]->compound_walls = 0;
}

void dropHeatByFivePercent(int l) {
	listOfLocations[l]->heat = static_cast<int>(listOfLocations[l]->heat * 0.95);
}

int getTimeUntilSiege(int l) {
	return listOfLocations[l]->siege.timeuntillocated;
}

void huntFasterIfSiteIncrediblyHot(int l) {
	if (listOfLocations[l]->heat > 100)
	{
		int hunt_speed;
		hunt_speed = listOfLocations[l]->heat / 50;
		while (hunt_speed&&listOfLocations[l]->siege.timeuntillocated > 1)
		{
			listOfLocations[l]->siege.timeuntillocated--;
			hunt_speed--;
		}
	}
}

void updateLocationHeatProtection(int l) {
	listOfLocations[l]->update_heat_protection();
}

void letPlaceCoolOffUnlessCrime(int crimes, int l) {
	if (crimes < listOfLocations[l]->heat)
	{
		listOfLocations[l]->heat -= 1;
		if (listOfLocations[l]->heat < 0)
			listOfLocations[l]->heat = 0;
	}
	else
	{
		// Update location heat
		if (crimes > listOfLocations[l]->heat) listOfLocations[l]->heat += (crimes - listOfLocations[l]->heat) / 10 + 1;
		// Begin planning siege if high heat on location
		if (listOfLocations[l]->heat > listOfLocations[l]->heat_protection &&
			LCSrandom(500) < listOfLocations[l]->heat &&
			!(listOfLocations[l]->siege.timeuntillocated >= 0)) //Do not re-plan siege.
		{
			// Set time until siege is carried out
			listOfLocations[l]->siege.timeuntillocated += 2 + LCSrandom(6);
		}
	}
}

void policeSiege(int l) {

	listOfLocations[l]->siege.siege = 1;
	listOfLocations[l]->siege.siegetype = SIEGE_POLICE;
	listOfLocations[l]->siege.lights_off = 0;
	listOfLocations[l]->siege.cameras_off = 0;
}
void corporateSiege(int l) {
	listOfLocations[l]->siege.siege = 1;
	listOfLocations[l]->siege.siegetype = SIEGE_CORPORATE;
	listOfLocations[l]->siege.underattack = 1;
	listOfLocations[l]->siege.lights_off = 0;
	listOfLocations[l]->siege.cameras_off = 0;
}

void CCSSiege(int l) {
	listOfLocations[l]->siege.siege = 1;
	listOfLocations[l]->siege.siegetype = SIEGE_CCS;
	listOfLocations[l]->siege.underattack = 1;
	listOfLocations[l]->siege.lights_off = 0;
	listOfLocations[l]->siege.cameras_off = 0;
}

void CIASiege(int l) {
	listOfLocations[l]->siege.siege = 1;
	listOfLocations[l]->siege.siegetype = SIEGE_CIA;
	listOfLocations[l]->siege.underattack = 1;
	listOfLocations[l]->siege.lights_off = 1;
	listOfLocations[l]->siege.cameras_off = 1;
}

void hicksSiege(int l) {
	listOfLocations[l]->siege.siege = 1;
	listOfLocations[l]->siege.siegetype = SIEGE_HICKS;
	listOfLocations[l]->siege.underattack = 1;
	listOfLocations[l]->siege.lights_off = 0;
	listOfLocations[l]->siege.cameras_off = 0;
}

void firemanSiege(int l) {
	listOfLocations[l]->siege.siege = 1;
	listOfLocations[l]->siege.siegetype = SIEGE_FIREMEN;
	listOfLocations[l]->siege.underattack = 1;
	listOfLocations[l]->siege.lights_off = 0;
	listOfLocations[l]->siege.cameras_off = 0;

}

void escalateSite(int l) {
	listOfLocations[l]->siege.timeuntillocated = LCSrandom(4) + 4;
	listOfLocations[l]->siege.escalationstate++;
}

void dumpLootAtLocation(int homes, vector<Item *>& loot) {
	listOfLocations[homes]->getloot(loot);
}

void deleteGeneratorLightsOff(int l) {

	listOfLocations[l]->compound_walls &= ~COMPOUND_GENERATOR;
	listOfLocations[l]->siege.lights_off = 1;
}

int LocationsPool::getTimeUntilSiege(int loc, int type)const {
	switch (type) {
	case SIEGE_POLICE:
	//case INT_GETTIMEUNTILSIEGE_POLICE:
<<<<<<< HEAD
		return listOfLocations[loc]->siege.timeuntillocated;
		break;
	case SIEGE_CIA:
	//case INT_GETTIMEUNTILSIEGE_CIA:
		return listOfLocations[loc]->siege.timeuntilcia;
		break;
	case SIEGE_CORPORATE:
	//case INT_GETTIMEUNTILSIEGE_CORPORATE:
		return listOfLocations[loc]->siege.timeuntilcorps;
		break;
	case SIEGE_CCS:
	//case INT_GETTIMEUNTILSIEGE_CCS:
		return listOfLocations[loc]->siege.timeuntilccs;
		break;
	case SIEGE_FIREMEN:
	//case INT_GETTIMEUNTILSIEGE_FIREMEN:
		return listOfLocations[loc]->siege.timeuntilfiremen;
=======
		return location[loc]->siege.timeuntillocated;
		break;
	case SIEGE_CIA:
	//case INT_GETTIMEUNTILSIEGE_CIA:
		return location[loc]->siege.timeuntilcia;
		break;
	case SIEGE_CORPORATE:
	//case INT_GETTIMEUNTILSIEGE_CORPORATE:
		return location[loc]->siege.timeuntilcorps;
		break;
	case SIEGE_CCS:
	//case INT_GETTIMEUNTILSIEGE_CCS:
		return location[loc]->siege.timeuntilccs;
		break;
	case SIEGE_FIREMEN:
	//case INT_GETTIMEUNTILSIEGE_FIREMEN:
		return location[loc]->siege.timeuntilfiremen;
>>>>>>> acaa55987f7b177b662bdb7f42ebb6850475784a
		break;
	default:
	case SIEGE_ORG:
	//case INT_GETTIMEUNTILSIEGE_ORG:
	case SIEGE_HICKS:
	//case INT_GETTIMEUNTILSIEGE_HICKS:
	case SIEGENUM:
		return 0;
		break;

	}
}

void LocationsPool::setTimeUntilSiege(int loc, int type, int time) {
	switch (type) {
	case SIEGE_POLICE:
		listOfLocations[loc]->siege.timeuntillocated = time;
		break;
	case SIEGE_CIA:
		listOfLocations[loc]->siege.timeuntilcia = time;
		break;
	case SIEGE_CORPORATE:
		listOfLocations[loc]->siege.timeuntilcorps = time;
		break;
	case SIEGE_CCS:
		listOfLocations[loc]->siege.timeuntilccs = time;
		break;
	case SIEGE_FIREMEN:
		listOfLocations[loc]->siege.timeuntilfiremen = time;
		break;
	default:
	case SIEGE_ORG:
	case SIEGE_HICKS:
	case SIEGENUM:
		;
		break;
	}
}

bool hasBusinessFront(int l) {
	return listOfLocations[l]->front_business != -1;
}

bool hasAGenerator(int l) {
	return listOfLocations[l]->compound_walls & COMPOUND_GENERATOR;
}

bool hasCameras(int l) {
	return listOfLocations[l]->compound_walls & COMPOUND_CAMERAS;
}

void deleteAAGun(int l) {
	listOfLocations[l]->compound_walls &= ~COMPOUND_AAGUN;
}

bool siteHasAAGun(int l) {
	return listOfLocations[l]->compound_walls & COMPOUND_AAGUN;
}

void setUnderAttack(int l) {

	listOfLocations[l]->siege.underattack = 1;
}

bool getLightsOff(int l) {
	return listOfLocations[l]->siege.lights_off;
}

void setLightsOff(int l) {

	listOfLocations[l]->siege.lights_off = 1;
}

bool hasBasicCompoundWalls(int l) {
	return listOfLocations[l]->compound_walls & COMPOUND_BASIC;
}

const int LocationsPool::get_specific_integer(const locationsPoolIntegers lPI, const int cursite) const {
	switch (lPI) {
	case INT_GETSTORESAMOUNT: return getStoresAmount(cursite); break;
	case INT_GETRENTINGTYPE: return getRentingType(cursite); break;
	case INT_ISTHISPLACEHIGHSECURITY: return isThisPlaceHighSecurity(cursite); break;
	case INT_GETLOCATIONCITY: return getLocationCity(cursite); break;

	//case INT_GETTIMEUNTILSIEGE_POLICE: 
	//case INT_GETTIMEUNTILSIEGE_CIA: 
	//case INT_GETTIMEUNTILSIEGE_CORPORATE: 
	//case INT_GETTIMEUNTILSIEGE_CCS: 
	//case INT_GETTIMEUNTILSIEGE_FIREMEN: 
	//case INT_GETTIMEUNTILSIEGE_ORG: 
	//case INT_GETTIMEUNTILSIEGE_HICKS: 
	//	return getTimeUntilSiege(cursite, lPI); break;

	case INT_GETCOMPOUNDWALLS: return getCompoundWalls(cursite); break;
	case INT_ISTHISSITECLOSED: return isThisSiteClosed(cursite); break;
	case INT_GETLOCATIONPARENT: return getLocationParent(cursite); break;
	case INT_GETATTACKTIME: return getAttackTime(cursite); break;
	case INT_DOWEHAVETANKTRAPS: return doWeHaveTankTraps(cursite); break;
	case INT_GETSIEGEKILLS: return getSiegeKills(cursite); break;
	case INT_GETSIEGETANKS: return getSiegeTanks(cursite); break;
	case INT_GETHEAT: return getHeat(cursite); break;
	case INT_LENLOOT: return lenloot(cursite); break;

	default: //ERROR
		return -1;
	}
}