/*
  xrus - keyboard switcher/indicator
  Copyright (c) 1995-1998 Alexander V. Lukyanov
  This is free software with no warranty.
  See COPYING for details.

  Athena version of properties dialog
  by Vladimir Grachev, e-mail vgrachev@ssau.ru (1998)
*/
/*__________________________________________________________________________
**
** File: props.c
**__________________________________________________________________________
*/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#if TK==TK_MOTIF
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/DialogS.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/Scale.h>
#include <Xm/Frame.h>
#include <Xm/TextF.h>
#include <Xm/LabelG.h>
#include <Xm/MessageB.h>

#elif TK==TK_XAW

#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#endif

#include "keycomb.h"
#include "xrus.h"

#include "xalloca.h"

#if TK!=TK_NONE

#if TK==TK_MOTIF
struct
{
   Widget   shell,form,ok_button,apply_button,cancel_button;
   Widget   allow_locker,autolock_delay,use_bell;
   Widget   frame1,frame2,form1;
   Widget   form2,form4;
   Widget   locker,switch_keys,locker_def,switch_keys_def;
   Widget   to_lat_keys,to_rus_keys;
   Widget   switch_for_one_keys;
   Widget   locker_test,save_button;
}
   props;
#elif TK==TK_XAW

#define MAX_LOCKER_TIMEOUT 120

int locker_timeout;

struct {
  Widget shell, form;
  Widget locker_toggle, minutes_sb, minutes_label, minutes_value_label;
  Widget command_label, locker;
  Widget test_button, default1_button;
  Widget bell_toggle;
  Widget switch_label, switch_keys;
  Widget lat_label, to_lat_keys;
  Widget rus_label, to_rus_keys;
  Widget switch_for_one_keys;
  Widget default2_button;
  Widget ok_button, apply_button, save_button, cancel_button;
} props;

void set_minutes(void) {
Dimension length;
char buffer[20];
float top;
Arg args[1];
XtArgVal *l_top;

  XtVaGetValues(props.minutes_sb,
     XtNlength, &length,
     NULL);

  /* This doesn't work! Why??? */
  /*  XawScrollbarSetThumb(props.minutes_sb,
    (float) ((float) locker_timeout / (float) MAX_LOCKER_TIMEOUT), 0.0);*/

  top = (float) ((float) locker_timeout / (float) MAX_LOCKER_TIMEOUT);
  l_top = (XtArgVal *) &top;
  XtSetArg(args[0], XtNtopOfThumb, *l_top);
  XtSetValues(props.minutes_sb, args, 1);

  sprintf(buffer, "%d ", locker_timeout);
  XtVaSetValues(props.minutes_value_label,
    XtNlabel, buffer,
    NULL);

};
#endif /* TK==TK_XAW */

static
char *get_string(Widget w)
{
   char  *value;
#if TK==TK_MOTIF
   XtVaGetValues(w,XmNvalue,&value,NULL);
#elif TK==TK_XAW
   XtVaGetValues(w,XtNstring,&value,NULL);
#endif
   return value;
}

static
void set_string(Widget w,const char *value)
{
#if TK==TK_MOTIF
   XtVaSetValues(w,XmNvalue,value,NULL);
#elif TK==TK_XAW
   XtVaSetValues(w,XtNstring,value,NULL);
#endif
}

static
void  PropsSet()
{
#if TK==TK_MOTIF
   XtVaSetValues(props.allow_locker,XmNset,!!AppData.autolock,NULL);
   XtVaSetValues(props.autolock_delay,XmNvalue,AppData.timeout,NULL);
   XtVaSetValues(props.use_bell,XmNset,!!AppData.useBell,NULL);
#elif TK==TK_XAW
   XtVaSetValues(props.locker_toggle,
     XtNstate, !!AppData.autolock,
     NULL);

   locker_timeout = AppData.timeout;
   set_minutes();

   XtVaSetValues(props.minutes_sb,
     XtNvalue, AppData.timeout,
     NULL);
   XtVaSetValues(props.bell_toggle,
     XtNstate, !!AppData.useBell,
     NULL);
#endif

   set_string(props.locker,AppData.locker);
   set_string(props.switch_keys,AppData.switchKeys);
   set_string(props.to_lat_keys,AppData.toLatKeys);
   set_string(props.to_rus_keys,AppData.toRusKeys);
   set_string(props.switch_for_one_keys,AppData.switchForOneKeys);
}

static
void  ErrorDismiss_CB(Widget w,XtPointer closure,XtPointer call_data)
{
   XtDestroyWidget(w);
}

