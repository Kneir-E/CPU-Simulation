#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

// Function Prototypes
int CU(void);
void initMemory(void);
void IOMemory(void);
void MainMemory(void);
void echo_Registers(int MBR, int IOBR, int MAR, int IOAR);

int ALU(void);
unsigned char FLAGS = 0;
unsigned char twosComp(unsigned data);
void setFlags(unsigned short int ACC);
void resetFlags();
void printFlags();

void InputSim();
void SevenSegment();

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



// -------------------- START OF IO IMPLEMENTATION -------------------- //
unsigned char iOData[32];

void IOMemory(){
    if(OE){
        if(RW && !IOM){
            if(ADDR>=0x000&&ADDR<=0x00F)iOData[ADDR] = BUS;
        }else{
            if(ADDR>=0x010&&ADDR<=0x01F) BUS = iOData[ADDR];
        }
        InputSim();
        if(ADDR==0x000) SevenSegment();
    }
}
// -------------------- END OF IO IMPLEMENTATION -------------------- //

// -------------------- START OF MAIN MEMORY IMPLEMENTATION -------------------- //
#define memWidth 32

unsigned long   A1[memWidth], A2[memWidth],    // 0x00
                A3[memWidth], A4[memWidth],    //  |
                A5[memWidth], A6[memWidth],    //  |
                A7[memWidth], A8[memWidth];    // 0x3FF

unsigned long   B1[memWidth], B2[memWidth],    // 0x400
                B3[memWidth], B4[memWidth],    //  |
                B5[memWidth], B6[memWidth],    //  |
                B7[memWidth], B8[memWidth];    // 0x7FF

