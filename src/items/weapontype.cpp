
#include "../includes.h"
const string CONST_weapontypeB091 = "striking";
const string CONST_weapontype099 = "UNDEF";
const string CONST_weapontype098 = "INVALID SUBTYPE";
const string CONST_weapontype096 = "Unknown element for attack: ";
const string CONST_weapontype095 = "Unknown element for attack::fire: ";
const string CONST_weapontype094 = "Unknown element for attack::critical: ";
const string CONST_weapontype093 = "Invalid severtype for attack::critical::severtype: ";
const string CONST_weapontype092 = "Invalid boolean value for attack::damages_armor: ";
const string CONST_weapontype091 = "Invalid severtype for attack::severtype: ";
const string CONST_weapontype090 = "Invalid boolean value for attack::bleeding: ";
const string CONST_weapontype089 = "Invalid boolean value for attack::shoots: ";
const string CONST_weapontype088 = "Invalid boolean value for attack::burns: ";
const string CONST_weapontype087 = "Invalid boolean value for attack::cuts: ";
const string CONST_weapontype086 = "Invalid boolean value for attack::tears: ";
const string CONST_weapontype085 = "Invalid boolean value for attack::bruises: ";
const string CONST_weapontype084 = "Invalid skill name for attack::skill: ";
const string CONST_weapontype083 = "Invalid boolean value for attack::always_describe_hit: ";
const string CONST_weapontype082 = "Invalid boolean value for attack::thrown ";
const string CONST_weapontype081 = "Invalid boolean value for attack::ranged ";
const string CONST_weapontype080 = "assaults";
const string CONST_weapontype078 = ": ";
const string CONST_weapontype077 = "Unknown element for weapon type ";
const string CONST_weapontype076 = "::suspicious: ";
const string CONST_weapontype075 = "Invalid boolean value for weapon type ";
const string CONST_weapontype074 = "::auto_break_locks: ";
const string CONST_weapontype072 = "::graffiti: ";
const string CONST_weapontype070 = "::instrument: ";
const string CONST_weapontype068 = "::musical_attack: ";
const string CONST_weapontype066 = "::threatening: ";
const string CONST_weapontype064 = "::can_take_hostages: ";

