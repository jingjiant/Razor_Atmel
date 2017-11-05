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
    static u8 u8NumCharsMessage[] = "\n\rError,Please Printf'Q'~'U','1'~'7','A'~'J' ";
    static u32 au32Tone[7][3]=
    {
      {262,523,1046},
      {294,578,1175},
      {330,628,1318},
      {349,698,1397},
      {392,784,1568},
      {440,880,1760},
      {494,998,1976}
    };
    static u8 au8ArryHigh[]={'Q','W','E','R','T','Y','U'};
    static u8 au8ArryMedium[]={'1','2','3','4','5','6','7'};
    static u8 au8ArryLow[]={'A','S','D','F','G','H','J'};
    static u8 au8ArryError[]={'Q','W','E','R','T','Y','U','1','2','3','4','5','6','7','A','S','D','F','G','H','J'};/*judge the button whether correct*/
    u8 u8CharCount;
    static u8 u8Number=0; 
    static u8 u8Number2=0;
    bool bFlag=FALSE;
    
    u8CharCount = DebugScanf(UserApp_au8UserInputBuffer);
    UserApp_au8UserInputBuffer[u8CharCount] = '\0';
    
    for(u8 i=0;i<7;i++)         /*high pitch*/
        {
            if(au8ArryHigh[i]==UserApp_au8UserInputBuffer[0])
            {
              u8Number=i;
              u8Number2=2;
            }
        }
    
    for(u8 i=0;i<7;i++)         /*alto voice*/
        {
            if(au8ArryMedium[i]==UserApp_au8UserInputBuffer[0])
            {
              u8Number=i;
              u8Number2=1;     
            }
        }
    
    for(u8 i=0;i<7;i++)         /*low pitch*/        
        {
            if(au8ArryLow[i]==UserApp_au8UserInputBuffer[0])
            {
              u8Number=i;
              u8Number2=0;        
            }
        }

    for(u8 i=0;i<21;i++)        /*judge the button whether correct*/
        {
            if(au8ArryError[i]==UserApp_au8UserInputBuffer[0])
            {
              bFlag=TRUE;         
            }
        }

    if(u8CharCount)
        {
            PWMAudioSetFrequency(BUZZER1,au32Tone[u8Number][u8Number2]);
            if(!bFlag)          /*if button error,printf error*/
              {
                DebugPrintf(u8NumCharsMessage);
              }
        }
      PWMAudioOn(BUZZER1);
    
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