static
int  PropsApply()
{
   char  old_switchKeys[256];
   char  message[1024];
   char  large_message[3*1024];

   large_message[0]=0;

   strcpy(old_switchKeys,AppData.switchKeys);
   strcpy(AppData.switchKeys,get_string(props.switch_keys));
   ParseKeyCombination(AppData.switchKeys,&SwitchKeys,message);
   if(*message)
   {
      strcat(large_message,message);

      strcpy(AppData.switchKeys,old_switchKeys);
      ParseKeyCombination(AppData.switchKeys,&SwitchKeys,message);
   }

   strcpy(old_switchKeys,AppData.toLatKeys);
   strcpy(AppData.toLatKeys,get_string(props.to_lat_keys));
   ParseKeyCombination(AppData.toLatKeys,&ToLatKeys,message);
   if(*message)
   {
      strcat(large_message,message);

      strcpy(AppData.toLatKeys,old_switchKeys);
      ParseKeyCombination(AppData.toLatKeys,&ToLatKeys,message);
   }

   strcpy(old_switchKeys,AppData.toRusKeys);
   strcpy(AppData.toRusKeys,get_string(props.to_rus_keys));
   ParseKeyCombination(AppData.toRusKeys,&ToRusKeys,message);
   if(*message)
   {
      strcat(large_message,message);

      strcpy(AppData.toRusKeys,old_switchKeys);
      ParseKeyCombination(AppData.toRusKeys,&ToRusKeys,message);
   }

   strcpy(old_switchKeys,AppData.switchForOneKeys);
   strcpy(AppData.switchForOneKeys,get_string(props.switch_for_one_keys));
   ParseKeyCombination(AppData.switchForOneKeys,&SwitchForOneKeys,message);
   if(*message)
   {
      strcat(large_message,message);

      strcpy(AppData.switchForOneKeys,old_switchKeys);
      ParseKeyCombination(AppData.switchForOneKeys,&SwitchForOneKeys,message);
   }

   if(large_message[0])
   {
#if TK==TK_MOTIF
      Widget error;
      Arg   arg[3];
      XmString msg=XmStringCreateLocalized(large_message);
      XmString title=XmStringCreateLocalized("Parse error");

      if(message[strlen(message)-1]=='\n')
         message[strlen(message)-1]=0;

      XtSetArg(arg[0],XmNmessageString,msg);
      XtSetArg(arg[1],XmNdialogStyle,XmDIALOG_APPLICATION_MODAL);
      XtSetArg(arg[2],XmNdialogTitle,title);
      error=XmCreateErrorDialog(props.shell,"error",arg,3);
      XtUnmanageChild(XtNameToWidget(error,"Cancel"));
      XtUnmanageChild(XtNameToWidget(error,"Help"));
      XtAddCallback(error,XmNunmapCallback,ErrorDismiss_CB,0);

      XmStringFree(msg);
      XmStringFree(title);
      XtManageChild(error);
#elif TK==TK_XAW
      Widget error_shell, error_dialog;

      error_shell = XtVaCreatePopupShell("error_shell",
        topLevelShellWidgetClass, props.shell,
        NULL);

      error_dialog = XtVaCreateManagedWidget("error_dialog",
        dialogWidgetClass, error_shell,
        XtNlabel, large_message,
        NULL);

      XawDialogAddButton(error_dialog,
        "Ok", ErrorDismiss_CB, (XtPointer) error_shell);

      XtPopup(error_shell, XtGrabExclusive);

#endif /* TK==TK_XAW */
      return 0;
   }

   strcpy(AppData.locker,get_string(props.locker));

#if TK==TK_MOTIF
   XtVaGetValues(props.allow_locker,XmNset,&AppData.autolock,NULL);
   XtVaGetValues(props.autolock_delay,XmNvalue,&AppData.timeout,NULL);
   XtVaGetValues(props.use_bell,XmNset,&AppData.useBell,NULL);
#elif TK==TK_XAW
   XtVaGetValues(props.locker_toggle,
     XtNstate, &AppData.autolock,
     NULL);
   AppData.timeout = locker_timeout;
   XtVaGetValues(props.bell_toggle,
     XtNstate, &AppData.useBell,
     NULL);
#endif
   SetAlarm();
   return 1;
}

static
void  PropsPopdown()
{
   XtPopdown(props.shell);
   XtUnmanageChild(props.form);
}

