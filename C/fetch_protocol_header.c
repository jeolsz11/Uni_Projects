/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Fetch protocol: download a file from a fetch server
 * J.A. Jerkins <jajerkins@una.edu>, 03/16/2021
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#define UDP_PORT 1514

enum OPCODE {
     OP_INVALID,
     OP_GET,
     OP_DAT,
     OP_ACK,
     OP_ERR
};

static const int OPCODE_LEN  = sizeof(unsigned short),
                  SEQNUM_LEN  = sizeof(unsigned short);

enum OPCODE     getOpCode(const char*);
char*           getFilename(const char*);
unsigned short  getBlockNum(const char*);
char*           getDataBlock(const char*, size_t);
char*           getErrorText(const char*);

size_t          mkGet(char**, const char*);
size_t          mkDat(char**, unsigned short, const char*, size_t);
size_t          mkAck(char**, unsigned short);
size_t          mkErr(char**, const char*);

#endif /* PROTOCOL_H */
