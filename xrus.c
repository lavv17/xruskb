/*
  xrus - keyboard switcher/indicator and autolock.
  Copyright (c) 1995-2001 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.
*/
/*__________________________________________________________________________
**
**    File:    xrus.c
**__________________________________________________________________________
*/

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xmd.h>
#include <X11/Xlibint.h>   /* for resource_mask */

#if TK==TK_MOTIF
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/MwmUtil.h>
#elif TK==TK_XAW
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#endif

#define  XK_MISCELLANY
#include <X11/keysymdef.h>

#ifndef SA_RESTART
#define SA_RESTART 0
#endif

#ifndef R_OK
# define R_OK 4
#endif

#include "xalloca.h"

#include "keycomb.h"
#include "xrus.h"
#include "props.h"
#include "menu.h"
#include "kbdstate.h"

#include "confpaths.h"

const char  AppClass[]="Xrus";

extern const char *DefaultResources;

XtActionsRec   Actions[]=
{
   {"XrusMenuPopup",XrusMenuPopup}
};

struct wincache_cell
{
   Window   win;
   time_t   time;
}
   window_cache[256];

XrusRec  AppData;

char  DefaultSwitchKeys[]="Shift_L+Shift_R";
char  DefaultToLatKeys[]="";
char  DefaultToRusKeys[]="";
char  DefaultSwitchForOneKeys[]="";
char  DefaultLocker[]="exec xlock -remote >/dev/null 2>&1";

char  LockerData[256];
char  SwitchKeysData[256];
char  ToLatKeysData[256];
char  ToRusKeysData[256];
char  SwitchForOneKeysData[256];

XtResource        resources[]=
{
/* {  name        class       type        size
         offset
         default_type default_addr              },*/
/* {  ----------- ----------- ----------- ------------
         -----------------------------------
         ----------- -------------------------  },*/
   {  "autolock", "Autolock", XtRBoolean, sizeof(Boolean),
         XtOffsetOf(XrusRec,autolock),
         XtRImmediate, (XtPointer)False         },
   {  "locker",   "Locker",   XtRString,  sizeof(String),
         XtOffsetOf(XrusRec,locker),
         XtRString,  DefaultLocker              },
   {  "timeout",  "Timeout",  XtRInt,     sizeof(int),
         XtOffsetOf(XrusRec,timeout),
         XtRImmediate,  (XtPointer)30           },
   {  "useBell", "UseBell",   XtRBoolean, sizeof(int),
         XtOffsetOf(XrusRec,useBell),
         XtRImmediate, (XtPointer)False         },
   {  "switchKeys","SwitchKeys",XtRString,sizeof(String),
         XtOffsetOf(XrusRec,switchKeys),
         XtRString,  DefaultSwitchKeys          },
   {  "toRusKeys","ToRusKeys",XtRString,  sizeof(String),
         XtOffsetOf(XrusRec,toRusKeys),
         XtRString,  DefaultToRusKeys           },
   {  "toLatKeys","ToLatKeys",XtRString,  sizeof(String),
         XtOffsetOf(XrusRec,toLatKeys),
         XtRString,  DefaultToLatKeys           },
   {  "switchForOneKeys","SwitchForOneKeys",XtRString,sizeof(String),
         XtOffsetOf(XrusRec,switchForOneKeys),
         XtRString,  DefaultSwitchForOneKeys    },
   {  "xmodmap",  "Xmodmap",  XtRString,  sizeof(String),
         XtOffsetOf(XrusRec,xmodmap),
         XtRString,  ""                         },
   {  "led",      "Led",      XtRInt,     sizeof(int),
         XtOffsetOf(XrusRec,led),
         XtRImmediate,  (XtPointer)0            },
   {  "altMaps",  "AltMaps",  XtRString,  sizeof(String),
         XtOffsetOf(XrusRec,altMaps),
         XtRString,  ""                         },
   {  "icon",     "Icon",     XtRBoolean, sizeof(Boolean),
         XtOffsetOf(XrusRec,icon),
         XtRImmediate,  (XtPointer)True         },
   {  "adjustModeButtons","AdjustModeButtons",XtRBoolean,sizeof(Boolean),
         XtOffsetOf(XrusRec,adjustModeButtons),
         XtRImmediate,  (XtPointer)True         },
   {  "alwaysOnTop","AlwaysOnTop",XtRBoolean,sizeof(Boolean),
         XtOffsetOf(XrusRec,alwaysOnTop),
         XtRImmediate,  (XtPointer)False        },
   {  "alwaysMapped","AutoMap",XtRBoolean,sizeof(Boolean),
         XtOffsetOf(XrusRec,alwaysMapped),
         XtRImmediate,  (XtPointer)False	},
   {  "capsLockEmu","CapsLockEmu",XtRBoolean,sizeof(Boolean),
         XtOffsetOf(XrusRec,capsLockEmu),
         XtRImmediate,  (XtPointer)False        },
   {  "capsLockLed","CapsLockLed",XtRInt, sizeof(int),
         XtOffsetOf(XrusRec,capsLockLed),
         XtRImmediate,  (XtPointer)0            },
   {  "recheckTime","RecheckTime",XtRInt, sizeof(int),
         XtOffsetOf(XrusRec,recheckTime),
         XtRImmediate,  (XtPointer)1500         },
   {  "noFork",   "NoFork",   XtRBoolean, sizeof(Boolean),
         XtOffsetOf(XrusRec,noFork),
         XtRImmediate,  (XtPointer)False        },
   {  "wmIcon",   "WMIcon",   XtRBoolean, sizeof(Boolean),
            XtOffsetOf(XrusRec,wm_icon),
            XtRImmediate,  (XtPointer)False     },
   {  "wMaker",   "WMaker",   XtRBoolean, sizeof(Boolean),
            XtOffsetOf(XrusRec,wmaker_icon),
            XtRImmediate,  (XtPointer)False     },
   {  "xmodmapProgram", "XmodmapProgram", XtRString, sizeof(String),
            XtOffsetOf(XrusRec,xmodmap_program),
            XtRString,  BINDIR "/xrusmodmap"    },
   {  "title0",	  "Title0",   XtRString,  sizeof(String),
            XtOffsetOf(XrusRec,title0),
            XtRString,  (XtPointer)0		},
   {  "title1",	  "Title1",   XtRString,  sizeof(String),
            XtOffsetOf(XrusRec,title1),
            XtRString,  (XtPointer)0		},
   {  "perWindow","PerWindow",XtRBoolean, sizeof(Boolean),
            XtOffsetOf(XrusRec,per_window_state),
            XtRImmediate,  (XtPointer)False     },
   {  "titlePerWindow0","TitlePerWindow0",XtRString,sizeof(String),
            XtOffsetOf(XrusRec,titlePerWindow0),
            XtRString,  (XtPointer)0		},
   {  "titlePerWindow1","TitlePerWindow1",XtRString,sizeof(String),
            XtOffsetOf(XrusRec,titlePerWindow1),
            XtRString,  (XtPointer)0		},
   {  "occupyAllDesks","OccupyAllDesks",XtRBoolean, sizeof(Boolean),
            XtOffsetOf(XrusRec,occupyAllDesks),
            XtRImmediate,  (XtPointer)True      },
   {  "keyLog",   "KeyLog",   XtRString,  sizeof(String),
            XtOffsetOf(XrusRec,keylog_file),
            XtRString,  (XtPointer)0            },
};

