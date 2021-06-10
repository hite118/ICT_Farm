#include <SimpleTimer.h>
#include <EEPROM.h>

#define ENCODER_1_A 2
#define ENCODER_1_B 22
#define ENCODER_2_A 3
#define ENCODER_2_B 23
#define FALSE 0
#define TRUE 1
#define ENCODER_1 1
#define ENCODER_2 2
//#define MAX_ROUND_CNT 825
#define WASHER_SIG 24


SimpleTimer timer;

short ENCODER_1_CNT = 0;
short ENCODER_2_CNT = 0;
short sec;
short ms;

uint8_t WASHER_flag = FALSE;
 

uint8_t Kind = 0;
uint8_t txbuff_SEND[11];
uint8_t STX = 2;
uint8_t ETX = 3;
char Select_data;

void setup()
{
  //PIN SET
  pinMode(ENCODER_1_A, INPUT_PULLUP); 
  pinMode(ENCODER_1_B, INPUT_PULLUP);
  pinMode(ENCODER_2_A, INPUT_PULLUP); 
  pinMode(ENCODER_2_B, INPUT_PULLUP);
  pinMode(WASHER_SIG, INPUT_PULLUP);
  
  //INTERUPT SET
  attachInterrupt(digitalPinToInterrupt(2), ENCODER_1_INT, CHANGE); // encoder pin on interrupt 0 (pin 2);
  attachInterrupt(digitalPinToInterrupt(3), ENCODER_2_INT, CHANGE); // encoder pin on interrupt 1 (pin 3);
  // SERIAL SET
  Serial.begin (115200); 
  timer.setInterval(100, send_data_run);
  //EEPROM Reading ENCODER_1,ENCODER_2 COOUNT 
  EEPROM.get(0,ENCODER_1_CNT);
  EEPROM.get(2,ENCODER_2_CNT);
}

void loop()
{
  //ON timer is activated in the washing unit
    if(digitalRead(WASHER_SIG)) 
    {
        timer.run();  
    }  
}

void ENCODER_1_INT()
{ 
  //Serial.println(encoder0Pos);
  if(!digitalRead(ENCODER_1_A))
  {
    if(!digitalRead(ENCODER_1_B))
    {
      ENCODER_1_CNT++;
    }
    else
    {
      ENCODER_1_CNT--;
    }
  }
}

void ENCODER_2_INT()
{
    if(!digitalRead(ENCODER_2_A))
    {
      if(!digitalRead(ENCODER_2_B))
      {
       ENCODER_2_CNT++;
      }
      else
      {
       ENCODER_2_CNT--;
      }   
    }
}

void send_data(short data,short data1,short data2)
{
  txbuff_SEND[0] = STX;
  txbuff_SEND[1] = sizeof(data)+sizeof(data1)+sizeof(data2);
  txbuff_SEND[2] = data >> 8;
  txbuff_SEND[3] = data;
  txbuff_SEND[4] = data1 >> 8;
  txbuff_SEND[5] = data1;
  txbuff_SEND[6] = data2 >> 8;
  txbuff_SEND[7] = data2;
  txbuff_SEND[8] = ETX;
  Serial.write(txbuff_SEND,9);
}

void send_data_run()
{  
    send_data(ENCODER_1_CNT,ENCODER_2_CNT,ms++);
    if(!digitalRead(WASHER_SIG))
    {
        EEPROM.put(0,ENCODER_1_CNT);
        EEPROM.put(2,ENCODER_2_CNT);
    } 
}
