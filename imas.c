#include "imas.h"

void memory_read(imas_t *imas) {
    imas->mbr = (int16_t)imas->memory[imas->mar & 0xFFF]; 
}

void memory_write(imas_t *imas, bool modify_address) {
    uint16_t addr = imas->mar & 0xFFF;
    if (modify_address) { 
        uint16_t original = imas->memory[addr];
        uint16_t novo_addr = (uint16_t)imas->ac & 0x0FFF;
        imas->memory[addr] = (original & 0xF000) | novo_addr;
    } else { 
        imas->memory[addr] = (uint16_t)imas->mbr;
    }
}

void io_read(imas_t *imas) {
    printf("IN => ");
    scanf("%hd", &imas->ac);
}

void io_write(imas_t *imas) {
    printf("OUT => %hd\n", imas->ac);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.txt> [breakpoints...]\n", argv[0]);
        return 1;
    }
    
    imas_t imas = {0};

    FILE *f = fopen(argv[1], "r");
    if (!f) return 1;
    uint32_t a, v;
    while (fscanf(f, "%x %x%*[^\n]", &a, &v) == 2) {
        imas.memory[a & 0xFFF] = (uint16_t)v;
    }
    fclose(f);

    while (true) {
        uint16_t pc_atual = imas.pc;

        imas.mar = imas.pc;
        memory_read(&imas);
        imas.pc++;
        imas.ibr = (uint16_t)imas.mbr;

        imas.ir = (imas.ibr & 0xF000) >> 12;
        imas.mar = (imas.ibr & 0x0FFF);

        switch (imas.ir) {
            case IMAS_HALT: return 0; 
            case IMAS_LOAD_M: memory_read(&imas); imas.ac = imas.mbr; break;
            case IMAS_LOAD_MQ: imas.ac = imas.mq; break;
            case IMAS_LOAD_MQM: memory_read(&imas); imas.mq = imas.mbr; break;
            case IMAS_STOR_M: imas.mbr = imas.ac; memory_write(&imas, false); break;
            case IMAS_STA_M: memory_write(&imas, true); break;
            case IMAS_ADD_M: memory_read(&imas); imas.ac += imas.mbr; break;
            case IMAS_SUB_M: memory_read(&imas); imas.ac -= imas.mbr; break;
            
            case IMAS_MUL_M: 
                memory_read(&imas);
                int32_t prod = (int32_t)imas.mq * (int32_t)imas.mbr;
                imas.ac = (int16_t)(prod >> 16);
                imas.mq = (int16_t)(prod & 0xFFFF);
                break;
                
            case IMAS_DIV_M: 
                memory_read(&imas);
                if (imas.mbr != 0) {
                    int16_t dividendo = imas.mq;
                    imas.mq = dividendo / imas.mbr;
                    imas.ac = dividendo % imas.mbr;
                }
                break;

            case IMAS_JMP_M: imas.pc = imas.mar; break;
            case IMAS_JZ_M: if (imas.ac == 0) imas.pc = imas.mar; break;
            case IMAS_JNZ_M: if (imas.ac != 0) imas.pc = imas.mar; break;
            case IMAS_JPOS_M: if (imas.ac >= 0) imas.pc = imas.mar; break;
            case IMAS_IN: io_read(&imas); break;
            case IMAS_OUT: io_write(&imas); break;
            default: break;
        }

        for (int i = 2; i < argc; i++) {
            uint16_t bp = (uint16_t)strtol(argv[i], NULL, 16);
            if (pc_atual == bp) {
                printf("IMAS Registers\n");
                printf("PC=0x%04X\n", pc_atual);
                printf("PC+=0x%04X\n", imas.pc);
                printf("MAR=0x%04X\n", imas.mar);
                printf("IBR=0x%04X\n", imas.ibr);
                printf("IR=0x%04X\n", imas.ir);
                printf("MBR=0x%04X\n", (uint16_t)imas.mbr);
                printf("AC=0x%04X\n", (uint16_t)imas.ac);
                printf("MQ=0x%04X\n", (uint16_t)imas.mq);
                printf("--------------------\n");
            }
        }
    }
    return 0;
}