XrmOptionDescRec  options[]=
{
/* {  option,       specifier,     argKind,          value  },*/
/* {  ------------- -------------- ----------------- ------ },*/
   {  "-autolock",  "*autolock",   XrmoptionNoArg,   "true" },
   {  "+autolock",  "*autolock",   XrmoptionNoArg,   "false"},
   {  "-locker",    "*locker",     XrmoptionSepArg,  NULL   },
   {  "-timeout",   "*timeout",    XrmoptionSepArg,  NULL   },
   {  "-bell",      "*useBell",    XrmoptionNoArg,   "true" },
   {  "+bell",      "*useBell",    XrmoptionNoArg,   "false"},
   {  "-led",       "*led",        XrmoptionSepArg,  "0"    },
   {  "+led",       "*led",        XrmoptionNoArg,   "0"    },
   {  "-icon",      "*icon",       XrmoptionNoArg,   "true" },
   {  "+icon",      "*icon",       XrmoptionNoArg,   "false"},
   {  "-nofork",    "*noFork",     XrmoptionNoArg,   "true" },
   {  "+nofork",    "*noFork",     XrmoptionNoArg,   "false"},
   {  "-fork",      "*noFork",     XrmoptionNoArg,   "false"},
   {  "+fork",      "*noFork",     XrmoptionNoArg,   "true" },
   {  "-wmicon",    "*wmIcon",     XrmoptionNoArg,   "true" },
   {  "-wmaker",    "*wMaker",     XrmoptionNoArg,   "true" },
   {  "-perwindow", "*perWindow",  XrmoptionNoArg,   "true" },
   {  "-keylog",    "*keyLog",     XrmoptionSepArg,  NULL   },
};

Display        *disp;
Atom           wm_delete_window;
Atom           wm_protocols;
XtAppContext   app_context;

#define	StartArgs()		count=0
#define	AddArg(name,val)	(XtSetArg(args[count],(name),(val)),++count)

Arg   args[32];
int   count;

char  *program;

struct KeyCombination   SwitchKeys,ToLatKeys,ToRusKeys,SwitchForOneKeys;

#define ShouldUseLat()        IsModifierPressed(ControlMapIndex)

enum
{
   MODE=1,
   RUS=1,
   LAT=0,
   TEMP_LAT=2,
   CAPSLOCK_ON=4,
   FOR_ONE=8,
   SAVE_MASK=MODE
};
int   Mode=LAT;
int   NewMode;

Widget   top_level=NULL;
Window   our_host_window=0;
#if TK!=TK_NONE
Widget   form_w;
Widget   switch_button[2];
#endif

Atom  type_ret;
int   format_ret;
unsigned long nitems_ret,bytes_after_ret;
unsigned char *prop;

static Atom kbd_state_atom;
static Window focus_window;
static int    revert_to=RevertToNone;
static Window last_top_level_focus_window;
static int    last_top_level_revert_to=RevertToNone;
static Bool   pushed=False;

static int xrus_check=1;

static int keylog_fd=-1;
static int keylog_pos=0;
static time_t keylog_time=0;

void  SetAlarm();

int   (*old_error_handler)(Display *d,XErrorEvent *ev);

pid_t LockerRunning=0;

time_t alarm_expected;

static int MappingNotifyIgnoreCount=0;
static XtIntervalId MappingNotifyTimeout=-1;

#if TK!=TK_NONE
static void ShowSwitchButton(void);
static void SetTitle(int);
#else
# define ShowSwitchButton()
# define SetTitle(x)
#endif

#define MaxLetterKeySym 0x1000
#define NationalKeySym(ks) (ks>=128 && ks<MaxLetterKeySym)
#define AsciiKeySym(ks) (ks>0 && ks<128)

XID client_id_mask;
#define ClientId(w) ((w)&client_id_mask)

/*________________________________________________________________________
*/


/* SwappableKey, modified by Tobias Ernst:

   Now returns 0 if this key is not to be swapped, non-zero otherwise. The
   non-zero number indicates how many keysyms are grouped into one block. These
   are two in a standard English-Russian setup, but could be four e.g. in a
   German-Russian setup (keysyms 0 and 1 for Latin letters plus keysyms 2 and 3
   for "AltGr" key combinations in a group, and keysyms 4 and 5 for cyrillic
   letters and 6 and 7 with NoSymbol in the other group.) */

static int SwappableKey(KeySym *k)
{
    int i, j = 0;

    for (i = 2; i <= (keysyms_per_keycode / 2); i += 2)
    {
        if (k[i] > 0 && k[i]<MaxLetterKeySym)
        {
            j = i;
        }
    }

    for (i = 0; i < 2*j; i++)
    {
        if (!(k[i] >= 0 && k[i] < MaxLetterKeySym))
        {
            /* nonprintable character found other than NoSymbol found,
               and we don't want to swap this */
            return 0;
        }
    }

    return j;
}

static int CapsLockableKey(KeySym *k)
{
   KeySym k0=k[0],k1=k[1];
   return((NationalKeySym(k0) && NationalKeySym(k1))
      || (((k0>='a' && k0<='z') || (k0>='A' && k0<='Z'))
          &&
          ((k1>='a' && k1<='z') || (k1>='A' && k1<='Z'))));
}

void  FixNewMode()
{
   NewMode&=~(TEMP_LAT);
   if(ShouldUseLat())
      NewMode|=TEMP_LAT;
}

static void SetTitleIndicator(Window w,int mode)
{
   XTextProperty prop;
   char *new_title, *append, *old;
   int old_len, append_len, title_len;

   if((!AppData.titlePerWindow0    || !AppData.titlePerWindow1)
   || (!AppData.titlePerWindow0[0] && !AppData.titlePerWindow1[0]))
      return;

   if(!XGetWMName(disp,w,&prop) || prop.format!=8)
      return;

   mode&=MODE;

   append=mode?AppData.titlePerWindow1:AppData.titlePerWindow0;
   old   =mode?AppData.titlePerWindow0:AppData.titlePerWindow1;
   new_title=alloca(prop.nitems+256);
   old_len=strlen(old);
   append_len=strlen(append);

   memcpy(new_title,prop.value,title_len=prop.nitems);
   new_title[title_len]=0; /* for strcmp below */
   XFree(prop.value);

   if(title_len>old_len
   && !strcmp(new_title+title_len-old_len,old))
      title_len-=old_len;
   else if(title_len>append_len
   && !strcmp(new_title+title_len-append_len,append))
      return;

   if(title_len==0)
   {
      strcpy(new_title,"untitled");
      title_len=strlen(new_title);
   }
   if(new_title[title_len-1]!=' ')
      new_title[title_len++]=' ';
   strcpy(new_title+title_len,append);
   title_len+=append_len;

   prop.nitems=title_len;
   prop.value=new_title;
   XSetWMName(disp,w,&prop);
}

static int GetKbdState(Window w)
{
   long *prop=0;
   if(XGetWindowProperty(disp,w,kbd_state_atom,0L,256L,0,XA_CARDINAL,
         &type_ret,&format_ret,&nitems_ret,&bytes_after_ret,(void*)&prop)
        ==Success && type_ret==XA_CARDINAL && nitems_ret==1 && prop)
   {
      int ret=(prop[0]&SAVE_MASK);
      XFree(prop);
      return ret;
   }
   return -1;
}

