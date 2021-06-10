#include "main.h"
#include "ict_sys.h"

/*/////////////////////////////////////////////// System setting */////////////////////////////////////////////*/
// Serial commuincation set variable 
string device_s = "/dev/ttyUSB0";
char device[] = "/dev/ttyUSB0"; //Serial communication connection port
unsigned long baud = 115200;
int fd;
int cnt;
///////////////////// File set variable///////////////////////////// 
FILE* fp_W;
FILE* fp_L;
FILE* fp_T_H;
FILE* fp_D;

char day[100];
///////////////////// Time set variable///////////////////////////// 

time_t timer;
struct timeb tb;
struct tm* t;
int Current_date;
int old_Time[8];
int now_Time[8];
//////////////////// DATA SET ////////////////////

char buf_w[BUF_SIZE];
char buf_l[BUF_SIZE];
char buf_d[BUF_SIZE];
char buf_TnH[BUF_SIZE];
char buf_Time[BUF_SIZE];
char buf_Encoder[BUF_SIZE];
char buf_EncoderTime[BUF_SIZE];
char buf_Encoder_start[BUF_SIZE];
char buf_Dispenser_start[BUF_SIZE];
char buf[15];
int rdcnt;
//////////////////////////ENCODER Set value//////////////////////
float ENCODER_1_CNT, ENCODER_2_CNT;
TIME wash_time;
/**********************Function ********************/
void loop()
{
	
	short wash_encoder_1_cnt = 0 , wash_encoder_2_cnt = 0;
	
	timer = time(NULL);
	ftime(&tb);
	t = localtime(&tb.time);
	if(Current_date =! t->tm_mday )
	{
		Wash_save_file_creat();
		Dispenser_save_file_creat();
	}

	if (serialDataAvail(fd))
	{
		digitalWrite(LED, HIGH);
		rdcnt = read(fd, buf, sizeof(buf));
	    if(buf[0]== 2 && buf[rdcnt-1] == 3)
		{
			wash_encoder_1_cnt = buf[3];
		    wash_encoder_1_cnt += (buf[2]<<8);

			wash_encoder_2_cnt = buf[5];
			wash_encoder_2_cnt += (buf[4]<<8);

			wash_time.ms = buf[7];
			wash_time.ms += (buf[6]<<8);

			wash_time.sec = buf[9];
			wash_time.sec += (buf[8]<<8);

			wash_time.min = buf[11];
			wash_time.min += (buf[10]<<8);

			wash_time.hour = buf[13];
			wash_time.hour += (buf[12]<<8);

			ENCODER_1_CNT = (wash_encoder_1_cnt * 0.2);
			ENCODER_2_CNT = (wash_encoder_2_cnt * 0.2);

			sprintf(buf_Time,"%02d%02d%02d.%d", t->tm_hour, t->tm_min, t->tm_sec, tb.millitm);
			sprintf(buf_Encoder,"%.1f,%.1f\r\n",ENCODER_1_CNT, ENCODER_2_CNT);
			sprintf(buf_w,"%s,%s", buf_Time, buf_Encoder);
			fprintf(fp_W,"%s",buf_w);
			serialFlush(fd);
			fflush(stdout);
		}
	}
	digitalWrite(LED, LOW);
}

void setup()
{
	//Communication connection check///////////////////////////////
	if ((fd = serialOpen(device, baud)) < 0)
	{
		fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
		exit(1);//error
	}
	//GPIO check//////////////////////////////////////////////
	if (wiringPiSetup() < 0)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		exit(1); //error
	}


	wiringPiSetupSys();
	printf("%s \n", "ICT_SYSTEM Startup!");

	pinMode(LED, OUTPUT); //Output mode set
	pinMode(PACKER_DISPENSER_1_SIG, INPUT);
	pinMode(PACKER_DISPENSER_2_SIG, INPUT);
	pinMode(PACKER_DISPENSER_3_SIG, INPUT);
	pinMode(PACKER_DISPENSER_4_SIG, INPUT);
	pinMode(PACKER_DISPENSER_5_SIG, INPUT);
	pinMode(PACKER_DISPENSER_6_SIG, INPUT);

	fflush(stdout);   //Force the output buffer of standard output to be flushed
	fflush(stdin);   //Force the input buffer of standard output to be flushed

	Wash_save_file_creat();
	Dispenser_save_file_creat();

	wiringPiISR(PACKER_DISPENSER_1_SIG, INT_EDGE_RISING, PACKER_DISPENSER1_ISR);
	wiringPiISR(PACKER_DISPENSER_2_SIG, INT_EDGE_RISING, PACKER_DISPENSER2_ISR);
	wiringPiISR(PACKER_DISPENSER_3_SIG, INT_EDGE_RISING, PACKER_DISPENSER3_ISR);
	wiringPiISR(PACKER_DISPENSER_4_SIG, INT_EDGE_RISING, PACKER_DISPENSER4_ISR);
	wiringPiISR(PACKER_DISPENSER_5_SIG, INT_EDGE_RISING, PACKER_DISPENSER5_ISR);
	wiringPiISR(PACKER_DISPENSER_6_SIG, INT_EDGE_RISING, PACKER_DISPENSER6_ISR);
}

int dir_creat()
{
	DIR *dir_info;
	struct dirent *dir_entry;
	
	if(mkdir("Wash_Data", 0755) == -1 && errno != EEXIST)
	{
		  printf("mkdir error\n");
		  return -1;
	}
	if(mkdir("Loadcell_Data", 0755) == -1 && errno != EEXIST)
	{
		  printf("mkdir error\n");
		  return -1;
	}
	if(mkdir("Dispenser_Data", 0755) == -1 && errno != EEXIST)
	{
		  printf("mkdir error\n");
		  return -1;
	}
	if(mkdir("Temperature_Humidity_Data", 0755) == -1 && errno != EEXIST)
	{
		  printf("mkdir error\n");
		  return -1;
	}
	return 0;
}

