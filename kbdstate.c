/*
  xrus - keyboard switcher/indicator
  Copyright (c) 1996 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.
*/

#include <config.h>
#include <X11/Intrinsic.h>

#include "xrus.h"
#include "kbdstate.h"

static char  key_vector[32];

int	 min_keycode,max_keycode;
KeySym	 *keymap;
int	 keysyms_per_keycode;
XModifierKeymap *modifiermap;

void  KeyboardStateInit()
{
   XDisplayKeycodes(disp,&min_keycode,&max_keycode);
   XQueryKeymap(disp,key_vector);
}

void  KeepTrackOfKeyboard(XEvent *ev)
{
   switch(ev->type)
   {
   case KeyPress:
      key_vector[ev->xkey.keycode/8]|=(1<<(ev->xkey.keycode%8));
      break;
   case KeyRelease:
      key_vector[ev->xkey.keycode/8]&=~(1<<(ev->xkey.keycode%8));
      break;
   case KeymapNotify:
#ifdef DEBUG
      printf("KeymapNotify\n");
#endif
      memcpy(key_vector,ev->xkeymap.key_vector,32);
      break;
   case MappingNotify:
      switch(ev->xmapping.request)
      {
      case MappingModifier:
         if(modifiermap)
	    XFreeModifiermap(modifiermap);
	 modifiermap=NULL;
	 break;
      case MappingKeyboard:
	 if(keymap)
	    XFree(keymap);
	 keymap=NULL;
	 break;
      }
      break;
   }
}

int   IsKeyCodePressed(KeyCode i)
{
   return(key_vector[i/8]&(1<<(i%8)));
}
int   IsKeySymPressed(KeySym ks)
{
   return IsKeyCodePressed(XKeysymToKeycode(disp,ks));
}

KeySym   *GetKeymap()
{
   if(!keymap)
      keymap=XGetKeyboardMapping(disp,min_keycode,
               max_keycode-min_keycode+1,&keysyms_per_keycode);
   return(keymap);
}

XModifierKeymap  *GetModifiers()
{
   if(!modifiermap)
      modifiermap=XGetModifierMapping(disp);
   return(modifiermap);
}

int   IsModifierPressed(int n)
{
   int i;
   KeyCode *kc;
   XModifierKeymap *mod=GetModifiers();

   kc=mod->modifiermap+n*mod->max_keypermod;
   for(i=0; i<mod->max_keypermod; i++)
   {
      if(*kc && IsKeyCodePressed(*kc))
         return 1;
      kc++;
   }
   return 0;
}
