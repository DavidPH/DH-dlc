//DHLX
// Action Specials (ZDoom)

int AS_None                          =   0;
int AS_Polyobj_StartLine             =   1;
int AS_Polyobj_RotateLeft            =   2;
int AS_Polyobj_RotateRight           =   3;
int AS_Polyobj_Move                  =   4;
int AS_Polyobj_ExplicitLine          =   5;
int AS_Polyobj_MoveTimes8            =   6;
int AS_Polyobj_DoorSwing             =   7;
int AS_Polyobj_DoorSlide             =   8;
int AS_Line_Horizon                  =   9;
int AS_Door_Close                    =  10;
int AS_Door_Open                     =  11;
int AS_Door_Raise                    =  12;
int AS_Door_LockedRaise              =  13;
int AS_Door_Animated                 =  14;
int AS_Autosave                      =  15;
int AS_Transfer_WallLight            =  16;
int AS_Thing_Raise                   =  17;
int AS_StartConversation             =  18;
int AS_Thing_Stop                    =  19;
int AS_Floor_LowerByValue            =  20;
int AS_Floor_LowerToLowest           =  21;
int AS_Floor_LowerToNearest          =  22;
int AS_Floor_RaiseByValue            =  23;
int AS_Floor_RaiseToHighest          =  24;
int AS_Floor_RaiseToNearest          =  25;
int AS_Stairs_BuildDown              =  26;
int AS_Stairs_BuildUp                =  27;
int AS_Floor_RaiseAndCrush           =  28;
int AS_Pillar_Build                  =  29;
int AS_Pillar_Open                   =  30;
int AS_Stairs_BuildDownSync          =  31;
int AS_Stairs_BuildUpSync            =  32;
int AS_ForceField                    =  33;
int AS_ClearForceField               =  34;
int AS_Floor_RaiseByValueTimes8      =  35;
int AS_Floor_LowerByValueTimes8      =  36;
int AS_Floor_MoveToValue             =  37;
int AS_Ceiling_Waggle                =  38;
int AS_Teleport_ZombieChanger        =  39;
int AS_Ceiling_LowerByValue          =  40;
int AS_Ceiling_RaiseByValue          =  41;
int AS_Ceiling_CrushAndRaise         =  42;
int AS_Ceiling_LowerAndCrush         =  43;
int AS_Ceiling_CrushStop             =  44;
int AS_Ceiling_CrushRaiseAndStay     =  45;
int AS_Floor_CrushStop               =  46;
int AS_Ceiling_MoveToValue           =  47;
int AS_Sector_Attach3dMidtex         =  48;
int AS_GlassBreak                    =  49;
int AS_ExtraFloor_LightOnly          =  50;
int AS_Sector_SetLink                =  51;
int AS_Scroll_Wall                   =  52;
int AS_Line_SetTextureOffset         =  53;
int AS_Sector_ChangeFlags            =  54;
int AS_Line_SetBlocking              =  55;
int AS_Line_SetTextureScale          =  56;
int AS_Sector_SetPortal              =  57;
int AS_Sector_CopyScroller           =  58;

int AS_Plat_PerpetualRaise           =  60;
int AS_Plat_Stop                     =  61;
int AS_Plat_DownWaitUpStay           =  62;
int AS_Plat_DownByValue              =  63;
int AS_Plat_UpWaitDownStay           =  64;
int AS_Plat_UpByValue                =  65;
int AS_Floor_LowerInstant            =  66;
int AS_Floor_RaiseInstant            =  67;
int AS_Floor_MoveToValueTimes8       =  68;
int AS_Ceiling_MoveToValueTimes8     =  69;
int AS_Teleport                      =  70;
int AS_Teleport_NoFog                =  71;
int AS_ThrustThing                   =  72;
int AS_DamageThing                   =  73;
int AS_Teleport_NewMap               =  74;
int AS_Teleport_EndGame              =  75;
int AS_TeleportOther                 =  76;
int AS_TeleportGroup                 =  77;
int AS_TeleportInSector              =  78;

int AS_ACS_Execute                   =  80;
int AS_ACS_Suspend                   =  81;
int AS_ACS_Terminate                 =  82;
int AS_ACS_LockedExecute             =  83;
int AS_ACS_ExecuteWithResult         =  84;
int AS_ACS_LockedExecuteDoor         =  85;




int AS_Polyobj_OR_RotateLeft         =  90;
int AS_Polyobj_OR_RotateRight        =  91;
int AS_Polyobj_OR_Move               =  92;
int AS_Polyobj_OR_MoveTimes8         =  93;
int AS_Pillar_BuildAndCrush          =  94;
int AS_FloorAndCeiling_LowerByValue  =  95;
int AS_FloorAndCeiling_RaiseByValue  =  96;



int AS_Scroll_Texture_Left           = 100;
int AS_Scroll_Texture_Right          = 101;
int AS_Scroll_Texture_Up             = 102;
int AS_Scroll_Texture_Down           = 103;





int AS_Light_ForceLightning          = 109;
int AS_Light_RaiseByValue            = 110;
int AS_Light_LowerByValue            = 111;
int AS_Light_ChangeToValue           = 112;
int AS_Light_Fade                    = 113;
int AS_Light_Glow                    = 114;
int AS_Light_Flicker                 = 115;
int AS_Light_Strobe                  = 116;
int AS_Light_Stop                    = 117;
int AS_Plane_Copy                    = 118;
int AS_Thing_Damage                  = 119;
int AS_Radius_Quake                  = 120;
int AS_Line_SetIdentification        = 121;



int AS_Thing_Move                    = 125;