void printBin(int data, unsigned char data_width){  //========================================================================DELETE DIS====================
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


void MainMemory(void){      // handles MainMemory <-> BUS exchanges
    unsigned short int address = ADDR;      // 11-bit
    unsigned char row = (unsigned char) ((address >> 5) & 0b0000000000011111);   // 5-bit
    unsigned char col = (unsigned char) ( address & 0b0000000000011111);   // 5-bit
    bool cs = (address & 0x0400) == 0x0400;   // 0 - address is 0x3FF or below ; 1 - address is 0x400 or above

    //printf("\naddr - 0x%03x\trow - 0x%02x\tcol - 0x%02x\tcs = %d", address, row, col, cs);

    if(IOM==1){
        unsigned char shiftVal = (memWidth - 1 - col);  // Contains the number to shift the data by (getting the column)
        if(RW==0 && OE==1){// memory read
        	//printf("Memory Read");
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
                //row = row - 32;
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
        	//printf("Memory Write");
            unsigned char data = BUS;
            if(!cs){
                A1[row] = A1[row] & ~( (long) 0b00000001 << shiftVal );  //reset data in address
                A2[row] = A2[row] & ~( (long) 0b00000001 << shiftVal );
                A3[row] = A3[row] & ~( (long) 0b00000001 << shiftVal );
                A4[row] = A4[row] & ~( (long) 0b00000001 << shiftVal );
                A5[row] = A5[row] & ~( (long) 0b00000001 << shiftVal );
                A6[row] = A6[row] & ~( (long) 0b00000001 << shiftVal );
                A7[row] = A7[row] & ~( (long) 0b00000001 << shiftVal );
                A8[row] = A8[row] & ~( (long) 0b00000001 << shiftVal );

                A1[row] = A1[row] | ( (long) ((data & 0b00000001) >> 0) << shiftVal );  //write data to address
                A2[row] = A2[row] | ( (long) ((data & 0b00000010) >> 1) << shiftVal );
                A3[row] = A3[row] | ( (long) ((data & 0b00000100) >> 2) << shiftVal );
                A4[row] = A4[row] | ( (long) ((data & 0b00001000) >> 3) << shiftVal );
                A5[row] = A5[row] | ( (long) ((data & 0b00010000) >> 4) << shiftVal );
                A6[row] = A6[row] | ( (long) ((data & 0b00100000) >> 5) << shiftVal );
                A7[row] = A7[row] | ( (long) ((data & 0b01000000) >> 6) << shiftVal );
                A8[row] = A8[row] | ( (long) ((data & 0b10000000) >> 7) << shiftVal );
            }else{
                //row = row-32;
                B1[row] = B1[row] & ~( (long) 0b00000001 << shiftVal );  //reset data in address
                B2[row] = B2[row] & ~( (long) 0b00000001 << shiftVal );
                B3[row] = B3[row] & ~( (long) 0b00000001 << shiftVal );
                B4[row] = B4[row] & ~( (long) 0b00000001 << shiftVal );
                B5[row] = B5[row] & ~( (long) 0b00000001 << shiftVal );
                B6[row] = B6[row] & ~( (long) 0b00000001 << shiftVal );
                B7[row] = B7[row] & ~( (long) 0b00000001 << shiftVal );
                B8[row] = B8[row] & ~( (long) 0b00000001 << shiftVal );

                B1[row] = B1[row] | ( (long) ((data & 0b00000001) >> 0) << shiftVal );  //write data to address
                B2[row] = B2[row] | ( (long) ((data & 0b00000010) >> 1) << shiftVal );
                B3[row] = B3[row] | ( (long) ((data & 0b00000100) >> 2) << shiftVal );
                B4[row] = B4[row] | ( (long) ((data & 0b00001000) >> 3) << shiftVal );
                B5[row] = B5[row] | ( (long) ((data & 0b00010000) >> 4) << shiftVal );
                B6[row] = B6[row] | ( (long) ((data & 0b00100000) >> 5) << shiftVal );
                B7[row] = B7[row] | ( (long) ((data & 0b01000000) >> 6) << shiftVal );
                B8[row] = B8[row] | ( (long) ((data & 0b10000000) >> 7) << shiftVal );
            }
            
        }
        // printf("\n");
        // printBin(B1[0], 32);printf("\n");
        // printBin(B2[0], 32);printf("\n");
        // printBin(B3[0], 32);printf("\n");
        // printBin(B4[0], 32);printf("\n");
        // printBin(B5[0], 32);printf("\n");
        // printBin(B6[0], 32);printf("\n");
        // printBin(B7[0], 32);printf("\n");
        // printBin(B8[0], 32);printf("\n");
    }
}

// -------------------- END OF MAIN MEMORY IMPLEMENTATION -------------------- //



// -------------------- START OF CONTROL UNIT IMPLEMENTATION -------------------- //
int CU(void){
    // Initialize CU variables
    unsigned char  MBR=0, IOBR=0, inst_code=0;
    unsigned short int PC=0, IR=0, MAR=0, IOAR=0, operand=0;

    unsigned char Inc=0, Fetch=0, IO=0, MEM=0;
    int ret=0;

    // CONTROL UNIT SUPERLOOP
    while(1){
        printf("\n***********************************");
        printf("\nPC                     : 0x%03X", PC);
        printf("\nFetching instruction...");

        // Fetch Cycle
        /* setting external control signals */
        CONTROL = inst_code;    // setting the control signals
        IOM = 1;        RW = 0;         OE = 1;     // SYS_MEM - READ - DATA ENABLE

        /* Fetching Instruction (2 cycle) */
        Fetch = 1;      IO = 0;         MEM = 0;    // Fetch - reset IO and MEM access control signals
        ADDR = PC;
        MainMemory();   //Call BUS Function
        if(Fetch==1){
            IR = (int) BUS;     // load instruction to IR
            IR = IR << 8;       // shift IR 8 bits to the left
            PC++;               // points to the lower byte
            ADDR = PC;          // update address bus
        }
        /* fetching the lower byte */
        MainMemory();           // fetch lower byte
        if(Fetch==1){
            IR = IR | BUS;      // load the instruction on bus to lower 8 bits of IR
            PC++;               // points to the next instruction
        }
        printf("\nIR                     : 0x%03X", IR);

        // Decode Cycle
        inst_code = IR >> 11;   // get the 5-bit instruction code
        CONTROL = inst_code;
        operand = IR & 0x07FF;  // get the 11-bit operand
        printf("\nInstruction Code       : 0x%02X", inst_code);
        printf("\nOperand                : 0x%03X", operand);

        // Execute Cycle
        if(inst_code==0x01){            // INSTRUCTION WM: WRITE to SYS_MEM --------------------
            printf("\nInstruction            : WM");
            printf("\nWriting data to memory...");
            MAR = operand;              // load the operand to MAR

            // ----- LOCAL control signals -----
            Fetch = 0;      MEM = 1;        IO = 0;     // No Fetch - MEMORY access
            
            // ----- BUS control signals (external Signals) -----
            CONTROL = inst_code;    // set CONTROL signals
            IOM = 1;        RW = 1;         OE = 1;     // SYS_MEM - WRITE - DATA ENABLE

            ADDR = MAR;         // load MAR to Address Bus
            if(MEM) BUS = MBR;  // STORE MBR to BUS
            MainMemory();       // write data in data bus to memory

            echo_Registers(MBR, IOBR, MAR, IOAR);

        }else if(inst_code==0x02){      // INSTRUCTION RM: READ from SYS_MEM -------------------
            printf("\nInstruction            : RM");
            printf("\nReading data from memory...");
            MAR = operand;              // load the operand to MAR

            // ----- LOCAL control signals -----
            Fetch = 0;      MEM = 1;         IO = 0;    // No Fetch - MEMORY access

            // ----- BUS control signals (external Signals) -----
            CONTROL = inst_code;    // set CONTROL signals
            IOM = 1;        RW = 0;         OE = 1;     // SYS_MEM - READ - DATA ENABLE

            ADDR = MAR;         // load MAR to Address Bus
            MainMemory();       // read data from memory to data bus
            if(MEM) MBR = BUS;  // STORE data in BUS to MBR

            printf("\nMBR                    : 0x%02X", MBR);
        
            echo_Registers(MBR, IOBR, MAR, IOAR);
            
        }else if(inst_code==0x03){      // INSTRUCTION BR: BRANCH to INST ----------------------
            printf("\nInstruction            : BR");
            
            printf("\nBranch to 0x%03X on next cycle.", operand);
            ADDR = operand;
            PC = ADDR;                  // update PC to point to new INST

            echo_Registers(MBR, IOBR, MAR, IOAR);
            
        }else if(inst_code==0x04){      // INSTRUCTION RIO: READ from IO_BUFF -------------------
            printf("\nInstruction            : RIO");
            printf("\nReading data from IO...");
            IOAR = operand;             // load the operand to IOAR

            // ----- LOCAL control signals -----
            Fetch = 0;      MEM = 0;        IO = 1;     // No Fetch - IO access

            // ----- BUS control signals (external Signals) -----
            CONTROL = inst_code;    // set CONTROL signals
            IOM = 0;        RW = 0;         OE = 1;     // IO MEM - READ - DATA ENABLE

            ADDR = IOAR;        // load IOAR to Address Bus
            IOMemory();         // read data from memory to data bus
            if(IO) IOBR = BUS;  // STORE data in BUS to IOBR

            printf("\nIOBR                   : 0x%02X", IOBR);
            echo_Registers(MBR, IOBR, MAR, IOAR);
            
        }else if(inst_code==0x05){      // INSTRUCTION WIO: WRITE to IO_BUFF --------------------
            printf("\nInstruction            : WIO");
            printf("\nWriting to IO buffer...");
            IOAR = operand;             // load the operand to IOAR      !!! IOAR should be 5 bits.

            // ----- LOCAL control signals -----
            Fetch = 0;      MEM = 0;        IO = 1;     // IO MEM - READ - DATA ENABLE

            // ----- BUS control signals (external Signals) -----
            CONTROL = inst_code;    // set CONTROL signals
            IOM = 0;        RW = 1;         OE = 1;     // IO MEM - WRITE - DATA ENABLE

            ADDR = IOAR;        // load IOAR to Address Bus
            if(IO) BUS = IOBR;  // STORE IOBR to BUS
            IOMemory();         // write data from data bus to IO Memory
            
            echo_Registers(MBR, IOBR, MAR, IOAR);
        }else if(inst_code==0x06){      // INSTRUCTION WB: WRITE to MBR ------------------------
            printf("\nInstruction            : WB");
            printf("\nWriting data to MBR...");
            MBR=(unsigned char)operand; // load the lower 8 bits to MBR
            printf("\nMBR                    : 0x%02X", MBR);
            
            echo_Registers(MBR, IOBR, MAR, IOAR);

        }else if(inst_code==0x07){      // INSTRUCTION WIB: INSTRUCTION: WRITE to IOBR ----------
            printf("\nInstruction            : WIB");
            printf("\nLoading data to IOBR...");
            IOBR=(unsigned char)operand;// load the lower 8 bits to IOBR
            printf("\nIOBR                   : 0x%02X", IOBR);
            
            echo_Registers(MBR, IOBR, MAR, IOAR);

        }else if(inst_code>=0x15 && inst_code<=0x1E && inst_code!=0x1C){    //ALU Operation
            printf("\nInstruction            : ALU OPERATION");
            BUS = MBR;
            ALU();
            echo_Registers(MBR, IOBR, MAR, IOAR);
        }else if( inst_code>=0x11 && inst_code<=0x14 ){                     //Conditional Branching
            if (CONTROL==0b10100){ printf("\nInstruction            : BRE"); }
            if (CONTROL==0b10011){ printf("\nInstruction            : BRNE"); }
            if (CONTROL==0b10010){ printf("\nInstruction            : BRGT"); }
            if (CONTROL==0b10001){ printf("\nInstruction            : BRLT"); }
            ADDR = operand;
            BUS = MBR;
            //printf("\n===== 0x%X ---- 0x%X =====", ADDR, BUS);
            ALU();
            unsigned char ZF = (FLAGS & 0b00000001);
            unsigned char CF = (FLAGS & 0b00000010) >> 1;
            unsigned char SF = (FLAGS & 0b00000100) >> 2;
            unsigned char OF = (FLAGS & 0b10000000) >> 7;
            if (CONTROL==0b10100 & ZF==1){ PC=ADDR;         printf("\nBranching to instruction in next cycle");}
            else if (CONTROL==0b10011 & ZF==0){ PC=ADDR;    printf("\nBranching to instruction in next cycle");}
            else if (CONTROL==0b10010 & SF==0){ PC=ADDR;    printf("\nBranching to instruction in next cycle");}
            else if (CONTROL==0b10001 & SF==1){ PC=ADDR;    printf("\nBranching to instruction in next cycle");}
            else printf("\nNo Branch");
            echo_Registers(MBR, IOBR, MAR, IOAR);
        }else if( inst_code==0x09 ){   //WACC
            printf("\nInstruction            : WACC");
            BUS = MBR;
            ALU();
            echo_Registers(MBR, IOBR, MAR, IOAR);
        }else if( inst_code==0x0B ){   //RACC
            printf("\nInstruction            : RACC");
            ALU();
            MBR = BUS;
            echo_Registers(MBR, IOBR, MAR, IOAR);
        }else if(inst_code==0x0E){      //SWAP ----------------------
            printf("\nInstruction            : SWAP");
            unsigned char temp = MBR;
            MBR = IOBR;
            IOBR = temp;
            echo_Registers(MBR, IOBR, MAR, IOAR);
        }else if(inst_code==0x1F){      // INSTRUCTION EOP: END of PROGRAM ----------------------
            printf("\nInstruction            : EOP");
            echo_Registers(MBR, IOBR, MAR, IOAR);
            
            printf("\n\nEnd of program\n***********************************");
            ret=1;
            break;
        }else{                          // Error: inst_code not recognized
            ret=0;
            break;
        }

        getch();
    }
    return ret;
}
// -------------------- END OF CONTROL UNIT IMPLEMENTATION -------------------- //


// -------------------- START OF ALU IMPLEMENTATION -------------------- //
int ALU(void){
	static unsigned short int ACC = 0;
	printf("\n    === Entered ALU ===\n");
    
	int temp_OP2=BUS, temp_ACC=0, ret=0, skipFlagPrint=0;
	resetFlags();

	if(CONTROL==0b11110 || CONTROL==0b11101){	//ADD or SUB
		if(CONTROL==0b11101){
            temp_OP2 = twosComp(temp_OP2);
            printf("    Operation:      SUB\n");
        }else printf("    Operation:      ADD\n");
		temp_ACC = (int) ACC + temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b11011){		//MUL
        printf("    Operation:      MUL\n");
		temp_ACC = (int) ACC * temp_OP2;
		ACC = (unsigned short int) temp_ACC;
		setFlags(ACC);
        printf("    ACC:            0x%04X\n", ACC);
	}else if(CONTROL==0b11010){		//AND
        printf("    Operation:      AND\n");
		temp_ACC = ACC & temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b11001){		//OR
        printf("    Operation:      OR\n");
		temp_ACC = ACC | temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b11000){		//NOT
        printf("    Operation:      NOT\n");
		temp_ACC = ~ACC;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b10111){		//XOR
        printf("    Operation:      XOR\n");
		temp_ACC = ACC ^ temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(ACC);
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b10110){		//SHL
        printf("    Operation:      SHL\n");
		temp_ACC = ACC << 1;
        FLAGS = FLAGS | (((unsigned char) ACC >> 6) & 0b00000010);
		ACC = (unsigned char) temp_ACC;
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b10101){		//SHR
        printf("    Operation:      SHR\n");
		temp_ACC = ACC >> 1;
	    FLAGS = FLAGS | (((unsigned char) ACC << 1) & 0b00000010);
		ACC = (unsigned char) temp_ACC;
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b01001){		//WACC
		ACC = BUS;
		skipFlagPrint=1;
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b01011){		//RACC
        printf("    Operation:      RACC\n");
		BUS = (unsigned char) ACC;
		skipFlagPrint=1;
        printf("    ACC:            0x%02X\n", ACC);
	}else if(CONTROL==0b10100 | CONTROL==0b10011 | CONTROL==0b10010 | CONTROL==0b10001){		//BRE	//BRNE	//BRGT	//BRLT
        printf("    Operation:      CMP\n");
		temp_ACC = ACC - temp_OP2;
		ACC = (unsigned char) temp_ACC;
		setFlags(temp_ACC);
        printf("    ACC:            0x%02X\n", ACC);
	}else	ret=1;
	if(skipFlagPrint==0) printFlags();
    printf("    === Exited ALU ===");
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
    if(ACC>0xff)	FLAGS = FLAGS | 0b10000000; 	else FLAGS = FLAGS & 0b01111111;	// OF
}
void resetFlags(){
	FLAGS = 0;
}
void printFlags(){
	unsigned char ZF = (FLAGS & 0b00000001);
	unsigned char CF = (FLAGS & 0b00000010) >> 1;
	unsigned char SF = (FLAGS & 0b00000100) >> 2;
	unsigned char OF = (FLAGS & 0b10000000) >> 7;
	printf("    Flags:\n");
    printf("           ZF=      %1d\n", ZF);
    printf("           CF=      %1d\n", CF);
    printf("           OF=      %1d\n", OF);
    printf("           SF=      %1d\n", SF);
}
// -------------------- END OF ALU IMPLEMENTATION -------------------- //

void echo_Registers(int MBR, int IOBR, int MAR, int IOAR){
    printf("\n");
    printf("\n   BUS                 : 0x%02X", BUS);
    printf("\n   ADDR                : 0x%03X", ADDR);
    printf("\n   MAR                 : 0x%03X", MAR);
    printf("\n   IOAR                : 0x%03X", IOAR);
    printf("\n   IOM                 : 0x%02X", IOM);
    printf("\n   MBR                 : 0x%02X", MBR);
    printf("\n   IOBR                : 0x%02X", IOBR);
    printf("\n   CONTROL             : 0x%02X", CONTROL);
}

// Main Function
void main(void){
    printf("Initializing Main Memory...");
    initMemory();   // Populate the Memory
    if(CU()==1)     printf("\n\nProgram run successfully!");
    else            printf("\n\nError encountered, program terminated!");
}


void SevenSegment(){
    if(iOData[ADDR]>=0x00&&iOData[ADDR]<=0x09)printf("\n\nOUTPUT 7SEG:\n");
    if(iOData[ADDR]==0x01)
    {
        printf("    X\n");
        printf("    X\n");
        printf("    X\n");
        printf("    X\n");
        printf("    X\n");
        printf("    X\n");
        printf("    X\n");
    }    
    else if(iOData[ADDR]==0x02)
    {
        printf(" XXXXX\n");
        printf("     X\n");
        printf("     X\n");
        printf(" XXXXX\n");
        printf(" X    \n");
        printf(" X    \n");
        printf(" XXXXX\n");
    }    
    else if(iOData[ADDR]==0x03)
    {
        printf(" XXXXX\n");
        printf("     X\n");
        printf("     X\n");
        printf(" XXXXX\n");
        printf("     X\n");
        printf("     X\n");
        printf(" XXXXX\n");
    }    
    else if(iOData[ADDR]==0x04)
    {
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" XXXXX\n");
        printf("     X\n");
        printf("     X\n");
        printf("     X\n");
    }
    else if(iOData[ADDR]==0x05)
    {
        printf(" XXXXX\n");
        printf(" X    \n");
        printf(" X    \n");
        printf(" XXXXX\n");
        printf("     X\n");
        printf("     X\n");
        printf(" XXXXX\n");
    }    
    else if(iOData[ADDR]==0x06)
    {
        printf(" XXXXX\n");
        printf(" X    \n");
        printf(" X    \n");
        printf(" XXXXX\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" XXXXX\n");
    }
    else if(iOData[ADDR]==0x07)
    {
        printf(" XXXXX\n");
        printf("     X\n");
        printf("     X\n");
        printf("     X\n");
        printf("     X\n");
        printf("     X\n");
        printf("     X\n");
    }  
    else if(iOData[ADDR]==0x08)
    {
        printf(" XXXXX\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" XXXXX\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" XXXXX\n");
    }
    else if(iOData[ADDR]==0x09)
    {
        printf(" XXXXX\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" XXXXX\n");
        printf("     X\n");
        printf("     X\n");
        printf(" XXXXX\n");
    }
    else if(iOData[ADDR]==0x00)
    {
        printf(" XXXXX\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" X   X\n");
        printf(" XXXXX\n");
    }
    //getch();
   // getc(stdin);
}

void InputSim(void){
    unsigned char data;
    int i;
    for(i=7; i>=0; i--){
        data = iOData[0x001];               /* load source data */
        data = data >> i;                   /* shift bit to LSB */
        data = data & 0x01;                 /* mask upper bits */
        data = data << (7 - i);             /* position bit */
        iOData[0x01F] = iOData[0x01F] | data;   /* write bit to dest buffer */
    }
}

void initMemory(void){
	IOM = 1; RW = 1; OE = 1;
    ADDR = 0x000; BUS = 0x30; MainMemory();	//START:  WB 0X09         ;MBR = 9
    ADDR = 0x001; BUS = 0x09; MainMemory();
    ADDR = 0x002; BUS = 0x0C; MainMemory();	//        WM 0X406        ;MainMemory[0x406] = 9,8,7,6,5,4,3,2,1,0
    ADDR = 0x003; BUS = 0x06; MainMemory();
    ADDR = 0x004; BUS = 0x14; MainMemory();	//LOOP:   RM 0X406        ;MBR = MainMemory[0x406] = 9,8,7,6,5,4,3,2,1,0
    ADDR = 0x005; BUS = 0x06; MainMemory();
    ADDR = 0x006; BUS = 0x48; MainMemory();	//        WACC            ;ACC = MBR = 9,8,7,6,5,4,3,2,1,0
    ADDR = 0x007; BUS = 0x00; MainMemory();
    ADDR = 0x008; BUS = 0x70; MainMemory();	//        SWAP            ;MBR <-> IOBR
    ADDR = 0x009; BUS = 0x00; MainMemory();
    ADDR = 0x00A; BUS = 0x28; MainMemory();	//        WIO 0X00        ;IOMEM[0x00] = IOBR = 9,8,7,6,5,4,3,2,1,0
    ADDR = 0x00B; BUS = 0x00; MainMemory();
    ADDR = 0x00C; BUS = 0x30; MainMemory();	//        WB  0X01        ;MBR = 1
    ADDR = 0x00D; BUS = 0x01; MainMemory();
    ADDR = 0x00E; BUS = 0xE8; MainMemory();	//        SUB             ;ACC = ACC - 1
    ADDR = 0x00F; BUS = 0x00; MainMemory();
    ADDR = 0x010; BUS = 0x58; MainMemory();	//        RACC            ;MBR = ACC
    ADDR = 0x011; BUS = 0x00; MainMemory();
    ADDR = 0x012; BUS = 0x0C; MainMemory();	//        WM 0X406        ;MainMemory[0x406] = ACC
    ADDR = 0x013; BUS = 0x06; MainMemory();
    ADDR = 0x014; BUS = 0x30; MainMemory();	//        WB 0X00         ;WB = 0x00
    ADDR = 0x015; BUS = 0x00; MainMemory();
    ADDR = 0x016; BUS = 0x90; MainMemory();	//        BRGT LOOP       ;JMP to LOOP if currnum>=0
    ADDR = 0x017; BUS = 0x04; MainMemory();
    ADDR = 0x018; BUS = 0xF8; MainMemory();	//        EOP
    ADDR = 0x019; BUS = 0x00; MainMemory();
}

