#include <stdio.h>

//=============GLOBAL VARIABLES=================
	unsigned short int ACC = 0;
	unsigned char C = 0;
	unsigned char Z = 0;
	unsigned char OF = 0;
	unsigned char SF = 0;
//===========GLOBAL VARIABLES END==============

void ALU(unsigned char operand1, unsigned char operand2, unsigned char operation_code);
void conv_DecToBinStr(unsigned char num, char* result_string);
unsigned char conv_BinStrToDec(char* binary);

unsigned char twosComp(unsigned data);
unsigned char setFlags(unsigned int ACC);
void printBin(int data, unsigned char data_width);

unsigned char addBin (unsigned char num1, unsigned char num2);
unsigned char subBin(unsigned char operand1, unsigned char operand2);

unsigned char getMSB(unsigned char num);
unsigned char putLSBToMSB(unsigned char num1, unsigned char num2);
unsigned char putMSBToMSB(unsigned char num1, unsigned char num2);
unsigned short mergeCharsToShort(unsigned char num1, unsigned char num2);
void printBooth(unsigned char A, unsigned char Q, unsigned char M, unsigned char Q_1);


//==================CONVERSION FUNCTIONS======================//
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
	return addBin((~data),0x01);
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

unsigned char op_multiplication(unsigned char num1, unsigned char num2) {

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
    M = num1;
    Q = num2;

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
            accTemp = addBin(accTemp, M);
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
            accTemp = subBin(accTemp, M);
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

unsigned short mergeCharsToShort(unsigned char num1, unsigned char num2) {
    unsigned short result = 0;  
    result |= ((unsigned short)num1 << 8);
    result |= num2;
    return result;
}

unsigned char putMSBToMSB(unsigned char num1, unsigned char num2) {
    unsigned char msb_num1 = (num1 >> 7) & 1;
    
    num2 &= 0x7F; 
    num2 |= (msb_num1 << 7); 
    return num2;
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

unsigned char putLSBToMSB(unsigned char num1, unsigned char num2) {
    // Extract the LSB of num1
    unsigned char lsb_num1 = num1 & 1;

    // Shift the LSB to the leftmost position
    lsb_num1 <<= (sizeof(unsigned char) * 8 - 1);

    // OR the shifted LSB with num2
    num2 |= lsb_num1;

    return num2;
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

unsigned char addBin (unsigned char num1, unsigned char num2){
	ACC = 0;				//RESET THE ACC REGISTER
	if(num2<0){
    	twosComp(num2); 	
	}
	
	int carry=0;
	int tempACC = 0;
	int bitPos = 1;
	
	while (num1 != 0 || num2 != 0){
	    int A = num1&1;
	    int B = num2&1;
	    int sum = A^B^carry;
	    
	    carry = (A&B)|(B&carry)|(A&carry);
	    
	    tempACC |= sum * bitPos;
	    
	    num1>>=1;
	    num2>>=1;
	    
	    bitPos<<=1;
    }
    
	tempACC |= carry * bitPos;
	if(tempACC%0x0100)	ACC += 0x100;
    return (unsigned char)tempACC;
}

unsigned char subBin(unsigned char operand1, unsigned char operand2)
{
    return addBin(operand1, operand2);
}

void ALU(unsigned char operand1, unsigned char operand2, unsigned char operation_code){
	printf("===============================\n");
	printf("Fetching Operands...\n");
	printf("Operand 1: ");
	printBin(operand1, 8);
	printf("\n");
	printf("Operand 2: ");
	printBin(operand2, 8);
	printf("\n");
	switch(operation_code){
		case 1:
			printf("Operation: Addition\n");
			ACC = addBin(operand1, operand2);
			printf("ACC: ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 2:
			printf("Operation: Subtraction\n");
			unsigned char twos_OP2 = 0x00;
			twos_OP2 = twosComp(operand2);
			printf("Two's Complement of Operand 2: ");
			printBin(twos_OP2,8);
			ACC = subBin(operand1, twos_OP2);
			printf("\nACC: ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 3:
			printf("Operation: Multiplication\n");
			op_multiplication(operand1, operand2);
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 4:
			printf("Operation: Bitwise AND\n");
			ACC = operand1 & operand2;
			printf("ACC: ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 5:
			printf("Operation: Bitwise OR\n");
			ACC = operand1 | operand2;
			printf("ACC: ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 6:
			printf("Operation: Bitwise NOT\n");
			ACC = ~operand1;
			printf("ACC: ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 7:
			printf("Operation: Bitwise XOR\n");
			ACC = operand1 ^ operand2;
			printf("ACC: ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 8:
			printf("Operation: Shift Right\n");
			ACC = operand1 >> 1;
			if(operand1%2) ACC+=0x100;
			printf("ACC: ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		case 9:
			printf("Operation: Shift Left\n");
			ACC = operand1 << 1;
			printf("ACC: ");
			printBin(ACC, 8);
			printf("\n");
			setFlags(ACC);
			printFlags();
			resetFlags();
			break;
		default:
			printf("Opcode not recognized\n");
	}
	//printf("===============================\n");
	return;
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

