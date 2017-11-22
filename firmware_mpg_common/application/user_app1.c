/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

Test1.
Test2 from Engenuics.
Test3.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  
    /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for input */
static u8 u8c=0;
static u8 u8CharCount=0;
static u8 u8Number4=0;
static u8 u8CharCount2=0;
static u8 au8Arry4[128];
static u8 u8Counter=0;
static u8 au8Arry[20];
static u8 u8Counter2=0;
static void UserApp1SM_Idle(void)
{
  static u8 au8Arry1[128];
  static u8 u8Number1=0;
  static u8 u8Number2=0;
  static u8 u8States1=0;
  static  LedCommandType Command;
  static u8 u8States2=3;
  static u8 u8States3=0;
  static u8 u8States4=1;
  static u8 au8Arry2[]={'W','w','P','p','B','b','C','c','G','g','Y','y','O','o','R','r'};
  static u8 au8Arry3[10];
  static u8 u8i=0;
  static u8 u8a=0;
  static u8 u8b=0;
  static u8 u8e=0;
  static u8 u8Number3=0;
  static u32 u32Number5=0;
  
  static u8 u8States5=1;
  static u8 au8Arry5[128];
  static u8 au8Arry6[128];
  
  
  if(u8States2==3)
  {
    u8States2=0;
    DebugPrintf("\n\r**********************************************************\n\r");
    DebugPrintf("LED Programming Interface\n\r");
    DebugPrintf("Press 1 to program LED command sequence\n\r");
    DebugPrintf("Press 2 to show current USER program\n\r");
    DebugPrintf("Press 3 to enter calculation mode\n\r");
    DebugPrintf("**********************************************************\n\r\n\r");
  }
  if(u8States2==0)
  {
    DebugScanf(au8Arry3);
    if(au8Arry3[0]=='1')
    {
      u8b=0;
      u8c=0;
      au8Arry3[0]=0;
      u8States2=1;
      u8States3=1;
      LedDisplayStartList();
      u8States4=1;
    }
    if(au8Arry3[0]=='2')
    {
      u8i=0;
      au8Arry3[0]=0;
      u8States2=2;
      u8States3=2;
    }
    if(au8Arry3[0]=='3')
    {
      LCDCommand(LCD_CLEAR_CMD);
      au8Arry3[0]=0;
      u8States2=4;
      DebugPrintf("\n\r           calculation mode! \n\r");
      u8Number3=0;
      
    }
  }
  
  if(u8States3==1)
  {
    u8States3=0;
    DebugPrintf("\n\r\n\r\n\rEnter commands as LED-ONTIME-OFFTIME and press Enter\n\r");
    DebugPrintf("Time is in milliseconds,max 100 commands\n\r");
    DebugPrintf("LED colors: R,O,Y,G,C,B,P,W\n\r");
    DebugPrintf("Example: R-100-200(Red on at 100ms an off at 200ms)\n\r");
    DebugPrintf("Press Enter on blank line to end\n\r");
    DebugPrintNumber(1);
    DebugPrintf(":");
  }
  if(u8States3==2)
  {
    u8States3=0;
    DebugPrintf("\n\r\n\r\n\rCurrent USER program:\n\r");
    DebugPrintf("\n\rLED  ON TIME   OFF TIME\n\r");
    DebugPrintf("----------------------------------------\n\r");
  }
  
  if(u8States2==1)                      /*input command*/
  {
    
    
    for(u8Number1=0;u8Number1<G_u8DebugScanfCharCount;u8Number1++)
    {
      if(G_au8DebugScanfBuffer[u8Number1]==0x0D)
      {
        u8CharCount=DebugScanf(au8Arry1);
        
        u8States1=8;
        DebugPrintf("\n\r");
      }
      
    }
    if(u8States1==8)
    {
      
      if(au8Arry1[u8a]==0x08)
      {
        for(u8b=u8a;u8b<u8CharCount;u8b++)
        {
          au8Arry1[u8b]=au8Arry1[u8b+1];
        }
        for(u8b=u8a-1;u8b<u8CharCount;u8b++)
        {
          au8Arry1[u8b]=au8Arry1[u8b+1];
        }
        u8a=0;
        u8c=u8c+2;
      }
      else
      {
        u8a++;
      }
      
      if(u8a==128)
      {
        u8States1=1;
        u8a=0;
      }
    }
    if(u8States1==1)
    {
      
      
      for(u8 i=0;i<16;i++)
      {
        if(au8Arry1[0]==au8Arry2[i])
        {
          u8States1=2;
        }
      }
      if(u8States1!=2)
      {
        u8States1=7;
      }
    }
    
    if(u8States1==2)
    {
      if(au8Arry1[1]==0x2D)
      {
        u8States1=3;
      }
      else
      {
        u8States1=7;
      }
    }
    
    if(u8States1==3)
    {
      for(u8 a=2;a< u8CharCount;a++)
      {
        if(au8Arry1[a]==0x2D)
        {
          u8States1=4;
        }
      }
      if(u8States1!=4)
      {
        u8States1=7;
      }
    }
    if(u8States1==4)
    {
      for(u8 i=2;i< u8CharCount-u8c;i++)
      {
        if(au8Arry1[i]==0x2D)
        {
          u8Number2=i;
        }
      }
      for(u8 i=2;i<u8Number2;i++)
      {
        if(au8Arry1[i]<'0'||au8Arry1[i]>'9')
        {
          u8States1=7;
        }
      }
      if(u8States1!=7)
      {
        u8States1=5;
      }
    }
    if(u8States1==5)
    {
      for(u8 i=u8Number2+1;i< u8CharCount-u8c-2;i++)
      {
        if(au8Arry1[i]<'0'||au8Arry1[i]>'9')
        {
          u8States1=7;
        }
      }
      if(u8States1!=7)
      {
        u8States1=6;
      }
      
    }
    if(u8States1==6)
    {
      
      u8States4++;
      DebugPrintNumber(u8States4);
      DebugPrintf(":");
      u8States1=0;
      Command.eLED=Color(au8Arry1);
      Command.bOn=TRUE;
      Command.u32Time=StartTime(au8Arry1);
      Command.eCurrentRate=LED_PWM_0;
      LedDisplayAddCommand(USER_LIST,&Command);
      
      Command.eLED=Color(au8Arry1);
      Command.bOn=FALSE;
      Command.u32Time=EndTime(au8Arry1);
      Command.eCurrentRate=LED_PWM_0;
      LedDisplayAddCommand(USER_LIST,&Command);
      
    }
    if(u8States1==7)
    {
      if(au8Arry1[0]==0x0D)
      {
        u8States2=3;
        u8States1=0;
      }
      else
      {
        u8States1=0;
        DebugPrintf("Invalid command:incorrect format.Please use L-ONTIME-OFFTIME\n\r\n\r");
        DebugPrintNumber(u8States4);
        DebugPrintf(":");
      }
      
    }
  }
  if(u8States2==2)                             /*display command*/
  {
    if(LedDisplayPrintListLine(u8i))
    {
      u8i++;
    }
    else
    {
      DebugPrintf("---------------------------------------\n\r");
      u8States2=0;
      u8States2=3;
    }
  }
  if(u8States2==4)              /*calculation mode*/
  {
    if(u8States5==0)
    {
      
      if(u8Number3!=G_u8DebugScanfCharCount)
      {
        if(G_au8DebugScanfBuffer[0]==0x1B)
        {
          u8States2=3;
          
        }
        u8Number3++;
        if(u8Number3==1)
        {
          LCDCommand(LCD_CLEAR_CMD);
        }
        LCDMessage(LINE1_END_ADDR-G_u8DebugScanfCharCount+1, G_au8DebugScanfBuffer);
        
        if(G_au8DebugScanfBuffer[G_u8DebugScanfCharCount-1]=='=')
        {
          u8States5=1;
          DebugPrintf("\n\r");
        }
      }
    }
    if(u8States5==1)
    {
      u8Number3=0;
      u8States5=0;
      u8CharCount2=DebugScanf(au8Arry4);
      for(u8 u8d=0;u8d<u8CharCount2-1;u8d++)
      {
        if(au8Arry4[u8d]<'0'||au8Arry4[u8d]>'9')
        {
          u8Number4=u8d;
        }
      }
      for(u8 u8d=0;u8d<u8Number4;u8d++)
      {
        au8Arry5[u8d]=au8Arry4[u8d];
        if(au8Arry4[u8d]<'0'||au8Arry4[u8d]>'9')
        {
          u8States5=2;
          LCDCommand(LCD_CLEAR_CMD);
        }
      }
      for(u8 u8d=u8Number4+1;u8d<u8CharCount2-1;u8d++)
      {
        au8Arry6[u8e]=au8Arry4[u8d];
        u8e++;
        if(au8Arry4[u8d]<'0'||au8Arry4[u8d]>'9')
        {
          
          u8States5=2;
          LCDCommand(LCD_CLEAR_CMD);
        }
      }
      if(u8States5!=2)
      {
        u8e=0;
        u32Number5=jisuan(au8Arry5,au8Arry6);
        u8Arry(u32Number5);
        if(u8Counter==1)
        {
          DebugPrintNumber(u32Number5);
          DebugPrintf("\n\r");
          LCDMessage(LINE2_END_ADDR-u8Counter2+1,au8Arry);
        }
        if(u8Counter==2)
        {
          DebugPrintf("-");
          DebugPrintNumber(u32Number5);
          DebugPrintf("\n\r");
          LCDMessage(LINE2_END_ADDR-u8Counter2,"-");
          LCDMessage(LINE2_END_ADDR-u8Counter2+1,au8Arry);
        }
      }
      
    }
    if(u8States5==2)
    {
      u8e=0;
      u8States5=0;
      
      LCDMessage(LINE1_START_ADDR+8,"Error!");
    }
  }
  
} /* end UserApp1SM_Idle() */

