# MIDI Keyboard Requirements

| Requirement | MoSCoW | Notes | Fulfilled By |
| :---------- | :----- |:----- | :----------- |
| 61 separate MIDI notes must be handled by keys | MUST | Driven by number of keys in keyboard chassis. Upgrade on EMBSYS320 project |midi_handler.c
| Touchscreen must handle 4 different drum pads | MUST | Carried over from EMBSYS320 | NOT MET
| Touchscreen must be able to change instruments | MUST | Upgrade on EMBSYS320 | menus.c
| User must be able to select different MIDI effects | MUST | Upgrade on EMBSYS320 |smf_console_cmds.c
| Key latency must not be higher than EMBSYS320 project | MUST | EMBSYS320 project had acceptable latency and will be used a standard to measure key latency against. |NOT MEASURED
| GUI module architecture should allow for expansion | SHOULD | For easier iteration on the project, the process of adding a menu in the GUI should be straightforward and not involve large scale refactoring of the code. |menus.c
| System could integrate pitch adjustment via a GUI menu | COULD | This is an option available on the VS1053, but it's not certain that time will allow for implementation |NOT MET
| System could integrate different keymapping via a GUI menu | COULD | This would be a fun and relatively unique feature, but it's not certain that time will allow for implementation |NOT MET
| System will not offer playback or looping | WON'T | This would be a really cool feature, and it's possible, but out of scope of this project. | N/A