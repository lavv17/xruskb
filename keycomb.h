/*
  xrus - keyboard switcher/indicator
  Copyright (c) 1996 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.
*/

struct   KeyCombination
{
   int      KeysNum;       /* Number of keys in this combination  */
   KeySym   Keys[4];       /* KeySyms of the combinations         */
   int      KeysPressed;   /* Bit mask of pressed keys            */
   int      KeysArmed;     /* Flag indicating that the combination is armed
                              and will fire upon release of any of the keys */
   void     (*Fire)(void); /* Callback */
};

void  ParseKeyCombination(const char *str,struct KeyCombination *kc,char *err);
void  KeyCombinationProcessKeyPress(struct KeyCombination *kc,KeySym ks);
int   KeyCombinationProcessKeyRelease(struct KeyCombination *kc,KeySym ks,int fire);
void  PrintParseErrors(const char *resource,char *err);