int AS_Thing_SetSpecial              = 127;
int AS_ThrustThingZ                  = 128;
int AS_UsePuzzleItem                 = 129;
int AS_Thing_Activate                = 130;
int AS_Thing_Deactivate              = 131;
int AS_Thing_Remove                  = 132;
int AS_Thing_Destroy                 = 133;
int AS_Thing_Projectile              = 134;
int AS_Thing_Spawn                   = 135;
int AS_Thing_ProjectileGravity       = 136;
int AS_Thing_SpawnNoFog              = 137;
int AS_Floor_Waggle                  = 138;
int AS_Thing_SpawnFacing             = 139;
int AS_Sector_ChangeSound            = 140;




int AS_Player_SetTeam                = 145; // (Skulltag)






int AS_Team_Score                    = 152; // (Skulltag)
int AS_Team_GivePoints               = 153; // (Skulltag)
int AS_Teleport_NoStop               = 154;


int AS_SetGlobalFogParameter         = 157; // (GZDoom)
int AS_FS_Execute                    = 158; // (GZDoom)
int AS_Sector_SetPlaneReflection     = 159; // (OpenGL)
int AS_Sector_Set3dFloor             = 160; // (OpenGL)
int AS_Sector_SetContents            = 161; // (OpenGL)







int AS_Generic_Crusher2              = 169;
int AS_Sector_SetCeilingScale2       = 170;
int AS_Sector_SetFloorScale2         = 171;
int AS_Plat_UpNearestWaitDownStay    = 172;
int AS_NoiseAlert                    = 173;
int AS_SendToCommunicator            = 174;
int AS_Thing_ProjectileIntercept     = 175;
int AS_Thing_ChangeTID               = 176;
int AS_Thing_Hate                    = 177;
int AS_Thing_ProjectileAimed         = 178;
int AS_ChangeSkill                   = 179;
int AS_Thing_SetTranslation          = 180;
int AS_Plane_Align                   = 181;
int AS_Line_Mirror                   = 182;
int AS_Line_AlignFloor               = 183;
int AS_Line_AlignCeiling             = 184;
int AS_Sector_SetRotation            = 185;
int AS_Sector_SetCeilingPanning      = 186;
int AS_Sector_SetFloorPanning        = 187;
int AS_Sector_SetCeilingScale        = 188;
int AS_Sector_SetFloorScale          = 189;
int AS_Static_Init                   = 190;
int AS_SetPlayerProperty             = 191;
int AS_Ceiling_LowerToHighestFloor   = 192;
int AS_Ceiling_LowerInstant          = 193;
int AS_Ceiling_RaiseInstant          = 194;
int AS_Ceiling_CrushRaiseAndStayA    = 195;
int AS_Ceiling_CrushAndRaiseA        = 196;
int AS_Ceiling_CrushAndRaiseSilentA  = 197;
int AS_Ceiling_RaiseByValueTimes8    = 198;
int AS_Ceiling_LowerByValueTimes8    = 199;
int AS_Generic_Floor                 = 200;
int AS_Generic_Ceiling               = 201;
int AS_Generic_Door                  = 202;
int AS_Generic_Lift                  = 203;
int AS_Generic_Stairs                = 204;
int AS_Generic_Crusher               = 205;
int AS_Plat_DownWaitUpStayLip        = 206;
int AS_Plat_PerpetualRaiseLip        = 207;
int AS_TranslucentLine               = 208;
int AS_Transfer_Heights              = 209;
int AS_Transfer_FloorLight           = 210;
int AS_Transfer_CeilingLight         = 211;
int AS_Sector_SetColor               = 212;
int AS_Sector_SetFade                = 213;
int AS_Sector_SetDamage              = 214;
int AS_Teleport_Line                 = 215;
int AS_Sector_SetGravity             = 216;
int AS_Stairs_BuildUpDoom            = 217;
int AS_Sector_SetWind                = 218;
int AS_Sector_SetFriction            = 219;
int AS_Sector_SetCurrent             = 220;
int AS_Scroll_Texture_Both           = 221;
int AS_Scroll_Texture_Model          = 222;
int AS_Scroll_Floor                  = 223;
int AS_Scroll_Ceiling                = 224;
int AS_Scroll_Texture_Offsets        = 225;
int AS_ACS_ExecuteAlways             = 226;
int AS_PointPush_SetForce            = 227;
int AS_Plat_RaiseAndStayTx0          = 228;
int AS_Thing_SetGoal                 = 229;
int AS_Plat_UpByValueStayTx          = 230;
int AS_Plat_ToggleCeiling            = 231;
int AS_Light_StrobeDoom              = 232;
int AS_Light_MinNeighbor             = 233;
int AS_Light_MaxNeighbor             = 234;
int AS_Floor_TransferTrigger         = 235;
int AS_Floor_TransferNumeric         = 236;
int AS_ChangeCamera                  = 237;
int AS_Floor_RaiseToLowestCeiling    = 238;
int AS_Floor_RaiseByValueTxTy        = 239;
int AS_Floor_RaiseByTexture          = 240;
int AS_Floor_LowerToLowestTxTy       = 241;
int AS_Floor_LowerToHighest          = 242;
int AS_Exit_Normal                   = 243;
int AS_Exit_Secret                   = 244;
int AS_Elevator_RaiseToNearest       = 245;
int AS_Elevator_MoveToFloor          = 246;
int AS_Elevator_LowerToNearest       = 247;
int AS_HealThing                     = 248;
int AS_Door_CloseWaitOpen            = 249;
int AS_Floor_Donut                   = 250;
int AS_FloorAndCeiling_LowerRaise    = 251;
int AS_Ceiling_RaiseToNearest        = 252;
int AS_Ceiling_LowerToLowest         = 253;
int AS_Ceiling_LowerToFloor          = 254;
int AS_Ceiling_CrushRaiseAndStaySilA = 255;



