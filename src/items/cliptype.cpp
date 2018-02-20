
#include <includes.h>

#include "creature/creatureEnums.h"
//#include "items/item.h"
#include "items/itemtype.h"
//#include "items/clip.h"
#include "items/cliptype.h"
//#include "items/cliptype.h"
//own header currently inside includes.h
//own header

#include "common/stringconversion.h"
//for string conversion


#include <cursesAlternative.h>
#include <customMaps.h>
#include <constant_strings.h>
#include <gui_constants.h>
#include <set_color_support.h>
ClipType::ClipType(MCD_STR xmlstring) : ItemType(xmlstring), ammo_(1)
{
   CMarkup xml;
   xml.SetDoc(xmlstring);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem()) //Loop over all the elements inside the cliptype element.
   {
      std::string element=xml.GetTagName();
      if(element=="ammo")
         ammo_=atoi(xml.GetData());
      /*else
         errorlog << "Unknown element for clip type " << idname() << ": " << element << endl;*/
   }
}