void  PropsSave()
{
   FILE  *f;
   char  *HOME=getenv("HOME");
   char  *filename;

   if(!HOME)
      return;

   filename=alloca(strlen(HOME)+10);
   sprintf(filename,"%s/.xrus",HOME);

   f=fopen(filename,"w");
   if(!f)
   {
      perror(filename);
      return;
   }

   fprintf(f,"! This file was created automatically by Xrus\n"
      "Xrus*locker:\t\t%s\n"
      "Xrus*timeout:\t\t%d\n"
      "Xrus*autolock:\t\t%s\n"
      "Xrus*useBell:\t\t%s\n"
      "Xrus*switchKeys:\t%s\n"
      "Xrus*toLatKeys:\t\t%s\n"
      "Xrus*toRusKeys:\t\t%s\n"
      "Xrus*switchForOneKeys:\t%s\n",

      AppData.locker,
      AppData.timeout,
      AppData.autolock?"true":"false",
      AppData.useBell ?"true":"false",
      AppData.switchKeys,
      AppData.toLatKeys,
      AppData.toRusKeys,
      AppData.switchForOneKeys
      );

   fclose(f);
}

static
void  Button_CB(Widget w,XtPointer user_data,XtPointer cb_data)
{
   switch((int)user_data)
   {
   case(0): /* Ok */
      if(!PropsApply())
         return;
      PropsPopdown();
      break;
   case(1): /* Apply */
      if(!PropsApply())
         return;
      break;
   case(2): /* Cancel */
      PropsPopdown();
      break;
   case(3): /* Save */
      if(!PropsApply())
         return;
      PropsSave();
      PropsPopdown();
      break;
   }
}

static
void  LockerTest_CB(Widget w,XtPointer user_data,XtPointer cb_data)
{
   char  old_locker[256];

   strcpy(old_locker,AppData.locker);
   strcpy(AppData.locker,get_string(props.locker));

   LockScreen();

   strcpy(AppData.locker,old_locker);
}

static
void  Default_CB(Widget w,XtPointer closure,XtPointer cd_data)
{
   switch((int)closure)
   {
   case(0):
      /* set locker defaults */
#if TK==TK_MOTIF
      XtVaSetValues(props.locker,XmNvalue,DefaultLocker,NULL);
      XtVaSetValues(props.allow_locker,XmNset,0,NULL);
      XtVaSetValues(props.autolock_delay,XmNvalue,30,NULL);
#elif TK==TK_XAW
      XtVaSetValues(props.locker,
        XtNstring, DefaultLocker,
        NULL);
      locker_timeout = 30;
      set_minutes();
      XtVaSetValues(props.locker_toggle,
        XtNstate, 0,
        NULL);
/*      XtVaSetValues(props.autolock_delay,XmNvalue,30,NULL);*/
#endif
      break;
   case(1):
      /* set switch keys default */
#if TK==TK_MOTIF
      XtVaSetValues(props.use_bell,XmNset,0,NULL);
#elif TK==TK_XAW
      XtVaSetValues(props.bell_toggle,
        XtNstate, 0,
        NULL);
#endif
      set_string(props.switch_keys,DefaultSwitchKeys);
      set_string(props.to_lat_keys,DefaultToLatKeys);
      set_string(props.to_rus_keys,DefaultToRusKeys);
      set_string(props.switch_for_one_keys,DefaultSwitchForOneKeys);
      break;
   }
}

#if TK==TK_XAW
static void  cb_scroll(Widget w, XtPointer client_data, XtPointer position) {

  if ((int) position > 0 && locker_timeout < MAX_LOCKER_TIMEOUT)
    locker_timeout++;
  if ((int) position < 0 && locker_timeout > 1)
    locker_timeout--;

  set_minutes();

};

static void  cb_jump(Widget w, XtPointer client_data, XtPointer percent_ptr) {

  locker_timeout = (int) (*(float *) percent_ptr * MAX_LOCKER_TIMEOUT);

  set_minutes();

};
#endif /* TK_XAW */