static XtIntervalId SetTitleTimeout=-1;
static void SetTitleIndicatorOnTimeout(XtPointer closure,XtIntervalId *id)
{
   Window w=(Window)closure;
   int mode=GetKbdState(w);
   if(mode!=-1)
      SetTitleIndicator(w,mode);
   SetTitleTimeout=-1;
}

static Bool window_is_top_level(Window w)
{
   int num,i;
   Atom *list=XListProperties(disp,w,&num);

   if(!list)
      return False;

   for(i=0; i<num; i++)
   {
      if(list[i]==XA_WM_NAME
      || list[i]==XA_WM_CLASS
      || list[i]==XA_WM_COMMAND)
      {
         XFree(list);
         return True;
      }
   }
   XFree(list);
   return False;
}

static Bool window_is_ours(Window w)
{
   if(!top_level)
      return False;
   return (ClientId(w)==ClientId(XtWindow(top_level)));
   /* Also possibe:
   return (XtWindowToWidget(disp,w)!=NULL); */
}

static void SaveModeForWindow(Window w)
{
   long m=Mode&SAVE_MASK;
   if(!AppData.per_window_state)
      return;
   if(w==0 || !window_is_top_level(w))
      w=last_top_level_focus_window;
   if(w==0)
      return;
   SetTitleIndicator(w,Mode);
   XChangeProperty(disp,w,kbd_state_atom,XA_CARDINAL,32,
                   PropModeReplace,(void*)&m,1);
}

void  SwitchKeyboard(int to)
{
   KeySym   tmp;
   KeySym   *keymap;
   int      i, j, groupwidth;
   XKeyboardControl  kbd_control;
   int   from_mode,to_mode,from_caps,to_caps;
   int   changed=0;
   int   from=Mode;

   if(to==Mode)
      return;

   keymap=GetKeymap();
   if(keymap==0)
      return;

   if(keysyms_per_keycode<4)   /* should not happen */
      return;

   from_mode=((Mode&TEMP_LAT)?LAT:(Mode&FOR_ONE)?((Mode&MODE)^MODE):Mode&MODE);
   to_mode=  ((to  &TEMP_LAT)?LAT:(to  &FOR_ONE)?((to  &MODE)^MODE):to  &MODE);

   from_caps=((Mode&TEMP_LAT)?0:Mode&CAPSLOCK_ON);
   to_caps=((to&TEMP_LAT)?0:to&CAPSLOCK_ON);

   for(i=(max_keycode-min_keycode)*keysyms_per_keycode;
       i>=0; i-=keysyms_per_keycode)
   {
      groupwidth = SwappableKey(keymap + i);

      if(from_mode!=to_mode && groupwidth!=0)
      {
         for (j=0; j < groupwidth; j++)
         {
             tmp=keymap[i+j];
             keymap[i+j]=keymap[i+j+groupwidth];
             keymap[i+j+groupwidth]=tmp;
         }
         changed=1;
      }

      if(AppData.capsLockEmu && from_caps!=to_caps)
      {
         if(CapsLockableKey(keymap+i))
         {
            tmp=keymap[i];
            keymap[i]=keymap[i+1];
            keymap[i+1]=tmp;
            changed=1;
         }
         if(groupwidth && CapsLockableKey(keymap+i+groupwidth))
         {
            tmp=keymap[i+groupwidth];
            keymap[i+groupwidth]=keymap[i+groupwidth+1];
            keymap[i+groupwidth+1]=tmp;
            changed=1;
         }
      }
   }
   if(changed)
   {
      XChangeKeyboardMapping(disp,min_keycode,keysyms_per_keycode,
        		     keymap,max_keycode-min_keycode+1);
      MappingNotifyIgnoreCount++;

      /* SGI X server goes crazy without this */
      XSetModifierMapping(disp,GetModifiers());
   }

   if(AppData.capsLockEmu && from_caps!=to_caps)
   {
      if(AppData.capsLockLed>0 && AppData.capsLockLed<=32)
      {
         kbd_control.led_mode=(NewMode&CAPSLOCK_ON?LedModeOn:LedModeOff);
         kbd_control.led=AppData.capsLockLed;
         XChangeKeyboardControl(disp,KBLed|KBLedMode,&kbd_control);
      }
   }

   if((Mode&MODE)!=(to&MODE))
   {
      if(AppData.led>0 && AppData.led<=32)
      {
         kbd_control.led_mode=((to&MODE)==RUS?LedModeOn:LedModeOff);
         kbd_control.led=AppData.led;
         XChangeKeyboardControl(disp,KBLed|KBLedMode,&kbd_control);
      }

      if(AppData.useBell)
         XBell(disp,30);
   }
   Mode=to;
   if((from&SAVE_MASK) != (to&SAVE_MASK))
      SaveModeForWindow(focus_window);
   ShowSwitchButton();
}

static void SwitchKeyboardForWindow(Window w)
{
   int mode;
   Window old_last_top_level_focus_window=last_top_level_focus_window;
   Window old_focus_window=focus_window;
   if(w==0)
      return;
   focus_window=0;
   last_top_level_focus_window=0;
   mode=GetKbdState(w);
   if(mode==-1)
      mode=0;
   NewMode=(Mode&~SAVE_MASK)|mode;

   NewMode&=~FOR_ONE;
   FixNewMode();
   SwitchKeyboard(NewMode);

   if(prop)
      XFree(prop);

   focus_window=old_focus_window;
   last_top_level_focus_window=old_last_top_level_focus_window;
}
static void InitCapsLockEmu()
{
   KeyCode  *lock;
   int i,changed=0;
   XModifierKeymap *mod;

   if(!AppData.capsLockEmu)
      return;

   mod=XGetModifierMapping(disp);
   lock=mod->modifiermap
        +LockMapIndex*mod->max_keypermod;
   for(i=0; i<mod->max_keypermod; i++)
   {
      if(lock[i])
      {
         changed=1;
         lock[i]=0;
      }
   }
   if(changed && XSetModifierMapping(disp,mod)!=MappingSuccess)
   {
      fprintf(stderr,"%s: can't remove Caps_Lock from lock -- CapsLock emulation disabled\n",program);
      AppData.capsLockEmu=False;
   }
   XFreeModifiermap(mod);
}

static const char *load_map=0;
void load_map_delayed(const char *map)
{
   if(MappingNotifyIgnoreCount==0)
      run_xmodmap(map);
   else
      load_map=map;
}

#if TK!=TK_NONE
static void RaiseButton()
{
   if(AppData.wm_icon || AppData.wmaker_icon || top_level==0)
      return;
   XRaiseWindow(disp,XtWindow(top_level));
}

static XtIntervalId raise_tmout=-1;
static void RaiseButtonOnTimeout(XtPointer closure,XtIntervalId *id)
{
   (void)closure; (void)id;
   raise_tmout=-1;
   RaiseButton();
}

static void SetTitle(int n)
{
   const char *title=(n==0 ? AppData.title0 : AppData.title1);
   if(title==0)
      title="xrus";
   XtSetArg(args[0],XtNtitle,title);
   XtSetArg(args[1],XtNiconName,title);
   XtSetValues(top_level,args,2);
}

