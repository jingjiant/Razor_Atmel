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
static u8 au8DataMessage[16][10];

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
  //AT91C_BASE_PIOA->PIO_CODR   = OE; 
  //AT91C_BASE_PIOA->PIO_SODR   = LE;
  CharData();
  
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
  //static u8 au8YiChuData[16][2];
  //static u8 u8a,u8b;
  
  for(u8hangshu=0;u8hangshu<16;u8hangshu++)
  {
    Clear();
    display(u8hangshu,au8DataMessage);
    Delay(100);
  }
  
  u16Count++;
  
  if(u16Count==250)
  {
    CharData();
    u16Count = 0;
  }

  
  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */


static void SendData(u8 u8Data)
{
  static u8 u8DataSize = 0;
  
  AT91C_BASE_PIOA->PIO_SODR   = OE; 
  AT91C_BASE_PIOA->PIO_SODR   = LE;
  AT91C_BASE_PIOA->PIO_CODR   = M_CLK;
  //Delay(10);
  
  for(u8DataSize=0;u8DataSize<8;u8DataSize++)
  {
    if((u8Data&0X80)==0X80)
    {
      AT91C_BASE_PIOA->PIO_SODR   = M_SDI;
    }
    
    else
    {
      AT91C_BASE_PIOA->PIO_CODR   = M_SDI;
    }
    
    AT91C_BASE_PIOA->PIO_CODR   = M_CLK;
    //Delay(10);
    AT91C_BASE_PIOA->PIO_SODR   = M_CLK;
    //Delay(10);
    
    u8Data = u8Data<<1;
  } 


}

static void Delay(u16 u16i)
{
  static u16 u16a;
  for(u16a=0;u16a<u16i;u16a++);
}



static void ABCD(u8 d,u8 c,u8 b,u8 a)
{
  if(a==1)
  {
  AT91C_BASE_PIOA->PIO_SODR   = A;
  }
  
  else
  {
    AT91C_BASE_PIOA->PIO_CODR   = A;
  }
  
    if(b==1)
  {
    AT91C_BASE_PIOA->PIO_SODR   = B;
  }
  
  else
  {
    AT91C_BASE_PIOA->PIO_CODR   = B;
  }
  
    if(c==1)
  {
    AT91C_BASE_PIOA->PIO_SODR   = C;
  }
  
  else
  {
    AT91C_BASE_PIOA->PIO_CODR   = C;
  }
  
    if(d==1)
  {
    AT91C_BASE_PIOA->PIO_SODR   = D;
  }
  
  else
  {
    AT91C_BASE_PIOA->PIO_CODR   = D;
  }
}

static void display(u8 u8Paral,u8 au8Message[16][10])
{
  switch(u8Paral)
  {
  case 0:
    ABCD(0,0,0,0);
    break;
  case 1:
    ABCD(0,0,0,1);
    break;
  case 2:
    ABCD(0,0,1,0);
    break;
  case 3:
    ABCD(0,0,1,1);
    break;
  case 4:
    ABCD(0,1,0,0);
    break;
  case 5:
    ABCD(0,1,0,1);
    break;
  case 6:
    ABCD(0,1,1,0);
    break;
  case 7:
    ABCD(0,1,1,1);
    break;
  case 8:
    ABCD(1,0,0,0);
    break;
  case 9:
    ABCD(1,0,0,1);
    break;
  case 10: 
    ABCD(1,0,1,0);
    break;
  case 11: 
    ABCD(1,0,1,1);
    break;
  case 12: 
    ABCD(1,1,0,0);
    break;
  case 13:
    ABCD(1,1,0,1);
    break;
  case 14:
    ABCD(1,1,1,0);
    break;
  case 15:
    ABCD(1,1,1,1);
    break; 
  }
  
  for(u8 u8i=0;u8i<10;u8i++)
  {
    SendData(au8Message[u8Paral][9-u8i]);
  }
  AT91C_BASE_PIOA->PIO_CODR   = LE;
  AT91C_BASE_PIOA->PIO_CODR   = OE; 
}

static void Clear(void)
{
  static u8 a=0;
  for(a=0;a<10;a++)
  {
    SendData(0X00);
  }
}

