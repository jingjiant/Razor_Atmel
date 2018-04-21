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
extern u32 G_u32AntApiCurrentMessageTimeStamp;                    /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;              /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                        /* Timeout counter used across states */
static bool bMode = TRUE;
static AntAssignChannelInfoType UserApp1_sChannelInfo; /* ANT setup parameters */

static u8 UserApp1_au8MessageFail[] = "\n\r***ANT channel setup failed***\n\n\r";

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
  u8 au8WelcomeMessage1[] = "Hide and Go Seek!";
  u8 au8WelcomeMessage2[] = "Press B0 to Start!";
  /* Write a weclome message on the LCD */
#if EIE1
  /* Set a message up on the LCD. Delay is required to let the clear command send. */
  LCDCommand(LCD_CLEAR_CMD);
  for(u32 i = 0; i < 10000; i++);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage1);
   LCDMessage(LINE2_START_ADDR, au8WelcomeMessage2);
#endif /* EIE1 */
  
#if 0 // untested for MPG2
  
#endif /* MPG2 */

 /* Configure ANT for this application */
  UserApp1_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  UserApp1_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  UserApp1_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
 
  UserApp1_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  UserApp1_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  UserApp1_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;

  UserApp1_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
  /* Attempt to queue the ANT channel setup */
  if( AntAssignChannel(&UserApp1_sChannelInfo) )
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_AntChannelAssign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    DebugPrintf(UserApp1_au8MessageFail);
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
/* Wait for ANT channel assignment */
static void UserApp1SM_AntChannelAssign()
{
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    /* Channel assignment is successful, so open channel and
    proceed to Idle state */
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 3000))
  {
    DebugPrintf(UserApp1_au8MessageFail);
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
     
} /* end UserApp1SM_AntChannelAssign */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  AntReadAppMessageBuffer();
  if( WasButtonPressed(BUTTON0) )
  {
     ButtonAcknowledge(BUTTON0);
     LCDCommand(LCD_CLEAR_CMD);
     LCDMessage(LINE1_START_ADDR,"ARE YOU READY?");
     UserApp1_StateMachine = UserApp1SM_StartTime;
  }
  
  
  
 
  
  
} /* end UserApp1SM_Idle() */

static void UserApp1SM_Hide(void)
{
  static u8 au8TrueMessage[]={1, 1, 1, 1, 1, 1, 1, 1};
  static u8 au8TestMessage[] = {1, 2, 3, 4, 0xFF, 0, 0, 0};
  static u8 u8Counter=0;
  
  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {

      u8Counter=0;
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        if(G_au8AntApiCurrentMessageBytes[i] == au8TrueMessage[i])
        {
          u8Counter++;
        }
      }
      if(u8Counter==8)
      {
        AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
        //AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TrueMessage);
        LCDClearChars(LINE1_START_ADDR, 20);
        LCDMessage(LINE1_START_ADDR, "YOU FUND ME!");
        bMode = FALSE;
        UserApp1_StateMachine = UserApp1SM_AntCloseChannel;
        
        
      }
      
     

    }
    
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      au8TestMessage[7]++;
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
    }
  }
}

static void UserApp1SM_AntCloseChannel(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CLOSED)
  {
   
    AntUnassignChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_AntUnassignChannel;
    
  }
  
}




static void UserApp1SM_AntUnassignChannel(void)
{

   
  
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_UNCONFIGURED)
  {
    for(u32 i=0;i<1000;i++){
   for(u32 j=0;j<5000;j++);
    }
    if(bMode)
    {
    UserApp1_sChannelInfo.AntChannelType          = CHANNEL_TYPE_MASTER;
    AntAssignChannel(&UserApp1_sChannelInfo);  
    UserApp1_StateMachine = UserApp1SM_AntAssignChannel;
    }
    else
    {
    UserApp1_sChannelInfo.AntChannelType          = CHANNEL_TYPE_SLAVE;
    AntAssignChannel(&UserApp1_sChannelInfo);  
    UserApp1_StateMachine = UserApp1SM_AntAssignChannel;
    }

  }
}