/* shows proper indicator button */
static void ShowSwitchButton(void)
{
   int   to_manage=-1;

   if(!switch_button[0] || !switch_button[1])
      return;  /* no button to show ! */

   if((Mode&MODE)==RUS && !XtIsManaged(switch_button[1]))
      to_manage=1;
   else if((Mode&MODE)==LAT && !XtIsManaged(switch_button[0]))
      to_manage=0;

   if(to_manage!=-1)
   {
      XtSetArg(args[0],XtNallowShellResize,False);
      XtSetValues(top_level,args,1);

      XtUnmanageChild(switch_button[1-to_manage]);

      XtSetArg(args[0],XtNallowShellResize,True);
      XtSetValues(top_level,args,1);

      XtManageChild(switch_button[to_manage]);

      SetTitle(to_manage);

      RaiseButton();
   }
}

void  UnmapHandler(Widget w,XtPointer closure,XEvent *ev,Boolean *cont)
{
   *cont=True;
   if(ev->type==UnmapNotify && AppData.alwaysMapped && AppData.icon
   && !AppData.wm_icon && !AppData.wmaker_icon)
   {
      XtMapWidget(top_level);
   }
   else if(ev->type==MapNotify)
   {
      if(AppData.wmaker_icon)
         XWithdrawWindow(disp,XtWindow(top_level),DefaultScreen(disp));
      else if(AppData.wm_icon)
         XIconifyWindow(disp,XtWindow(top_level),DefaultScreen(disp));
   }
}

void  VisibilityChange(Widget w,XtPointer closure,XEvent *ev,Boolean *cont)
{
   (void)w; (void)closure;
   *cont=True;

   if(ev->type==VisibilityNotify
   && ev->xvisibility.state!=VisibilityUnobscured
   && AppData.alwaysOnTop && raise_tmout==-1)
   {
      raise_tmout=XtAppAddTimeOut(app_context,1000,
                        (XtTimerCallbackProc)RaiseButtonOnTimeout,NULL);
   }
}

XtIntervalId pushed_tmout=-1;
static void drop_pushed()
{
   pushed_tmout=-1;
   pushed=False;
}
static void PerformSwitch(void)
{
   NewMode=(Mode^MODE)&~FOR_ONE;
   FixNewMode();
   SwitchKeyboard(NewMode);
   pushed=True;
   if(pushed_tmout!=-1)
      XtRemoveTimeOut(pushed_tmout);
   pushed_tmout=XtAppAddTimeOut(app_context,2000,
                        (XtTimerCallbackProc)drop_pushed,NULL);
}
#endif /* TK!=TK_NONE */

/* a window is in cache => we have requested all needed input from the window */
/* returns a boolean indicating the window was in cache
   and adds it to cache if it was not */
int   AddToWindowCache(Window w)
{
   time_t   t=time(NULL);
   time_t   best_time=t;
   int      best_cell=-1;
   int      i;

   for(i=0; i<XtNumber(window_cache); i++)
   {
      if(window_cache[i].win==w && window_cache[i].time!=0)
      {
         window_cache[i].time=t;
         return(0);
      }
      if(best_cell==-1 || best_time>window_cache[i].time || window_cache[i].win==0)
      {
         best_cell=i;
         best_time=window_cache[i].time;
      }
   }
   window_cache[best_cell].time=t;
   window_cache[best_cell].win=w;
   return(1);
}

/* deletes a window from the cache */
void  DelWindow(Window w)
{
   int   i;

   for(i=0; i<XtNumber(window_cache); i++)
   {
      if(window_cache[i].win==w)
      {
         window_cache[i].win=0;
         window_cache[i].time=0;
      }
   }
}

static void AddWindow(Window w,int tmout,int max_depth);

static void AddWindowOnTimeout(XtPointer closure,XtIntervalId *id)
{
   (void)id;
   DelWindow((Window)closure);
   AddWindow((Window)closure,False,1000000);
}

static void XrusCheck(Window w)
{
   int class_len=strlen(AppClass);
   XTextProperty prop;
   unsigned char *zero;

   if(!xrus_check)
      return;

   if(!XGetTextProperty(disp,w,&prop,XA_WM_CLASS))
      return;

   if(prop.format!=8)
      return;

   zero=memchr(prop.value,0,prop.nitems);
   if(zero && zero+1+class_len<=prop.value+prop.nitems
   && !memcmp(zero+1,AppClass,class_len)
   && (zero+1+class_len==prop.value+prop.nitems
       || zero[1+class_len]==0))
   {
      fprintf(stderr,"%s: xrus already runs on the display\n",program);
      exit(1);
   }
   XFree(prop.value);
}

static void AddWindow(Window w,int tmout,int max_depth)
{
   XWindowAttributes wa;
   long mask;
   Window root1,parent,*children;
   unsigned children_num,i;
   Bool all_events_allowed=False;

   if(max_depth<1)
      return;

   if(!AddToWindowCache(w)) /* if the window is in cache, skip it */
      return;

   XrusCheck(w);

   if(!XQueryTree(disp,w,&root1,&parent,&children,&children_num))
   {
      DelWindow(w);
      return;
   }

   if(w==focus_window)
      all_events_allowed=True;

   if(!all_events_allowed && !window_is_ours(w))
   {
      /* if it is not our own window, unselect any events,
         so that all_event_masks has only event bits of other apps. */
      /* ...unless it is possible to select any events. */
      XSelectInput(disp,w,0);
   }

   if(!XGetWindowAttributes(disp,w,&wa))
   {
      DelWindow(w);
      return;
   }

   mask=wa.your_event_mask;

   /* we want to be notified of subwindows creation/destruction etc */
   mask|=SubstructureNotifyMask;

   /* keep track of focus window */
   mask|=FocusChangeMask;

   /* we request events only if
      1. the window owner has already requested this kind of events
      2. the window is blocking the events from propagation
      3. parent window belongs to a different client.
   */
   mask|=(wa.all_event_masks|wa.do_not_propagate_mask)
         &(KeyPressMask|KeyReleaseMask|PointerMotionMask);

   if(all_events_allowed)
      mask|=(KeyPressMask|KeyReleaseMask|PointerMotionMask|PropertyChangeMask);

   /* we always want both Press and Release events */
   if(mask&(KeyPressMask|KeyReleaseMask))
      mask|=(KeyPressMask|KeyReleaseMask);

   if(!(mask&KeyPressMask))
      DelWindow(w);  /* we'll check input mask later */

   XSelectInput(disp,w,mask);

   for(i=0; i<children_num; i++)
   {
      AddWindow(children[i],False,max_depth-1);
      if(w==focus_window && window_is_top_level(children[i]))
      {
         last_top_level_focus_window=children[i];
         last_top_level_revert_to=revert_to;
      }
   }
   XFree(children);

   if(tmout)
   {
      XtAppAddTimeOut(app_context,AppData.recheckTime,AddWindowOnTimeout,
                      (XtPointer)w);
   }
}

void  SwitchKeysFire(void)
{
   NewMode^=MODE;
   NewMode&=~FOR_ONE;
}
void  ToLatKeysFire(void)
{
   NewMode&=~MODE;
   NewMode&=~FOR_ONE;
}
void  ToRusKeysFire(void)
{
   NewMode|=MODE;
   NewMode&=~FOR_ONE;
}
void  SwitchForOneKeysFire(void)
{
   NewMode^=FOR_ONE;
}