u8 Color(u8 *au8Arry)
{
  static u8 u8Number=0;
  static u8 u8Number2=0;
  static u8 au8Color1[]={'W','P','B','C','G','Y','O','R'};
  static u8 au8Color2[]={'w','p','b','c','g','y','o','r'};
  static LedNumberType au8Color3[]={WHITE,PURPLE, BLUE, CYAN, GREEN, YELLOW, ORANGE, RED};
  for(u8Number=0;u8Number<8;u8Number++)
  {
    if(au8Color1[u8Number]==au8Arry[0])
    {
      u8Number2=u8Number;
    }
  }
  for(u8Number=0;u8Number<8;u8Number++)
  {
    if(au8Color2[u8Number]==au8Arry[0])
    {
      u8Number2=u8Number;
    }
  }
  return(au8Color3[u8Number2]);
}
u32 StartTime(u8 *au8Arry)
{
  u8 u8Number2=0;
  u8 u8Number1;
  u32 u32Number3=0;
  u8 u8Number4=2;
  for(u8Number1=2;u8Number1< u8CharCount-u8c;u8Number1++)
  {
    if(au8Arry[u8Number1]==0x2D)
    {
      u8Number2=u8Number1;
    }
  }
  while(u8Number4<u8Number2)
  {
    u32Number3=u32Number3*10+*(au8Arry+2)-'0';
    au8Arry++;
    u8Number4++;
  }
  
  return u32Number3;
}
u32 EndTime(u8 *au8Arry)
{
  u8 u8Number2=0;
  u8 u8Number1;
  u32 u32Number3=0;
  for(u8Number1=2;u8Number1< u8CharCount-u8c;u8Number1++)
  {
    if(au8Arry[u8Number1]==0x2D)
    {
      u8Number2=u8Number1;
    }
  }
  while((u8Number2+1)<( u8CharCount-u8c-1))
  {
    u32Number3=u32Number3*10+*(au8Arry+u8Number2+1)-'0';
    u8Number2++;
  }
  return u32Number3;
}  

