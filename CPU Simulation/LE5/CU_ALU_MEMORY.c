#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

// Function Prototypes
int CU(void);
void initMemory(void);
void IOMemory(void);
void MainMemory(void);
void echo_Registers(int IOBR, int MAR, int IOAR);

int ALU(void);
    unsigned char FLAGS = 0;
	unsigned char twosComp(unsigned data);
	void setFlags(unsigned short int ACC);
	void resetFlags();
	void printFlags();

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
    
    // DATA BUS FUNCTIONS
//			//MOVED TO MAIN MEMORY IMPLEMENTATION
//        void MainMemory(void){      // handles MainMemory <-> BUS exchanges
//            if(IOM==1){
//                if(RW==0 && OE==1)      BUS = SYS_MEM[ADDR];// memory read
//                else if(RW==1 && OE==1) SYS_MEM[ADDR] = BUS;// memory write
//            }
//        }
		unsigned char IO_BUFF[0x1F];    // IO Memory
        void IOMemory(void){        // handles IOMemory <-> BUS exchanges
            if(IOM==0){
                if(RW==0 && OE==1)      BUS = IO_BUFF[ADDR];// IO read
                else if(RW==1 && OE==1) IO_BUFF[ADDR] = BUS;// IO write
            }
        }
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
                A1[row] = A1[row] | ( (long) ((data & 0b00000001) >> 0) << shiftVal );
                A2[row] = A2[row] | ( (long) ((data & 0b00000010) >> 1) << shiftVal );
                A3[row] = A3[row] | ( (long) ((data & 0b00000100) >> 2) << shiftVal );
                A4[row] = A4[row] | ( (long) ((data & 0b00001000) >> 3) << shiftVal );
                A5[row] = A5[row] | ( (long) ((data & 0b00010000) >> 4) << shiftVal );
                A6[row] = A6[row] | ( (long) ((data & 0b00100000) >> 5) << shiftVal );
                A7[row] = A7[row] | ( (long) ((data & 0b01000000) >> 6) << shiftVal );
                A8[row] = A8[row] | ( (long) ((data & 0b10000000) >> 7) << shiftVal );
            }else{
                //row = row-32;
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

            echo_Registers(IOBR, MAR, IOAR);

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
        
            echo_Registers(IOBR, MAR, IOAR);
            
        }else if(inst_code==0x03){      // INSTRUCTION BR: BRANCH to INST ----------------------
            printf("\nInstruction            : BR");
            
            printf("\nBranch to 0x%03X on next cycle.", operand);
            ADDR = operand;
            PC = ADDR;                  // update PC to point to new INST

            echo_Registers(IOBR, MAR, IOAR);
            
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
            echo_Registers(IOBR, MAR, IOAR);
            
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
            
            echo_Registers(IOBR, MAR, IOAR);
        }else if(inst_code==0x06){      // INSTRUCTION WB: WRITE to MBR ------------------------
            printf("\nInstruction            : WB");
            printf("\nWriting data to MBR...");
            MBR=(unsigned char)operand; // load the lower 8 bits to MBR
            printf("\nMBR                    : 0x%02X", MBR);
            
            echo_Registers(IOBR, MAR, IOAR);

        }else if(inst_code==0x07){      // INSTRUCTION WIB: INSTRUCTION: WRITE to IOBR ----------
            printf("\nInstruction            : WIB");
            printf("\nLoading data to IOBR...");
            IOBR=(unsigned char)operand;// load the lower 8 bits to IOBR
            printf("\nIOBR                   : 0x%02X", IOBR);
            
            echo_Registers(IOBR, MAR, IOAR);

        }else if(inst_code>=0x15 && inst_code<=0x1E && inst_code!=0x1C){    //ALU Operation
            printf("\nInstruction            : ALU OPERATION");
            BUS = MBR;
            ALU();
            echo_Registers(IOBR, MAR, IOAR);
        }else if( inst_code>=0x11 && inst_code<=0x14 ){                     //Conditional Branching
            if (CONTROL==0b10100){ printf("\nInstruction            : BRE"); }
            if (CONTROL==0b10011){ printf("\nInstruction            : BRNE"); }
            if (CONTROL==0b10010){ printf("\nInstruction            : BRGT"); }
            if (CONTROL==0b10001){ printf("\nInstruction            : BRLT"); }
            ADDR = operand;
            BUS = MBR;
            printf("\n===== 0x%X ---- 0x%X =====", ADDR, BUS);
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
            echo_Registers(IOBR, MAR, IOAR);
        }else if( inst_code==0x09 ){   //WACC
            printf("\nInstruction            : WACC");
            BUS = MBR;
            ALU();
            echo_Registers(IOBR, MAR, IOAR);
        }else if( inst_code==0x0B ){   //RACC
            printf("\nInstruction            : RACC");
            ALU();
            MBR = BUS;
            echo_Registers(IOBR, MAR, IOAR);
        }else if(inst_code==0x0E){      //SWAP ----------------------
            printf("\nInstruction            : SWAP");
            unsigned char temp = MBR;
            MBR = IOBR;
            IOBR = temp;
            echo_Registers(IOBR, MAR, IOAR);
        }else if(inst_code==0x1F){      // INSTRUCTION EOP: END of PROGRAM ----------------------
            printf("\nInstruction            : EOP");
            echo_Registers(IOBR, MAR, IOAR);
            
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
	//if(ACC>0x7f&&ACC<0xff)	FLAGS = FLAGS | 0b10000000; 	else FLAGS = FLAGS & 0b01111111;	// OF
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

void echo_Registers(int IOBR, int MAR, int IOAR){
    printf("\n");
    printf("\n   BUS                 : 0x%02X", BUS);
    printf("\n   ADDR                : 0x%03X", ADDR);
    printf("\n   MAR                 : 0x%03X", MAR);
    printf("\n   IOAR                : 0x%03X", IOAR);
    printf("\n   IOM                 : 0x%02X", IOM);
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

void initMemory(void){
	IOM = 1; RW = 1; OE = 1;
    ADDR = 0x000; BUS = 0x030; MainMemory(); // MBR = 0x15 
	ADDR = 0x001; BUS = 0x015; MainMemory();
	ADDR = 0x002; BUS = 0x00C; MainMemory(); // dataMemory[0x400] : 0x15 
	ADDR = 0x003; BUS = 0x000; MainMemory();
	ADDR = 0x004; BUS = 0x030; MainMemory(); // MBR = 0x05 
	ADDR = 0x005; BUS = 0x005; MainMemory();
	ADDR = 0x006; BUS = 0x048; MainMemory(); // ACC = 0x05 
	ADDR = 0x007; BUS = 0x000; MainMemory();
	ADDR = 0x008; BUS = 0x030; MainMemory(); // MBR = 0x08 
	ADDR = 0x009; BUS = 0x008; MainMemory();
	ADDR = 0x00A; BUS = 0x0F0; MainMemory(); // ACC = (0x05) + (0x08) = 0x0D ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x00B; BUS = 0x000; MainMemory();
	ADDR = 0x00C; BUS = 0x014; MainMemory(); // MBR = 0x15 
	ADDR = 0x00D; BUS = 0x000; MainMemory();
	ADDR = 0x00E; BUS = 0x0D8; MainMemory(); // ACC = (0x0D) x (0x15) = 0x11 ZF=0, CF=1, OF=1, SF=0 
	ADDR = 0x00F; BUS = 0x000; MainMemory();
	ADDR = 0x010; BUS = 0x058; MainMemory(); // MBR = 0x11 
	ADDR = 0x011; BUS = 0x000; MainMemory();
	ADDR = 0x012; BUS = 0x00C; MainMemory(); // dataMemory[0x401] : 0x11 
	ADDR = 0x013; BUS = 0x001; MainMemory();
	ADDR = 0x014; BUS = 0x038; MainMemory(); // IOBR = 0x0B 
	ADDR = 0x015; BUS = 0x00B; MainMemory();
	ADDR = 0x016; BUS = 0x028; MainMemory(); // ioBuffer[0x000] : 0x0B 
	ADDR = 0x017; BUS = 0x000; MainMemory();
	ADDR = 0x018; BUS = 0x030; MainMemory(); // MBR = 0x10 
	ADDR = 0x019; BUS = 0x010; MainMemory();
	ADDR = 0x01A; BUS = 0x0E8; MainMemory(); // ACC = (0x11) - (0x10) = 0x01 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x01B; BUS = 0x000; MainMemory();
	ADDR = 0x01C; BUS = 0x058; MainMemory(); // MBR = 0x01 
	ADDR = 0x01D; BUS = 0x000; MainMemory();
	ADDR = 0x01E; BUS = 0x028; MainMemory(); // ioBuffer[0x001] : 0x0B 
	ADDR = 0x01F; BUS = 0x001; MainMemory();
	ADDR = 0x020; BUS = 0x0B0; MainMemory(); // ACC = (0x01) << 1 = 0x02 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x021; BUS = 0x000; MainMemory();
	ADDR = 0x022; BUS = 0x0B0; MainMemory(); // ACC = (0x02) << 1 = 0x04 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x023; BUS = 0x000; MainMemory();
	ADDR = 0x026; BUS = 0x0A8; MainMemory(); // ACC = (0x04) >> 1 = 0x02 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x027; BUS = 0x000; MainMemory();
	ADDR = 0x024; BUS = 0x014; MainMemory(); // MBR = 0x11 
	ADDR = 0x025; BUS = 0x001; MainMemory();
	ADDR = 0x028; BUS = 0x0C8; MainMemory(); // ACC = (0x02) OR (0x11) = 0x13 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x029; BUS = 0x000; MainMemory();
	ADDR = 0x02A; BUS = 0x0C0; MainMemory(); // ACC = NOT (0x13) = 0xEC ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x02B; BUS = 0x000; MainMemory();
	ADDR = 0x02C; BUS = 0x020; MainMemory(); // IOBR = 0x0B 
	ADDR = 0x02D; BUS = 0x001; MainMemory();
	ADDR = 0x02E; BUS = 0x070; MainMemory(); // MBR = 0x0B, IOBR = 0x11 
	ADDR = 0x02F; BUS = 0x000; MainMemory();
	ADDR = 0x030; BUS = 0x0B8; MainMemory(); // ACC = (0xEC) XOR (0x0B) = 0xE7 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x031; BUS = 0x000; MainMemory();
	ADDR = 0x032; BUS = 0x030; MainMemory(); // MBR = 0xFF 
	ADDR = 0x033; BUS = 0x0FF; MainMemory();
	ADDR = 0x034; BUS = 0x0D0; MainMemory(); // ACC = (0xE7) AND (0xFF) = 0xE7 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x035; BUS = 0x000; MainMemory();
	ADDR = 0x036; BUS = 0x014; MainMemory(); // MBR = 0x11 
	ADDR = 0x037; BUS = 0x001; MainMemory();
	ADDR = 0x038; BUS = 0x0A0; MainMemory(); // ACC = (0xE7) - (0x11) = 0xD6 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x039; BUS = 0x03C; MainMemory();
	ADDR = 0x03A; BUS = 0x030; MainMemory(); // MBR = 0xF0 
	ADDR = 0x03B; BUS = 0x0F0; MainMemory();
	ADDR = 0x03C; BUS = 0x090; MainMemory(); // ACC = (0xD6) - (0xF0) = 0xE6 ZF=0, CF=1, OF=1, SF=1 
	ADDR = 0x03D; BUS = 0x040; MainMemory();
	ADDR = 0x03E; BUS = 0x088; MainMemory(); // ACC = (0xE6) - (0xF0) = 0xF6 ZF=0, CF=1, OF=1, SF=1 
	ADDR = 0x03F; BUS = 0x044; MainMemory();
	ADDR = 0x040; BUS = 0x030; MainMemory(); // unreachable 
	ADDR = 0x041; BUS = 0x000; MainMemory();
	ADDR = 0x042; BUS = 0x048; MainMemory(); // unreachable 
	ADDR = 0x043; BUS = 0x000; MainMemory();
	ADDR = 0x044; BUS = 0x030; MainMemory(); // MBR = 0x03 
	ADDR = 0x045; BUS = 0x003; MainMemory();
	ADDR = 0x046; BUS = 0x048; MainMemory(); // ACC = 0x03 
	ADDR = 0x047; BUS = 0x000; MainMemory();
	ADDR = 0x048; BUS = 0x030; MainMemory(); // MBR = 0x00 
	ADDR = 0x049; BUS = 0x000; MainMemory();
	ADDR = 0x04A; BUS = 0x0A0; MainMemory(); // ACC = (0x03) - (0x00) = 0x03 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x04B; BUS = 0x052; MainMemory();
	ADDR = 0x04C; BUS = 0x030; MainMemory(); // MBR = 0x01 
	ADDR = 0x04D; BUS = 0x001; MainMemory();
	ADDR = 0x04E; BUS = 0x0E8; MainMemory(); // ACC = (0x03) - (0x01) = 0x02 ZF=0, CF=0, OF=0, SF=0 
	ADDR = 0x04F; BUS = 0x000; MainMemory();
	ADDR = 0x050; BUS = 0x018; MainMemory(); // PC <- 0x048 (loop) 
	ADDR = 0x051; BUS = 0x048; MainMemory();
	ADDR = 0x052; BUS = 0x0F8; MainMemory(); 
	ADDR = 0x053; BUS = 0x000; MainMemory();
}

