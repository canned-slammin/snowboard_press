/* 8/6/2025 */
#include "main.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*TODO modbus_read_float*/
/*

"By default the low order word contains the two low bytes of the
32-bit parameter"

example: Analog Input is represented by 360 (low order bytes) and 361 (high order bytes)

  uint16_t high_order = 0x42c5;
  uint16_t low_order = 0x8000;
  uint32_t temp = low_order | (high_order << 16) ;
  float f = *((float*)&temp);

modbus_reg_read 2 registers starting at first address, will put [low order, high order] into buffer



*/

static int modbus_reg_read_fp(const struct shell *sh, size_t argc, char **argv) {

    int err = 0;
    const uint8_t unit_id = atoi(argv[1]);
    const uint16_t start_addr = atoi(argv[2]);
    float reg_val;

    err = modbus_read_holding_regs_fp(client_iface, unit_id, start_addr, &reg_val, 1);

    if (err != 0) {
        shell_print(sh, "ERROR: FC03 failed to read floating point register at address %d from unit %d", start_addr, unit_id);
        shell_print(sh, "Error code: %d", err);
    }
    else {
        shell_print(sh, "Unit ID = %d", unit_id);
        shell_print(sh, "Address = %d", start_addr);
        shell_print(sh, "Reg val = %f", reg_val);
        shell_print(sh, "Reg val in mem = %x, reg_val");
    }

    return err;

}

/*modbus write holding regs with error reporting*/

static int modbus_reg_write(const struct shell *sh, size_t argc, char **argv)
{

    int err = 0;
    const uint8_t unit_id = atoi(argv[1]);
    const uint16_t start_addr = atoi(argv[2]);
    const uint16_t reg_val = atoi(argv[3]);

    err = modbus_write_holding_reg(client_iface, unit_id, start_addr, reg_val);

    if (err != 0) {
        shell_print(sh, "ERROR: FC06 failed to write value %d to holding register %d on unit %d", reg_val, start_addr, unit_id);
        shell_print(sh, "Error code: %d", err);
    }
    else {
        shell_print(sh, "FC06 Wrote %d to register %d on unit %d", reg_val, start_addr, unit_id);
    }

    return err;

}

/*modbus read holding regs with error reporting*/

static int modbus_reg_read(const struct shell *sh, size_t argc, char **argv)
{
    
    int err = 0;
    const uint8_t unit_id = atoi(argv[1]);
    const uint16_t start_addr = atoi(argv[2]);
    uint16_t reg_val = 0;

    err = modbus_read_holding_regs(client_iface, unit_id, start_addr, &reg_val, 1);

    if (err != 0) {
        shell_print(sh, "ERROR: FC03 failed to read register at address %d from unit %d", start_addr, unit_id);
        shell_print(sh, "Error code: %d", err);
    }
    else {
        shell_print(sh, "Unit ID = %d", unit_id);
        shell_print(sh, "Address = %d", start_addr);
        shell_print(sh, "Reg val = %d", reg_val);
    }

    return err;

}

/*TODO modbus write coils with error reporting*/

static int modbus_coil_write(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Write Coils not yet implemented");
    return -1;
}

/*TODO modbus read coils with error reporting*/

static int modbus_coil_read(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Read Coils not yet implemented");
    return -1;
}

/*TODO create subcommands (level 1 command) array for command "modbus"*/

/*

shell static subcommand create:
- sub_modbus
- shell command args: write_reg
- shell command args: read_reg
- shell command args: write_coil
- shell command args: read_coli
- shell subcommand set end

*/

SHELL_STATIC_SUBCMD_SET_CREATE(sub_modbus,
                               SHELL_CMD_ARG(write_reg,
                                             NULL,
                                             "<unit_id> <start_addr> <reg_val> - Write holding register",
                                            modbus_reg_write, 3, 3),
                               SHELL_CMD_ARG(read_reg,
                                             NULL,
                                             "<unit_id> <start_addr> - Read holding register",
                                            modbus_reg_read, 2, 2),
                               SHELL_CMD_ARG(write_coil,
                                             NULL,
                                             "Write coil state",
                                            modbus_coil_write, 3, 3),
                               SHELL_CMD_ARG(read_coil,
                                             NULL,
                                             "Read coil state",
                                            modbus_coil_read, 2, 2),
                               SHELL_CMD_ARG(read_reg_fp,
                                             NULL,
                                             "<unit_id> <start_addr> - Read floating point holding register",
                                            modbus_reg_read_fp, 2, 2),
                               SHELL_SUBCMD_SET_END);

/*TODO create root (level 0) command "modbus"

shell command register:
- modbus
- &sub_modbus
- "Modbus commands"
- handler = NULL (all done by subcommand)

*/

SHELL_CMD_REGISTER(modbus, &sub_modbus, "Modbus commands", NULL);