static
void  PropsInit()
{
   static int inited=0;

#if TK==TK_MOTIF
   int   count;
   Arg   args[10];
#endif

   if(inited)
      return;

/* Create Properties sheet */

#if TK==TK_MOTIF

   StartArgs();
   AddArg(XmNmappedWhenManaged,False);
   props.shell=XmCreateDialogShell(top_level,"props",args,count);

   props.form=XtVaCreateWidget("form",xmFormWidgetClass,props.shell,
      XmNautoUnmanage,False,
      NULL);

   props.frame1=XtVaCreateManagedWidget("frame1",xmFrameWidgetClass,props.form,NULL);
   props.form1=XtVaCreateManagedWidget("form1",xmFormWidgetClass,props.frame1,NULL);

   props.locker_def=XtVaCreateManagedWidget("locker_def",xmPushButtonWidgetClass,props.form1,NULL);
   XtAddCallback(props.locker_def,XmNactivateCallback,Default_CB,(XtPointer)0);

   props.locker_test=XtVaCreateManagedWidget("locker_test",xmPushButtonWidgetClass,props.form1,NULL);
   XtAddCallback(props.locker_test,XmNactivateCallback,LockerTest_CB,(XtPointer)0);

   props.locker=XtVaCreateManagedWidget("locker",xmTextFieldWidgetClass,props.form1,NULL);
   XtVaCreateManagedWidget("locker_label",xmLabelGadgetClass,props.form1,NULL);

   props.allow_locker=XtVaCreateManagedWidget("allow_locker",xmToggleButtonWidgetClass,props.form1,NULL);
   props.autolock_delay=XtVaCreateManagedWidget("autolock_delay",xmScaleWidgetClass,props.form1,NULL);

   props.frame2=XtVaCreateManagedWidget("frame2",xmFrameWidgetClass,props.form,NULL);
   props.form2=XtVaCreateManagedWidget("form2",xmFormWidgetClass,props.frame2,NULL);

   props.use_bell=XtVaCreateManagedWidget("use_bell",xmToggleButtonWidgetClass,props.form2,NULL);
   XtVaCreateManagedWidget("switch_keys_label",xmLabelGadgetClass,props.form2,NULL);
   props.switch_keys=XtVaCreateManagedWidget("switch_keys",xmTextFieldWidgetClass,props.form2,NULL);
   XtVaCreateManagedWidget("to_lat_keys_label",xmLabelGadgetClass,props.form2,NULL);
   props.to_lat_keys=XtVaCreateManagedWidget("to_lat_keys",xmTextFieldWidgetClass,props.form2,NULL);
   XtVaCreateManagedWidget("to_rus_keys_label",xmLabelGadgetClass,props.form2,NULL);
   props.to_rus_keys=XtVaCreateManagedWidget("to_rus_keys",xmTextFieldWidgetClass,props.form2,NULL);
   XtVaCreateManagedWidget("switch_for_one_keys_label",xmLabelGadgetClass,props.form2,NULL);
   props.switch_for_one_keys=XtVaCreateManagedWidget("switch_for_one_keys",xmTextFieldWidgetClass,props.form2,NULL);

   props.switch_keys_def=XtVaCreateManagedWidget("switch_keys_def",xmPushButtonWidgetClass,props.form2,NULL);
   XtAddCallback(props.switch_keys_def,XmNactivateCallback,Default_CB,(XtPointer)1);

   props.form4=XtVaCreateManagedWidget("form4",xmFormWidgetClass,props.form,NULL);

   props.ok_button=XtVaCreateManagedWidget("ok",xmPushButtonWidgetClass,props.form4,NULL);
   XtVaSetValues(props.form,
      XmNdefaultButton,props.ok_button,
      NULL);
   XtVaSetValues(props.form4,
      XmNdefaultButton,props.ok_button,
      NULL);

   props.cancel_button=XtVaCreateManagedWidget("cancel",xmPushButtonWidgetClass,props.form4,NULL);
   props.apply_button=XtVaCreateManagedWidget("apply",xmPushButtonWidgetClass,props.form4,NULL);
   props.save_button=XtVaCreateManagedWidget("save",xmPushButtonWidgetClass,props.form4,NULL);

   XtVaSetValues(props.form,
      XmNcancelButton,props.cancel_button,
      NULL);

   XtAddCallback(props.ok_button,XmNactivateCallback,Button_CB,(XtPointer)0);
   XtAddCallback(props.apply_button,XmNactivateCallback,Button_CB,(XtPointer)1);
   XtAddCallback(props.cancel_button,XmNactivateCallback,Button_CB,(XtPointer)2);
   XtAddCallback(props.save_button,XmNactivateCallback,Button_CB,(XtPointer)3);

#elif TK==TK_XAW

  props.shell = XtVaCreatePopupShell("props_xaw",
    topLevelShellWidgetClass, top_level,
    NULL);

  props.form = XtVaCreateWidget("form",
    formWidgetClass, props.shell,
    NULL);

  props.locker_toggle = XtVaCreateManagedWidget("locker_toggle",
    toggleWidgetClass, props.form,
    NULL);

  props.minutes_sb = XtVaCreateManagedWidget("minutes_sb",
    scrollbarWidgetClass, props.form,
    XtNorientation, XtorientHorizontal,
    NULL);
  XtAddCallback(props.minutes_sb,
    XtNscrollProc, cb_scroll, (XtPointer) NULL);
  XtAddCallback(props.minutes_sb,
    XtNjumpProc, cb_jump, (XtPointer) NULL);

  props.minutes_label = XtVaCreateManagedWidget("minutes_label",
    labelWidgetClass, props.form,
    NULL);

  props.minutes_value_label = XtVaCreateManagedWidget("minutes_value_label",
    labelWidgetClass, props.form,
    NULL);

  props.command_label = XtVaCreateManagedWidget("command_label",
    labelWidgetClass, props.form,
    NULL);

  props.locker = XtVaCreateManagedWidget("command_text",
    asciiTextWidgetClass, props.form,
    XtNtype, XawAsciiString,
    XtNeditType, XawtextEdit,
    NULL);

  props.test_button = XtVaCreateManagedWidget("test_button",
    commandWidgetClass, props.form,
    NULL);
  XtAddCallback(props.test_button,
    XtNcallback, LockerTest_CB, (XtPointer) 0);

  props.default1_button = XtVaCreateManagedWidget("default1_button",
    commandWidgetClass, props.form,
    NULL);
  XtAddCallback(props.default1_button,
    XtNcallback, Default_CB, (XtPointer) 0);

  props.bell_toggle = XtVaCreateManagedWidget("bell_toggle",
    toggleWidgetClass, props.form,
    NULL);

  props.switch_label = XtVaCreateManagedWidget("switch_label",
    labelWidgetClass, props.form,
    NULL);

  props.switch_keys = XtVaCreateManagedWidget("switch_text",
    asciiTextWidgetClass, props.form,
    XtNtype, XawAsciiString,
    XtNeditType, XawtextEdit,
    NULL);

  props.lat_label = XtVaCreateManagedWidget("lat_label",
    labelWidgetClass, props.form,
    NULL);

  props.to_lat_keys = XtVaCreateManagedWidget("lat_text",
    asciiTextWidgetClass, props.form,
    XtNtype, XawAsciiString,
    XtNeditType, XawtextEdit,
    NULL);

  props.rus_label = XtVaCreateManagedWidget("rus_label",
    labelWidgetClass, props.form,
    NULL);

  props.to_rus_keys = XtVaCreateManagedWidget("rus_text",
    asciiTextWidgetClass, props.form,
    XtNtype, XawAsciiString,
    XtNeditType, XawtextEdit,
    NULL);

  XtVaCreateManagedWidget("switch_for_one_keys_label",
    labelWidgetClass, props.form,
    NULL);

  props.switch_for_one_keys = XtVaCreateManagedWidget("switch_for_one_keys",
    asciiTextWidgetClass, props.form,
    XtNtype, XawAsciiString,
    XtNeditType, XawtextEdit,
    NULL);

  props.default2_button = XtVaCreateManagedWidget("default2_button",
    commandWidgetClass, props.form,
    NULL);
  XtAddCallback(props.default2_button,
    XtNcallback, Default_CB, (XtPointer) 1);

  props.ok_button = XtVaCreateManagedWidget("ok_button",
    commandWidgetClass, props.form,
    NULL);
  XtAddCallback(props.ok_button,
    XtNcallback, Button_CB, (XtPointer) 0);

  props.apply_button = XtVaCreateManagedWidget("apply_button",
    commandWidgetClass, props.form,
    NULL);
  XtAddCallback(props.apply_button,
    XtNcallback, Button_CB, (XtPointer) 1);

  props.save_button = XtVaCreateManagedWidget("save_button",
    commandWidgetClass, props.form,
    NULL);
  XtAddCallback(props.save_button,
    XtNcallback, Button_CB, (XtPointer) 3);

  props.cancel_button = XtVaCreateManagedWidget("cancel_button",
    commandWidgetClass, props.form,
    NULL);
  XtAddCallback(props.cancel_button,
    XtNcallback, Button_CB, (XtPointer) 2);

#endif /* TK_XAW */

   inited=1;
}

void  PropsPopup()
{
   PropsInit();

   if(!XtIsManaged(props.form))
      PropsSet();

   XtManageChild(props.form);

#if TK==TK_MOTIF
{
   int   count;
   Arg   args[10];
   Dimension   width,height;
   static int limit_set=0;

   if(!limit_set)
   {
      StartArgs();
      AddArg(XmNwidth,&width);
      AddArg(XmNheight,&height);
      XtGetValues(props.shell,args,count);
      StartArgs();
      AddArg(XmNminWidth,width);
      AddArg(XmNminHeight,height);
      AddArg(XmNmaxHeight,height);
      XtSetValues(props.shell,args,count);

      limit_set=1;
   }
}
#endif /* TK_MOTIF */

  XtPopup(props.shell, XtGrabNone);
}
#else /* TK_NONE */
void PropsPopup() {}
#endif
