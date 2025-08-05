#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/modbus/modbus.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*TODO how to get the client interface shared from main?*/

/*TODO modbus write holding regs with error reporting*/

static int modbus_write_reg(const struct shell *sh, size_t argc, char **argv)
{
    shell_print("Write Registers not yet implemented");
    return -1;
}

/*TODO modbus read holding regs with error reporting*/

static int modbus_read_reg(const struct shell *sh, size_t argc, char **argv)
{
    shell_print("Read Registers not yet implemented");
    return -1;
}

/*TODO modbus write coils with error reporting*/

static int modbus_write_coil(const struct shell *sh, size_t argc, char **argv)
{
    shell_print("Write Coils not yet implemented");
    return -1;
}

/*TODO modbus read coils with error reporting*/

static int modbus_read_coil(const struct shell *sh, size_t argc, char **argv)
{
    shell_print("Read Coils not yet implemented");
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
                                             "Write holding registers",
                                            modbus_write_reg, 3, 3),
                               SHELL_CMD_ARG(read_reg,
                                             NULL,
                                             "Read holding registers",
                                            modbus_read_reg, 3, 3),
                               SHELL_CMD_ARG(write_coil,
                                             NULL,
                                             "Write coil state",
                                            modbus_write_coil, 3, 3),
                               SHELL_CMD_ARG(read_coil,
                                             NULL,
                                             "Read coil state",
                                            modbus_read_coil, 2, 2),)

/*TODO create root (level 0) command "modbus"

shell command register:
- modbus
- &sub_modbus
- "Modbus commands"
- handler = NULL (all done by subcommand)

*/

SHELL_CMD_REGISTER(modbus, &sub_modbus, "Modbus commands", NULL);