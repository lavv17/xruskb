/*
  xrus - keyboard switcher/indicator
  Copyright (c) 1996 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.
*/
#include <config.h>

#if TK!=TK_NONE

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#if TK==TK_MOTIF
# include <Xm/Xm.h>
# include <Xm/RowColumn.h>
# include <Xm/PushB.h>
#elif TK==TK_XAW
# include <X11/Xaw/SmeBSB.h>
#endif

#include "altmap.h"
#include "xrus.h"
#include "keycomb.h"

#include "xalloca.h"

static
void  altmap_CB(Widget w,XtPointer closure,XtPointer call_data)
{
   NewMode=Mode;
   ToLatKeys.Fire();
   FixNewMode();
   SwitchKeyboard(NewMode);
   load_map_delayed((const char*)closure);
}

Widget   CreateAltMapMenu(Widget parent,const char *altmap)
{
   int   altmap_count=0;
#if TK==TK_MOTIF
   Widget   menu_pane=XmCreatePulldownMenu(parent,"altmap_menu",NULL,0);
#endif
   Widget   button;
   char     *scan=alloca(strlen(altmap)+1);
   char     *file;
   char     button_name[32];

   strcpy(scan,altmap);

   for(;;)
   {
      /* ignore leading spaces */
      while(*scan && isspace((unsigned char)*scan))
         scan++;
      if(!*scan)
         break;

      file=scan;

      /* track down the end of file name */
      while(*scan && !isspace((unsigned char)*scan))
         scan++;

      if(*scan)
         *scan++=0;

      /* add the item to the menu_pane */
      sprintf(button_name,"altmap%d",altmap_count++);
#if TK==TK_MOTIF
      button=XmCreatePushButton(menu_pane,button_name,NULL,0);
      XtAddCallback(button,XmNactivateCallback,altmap_CB,strdup(file));
      XtManageChild(button);
#elif TK==TK_XAW
      button=XtVaCreateManagedWidget(button_name,smeBSBObjectClass,parent,NULL);
      XtAddCallback(button,XtNcallback,altmap_CB,strdup(file));
#endif
   }

#if TK==TK_MOTIF
   if(altmap_count==0)
   {
      XtDestroyWidget(XtParent(menu_pane));
      menu_pane=NULL;
   }
   return menu_pane;
#elif TK==TK_XAW
   return NULL;
#endif
}

#endif /* !TK_NONE */
