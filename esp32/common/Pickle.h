#ifndef __PICKLE_H__
#define __PICKLE_H__

#include <stdint.h>
#include <Arduino.h>
#include <iostream>

#define MARK   		   '('   // push special markobject on stack
#define STOP           '.'   // every pickle ends with STOP
#define POP            '0'   // discard topmost stack item
#define POP_MARK       '1'   // discard stack top through topmost markobject
#define DUP            '2'   // duplicate top stack item
#define FLOAT          'F'   // push float object; decimal string argument
#define INT            'I'   // push integer or bool; decimal string argument
#define BININT         'J'   // push four-byte signed int
#define BININT1        'K'   // push 1-byte unsigned int
#define LONG           'L'   // push long; decimal string argument
#define BININT2        'M'   // push 2-byte unsigned int
#define NONE           'N'   // push None
#define PERSID         'P'   // push persistent object; id is taken from string arg
#define BINPERSID      'Q'   //  "       "         "  ;  "  "   "     "  stack
#define REDUCE         'R'   // apply callable to argtuple, both on stack
#define STRING         'S'   // push string; NL-terminated string argument
#define BINSTRING      'T'   // push string; counted binary string argument
#define SHORT_BINSTRING'U'   //  "     "   ;    "      "       "      " < 256 bytes
#define UNICODE        'V'   // push Unicode string; raw-unicode-escaped'd argument
#define BINUNICODE     'X'   //   "     "       "  ; counted UTF-8 string argument
#define APPEND         'a'   // append stack top to list below it
#define BUILD          'b'   // call __setstate__ or __dict__.update()
#define GLOBAL         'c'   // push self.find_class(modname, name); 2 string args
#define DICT           'd'   // build a dict from stack items
#define EMPTY_DICT     '}'   // push empty dict
#define APPENDS        'e'   // extend list on stack by topmost stack slice
#define GET            'g'   // push item from memo on stack; index is string arg
#define BINGET         'h'   //   "    "    "    "   "   "  ;   "    " 1-byte arg
#define INST           'i'   // build & push class instance
#define LONG_BINGET    'j'   // push item from memo on stack; index is 4-byte arg
#define LIST           'l'   // build list from topmost stack items
#define EMPTY_LIST     ']'   // push empty list
#define OBJ            'o'   // build & push class instance
#define PUT            'p'   // store stack top in memo; index is string arg
#define BINPUT         'q'   //   "     "    "   "   " ;   "    " 1-byte arg
#define LONG_BINPUT    'r'   //   "     "    "   "   " ;   "    " 4-byte arg
#define SETITEM        's'   // add key+value pair to dict
#define TUPLE          't'   // build tuple from topmost stack items
#define EMPTY_TUPLE    ')'   // push empty tuple
#define SETITEMS       'u'   // modify dict by adding topmost key+value pairs
#define BINFLOAT       'G'   // push float; arg is 8-byte float encoding

#define TRUE           'I01\n'  // not an opcode; see INT docs in pickletools.py
#define FALSE          'I00\n'  // not an opcode; see INT docs in pickletools.py

// Protocol 2

#define PROTO          0x80  // identify pickle protocol
#define NEWOBJ         0x81  // build object by applying cls.__new__ to argtuple
#define EXT1           0x82  // push object from extension registry; 1-byte index
#define EXT2           0x83  // ditto, but 2-byte index
#define EXT4           0x84  // ditto, but 4-byte index
#define TUPLE1         0x85  // build 1-tuple from stack top
#define TUPLE2         0x86  // build 2-tuple from two topmost stack items
#define TUPLE3         0x87  // build 3-tuple from three topmost stack items
#define NEWTRUE        0x88  // push True
#define NEWFALSE       0x89  // push False
#define LONG1          0x8a  // push long from < 256 bytes
#define LONG4          0x8b  // push really big lon
//_tuplesize2code = [EMPTY_TUPLE, TUPLE1, TUPLE2, TUPLE3]

// Protocol 3 (Python 3.x)

#define BINBYTES       'B'   //  push bytes; counted binary string argument
#define SHORT_BINBYTES 'C'   //  "     "   ;    "      "       "      " < 256 bytes

// Protocol 4

#define SHORT_BINUNICODE 0x8c  // push short string; UTF-8 length < 256 bytes
#define BINUNICODE8      0x8d  // push very long string
#define BINBYTES8        0x8e  // push very long bytes string
#define EMPTY_SET        0x8f  // push empty set on the stack
#define ADDITEMS         0x90  // modify set by adding topmost stack items
#define FROZENSET        0x91  // build frozenset from topmost stack items
#define NEWOBJ_EX        0x92  // like NEWOBJ but work with keyword only arguments
#define STACK_GLOBAL     0x93  // same as GLOBAL but using names on the stacks
#define MEMOIZE          0x94  // store top of the stack in memo
#define FRAME            0x95  // indicate the beginning of a new frame


#define DEFAULT_PROTOCOL 0X03
#define HIGHEST_PROTOCOL 0X04

#define MAX_OPCODE 		 (8)

#define MAX_BUFFER_SIZE  64


class Pickler
{
	public:
		Pickler(uint8_t* current_frame);

		void start_frame();
		void end_frame();

		template<typename T> void dump(T var);

	protected:
		uint8_t * current_frame;;
		uint32_t  ptr;
		int num;
};

class UnPickler
{
	public:

		UnPickler(uint8_t* current_frame);

		void remove_start_frame();
		void remove_end_frame();
		void remove_tuple_header();

		template<typename T> T load();

		bool is_tuple();

	protected:
		uint8_t * current_frame;
		uint32_t  ptr = 0;

};


#endif // __PICKLE_H__
