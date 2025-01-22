#include <stdio.h>


// -------------------- START OF ALU IMPLEMENTATION -------------------- //
unsigned char FLAGS = 0;

int ALU(unsigned char CONTROL);
	// unsigned char ADD(unsigned char OP1, unsigned char OP2);
	// unsigned char SUB(unsigned char OP1, unsigned char OP2);
	// unsigned char MUL(unsigned char OP1, unsigned char OP2);
	// 	unsigned char putLSBToMSB(unsigned char OP1, unsigned char OP2);
	// 	unsigned char putMSBToMSB(unsigned char OP1, unsigned char OP2);
	// 	unsigned short mergeCharsToShort(unsigned char OP1, unsigned char OP2);
	// 	void printBooth(unsigned char A, unsigned char Q, unsigned char M, unsigned char Q_1);
	// void conv_DecToBinStr(unsigned char num, char* result_string);
	// unsigned char conv_BinStrToDec(char* binary);

	
	// void printBin(int data, unsigned char data_width);
	// unsigned char getMSB(unsigned char num);

	unsigned char twosComp(unsigned data);
	void setFlags(unsigned short int ACC);
	void resetFlags();
	void printFlags();

// -------------------- START OF ALU IMPLEMENTATION -------------------- //
unsigned char FLAGS = 0;
unsigned short int ACC = 0;

int ALU(unsigned char CONTROL){
	static unsigned short int ACC = 0;
	printf("===============================\n");
	
	int temp_OP2=BUS, temp_ACC=0, ret=0, skipFlagPrint=0;
	resetFlags();

	if(CONTROL==0b11110 || CONTROL==0b11101){	//ADD or SUB
		if(CONTROL==0b11101)	temp_OP2 = twosComp(temp_OP2);
		temp_ACC = (int) ACC + temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
	}else if(CONTROL==0b11011){		//MUL
		temp_ACC = (int) ACC * temp_OP2;
		ACC = (unsigned short int) temp_ACC;
		setFlags(ACC);
	}else if(CONTROL==0b11010){		//AND
		temp_ACC = ACC & temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
	}else if(CONTROL==0b11001){		//OR
		temp_ACC = ACC | temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
	}else if(CONTROL==0b11000){		//NOT
		temp_ACC = ~ACC;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
	}else if(CONTROL==0b10111){		//XOR
		temp_ACC = ACC ^ temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
	}else if(CONTROL==0b10110){		//SHL
		temp_ACC = ACC << 1;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
	}else if(CONTROL==0b10101){		//SHR
		temp_ACC = ACC >> 1;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
	}else if(CONTROL==0b01001){		//WACC
		ACC = BUS;
		skipFlagPrint=1;
	}else if(CONTROL==0b01011){		//RACC
		BUS = ACC;
		skipFlagPrint=1;
	}else if(CONTROL==0b10100 | CONTROL==0b10011 | CONTROL==0b10010 | CONTROL==0b10001){		//BRE	//BRNE	//BRGT	//BRLT
		temp_ACC = ACC - temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
		//if (CONTROL==0b10100 & ZF==1), PC=addr ------- This part is done on the CU
		//if (CONTROL==0b10011 & ZF==0), PC=addr ------- This part is done on the CU
		//if (CONTROL==0b10010 & SF==0), PC=addr ------- This part is done on the CU
		//if (CONTROL==0b10001 & SF==1), PC=addr ------- This part is done on the CU
	}else	ret=1;
	if(skipFlagPrint==0) printFlags();
	return ret;
}

//ALU FUNCTIONS

unsigned char twosComp(unsigned data){
	return ~data+1;
}
void setFlags(unsigned short int ACC){
	if(ACC==0)				FLAGS = FLAGS | 0b00000001; 	else FLAGS = FLAGS & 0b11111110;	// ZF
	if(ACC>0xff)			FLAGS = FLAGS | 0b00000010; 	else FLAGS = FLAGS & 0b11111101;	// CF
	if((ACC&0x80)==0x80)	FLAGS = FLAGS | 0b00000100; 	else FLAGS = FLAGS & 0b11111011;	// SF
	if(ACC>0x7f&&ACC<0xff)	FLAGS = FLAGS | 0b10000000; 	else FLAGS = FLAGS & 0b01111111;	// OF
	return 0;
}
void resetFlags(){
	FLAGS = 0;
}
void printFlags(){
	unsigned char ZF = (FLAGS & 0b00000001);
	unsigned char CF = (FLAGS & 0b00000010) >> 1;
	unsigned char SF = (FLAGS & 0b00000100) >> 2;
	unsigned char OF = (FLAGS & 0b10000000) >> 7;
	printf("Flags: ZF=%1d	CF=%1d	SF=%1d	 OF=%1d\n", ZF, CF, SF, OF);
}
// -------------------- END OF ALU IMPLEMENTATION -------------------- //