static void CharData(void)
{
  static u8 au8CharSave[][32]={
    {0x44,0x04,0x44,0x04,0xFF,0x05,0x44,0x7C,0x02,0x22,0xFE,0x22,0x82,0x22,0x81,0x25,0xBE,0x14,0xA2,0x14,0xA2,0x08,0xBE,0x08,0xA2,0x14,0x80,0x14,0x50,0x22,0x20,0x41},/*"¾´",0*/
    {0x10,0x20,0x10,0x20,0x28,0x20,0x48,0x24,0x84,0x24,0x02,0x25,0x7D,0x24,0x00,0x24,0x88,0x24,0x90,0x24,0x92,0x24,0x44,0x24,0x44,0x20,0xE0,0x21,0x1E,0x28,0x04,0x10},/*"½£",1*/
    {0x80,0x00,0x00,0x01,0xFC,0x7F,0x04,0x01,0xF4,0x1F,0x04,0x11,0xFC,0x7F,0x04,0x11,0xF4,0x1F,0x04,0x01,0xF4,0x1F,0x14,0x10,0x12,0x10,0x12,0x10,0xF1,0x1F,0x10,0x10},/*"ÌÆ",0*/
    {0x84,0x00,0x88,0x00,0x88,0x7F,0x40,0x00,0x3F,0x04,0x84,0x3F,0x04,0x0A,0x3C,0x11,0xE4,0x7F,0x24,0x10,0xA4,0x17,0xA4,0x14,0xA4,0x17,0x22,0x10,0x2A,0x14,0x11,0x08},/*"ì½",1*/
    {0x44,0x04,0x44,0x04,0xFF,0x05,0x44,0x7C,0x02,0x22,0xFE,0x22,0x82,0x22,0x81,0x25,0xBE,0x14,0xA2,0x14,0xA2,0x08,0xBE,0x08,0xA2,0x14,0x80,0x14,0x50,0x22,0x20,0x41},/*"¾´",0*/
    {0x08,0x02,0x08,0x02,0xFC,0x7E,0x12,0x09,0xA1,0x10,0x08,0x02,0x08,0x02,0x24,0x7E,0x24,0x11,0x26,0x11,0xA5,0x12,0x24,0x0A,0x24,0x04,0x04,0x0A,0x04,0x11,0xC4,0x60},/*"óã",1*/
    {0x08,0x00,0x88,0x1F,0x08,0x10,0x08,0x0A,0x3F,0x04,0x24,0x08,0xE4,0x7F,0x24,0x44,0x24,0x24,0x12,0x04,0x14,0x04,0x08,0x04,0x14,0x04,0x22,0x04,0x21,0x05,0x00,0x02},/*"æ¥",2*/
    {0x44,0x04,0x44,0x04,0xFF,0x05,0x44,0x7C,0x02,0x22,0xFE,0x22,0x82,0x22,0x81,0x25,0xBE,0x14,0xA2,0x14,0xA2,0x08,0xBE,0x08,0xA2,0x14,0x80,0x14,0x50,0x22,0x20,0x41},/*"¾´",0*/
    {0x10,0x20,0x10,0x20,0x28,0x20,0x48,0x24,0x84,0x24,0x02,0x25,0x7D,0x24,0x00,0x24,0x88,0x24,0x90,0x24,0x92,0x24,0x44,0x24,0x44,0x20,0xE0,0x21,0x1E,0x28,0x04,0x10},/*"½£",1*/
    {0x80,0x00,0x00,0x01,0xFC,0x7F,0x04,0x01,0xF4,0x1F,0x04,0x11,0xFC,0x7F,0x04,0x11,0xF4,0x1F,0x04,0x01,0xF4,0x1F,0x14,0x10,0x12,0x10,0x12,0x10,0xF1,0x1F,0x10,0x10},/*"ÌÆ",0*/
    {0x84,0x00,0x88,0x00,0x88,0x7F,0x40,0x00,0x3F,0x04,0x84,0x3F,0x04,0x0A,0x3C,0x11,0xE4,0x7F,0x24,0x10,0xA4,0x17,0xA4,0x14,0xA4,0x17,0x22,0x10,0x2A,0x14,0x11,0x08},/*"ì½",1*/
    
    
    
    
    
    
  };

                              
  static u8 u8a,u8b,u8c,u8d=0;
  
  u8a=sizeof(au8CharSave)/32;
 
  
  for(u8b=u8d;u8b<5+u8d;u8b++)
  {
    for(u8c=0;u8c<16;u8c++)
    {
      au8DataMessage[u8c][(u8b-u8d)*2] = au8CharSave[u8b][u8c*2];
      au8DataMessage[u8c][(u8b-u8d)*2+1] = au8CharSave[u8b][u8c*2+1];
    }
  }
  
  u8d++;
  
  if((u8a-u8d)==4)
  {
    u8d = 0;
  }
                 
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
