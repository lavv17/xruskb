/*
  xrus - keyboard switcher/indicator
  Copyright (c) 1996 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.
*/
/*__________________________________________________________________________
**
** File: xrus.h
**__________________________________________________________________________
*/

typedef struct
{
   Boolean  autolock;
   String   locker;
   int      timeout;
   Boolean  useBell;
   String   switchKeys;
   String   toRusKeys;
   String   toLatKeys;
   String   switchForOneKeys;
   String   alternateMappings;
   String   xmodmap;
   int      led;
   String   altMaps;
   Boolean  icon;
   Boolean  adjustModeButtons;
   Boolean  alwaysOnTop;
   Boolean  alwaysMapped;
   Boolean  capsLockEmu;
   int      capsLockLed;
   int      recheckTime;
   Boolean  noFork;
   Boolean  wm_icon;
   Boolean  wmaker_icon;
   String   xmodmap_program;
   String   title0;
   String   title1;
} XrusRec;

extern   XrusRec  AppData;

extern   char  DefaultLocker[];
extern   char  DefaultSwitchKeys[];
extern   char  DefaultToLatKeys[];
extern   char  DefaultToRusKeys[];
extern   char  DefaultSwitchForOneKeys[];

extern   Display        *disp;
extern   XtAppContext   app_context;

#define	 StartArgs()		count=0
#define	 AddArg(name,val)	(XtSetArg(args[count],(name),(val)),++count)

extern   char  *program;

extern   Widget   top_level;
extern   Widget   switch_button[2];

void  SetAlarm();
void  LockScreen();
void  run_xmodmap(const char *file);
void  load_map_delayed(const char *map);

void  SwitchKeyboard(int);
void  FixNewMode();

void  cleanup();  /* should be called before DestroyApplicationContext */

extern struct KeyCombination   SwitchKeys,ToLatKeys,ToRusKeys,SwitchForOneKeys;

extern   int   Mode,NewMode;

extern   int   count;
extern   Arg   args[];