void  CheckKeymap()
{
   static int first_time=1;

   int      ok_rus=0;
   int      ok_lat=0;
   int      i,groupwidth;
   KeySym   *keymap=GetKeymap();

   if(keymap==0)
   {
      Mode=LAT;
      exit(1);
   }

   if(keysyms_per_keycode>=4)
   {
      for(i=(max_keycode-min_keycode)*keysyms_per_keycode;
          i>=0; i-=keysyms_per_keycode)
      {
         if((groupwidth=SwappableKey(keymap+i)) > 0)
         {
            ok_lat+=(NationalKeySym(keymap[i+groupwidth]) && NationalKeySym(keymap[i+groupwidth+1]));
            ok_rus+=(NationalKeySym(keymap[i+0]) && NationalKeySym(keymap[i+1]));
         }
      }
   }

   if(ok_rus==ok_lat)
   {
      fprintf(stderr,"%s: key mapping is not in proper coding, exiting.\n",program);
      if(first_time)
      {
         fprintf(stderr,"   You might probably want to load a localized modmap before starting xrus.\n"
                        "   Alternatively, you can specify modmap file name on the command line,\n"
                        "   like this:   ./xrus jcuken-koi8.xmm\n");
      }
      else
      {
         fprintf(stderr,"   This means your modmap has changed to single-mode one.\n"
                        "   If you were doing xmodmap <file> at the time, check your mapping in\n"
                        "   the file. For normal xrus work it is required that modmap has both\n"
                        "   national and latin letters\n");
      }
      Mode=LAT;
      exit(1);
   }

   NewMode=Mode=(Mode&~(MODE))|(ok_rus>ok_lat?RUS:LAT);
   FixNewMode();
   SwitchKeyboard(NewMode);

   ShowSwitchButton();

   first_time=0;
}

static Window set_focus;
static int set_revert_to;
XtIntervalId focus_tmout=-1;
static void SetFocusOnTimeout()
{
   if(set_focus && pushed)
      XSetInputFocus(disp,set_focus,set_revert_to,CurrentTime);
   focus_tmout=-1;
}
static void NewFocus(Window w,int rt)
{
   if(w==focus_window)
      return;
   if((top_level && w==XtWindow(top_level)) || w==our_host_window)
   {
      if(focus_tmout!=-1)
         XtRemoveTimeOut(focus_tmout);
      set_focus=last_top_level_focus_window;
      set_revert_to=last_top_level_revert_to;
      focus_tmout=XtAppAddTimeOut(app_context,200,
                        (XtTimerCallbackProc)SetFocusOnTimeout,NULL);
      return;
   }
   if(window_is_top_level(w))
   {
      last_top_level_focus_window=w;
      last_top_level_revert_to=rt;
      if(AppData.per_window_state)
         SwitchKeyboardForWindow(w);
   }
   focus_window=w;
   revert_to=rt;
   DelWindow(w);
   AddWindow(w,False,1);
}

static void QueryFocus()
{
   Window w=0;
   int revert_to=0;
   XGetInputFocus(disp,&w,&revert_to);
   if(w)
      NewFocus(w,revert_to);
}

static
void  MappingNotifyTimeoutHandler(XtPointer closure,XtIntervalId *id)
{
   (void)closure; (void)id;
   MappingNotifyTimeout=-1;
   CheckKeymap();
   if(focus_window==0)
      QueryFocus();
}

void LogKeyStr(const char *string,int len)
{
   const char *nl;

   if(len==-1)
      len=strlen(string);

   nl=memchr(string,'\n',len);
   if(nl)
      keylog_pos=len-(nl-string+1);
   else
      keylog_pos+=len;

   if(keylog_pos>75)
   {
      write(keylog_fd,"\n",1);
      keylog_pos=nl?len-(nl-string+1):len;
   }

   write(keylog_fd,string,len);
}

void  LogKey(KeySym ks,char *string,int len)
{
   time_t t;

   if(keylog_fd==-1)
      return;

   if(len>0)
   {
      if(string[0]=='\r')
      {
         strcpy(string,"<CR>\n");
         len=-1;
      }
      else if((unsigned char)string[0] < 32)
      {
         sprintf(string,"^%c",string[0]+'@');
         len=2;
      }
   }
   else
   {
      sprintf(string,"<%.60s>",XKeysymToString(ks));
      len=-1;
   }
   if(time(&t)-keylog_time>=60)
   {
      char ts[64];
      strftime(ts,sizeof(ts),"<%Y-%m-%d %H:%M:%S>",localtime(&t));
      LogKeyStr(ts,-1);
      keylog_time=t;
   }
   LogKeyStr(string,len);
}

void  MainLoop(void)
{
   XEvent   ev;
   KeySym   ks;
   int      was_caps_locked;
   Bool     fired;
   int      len;
   char     string[64];

   for(;;)
   {
      alloca(0);  /* garbage collection */

      if(app_context==NULL)
         return;
      XtAppNextEvent(app_context,&ev);

      was_caps_locked=IsKeySymPressed(XK_Caps_Lock);

      XtDispatchEvent(&ev);

      KeepTrackOfKeyboard(&ev);

      if(NewMode!=Mode)
      {
         FixNewMode();
         SwitchKeyboard(NewMode);
      }

      switch(ev.type)
      {
      case(MapNotify):
         AddWindow(ev.xmap.window,True,1000000);
         QueryFocus();
         break;
      case(ReparentNotify):
         /* the structure could change under us - re-add the window */
         AddWindow(ev.xreparent.window,True,1000000);
         if(ev.xreparent.window==XtWindow(top_level))
            our_host_window=ev.xreparent.parent;
         break;
      case(DestroyNotify):
         DelWindow(ev.xdestroywindow.window);
         break;
      case(KeyPress):
         SetAlarm();

         ks=XLookupKeysym(&ev.xkey,0);
         len=XLookupString(&ev.xkey,string,sizeof(string),0,0);

         NewMode=Mode;

         if(ks==XK_Caps_Lock && !was_caps_locked)
            NewMode^=CAPSLOCK_ON;

         KeyCombinationProcessKeyPress(&SwitchKeys,ks);
         KeyCombinationProcessKeyPress(&ToRusKeys,ks);
         KeyCombinationProcessKeyPress(&ToLatKeys,ks);
         KeyCombinationProcessKeyPress(&SwitchForOneKeys,ks);

         if(len>0)
            NewMode&=~FOR_ONE;

         LogKey(ks,string,len);

         FixNewMode();
         SwitchKeyboard(NewMode);

         break;
      case(KeyRelease):
         SetAlarm();

         ks=XLookupKeysym(&ev.xkey,0);

         NewMode=Mode;

         fired=False;
         fired|=KeyCombinationProcessKeyRelease(&SwitchKeys,ks,!fired);
         fired|=KeyCombinationProcessKeyRelease(&ToLatKeys,ks,!fired);
         fired|=KeyCombinationProcessKeyRelease(&ToRusKeys,ks,!fired);
         fired|=KeyCombinationProcessKeyRelease(&SwitchForOneKeys,ks,!fired);

         FixNewMode();
         SwitchKeyboard(NewMode);

         break;
      case(MotionNotify):
      case(ButtonPress):
      case(ButtonRelease):
         SetAlarm();
         break;
      case(MappingNotify):
         if(ev.xmapping.request==MappingKeyboard)
         {
            /* workaround with timeout for xmodmap bug (too many events) */
            if(MappingNotifyIgnoreCount==0)
            {
               if(MappingNotifyTimeout!=-1)
                  XtRemoveTimeOut(MappingNotifyTimeout);
               MappingNotifyTimeout=XtAppAddTimeOut(app_context,200,
                                       MappingNotifyTimeoutHandler,NULL);
            }
            else
            {
               MappingNotifyIgnoreCount--;
               if(MappingNotifyIgnoreCount==0 && load_map!=0)
               {
                  run_xmodmap(load_map);
                  load_map=0;
               }
            }
         }
         break;
      case(FocusIn):
         QueryFocus();
         break;
      case(FocusOut):
      {
         Window old=focus_window;
         focus_window=0;
         DelWindow(ev.xany.window);
         AddWindow(ev.xany.window,False,1);
         focus_window=old;
         break;
      }
      case(PropertyNotify):
         if(AppData.per_window_state && ev.xproperty.atom==XA_WM_NAME
         && ev.xproperty.state==PropertyNewValue)
         {
            if(SetTitleTimeout!=-1)
               XtRemoveTimeOut(SetTitleTimeout);
            SetTitleTimeout=XtAppAddTimeOut(app_context,1000,
                        (XtTimerCallbackProc)SetTitleIndicatorOnTimeout,
                        (XtPointer)ev.xproperty.window);
         }
         break;
#if TK==TK_XAW
      case(ClientMessage):
         fflush(stdout);
         if(ev.xclient.message_type==wm_protocols && ev.xclient.format==32
         && ev.xclient.data.l[0]==wm_delete_window)
         {
            if(ev.xclient.window==XtWindow(top_level))
            {
               XtDestroyApplicationContext(app_context);
               app_context=NULL;
            }
            else
            {
               Widget w=XtWindowToWidget(disp,ev.xclient.window);
               if(w)
                  XtPopdown(w);
            }
         }
         break;
#endif /* TK_XAW */
      }
   }
}

