/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

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
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/

static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */



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
  PWMAudioSetFrequency(BUZZER1, 500);
  AntAssignChannelInfoType sAntSetupData;
 /* Configure ANT for this application */
  sAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  sAntSetupData.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  sAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  
  sAntSetupData.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

  sAntSetupData.AntNetwork = ANT_NETWORK_DEFAULT;

  sAntSetupData.AntNetworkKey[0] = N_0;
  sAntSetupData.AntNetworkKey[1] = N_1;
  sAntSetupData.AntNetworkKey[2] = N_2;
  sAntSetupData.AntNetworkKey[3] = N_3;
  sAntSetupData.AntNetworkKey[4] = N_4;
  sAntSetupData.AntNetworkKey[5] = N_5;
  sAntSetupData.AntNetworkKey[6] = N_6;
  sAntSetupData.AntNetworkKey[7] = N_7;
  
    
  /* If good initialization, set state to Idle */
  if( AntAssignChannel(&sAntSetupData) )
  {
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
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
/* Wait for the ANT channel assignment to finish */
static void UserApp1SM_WaitChannelAssign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
      
} /* end UserApp1SM_WaitChannelAssign() */


  /*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static bool bLcdDisplay = TRUE;
static void UserApp1SM_Idle(void)
{
  AntReadAppMessageBuffer();
  if(bLcdDisplay)
  {
    bLcdDisplay = FALSE;
    LCDMessage(LINE1_START_ADDR, "initialized succeed");
    LCDMessage(LINE2_START_ADDR, "pressed b0 to start");
  }
  /* Look for BUTTON 0 to open channel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button, so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON0);
    LCDClearChars(LINE1_START_ADDR, 20);
    LCDClearChars(LINE2_START_ADDR, 20);
    LCDMessage(LINE1_START_ADDR, "wait message...");
    /* Queue open channel and change LED0 from yellow to blinking green to indicate channel is opening */
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
  }
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to open */
static void UserApp1SM_WaitChannelOpen(void)
{
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {  
    UserApp1_StateMachine = UserApp1SM_ChannelOpen;
  }
  
  /* Check for timeout */
    
} /* end UserApp1SM_WaitChannelOpen() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Channel is open, so monitor data */
static void UserApp1SM_ChannelOpen(void)
{
  static u8 au8RateContent[] = "Rate:xxxbpm/min";
  static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  static u8 au8TestMessage[] = {0x46, 0xFF, 0xFF, 0xFF, 0xFF, 0x84, 0x07, 0x01};
  static u16 u16Counter;
  static u8 au8BatteryStatus[] = "Battery Level:xxx%";
  static u8 u8HeartRate = 0;
  static u8 au8DebugContent[] = "----------                                                  ";     
  
  u16Counter++;
  /* Check for BUTTON0 to close channel */
  
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP,au8TestMessage);
    
  }
    if(WasButtonPressed(BUTTON2))
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    ButtonAcknowledge(BUTTON2);
    UserApp1_StateMachine = UserApp1SM_Idle;
    bLcdDisplay = TRUE; 
    LedOff(RED);
    PWMAudioOff(BUZZER1);
  }
  /* Always check for ANT messages */
  if( AntReadAppMessageBuffer() )
  {
     /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      
      /* Check if the new data is the same as the old data and update as we go */
      if(G_au8AntApiCurrentMessageBytes[6] != au8LastAntData[6])
      {
        for(u8 i=10;i<60;i++)
        {
          au8DebugContent[i] = ' ';
        }
        u8HeartRate = G_au8AntApiCurrentMessageBytes[7];
        for(u8 a=0;a<(u8HeartRate-60);a++)
        {
          au8DebugContent[a+10] = '-';
        }
        DebugPrintf(au8DebugContent);
        DebugPrintf("\n\r");
        u16Counter = 0;
        au8RateContent[5] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[7]/100);
        au8RateContent[6] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[7]%100/10);
        au8RateContent[7] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[7]%100%10);
        LCDClearChars(LINE1_START_ADDR, 20);
        LCDMessage(LINE1_START_ADDR, au8RateContent); 
      }
      if((u8HeartRate>100)||(u8HeartRate<60))
      {
        LedOn(RED);
        PWMAudioOn(BUZZER1);
      }
      else
      {
        LedOff(RED);
        PWMAudioOff(BUZZER1);
      }
      if(u16Counter>1500)
      {
        LedBlink(RED,LED_4HZ);
      }
      if((G_au8AntApiCurrentMessageBytes[0] == 0x07)||(G_au8AntApiCurrentMessageBytes[0] == 0x87))
      {
        au8BatteryStatus[14] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[1]/100);
        au8BatteryStatus[15] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[1]%100/10);
        au8BatteryStatus[16] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[1]%100%10);
        LCDMessage(LINE2_START_ADDR, au8BatteryStatus);
      }
      for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++)
      {
        if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i])
        {
          au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];
        }
      }
               

    } /* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */
    
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      if(G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX] == 0x01)
      {
        UserApp1_StateMachine = UserApp1SM_Idle;
        bLcdDisplay = TRUE;
        LedOff(RED);
        PWMAudioOff(BUZZER1);
      }
      if(G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX] == 0x08)
      {
         LCDMessage(LINE1_START_ADDR, "RX_FAIL_GO_TO_SEARCH");
      }
    }
  } /* end AntReadAppMessageBuffer() */
  

      
} /* end UserApp1SM_ChannelOpen() */



/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
