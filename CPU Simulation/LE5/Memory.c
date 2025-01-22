#include <stdbool.h>
#include <stdio.h>

void printBin(unsigned long int data, unsigned char data_width);


// -------------------- START OF BUS IMPLEMENTATION -------------------- //
    // BUS VARIABLES AND CONTROL SIGNALS
        unsigned short int ADDR;
            // ADDR - Address bus, contains the address to be accessed
        unsigned char BUS, CONTROL, IOM, RW, OE;
            // BUS      - Data bus, contains the data
            // CONTROL  - Control Signals: Contains the inst_code
            // IOM      - IO/M:         1-access Memory     0-access IO
            // RW       - Read/Write:   1-Write             0-Read
            // OE       - Enable:       1-Mem/IO access enabled
// -------------------- END OF BUS IMPLEMENTATION -------------------- //

// -------------------- START OF MAIN MEMORY IMPLEMENTATION -------------------- //
#define memWidth 32

unsigned long A1[memWidth], A2[memWidth],    // 0x00
     A3[memWidth], A4[memWidth],    //  |
     A5[memWidth], A6[memWidth],    //  |
     A7[memWidth], A8[memWidth];    // 0x3FF

unsigned long B1[memWidth], B2[memWidth],    // 0x400
     B3[memWidth], B4[memWidth],    //  |
     B5[memWidth], B6[memWidth],    //  |
     B7[memWidth], B8[memWidth];    // 0x7FF

void MainMemory(void){      // handles MainMemory <-> BUS exchanges
    unsigned short int address = ADDR;      // 11-bit
    unsigned char row = address/memWidth;   // 5-bit
    unsigned char col = address%memWidth;   // 5-bit
    bool cs = (address & 0x0400) == 0x0400;   // 0 - address is 0x3FF or below ; 1 - address is 0x400 or above

    printf("\naddr - 0x%03x\trow - 0x%02x\tcol - 0x%02x\tcs = %d", address, row, col, cs);

    if(IOM==1){
        unsigned char shiftVal = (memWidth - 1 - col);  // Contains the number to shift the data by (getting the column)
        if(RW==0 && OE==1){// memory read
            unsigned long reader = 0x00000001;                       // Used to determine which bit to read/write
            unsigned char data = 0x00;
            if (!cs) {
                data = data | (unsigned char)( (A1[row] & (reader << shiftVal) ) >> (shiftVal) ) << 0;    // Get value stored in bit no. col of A1[row], shift it to lsb, then shift to 1st bit then store to data
                data = data | (unsigned char)( (A2[row] & (reader << shiftVal) ) >> (shiftVal) ) << 1;    // Get value stored in bit no. col of A2[row], shift it to lsb, then shift to 2nd bit then store to data
                data = data | (unsigned char)( (A3[row] & (reader << shiftVal) ) >> (shiftVal) ) << 2;    // Get value stored in bit no. col of A3[row], shift it to lsb, then shift to 3rd bit then store to data
                data = data | (unsigned char)( (A4[row] & (reader << shiftVal) ) >> (shiftVal) ) << 3;    // Get value stored in bit no. col of A4[row], shift it to lsb, then shift to 4th bit then store to data
                data = data | (unsigned char)( (A5[row] & (reader << shiftVal) ) >> (shiftVal) ) << 4;    // Get value stored in bit no. col of A5[row], shift it to lsb, then shift to 5th bit then store to data
                data = data | (unsigned char)( (A6[row] & (reader << shiftVal) ) >> (shiftVal) ) << 5;    // Get value stored in bit no. col of A6[row], shift it to lsb, then shift to 6th bit then store to data
                data = data | (unsigned char)( (A7[row] & (reader << shiftVal) ) >> (shiftVal) ) << 6;    // Get value stored in bit no. col of A7[row], shift it to lsb, then shift to 7th bit then store to data
                data = data | (unsigned char)( (A8[row] & (reader << shiftVal) ) >> (shiftVal) ) << 7;    // Get value stored in bit no. col of A8[row], shift it to lsb, then shift to 8th bit then store to data
            }else{
                row = row - 32;
                data = data | (unsigned char)( (B1[row] & (reader << shiftVal) ) >> (shiftVal) ) << 0;    // Get value stored in bit no. col of A1[row], shift it to lsb, then shift to 1st bit then store to data
                data = data | (unsigned char)( (B2[row] & (reader << shiftVal) ) >> (shiftVal) ) << 1;    // Get value stored in bit no. col of A2[row], shift it to lsb, then shift to 2nd bit then store to data
                data = data | (unsigned char)( (B3[row] & (reader << shiftVal) ) >> (shiftVal) ) << 2;    // Get value stored in bit no. col of A3[row], shift it to lsb, then shift to 3rd bit then store to data
                data = data | (unsigned char)( (B4[row] & (reader << shiftVal) ) >> (shiftVal) ) << 3;    // Get value stored in bit no. col of A4[row], shift it to lsb, then shift to 4th bit then store to data
                data = data | (unsigned char)( (B5[row] & (reader << shiftVal) ) >> (shiftVal) ) << 4;    // Get value stored in bit no. col of A5[row], shift it to lsb, then shift to 5th bit then store to data
                data = data | (unsigned char)( (B6[row] & (reader << shiftVal) ) >> (shiftVal) ) << 5;    // Get value stored in bit no. col of A6[row], shift it to lsb, then shift to 6th bit then store to data
                data = data | (unsigned char)( (B7[row] & (reader << shiftVal) ) >> (shiftVal) ) << 6;    // Get value stored in bit no. col of A7[row], shift it to lsb, then shift to 7th bit then store to data
                data = data | (unsigned char)( (B8[row] & (reader << shiftVal) ) >> (shiftVal) ) << 7;    // Get value stored in bit no. col of A8[row], shift it to lsb, then shift to 8th bit then store to data
            }
            BUS = data; // store obtained data to data BUS
        }else if(RW==1 && OE==1){// memory write
            unsigned char data = BUS;
            if(!cs){
                A1[row] = A1[row] | ( (long) ((data & 0b00000001) >> 0) << shiftVal );
                A2[row] = A2[row] | ( (long) ((data & 0b00000010) >> 1) << shiftVal );
                A3[row] = A3[row] | ( (long) ((data & 0b00000100) >> 2) << shiftVal );
                A4[row] = A4[row] | ( (long) ((data & 0b00001000) >> 3) << shiftVal );
                A5[row] = A5[row] | ( (long) ((data & 0b00010000) >> 4) << shiftVal );
                A6[row] = A6[row] | ( (long) ((data & 0b00100000) >> 5) << shiftVal );
                A7[row] = A7[row] | ( (long) ((data & 0b01000000) >> 6) << shiftVal );
                A8[row] = A8[row] | ( (long) ((data & 0b10000000) >> 7) << shiftVal );
            }else{
                row = row-32;
                B1[row] = B1[row] | ( (long) ((data & 0b00000001) >> 0) << shiftVal );
                B2[row] = B2[row] | ( (long) ((data & 0b00000010) >> 1) << shiftVal );
                B3[row] = B3[row] | ( (long) ((data & 0b00000100) >> 2) << shiftVal );
                B4[row] = B4[row] | ( (long) ((data & 0b00001000) >> 3) << shiftVal );
                B5[row] = B5[row] | ( (long) ((data & 0b00010000) >> 4) << shiftVal );
                B6[row] = B6[row] | ( (long) ((data & 0b00100000) >> 5) << shiftVal );
                B7[row] = B7[row] | ( (long) ((data & 0b01000000) >> 6) << shiftVal );
                B8[row] = B8[row] | ( (long) ((data & 0b10000000) >> 7) << shiftVal );
            }
            
        }
    }
}

