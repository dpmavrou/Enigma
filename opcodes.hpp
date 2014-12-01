#ifndef OPCODES_HPP
#define OPCODES_HPP

enum class opcodes : char
{
   get,
   put
};

// ADD TO THIS
enum class errors : char
{
   noError = 0x00,
   badOpcode,
   serverError = 0xff
};

#endif