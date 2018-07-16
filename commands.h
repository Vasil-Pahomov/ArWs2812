#define COMMAND_LENGTH 5 // total length of a command packet (in bytes)
#define COMMAND_MARKER 255 //byte marking the command is going on
#define CMD_SETAP '=' //set animation and palette
#define CMD_MAGIC '!' //enter magic mode
#define CMD_MPOS 'P' //data in magic mode

/*command structure:
byte 0 - command marker
byte 1 - command code
byte 2 - command data 1
byte 3 - command data 2
byte 4 - checksum (code+data1+data2 over a byte)
*/
