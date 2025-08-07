/* 8/6/2025 */
#include "main.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*TODO modbus write holding regs with error reporting*/

static int modbus_reg_write(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Write Registers not yet implemented");
	shell_print(sh, "client interface = %d", client_iface);
    return -1;
}

/*TODO modbus read holding regs with error reporting*/

static int modbus_reg_read(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Read Registers not yet implemented");
    return -1;
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
                                             "Write holding registers",
                                            modbus_reg_write, 3, 3),
                               SHELL_CMD_ARG(read_reg,
                                             NULL,
                                             "Read holding registers",
                                            modbus_reg_read, 3, 3),
                               SHELL_CMD_ARG(write_coil,
                                             NULL,
                                             "Write coil state",
                                            modbus_coil_write, 3, 3),
                               SHELL_CMD_ARG(read_coil,
                                             NULL,
                                             "Read coil state",
                                            modbus_coil_read, 2, 2),
                               SHELL_SUBCMD_SET_END);

/*TODO create root (level 0) command "modbus"

shell command register:
- modbus
- &sub_modbus
- "Modbus commands"
- handler = NULL (all done by subcommand)

*/

SHELL_CMD_REGISTER(modbus, &sub_modbus, "Modbus commands", NULL);