void  cleanup(void)
{
   if(app_context)
      SwitchKeyboard(LAT);
}

void  sig_term(int sig)
{
   fprintf(stderr,"%s: caught signal %d, exiting\n",program,sig);
   cleanup();
   exit(1);
}

void  sig_chld(int sig)
{
   int   status;
   if(wait(&status)==LockerRunning)
   {
      LockerRunning=0;
      SetAlarm();
   }
}

void  LockScreen()
{
   pid_t pid;
   sigset_t nset;

   SwitchKeyboard(Mode&~MODE);

   fflush(stderr);
   switch(pid=fork())
   {
   case(0):
      sigemptyset (&nset);
      sigprocmask (SIG_SETMASK, &nset, NULL);
      execl("/bin/sh","sh","-c",AppData.locker,NULL);
      fprintf(stderr,"%s: cannot execute /bin/sh - %s\n",program,strerror(errno));
      fflush(stderr);
      _exit(1);
   case(-1):
      fprintf(stderr,"%s: cannot do fork() - %s\n",program,strerror(errno));
      fflush(stderr);
      break;
   default:
      LockerRunning=pid;
   }
}

void  run_xmodmap(const char *file)
{
   char *alt_file;
   char *xmodmap=AppData.xmodmap_program;

   XSync(disp,False);

try_again:
   if(access(file,R_OK)==-1)
   {
      if(file[0]!='/')
      {
         alt_file=alloca(strlen(KEYMAP_DIR)+strlen(file)+2+4);
         sprintf(alt_file,"%s/%s",KEYMAP_DIR,file);
         if(access(alt_file,R_OK)==0)
            file=alt_file;
         else
         {
            strcat(alt_file,".xmm");
            if(access(alt_file,R_OK)==0)
               file=alt_file;
            else
            {
               /* backward compatibility */
               char *x=strstr(file,"-xrus");
               if(x)
               {
                  memmove(x,x+4,strlen(x+4)+1);
                  goto try_again;
               }
            }
         }
      }
   }

   fflush(stderr);
   switch(fork())
   {
   case(-1):
      fprintf(stderr,"%s: fork() failed - %s\n",program,strerror(errno));
      break;
   case(0):
      /* child */
      execlp(xmodmap,xmodmap,file,NULL);
      fprintf(stderr,"%s: execlp(%s) failed - %s\n",program,xmodmap,strerror(errno));
      fflush(stderr);
      _exit(1);
   default:
      /* parent */
      ;
   }
}

void  AlarmHandler(int sig)
{
   time_t now=time(0);
   if(now < alarm_expected)
   {
      fprintf(stderr,"%s: unexpected early alarm signal\n",program);
      alarm(alarm_expected-now);
      return;
   }
   if(AppData.autolock)
   {
#ifdef DEBUG
      printf("calling LockScreen() at %ld\n",now);
#endif
      LockScreen();
   }
   (void)sig;
}

void  SetAlarm()
{
   alarm(0);
   if(AppData.autolock && AppData.timeout>0 && !LockerRunning)
   {
      alarm(AppData.timeout*60);
      alarm_expected=time(0)+AppData.timeout*60-1;
#ifdef DEBUG
      printf("alarm expected at %ld\n",alarm_expected);
#endif
   }
}

int   X_error_handler(Display *d,XErrorEvent *ev)
{
   /* A window can be deleted between CreateNotify and our request */

   if(ev->error_code==BadWindow
   || ev->error_code==BadDrawable
   || ev->error_code==BadAccess)
      return(-1);
   return(old_error_handler(d,ev));
}

void  hook_signals(void)
{
   struct sigaction  act;

   act.sa_handler=sig_term;
   sigemptyset(&act.sa_mask);
   act.sa_flags=0;
   sigaction(SIGTERM,&act,NULL);
   sigaction(SIGINT,&act,NULL);
   sigaction(SIGQUIT,&act,NULL);

   act.sa_handler=SIG_IGN;
   sigaction(SIGHUP,&act,NULL);

   act.sa_handler=AlarmHandler;
   act.sa_flags=SA_RESTART;
   sigaction(SIGALRM,&act,NULL);
   act.sa_handler=sig_chld;
   act.sa_flags=SA_RESTART|SA_NOCLDSTOP;
   sigaction(SIGCHLD,&act,NULL);

   act.sa_handler=ToLatKeysFire;
   act.sa_flags=0;
   sigaction(SIGUSR1,&act,NULL);
   act.sa_handler=ToRusKeysFire;
   act.sa_flags=0;
   sigaction(SIGUSR2,&act,NULL);
}

