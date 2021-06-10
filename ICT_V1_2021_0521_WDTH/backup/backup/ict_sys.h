#ifndef ICT_SYS_H_
#define ICT_SYS_H_
////////////////////////////// raspi define /////////////////////////////////////////////*/
#define	LED	0 //LED GPIO this is Wiring trigger
#define	PACKER_DISPENSER_1_SIG 2  //BCM => 27
#define	PACKER_DISPENSER_2_SIG 3  //BCM => 22
#define	PACKER_DISPENSER_3_SIG 23 //BCM => 13
#define	PACKER_DISPENSER_4_SIG 24  //BCM => 19
#define	PACKER_DISPENSER_5_SIG 25 //BCM => 25
#define	PACKER_DISPENSER_6_SIG 1 //BCM => 18
// Interrupt levels(mode)
#define INT_EDGE_SETUP 0
#define INT_EDGE_FALLING 1
#define INT_EDGE_RISING 2
#define INT_EDGE_BOTH 3

/*//////////////////////////////////Server SETTING/////////////////////////////////////////////////////////////////*/
#define  BUF_SIZE   1024 
#define _CRT_SECURE_NO_WARNINGS 

#pragma pack(push, 1)
typedef struct Time
{
	uint16_t hour;
	uint16_t min;
	uint16_t sec;
	uint16_t ms;
}TIME;
#pragma pack(pop)


void setup(void);	// Setup 
void loop(void);	// loop
int dir_creat();
int Wash_save_file_creat();
int Dispenser_save_file_creat();
//PACKER_DISPENSER is Interrupt  
void PACKER_DISPENSER1_ISR(void); // PACKER_DISPENSER1 is Interrupt  
void PACKER_DISPENSER2_ISR(void); // PACKER_DISPENSER2 is Interrupt   
void PACKER_DISPENSER3_ISR(void); // PACKER_DISPENSER3 is Interrupt  
void PACKER_DISPENSER4_ISR(void); // PACKER_DISPENSER4 is Interrupt  
void PACKER_DISPENSER5_ISR(void); // PACKER_DISPENSER5 is Interrupt  
void PACKER_DISPENSER6_ISR(void); // PACKER_DISPENSER6 is Interrupt  
void sig_Handler(int sig);
void init(void);

#endif /* ICT_SYS_H_ */
