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
extern bool bConvertCom;

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
  CD4053Initialize();
  X9C103Initialize();
  AT91C_BASE_PIOA->PIO_SODR   = 0X00000200;
  Adc12AssignCallback(ADC12_CH2,Adc12DefaultCallback);
  LCDClearChars(LINE1_START_ADDR , 20);
  LCDClearChars(LINE2_START_ADDR , 20);
  LCDMessage(LINE1_START_ADDR, "Press Button3");
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
  static u8 u8State = 0;
  static u8 u8Measer = 1;
  static u8 u8Counter = 0;
  static u8 a,b;
  static u16 u16ConvertResult;
  static u8 u8Level = 0;
  static u8 au8DisplayLevel[]="Current Level 0";
  static u8 au8DisplayAccess1[]="AUD1";
  static u8 au8DisplayAccess2[]="AUD2";
  static u8 u8Location = 0;
  static u8 au8Location[]="location xx%";
  
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    IncreaseSound();
    LedOn(RED);
    for(a=0;a<100;a++)
    {
      for(b=0;b<100;b++)
      {
        
      }
    }
    LedOff(RED);
    u8Level++;
    if(u8Level>9)
    {
      u8Level=9;
    }
    au8DisplayLevel[14]=HexToASCIICharLower(u8Level);
    LCDClearChars(LINE1_START_ADDR , 20);
    LCDMessage(LINE1_START_ADDR, au8DisplayLevel);
  }
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    DecreaseSound();
    LedOn(RED);
    for(a=0;a<100;a++)
    {
      for(b=0;b<100;b++)
      {
        
      }
    }
    LedOff(RED);
    u8Level--;
    if(u8Level==0xFF)
    {
      u8Level=0;
    }
    au8DisplayLevel[14]=HexToASCIICharLower(u8Level);
    LCDClearChars(LINE1_START_ADDR , 20);
    LCDMessage(LINE1_START_ADDR, au8DisplayLevel);
  }
  
  
  if(WasButtonPressed(BUTTON3))
  {
    LCDClearChars(LINE1_START_ADDR , 20);
    LCDMessage(LINE1_START_ADDR, au8DisplayLevel);
    ButtonAcknowledge(BUTTON3);
    LedOn(RED);
    for(a=0;a<100;a++)
    {
      for(b=0;b<100;b++)
      {
        
      }
    }
    LedOff(RED);
    if(u8State == 0)
    {
      SelectState(OutPutAUD1);
      u8State = 1;
      LedOn(BLUE);
      LedOff(GREEN);
      LedOff(PURPLE);
      LCDClearChars(LINE2_START_ADDR , 20);
      LCDMessage(LINE2_START_ADDR, au8DisplayAccess1);
    }
    else  if(u8State == 1)
    {
      SelectState(OutPutAUD2);
      u8State = 2;
      LedOn(GREEN);
      LedOff(BLUE);
      LedOff(PURPLE);
      LCDClearChars(LINE2_START_ADDR , 20);
      LCDMessage(LINE2_START_ADDR, au8DisplayAccess2);
    }
    else  if(u8State == 2)
    {
      SelectState(OutPutGND);
      u8State = 0;
      LedOn(PURPLE);
      LedOff(GREEN);
      LedOff(BLUE);
      LCDClearChars(LINE2_START_ADDR , 20);
      LCDMessage(LINE2_START_ADDR, "no sound");
    }
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    if(u8Measer == 0)
    {
      u8Measer = 1;
      AT91C_BASE_PIOB->PIO_CODR   = 0X00000010;
      LedOff(WHITE);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, "select access");
    }
    else if(u8Measer == 1)
    {
      SelectState(OutPutHigh);
      u8Measer=2;
      AT91C_BASE_PIOB->PIO_SODR   = 0X00000010;
      LedOn(WHITE);
    }
    
    
  }
  
  if(u8Measer == 2)
  {
    u8Counter++;
    if(u8Counter==100)
    {
      u8Counter=0;
      Adc12StartConversion(ADC12_CH2);
    }
    if(bConvertCom)
    {
      bConvertCom = FALSE;
      u16ConvertResult=AT91C_BASE_ADC12B->ADC12B_CDR[ADC12_CH2];
    
    u8Location=u16ConvertResult*100/0XFFF;
    au8Location[9]=HexToASCIICharLower(u8Location/10);
    au8Location[10]=HexToASCIICharLower(u8Location%10);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, au8Location);
    u8Measer=0;
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
