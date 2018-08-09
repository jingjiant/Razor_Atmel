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
volatile u8 u8CharNumber;

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */
extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
extern u8 G_u8DebugScanfCharCount;  /* From debug.c */
extern bool bNewMessage;
extern u8 spi_slave_au8Message[128];

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
extern  u8 au8DataMessage[16][10];
extern  u8 au8CharSave[][16];

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
  
  AT91C_BASE_PIOA->PIO_PER    = 0X0000D9F9;           
  AT91C_BASE_PIOA->PIO_OER    = 0X0000D9F9;
  AT91C_BASE_PIOA->PIO_SODR   = 0X00000020;
  AT91C_BASE_PIOA->PIO_CODR   = 0X000001D9;
  AT91C_BASE_PIOA->PIO_PER    = 0x00000600;
  AT91C_BASE_PIOA->PIO_OER    = 0x00000400;
  
  AT91C_BASE_PIOB->PIO_PER    = 0X000001E0;
  AT91C_BASE_PIOB->PIO_OER    = 0X000001C0;
  AT91C_BASE_PIOB->PIO_ODR    = 0X00000020;
  
  
  At24c02_Serial_Read(0x00,&au8CharSave[0][0],256,0XA1,0XA0);
  At24c02_Serial_Read(0x00,&au8CharSave[16][0],256,0XA3,0XA2);
  At24c02_Serial_Read(0x00,&au8CharSave[32][0],256,0XA5,0XA4);
  At24c02_Serial_Read(0x00,&au8CharSave[48][0],256,0XA7,0XA6);
  At24c02_Serial_Read(0x00,&au8CharSave[64][0],256,0XA9,0XA8);
  At24c02_Serial_Read(0x00,&au8CharSave[80][0],256,0XAB,0XAA);
  At24c02_Serial_Read(0x00,&au8CharSave[96][0],256,0XAD,0XAC);
  At24c02_Serial_Read(0x00,&au8CharSave[112][0],256,0XAF,0XAE);
  u8CharNumber = au8CharSave[112][0];
  
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
  static u8 u8hangshu = 0;
  static u16 u16Count = 0;
  static u8 au8Char[129];
  static bool bFlag;
  static u8 au8uBleMessage[18];
  static u8 u8NumberByte=0;
  
  Clear();
  display(u8hangshu,au8DataMessage);
  u8hangshu++;
  if(u8hangshu==16)
  {
    u8hangshu = 0;
  }
  
  u16Count++;
  
  if(u16Count==150)
  {
    CharData();
    u16Count = 0;
  }
  

    if(G_au8DebugScanfBuffer[G_u8DebugScanfCharCount-1]==0x0D)
    {
      u8CharNumber=DebugScanf(au8Char);
      bFlag = TRUE;
      if(au8Char[0]==0x0A)
      {
        for(u8 u8a=0;u8a<(u8CharNumber-1);u8a++)
        {
          au8Char[u8a] = au8Char[u8a+1];
        }
      }
      for(u8 u8a=0;u8a<128;u8a++)
      {
        for(u8 u8b=0;u8b<16;u8b++)
        {
          au8CharSave[u8a][u8b]= 0X00;
        }
      } 
    }
  
    if(bNewMessage)
    {
      bNewMessage = FALSE;
      memcpy(au8uBleMessage, (const u8*)&spi_slave_au8Message[2], spi_slave_au8Message[1]);
      if(au8uBleMessage[0]==0x80)
      {
        memcpy(&au8Char[u8NumberByte], (const u8*)&au8uBleMessage[2], au8uBleMessage[1]);
        u8NumberByte = u8NumberByte+au8uBleMessage[1];
      }
      if(au8uBleMessage[0]==0x88)
      {
        memcpy(&au8Char[u8NumberByte], (const u8*)&au8uBleMessage[2], au8uBleMessage[1]);
        u8NumberByte = u8NumberByte+au8uBleMessage[1];
        
        for(u8 a=0;a<u8NumberByte;a++)
        {
          au8Char[a]=au8Char[a]+0xA0;
        }
        
        for(u8 u8a=0;u8a<128;u8a++)
      {
        for(u8 u8b=0;u8b<16;u8b++)
        {
          au8CharSave[u8a][u8b]= 0X00;
        }
      }
      bFlag = TRUE;
      u8CharNumber = u8NumberByte+2;
      u8NumberByte = 0;
      }
    }
  
  if(bFlag)
  {
    bFlag = FALSE;
    ChangeWord(au8Char,u8CharNumber);
    au8CharSave[112][0] = u8CharNumber;
    At24c02_Serial_Write(0x00,&au8CharSave[0][0],256,0XA0);
    At24c02_Serial_Write(0x00,&au8CharSave[16][0],256,0XA2);
    At24c02_Serial_Write(0x00,&au8CharSave[32][0],256,0XA4);
    At24c02_Serial_Write(0x00,&au8CharSave[48][0],256,0XA6);
    At24c02_Serial_Write(0x00,&au8CharSave[64][0],256,0XA8);
    At24c02_Serial_Write(0x00,&au8CharSave[80][0],256,0XAA);
    At24c02_Serial_Write(0x00,&au8CharSave[96][0],256,0XAC);
    At24c02_Serial_Write(0x00,&au8CharSave[112][0],256,0XAE);
    //DebugPrintf("\n\rDisplay Succeed!\n\r");
    CharData();
  }
  
  
  /*static u8 au81[][3]={{0xFA,0XFE,0XFD},{0xF3,0XF4,0XF5},{0xF6,0XF7,0XF8},
};
  static u8 au82[3][3];
  
  At24c02_Serial_Write(0x00,*au81,9);
  At24c02_Serial_Read(0x00,*au82,9);*/
} /* end UserApp1SM_Idle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



void Delay(u16 u16i)
{
  static u16 u16a;
  for(u16a=0;u16a<u16i;u16a++);
}


static void Clear(void)
{
  static u8 a=0;
  for(a=0;a<10;a++)
  {
    SendData(0X00);
  }
}

static void ChangeWord(u8 *au8,u8 u8Count)
{
  //static u16 u16Word[]={'¾´','½£','ÈÝ','Ò×','Ì«','ÄÑ','¾´','Ð¡','Óã','ÌÇ'};
  static u8 *au8WordData;
  static u8 a,b;
  
  u8Count = (u8Count-1)/2;
  for(a=0;a<u8Count;a++)
  {
    au8WordData = GetZiKuWord(au8[a*2],au8[a*2+1]);
    
    for(b=0;b<16;b++)
    {
      au8CharSave[a*2][b] = *(au8WordData+2*b);
      au8CharSave[a*2+1][b] = *(au8WordData+2*b+1);
    }
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
