#include <stdio.h>

//=============GLOBAL VARIABLES=================
	unsigned char C = 0;
	unsigned char Z = 0;
	unsigned char OF = 0;
	unsigned char SF = 0;
//===========GLOBAL VARIABLES END==============

void ALU(unsigned char OP1, unsigned char OP2, unsigned char OP_Code);
	unsigned char ADD(unsigned char OP1, unsigned char OP2);
	unsigned char SUB(unsigned char OP1, unsigned char OP2);
	unsigned char MUL(unsigned char OP1, unsigned char OP2);
		unsigned char putLSBToMSB(unsigned char OP1, unsigned char OP2);
		unsigned char putMSBToMSB(unsigned char OP1, unsigned char OP2);
		unsigned short mergeCharsToShort(unsigned char OP1, unsigned char OP2);
		void printBooth(unsigned char A, unsigned char Q, unsigned char M, unsigned char Q_1);
	void conv_DecToBinStr(unsigned char num, char* result_string);
	unsigned char conv_BinStrToDec(char* binary);

	unsigned char twosComp(unsigned data);
	unsigned char setFlags(unsigned int ACC);
	void printBin(int data, unsigned char data_width);
	unsigned char getMSB(unsigned char num);

	void resetFlags();
	void printFlags();


unsigned short int ACC = 0;
//==================ALU======================//
void ALU(unsigned char OP1, unsigned char OP2, unsigned char OP_Code){
	static unsigned short int ACC = 0;
	printf("===============================\n");
	printf("Fetching Operands...\n");
	printf("Operand 1: ");			printBin(OP1, 8);			printf("\n");
	printf("Operand 2: ");			printBin(OP2, 8);			printf("\n");
	switch(OP_Code){
		case 1:
			printf("Operation: ADD\n");
			ACC = ADD(OP1, OP2);
			printf("ACC: 	   ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 2:
			printf("Operation: SUB\n");
			unsigned char twos_OP2 = 0x00;
			twos_OP2 = twosComp(OP2);
			printf("Two's Complement of Operand 2: ");
			printBin(twos_OP2,8);
			ACC = SUB(OP1, twos_OP2);
			printf("\nACC: 	   ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 3:
			printf("Operation: MUL\n");
			MUL(OP1, OP2);
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 4:
			printf("Operation: AND\n");
			ACC = OP1 & OP2;
			printf("ACC: 	   ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 5:
			printf("Operation: OR\n");
			ACC = OP1 | OP2;
			printf("ACC: 	   ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 6:
			printf("Operation: NOT\n");
			ACC = ~OP1;
			printf("ACC: 	   ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 7:
			printf("Operation: XOR\n");
			ACC = OP1 ^ OP2;
			printf("ACC: 	   ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 8:
			printf("Operation: SHR\n");
			ACC = OP1 >> 1;
			if(OP1%2) ACC+=0x100;
			printf("ACC: 	   ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 9:
			printf("Operation: SHL\n");
			ACC = OP1 << 1;
			printf("ACC: 	   ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		default:
			printf("Opcode not recognized\n");
	}
	return;
}

//==================ALU FUNCTIONS======================//
unsigned char ADD (unsigned char OP1, unsigned char OP2){
	ACC = 0;				//RESET THE ACC REGISTER
	if(OP2<0){
    	twosComp(OP2); 	
	}
	
	int carry=0;
	int tempACC = 0;
	int bitPos = 1;
	
	while (OP1 != 0 || OP2 != 0){
	    int A = OP1&1;
	    int B = OP2&1;
	    int sum = A^B^carry;
	    
	    carry = (A&B)|(B&carry)|(A&carry);
	    
	    tempACC |= sum * bitPos;
	    
	    OP1>>=1;
	    OP2>>=1;
	    
	    bitPos<<=1;
    }
    
	tempACC |= carry * bitPos;
	if(tempACC%0x0100)	ACC += 0x100;
    return (unsigned char)tempACC;
}

unsigned char SUB(unsigned char OP1, unsigned char OP2){
    return ADD(OP1, OP2);
}

void conv_DecToBinStr(unsigned char num, char* outputBin){
	int i=0;
	for(i=1; i<=8;i++){
		if(num%2==1)	outputBin[8-i] = '1';
		else			outputBin[8-i] = '0';
		num/=2;
	}
	outputBin[i-1] = 0; //Add String Terminator for Printing
}

unsigned char conv_BinStrToDec(char* binary){
	unsigned char dec;
	int i;
	for(i=1; i<=8;i++){
		if(binary[i]=='1') dec+= 2^(8-i);
	}
	return dec;
}

unsigned char twosComp(unsigned data){
	return ADD((~data),0x01);
}
unsigned char setFlags(unsigned int ACC){
	if(ACC==0)				Z = 1; 		else Z = 0;
	if((ACC&0x80)==0x80)		SF = 1; 	else SF = 0;
	if(ACC>0xff)			C = 1; 		else C = 0;
	if(ACC>0x7f&&ACC<0xff)	OF = 1; 		else OF = 0;
	return 0;
}
void resetFlags(){
	C = 0;
	Z = 0;
	OF = 0;
	SF = 0;
}
void printFlags(){
	printf("Flags: C=%1d  Z=%1d  OF=%1d  SF=%1d \n", C, Z, OF, SF);
}
void printBin(int data, unsigned char data_width){
	int i=0;
	char outputBinary[data_width+1];
	for(i=1; i<=data_width;i++){
		if(data%2==1)	outputBinary[data_width-i] = '1';
		else			outputBinary[data_width-i] = '0';
		data/=2;
	}
	outputBinary[i-1] = 0; //Add String Terminator for Printing
	printf("%s", outputBinary);
}

unsigned char MUL(unsigned char OP1, unsigned char OP2) {

    unsigned char accTemp;
    unsigned char M;
    unsigned char Q;
    unsigned char Qprev;
    unsigned char Aprev;
    
    unsigned char Q_1 = 0b00000000;
                      
    //number of bits in multiplier
    int n = 8;

    //Initialize variables
    accTemp = 0b00000000;
    M = OP1;
    Q = OP2;

    int bit;

    printf("   A            Q            Q_1           M\n");
    printBin(accTemp, 8);
    printf("     ");
    printBin(Q, 8);
    printf("     ");
    printBin(Q_1, 8);
    printf("     ");
    printBin(M, 8);
    printf("     ");
    printf("\n");

    
    for(int i=0; i < n; i++) {    
        
        //check if Q and Q_1 is 00 or 11
        if((((Q & 1) == 0) && ((Q_1 & 1) == 0)) || (((Q & 1) == 1) && ((Q_1 & 1) == 1))) {
            Aprev = accTemp;
            //Shift right    
            Qprev = Q & 1;
            Q_1 = Qprev;
            Q = Q >> 1;
            Q = putLSBToMSB(accTemp, Q);

            accTemp = accTemp >> 1;

            accTemp = putMSBToMSB(Aprev, accTemp);

            printBooth(accTemp, Q, M, Q_1);

            //check if 01
        }else if(((Q & 1) == 0) && ((Q_1 & 1) == 1)) {
            accTemp = ADD(accTemp, M);
            Aprev = accTemp;
            //printf("hi");
            //Shift right 
            Qprev = Q & 1;
            Q_1 = Qprev;
            Q = Q >> 1;
            Q = putLSBToMSB(accTemp, Q);

            accTemp = accTemp >> 1; 

            accTemp = putMSBToMSB(Aprev, accTemp);

            printBooth(accTemp, Q, M, Q_1);
          
          //check if 10
        }else if(((Q & 1) == 1) && ((Q_1 & 1) == 0)) { 
            //printf("hi");
            accTemp = SUB(accTemp, M);
            Aprev = accTemp;

            //Shift right 
            Qprev = Q & 1;
            Q_1 = Qprev;
            Q = Q >> 1;
            Q = putLSBToMSB(accTemp, Q);

            accTemp = accTemp >> 1;

            accTemp = putMSBToMSB(Aprev, accTemp);

            printBooth(accTemp, Q, M, Q_1);   
        }
    }

    printf("A: ");
    printBin(accTemp, 8);
    printf("\nQ: ");
    printBin(Q, 8);

    //Combine accTemp and Q to put it on the ACC register
    ACC = mergeCharsToShort(accTemp, Q);


    //Print ACC
    printf("\nACC: ");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (ACC >> i) & 1);
    }
    printf("\n");
}

unsigned short mergeCharsToShort(unsigned char OP1, unsigned char OP2) {
    unsigned short result = 0;  
    result |= ((unsigned short)OP1 << 8);
    result |= OP2;
    return result;
}

unsigned char putMSBToMSB(unsigned char OP1, unsigned char OP2) {
    unsigned char msb_OP1 = (OP1 >> 7) & 1;
    
    OP2 &= 0x7F; 
    OP2 |= (msb_OP1 << 7); 
    return OP2;
}

void printBooth(unsigned char A, unsigned char Q, unsigned char M, unsigned char Q_1) {
    printBin(A, 8);
    printf("     ");
    printBin(Q, 8);
    printf("     ");
    printBin(Q_1, 8);
    printf("     ");
    printBin(M, 8);
    printf("     ");
    printf("\n");

}

unsigned char putLSBToMSB(unsigned char OP1, unsigned char OP2) {
    // Extract the LSB of OP1
    unsigned char lsb_OP1 = OP1 & 1;

    // Shift the LSB to the leftmost position
    lsb_OP1 <<= (sizeof(unsigned char) * 8 - 1);

    // OR the shifted LSB with OP2
    OP2 |= lsb_OP1;

    return OP2;
}

unsigned char getMSB(unsigned char num) {
    int n = 7;
    printBin(num, 8);
    for(int i=0; i < n; i++) {
        num = num >> 1;
        printBin(num, 8);
    }

    return num;
}

void main(){
	ALU(0x9A,0x02,0x01);
	ALU(0x11,0x22,0x02);
	ALU(0x9A,0x02,0x03);
	ALU(0x11,0x22,0x04);
	ALU(0x11,0x22,0x05);
	ALU(0x11,0x22,0x06);
	ALU(0x11,0x22,0x07);
	ALU(0x11,0x22,0x08);
	ALU(0xff,0x22,0x09);
}