int Wash_save_file_creat()
{  
 	timer = time(NULL);
  	t = localtime(&timer);
  	sprintf(day,"%s%s%04d%02d%02d%s","Wash_Data/","Wash_data_" ,t->tm_year+1900,t->tm_mon+1, t->tm_mday,".csv" );//, tm.tm_hour, tm.tm_min, tm.tm_sec, ".csv");
  	Current_date =  t->tm_mday;	
  	if(!(fp_W = fopen(day,"a")))
  	{
		return -1;
 	}
  	else
  	{
	//sprintf(buf_Encoder_start,"%s\r\n", "OS_time,Encoder_A_value,Encoder_B_value");
	//fprintf(fp_W,"%s",buf_Encoder_start);
  		return 0;
  	}
	return 0;
}

int Dispenser_save_file_creat()
{  
  timer = time(NULL);
  t = localtime(&timer);
  sprintf(day,"%s%s%04d%02d%02d%s","Dispenser_Data/","Dispenser_Data_" ,t->tm_year+1900,t->tm_mon+1, t->tm_mday,".csv" );//, tm.tm_hour, tm.tm_min, tm.tm_sec, ".csv");
  Current_date =  t->tm_mday;
  if(!(fp_D = fopen(day,"a")))
  {
	return -1;
  }
  else
  {
	//sprintf(buf_Dispenser_start,"%s\r\n", "Occurrence time,Error");
	//fprintf(fp_D,"%s",buf_Dispenser_start);
	fclose(fp_D);
  	return 1;
  }
}

void PACKER_DISPENSER1_ISR()
{
	//Save time measurement and Number of failures
	timer = time(NULL);
	ftime(&tb);
	t = localtime(&tb.time);
	now_Time[0] = t->tm_sec;
	if(old_Time[0] != now_Time[0])
	{
		fp_D = fopen(day,"a");
		sprintf(buf_d, "%04d%02d%02d-%02d%02d%02d,%s\r\n",t->tm_year+1900,t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,"Dispenser1 error");
		fprintf(fp_D, "%s", buf_d);
		fflush(stdout);
		old_Time[0] = now_Time[0];
		fclose(fp_D);
	}
}
void PACKER_DISPENSER2_ISR()
{
	//Save time measurement and Number of failures
	timer = time(NULL);
	ftime(&tb);
	t = localtime(&tb.time);
	now_Time[1] = t->tm_sec;
	if(old_Time[1] != now_Time[1] )
	{
		fp_D = fopen(day,"a");
		sprintf(buf_d, "%04d%02d%02d-%02d%02d%02d,%s\r\n",t->tm_year+1900,t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,"Dispenser2 error");
		fprintf(fp_D, "%s", buf_d);
		fflush(stdout);
		old_Time[1] = now_Time[1];
		fclose(fp_D);
	}

}
void PACKER_DISPENSER3_ISR()
{
	//Save time measurement and Number of failures
	timer = time(NULL);
	ftime(&tb);
	t = localtime(&tb.time);
	now_Time[2] = t->tm_sec;
	if(old_Time[2] != now_Time[2] )
	{
		fp_D = fopen(day,"a");
		sprintf(buf_d, "%04d%02d%02d-%02d%02d%02d,%s\r\n",t->tm_year+1900,t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,"Dispenser3 error");
		fprintf(fp_D, "%s", buf_d);
		fflush(stdout);
		old_Time[2] = now_Time[2];
		fclose(fp_D);
	}

}
void PACKER_DISPENSER4_ISR()
{
	//Save time measurement and Number of failures
	timer = time(NULL);
	ftime(&tb);
	t = localtime(&tb.time);
	now_Time[3] = t->tm_sec;
	if(old_Time[3] != now_Time[3] )
	{
		fp_D = fopen(day,"a");
		sprintf(buf_d, "%04d%02d%02d-%02d%02d%02d,%s\r\n",t->tm_year+1900,t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,"Dispenser4 error");
		fprintf(fp_D, "%s", buf_d);
		fflush(stdout);
		old_Time[3] = now_Time[3];
		fclose(fp_D);
	}

}
void PACKER_DISPENSER5_ISR()
{
	//Save time measurement and Number of failures
	timer = time(NULL);
	ftime(&tb);
	t = localtime(&tb.time);
	now_Time[4] = t->tm_sec;
	if(old_Time[4] != now_Time[4] )
	{
		fp_D = fopen(day,"a");
		sprintf(buf_d, "%04d%02d%02d-%02d%02d%02d,%s\r\n",t->tm_year+1900,t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,"Dispenser5 error");
		fprintf(fp_D, "%s", buf_d);
		fflush(stdout);
		old_Time[4] = now_Time[4];
		fclose(fp_D);
	}

}
void PACKER_DISPENSER6_ISR()
{
	//Save time measurement and Number of failures
	timer = time(NULL);
	ftime(&tb);
	t = localtime(&tb.time);
	now_Time[5] = t->tm_sec;
	if(old_Time[5] != now_Time[5] )
	{
		fp_D = fopen(day,"a");
		sprintf(buf_d, "%04d%02d%02d-%02d%02d%02d,%s\r\n",t->tm_year+1900,t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,"Dispenser6 error");
		fprintf(fp_D, "%s", buf_d);
		old_Time[5] = now_Time[5];
		fclose(fp_D);
	}

}

void sig_Handler(int sig) 
{
	fclose(fp_D);
	fclose(fp_W);
	printf("\n\nEnding....\n\n");
	init(); // call init() func.
	exit(0); //
}

void init(void) 
{
	digitalWrite(LED, LOW); // LED OFF
}


