/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


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
static u8 UserApp_au8UserInputBuffer[U16_USER_INPUT_BUFFER_SIZE];
void UserApp1Initialize(void)
{
  for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE  ; i++)
  {
    UserApp_au8UserInputBuffer[i] = 0;
  }
  
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, "Press Button3!");
  LCDMessage(LINE2_START_ADDR, "Enter select mode!");
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
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
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u32 u8Number6=100;
  static u8 u8States=0;
  static u8 UserApp_NameStart1;
  static u8 UserApp_NameStart2;
  static LedRateType Blink[]={LED_0_5HZ,LED_1HZ,LED_2HZ,LED_4HZ,LED_8HZ};
  static LedRateType Pwm[]={LED_PWM_15,LED_PWM_30,LED_PWM_50,LED_PWM_70,LED_PWM_100};
  static u8 u8Number7=0;
  static u16 u8Number8=0;
  u8 u8CharCount;
  static u8 u8Number1=0; 
  static u8 u8Number2=0; 
  static u8 u8Number3=0; 
  static u8 u8Number4=0; 
  static u8 u8Number5=0; 
  static u8 u8Addres1=LINE1_START_ADDR;
  static u8 u8Addres2=LINE2_START_ADDR;
  static u8 au8Arry[200];
  static u8 au8Arry2[20];
  static bool bFlag=FALSE;
  
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    u8States=1;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Press BUTTON0!");
    LCDMessage(LINE2_START_ADDR, "Press BUTTON1!");
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    u8Number1=0;
    u8Number2=0;
    u8Number3=0;
    u8Addres1=LINE1_START_ADDR;
  }
  
  if(u8States==1)
  {
    if(WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      u8States=2;
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, "JingJian");
      UserApp_NameStart1 = LINE1_START_ADDR;
      UserApp_NameStart2=LINE2_START_ADDR;
    }
    if(WasButtonPressed(BUTTON1))
    {
      LCDCommand(LCD_CLEAR_CMD);
      ButtonAcknowledge(BUTTON1);
      u8States=3;
    }
  }
  if(u8States==2)
  {
    if(WasButtonPressed(BUTTON1))
    {
      if(u8Number7==4)
      {
        u8Number7=0;
      }
      else
      {
        u8Number7++;
      }
      ButtonAcknowledge(BUTTON1);
      PWMAudioOff(BUZZER2);
      u8Number6=0;
      if(UserApp_NameStart2==LINE2_END_ADDR-7)
      {
        PWMAudioSetFrequency(BUZZER1,200);
        PWMAudioOn(BUZZER1);
      }
      
      else
      {
        if(UserApp_NameStart1==0x14)
        {
          UserApp_NameStart2++;
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(UserApp_NameStart2, "JingJian");
        }
        else
        {
          UserApp_NameStart1++;      
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(UserApp_NameStart1, "JingJian");
          LCDMessage(UserApp_NameStart1+20, "JingJian");
        }
      }
    }
    if(WasButtonPressed(BUTTON0))
    {
      if(u8Number7==0)
      {
        u8Number7=4;
      }
      else
      {
        u8Number7--;
      }
      ButtonAcknowledge(BUTTON0);
      PWMAudioOff(BUZZER1);
      u8Number6=0;
      if(UserApp_NameStart1==LINE1_START_ADDR)
      {
        PWMAudioSetFrequency(BUZZER2,2000);
        PWMAudioOn(BUZZER2);
      }  
      else
      {
        if(UserApp_NameStart2==0x40)
        {
          UserApp_NameStart1--;
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(UserApp_NameStart1, "JingJian");
          LCDMessage(UserApp_NameStart1+20, "JingJian");
        }
        else
        {
          UserApp_NameStart2--;      
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(UserApp_NameStart2, "JingJian");
        }
      }
    }
    u8Number8++;
    if(u8Number8==2000)
    {
      u8Number8=0;
      LedPWM(WHITE,Pwm[u8Number7]);
      LedPWM(BLUE,Pwm[u8Number7]);
      LedPWM(GREEN,Pwm[u8Number7]);
      LedPWM(ORANGE,Pwm[u8Number7]);
      LedBlink(PURPLE,Blink[u8Number7]);
      LedBlink(CYAN,Blink[u8Number7]);
      LedBlink(YELLOW,Blink[u8Number7]);
      LedBlink(RED,Blink[u8Number7]);
    }
  }
  if(u8States==3)
  {
    u8CharCount = DebugScanf(UserApp_au8UserInputBuffer);
    UserApp_au8UserInputBuffer[u8CharCount] = '\0';
    if(u8CharCount==1)
    {
      if(u8Number1==20)
      {
        au8Arry[u8Number3]=UserApp_au8UserInputBuffer[0];
        u8Number3++;
      }
      if(u8Number1<20)
      {
        if(UserApp_au8UserInputBuffer[0]==0x0D)
        {   
          LCDClearChars(LINE2_START_ADDR,20 );
          DebugPrintf("\n\rHave a newline!\n\r");
          u8Number1=19;
          u8Number2=0;
          u8Number3=0;
          u8Addres2=LINE2_START_ADDR;
        }   
        LCDMessage(u8Addres1+u8Number1,UserApp_au8UserInputBuffer);
        u8Number1++;
        if(u8Number1==20)
        {
          u8Number2=0;
          LCDClearChars(LINE2_START_ADDR,20 );
        }   
      }
      else   if(u8Number1==20)
      {
        if(UserApp_au8UserInputBuffer[0]==0x0D)
        {
          LCDClearChars(LINE1_START_ADDR,20 );
          DebugPrintf("\n\rHave a newline!\n\r");
          u8Number1=0;
          u8Number3=0;
          u8Addres1=LINE1_START_ADDR;
        }
        if(u8Number2<20)
        {
          LCDMessage(u8Addres2+u8Number2,UserApp_au8UserInputBuffer);
          u8Number2++;
        }
        else
        {
          bFlag=TRUE;
        }
      }
      if(bFlag)
      {
        if(UserApp_au8UserInputBuffer[0]==0x0D)
        {
          LCDClearChars(LINE1_START_ADDR,20 );
          DebugPrintf("\n\rHave a newline!\n\r");
          u8Number1=0;
          u8Number3=0;
          u8Addres1=LINE1_START_ADDR;
          bFlag=FALSE;
        }
        u8Number4=u8Number3-20;
        for(u8Number5=0;u8Number5<20;u8Number5++)
        {
          au8Arry2[u8Number5]=au8Arry[u8Number4];
          u8Number4++;
        }  
        LCDMessage(LINE2_START_ADDR, au8Arry2);
      }    
    }  
  }
  
  u8Number6++;
  if(u8Number6<100)
  {
    PWMAudioSetFrequency(BUZZER1,523);
    PWMAudioOn(BUZZER1);
  }
  else
  {   
    if(UserApp_NameStart2!=LINE2_END_ADDR-7)
    {
      PWMAudioOff(BUZZER1);
    }
  }  
} /* end UserApp1SM_Idle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
