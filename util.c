//
// Created by Miles on 7/3/18.
//

#include <memory.h>
#include "util.h"


/**
 *
 * @param filepath
 * @param cpu
 */
void load_mem(cpu_t *cpu, const char *filepath)
{
    int byte;

    FILE *file;

    file = fopen(filepath, "r");

    printf("loading memory from file: %s\n", filepath);

    if (file)
    {
        address_t i = 0;

        memset(cpu->physical_memory, 0, PHYSICAL_MEMORY_BYTES);

        while ((byte = getc(file)) != EOF)
        {
            cpu->physical_memory[i++] = (data_t) byte;
        }
        fclose(file);
    }
    else
    {
        printf("error: unable to read file\n");
    }
}

/**
 *
 * @param n
 * @param k
 * @param f
 */
void print_file(cpu_t *cpu, int m, int n, int k, data_t *f, bool highlight_addr, bool lichten)
{
    for (int i = m; i < n; i++)
    {
        if (!((i) % k))
        {
            printf("\t%04x   ", i);
        }

        if (highlight_addr && (i == cpu->_last_mem_write_addr)) printf(KYEL);
        if (highlight_addr && (i == cpu->_last_mem_read_addr)) printf(KCYN);
        if (highlight_addr && (i == cpu->_last_inst_addr)) printf(KGRN);

        if(lichten)
            print_byte(f[i]);
        else
            printf("%02x ", f[i]);

        if (highlight_addr && (i == cpu->_last_mem_write_addr))
        {
            printf(KNRM);
            cpu->_last_mem_write_addr = -1;
        }
        if (highlight_addr && (i == cpu->_last_mem_read_addr))
        {
            printf(KNRM);
            cpu->_last_mem_read_addr = -1;
        }
        if (highlight_addr && (i == cpu->_last_inst_addr))
        { printf(KNRM); }


        if (!((i + 1) % k) || i == n - 1)
        {
            printf("\n");
        }
    }
}


/**
 *
 * @param cpu
 */
void print_memory(cpu_t *cpu)
{
    int dsp = 0x80;
    printf("\n\tphysical memory\n");
    print_file(cpu, 0, dsp, 0x10, cpu->physical_memory, true, LICHTEN_DISPLAY);
    printf("\t...\n");
    print_file(cpu, PHYSICAL_MEMORY_BYTES - dsp, PHYSICAL_MEMORY_BYTES, 0x10, cpu->physical_memory, 0, LICHTEN_DISPLAY);
}


void print_register_file(cpu_t *cpu)
{
    printf("\n\tregister file\n");
    print_file(cpu, 0, REGISTER_FILE_BYTES, 8, cpu->register_file, false, LICHTEN_DISPLAY);
}

void print_byte(u_int8_t byte)
{
    u_int8_t bit = 0x80;
    printf("██");
    printf(KNRM);
    for (int i = 0; i < 8; i++)
    {
        if (bit & byte)
        {
            printf(KRED);
            printf("█");
        }
        else
        {
            printf("░");
        }
        printf(KNRM);

        bit = bit >> 1;
    }
}