static void UserApp1SM_AntAssignChannel(void)
{
   
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_AntOpenChannel;
  }
}

static void UserApp1SM_AntOpenChannel(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
    UserApp1_StateMachine = UserApp1SM_StartTime;
  }
}

static void UserApp1SM_Seek(void)
{
  static u8 u8Counter = 0;
  static u8 au8TrueMessage[]={2, 2, 2, 2, 2, 2, 2, 2};
  static u8 au8RssiMessage[]="RSSI:-xxx dbm";
  static s8 s8RssiChannel0;
  static bool bBuzzer1State = TRUE;
  
  if( WasButtonPressed(BUTTON1) )
  {
    ButtonAcknowledge(BUTTON1);
    if(bBuzzer1State)
    {
      bBuzzer1State = FALSE;
      PWMAudioOn(BUZZER1);
    }
    else
    {
      bBuzzer1State = TRUE;
      PWMAudioOff(BUZZER1);
    }
  }
  
  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      u8Counter++;
      if(u8Counter == 4)
      {
      u8Counter = 0;
      s8RssiChannel0 = G_sAntApiCurrentMessageExtData.s8RSSI;
      s8RssiChannel0 = s8RssiChannel0/(-1);
      au8RssiMessage[6] = HexToASCIICharUpper(s8RssiChannel0/100);
      au8RssiMessage[7] = HexToASCIICharUpper(s8RssiChannel0%100/10);
      au8RssiMessage[8] = HexToASCIICharUpper(s8RssiChannel0%100%10);    
      LCDClearChars(LINE2_START_ADDR, 20);
      LCDMessage(LINE2_START_ADDR, au8RssiMessage);
      
      if(s8RssiChannel0>95)
      {
        UserApp1SM_AllLedOff();
      }
      if(85<s8RssiChannel0&&s8RssiChannel0<=95)
      {
        UserApp1SM_AllLedOff();
       LedPWM(RED,LED_PWM_5);
        PWMAudioSetFrequency(BUZZER1, 150);
      }
      if(75<s8RssiChannel0&&s8RssiChannel0<=85)
      {
        UserApp1SM_AllLedOff();
        LedPWM(ORANGE,LED_PWM_10);
        LedPWM(RED,LED_PWM_5);
        PWMAudioSetFrequency(BUZZER1, 200);
      }
      if(65<s8RssiChannel0&&s8RssiChannel0<=75)
      {
        UserApp1SM_AllLedOff();
        LedPWM(YELLOW,LED_PWM_15);
        LedPWM(ORANGE,LED_PWM_10);
        LedPWM(RED,LED_PWM_5);
        PWMAudioSetFrequency(BUZZER1, 250);
      }
      if(60<s8RssiChannel0&&s8RssiChannel0<=65)
      {
        UserApp1SM_AllLedOff();
        LedPWM(GREEN,LED_PWM_20);
        LedPWM(YELLOW,LED_PWM_15);
        LedPWM(ORANGE,LED_PWM_10);
        LedPWM(RED,LED_PWM_5);
        PWMAudioSetFrequency(BUZZER1, 300);
      }
      if(55<s8RssiChannel0&&s8RssiChannel0<=60)
      {
        UserApp1SM_AllLedOff();
        LedPWM(CYAN,LED_PWM_25);
        LedPWM(GREEN,LED_PWM_20);
        LedPWM(YELLOW,LED_PWM_15);
        LedPWM(ORANGE,LED_PWM_10);
        LedPWM(RED,LED_PWM_5);
        PWMAudioSetFrequency(BUZZER1, 350);
      }
      if(50<s8RssiChannel0&&s8RssiChannel0<=55)
      {
        UserApp1SM_AllLedOff();
        LedPWM(BLUE,LED_PWM_30);
        LedPWM(CYAN,LED_PWM_25);
        LedPWM(GREEN,LED_PWM_20);
        LedPWM(YELLOW,LED_PWM_15);
        LedPWM(ORANGE,LED_PWM_10);
        LedPWM(RED,LED_PWM_5);
        PWMAudioSetFrequency(BUZZER1, 400);
      }
      if(45<s8RssiChannel0&&s8RssiChannel0<=50)
      {
        UserApp1SM_AllLedOff();
        LedPWM(PURPLE,LED_PWM_35);
        LedPWM(BLUE,LED_PWM_30);
        LedPWM(CYAN,LED_PWM_25);
        LedPWM(GREEN,LED_PWM_20);
        LedPWM(YELLOW,LED_PWM_15);
        LedPWM(ORANGE,LED_PWM_10);
        LedPWM(RED,LED_PWM_5);
        PWMAudioSetFrequency(BUZZER1, 450);
      }
      if(s8RssiChannel0<=45)
      {
        UserApp1SM_AllLedOff();
        LedPWM(WHITE,LED_PWM_40);
        LedPWM(PURPLE,LED_PWM_35);
        LedPWM(BLUE,LED_PWM_30);
        LedPWM(CYAN,LED_PWM_25);
        LedPWM(GREEN,LED_PWM_20);
        LedPWM(YELLOW,LED_PWM_15);
        LedPWM(ORANGE,LED_PWM_10);
        LedPWM(RED,LED_PWM_5);
        AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TrueMessage);
        PWMAudioSetFrequency(BUZZER1, 500);
        LCDClearChars(LINE1_START_ADDR, 20);
        LCDMessage(LINE1_START_ADDR, "I FUND YOU!");
        bMode = TRUE;
        UserApp1_StateMachine = UserApp1SM_AntCloseChannel;
         AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
        
      }   
    }
      
    }
    
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      if(G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX]==0x06)
      {
        UserApp1SM_AllLedOff();
      }
                  if(G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX]==0x01)
      {
        AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
      }
    }
    
  }
  
  
}


