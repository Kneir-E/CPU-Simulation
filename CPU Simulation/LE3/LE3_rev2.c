#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


int CU();
void initMemory();
void displayData(unsigned int PC, unsigned int MAR, unsigned int MBR, unsigned int IOAR, unsigned int IOBR);
void mainMemory();

unsigned char ioBuffer[32];
unsigned char dataMemory[2048];


unsigned int CONTROL = 0;
unsigned int IOM     = 0;
unsigned int RW      = 0;    
unsigned int OE      = 0;


unsigned int addressBus;
unsigned int dataBus;


void IOMemory() {

    if (IOM == 0) {
        if (RW == 0 && OE == 1) {               // IO read
            dataBus = ioBuffer[addressBus];

        }else if(RW == 1 && OE == 1) {          // IO write
            ioBuffer[addressBus] = dataBus;
        }
    }

}

void mainMemory() {

    if (IOM == 1) {
        if (RW == 0 && OE == 1) {               // Memory read
            dataBus = dataMemory[addressBus];  

        }else if(RW == 1 && OE == 1) {          // Memory write
            dataMemory[addressBus] = dataBus;
        }
    }
    
}

int CU() {

    unsigned int PC=0, IR=0, MAR=0, MBR=0, IOAR=0, IOBR=0, inst_code=0, operand=0;
    int result = 0,i;

    // Control Lines
    unsigned int fetch;
    unsigned int INT_IO;
    unsigned int MEMORY;

    for (i=0; i<2048; i++) {

        printf("PC \t\t\t: 0x%03x \n", PC);

        //fetching

        CONTROL = inst_code;   
        IOM     = 1;            
        RW      = 0;           
        OE      = 1;           

        fetch   = 1;
        INT_IO      = 0;
        MEMORY  = 0;

        addressBus = PC;
        mainMemory();

        if (fetch == 1) {

            IR = (int) dataBus;
            IR = IR << 8;
            PC++;
            addressBus = PC;

        }

        mainMemory();

        if (fetch == 1) {

            IR = IR | dataBus;
            PC++;   
        }

        printf("IR  \t\t\t: 0x%04x \n", IR);

        inst_code = IR>>11;
        operand = IR & 0x07FF;

        printf("Instruction Code\t: 0x%02x\n", inst_code);
        printf("Operand \t\t: 0x%03x \n", operand);
        printf("\n--------------------------------------------------\n\n");

        if (inst_code == 0x01) {

            MAR = operand;

            fetch   = 0;
            INT_IO      = 0;
            MEMORY  = 1;

            
            CONTROL = inst_code;
            IOM     = 1;
            RW      = 1;
            OE      = 1;

            addressBus = MAR; 
            if (MEMORY)
                dataBus = MBR; 
            
            mainMemory();

            printf("INSTRUCTION \t\t: WM \n");
            printf("\nStoring data in memory...\n\n");

            displayData(PC,MAR,MBR,IOAR,IOBR);
        }else if(inst_code==0x02) {

            MAR = operand;
           
            fetch   = 0;
            INT_IO      = 0;
            MEMORY  = 1;

            CONTROL = inst_code;
            IOM     = 1;
            RW      = 0;
            OE      = 1;

            addressBus = MAR; 
            if (MEMORY)
                MBR = dataBus; 
            
            mainMemory();

            printf("INSTRUCTION \t\t: RM \n");
            printf("\nReading data from memory...\n\n");
            printf("MBR \t\t\t: 0x%o2x \n", MBR);
            displayData(PC,MAR,MBR,IOAR,IOBR);

        }else if (inst_code==0x03) {

            PC = operand;
            printf("INSTRUCTION \t\t: BR \n");
            printf("\nTransitioning to 0x%03x for the next cycle...\n\n", PC);
            displayData(PC,MAR,MBR,IOAR,IOBR);

        }else if (inst_code==0x04) {
            
            IOAR = operand;
             
            fetch   = 0;
            INT_IO      = 1;
            MEMORY  = 0;

            CONTROL = inst_code;
            IOM     = 0;
            RW      = 1;
            OE      = 1;

            addressBus = IOAR; 
            if(INT_IO) {
                IOBR = dataBus; 
            }

            IOMemory();

            printf("INSTRUCTION \t: RIO \n");
            printf("\nFetching information from the I/O buffer...\n\n");
            printf("IOBR \t\t: 0x%02x \n", IOBR);
            displayData(PC,MAR,MBR,IOAR,IOBR);

        }else if(inst_code==0x05) {

            IOAR = operand;
             
            fetch   = 0;
            INT_IO      = 1;
            MEMORY  = 0;
          
            CONTROL = inst_code;
            IOM     = 0;
            RW      = 0;
            OE      = 1;

            addressBus = IOAR; 
            if (INT_IO) {

                dataBus = IOBR; 

            }
            IOMemory();

            printf("INSTRUCTION \t\t: WIO \n");
            printf("\nWriting to IO buffer...\n\n");
            displayData(PC,MAR,MBR,IOAR,IOBR);

        }else if(inst_code==0x06){

            MBR = operand;
            printf("INSTRUCTION \t\t: WB \n");
            printf("\nLoading data to MBR...\n\n");
            displayData(PC,MAR,MBR,IOAR,IOBR);

        }else if(inst_code==0x07){

            IOBR = operand;
            printf("INSTRUCTION \t\t: WIB \n");
            printf("\nLoading data to IOBR...\n\n");
           
            displayData(PC,MAR,MBR,IOAR,IOBR);

        }else if(inst_code==0x1F){

            result = 1;
            printf("INSTRUCTION \t\t: EOP \n");
            printf("\nEXECUTION FINSISHED\n\n");
            displayData(PC,MAR,MBR,IOAR,IOBR);
            break;
        }
    }
    return result;
}

void displayData(unsigned int PC, unsigned int MAR, unsigned int MBR, unsigned int IOAR, unsigned int IOBR) {
    printf("Address Bus \t\t: 0x%03x \n", addressBus);
    printf("Control \t\t: 0x%03x \n", CONTROL);
    printf("Data Bus \t\t: 0x%03x \n", dataBus);
    printf("MBR \t\t\t: 0x%02x \n", MBR);
    printf("MAR \t\t\t: 0x%03x \n", MAR);
    printf("PC \t\t\t: 0x%02x\n", PC);

    printf("IOAR \t\t: 0x%02x \n", IOAR);
    printf("IOBR \t\t: 0x%02x \n", IOBR);
}


void initMemory() {

    printf("Main Memory is Initializing...\n\n");

    dataMemory[0x000] = 0x30;
    dataMemory[0x001] = 0xFF;
    dataMemory[0x002] = 0x0C;
    dataMemory[0x003] = 0x00;
    dataMemory[0x004] = 0x14;
    dataMemory[0x005] = 0x00;
    dataMemory[0x006] = 0x19;
    dataMemory[0x007] = 0x2A;
    dataMemory[0x12A] = 0x38;
    dataMemory[0x12B] = 0x05;
    dataMemory[0x12C] = 0x28;
    dataMemory[0x12D] = 0x0A;
    dataMemory[0x12E] = 0xF8;
    dataMemory[0x12F] = 0x00;
}

int main() {
    initMemory();
    if (CU()==1) {

        printf("\nPROGRAM EXECUTED");

    } else {

        printf("\nPROGRAM TERMINATED");

    }
    return 0;
}