const string tag_chance_causes_debris = "chance_causes_debris";
const string tag_chance = "chance";
const string tag_fire = "fire";
const string tag_severtype = "severtype";
const string tag_fixed_damage = "fixed_damage";
const string tag_random_damage = "random_damage";
const string tag_hits_required = "hits_required";
const string tag_critical = "critical";
const string tag_no = "no";
const string tag_no_DR_for_limbs_chance = "no_damage_reduction_for_limbs_chance";
const string tag_armor = "armor";
const string tag_armorpiercing = "armorpiercing";
const string tag_damages_armor = "damages_armor";
const string tag_bleeding = "bleeding";
const string tag_shoots = "shoots";
const string tag_burns = "burns";
const string tag_cuts = "cuts";
const string tag_tears = "tears";
const string tag_bruises = "bruises";
const string tag_strength = "strength";
const string tag_strength_max = "strength_max";
const string tag_strength_min = "strength_min";
const string tag_successive_attacks_difficulty = "successive_attacks_difficulty";
const string tag_number_attacks = "number_attacks";
const string tag_accuracy_bonus = "accuracy_bonus";
const string tag_skill = "skill";
const string tag_hit_punctuation = "hit_punctuation";
const string tag_always_describe_hit = "always_describe_hit";
const string tag_hit_description = "hit_description";
const string tag_attack = "attack";
const string tag_attack_description = "attack_description";
const string tag_ammotype = "ammotype";
const string tag_ammo = "ammo";
const string tag_can_backstab = "can_backstab";
const string tag_thrown = "thrown";
const string tag_ranged = "ranged";
const string tag_priority = "priority";
const string tag_size = "size";
const string tag_suspicious = "suspicious";
const string tag_auto_break_locks = "auto_break_locks";
const string tag_bashstrengthmod = "bashstrengthmod";
const string tag_legality = "legality";
const string tag_graffiti = "graffiti";
const string tag_instrument = "instrument";
const string tag_music = "music";
const string tag_musical_attack = "musical_attack";
const string tag_protects_against_kidnapping = "protects_against_kidnapping";
const string tag_can_threaten_hostages = "can_threaten_hostages";
const string tag_threatening = "threatening";
const string tag_can_take_hostages = "can_take_hostages";
const string tag_shortname_future = "shortname_future";
const string tag_shortname = "shortname";
const string tag_shortname_future_sub_2 = "shortname_future_sub_2";
const string tag_shortname_future_sub_1 = "shortname_future_sub_1";
const string tag_shortname_sub_2 = "shortname_sub_2";
const string tag_shortname_sub_1 = "shortname_sub_1";
const string tag_name_future = "name_future";
const string tag_name = "name";
const string tag_name_future_sub_2 = "name_future_sub_2";
const string tag_name_future_sub_1 = "name_future_sub_1";
const string tag_name_sub_2 = "name_sub_2";
const string tag_name_sub_1 = "name_sub_1";
#include "../creature/creatureEnums.h"
#include "../items/itemtype.h"
#include "../items/cliptype.h"
#include "../items/weapontype.h"
//own header currently inside includes.h
//own header
#include "../common/stringconversion.h"
//for stringtobool
extern int year;
extern string singleDot;
extern short lawList[LAWNUM];
WeaponType::WeaponType(MCD_STR xmlstring)
 : ItemType(xmlstring), name_sub_1_defined_(false), name_sub_2_defined_(false),
   name_future_sub_1_defined_(false), name_future_sub_2_defined_(false),
   shortname_(CONST_weapontype099), shortname_defined_(false), shortname_future_defined_(false),
   shortname_sub_1_defined_(false), shortname_sub_2_defined_(false),
   shortname_future_sub_1_defined_(false), shortname_future_sub_2_defined_(false),
   can_take_hostages_(false), threatening_(false), can_threaten_hostages_(true),
   protects_against_kidnapping_(true),
   musical_attack_(false), instrument_(false), legality_(2), bashstrengthmod_(1),
   suspicious_(true), size_(15), can_graffiti_(false), auto_break_lock_(false)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem()) //Loop over all the elements inside the weapontype element.
   {
      std::string element = xml.GetTagName();
      if (element == tag_shortname)
      {
         shortname_ = xml.GetData();
         shortname_defined_ = true;
      }
      else if (element == tag_shortname_future)
      {
         shortname_future_ = xml.GetData();
         shortname_future_defined_ = true;
      }
      else if (element == tag_name_sub_1)
      {
         name_sub_1_ = xml.GetData();
         name_sub_1_defined_ = true;
      }
      else if (element == tag_name_sub_2)
      {
         name_sub_2_ = xml.GetData();
         name_sub_2_defined_ = true;
      }
      else if (element == tag_name_future_sub_1)
      {
         name_future_sub_1_ = xml.GetData();
         name_future_sub_1_defined_ = true;
      }
      else if (element == tag_name_future_sub_2)
      {
         name_future_sub_2_ = xml.GetData();
         name_future_sub_2_defined_ = true;
      }
      else if (element == tag_shortname_sub_1)
      {
         shortname_sub_1_ = xml.GetData();
         shortname_sub_1_defined_ = true;
      }
      else if (element == tag_shortname_sub_2)
      {
         shortname_sub_2_ = xml.GetData();
         shortname_sub_2_defined_ = true;
      }
      else if (element == tag_shortname_future_sub_1)
      {
         shortname_future_sub_1_ = xml.GetData();
         shortname_future_sub_1_defined_ = true;
      }
      else if (element == tag_shortname_future_sub_2)
      {
         shortname_future_sub_2_ = xml.GetData();
         shortname_future_sub_2_defined_ = true;
      }
      else if (element == tag_can_take_hostages)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_take_hostages_ = true;
         else if (b == 0)
            can_take_hostages_ = false;
         /*else
            errorlog << CONST_weapontype075 << idname()
                      << CONST_weapontype064 << xml.GetData() << endl;*/
      }
      else if (element == tag_threatening)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            threatening_ = true;
         else if (b == 0)
            threatening_ = false;
         /*else
            errorlog << CONST_weapontype075 << idname()
                      << CONST_weapontype066 << xml.GetData() << endl;*/
      }
      else if (element == tag_can_threaten_hostages)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_threaten_hostages_ = true;
         else if (b == 0)
            can_threaten_hostages_ = false;
      }
      else if (element == tag_protects_against_kidnapping)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            protects_against_kidnapping_ = true;
         else if (b == 0)
            protects_against_kidnapping_ = false;
      }
      else if (element == tag_musical_attack)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            musical_attack_ = true;
         else if (b == 0)
            musical_attack_ = false;
         /*else
            errorlog << CONST_weapontype075 << idname()
                      << CONST_weapontype068 << xml.GetData() << endl;*/
      }
      else if (element == tag_instrument)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            instrument_ = true;
         else if (b == 0)
            instrument_ = false;
         /*else
            errorlog << CONST_weapontype075 << idname()
                      << CONST_weapontype070 << xml.GetData() << endl;*/
      }
      else if (element == tag_graffiti)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_graffiti_ = true;
         else if (b == 0)
            can_graffiti_ = false;
         /*else
            errorlog << CONST_weapontype075 << idname()
                      << CONST_weapontype072 << xml.GetData() << endl;*/
      }
      else if (element == tag_legality)
         legality_ = atoi(xml.GetData());
      else if (element == tag_bashstrengthmod)
         bashstrengthmod_ = atoi(xml.GetData()) / 100.0;
      else if (element == tag_auto_break_locks)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            auto_break_lock_ = true;
         else if (b == 0)
            auto_break_lock_ = false;
         /*else
            errorlog << CONST_weapontype075 << idname()
                      << CONST_weapontype074 << xml.GetData() << endl;*/
      }
      else if (element == tag_suspicious)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            suspicious_ = true;
         else if (b == 0)
            suspicious_ = false;
         /*else
            errorlog << CONST_weapontype075 << idname()
                      << CONST_weapontype076 << xml.GetData() << endl;*/
      }
      else if (element == tag_size)
         size_ = atoi(xml.GetData());
      else if (element == tag_attack)
      {
         attackst* attack = new attackst(xml.GetSubDoc());
         int i;
         for(i=0;i<len(attacks_)&&attack->priority>=attacks_[i]->priority;i++);
         attacks_.insert(attacks_.begin()+i,attack);
      }
      /*else
         errorlog << CONST_weapontype077 << idname()
                   << CONST_weapontype078 << element << endl;*/
   }
   if (!shortname_defined_)
   {
      if ((uses_ammo() && len(name_) <= 9)
          || len(name_) <= 14)
         shortname_ = name_;
   }
   else
   {
      if (len(shortname_) > 9 && uses_ammo())
         shortname_.resize(9);
      else if (len(shortname_) > 14)
         shortname_.resize(14);
   }
}
attackst::attackst(MCD_STR xmlstring)
 : priority(1), ranged(false), thrown(false), ammotype(CONST_weapontype099), uses_ammo(false),
   attack_description(CONST_weapontype080), hit_description(CONST_weapontypeB091),
   always_describe_hit(false), can_backstab(false), hit_punctuation(singleDot),
   skill(SKILL_CLUB), accuracy_bonus(0), number_attacks(1),
   successive_attacks_difficulty(0), strength_min(5), strength_max(10),
	random_damage(1), fixed_damage(1), bruises(false), tears(false), cuts(false),
	burns(false), shoots(false), bleeding(false), severtype(0), damages_armor(false),
	armorpiercing(0), no_damage_reduction_for_limbs_chance(0)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem()) //Loop over all the elements inside the vehicletype element.
   {
      std::string element = xml.GetTagName();
      if (element == tag_priority)
         priority = atoi(xml.GetData());
      else if (element == tag_ranged)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            ranged = true;
         else if (b == 0)
            ranged = false;
         /*else
            errorlog << CONST_weapontype081 << xml.GetData() << endl;*/
      }
      else if (element == tag_thrown)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            thrown = true;
         else if (b == 0)
            thrown = false;
         /*else
            errorlog << CONST_weapontype082 << xml.GetData() << endl;*/
      }
      else if (element == tag_can_backstab)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            can_backstab = true;
         else if (b == 0)
            can_backstab = false;
      }
      else if (element == tag_ammotype)
      {
         ammotype = xml.GetData();
         uses_ammo = true;
      }
      else if (element == tag_attack_description)
         attack_description = xml.GetData();
      else if (element == tag_hit_description)
         hit_description = xml.GetData();
      else if (element == tag_always_describe_hit)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            always_describe_hit = true;
         else if (b == 0)
            always_describe_hit = false;
         /*else
            errorlog << CONST_weapontype083 << xml.GetData() << endl;*/
      }
      else if (element == tag_hit_punctuation)
         hit_punctuation = xml.GetData();
      else if (element == tag_skill)
      {
         int s = skill_string_to_enum(xml.GetData());
         if (s != -1)
            skill = s;
         /*else
            errorlog << CONST_weapontype084 << xml.GetData() << endl; */
      }
      else if (element == tag_accuracy_bonus)
         accuracy_bonus = atoi(xml.GetData());
      else if (element == tag_number_attacks)
         number_attacks = atoi(xml.GetData());
      else if (element == tag_successive_attacks_difficulty)
         successive_attacks_difficulty = atoi(xml.GetData());
      else if (element == tag_strength_min)
         strength_min = atoi(xml.GetData());
      else if (element == tag_strength_max)
         strength_max = atoi(xml.GetData());
      else if (element == tag_random_damage)
         random_damage = atoi(xml.GetData());
      else if (element == tag_fixed_damage)
         fixed_damage = atoi(xml.GetData());
      else if (element == tag_bruises)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            bruises = true;
         else if (b == 0)
            bruises = false;
         /*else
            errorlog << CONST_weapontype085 << xml.GetData() << endl;*/
      }
      else if (element == tag_tears)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            tears = true;
         else if (b == 0)
            tears = false;
         /*else
            errorlog << CONST_weapontype086 << xml.GetData() << endl;*/
      }
      else if (element == tag_cuts)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            cuts = true;
         else if (b == 0)
            cuts = false;
         /*else
            errorlog << CONST_weapontype087 << xml.GetData() << endl;*/
      }
      else if (element == tag_burns)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            burns = true;
         else if (b == 0)
            burns = false;
         /*else
            errorlog << CONST_weapontype088 << xml.GetData() << endl;*/
      }
      else if (element == tag_shoots)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            shoots = true;
         else if (b == 0)
            shoots = false;
         /*else
            errorlog << CONST_weapontype089 << xml.GetData() << endl;*/
      }
      else if (element == tag_bleeding)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            bleeding = true;
         else if (b == 0)
            bleeding = false;
         /*else
            errorlog << CONST_weapontype090 << xml.GetData() << endl;*/
      }
      else if (element == tag_severtype)
      {
         int s = severtype_string_to_enum(xml.GetData());
         if (s != -1)
            severtype = s;
         /*else
            errorlog << CONST_weapontype091 << xml.GetData() << endl; */
      }
      else if (element == tag_damages_armor)
      {
         int b = stringtobool(xml.GetData());
         if (b == 1)
            damages_armor = true;
         else if (b == 0)
            damages_armor = false;
         /*else
            errorlog << CONST_weapontype092 << xml.GetData() << endl;*/
      }
      else if (element == tag_armorpiercing)
         armorpiercing = atoi(xml.GetData());
      else if (element == tag_no_DR_for_limbs_chance)
         no_damage_reduction_for_limbs_chance = atoi(xml.GetData());
      else if (element == tag_critical)
      {
         xml.IntoElem();
         while(xml.FindElem())
         {
            element = xml.GetTagName();
            if (element == tag_chance)
               critical.chance = atoi(xml.GetData());
            else if (element == tag_hits_required)
               critical.hits_required = atoi(xml.GetData());
            else if (element == tag_random_damage)
            {
               critical.random_damage = atoi(xml.GetData());
               critical.random_damage_defined = true;
            }
            else if (element == tag_fixed_damage)
            {
               critical.fixed_damage = atoi(xml.GetData());
               critical.fixed_damage_defined = true;
            }
            else if (element == tag_severtype)
            {
               int s = severtype_string_to_enum(xml.GetData());
               if (s != -1)
               {
                  critical.severtype = s;
                  critical.severtype_defined = true;
               }
               /*else
                  errorlog << CONST_weapontype093 << xml.GetData() << endl; */
            }
            /*else
               errorlog << CONST_weapontype094 << element << endl; */
         }
         xml.OutOfElem();
      }
      else if (element == tag_fire)
      {
         xml.IntoElem();
         while(xml.FindElem())
         {
            element = xml.GetTagName();
            if (element == tag_chance)
               fire.chance = atoi(xml.GetData());
            else if (element == tag_chance_causes_debris)
               fire.chance_causes_debris = atoi(xml.GetData());
            /*else
               errorlog << CONST_weapontype095 << element << endl; */
         }
         xml.OutOfElem();
      }
      /*else
         errorlog << CONST_weapontype096 << element << endl; */
   }
   if (!bruises && !tears && !cuts && !burns && !shoots)
      bruises = true; //If no type specified, then bruise.
}
attackst::criticalst::criticalst()
 : chance(0), hits_required(1), random_damage(1), random_damage_defined(false),
   fixed_damage(1), fixed_damage_defined(false), severtype(0), severtype_defined(false)
{
}
attackst::firest::firest()
 : chance(0), chance_causes_debris(0)
{
}
WeaponType::~WeaponType()
{
   delete_and_clear(attacks_);
}
const string& WeaponType::get_name(unsigned subtype) const
{
   if (subtype == 0)
      return get_name();
   else if (subtype == 1)
   {
      if (name_future_sub_1_defined_ && year >= 2100)
         return name_future_sub_1_;
      else if (name_sub_1_defined_)
         return name_sub_1_;
      else
         return get_name();
   }
   else if (subtype == 2)
   {
      if (name_future_sub_2_defined_ && year >= 2100)
         return name_future_sub_2_;
      else if (name_sub_2_defined_)
         return name_sub_2_;
      else
         return get_name();
   }
   else
      return get_name(); //return CONST_weapontype098; //Reference to temporary. -XML
}
const string& WeaponType::get_shortname(unsigned subtype) const
{
   if (subtype > 2)
      return shortname_; //return CONST_weapontype098; //Reference to temporary. -XML
   else if (subtype == 1)
   {
      if (shortname_future_sub_1_defined_ && year >= 2100)
         return shortname_future_sub_1_;
      else if (year >= 2100 && name_future_sub_1_defined_ && len(name_future_sub_1_) <= 14) //Too long for ammo using weapons. -XML
         return name_future_sub_1_;
      else if (shortname_sub_1_defined_)
         return shortname_sub_1_;
      else if (name_sub_1_defined_ && len(name_sub_1_) <= 14) //Too long for ammo using weapons. -XML
         return name_sub_1_;
   }
   else if (subtype == 2)
   {
      if (shortname_future_sub_2_defined_ && year >= 2100)
         return shortname_future_sub_2_;
      else if (year >= 2100 && name_future_sub_2_defined_ && len(name_future_sub_2_) <= 14) //Too long for ammo using weapons. -XML
         return name_future_sub_2_;
      else if (shortname_sub_2_defined_)
         return shortname_sub_2_;
      else if (name_sub_2_defined_ && len(name_sub_2_) <= 14) //Too long for ammo using weapons. -XML
         return name_sub_2_;
   }
   if (shortname_future_defined_ && year >= 2100)
      return shortname_future_;
   else if (year >= 2100 && name_future_defined_ && len(name_future_) <= 14) //Too long for ammo using weapons. -XML
      return name_future_;
   else //if (shortname_defined_)
      return shortname_;
   /*else if (len(name()) <= 14) //Too long for ammo using weapons. -XML
      return name();
   else
      return CONST_weapontype099;*/
}
bool WeaponType::uses_ammo() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->uses_ammo)
        return true;
   return false;
}
bool WeaponType::acceptable_ammo(const string& clipname) const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->ammotype==clipname)
        return true;
   return false;
}
bool WeaponType::is_ranged() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->ranged)
        return true;
   return false;
}
bool WeaponType::is_throwable() const
{
   for(int i=0;i<len(attacks_);i++)
      if(attacks_[i]->thrown)
        return true;
   return false;
}
bool WeaponType::is_legal() const
{
   return legality_>=lawList[LAW_GUNCONTROL];
}