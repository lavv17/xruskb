/*
  xrus - keyboard switcher/indicator
  Copyright (c) 1996 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.
*/
#include <config.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "keycomb.h"
#include "xrus.h"

#include "xalloca.h"

void  ParseKeyCombination(const char *str,struct KeyCombination *kc,char *err)
{
   char  *plus;
   char  *scan;
   KeySym   ks;
   int   i;

   scan=alloca(strlen(str)+1);
   strcpy(scan,str);

   kc->KeysNum=0;
   err[0]=0;
   for(;;)
   {
      plus=strchr(scan,'+');
      if(plus==NULL)
      {
         if(*scan==0)
            break;
      }
      else
      {
         *plus=0;
      }
      ks=XStringToKeysym(scan);
      if(ks==NoSymbol)
      {
         sprintf(err+strlen(err),"Invalid key name (%s)\n",scan);
      }
      else
      {
         for(i=0; i<kc->KeysNum; i++)
         {
            if(kc->Keys[i]==ks)
               break;
         }
         if(i==kc->KeysNum)
         {
            if(kc->KeysNum==sizeof(kc->Keys)/sizeof(kc->Keys[0]))
            {
               sprintf(err+strlen(err),"Too much keys\n");
               break;
            }
            kc->Keys[kc->KeysNum++]=ks;
         }
      }
      if(plus!=NULL)
         scan=plus+1;
      else
         break;
   }
   kc->KeysPressed=0;
   kc->KeysArmed=0;
}

void  KeyCombinationProcessKeyPress(struct KeyCombination *kc,KeySym ks)
{
   int      oldKeysPressed;
   int      i;

   oldKeysPressed=kc->KeysPressed;
   for(i=0; i<kc->KeysNum; i++)
   {
      if(kc->Keys[i]==ks)
         kc->KeysPressed|=(1<<i);
   }
   if(kc->KeysPressed!=oldKeysPressed && kc->KeysPressed==(1<<kc->KeysNum)-1)
      kc->KeysArmed=1;
   else
      kc->KeysArmed=0;
}

int   KeyCombinationProcessKeyRelease(struct KeyCombination *kc,KeySym ks,int fire)
{
   int      oldKeysPressed;
   int      i;

   oldKeysPressed=kc->KeysPressed;
   for(i=0; i<kc->KeysNum; i++)
   {
      if(kc->Keys[i]==ks)
         kc->KeysPressed&=~(1<<i);
   }
   i=0;
   if(kc->KeysPressed!=oldKeysPressed && oldKeysPressed==(1<<kc->KeysNum)-1 && kc->KeysArmed)
   {
      if(fire && kc->Fire)
         kc->Fire();
      i=1;
   }
   kc->KeysArmed=0;
   return i;
}

void  PrintParseErrors(const char *resource,char *err)
{
   if(*err)
   {
      char *nl=err-1;
      char *nl1;
      while((nl1=strchr(nl+1,'\n'))!=NULL)
      {
         *nl1=0;
         fprintf(stderr,"%s: %s parse error: %s\n",program,resource,nl+1);
         nl=nl1;
      }
   }
}
