#include <stdio.h>
#include "control.h"
#include "isa.h"
#include "register_file.h"
#include "util.h"
#include "memory.h"
#include "cpu.h"


/**
 *
 * @param cpu
 */
void start(cpu_t *cpu)
{
    cpu->running = true;

    while (cpu->running)
    {
        inst_cycle(cpu);
    }
}


/**
 *
 * @param cpu
 */
void inst_cycle(cpu_t *cpu)
{
    address_t
        inst_addr = read_register_wide(cpu, R_PX, R_PY),
        imm_16    = 0;

    data_t
        inst  = mem_read(cpu, pc_register_wide_incr(cpu)),
        imm_0 = 0,
        imm_1 = 0;

    switch (inst)
    {
        case I_CLAC:
            cpu->register_file[R_AC] = 0;
            break;
        case I_ADDI:
            imm_0 = mem_read(cpu, pc_register_wide_incr(cpu));
            cpu->register_file[R_AC] += imm_0;
            break;
        case I_SUBI:
            imm_0 = mem_read(cpu, pc_register_wide_incr(cpu));
            cpu->register_file[R_AC] -= imm_0;
            break;
        case I_ADD:
            imm_0  = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_1  = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_16 = imm_0 + imm_1 * (u_int16_t) 0x100;
            cpu->register_file[R_AC] += mem_read(cpu, imm_16);
            break;
        case I_SUB:
            imm_0  = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_1  = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_16 = imm_0 + imm_1 * (u_int16_t) 0x100;
            cpu->register_file[R_AC] -= mem_read(cpu, imm_16);
            break;
        case I_STOR:
            imm_0  = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_1  = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_16 = imm_0 + imm_1 * (u_int16_t) 0x100;
            mem_write(cpu, cpu->register_file[R_AC], imm_16);
            break;
        case I_BEQZ:
            imm_0  = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_1  = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_16 = imm_0 + imm_1 * (u_int16_t) 0x100;
            if (cpu->register_file[R_AC] == 0)
            {
                write_register_wide(cpu, imm_16, R_PX, R_PY);
            }
            break;
        case I_CSET:
            imm_0 = mem_read(cpu, pc_register_wide_incr(cpu));
            if (c0_bit(cpu, S_MODE) == 0) // privileged mode
            {
                printf("cset works in privileged mode!\n");
                cpu->register_file[R_C0] |= imm_0;
            }
            else
            {
                printf("exception: illegal instruction\n");
                cpu->running = false;
                // exception, callback
            }
            break;
        case I_VSET:
            imm_0 = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_1 = mem_read(cpu, pc_register_wide_incr(cpu));
            imm_16 = imm_0 + imm_1 * (u_int16_t) 0x100;
            if (c0_bit(cpu, S_MODE) == 0) // privileged mode
            {
                printf("vset works in privileged mode!\n");
                write_register_wide(cpu, imm_16, R_VX, R_VY);
                cpu->register_file[R_C0] |= imm_0;
            }
            else
            {
                printf("exception: illegal instruction\n");
                cpu->running = false;
                // exception, callback
            }
            break;
        case I_HALT:
            cpu->running = false;
            break;
        case I_NOOP:
        default:
            break;
    }

    printf("- cycle %d\n", cpu->inst_cycle_count);
    printf("\tinst:           %02x\n", inst);
    printf("\timm_16:       %02x%02x\n", imm_1, imm_0);
    printf("\tinst_addr:    %04x\n", inst_addr);
    print_register_file(cpu);
    print_memory(cpu);
    printf("\n");

    cpu->inst_cycle_count++;
}
