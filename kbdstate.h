/*
  xrus - keyboard switcher/indicator
  Copyright (c) 1996 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.
*/

void  KeepTrackOfKeyboard(XEvent *ev);

int   IsKeyCodePressed(KeyCode i);
int   IsKeySymPressed(KeySym ks);
int   IsModifierPressed(int mod_index);

void  KeyboardStateInit();

KeySym            *GetKeymap();
XModifierKeymap   *GetModifiers();

extern int  keysyms_per_keycode;
extern int  min_keycode,max_keycode;