static void UserApp1SM_StartTime(void)
{
  static u16 u16StartTime = 0;
  static u8 u8TimeDisplay = 0;
  static u8 au8TimeDisplay[]="Time left: x";
  u16StartTime++;
  AntReadAppMessageBuffer();
  if(bMode)
  {
    if(u16StartTime==2000)
    {
      PWMAudioOff(BUZZER1);
      UserApp1SM_AllLedOff();
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Hide!");
    }
    if(u16StartTime == 12000)
    {
      LCDClearChars(LINE2_START_ADDR, 20);
      UserApp1_StateMachine = UserApp1SM_Hide;
      u16StartTime = 0;
    }
    if(u16StartTime>2000)
    {
      if(u8TimeDisplay!=((12000-u16StartTime)/1000))
      {
        u8TimeDisplay = (12000-u16StartTime)/1000;
        au8TimeDisplay[11] = HexToASCIICharUpper(u8TimeDisplay);
        LCDClearChars(LINE2_START_ADDR, 20);
        LCDMessage(LINE2_START_ADDR,au8TimeDisplay);
      }
    }
  }
  else
  {
    if(u16StartTime==2000)
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, "Seek!");
      
    }
    if(u16StartTime == 12000)
    {
      LCDMessage(LINE1_START_ADDR, "Ready or not");
      LCDMessage(LINE2_START_ADDR, "Here I come!");
      UserApp1_StateMachine = UserApp1SM_Seek;
      u16StartTime = 0;
    }
    if(u16StartTime>2000)
    {
      if(u8TimeDisplay!=((12000-u16StartTime)/1000))
      {
        u8TimeDisplay = (12000-u16StartTime)/1000;
        au8TimeDisplay[11] = HexToASCIICharUpper(u8TimeDisplay);
        LCDClearChars(LINE2_START_ADDR, 20);
        LCDMessage(LINE2_START_ADDR,au8TimeDisplay);
      }
    }
  }
}


static void UserApp1SM_AllLedOff(void)
{
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error (for now, do nothing) */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
