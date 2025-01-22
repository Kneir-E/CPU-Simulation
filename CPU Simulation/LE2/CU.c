#include <stdio.h>

// Function Prototypes
int CU(void);
void initMemory(void);

// Main Function
void main(void){
    printf("Initializing Main Memory...");
    initMemory(); // this function populates the memory
    if(CU()==1)     printf("\n\nProgram run successfully!");
    else            printf("\n\nError encountered, program terminated!");
}

// MEMORY and IO BUFFER
unsigned char SYS_MEM[0x7FF];
unsigned char IO_BUFF[0x1F];

// SEGMENT ADDRESSES
// Use as:       SYS_MEM[  VAR_NAME + OFFSET  ]
// Example:      SYS_MEM[  DATA_SEG + 0x200  ] - Accesses the 0x200th byte in the DATA SEGMENT
const unsigned short int INST_SEG = 0x000;
const unsigned short int DATA_SEG = 0x400;

// Control Unit Function
int CU(void){
    unsigned char  MBR=0, IOBR=0, inst_code=0;
    unsigned short int PC=0, IR=0, MAR=0, IOAR=0, operand=0;
    int ret = 0;
    //Control Unit SUPERLOOP
    while(1){
        printf("\n****************************");
        printf("\nPC                    : 0x%03X", PC);
        printf("\nFetching instruction...");

        // Fetch Cycle
        IR = SYS_MEM[PC];       // Fetch Upper Byte
        IR = IR << 8;           // Shift Upper Byte to Higher Byte of IR
        PC++;                   // Update PC to point to Lower byte
        IR = IR | SYS_MEM[PC];  // Fetch Lower Byte
        PC++;                   // Update PC to point to next instruction
        printf("\nIR                    : 0x%03X", IR);

        // Decode Cycle
        inst_code = IR >> 11;   // get the 5-bit instruction code
        operand = IR & 0x07FF;  // get the 11-bit operand
        printf("\nInstruction Code      : 0x%02X", inst_code);
        printf("\nOperand               : 0x%03X", operand);

        // Execute Cycle
        if(inst_code==0x01){            // INSTRUCTION WM: WRITE to SYS_MEM --------------------
            printf("\nInstruction           : WM");
            printf("\nWriting data to memory...");
            MAR = operand;              // load the operand to MAR
            SYS_MEM[DATA_SEG+MAR] = MBR;// write data from MBR to SYS_MEM pointed to by MAR
        }else if(inst_code==0x02){      // INSTRUCTION RM: READ from SYS_MEM -------------------
            printf("\nInstruction           : RM");
            printf("\nReading data from memory...");
            MAR = operand;              // load the operand to MAR
            MBR = SYS_MEM[DATA_SEG+MAR];// write data from SYS_MEM pointed to by MAR to MBR
            printf("\nMBR                   : 0x%02X", MBR);
        }else if(inst_code==0x03){      // INSTRUCTION BR: BRANCH to INST ----------------------
            printf("\nInstruction           : BR");
            printf("\nBranch to 0x%03X on next cycle.", operand);
            PC = operand;               // update PC to point to new INST
        }else if(inst_code==0x04){      // INSTRUCTION RIO: READ from IO_BUFF -------------------
            printf("\nInstruction           : RIO");
            printf("\nReading data from IO...");
            IOAR = operand;             // load the operand to IOAR
            IOBR = IO_BUFF[IOAR];       // write data from SYS_MEM pointed to by IOAR to IOBR
            printf("\nIOBR                  : 0x%02X", IOBR);
        }else if(inst_code==0x05){      // INSTRUCTION WIO: WRITE to IO_BUFF --------------------
            printf("\nInstruction           : WIO");
            printf("\nWriting to IO buffer...");
            IOAR = operand;             // load the operand to IOAR      !!! IOAR should be 5 bits.
            IO_BUFF[IOAR] = IOBR;       // write data from IOBR to SYS_MEM pointed to by IOAR
        }else if(inst_code==0x06){      // INSTRUCTION WB: WRITE to MBR ------------------------
            printf("\nInstruction           : WB");
            printf("\nWriting data to MBR...");
            MBR=(unsigned char)operand; // load the lower 8 bits to MBR
            printf("\nMBR                   : 0x%02X", MBR);
        }else if(inst_code==0x07){      // INSTRUCTION WIB: INSTRUCTION: WRITE to IOBR ----------
            printf("\nInstruction           : WIB");
            printf("\nLoading data to IOBR...");
            IOBR=(unsigned char)operand;// load the lower 8 bits to IOBR
            printf("\nIOBR                  : 0x%02X", IOBR);
        }else if(inst_code==0x1F){      // INSTRUCTION EOP: END of PROGRAM ----------------------
            printf("\nInstruction           : EOP");
            printf("\nEnd of program");
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

void initMemory(void){
    SYS_MEM[0x000] = 0x30;  // write data 0xFF to MBR
    SYS_MEM[0x001] = 0xFF;
    SYS_MEM[0x002] = 0x0C;  // write data from MBR to memory address “100 0000 0000B” (0x400)
    SYS_MEM[0x003] = 0x00;
    SYS_MEM[0x004] = 0x14;  // read data at address “100 0000 0000B” (0x400) and store MBR
    SYS_MEM[0x005] = 0x00;
    SYS_MEM[0x006] = 0x19;  // branch to memory address “001 0010 1010” (0x12A)
    SYS_MEM[0x007] = 0x2A;
    SYS_MEM[0x12A] = 0x38;  // branch to memory address “001 0010 1010” (0x12A)
    SYS_MEM[0x12B] = 0x05;  
    SYS_MEM[0x12C] = 0x28;  // writes data from IOBR to IO buffer address “0 1010” (0x0A)
    SYS_MEM[0x12D] = 0x0A;
    SYS_MEM[0x12E] = 0xF8;  // end of program
    SYS_MEM[0x12F] = 0x00;
}