u32 jisuan(u8 *au8Arry1,u8 *au8Arry2)
{
  u8 u8Number1=0;
  u8 u8Number2=0;
  u32 u32Number3=0;
  u32 u32Number4=0;
  
  while(u8Number1<u8Number4)
  {
    
    u32Number3=u32Number3*10+*(au8Arry1+u8Number1)-'0';
    u8Number1++;
  }
  while(u8Number2<u8CharCount2-u8Number4-2)
  {
    
    u32Number4=u32Number4*10+*(au8Arry2+u8Number2)-'0';
    u8Number2++;
  }
  if(au8Arry4[u8Number4]=='+')
  {
    u8Counter=1;
    return (u32Number3+u32Number4);
  }
  if(au8Arry4[u8Number4]=='-')
  {
    if(u32Number3>=u32Number4)
    {
      u8Counter=1;
      return (u32Number3-u32Number4);
    }
    else
    {
      u8Counter=2;
      return (u32Number4-u32Number3);
    }
  }
  if(au8Arry4[u8Number4]=='*')
  {
    u8Counter=1;
    return (u32Number3*u32Number4);
  }
  if(au8Arry4[u8Number4]=='/')
  {
    u8Counter=1;
    return (u32Number3/u32Number4);
  }
}

u8 u8Arry(u32 Number)
{
  u8 u8Number1=0;
  u8 u8a=0;
  u32 Number1=1;
  
  
  for(u8 u8i=0;u8i<20;u8i++)
  {
    Number1=Number1*10;
    if(Number/Number1==0)
    {
      u8Number1=u8i+1;
      u8i=20;
      u8Counter2=u8Number1;
    }
  }
  for(u8 u8i=0;u8i<20;u8i++)
  {
    Number1=Number1/10;
    if(u8Number1!=0)
    {
      
      au8Arry[u8a]=Number/Number1+'0';
      Number=Number%Number1;
      u8Number1--;
      u8a++;
    }
    if(u8Number1==0)
    {
      u8i=20;
    }
  }
  return *au8Arry;
}
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
  
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
