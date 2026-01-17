META
{
  Parser = Menu;
  After = zUnionMenu.d;
  Namespace = WaterPhysics;
};

// Namespace = WaterPhysics
// Important !!!
// Supporting Unicode symbols.
// File encoding: UTF-8 (without BOM).

// ------ Constants ------
const int Start_PY  = 1400;
const int Title_PY  = 450;
const int Menu_DY   = 550;
// Text
const int Text_PX   = 400;
const int Text_SX   = 8000;
const int Text_SY   = 750;
const int Text_DY   = 120;
// Choice
const int Choice_PX = 6400;
const int Choice_SX = 1500;
const int Choice_SY = 350;
const int Choice_DY = 120;

const string MenuBackPic   = "UnionMenu_BackPic.tga";
const string ItemBackPic   = "";
const string ChoiceBackPic = "MENU_CHOICE_BACK.TGA";
const string FontSmall     = "font_old_10_white.tga";
const string FontBig       = "font_old_20_white.tga";

var int CurrentMenuItem_PY;

// ------ Prototypes ------
func void InitializeBackPicturesAndFonts()
{
  MenuBackPic   = MENU_BACK_PIC;
  ItemBackPic   = MENU_ITEM_BACK_PIC;
  ChoiceBackPic = MENU_CHOICE_BACK_PIC;
  FontSmall     = MENU_FONT_SMALL;
  FontBig       = MENU_FONT_DEFAULT;
};

prototype C_EMPTY_MENU_DEF(C_MENU)
{
  InitializeBackPicturesAndFonts();
  C_MENU_DEF();
  backpic    = MenuBackPic;
  items[0]   = "";
  items[100] = "Union_menuitem_back";
  flags      = flags | MENU_SHOW_INFO;
};

instance C_MENU_ITEM_TEXT_BASE(C_MENU_ITEM_DEF)
{
  backpic        = ItemBackPic;
  posx           = Text_PX;
  posy           = Start_PY;
  dimx           = Text_SX;
  dimy           = Text_SY;
  flags          = flags | IT_EFFECTS_NEXT;
  onselaction[0] = SEL_ACTION_UNDEF;
};

instance C_MENUITEM_CHOICE_BASE(C_MENU_ITEM_DEF)
{
  backpic  = ChoiceBackPic;
  type     = MENU_ITEM_CHOICEBOX;
  fontname = FontSmall;
  posx     = Choice_PX;
  posy     = Start_PY + Choice_DY;
  dimx     = Choice_SX;
  dimy     = Choice_SY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
};

instance C_MENUITEM_SLIDER_BASE(C_MENU_ITEM_DEF)
{
  backpic  = ChoiceBackPic;
  type     = MENU_ITEM_SLIDER;
  fontname = FontSmall;
  posx     = Choice_PX;
  posy     = Start_PY + Choice_DY;
  dimx     = Choice_SX;
  dimy     = Choice_SY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
};

instance MenuItem_Opt_Headline(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = Title_PY;
  dimx    = 8100;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = Str_GetLocalizedString(
    "WaterPhysics ННСТРОЙКИ",
    "WaterPhysics SETTINGS",
    "WaterPhysics EINSTELLUNGEN",
    "USTAWIENIA WaterPhysics"
  );
};

func int Act_OpenWebLink()
{
  Open_Link("https://github.com/Raster96/WaterPhysics-Union");
  return 0;
};

instance MenuItem_Opt_Open_Link(C_MENU_ITEM_DEF)
{
  C_MENU_ITEM_TEXT_BASE();
  posy += MENU_DY * 8;

  posx             = 64;
  onselaction[0]   = SEL_ACTION_UNDEF;
  oneventaction[1] = Act_OpenWebLink;
  flags            = flags | IT_TXT_CENTER;
  text[0]          = Str_GetLocalizedString(
    "Открыть ѝтраницу проекта",
    "Open project page",
    "Projektseite öffnen",
    "Otwórz stronę projektu"
  );

  text[1]          = "https://github.com/Raster96/WaterPhysics-Union";
};

// ------ Menu ------
instance MenuItem_Union_Auto_WaterPhysics(C_MENU_ITEM_UNION_DEF)
{
  text[0]          = "WaterPhysics";
  text[1] = Str_GetLocalizedString(
    "Наѝтройте параметры WaterPhysics",
    "Configure WaterPhysics settings",
    "WaterPhysics-Einstellungen konfigurieren",
    "Zmień ustawienia WaterPhysics"
  );
  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "WaterPhysics:Menu_Opt_WaterPhysics";
};

instance Menu_Opt_WaterPhysics(C_EMPTY_MENU_DEF)
{
  Menu_SearchItems("WaterPhysics:MENUITEM_OPT_WaterPhysics_*");
};

instance MenuItem_Opt_WaterPhysics_Headline(C_MENU_ITEM)
{
  MenuItem_Opt_Headline();
};

// ------ ItemEffects ------
instance MenuItem_Opt_WaterPhysics_ItemEffects(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 1;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "ItemEffects";
  text[1] = Str_GetLocalizedString(
    "Эффекты длѝ упавших предметов",
    "Effects for dropped items",
    "Effekte für fallen gelassene Gegenstände",
    "Efekty dla upuszczonych przedmiotów"
  );
};

instance MenuItem_Opt_WaterPhysics_ItemEffects_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "ItemEffects";
  onchgsetoptionsection = "WATER_PHYSICS";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ ArrowEffects ------
instance MenuItem_Opt_WaterPhysics_ArrowEffects(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 2;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "ArrowEffects";
  text[1] = Str_GetLocalizedString(
    "Эффекты длѝ ѝтрел и болтов",
    "Effects for arrows and bolts",
    "Effekte für Pfeile und Bolzen",
    "Efekty dla strzał i bełtów"
  );
};

instance MenuItem_Opt_WaterPhysics_ArrowEffects_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "ArrowEffects";
  onchgsetoptionsection = "WATER_PHYSICS";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ WeaponEffects ------
instance MenuItem_Opt_WaterPhysics_WeaponEffects(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 3;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "WeaponEffects";
  text[1] = Str_GetLocalizedString(
    "Эффекты длѝ ударов оружием",
    "Effects for weapon attacks",
    "Effekte für Waffenangriffe",
    "Efekty dla uderzeń bronią"
  );
};

instance MenuItem_Opt_WaterPhysics_WeaponEffects_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "WeaponEffects";
  onchgsetoptionsection = "WATER_PHYSICS";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ Open Project Page ------
instance MenuItem_Opt_WaterPhysics_Open_Link(C_MENU_ITEM)
{
  MenuItem_Opt_Open_Link();
};
