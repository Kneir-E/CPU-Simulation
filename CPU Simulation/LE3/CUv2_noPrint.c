#include <stdio.h>
#include <conio.h>

// Function Prototypes
int CU(void);
void initMemory(void);
void IOMemory(void);
void MainMemory(void);

// BUSES
unsigned short int ADDR;
    // ADDR - Address bus, contains the address to be accessed
unsigned char BUS, CONTROL, IOM, RW, OE;
    // BUS      - Data bus, contains the data
    // CONTROL  - Control Signals: contains the address to be accessed
    // IOM      - IO/M:         1-access Memory     0-access IO
    // RW       - Read/Write:   1-Write             0-Read
    // OE       - Enable:       1-Mem/IO access enabled
                                            

// MEMORY and IO BUFFER
unsigned char SYS_MEM[0x7FF];
unsigned char IO_BUFF[0x1F];


// Main Function
void main(void){
    printf("Initializing Main Memory...");
    initMemory(); // this function populates the memory
    if(CU()==1)     printf("\n\nProgram run successfully!");
    else            printf("\n\nError encountered, program terminated!");
}

// Control Unit Function
int CU(void){
    unsigned char  MBR=0, IOBR=0, inst_code=0;
    unsigned short int PC=0, IR=0, MAR=0, IOAR=0, operand=0;

    unsigned char Inc=0, Fetch=0, IO=0, MEM=0;  //LOCAL CONTROL SIGNALS

    int ret = 0;
    //Control Unit SUPERLOOP
    while(1){
        printf("\n****************************");
        printf("\nPC                    : 0x%03X", PC);
        printf("\nFetching instruction...");


        // Fetch Cycle
        /* setting external control signals */
        CONTROL = inst_code;    // setting the control signals
        IOM = 1;                // SYS_MEM
        RW = 0;                 // READ
        OE = 1;                 // DATA ENABLE

        /* Fetching Instruction (2 cycle) */
        Fetch = 1;              // set local control signal Fetch to 1 to signify fetch operation
        IO = 0;
        MEM = 0;
        /* fetching the upper byte */
        ADDR = PC;
        MainMemory();           // fetch upper byte
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

            // ----- LOCAL control signals -----
            Fetch = 0;
            MEM = 1;                // MEMORY access
            IO = 0;

            // ----- BUS control signals (external Signals) -----
            CONTROL = inst_code;    // set CONTROL signals
            IOM = 1;                // SYS_MEM
            RW = 1;                 // WRITE
            OE = 1;                 // DATA ENABLE

            ADDR = MAR;         // load MAR to Address Bus
            if(MEM) BUS = MBR;  // STORE MBR to BUS
            MainMemory();       // write data in data bus to memory

        }else if(inst_code==0x02){      // INSTRUCTION RM: READ from SYS_MEM -------------------
            printf("\nInstruction           : RM");
            printf("\nReading data from memory...");
            MAR = operand;              // load the operand to MAR

            // ----- LOCAL control signals -----
            Fetch = 0;
            MEM = 1;                // MEMORY access
            IO = 0;

            // ----- BUS control signals (external Signals) -----
            CONTROL = inst_code;    // set CONTROL signals
            IOM = 1;                // SYS_MEM
            RW = 0;                 // READ
            OE = 1;                 // DATA ENABLE

            ADDR = MAR;         // load MAR to Address Bus
            MainMemory();       // read data from memory to data bus
            if(MEM) MBR = BUS;  // STORE data in BUS to MBR

            printf("\nMBR                   : 0x%02X", MBR);
        }else if(inst_code==0x03){      // INSTRUCTION BR: BRANCH to INST ----------------------
            printf("\nInstruction           : BR");
            printf("\nBranch to 0x%03X on next cycle.", operand);
            PC = operand;               // update PC to point to new INST
        }else if(inst_code==0x04){      // INSTRUCTION RIO: READ from IO_BUFF -------------------
            printf("\nInstruction           : RIO");
            printf("\nReading data from IO...");
            IOAR = operand;             // load the operand to IOAR

            // ----- LOCAL control signals -----
            Fetch = 0;
            MEM = 0; 
            IO = 1;              // IO access

            // ----- BUS control signals (external Signals) -----
            CONTROL = inst_code;    // set CONTROL signals
            IOM = 0;                // IO MEM
            RW = 0;                 // READ
            OE = 1;                 // DATA ENABLE

            ADDR = IOAR;        // load IOAR to Address Bus
            IOMemory();         // read data from memory to data bus
            if(IO) IOBR = BUS;  // STORE data in BUS to IOBR

            //IOBR = IO_BUFF[IOAR];       // write data from SYS_MEM pointed to by IOAR to IOBR
            printf("\nIOBR                  : 0x%02X", IOBR);
        }else if(inst_code==0x05){      // INSTRUCTION WIO: WRITE to IO_BUFF --------------------
            printf("\nInstruction           : WIO");
            printf("\nWriting to IO buffer...");
            IOAR = operand;             // load the operand to IOAR      !!! IOAR should be 5 bits.

            // ----- LOCAL control signals -----
            Fetch = 0;
            MEM = 0; 
            IO = 1;             // IO access

            // ----- BUS control signals (external Signals) -----
            CONTROL = inst_code;    // set CONTROL signals
            IOM = 0;                // IO MEM
            RW = 1;                 // WRITE
            OE = 1;                 // DATA ENABLE

            ADDR = IOAR;        // load IOAR to Address Bus
            if(IO) BUS = IOBR;  // STORE IOBR to BUS
            IOMemory();         // write data from data bus to IO Memory

            //IO_BUFF[IOAR] = IOBR;       // write data from IOBR to SYS_MEM pointed to by IOAR
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

void MainMemory(void){
    if(IOM==1){
        if(RW==0 && OE==1)      BUS = SYS_MEM[ADDR];// memory read
        else if(RW==1 && OE==1) SYS_MEM[ADDR] = BUS;// memory write
    }
}

void IOMemory(void){
    if(IOM==0){
        if(RW==0 && OE==1)      BUS = IO_BUFF[ADDR];// IO read
        else if(RW==1 && OE==1) IO_BUFF[ADDR] = BUS;// IO write
    }
}