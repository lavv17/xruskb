/*
  xrus - keyboard switcher/indicator
  Copyright (c) 1996 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.
*/
#include <config.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#if TK!=TK_NONE

#if TK==TK_MOTIF
# include <Xm/Xm.h>
# include <Xm/RowColumn.h>
# include <Xm/PushB.h>
# include <Xm/Separator.h>
# include <Xm/CascadeB.h>
#elif TK==TK_XAW
# include <X11/Xaw/SimpleMenu.h>
# include <X11/Xaw/MenuButton.h>
# include <X11/Xaw/Command.h>
# include <X11/Xaw/Form.h>
# include <X11/Xaw/SmeBSB.h>
# include <X11/Xaw/SmeLine.h>
#endif

#include "xrus.h"
#include "menu.h"
#include "props.h"
#include "altmap.h"

Widget   menu;
Widget   menus[2]={0,0};

enum
{
   LOCK,
   PROPS,
   EXIT
};

static
void  menu_item_cb(Widget w,XtPointer closure,XtPointer call_data)
{
   switch((int)closure)
   {
   case LOCK:
      LockScreen();
      break;
   case PROPS:
      PropsPopup();
      break;
   case EXIT:
      cleanup();
      XtDestroyApplicationContext(app_context);
      app_context=NULL;
      break;
   }
}

static
XtCallbackRec  menu_cb[]=
{
   {menu_item_cb,0},
   {NULL}
};

static
void  popup_create(int n)
{
   if((menu=menus[n])!=NULL)
      return;

#if TK==TK_MOTIF
{
   Widget   altmap_menu;

   menu=menus[n]=XmCreatePopupMenu(switch_button[n],"menu",NULL,0);

   menu_cb[0].closure=(XtPointer)LOCK;
   XtVaCreateManagedWidget("lock",xmPushButtonWidgetClass,menu,XmNactivateCallback,menu_cb,NULL);
   menu_cb[0].closure=(XtPointer)PROPS;
   XtVaCreateManagedWidget("props",xmPushButtonWidgetClass,menu,XmNactivateCallback,menu_cb,NULL);

   altmap_menu=CreateAltMapMenu(menu,AppData.altMaps);
   if(altmap_menu)
   {
      XtVaCreateManagedWidget("altmap",xmCascadeButtonWidgetClass,menu,
         XmNsubMenuId,altmap_menu,
         NULL);
   }

   XtVaCreateManagedWidget("line",xmSeparatorWidgetClass,menu,NULL);
   menu_cb[0].closure=(XtPointer)EXIT;
   XtVaCreateManagedWidget("exit",xmPushButtonWidgetClass,menu,XmNactivateCallback,menu_cb,NULL);
}
#elif TK==TK_XAW
   menu=menus[n]=XtCreatePopupShell("menu",simpleMenuWidgetClass,switch_button[n],NULL,0);
   {
      static XtTranslations trans=None;
      if(trans==None)
	 trans=XtParseTranslationTable(
	    "<Btn3Down>: XawPositionSimpleMenu(menu)"
                       " MenuPopup(menu)");
      XtOverrideTranslations(switch_button[n],trans);
   }
   menu_cb[0].closure=(XtPointer)EXIT;
   XtVaCreateManagedWidget("exit",smeBSBObjectClass,menu,XtNcallback,menu_cb,NULL);
   XtVaCreateManagedWidget("line",smeLineObjectClass,menu,NULL);
   CreateAltMapMenu(menu,AppData.altMaps);
   XtVaCreateManagedWidget("line",smeLineObjectClass,menu,NULL);
   menu_cb[0].closure=(XtPointer)LOCK;
   XtVaCreateManagedWidget("lock",smeBSBObjectClass,menu,XtNcallback,menu_cb,NULL);
   menu_cb[0].closure=(XtPointer)PROPS;
   XtVaCreateManagedWidget("props",smeBSBObjectClass,menu,XtNcallback,menu_cb,NULL);
#else
# error bad TK
#endif
}

static
void  popup_menu(XEvent *ev)
{
#if TK==TK_MOTIF
   XmMenuPosition(menu,&(ev->xbutton));
   XtManageChild(menu);
#endif
}

void  XrusMenuCreate()
{
   popup_create(0);
   popup_create(1);
}

void  XrusMenuPopup(Widget w,XEvent *ev,String *params, Cardinal *num_params)
{
   popup_create(w==switch_button[1]);
   popup_menu(ev);
}
#else /* TK_NONE */
void XrusMenuPopup(Widget w,XEvent *e,String *p, Cardinal *n) {}
#endif
