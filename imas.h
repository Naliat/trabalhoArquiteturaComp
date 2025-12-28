#ifndef IMAS_H
#define IMAS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define IMAS_MEM_SIZE 4096

#define IMAS_HALT    0x0
#define IMAS_LOAD_M  0x1
#define IMAS_LOAD_MQ 0x2
#define IMAS_LOAD_MQM 0x3
#define IMAS_STOR_M  0x4
#define IMAS_STA_M   0x5
#define IMAS_ADD_M   0x6
#define IMAS_SUB_M   0x7
#define IMAS_MUL_M   0x8
#define IMAS_DIV_M   0x9
#define IMAS_JMP_M   0xA
#define IMAS_JZ_M    0xB
#define IMAS_JNZ_M   0xC
#define IMAS_JPOS_M  0xD
#define IMAS_IN      0xE
#define IMAS_OUT     0xF

typedef struct imas_t {
    uint16_t pc;
    uint16_t mar;
    uint16_t ibr;
    uint16_t ir;

    int16_t mbr;
    int16_t ac;
    int16_t mq;

    uint16_t memory[IMAS_MEM_SIZE];
} imas_t;

#endif