int   main(int argc,char **argv)
{
   Atom  ol_decor_del_atom,ol_decor_del[2];
   XrmDatabase default_rdb,rdb;
   char  *HOME;
   char  *buf;
   char  err[1024];
   int   scr;

   Window w;
   Atom  wm_client_leader;

   int   saved_argc=argc;
   char  **saved_argv=alloca((argc+1)*sizeof(char*));
   memcpy(saved_argv,argv,argc*sizeof(char*));
   saved_argv[argc]=0;

   program=argv[0];

   if(argc==2)
   {
      if(!strcmp(argv[1],"--version")
      || !strcmp(argv[1],"-version")
      || !strcmp(argv[1],"-v"))
      {
         printf("Xrus(keyboard switcher) version " VERSION "\n"
                "\n"
                "Copyright (c) 1995-2001 Alexander V. Lukyanov (lav@yars.free.net)\n"
                "This is free software that gives you freedom to use, modify and distribute\n"
                "it under certain conditions, see COPYING (GNU GPL) for details.\n"
                "There is ABSOLUTELY NO WARRANTY, use at your own risk.\n");
         exit(0);
      }
      if(!strcmp(argv[1],"--help")
      || !strcmp(argv[1],"-help")
      || !strcmp(argv[1],"-h")
      || !strcmp(argv[1],"-?"))
      {
         printf("Usage: %s [OPTIONS] [keymap_file]\n"
                "\n"
                " {-|+}autolock          turn on/off screen autolocking\n"
                " -locker \"locker cmd\"   specify locker command\n"
                " -timeout sec           set autolock timeout\n"
                " {-|+}bell              turn on/off bell on keyboard switch\n"
                " {-|+}icon              turn on/off indicator icon\n"
                " -led led_no            use the led to indicate alternative mode\n"
                " +led                   don't use led for indication\n"
                " -nofork                don't fork to background\n"
                " -wmaker                display in Window Maker appicon\n"
                " -wmicon                display in window manager icon\n"
                " -perwindow             remember state for each window\n"
                "\n",program);
         exit(0);
      }
   }

   hook_signals();

   XtToolkitInitialize();
   XtSetLanguageProc(NULL,NULL,NULL);
   app_context=XtCreateApplicationContext();
   disp=XtOpenDisplay(app_context,NULL,"xrus",AppClass,options,XtNumber(options),&argc,argv);
   if(disp==NULL)
   {
      fprintf(stderr,"%s: cannot open display\n",program);
      exit(1);
   }
   client_id_mask=~(disp->resource_mask);

   if(argc>1 && argv[1][0]=='-')
   {
      fprintf(stderr,"%s: invalid option -- %s\n",program,argv[1]);
      fprintf(stderr,"%s: Try `%s --help' for more information\n",program,program);
      exit(1);
   }

   default_rdb=XrmGetStringDatabase(DefaultResources);
   rdb=XtDatabase(disp);
   XrmCombineDatabase(default_rdb,&rdb,False);
   HOME=getenv("HOME");
   if(HOME)
   {
      buf=alloca(strlen(HOME)+10);
      sprintf(buf,"%s/.xrus",HOME);
      XrmCombineFileDatabase(buf,&rdb,True);
   }

   old_error_handler=XSetErrorHandler(X_error_handler);

   /* select events from all windows located on the display
      and check for another xrus instance */
   for(scr=0; scr<ScreenCount(disp); scr++)
      AddWindow(RootWindowOfScreen(ScreenOfDisplay(disp,scr)),True,1000000);
   xrus_check=0;

   StartArgs();
#if TK==TK_MOTIF
   AddArg(XmNmwmDecorations,MWM_DECOR_BORDER);
   AddArg(XmNmwmFunctions,MWM_FUNC_MOVE|MWM_FUNC_CLOSE);
#elif TK==TK_NONE
   AddArg(XtNwidth,10); /* no toolkit - set size to avoid 0x0 size */
   AddArg(XtNheight,10);
#endif
   AddArg(XtNinput,False);
   AddArg(XtNallowShellResize,True);
   AddArg(XtNmappedWhenManaged,False);
   AddArg(XtNargc,saved_argc);
   AddArg(XtNargv,saved_argv);
   top_level=XtAppCreateShell("xrus",AppClass,applicationShellWidgetClass,disp,args,count);

   XtGetApplicationResources(top_level,&AppData,resources,XtNumber(resources),NULL,0);

   SetTitle(0);

   /* replace pointer to constant string with pointer to our buffers */
   strcpy(LockerData,AppData.locker?AppData.locker:DefaultLocker);
   AppData.locker=LockerData;
   strcpy(SwitchKeysData,AppData.switchKeys?AppData.switchKeys:DefaultSwitchKeys);
   AppData.switchKeys=SwitchKeysData;
   strcpy(ToLatKeysData,AppData.toLatKeys?AppData.toLatKeys:DefaultToLatKeys);
   AppData.toLatKeys=ToLatKeysData;
   strcpy(ToRusKeysData,AppData.toRusKeys?AppData.toRusKeys:DefaultToRusKeys);
   AppData.toRusKeys=ToRusKeysData;
   strcpy(SwitchForOneKeysData,AppData.switchForOneKeys?AppData.switchForOneKeys:DefaultSwitchForOneKeys);
   AppData.switchForOneKeys=SwitchForOneKeysData;

   if(argc>1)
   {
      AppData.xmodmap=argv[1];
   }

   if(AppData.xmodmap[0])
   {
      run_xmodmap(AppData.xmodmap);
   }

   /* let the keymap to settle */
   MappingNotifyTimeout=XtAppAddTimeOut(app_context,500,
                                        MappingNotifyTimeoutHandler,NULL);

   /* initialize switch keys */
   SwitchKeys.Fire=SwitchKeysFire;
   ParseKeyCombination(AppData.switchKeys,&SwitchKeys,err);
   PrintParseErrors("switchKeys",err);
   ToLatKeys.Fire=ToLatKeysFire;
   ParseKeyCombination(AppData.toLatKeys,&ToLatKeys,err);
   PrintParseErrors("toLatKeys",err);
   ToRusKeys.Fire=ToRusKeysFire;
   ParseKeyCombination(AppData.toRusKeys,&ToRusKeys,err);
   PrintParseErrors("toRusKeys",err);
   SwitchForOneKeys.Fire=SwitchForOneKeysFire;
   ParseKeyCombination(AppData.switchForOneKeys,&SwitchForOneKeys,err);
   PrintParseErrors("switchForOneKeys",err);

   if(AppData.keylog_file)
   {
      keylog_fd=open(AppData.keylog_file,O_WRONLY|O_APPEND|O_CREAT,0600);
      if(keylog_fd==-1)
         perror(AppData.keylog_file);
   }

#if !defined(DEBUG) && !defined(DONT_FORK)
   if(!AppData.noFork && !AppData.wmaker_icon)
   {
      switch(fork())
      {
      case(0):
         break;
      case(-1):
         perror("fork");
         exit(1);
      default:
         _exit(0);
      }
   }
#endif

#ifdef HAVE_ATEXIT
   atexit(cleanup);
#else
# ifdef __sun__
   on_exit(cleanup,NULL);          /* non-ANSI exit handler */
# endif
#endif

   XtAppAddActions(app_context,Actions,XtNumber(Actions));

#if TK!=TK_NONE
   XtAddEventHandler(top_level,StructureNotifyMask,False,UnmapHandler,NULL);

# if TK==TK_MOTIF
   StartArgs();
   AddArg(XmNresizable,False);
   form_w=XtCreateManagedWidget("form",xmFormWidgetClass,top_level,args,count);

   switch_button[0]=XtCreateWidget("modeButton0",xmPushButtonWidgetClass,form_w,NULL,0);
   switch_button[1]=XtCreateWidget("modeButton1",xmPushButtonWidgetClass,form_w,NULL,0);

   XtAddCallback(switch_button[0],XmNactivateCallback,(XtCallbackProc)PerformSwitch,NULL);
   XtAddCallback(switch_button[1],XmNactivateCallback,(XtCallbackProc)PerformSwitch,NULL);
# elif TK==TK_XAW
   form_w=XtCreateManagedWidget("form",boxWidgetClass,top_level,NULL,0);

   switch_button[0]=XtCreateWidget("modeButton0",commandWidgetClass,form_w,NULL,0);
   switch_button[1]=XtCreateWidget("modeButton1",commandWidgetClass,form_w,NULL,0);

   XtAddCallback(switch_button[0],XtNcallback,(XtCallbackProc)PerformSwitch,NULL);
   XtAddCallback(switch_button[1],XtNcallback,(XtCallbackProc)PerformSwitch,NULL);
# else
#  error unknown toolkit
# endif

   XtManageChild(switch_button[(Mode&MODE)==RUS]);

   XtAddEventHandler(switch_button[0],VisibilityChangeMask,False,VisibilityChange,NULL);
   XtAddEventHandler(switch_button[1],VisibilityChangeMask,False,VisibilityChange,NULL);

   XrusMenuCreate();

#endif /* !TK_NONE */

   kbd_state_atom=XInternAtom(disp,"KBD_STATE",False);

   XtRealizeWidget(top_level);

   KeyboardStateInit();
   InitCapsLockEmu();

#if TK!=TK_NONE
   /* Set various window properties. */
   w=XtWindow(top_level);

   ol_decor_del_atom=XInternAtom(disp,"_OL_DECOR_DEL",False);
   ol_decor_del[0]=XInternAtom(disp,"_OL_DECOR_HEADER",False);
   ol_decor_del[1]=XInternAtom(disp,"_OL_DECOR_RESIZE",False);

   wm_client_leader=XInternAtom(disp,"WM_CLIENT_LEADER",False);
   wm_delete_window=XInternAtom(disp,"WM_DELETE_WINDOW",False);
   wm_protocols=XInternAtom(disp,"WM_PROTOCOLS",False);

   XChangeProperty(disp,w,ol_decor_del_atom,XA_ATOM,32,
         PropModeReplace,(void*)ol_decor_del,2);

   XChangeProperty(disp,w,wm_client_leader,XA_WINDOW,32,
         PropModeReplace,(void*)&w,1);

#define WinStateAllWorkspaces  (1 << 0)   /* appears on all workspaces */
#define WinStateMinimized      (1 << 1)   /* to iconbox,taskbar,... */
#define WinStateMaximizedVert  (1 << 2)   /* maximized vertically */
#define WinStateMaximizedHoriz (1 << 3)   /* maximized horizontally */
#define WinStateHidden         (1 << 4)   /* not on taskbar if any, but still accessible */
#define WinStateRollup         (1 << 5)   /* only titlebar visible */
#define WinStateFixedPosition  (1 << 10)  /* fixed position on virtual desktop*/
#define WinStateArrangeIgnore  (1 << 11)  /* ignore for auto arranging */
#define WinStateWithdrawn      (1 << 31)  /* managed, but not available to user */

if(AppData.occupyAllDesks)
{  /* make the window global for all desktops */
   Atom win_state,sgi_desks_hints,sgi_desks_always_global;
   Atom kwm_win_sticky;
   long value;
   /* for gnome */
   static long state[2]={WinStateAllWorkspaces|WinStateArrangeIgnore,63};
   win_state=XInternAtom(disp,"_WIN_STATE",False);
   XChangeProperty(disp,w,win_state,XA_CARDINAL,32,
      PropModeReplace,(void*)state,2);
   /* for SGI */
   sgi_desks_hints=XInternAtom(disp,"_SGI_DESKS_HINTS",False);
   sgi_desks_always_global=XInternAtom(disp,"_SGI_DESKS_ALWAYS_GLOBAL",False);
   value=sgi_desks_always_global;
   XChangeProperty(disp,w,sgi_desks_hints,XA_ATOM,32,
      PropModeReplace,(void*)&value,1);
   /* for KDE */
   value=1;
   kwm_win_sticky=XInternAtom(disp,"KWM_WIN_STICKY",False);
   XChangeProperty(disp,w,kwm_win_sticky,kwm_win_sticky,32,
      PropModeReplace,(void*)&value,1);
}

#define WinHintsSkipFocus      (1 << 0)
#define WinHintsSkipWindowMenu (1 << 1)
#define WinHintsSkipTaskBar    (1 << 2)
#define WinHintsGroupTransient (1 << 3)
#define WinHintsDockHorizontal (1 << 6)   /* docked horizontally */

{
   Atom win_hints=XInternAtom(disp,"_WIN_HINTS",False);
   long value[2];
   value[0]=WinHintsSkipFocus|WinHintsSkipWindowMenu;
   if(!AppData.wm_icon)
      value[0]|=WinHintsSkipTaskBar;
   value[1]=127;
   XChangeProperty(disp,w,win_hints,XA_CARDINAL,32,
      PropModeReplace,(void*)value,2);
}

#if TK!=TK_MOTIF
{
   /* we have to emulate motif hints as we don't have Motif */
   static long motif_hints[]={0x3, 0x24, 0x2, 0xffffffff, 0};
   Atom motif_hints_atom=XInternAtom(disp,"_MOTIF_WM_HINTS",False);
   XChangeProperty(disp,XtWindow(top_level),motif_hints_atom,motif_hints_atom,32,
         PropModeReplace,(void*)motif_hints,XtNumber(motif_hints));

   /* Xaw' shell don't catches WM_DELETE_WINDOW, have to do it myself */
   XSetWMProtocols(disp,XtWindow(top_level),&wm_delete_window,1);
}
#endif

   if(AppData.adjustModeButtons)
   {
      Dimension w0,h0,w1,h1;

      StartArgs();
      AddArg(XtNwidth,&w0);
      AddArg(XtNheight,&h0);
      XtGetValues(switch_button[0],args,count);

      StartArgs();
      AddArg(XtNwidth,&w1);
      AddArg(XtNheight,&h1);
      XtGetValues(switch_button[1],args,count);

      if(w0<w1)
         w0=w1;
      if(h0<h1)
         h0=h1;

      StartArgs();
      AddArg(XtNwidth,w0);
      AddArg(XtNheight,h0);
      XtSetValues(switch_button[0],args,count);
      XtSetValues(switch_button[1],args,count);
      XtSetValues(form_w,args,count);
   }

   if(AppData.wm_icon || AppData.wmaker_icon)
   {
      int state=IconicState;
      if(AppData.wmaker_icon)
         state=WithdrawnState;
      XtUnmapWidget(form_w);
      XReparentWindow(disp,XtWindow(form_w),RootWindowOfScreen(ScreenOfDisplay(disp,scr)),0,0);
      XtVaSetValues(top_level,
         XtNiconWindow,XtWindow(form_w),
         XtNinitialState,state,
         NULL);
      XtMapWidget(top_level);
   }
   else
   {
      if(AppData.icon)
         XtMapWidget(top_level);
   }
#endif /* TK!=TK_NONE */

   SetAlarm();
   MainLoop();

   cleanup();
   return 0;
}

#ifdef C_ALLOCA
/* alloca.c calls xmalloc */
void *xmalloc(size_t s)
{
   void *p=malloc(s);
   if(p)
      return(p);
   fprintf(stderr,"%s: out of virtual memory\n",program);
   cleanup();
   exit(1);
}
#endif /* C_ALLOCA */