// -------------------- START OF MAIN MEMORY IMPLEMENTATION -------------------- //

// BELOW HERE ARE JUST USED FOR TESTING

void printBin(unsigned long int data, unsigned char data_width){
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

void main(void){
    IOM = 1;    OE = 1;

    printf("\n\n=========START=========\n");
    BUS = 0xFF;
    ADDR = 0x401;
    RW = 1;     //WRITE OPERATION
    MainMemory();

    BUS = 0x00;
    ADDR = 0x401;
    RW = 0;     //READ OPERATION
    MainMemory();

    //Should be writing then reading from bit no 1 in B<n>[0]

    printf("\nWrote 0xFF to addr 0x401, then read: 0x%02x", BUS);
    printf("\nB1[0] - "); printBin(B1[0], 32);
    printf("\nB2[0] - "); printBin(B2[0], 32);
    printf("\nB3[0] - "); printBin(B3[0], 32);
    printf("\nB4[0] - "); printBin(B4[0], 32);
    printf("\nB5[0] - "); printBin(B5[0], 32);
    printf("\nB6[0] - "); printBin(B6[0], 32);
    printf("\nB7[0] - "); printBin(B7[0], 32);
    printf("\nB8[0] - "); printBin(B8[0], 32);


    printf("\n\n=========START=========\n");
    BUS = 0xFF;
    ADDR = 0x028;
    RW = 1;     //WRITE OPERATION
    MainMemory();

    //BUS = 0xFF;
    ADDR = 0x028;
    RW = 0;     //READ OPERATION
    MainMemory();

    //Should be writing then reading from bit no 8 in A<n>[1]
    printf("\nWrote 0xFF to addr 0x028, then read: 0x%02x", BUS);
    printf("\nA1[1] - "); printBin(A1[1], 32);
    printf("\nA2[1] - "); printBin(A2[1], 32);
    printf("\nA3[1] - "); printBin(A3[1], 32);
    printf("\nA4[1] - "); printBin(A4[1], 32);
    printf("\nA5[1] - "); printBin(A5[1], 32);
    printf("\nA6[1] - "); printBin(A6[1], 32);
    printf("\nA7[1] - "); printBin(A7[1], 32);
    printf("\nA8[1] - "); printBin(A8[1], 32);
}




