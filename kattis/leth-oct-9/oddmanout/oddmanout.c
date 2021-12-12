#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

// ltconst.c /////////////////////////////////////////////////////////////////
typedef short WORD;
typedef unsigned short ADDRESS;
typedef ADDRESS WORDU;
typedef int DWORD;
typedef unsigned int DWORDU;

#ifdef TEST
  const bool TESTING = true;
#else
  const bool TESTING = false;
#endif
const char* TEST_FILE = "test.vm";

#ifdef DEBUG
  const bool DEBUGGING = true;
#else
  const bool DEBUGGING = false;
#endif

// Sizes for the various memorys
const ADDRESS END_MEMORY = 0xffff;
const ADDRESS END_RETURN = 0xf000;
const ADDRESS END_STACK = 0xf000;

const WORDU WORD_SIZE = 16;
const WORDU BYTE_SIZE = 8;
const WORDU BUFFER_SIZE = 0x0400;

const WORDU DEFAULT_SCALE = 3;
const WORDU SCALE_MAX = 10;
const DWORDU SCALES[ 10 ] = {
  1, 10, 100, 1000, 10000, 100000,
  1000000, 10000000, 100000000,
  1000000000
};

// Exit codes //
const size_t EXIT_MEM = 1;
const size_t EXIT_LEN = 2;
const size_t EXIT_FILE = 3;
const size_t EXIT_SOF = 4;
const size_t EXIT_SUF = 5;
const size_t EXIT_POB = 6;
const size_t EXIT_OP = 7;
const size_t EXIT_STR = 8;
const size_t EXIT_ARGS = 9;
const size_t EXIT_RSOF = 10;
const size_t EXIT_RSUF = 11;

// ltrun.h ///////////////////////////////////////////////////////////////////
typedef enum op_codes { HALT=0,
  PUSH, POP, LOAD, STORE,  // 04
  FST, SEC, NTH,  // 07
  SWAP, ROT,  // 09
  RPUSH, RPOP, RGRAB,  // 0C

  DPUSH, DPOP, DLOAD, DSTORE,  // 10
  DFST, DSEC, DNTH,  // 13
  DSWAP, DROT,  // 15
  DRPUSH, DRPOP, DRGRAB,  // 18

  ADD, SUB, MULT, DIV, MOD,  // 1D
  EQ, LT, GT,  // 20
  MULTU, DIVU, MODU, LTU, GTU,  // 25

  SL, SR,  // 27
  AND, OR, NOT,  // 2A

  DADD, DSUB, DMULT, DDIV, DMOD,  // 2F
  DEQ, DLT, DGT,  // 32
  DMULTU_unused, DDIVU, DMODU, DLTU, DGTU,  // 37

  DSL, DSR,  // 39
  DAND, DOR, DNOT,  // 3C

  JUMP, BRANCH, CALL, RET,  // 40
  DSP, PC, BFP, FMP,  // 44

  WPRN, DPRN, WPRNU, DPRNU, FPRN, FPRNSC,  // 4A
  PRNCH, PRN, PRNLN, PRNSP_unused, PRNMEM,  // 4F

  WREAD, DREAD, FREAD, FREADSC,  // 53
  READCH, READ_unused, READLN, READSP_unused,  // 57

  BFSTORE, BFLOAD,  // 59
  HIGH, LOW, UNPACK, PACK,  // 5D

  MEMCOPY, STRCOPY,  // 5F
  FMULT, FDIV, FMULTSC, FDIVSC,  // 63

  PRNPK, READCH_BF, STREQ, MEMEQ, // 67
  IS_EOF, RESET_EOF, // 69

  BRKPNT, // 6A
  OPEN, CLOSE, // 6C
} OP_CODE;

enum copy_codes { MEM_BUF = 0, BUF_MEM };

/// ltmem.h //////////////////////////////////////////////////////////////////
static inline DWORD get_dword(WORD* mem, ADDRESS pos) {
  return (mem[pos] << WORD_SIZE) | (mem[pos+1] & 0xffff);
}

static inline void set_dword(WORD* mem, ADDRESS pos, DWORD val) {
  mem[pos] = val >> WORD_SIZE;
  mem[pos+1] = (WORD)val;
}

static inline DWORD get_rev_dword(WORD* mem, ADDRESS pos) {
  return (mem[pos+1] << WORD_SIZE) | (mem[pos] & 0xffff);
}

static inline void set_rev_dword(WORD* mem, ADDRESS pos, DWORD val) {
  mem[pos+1] = val >> WORD_SIZE;
  mem[pos] = (WORD)val;
}

static inline WORDU string_length(WORD* mem, ADDRESS start) {
  ADDRESS atemp = start;
  while (atemp) {
    if (!mem[atemp]) break;
    atemp--;
  }
  return start - (atemp - 1);
}

inline static WORDU mem_equal(WORD* mem, ADDRESS first,
                              ADDRESS second, ADDRESS length) {
  for (ADDRESS i = 0; i < length; i++)
    if (mem[first + i] != mem[second + i])
      return 0;
  return 1;
}

/// ltio.c ///////////////////////////////////////////////////////////////////
void display_range(WORD* mem, ADDRESS start, ADDRESS end, bool debug) {
  if (debug && end - 8 > start) {
    start = end - 8; 
    fprintf(stderr, "... ");
  }

  for (ADDRESS i = start; i < end; i++) {
    if (debug)
      fprintf(stderr, "%hx(%hd) ", mem[i], mem[i]);
    else
      printf("%04hx ", mem[i]);
  }

  if (debug)
    fprintf(stderr, "->\n");
  else
    printf("\n");
}

void print_string(WORD* mem, ADDRESS start, ADDRESS max) {
  ADDRESS atemp = start;
  while (atemp < max) {
    WORD chars = mem[atemp];
    WORD low = chars & 0xff;
    WORD high = chars >> BYTE_SIZE;

    if (!low) break;
    printf("%c", low);

    if (!high) break;
    printf("%c", high);

    atemp++;
  }
}

WORD read_string(WORD* mem, ADDRESS start, ADDRESS max, FILE* input) {
  ADDRESS atemp = start;
  bool first = true;
  WORDU two_chars = 0;

  while (atemp < max - 1) {
    char ch;
    int res = fscanf(input, "%c", &ch);

    if (ch == '\n' || ch == EOF) {
      if (first) {
        two_chars = 0;
      } else {
        two_chars &= 0xff;
      }
      mem[atemp] = two_chars;
      mem[atemp + 1] = 0;
      return ch == EOF ? -1 : 1;

    } else {
      if (first) {
        first = false;
        two_chars = ch & 0xff;
      } else {
        first = true;
        two_chars |= (ch << BYTE_SIZE);
        mem[atemp++] = two_chars;
      }
    }
  }
  mem[atemp + 1] = 0;
  return 0;
}

void display_op_name(OP_CODE op, FILE* stream) {
  switch (op) {
    case HALT: fprintf(stream, "HALT"); break;
    case PUSH: fprintf(stream, "PUSH"); break;
    case POP: fprintf(stream, "POP"); break;
    case LOAD: fprintf(stream, "LOAD"); break;
    case STORE: fprintf(stream, "STORE"); break;
    case FST: fprintf(stream, "FST"); break;
    case SEC: fprintf(stream, "SEC"); break;
    case NTH: fprintf(stream, "NTH"); break;
    case SWAP: fprintf(stream, "SWAP"); break;
    case ROT: fprintf(stream, "ROT"); break;
    case RPUSH: fprintf(stream, "RPUSH"); break;
    case RPOP: fprintf(stream, "RPOP"); break;
    case RGRAB: fprintf(stream, "RGRAB"); break;
    case DPUSH: fprintf(stream, "DPUSH"); break;
    case DPOP: fprintf(stream, "DPOP"); break;
    case DLOAD: fprintf(stream, "DLOAD"); break;
    case DSTORE: fprintf(stream, "DSTORE"); break;
    case DFST: fprintf(stream, "DFST"); break;
    case DSEC: fprintf(stream, "DSEC"); break;
    case DNTH: fprintf(stream, "DNTH"); break;
    case DSWAP: fprintf(stream, "DSWAP"); break;
    case DROT: fprintf(stream, "DROT"); break;
    case DRPUSH: fprintf(stream, "DRPUSH"); break;
    case DRPOP: fprintf(stream, "DRPOP"); break;
    case DRGRAB: fprintf(stream, "DRGRAB"); break;
    case ADD: fprintf(stream, "ADD"); break;
    case SUB: fprintf(stream, "SUB"); break;
    case MULT: fprintf(stream, "MULT"); break;
    case DIV: fprintf(stream, "DIV"); break;
    case MOD: fprintf(stream, "MOD"); break;
    case EQ: fprintf(stream, "EQ"); break;
    case LT: fprintf(stream, "LT"); break;
    case GT: fprintf(stream, "GT"); break;
    case MULTU: fprintf(stream, "MULTU"); break;
    case DIVU: fprintf(stream, "DIVU"); break;
    case MODU: fprintf(stream, "MODU"); break;
    case LTU: fprintf(stream, "LTU"); break;
    case GTU: fprintf(stream, "GTU"); break;
    case SL: fprintf(stream, "SL"); break;
    case SR: fprintf(stream, "SR"); break;
    case AND: fprintf(stream, "AND"); break;
    case OR: fprintf(stream, "OR"); break;
    case NOT: fprintf(stream, "NOT"); break;
    case DADD: fprintf(stream, "DADD"); break;
    case DSUB: fprintf(stream, "DSUB"); break;
    case DMULT: fprintf(stream, "DMULT"); break;
    case DDIV: fprintf(stream, "DDIV"); break;
    case DMOD: fprintf(stream, "DMOD"); break;
    case DEQ: fprintf(stream, "DEQ"); break;
    case DLT: fprintf(stream, "DLT"); break;
    case DGT: fprintf(stream, "DGT"); break;
    case DMULTU_unused: fprintf(stream, "DMULTU_unused"); break;
    case DDIVU: fprintf(stream, "DDIVU"); break;
    case DMODU: fprintf(stream, "DMODU"); break;
    case DLTU: fprintf(stream, "DLTU"); break;
    case DGTU: fprintf(stream, "DGTU"); break;
    case DSL: fprintf(stream, "DSL"); break;
    case DSR: fprintf(stream, "DSR"); break;
    case DAND: fprintf(stream, "DAND"); break;
    case DOR: fprintf(stream, "DOR"); break;
    case DNOT: fprintf(stream, "DNOT"); break;
    case JUMP: fprintf(stream, "JUMP"); break;
    case BRANCH: fprintf(stream, "BRANCH"); break;
    case CALL: fprintf(stream, "CALL"); break;
    case RET: fprintf(stream, "RET"); break;
    case DSP: fprintf(stream, "DSP"); break;
    case PC: fprintf(stream, "PC"); break;
    case BFP: fprintf(stream, "BFP"); break;
    case FMP: fprintf(stream, "FMP"); break;
    case WPRN: fprintf(stream, "WPRN"); break;
    case DPRN: fprintf(stream, "DPRN"); break;
    case WPRNU: fprintf(stream, "WPRNU"); break;
    case DPRNU: fprintf(stream, "DPRNU"); break;
    case FPRN: fprintf(stream, "FPRN"); break;
    case FPRNSC: fprintf(stream, "FPRNSC"); break;
    case PRNCH: fprintf(stream, "PRNCH"); break;
    case PRN: fprintf(stream, "PRN"); break;
    case PRNLN: fprintf(stream, "PRNLN"); break;
    case PRNSP_unused: fprintf(stream, "PRNSP_unused"); break;
    case PRNMEM: fprintf(stream, "PRNMEM"); break;
    case WREAD: fprintf(stream, "WREAD"); break;
    case DREAD: fprintf(stream, "DREAD"); break;
    case FREAD: fprintf(stream, "FREAD"); break;
    case FREADSC: fprintf(stream, "FREADSC"); break;
    case READCH: fprintf(stream, "READCH"); break;
    case READ_unused: fprintf(stream, "READ_unused"); break;
    case READLN: fprintf(stream, "READLN"); break;
    case READSP_unused: fprintf(stream, "READSP_unused"); break;
    case HIGH: fprintf(stream, "HIGH"); break;
    case LOW: fprintf(stream, "LOW"); break;
    case BFSTORE: fprintf(stream, "BFSTORE"); break;
    case BFLOAD: fprintf(stream, "BFLOAD"); break;
    case UNPACK: fprintf(stream, "UNPACK"); break;
    case PACK: fprintf(stream, "PACK"); break;
    case MEMCOPY: fprintf(stream, "MEMCOPY"); break;
    case STRCOPY: fprintf(stream, "STRCOPY"); break;
    case FMULT: fprintf(stream, "FMULT"); break;
    case FDIV: fprintf(stream, "FDIV"); break;
    case FMULTSC: fprintf(stream, "FMULTSC"); break;
    case FDIVSC: fprintf(stream, "FDIVSC"); break;
    case PRNPK: fprintf(stream, "PRNPK"); break;
    default: fprintf(stream, "code=%hx (%hd)", op, op); break;
  }
}

void debug_info_display(WORD* data_stack, WORD* return_stack, ADDRESS dsp,
                        ADDRESS rsp, ADDRESS pc, WORD op) {
  // print stacks and pointers
  fflush(stdout);
  fprintf(stderr, "\nDstack: ");
  display_range(data_stack, 0x0001, dsp + 1, DEBUGGING);
  fprintf(stderr, "Rstack: ");
  display_range(return_stack, 0x0001, rsp + 1, DEBUGGING);
  fprintf(stderr, "PC: %hx (%hu), Next OP: ", pc, pc);
  display_op_name((OP_CODE)op, stderr);
  fprintf(stderr, "\n");
}

bool debug_step() {
  char *buffer = NULL;
  size_t size;

  fflush(stdout);
  fprintf(stderr, "\n***Step? ");
  ssize_t res = getline(&buffer, &size, stdin);
  return res <= 1;
}

/// ltrun.c //////////////////////////////////////////////////////////////////
size_t execute(WORD* memory, size_t length, WORD* data_stack, WORD* return_stack) {
  // The input stream
  FILE* input = stdin;

  // Declare and initialize memory pointer "registers"
  ADDRESS dsp, rsp, pc, bfp, fmp;
  dsp = 0;
  rsp = 0;
  pc = 0;
  bfp = length;
  fmp = length + BUFFER_SIZE;

  // conditions
  bool eof = false;
  bool wait_for_break = false;

  // Declare some temporary "registers" for working with intermediate values
  ADDRESS atemp;
  WORD temp;
  WORDU utemp;
  DWORD dtemp;
  
  // Run the program in memory
  bool run = true;
  while (run) {
    // Print stack, op code, and pc before every execution
    if (DEBUGGING && !wait_for_break) {
      debug_info_display(data_stack, return_stack, dsp, rsp, pc, memory[pc] & 0xff);
      bool step = debug_step();
      if (!step) {
        wait_for_break = true;
        fprintf(stderr, "\n* Skipping To Break Point *\n\n");
      }
    }

    // Catch some common pointer/address errors
    if (pc >= bfp) {
      fprintf(stderr,
              "Error: program counter out of bounds, pc: %hx, bfp: %hx\n",
              pc, bfp);
      return EXIT_POB;
    } else if (dsp > 0x8000) {  // i.e. it has wrapped around into negatives
      fprintf(stderr, "Error: stack underflow, sp: %hx (%hd)\n", dsp, dsp);
      return EXIT_SUF;
    } else if (dsp > END_STACK) {
      fprintf(stderr, "Error: stack overflow, sp: %hx (%hd)\n", dsp, dsp);
      return EXIT_SOF;
    } else if (rsp > 0x8000) {  // i.e. it has wrapped around into negatives
      fprintf(stderr, "Error: return stack underflow, sp: %hx (%hd)\n",
              rsp, rsp);
      return EXIT_RSUF;
    } else if (rsp > END_RETURN) {
      fprintf(stderr, "Error: return stack overflow, sp: %hx (%hd)\n",
              rsp, rsp);
      return EXIT_RSOF;
    }

    // Switch to cover each opcode. It is too long, but for simplicity and
    // efficiency it is kept this way, with larger operations calling
    // functions. The functions for things like unpacking and packing
    // double words are declared as inline so they will be more efficient.
    // Larger functions for things like io operations are regular functions
    // because they are not really hurt by the function call.
    //fprintf(stderr, "%04hx\n", pc);
    switch (memory[pc] & 0xff) {
      case HALT:
        run = false;
        break;
      case BRKPNT:
        wait_for_break = false;
        break;

      /// Stack Manipulation ///
      case PUSH:
        data_stack[++dsp] = memory[++pc];
        break;
      case POP:
        dsp--;
        break;
      case LOAD:
        if (memory[pc] >> BYTE_SIZE & 1)
          data_stack[dsp] = memory[(ADDRESS)data_stack[dsp]];
        else
          data_stack[dsp] = memory[fmp + (ADDRESS)data_stack[dsp]];
        break;
      case STORE:
        if (memory[pc] >> BYTE_SIZE & 1) {
          memory[(ADDRESS)data_stack[dsp]] = data_stack[dsp-1];
        } else {
          memory[fmp + (ADDRESS)data_stack[dsp]] = data_stack[dsp-1];
        }
        dsp-=2;
        break;
      case FST:
        data_stack[dsp+1] = data_stack[dsp];
        dsp++;
        break;
      case SEC:
        data_stack[dsp+1] = data_stack[dsp-1];
        dsp++;
        break;
      case NTH:
        data_stack[dsp] = data_stack[dsp - data_stack[dsp] - 1];
        break;
      case SWAP:
        temp = data_stack[dsp];
        data_stack[dsp] = data_stack[dsp-1];
        data_stack[dsp-1] = temp;
        break;
      case ROT:
        temp = data_stack[dsp-2];
        data_stack[dsp-2] = data_stack[dsp-1];
        data_stack[dsp-1] = data_stack[dsp];
        data_stack[dsp] = temp;
        break;
      case RPUSH:
        return_stack[++rsp] = data_stack[dsp--];
        break;
      case RPOP:
        data_stack[++dsp] = return_stack[rsp--];
        break;
      case RGRAB:
        data_stack[++dsp] = return_stack[rsp];
        break;

      /// Double Word Stack Manipulation ///
      case DPUSH:
        data_stack[++dsp] = memory[++pc];
        data_stack[++dsp] = memory[++pc];
        break;
      case DPOP:
        dsp-=2;
        break;
      case DLOAD:
        atemp = data_stack[dsp--];
        if (memory[pc] >> BYTE_SIZE & 1) {
          data_stack[++dsp] = memory[atemp];
          data_stack[++dsp] = memory[atemp + 1];
        } else {
          data_stack[++dsp] = memory[fmp + atemp];
          data_stack[++dsp] = memory[fmp + atemp + 1];
        }
        break;
      case DSTORE:
        atemp = data_stack[dsp--];
        if (memory[pc] >> BYTE_SIZE & 1) {
          memory[atemp] = data_stack[dsp-1];
          memory[atemp + 1] = data_stack[dsp];
        } else {
          memory[fmp + atemp] = data_stack[dsp-1];
          memory[fmp + atemp + 1] = data_stack[dsp];
        }
        dsp-=2;
        break;
      case DFST:
        data_stack[dsp+1] = data_stack[dsp-1];
        data_stack[dsp+2] = data_stack[dsp];
        dsp+=2;
        break;
      case DSEC:
        data_stack[dsp+1] = data_stack[dsp-3];
        data_stack[dsp+2] = data_stack[dsp-2];
        dsp+=2;
        break;
      case DNTH:
        atemp = data_stack[dsp--] * 2;
        data_stack[dsp+1] = data_stack[dsp - atemp - 1];
        data_stack[dsp+2] = data_stack[dsp - atemp];
        dsp+=2;
        break;
      case DSWAP:
        temp = data_stack[dsp];
        data_stack[dsp] = data_stack[dsp-2];
        data_stack[dsp-2] = temp;

        temp = data_stack[dsp-1];
        data_stack[dsp-1] = data_stack[dsp-3];
        data_stack[dsp-3] = temp;
        break;
      case DROT:
        temp = data_stack[dsp-5];
        data_stack[dsp-5] = data_stack[dsp-3];
        data_stack[dsp-3] = data_stack[dsp-1];
        data_stack[dsp-1] = temp;

        temp = data_stack[dsp-4];
        data_stack[dsp-4] = data_stack[dsp-2];
        data_stack[dsp-2] = data_stack[dsp];
        data_stack[dsp] = temp;
        break;
      case DRPUSH:
        return_stack[++rsp] = data_stack[dsp-1];
        return_stack[++rsp] = data_stack[dsp];
        dsp-=2;
        break;
      case DRPOP:
        data_stack[++dsp] = return_stack[rsp-1];
        data_stack[++dsp] = return_stack[rsp];
        rsp-=2;
        break;
      case DRGRAB:
        data_stack[++dsp] = return_stack[rsp-1];
        data_stack[++dsp] = return_stack[rsp];
        break;

      /// Word Arithmetic ///
      case ADD:
        data_stack[dsp-1] = data_stack[dsp-1] + data_stack[dsp];
        dsp--;
        break;
      case SUB:
        data_stack[dsp-1] = data_stack[dsp-1] - data_stack[dsp];
        dsp--;
        break;
      case MULT:
        data_stack[dsp-1] = data_stack[dsp-1] * data_stack[dsp];
        dsp--;
        break;
      case DIV:
        data_stack[dsp-1] = data_stack[dsp-1] / data_stack[dsp];
        dsp--;
        break;
      case MOD:
        data_stack[dsp-1] = data_stack[dsp-1] % data_stack[dsp];
        dsp--;
        break;

      /// Signed Comparisson ///
      case EQ:
        data_stack[dsp-1] = data_stack[dsp-1] == data_stack[dsp];
        dsp--;
        break;
      case LT:
        data_stack[dsp-1] = data_stack[dsp-1] < data_stack[dsp];
        dsp--;
        break;
      case GT:
        data_stack[dsp-1] = data_stack[dsp-1] > data_stack[dsp];
        dsp--;
        break;

      /// Unsigned Artihmetic and Comparisson ///
      case MULTU:
        {
          // large signed numbers cast to unsigned dword as 0xffff____
          // so we have to zero those bits before the calculation 
          DWORDU a = (DWORDU)data_stack[dsp-1] & 0xffff;
          DWORDU b = (DWORDU)data_stack[dsp] & 0xffff;
          DWORDU res = a * b;
          data_stack[dsp-1] = res >> 16; 
          data_stack[dsp] = res;
        }
        break;
      case DIVU:
        data_stack[dsp-1] = (WORDU)data_stack[dsp-1] / (WORDU)data_stack[dsp];
        dsp--;
        break;
      case MODU:
        data_stack[dsp-1] = (WORDU)data_stack[dsp-1] % (WORDU)data_stack[dsp];
        dsp--;
        break;
      case LTU:
        data_stack[dsp-1] = (WORDU)data_stack[dsp-1] < (WORDU)data_stack[dsp];
        dsp--;
        break;
      case GTU:
        data_stack[dsp-1] = (WORDU)data_stack[dsp-1] > (WORDU)data_stack[dsp];
        dsp--;
        break;

      /// Double Arithmetic and Comparisson ///
      case DADD:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     + get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DSUB:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     - get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DMULT:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     * get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DDIV:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     / get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DMOD:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     % get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DEQ:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     == get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DLT:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     < get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DGT:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     > get_dword(data_stack, dsp-1));
        dsp-=2;
        break;

      /// Unsigned Double Arithmetic and Comparisson ///
      case DDIVU:
        set_dword(data_stack, dsp-3, (DWORDU)get_dword(data_stack, dsp-3)
                                     / (DWORDU)get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DMODU:
        set_dword(data_stack, dsp-3, (DWORDU)get_dword(data_stack, dsp-3)
                                     % (DWORDU)get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DLTU:
        set_dword(data_stack, dsp-3, (DWORDU)get_dword(data_stack, dsp-3)
                                     < (DWORDU)get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DGTU:
        set_dword(data_stack, dsp-3, (DWORDU)get_dword(data_stack, dsp-3)
                                     > (DWORDU)get_dword(data_stack, dsp-1));
        dsp-=2;
        break;

      /// Bitwise words ///
      case SL:
        data_stack[dsp-1] = data_stack[dsp-1] << data_stack[dsp];
        dsp--;
        break;
      case SR:
        data_stack[dsp-1] = data_stack[dsp-1] >> data_stack[dsp];
        dsp--;
        break;
      case AND:
        data_stack[dsp-1] = data_stack[dsp-1] & data_stack[dsp];
        dsp--;
        break;
      case OR:
        data_stack[dsp-1] = data_stack[dsp-1] | data_stack[dsp];
        dsp--;
        break;
      case NOT:
        data_stack[dsp] = ~data_stack[dsp];
        break;

      /// Bitwise double words ///
      case DSL:
        set_dword(data_stack, dsp-2, get_dword(data_stack, dsp-2)
                                     << data_stack[dsp]);
        dsp--;
        break;
      case DSR:
        set_dword(data_stack, dsp-2, get_dword(data_stack, dsp-2)
                                     >> data_stack[dsp]);
        dsp--;
        break;
      case DAND:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     & get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DOR:
        set_dword(data_stack, dsp-3, get_dword(data_stack, dsp-3)
                                     | get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case DNOT:
        set_dword(data_stack, dsp-1, ~get_dword(data_stack, dsp-1));
        break;

      /// Movement ///
      case JUMP:
        pc = data_stack[dsp--];
        continue;
      case BRANCH:
        atemp = data_stack[dsp--];
        temp = data_stack[dsp--];
        if (temp) {
          pc = atemp;
          continue;
        }
        break;
      case CALL:
        return_stack[++rsp] = pc + 1;
        pc = data_stack[dsp--];
        continue;
      case RET:
        pc = return_stack[rsp--];
        continue;
      case DSP:
        data_stack[dsp+1] = dsp;
        dsp++;
        break;
      case PC:
        data_stack[++dsp] = pc;
        break;
      case BFP:
        data_stack[++dsp] = bfp;
        break;
      case FMP:
        data_stack[++dsp] = fmp;
        break;

      /// Number Printing ///
      case WPRN:
        printf("%hd", data_stack[dsp--]);
        break;
      case DPRN:
        printf("%d", get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case WPRNU:
        printf("%hu", data_stack[dsp--]);
        break;
      case DPRNU:
        printf("%u", get_dword(data_stack, dsp-1));
        dsp-=2;
        break;
      case FPRN:
        printf("%.3lf", (double)get_dword(data_stack, dsp-1)
                        / SCALES[ DEFAULT_SCALE ]);
        dsp-=2;
        break;
      case FPRNSC:
        temp = data_stack[dsp--];
        if (temp && temp < SCALE_MAX) {
          dtemp = SCALES[temp];
        } else {
          dtemp = SCALES[ DEFAULT_SCALE ];
          temp = DEFAULT_SCALE;
        }
        printf("%.*lf", temp, (double)get_dword(data_stack, dsp-1) / dtemp);
        dsp-=2;
        break;

      /// Char and String printing ///
      case PRNCH:
        printf("%c", data_stack[dsp--] & 0xff);
        break;
      case PRNPK:
        temp = data_stack[dsp--];
        printf("%c", temp & 0xff);
        printf("%c", (temp >> BYTE_SIZE) & 0xff);
        break;
      case PRN:
        // Print from bfp to first null or buffer end
        print_string(memory, bfp, fmp);
        break;
      case PRNLN:
        // Print from bfp to first null or buffer end with a newline
        print_string(memory, bfp, fmp);
        printf("\n");
        break;
      case PRNMEM:
        atemp = data_stack[dsp--];
        if (memory[pc] >> BYTE_SIZE & 1) {
          print_string(memory, atemp, END_MEMORY);
        } else {
          print_string(memory, fmp + atemp, END_MEMORY);
        } 
        break;

      /// Reading ///
      case WREAD:
        dtemp = fscanf(input, "%hd", &temp);
        if (dtemp == EOF) {
          eof = true;
          data_stack[++dsp] = 0;
        } else {
          data_stack[++dsp] = temp;
        }
        if (DEBUGGING) getchar();
        break;
      case DREAD:
        {
          int res = fscanf(input, "%d", &dtemp);
          if (res == EOF) {
            eof = true;
            set_dword(data_stack, dsp + 1, 0);
          } else {
            set_dword(data_stack, dsp + 1, dtemp);
          }
          dsp+=2;
        }
        if (DEBUGGING) getchar();
        break;
      case FREAD:
        {
          double x;
          dtemp = fscanf(input, "%lf", &x);
          if (dtemp == EOF) {
            eof = true;
            set_dword(data_stack, dsp + 1, 0);
          } else {
            set_dword(data_stack, dsp + 1, (DWORD)(x * SCALES[ DEFAULT_SCALE ]));
          }
          dsp+=2;
        }
        if (DEBUGGING) getchar();
        break;
      case FREADSC:
        {
          // TODO no way to get scale off of stack?
          temp = data_stack[dsp--];
          if (temp && temp < SCALE_MAX) {
            dtemp = SCALES[temp];
          } else {
            dtemp = SCALES[ DEFAULT_SCALE ];
          }
          double x;
          int res = fscanf(input, "%lf", &x);
          if (res == EOF) {
            eof = true;
            set_dword(data_stack, dsp + 1, 0);
          } else {
            set_dword(data_stack, dsp + 1, (DWORD)(x * dtemp));
          }
          dsp+=2;
        }
        if (DEBUGGING) getchar();
        break;
      case READCH:
        {
          char ch;
          int res = fscanf(input, "%c", &ch);
          if (res == EOF) {
            eof = true;
            data_stack[++dsp] = 0;
          } else {
            data_stack[++dsp] = (WORD)ch & 0xff;
          }
        }
        if (DEBUGGING) getchar();
        break;
      case READLN:
        temp = read_string(memory, bfp, fmp, input);
        if (temp == -1) {
          eof = true;
        } else {
          data_stack[++dsp] = temp;
        }
        break;

      /// Buffer and Chars ///
      case BFSTORE:
        atemp = data_stack[dsp--];
        memory[bfp + atemp] = data_stack[dsp--];
        break;
      case BFLOAD:
        atemp = data_stack[dsp];
        data_stack[dsp] = memory[bfp + atemp];
        break;
      case HIGH:
        data_stack[dsp+1] = (data_stack[dsp] >> BYTE_SIZE) & 0xff;
        dsp++;
        break;
      case LOW:
        data_stack[dsp+1] = data_stack[dsp] & 0xff;
        dsp++;
        break;
      case UNPACK:
        temp = data_stack[dsp];
        data_stack[++dsp] =  (temp >> BYTE_SIZE) & 0xff;
        data_stack[++dsp] = temp & 0xff;
        break;
      case PACK:
        temp = data_stack[dsp--];
        data_stack[dsp] = temp | (data_stack[dsp] << BYTE_SIZE);
        break;

      /// Memory copying ///
      case MEMCOPY:
        utemp = data_stack[dsp--];
        switch (memory[pc] >> BYTE_SIZE) {
          case MEM_BUF:
            atemp = data_stack[dsp--];
            memcpy(memory + bfp,
                   memory + fmp + atemp,
                   utemp * 2);
            break;
          case BUF_MEM:
            atemp = data_stack[dsp--];
            memcpy(memory + fmp + atemp,
                   memory + bfp,
                   utemp * 2);
            break;
        }
        break;
      case STRCOPY:
        switch (memory[pc] >> BYTE_SIZE) {
          case MEM_BUF:
            atemp = data_stack[dsp--];
            utemp = string_length(memory, fmp + atemp);
            memcpy(memory + bfp,
                   memory + fmp + atemp,
                   utemp * 2);
            break;
          case BUF_MEM:
            atemp = data_stack[dsp--];
            utemp = string_length(memory, bfp);
            memcpy(memory + fmp + atemp,
                   memory + bfp,
                   utemp * 2);
            break;
        }
        break;

      /// Fixed point arithmetic ///
      // only for those operations that cannot be done by dword ops
      case FMULT:
        {
          long long inter = (long long)get_dword(data_stack, dsp-3)
                            * (long long)get_dword(data_stack, dsp-1);
          dsp-=2;
          set_dword(data_stack, dsp-1, inter / SCALES[ DEFAULT_SCALE ]);
        }
        break;
      case FDIV:
        {
          double inter = (double)get_dword(data_stack, dsp-3)
                         / (double)get_dword(data_stack, dsp-1);
          dsp-=2;
          set_dword(data_stack, dsp-1, inter * SCALES[ DEFAULT_SCALE ]);
        }
        break;
      case FMULTSC:
        {
          temp = data_stack[dsp--];
          if (temp && temp < SCALE_MAX) {
            dtemp = SCALES[temp];
          } else {
            dtemp = SCALES[ DEFAULT_SCALE ];
          }
          long long inter = (long long)get_dword(data_stack, dsp-3)
                            * (long long)get_dword(data_stack, dsp-1);
          dsp-=2;
          set_dword(data_stack, dsp-1, inter / dtemp);
        }
        break;
      case FDIVSC:
        {
          temp = data_stack[dsp--];
          if (temp && temp < SCALE_MAX) {
            dtemp = SCALES[temp];
          } else {
            dtemp = SCALES[ DEFAULT_SCALE ];
          }
          double inter = (double)get_dword(data_stack, dsp-3)
                         / (double)get_dword(data_stack, dsp-1);
          dsp-=2;
          set_dword(data_stack, dsp-1, inter * dtemp);
        }
        break;

      /// Other ///
      case READCH_BF:
        {
          char ch;
          int res = fscanf(input, "%c", &ch);
          atemp = data_stack[dsp--];
          if (atemp % 2 == 0) {
            memory[bfp + (atemp / 2)] = ch;
          } else {
            memory[bfp + (atemp / 2)] |= (ch << BYTE_SIZE);
            memory[bfp + (atemp / 2) + 1] = 0;
          }
        }
        break;
      case STREQ:
        {
          atemp = data_stack[dsp--];
          utemp = data_stack[dsp--];
          size_t i = 0;

          while (i + atemp < END_MEMORY && i + utemp < END_MEMORY) {
            WORDU chs1, chs2;
            chs1 = memory[atemp + i];
            chs2 = memory[utemp + i];

            if (chs1 != chs2) {
              data_stack[++dsp] = 0;
              break;
            } else if (chs1 == 0  // whole thing is 0
                       || (chs1 & 0x00ff) == 0     // first byte is 0
                       || (chs1 & 0xff00) == 0) {  // second byte is 0
              data_stack[++dsp] = 1;
              break;
            }
            i++;
          }
        }
        break;
      case MEMEQ:
        {
          WORDU size = data_stack[dsp--];
          WORDU first = data_stack[dsp--];
          WORDU second = data_stack[dsp--];
          data_stack[++dsp] = mem_equal(memory, first, second, size);
        }
        break;
      case IS_EOF:
        data_stack[++dsp] = eof ? 1 : 0;
        break;
      case RESET_EOF:
        eof = false;
        break;
      case OPEN:
        {
          char filename[256];
          memcpy(filename, memory + bfp, 256);
          input = fopen(filename, "r");
          if (input == NULL) {
            fprintf(stderr, "Error: Could not open file: %s\n", memory + bfp);
            input = stdin;
          }
        }
        break;
      case CLOSE:
        if (input != stdin) {
          fclose(input);
          input = stdin;
        }
        break;

      /// BAD OP CODE ///
      default:
        fprintf(stderr, "Error: Unknown OP code: 0x%hx\n", memory[pc]);
        return EXIT_OP;
    }
    pc++;
  }

  // When program is run for tests we print out the contents of the stack
  // to stdout to check that the program ended in the expected state.
  // Because we always increment dsp before pushing a value the true start of
  // the stack is index 1 and not 0.
  if (TESTING) {
    display_range(data_stack, 0x0001, dsp + 1, false);
  }

  return EXIT_SUCCESS;
}

/// main.c ///////////////////////////////////////////////////////////////////
size_t prog_length();
void set_program(WORD* mem, size_t length);

int main( int argc, char *argv[] ) {
  // VM memory pointers
  WORD* data_stack;
  WORD* return_stack;
  WORD* memory;

  // Allocate memory for the Data Stack
  data_stack = (WORD*) calloc((size_t)END_STACK + 1, sizeof(WORD));
  if (data_stack == NULL) {
    fprintf(stderr, "Error: Could not allocate the Data Stack\n");
    exit(EXIT_MEM);
  }

  // Allocate memory for the Return Stack
  return_stack = (WORD*) calloc((size_t)END_RETURN + 1, sizeof(WORD));
  if (return_stack == NULL) {
    fprintf(stderr, "Error: Could not allocate the Return Stack\n");
    exit(EXIT_MEM);
  }

  // Allocate memory for the Main Memory
  memory = (WORD*) calloc((size_t)END_MEMORY + 1, sizeof(WORD));
  if (memory == NULL) {
    fprintf(stderr, "Error: Could not allocate Main Program Memory\n");
    exit(EXIT_MEM);
  }

  // Load the program
  size_t length = prog_length();
  if (!length) {
    fprintf(stderr, "Error: program length is 0\n");
    exit(EXIT_FILE);
  } else if ((length / 2) + 1 >= END_MEMORY) {
    fprintf(stderr, "Error: program is to large to fit in memory\n");
    exit(EXIT_MEM);
  }
  set_program(memory, length);
  if (argc >= 2) {
    memcpy(memory + length, argv[1], strlen(argv[1]));
  }

  // Run program
  size_t result = execute(memory, length, data_stack, return_stack);

  // clean up
  free(memory);
  free(data_stack);
  free(return_stack);

  return result;
}

/** sample of what needs to be written for this to create a binary for a
 * full program with vm and assembled bytes

size_t prog_length() { return 6; }

void set_program(WORD* mem, size_t length) {
  char prog[] = {
    1,
    0,
    100,
    0,
    69,
    0
  };
  memcpy(mem, prog, length);
};

*/
size_t prog_length() { return 14478;  }
void set_program(WORD* mem, size_t length) {
// length is not used here, but is a required param
  WORD program[] = { 1, 569, 61, 512, 18766, 76, 0, 0, 0, 512, 84, 0, 0, 0, 3, 512, 70, 0, 0, 0, 9, 512, 30065, 29807, 101, 0, 15, 512, 28515, 25710, 0, 0, 21, 512, 25956, 26982, 25966, 0, 27, 512, 24940, 25197, 24932, 0, 33, 512, 25964, 116, 0, 0, 39, 512, 21061, 20306, 82, 0, 45, 512, 27946, 26977, 10862, 0, 51, 3840, 3, 3, 0, 3840, 9, 9, 63, 3840, 15, 15, 67, 3840, 51, 51, 71, 1, 29804, 28787, 8254, 0, 28769, 27760, 121, 30309, 27745, 0, 28525, 100, 25964, 29555, 0, 29287, 24933, 25972, 114, 25934, 27767, 28265, 101, 24916, 98, 28755, 25441, 101, 2683, 8224, 27706, 28261, 29799, 8296, 0, 8224, 27962, 28005, 29295, 8313, 91, 8202, 8224, 8224, 8224, 8224, 8224, 0, 2653, 2685, 0, 0, 1, 0, 10, 0, 100, 0, 1000, 0, 10000, 1, -31072, 15, 16960, 152, -27008, 1525, -7936, 15258, -13824, 10794, 8234, 26691, 29281, 25376, 28257, 28526, 8308, 29811, 29281, 8308, 8289, 31091, 25197, 27759, 8250, 0, 10794, 8234, 26691, 29281, 25376, 28257, 28526, 8308, 25954, 26912, 8302, 8289, 31091, 25197, 27759, 8250, 0, 10794, 8234, 26691, 29281, 25376, 28257, 28526, 8308, 25954, 26912, 8302, 8289, 30062, 25197, 29285, 8250, 0, 10794, 8234, 26691, 29281, 25376, 28257, 28526, 8308, 29811, 29281, 8308, 8289, 30062, 25197, 29285, 8250, 0, 10794, 8234, 30030, 25197, 29285, 8307, 29281, 8293, 26988, 26989, 25972, 8292, 28532, 24864, 27936, 30817, 28448, 8294, 12337, 25632, 26473, 29801, 11379, 26912, 25454, 30060, 26980, 26478, 25632, 25445, 28009, 27745, 25632, 26473, 29801, 115, 10794, 8234, 26950, 25976, 8292, 28528, 28265, 8308, 30062, 25197, 29285, 8307, 29281, 8293, 26988, 26989, 25972, 8292, 28532, 24864, 27936, 30817, 28009, 28021, 28448, 8294, 8243, 25956, 26979, 24941, 8300, 26980, 26983, 29556, 0, 10794, 8234, 31059, 25197, 27759, 26656, 29537, 28192, 29807, 25120, 25957, 8302, 25956, 26982, 25966, 14948, 32, 10794, 8234, 31059, 25197, 27759, 26656, 29537, 24864, 29292, 24933, 31076, 25120, 25957, 8302, 25956, 26982, 25966, 14948, 32, 10794, 8234, 28233, 24950, 26988, 8292, 24948, 26482, 29797, 26144, 29295, 25632, 26213, 28265, 14949, 32, 10794, 8234, 24899, 28270, 29807, 24864, 28784, 31084, 8250, 28494, 8308, 8289, 29296, 25455, 25701, 29301, 14949, 32, 10794, 8234, 24899, 28270, 29807, 25632, 8303, 29281, 29801, 28008, 29797, 25449, 8250, 28494, 8308, 8289, 30062, 25197, 29285, 8250, 0, 10794, 8234, 24912, 26990, 14947, 21536, 26994, 25701, 25888, 24950, 30060, 29793, 28265, 8295, 29537, 24864, 29472, 25968, 26979, 27745, 26144, 29295, 14957, 32, 10794, 8234, 28483, 29550, 29216, 29029, 26997, 25970, 8307, 8242, 29281, 30055, 25965, 29806, 2675, 0, 10794, 8234, 24909, 8304, 25970, 30065, 29289, 29541, 12832, 24864, 26482, 28021, 28261, 29556, 10, 10794, 8234, 26950, 29804, 29285, 29216, 29029, 26997, 25970, 8307, 8242, 29281, 30055, 25965, 29806, 2675, 0, 10794, 8234, 25938, 30052, 25955, 29216, 29029, 26997, 25970, 8307, 8243, 29281, 30055, 25965, 29806, 2675, 0, 10794, 8234, 30022, 25454, 26996, 28271, 29216, 29029, 26997, 25970, 8307, 8241, 29281, 30055, 25965, 29806, 8250, 0, 10794, 8234, 30022, 25454, 26996, 28271, 29216, 29029, 26997, 25970, 8307, 8242, 29281, 30055, 25965, 29806, 14963, 32, 10794, 8234, 30022, 25454, 26996, 28271, 29216, 29029, 26997, 25970, 8307, 30062, 25965, 26994, 8291, 29281, 30055, 25965, 29806, 14963, 32, 10794, 8234, 28494, 10784, 24941, 28265, 8234, 30054, 25454, 26996, 28271, 30496, 29537, 26144, 30063, 25710, 10, 10794, 8234, 28499, 29810, 27936, 29557, 8308, 24936, 25974, 24864, 27680, 29545, 8308, 28518, 8306, 29801, 8307, 26982, 29554, 8308, 29281, 30055, 25965, 29806, 10, 1, 57, 1, 2, 3, 1, 4870, 63, 5, 1, 602, 62, 2, 1, 4946, 63, 1, 0, 89, 1, 0, 30, 1, 598, 62, 1, 606, 63, 0, 1, 671, 63, 0, 1, 700, 63, 0, 1, -4370, 68, 26, 1, 256, 350, 1, 751, 63, 1, -4370, 68, 26, 1, 256, 94, 1, 3, 1, 2, 3, 1, 2865, 63, 2, 2, 1, 57, 1, 2, 3, 1, 4870, 63, 5, 1, 0, 30, 1, 667, 62, 1, 0, 8, 1, 3, 1, 3, 1, 4331, 63, 1, 2, 3, 1, 2792, 63, 2, 2, 64, 1, 527, 335, 64, 1, 751, 63, 1, 80, 335, 1, 6311, 63, 1, 2, 3, 1, 5420, 63, 2, 1, 5015, 63, 1, 10, 75, 1, 10, 75, 1, 674, 61, 64, 1, 0, 8, 1, 3, 1, 3, 1, 4331, 63, 1, 2, 3, 1, 2792, 63, 2, 2, 64, 1, 5015, 63, 1, 10, 75, 64, 5, 69, 1, 10, 75, 6, 69, 1, 10, 75, 1, 2, 7, 69, 1, 10, 75, 64, 1, 2, 29, 1, 1, 30, 64, 1, 1074, 63, 1, 1140, 63, 1, 1218, 63, 1, 1293, 63, 1, 1393, 63, 1, 1433, 63, 1, 1520, 63, 1, 1474, 63, 1, 1619, 63, 1, 1718, 63, 1, 1825, 63, 1, 1961, 63, 1, 2102, 63, 1, 2157, 63, 1, 2215, 63, 1, 2257, 63, 1, 2358, 63, 1, 2499, 63, 1, 2645, 63, 1, 2696, 63, 1, 2749, 63, 1, 2771, 63, 1, 2844, 63, 1, 2899, 63, 1, 3067, 63, 1, 2983, 63, 1, 3032, 63, 1, 3146, 63, 1, 3186, 63, 1, 3214, 63, 1, 3264, 63, 1, 3297, 63, 1, 3560, 63, 1, 3361, 63, 1, 3457, 63, 1, 3593, 63, 1, 3901, 63, 64, 1, 875, 63, 8, 6, 1, 891, 63, 1, 4835, 63, 64, 1, 5009, 63, 1, 4593, 63, 67, 6, 1, 4612, 63, 5, 1, 4903, 63, 64, 1, 4985, 63, 1, 1536, 6, 260, 8, 6, 1, 2, 25, 260, 8, 6, 1, 1, 25, 260, 64, 22, 8, 5, 1, 3, 30, 1, 972, 62, 5, 1, 1, 25, 259, 5, 1, 4347, 63, 1, 0, 30, 1, 986, 62, 5, 259, 1, 8, 39, 1, 4, 30, 1, 959, 62, 23, 1, 5, 7, 63, 22, 1, 2, 25, 259, 1, 913, 61, 23, 13, 0, 1000, 45, 1, 6, 7, 63, 1, 1003, 63, 64, 2, 8, 2, 8, 2, 23, 1, 4991, 63, 1, 4441, 63, 8, 64, 23, 14, 1, 360, 335, 1, 719, 63, 2, 8, 2, 8, 2, 1, 51, 8, 64, 22, 1, 2, 25, 259, 5, 1, 3, 30, 1, 1043, 62, 5, 1, 1, 25, 259, 5, 1, 4347, 63, 1, 0, 30, 1, 1057, 62, 23, 1, 6, 7, 63, 22, 1, 2, 25, 259, 1, 1008, 61, 2, 8, 2, 8, 2, 23, 1, 4991, 63, 1, 4455, 63, 8, 64, 23, 14, 1, 360, 335, 1, 719, 63, 2, 8, 2, 8, 2, 1, 51, 8, 64, 1, 28515, 1, 0, 88, 1, 29550, 1, 1, 88, 1, 0, 1, 2, 88, 1, 1095, 1, 863, 63, 64, 8, 5, 1, 3, 30, 6, 1, 2, 25, 259, 1, 3, 30, 41, 1, 1131, 62, 5, 1, 1, 25, 259, 8, 1, 2, 25, 259, 1, 1, 25, 259, 1, 4280, 63, 8, 64, 1, 405, 335, 8, 2, 1, 51, 8, 64, 1, 24931, 1, 0, 88, 1, 114, 1, 1, 88, 1, 1156, 1, 863, 63, 64, 8, 5, 1, 3, 30, 1, 1204, 62, 1, 1, 25, 259, 5, 1, 4157, 63, 1, 1209, 62, 5, 259, 1, 8, 39, 1, 0, 30, 6, 259, 1, 8, 39, 1, 1, 30, 41, 1, 0, 30, 1, 1204, 62, 1, 1, 25, 259, 8, 64, 2, 1, 3, 8, 64, 259, 1, 255, 40, 1, 4198, 63, 8, 64, 1, 25699, 1, 0, 88, 1, 114, 1, 1, 88, 1, 1234, 1, 863, 63, 64, 8, 5, 1, 3, 30, 1, 1204, 62, 1, 1, 25, 259, 5, 1, 4157, 63, 1, 1287, 62, 5, 259, 1, 8, 39, 1, 0, 30, 6, 259, 1, 8, 39, 1, 1, 30, 41, 1, 0, 30, 1, 1282, 62, 1, 2, 25, 259, 8, 64, 2, 1, 3, 8, 64, 1, 1, 25, 259, 8, 64, 1, 29029, 1, 0, 88, 1, 63, 1, 1, 88, 1, 1309, 1, 863, 63, 64, 8, 5, 1, 1, 25, 259, 6, 1, 2, 25, 259, 1, 1, 25, 259, 1, 4553, 63, 1, 1349, 62, 1, 4383, 63, 1, 1357, 62, 1, 4401, 63, 1, 1376, 62, 2, 2, 2, 1, 15, 8, 64, 30, 1, 4797, 63, 8, 2, 8, 64, 1, 1, 25, 271, 9, 1, 1, 25, 271, 48, 8, 2, 1, 4797, 63, 8, 2, 8, 64, 1, 4503, 63, 9, 1, 4503, 63, 48, 8, 2, 1, 4797, 63, 8, 2, 8, 64, 1, 29793, 1, 0, 88, 1, 28015, 1, 1, 88, 1, 63, 1, 2, 88, 1, 1414, 1, 863, 63, 64, 8, 1, 1, 25, 259, 1, 6282, 63, 1, 1429, 62, 1, 15, 8, 64, 1, 9, 8, 64, 1, 25697, 1, 0, 88, 1, 100, 1, 1, 88, 1, 1449, 1, 863, 63, 64, 1, 1468, 1, 1461, 20, 13, 0, 0, 1, 911, 63, 64, 9, 1, 1, 25, 271, 43, 64, 9, 1, 4528, 63, 43, 64, 1, 30061, 1, 0, 88, 1, 29804, 1, 1, 88, 1, 0, 1, 2, 88, 1, 1495, 1, 863, 63, 64, 1, 1514, 1, 1507, 20, 13, 0, 1, 1, 911, 63, 64, 9, 1, 1, 25, 271, 45, 64, 9, 1, 4528, 63, 96, 64, 1, 30067, 1, 0, 88, 1, 98, 1, 1, 88, 1, 1536, 1, 863, 63, 64, 1, 1613, 1, 1606, 20, 8, 5, 1, 3, 30, 1, 1598, 62, 5, 1, 1, 25, 259, 5, 1, 4347, 63, 1, 0, 30, 1, 1598, 62, 5, 259, 1, 8, 39, 1, 4, 30, 1, 1590, 62, 1, 1, 25, 271, 20, 1, 2, 25, 259, 8, 20, 1, 911, 63, 64, 1, 1, 25, 271, 1, 1003, 63, 64, 8, 13, 0, 0, 1, 911, 63, 64, 9, 1, 1, 25, 271, 44, 64, 9, 1, 4528, 63, 44, 64, 1, 26980, 1, 0, 88, 1, 118, 1, 1, 88, 1, 1635, 1, 863, 63, 64, 1, 1712, 1, 1705, 20, 8, 5, 1, 3, 30, 1, 1697, 62, 5, 1, 1, 25, 259, 5, 1, 4347, 63, 1, 0, 30, 1, 1697, 62, 5, 259, 1, 8, 39, 1, 4, 30, 1, 1689, 62, 1, 1, 25, 271, 20, 1, 2, 25, 259, 8, 20, 1, 911, 63, 64, 1, 1, 25, 271, 1, 1003, 63, 64, 8, 13, 0, 0, 1, 911, 63, 64, 9, 1, 1, 25, 271, 46, 64, 9, 1, 4528, 63, 97, 64, 1, 28525, 1, 0, 88, 1, 100, 1, 1, 88, 1, 1734, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2, 7, 1, 2, 25, 259, 1, 3, 30, 41, 1, 1793, 62, 6, 1, 1, 25, 259, 1, 2, 7, 1, 2, 25, 259, 1, 1, 25, 259, 1, 4401, 63, 1, 0, 30, 1, 1809, 62, 8, 1, 4503, 63, 9, 1, 4503, 63, 47, 1, 4469, 63, 9, 2, 8, 64, 1, 488, 335, 1, 90, 335, 1, 10, 75, 8, 2, 1, 51, 8, 64, 64, 1, 506, 335, 1, 90, 335, 1, 10, 75, 8, 2, 1, 51, 8, 64, 64, 1, 25964, 1, 0, 88, 1, 29555, 1, 1, 88, 1, 0, 1, 2, 88, 1, 1846, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2, 7, 1, 2, 25, 259, 1, 3, 30, 41, 1, 1929, 62, 6, 1, 1, 25, 259, 1, 2, 7, 1, 2, 25, 259, 1, 1, 25, 259, 1, 4401, 63, 1, 0, 30, 1, 1945, 62, 1, 4365, 63, 1, 0, 30, 1, 1911, 62, 8, 1, 1, 25, 271, 9, 1, 1, 25, 271, 1, 1919, 61, 8, 1, 4528, 63, 9, 1, 4528, 63, 49, 8, 2, 1, 4571, 63, 9, 2, 8, 64, 1, 488, 335, 1, 92, 335, 1, 10, 75, 8, 2, 1, 51, 8, 64, 64, 1, 506, 335, 1, 92, 335, 1, 10, 75, 8, 2, 1, 51, 8, 64, 64, 1, 29287, 1, 0, 88, 1, 24933, 1, 1, 88, 1, 25972, 1, 2, 88, 1, 114, 1, 3, 88, 1, 1987, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2, 7, 1, 2, 25, 259, 1, 3, 30, 41, 1, 2070, 62, 6, 1, 1, 25, 259, 1, 2, 7, 1, 2, 25, 259, 1, 1, 25, 259, 1, 4401, 63, 1, 0, 30, 1, 2086, 62, 1, 4365, 63, 1, 0, 30, 1, 2052, 62, 8, 1, 1, 25, 271, 9, 1, 1, 25, 271, 1, 2060, 61, 8, 1, 4528, 63, 9, 1, 4528, 63, 50, 8, 2, 1, 4571, 63, 9, 2, 8, 64, 1, 488, 335, 1, 95, 335, 1, 10, 75, 8, 2, 1, 51, 8, 64, 64, 1, 506, 335, 1, 95, 335, 1, 10, 75, 8, 2, 1, 51, 8, 64, 64, 1, 28257, 1, 0, 88, 1, 100, 1, 1, 88, 1, 2118, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2145, 62, 6, 1, 1, 25, 259, 1, 4583, 63, 1, 2151, 62, 8, 1, 2, 25, 259, 8, 1, 2118, 61, 8, 2, 1, 9, 8, 64, 8, 2, 1, 15, 8, 64, 1, 29295, 1, 0, 88, 1, 0, 1, 1, 88, 1, 2173, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2209, 62, 6, 1, 1, 25, 259, 1, 4583, 63, 1, 0, 30, 1, 2203, 62, 8, 1, 2, 25, 259, 8, 1, 2173, 61, 8, 2, 1, 9, 8, 64, 8, 2, 1, 15, 8, 64, 1, 28526, 1, 0, 88, 1, 116, 1, 1, 88, 1, 2231, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2251, 62, 8, 1, 1, 25, 259, 1, 4583, 63, 1, 4571, 63, 8, 64, 8, 2, 1, 9, 8, 64, 1, 24941, 1, 0, 88, 1, 112, 1, 1, 88, 1, 2273, 1, 863, 63, 64, 1, 0, 8, 9, 5, 1, 3, 30, 6, 1, 2, 25, 259, 1, 3, 30, 41, 1, 2350, 62, 5, 1, 1, 25, 259, 8, 1, 2, 25, 259, 1, 1, 25, 259, 9, 6, 1, 3, 30, 1, 2342, 62, 6, 1, 1, 25, 259, 1, 3, 1, 4280, 63, 1, 3, 7, 8, 9, 1, 5607, 63, 20, 1, 2, 25, 259, 9, 1, 2308, 61, 9, 2, 10, 1, 4331, 63, 11, 64, 1, 421, 335, 2, 1, 51, 8, 64, 1, 26982, 1, 0, 88, 1, 29804, 1, 1, 88, 1, 29285, 1, 2, 88, 1, 0, 1, 3, 88, 1, 2384, 1, 863, 63, 64, 1, 0, 8, 9, 5, 1, 3, 30, 6, 1, 2, 25, 259, 1, 3, 30, 41, 1, 2491, 62, 5, 1, 1, 25, 259, 8, 1, 2, 25, 259, 1, 1, 25, 259, 9, 6, 1, 719, 63, 6, 1, 3, 30, 1, 2483, 62, 6, 1, 1, 25, 259, 1, 3, 1, 4280, 63, 1, 3, 7, 8, 9, 1, 5607, 63, 6, 1, 4583, 63, 1, 2472, 62, 8, 2, 6, 1, 1, 25, 259, 8, 20, 1, 2, 25, 259, 9, 1, 2419, 61, 8, 2, 8, 1, 2, 25, 259, 8, 1, 2419, 61, 9, 2, 10, 1, 4331, 63, 11, 64, 1, 436, 335, 2, 1, 51, 8, 64, 1, 25970, 1, 0, 88, 1, 30052, 1, 1, 88, 1, 25955, 1, 2, 88, 1, 0, 1, 3, 88, 1, 2525, 1, 863, 63, 64, 8, 5, 1, 3, 30, 6, 1, 2, 25, 259, 1, 3, 30, 41, 6, 1, 2, 25, 259, 1, 2, 25, 259, 1, 3, 30, 41, 1, 2637, 62, 5, 1, 1, 25, 259, 6, 1, 2, 25, 259, 1, 1, 25, 259, 9, 1, 2, 25, 259, 1, 2, 25, 259, 1, 1, 25, 259, 20, 9, 9, 6, 1, 3, 30, 1, 2632, 62, 1, 0, 1, 4, 7, 1, 3, 7, 1, 1, 25, 259, 1, 3, 1, 4331, 63, 1, 3, 7, 8, 9, 1, 5607, 63, 10, 10, 1, 2, 25, 259, 9, 2, 11, 9, 9, 11, 1, 2585, 61, 8, 2, 8, 2, 64, 1, 453, 335, 2, 1, 51, 8, 64, 1, 28516, 1, 0, 88, 1, 0, 1, 1, 88, 1, 2661, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2688, 62, 6, 1, 2, 25, 259, 1, 3, 30, 1, 2689, 62, 8, 1, 2, 25, 259, 8, 1, 2661, 61, 64, 8, 1, 1, 25, 259, 8, 64, 1, 30309, 1, 0, 88, 1, 27745, 1, 1, 88, 1, 0, 1, 2, 88, 1, 2717, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2734, 62, 8, 1, 1, 25, 259, 8, 1, 5420, 63, 64, 1, 470, 335, 1, 87, 335, 1, 10, 75, 8, 2, 1, 51, 8, 64, 1, 26988, 1, 0, 88, 1, 29811, 1, 1, 88, 1, 0, 1, 2, 88, 1, 2770, 1, 863, 63, 64, 64, 1, 28769, 1, 0, 88, 1, 27760, 1, 1, 88, 1, 121, 1, 2, 88, 1, 2792, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2, 7, 1, 2, 25, 259, 1, 3, 30, 41, 1, 2829, 62, 6, 1, 1, 25, 259, 9, 1, 2, 25, 259, 1, 1, 25, 259, 9, 1, 5607, 63, 64, 1, 488, 335, 1, 84, 335, 1, 10, 75, 8, 2, 1, 51, 8, 64, 1, 28524, 1, 0, 88, 1, 25697, 1, 1, 88, 1, 0, 1, 2, 88, 1, 2865, 1, 863, 63, 64, 107, 84, 1, 6402, 63, 104, 1, 2888, 62, 1, 6334, 63, 1, 2, 3, 1, 5420, 63, 2, 2, 1, 2867, 61, 108, 105, 2, 2, 2, 1, 3, 1, 2, 3, 64, 1, 30052, 1, 0, 88, 1, 28781, 1, 1, 88, 1, 0, 1, 2, 88, 1, 2920, 1, 863, 63, 64, 1, 108, 335, 67, 69, 1, 10, 75, 1, 115, 335, 1, 0, 10, 12, 1, 20, 29, 1, 0, 30, 1, 0, 30, 12, 1, 0, 30, 41, 1, 2955, 62, 1, 121, 335, 12, 259, 69, 1, 32, 75, 12, 1, 0, 3, 30, 1, 2977, 62, 11, 1, 1, 25, 10, 1, 2934, 61, 11, 2, 1, 128, 335, 0, 1, 25970, 1, 0, 88, 1, 25697, 1, 1, 88, 1, 0, 1, 2, 88, 1, 3004, 1, 863, 63, 64, 84, 1, 6402, 63, 1, 6334, 63, 5, 1, 51, 30, 1, 3024, 62, 8, 2, 9, 2, 8, 64, 86, 2, 8, 2, 9, 2, 8, 64, 1, 25970, 1, 0, 88, 1, 25697, 1, 1, 88, 1, 28268, 1, 2, 88, 1, 0, 1, 3, 88, 1, 3058, 1, 863, 63, 64, 1, 7164, 63, 8, 2, 9, 2, 8, 64, 1, 25970, 1, 0, 88, 1, 25697, 1, 1, 88, 1, 28205, 1, 2, 88, 1, 28021, 1, 3, 88, 1, 3093, 1, 863, 63, 64, 84, 1, 6402, 63, 5, 1, 6731, 63, 1, 0, 30, 1, 3123, 62, 1, 6762, 63, 8, 2, 8, 9, 2, 6, 1, 51, 30, 1, 3143, 62, 64, 1, 202, 335, 5, 75, 1, 40, 75, 69, 1, 41, 75, 1, 10, 75, 1, 51, 8, 9, 2, 86, 2, 64, 1, 29296, 1, 0, 88, 1, 110, 1, 1, 88, 1, 3162, 1, 863, 63, 64, 1, 1, 1, 79, 260, 8, 5, 1, 3, 30, 1, 3179, 62, 1, 1, 25, 259, 1, 5015, 63, 1, 3, 8, 64, 1, 29296, 1, 0, 88, 1, 27758, 1, 1, 88, 1, 110, 1, 2, 88, 1, 3207, 1, 863, 63, 64, 1, 3162, 63, 1, 10, 75, 64, 1, 29296, 1, 0, 88, 1, 28265, 1, 1, 88, 1, 116, 1, 2, 88, 1, 3235, 1, 863, 63, 64, 1, 0, 1, 79, 260, 8, 5, 1, 3, 30, 1, 3252, 62, 1, 1, 25, 259, 1, 5015, 63, 1, 1, 1, 79, 260, 1, 3, 8, 64, 1, 29296, 1, 0, 88, 1, 28265, 1, 1, 88, 1, 27764, 1, 2, 88, 1, 110, 1, 3, 88, 1, 3290, 1, 863, 63, 64, 1, 3235, 63, 1, 10, 75, 64, 1, 28515, 1, 0, 88, 1, 28277, 1, 1, 88, 1, 116, 1, 2, 88, 1, 3318, 1, 863, 63, 64, 8, 13, 0, 0, 22, 5, 1, 3, 30, 1, 3354, 62, 1, 1, 25, 259, 5, 1, 3, 30, 1, 3354, 62, 23, 13, 0, 1, 43, 22, 1, 2, 25, 259, 1, 3334, 61, 2, 23, 1, 4469, 63, 8, 64, 1, 24948, 1, 0, 88, 1, 25963, 1, 1, 88, 1, 0, 1, 2, 88, 1, 3382, 1, 863, 63, 64, 8, 5, 1, 1, 25, 259, 1, 1, 25, 271, 22, 1, 2, 25, 259, 1, 1, 25, 259, 1, 0, 8, 5, 1, 3, 30, 1, 3440, 62, 24, 13, 0, 0, 48, 8, 2, 1, 3447, 62, 23, 13, 0, 1, 44, 22, 5, 1, 1, 25, 259, 8, 1, 2, 25, 259, 1, 3404, 61, 23, 14, 1, 4331, 63, 8, 64, 23, 14, 2, 1, 3, 1, 4331, 63, 8, 64, 1, 29284, 1, 0, 88, 1, 28783, 1, 1, 88, 1, 0, 1, 2, 88, 1, 3478, 1, 863, 63, 64, 8, 5, 1, 3, 30, 6, 1, 2, 25, 259, 1, 3, 30, 41, 1, 3545, 62, 5, 1, 1, 25, 259, 1, 1, 25, 271, 22, 1, 2, 25, 259, 1, 1, 25, 259, 5, 1, 3, 30, 24, 13, 0, 0, 48, 8, 2, 41, 1, 3541, 62, 23, 13, 0, 1, 44, 22, 1, 2, 25, 259, 1, 3513, 61, 23, 14, 8, 64, 1, 506, 335, 1, 2, 69, 1, 10, 75, 2, 1, 51, 8, 64, 64, 1, 29806, 1, 0, 88, 1, 104, 1, 1, 88, 1, 3576, 1, 863, 63, 64, 1, 3478, 63, 8, 1, 0, 8, 1, 3, 1, 4331, 63, 8, 1, 1156, 63, 64, 1, 28531, 1, 0, 88, 1, 29810, 1, 1, 88, 1, 0, 1, 2, 88, 1, 3614, 1, 863, 63, 64, 6, 1, 3, 30, 1, 2, 7, 1, 1, 25, 259, 259, 1, 8, 39, 1, 0, 30, 1, 0, 30, 41, 1, 3692, 62, 6, 1, 2, 25, 259, 1, 3, 30, 1, 3701, 62, 6, 1, 1, 25, 259, 1, 0, 8, 1, 3, 1, 4331, 63, 8, 1, 3318, 63, 8, 9, 5, 1, 1, 25, 259, 8, 1, 2, 25, 259, 1, 1, 25, 259, 8, 9, 1, 3756, 63, 8, 2, 8, 64, 1, 544, 335, 8, 2, 1, 51, 8, 64, 6, 8, 1, 3318, 63, 1, 2, 7, 1, 1, 25, 259, 9, 1, 3728, 63, 9, 9, 1, 3756, 63, 8, 2, 8, 9, 2, 64, 1, 25964, 1, 0, 88, 1, 29555, 1, 1, 88, 1, 0, 1, 2, 88, 1, 1846, 1, 875, 63, 8, 6, 1, 891, 63, 8, 2, 64, 5, 1, 1, 25, 271, 13, 0, 0, 48, 8, 2, 1, 3882, 62, 5, 1, 1, 25, 271, 13, 0, 1, 48, 8, 2, 1, 3884, 62, 9, 10, 5, 1, 1, 25, 271, 13, 0, 2, 46, 1, 4469, 63, 8, 6, 8, 1, 0, 8, 9, 1, 3, 1, 4331, 63, 8, 1, 1536, 63, 9, 5, 1, 3, 7, 8, 1, 0, 9, 9, 1, 3, 1, 4331, 63, 8, 1, 3382, 63, 1, 3, 7, 1, 0, 8, 9, 1, 3, 1, 4331, 63, 8, 1, 3478, 63, 20, 9, 9, 11, 9, 9, 1, 3756, 63, 10, 9, 9, 1, 3756, 63, 1, 0, 1, 2, 7, 9, 11, 1, 3, 1, 4331, 63, 8, 1, 3922, 63, 8, 64, 2, 64, 2, 5, 1, 1, 25, 259, 1, 0, 8, 1, 3, 1, 4331, 63, 8, 2, 64, 1, 25965, 1, 0, 88, 1, 26482, 1, 1, 88, 1, 101, 1, 2, 88, 1, 3922, 1, 863, 63, 64, 6, 1, 1, 25, 259, 1, 2, 7, 1, 2, 25, 259, 1, 1, 25, 259, 1, 3, 7, 1, 2, 25, 259, 1, 2, 25, 259, 1, 1, 25, 259, 5, 1, 3, 30, 1, 4027, 62, 6, 1, 3, 30, 1, 4034, 62, 1, 4042, 63, 10, 12, 6, 1, 3, 30, 1, 4003, 62, 1, 2, 7, 1, 3, 30, 1, 4015, 62, 10, 1, 4042, 63, 11, 6, 6, 1, 2, 25, 260, 2, 1, 3972, 61, 8, 2, 1, 2, 25, 260, 2, 8, 2, 11, 8, 64, 9, 2, 1, 2, 25, 260, 2, 8, 2, 11, 8, 64, 2, 8, 2, 9, 2, 8, 64, 8, 2, 8, 2, 9, 2, 8, 64, 1, 0, 1, 3, 7, 1, 3, 7, 1, 1, 25, 259, 1, 3, 7, 1, 1, 25, 259, 1, 3, 1, 4331, 63, 1, 4, 7, 1, 5420, 63, 2, 1, 9, 30, 1, 4093, 62, 5, 1, 2, 25, 259, 8, 1, 3, 6, 1, 2, 25, 260, 64, 6, 1, 2, 25, 259, 8, 9, 1, 3, 6, 1, 2, 25, 260, 64, 1, 1280, 6, 260, 1, 3, 6, 1, 1, 25, 260, 1, 3, 6, 1, 2, 25, 260, 1, 3, 6, 1, 3, 25, 260, 64, 8, 1, 8, 38, 6, 259, 1, 255, 40, 41, 8, 260, 64, 8, 6, 259, 1, -256, 40, 41, 8, 260, 64, 5, 259, 1, 8, 39, 1, 8, 30, 8, 259, 1, 8, 39, 1, 9, 30, 41, 64, 1, 4979, 63, 1, 8, 6, 1, 4134, 63, 1, 0, 6, 1, 4147, 63, 1, 3, 6, 1, 1, 25, 260, 64, 1, 4979, 63, 1, 8, 6, 1, 4134, 63, 8, 6, 1, 4147, 63, 1, 3, 6, 1, 1, 25, 260, 64, 1, 4979, 63, 1, 9, 6, 1, 4134, 63, 8, 6, 1, 4147, 63, 1, 3, 6, 1, 1, 25, 260, 64, 1, 0, 6, 260, 1, 3, 6, 1, 1, 25, 260, 1, 3, 6, 1, 2, 25, 260, 64, 1, 256, 6, 260, 1, 3, 6, 1, 1, 25, 260, 1, 3, 6, 1, 2, 25, 260, 64, 1, 4991, 63, 1, 0, 6, 260, 6, 1, 3, 30, 1, 4308, 62, 6, 1, 6282, 63, 1, 2, 7, 1, 4157, 63, 41, 1, 4321, 62, 8, 6, 1, 2, 25, 260, 8, 6, 1, 1, 25, 260, 64, 8, 1, 3, 1, 4280, 63, 8, 1, 4308, 61, 6, 1, 0, 30, 1, 4344, 62, 1, 4280, 63, 1, 4331, 61, 8, 2, 64, 5, 259, 1, 8, 39, 1, 3, 30, 8, 259, 1, 8, 39, 1, 4, 30, 41, 64, 6, 259, 1, 8, 39, 1, 3, 30, 6, 259, 1, 8, 39, 1, 3, 30, 40, 64, 6, 259, 1, 8, 39, 1, 4, 30, 6, 259, 1, 8, 39, 1, 4, 30, 40, 64, 5, 259, 1, 8, 39, 1, 3, 30, 6, 259, 1, 8, 39, 1, 4, 30, 41, 1, 2, 7, 259, 1, 8, 39, 1, 3, 30, 1, 3, 7, 259, 1, 8, 39, 1, 4, 30, 41, 40, 64, 1, 768, 6, 260, 9, 9, 1, 2, 7, 1, 1, 25, 272, 64, 1, 1024, 6, 260, 9, 9, 1, 2, 7, 1, 1, 25, 272, 64, 1, 4991, 63, 1, 768, 6, 260, 9, 9, 1, 2, 7, 1, 1, 25, 272, 64, 1, 4991, 63, 1, 1024, 6, 260, 9, 9, 1, 2, 7, 1, 1, 25, 272, 64, 5, 259, 1, 8, 39, 1, 3, 30, 1, 4523, 62, 1, 1, 25, 271, 13, 0, 1000, 46, 64, 1, 1, 25, 271, 64, 5, 259, 1, 8, 39, 1, 4, 30, 1, 4548, 62, 1, 1, 25, 271, 13, 0, 1000, 45, 64, 1, 1, 25, 271, 64, 6, 259, 1, 8, 39, 1, 2, 30, 6, 259, 1, 8, 39, 1, 2, 30, 40, 64, 1, 0, 30, 1, 4580, 62, 1, 9, 64, 1, 15, 64, 5, 1, 15, 30, 8, 1, 3, 30, 41, 64, 1, 512, 6, 260, 1, 0, 6, 1, 5, 25, 260, 1, 0, 6, 1, 1, 25, 260, 64, 1, 0, 10, 6, 12, 25, 259, 6, 6, 8, 1, 1, 25, 12, 25, 260, 92, 9, 2, 1, 0, 30, 8, 1, 0, 30, 41, 1, 4657, 62, 11, 1, 1, 25, 10, 12, 1, 4, 31, 1, 4615, 62, 1, 4681, 61, 11, 1, 1, 25, 10, 12, 1, 4, 30, 1, 4681, 62, 1, 0, 6, 1, 1, 25, 12, 25, 260, 1, 4657, 61, 11, 2, 2, 2, 64, 1, 1, 25, 1, 0, 10, 5, 12, 25, 259, 92, 9, 2, 5, 1, 0, 30, 1, 4731, 62, 75, 5, 1, 0, 30, 1, 4732, 62, 75, 11, 1, 1, 25, 10, 12, 1, 4, 31, 1, 4692, 62, 2, 11, 2, 64, 2, 2, 2, 11, 2, 64, 1, 0, 10, 6, 12, 25, 259, 6, 12, 25, 259, 6, 30, 1, 0, 30, 1, 4789, 62, 92, 9, 2, 1, 0, 30, 8, 1, 0, 30, 41, 1, 4782, 62, 11, 1, 1, 25, 10, 12, 1, 4, 31, 1, 4740, 62, 2, 2, 11, 2, 1, 1, 64, 2, 2, 2, 11, 2, 1, 0, 64, 1, 0, 30, 1, 4806, 62, 1, 9, 64, 1, 15, 64, 1, 3840, 6, 260, 1, 3, 6, 1, 1, 25, 260, 1, 3, 6, 1, 2, 25, 260, 1, 0, 6, 1, 3, 25, 260, 64, 1, 2, 3, 1, 4845, 63, 1, 2, 4, 64, 1, 4997, 63, 1, 4809, 63, 9, 6, 1, 2, 25, 260, 9, 6, 1, 1, 25, 260, 8, 6, 1, 3, 25, 260, 64, 5, 1, 0, 30, 1, 4900, 62, 5, 1, 1, 25, 259, 1, 2, 7, 30, 1, 4896, 62, 1, 3, 25, 259, 1, 4870, 61, 1, 2, 25, 259, 8, 2, 64, 1, 1, 3, 6, 1, 5, 25, 260, 1, 1, 4, 64, 1, 1, 3, 6, 6, 1, 1, 25, 1, 4737, 63, 1, 4943, 62, 1, 5, 25, 259, 5, 1, 0, 30, 1, 4943, 62, 1, 4918, 61, 8, 2, 64, 68, 1, 4, 25, 1, 0, 4, 1, 57, 1, 1, 4, 1, 75, 1, 2, 4, 1, 0, 1, 3, 4, 64, 1, 0, 3, 8, 6, 25, 1, 0, 4, 64, 1, 2, 1, 4969, 63, 64, 1, 3, 1, 4969, 63, 64, 1, 3, 1, 4969, 63, 64, 1, 4, 1, 4969, 63, 64, 1, 4, 1, 4969, 63, 64, 1, 6, 1, 4969, 63, 64, 5, 259, 1, 8, 39, 1, 3, 30, 1, 5092, 62, 5, 259, 1, 8, 39, 1, 4, 30, 1, 5098, 62, 5, 1, 4157, 63, 1, 5104, 62, 5, 259, 1, 8, 39, 1, 0, 30, 1, 5108, 62, 5, 259, 1, 8, 39, 1, 1, 30, 1, 5112, 62, 5, 259, 1, 8, 39, 1, 5, 30, 1, 5116, 62, 5, 259, 1, 8, 39, 1, 6, 30, 1, 5120, 62, 1, 4686, 63, 64, 1, 1, 25, 271, 70, 64, 1, 1, 25, 271, 73, 64, 1, 5289, 63, 64, 1, 5124, 63, 64, 1, 5189, 63, 64, 1, 5236, 63, 64, 1, 5266, 63, 64, 1, 40, 75, 5, 1, 1, 25, 259, 1, 5015, 63, 5, 259, 1, 8, 39, 1, 1, 30, 1, 5168, 62, 1, 2, 25, 259, 5, 1, 3, 30, 1, 5163, 62, 1, 32, 75, 1, 5127, 61, 2, 1, 41, 75, 64, 1, 2, 25, 259, 5, 1, 3, 30, 1, 5163, 62, 1, 32, 75, 1, 5015, 63, 1, 41, 75, 64, 1, 40, 75, 5, 1, 1, 25, 259, 1, 5015, 63, 5, 1, 2, 25, 259, 1, 3, 30, 1, 5231, 62, 1, 32, 75, 1, 46, 75, 1, 32, 75, 1, 2, 25, 259, 1, 5015, 63, 1, 41, 75, 64, 2, 1, 41, 75, 64, 1, 35, 75, 1, 40, 75, 5, 1, 1, 25, 259, 1, 5015, 63, 1, 32, 75, 5, 1, 2, 25, 259, 1, 5015, 63, 1, 41, 75, 2, 64, 1, 35, 75, 5, 1, 2, 25, 259, 1, 5015, 63, 1, 123, 75, 1, 1, 25, 259, 69, 1, 125, 75, 64, 1, 79, 259, 1, 1, 30, 1, 5302, 62, 1, 5398, 63, 64, 1, 34, 75, 1, 5312, 63, 1, 34, 75, 64, 5, 1, 3, 30, 1, 5330, 62, 5, 1, 5332, 63, 1, 1, 25, 259, 1, 5312, 61, 2, 64, 5, 259, 1, 8, 39, 1, 9, 30, 1, 5356, 62, 259, 1, 255, 40, 5, 1, 0, 30, 1, 5396, 62, 75, 64, 259, 1, 255, 40, 5, 1, 0, 30, 1, 5396, 62, 1, 92, 75, 5, 1, 10, 30, 1, 5386, 62, 5, 1, 9, 30, 1, 5391, 62, 75, 64, 2, 1, 110, 75, 64, 2, 1, 116, 75, 64, 2, 64, 5, 1, 3, 30, 1, 5418, 62, 5, 259, 1, 255, 40, 75, 1, 1, 25, 259, 1, 5398, 61, 2, 64, 6, 1, 4347, 63, 1, 2, 7, 259, 1, 8, 39, 1, 5, 30, 41, 1, 2, 7, 259, 1, 8, 39, 1, 6, 30, 41, 1, 2, 7, 1, 4157, 63, 41, 1, 5467, 62, 6, 1, 6282, 63, 1, 5468, 62, 1, 5499, 63, 64, 64, 6, 6, 1, 4870, 63, 5, 1, 0, 30, 1, 5484, 62, 9, 2, 8, 64, 2, 1, 290, 335, 8, 1, 4686, 63, 1, 10, 75, 1, 51, 8, 64, 6, 1, 1, 25, 259, 5, 1, 5720, 63, 1, 5559, 62, 8, 1, 5420, 63, 6, 1, 51, 30, 1, 5606, 62, 6, 259, 1, 8, 39, 1, 5, 30, 1, 5563, 62, 6, 259, 1, 8, 39, 1, 6, 30, 1, 5583, 62, 1, 342, 335, 6, 1, 719, 63, 9, 2, 9, 2, 1, 51, 8, 64, 1, 5747, 63, 64, 9, 1, 2, 25, 259, 8, 1, 5661, 63, 6, 1, 51, 30, 1, 5606, 62, 1, 6199, 63, 64, 9, 1, 2, 25, 259, 8, 1, 5661, 63, 6, 1, 51, 30, 1, 5606, 62, 9, 1, 1, 25, 259, 63, 64, 64, 1, 2, 7, 259, 1, 8, 39, 1, 5, 30, 1, 5650, 62, 1, 2, 7, 259, 1, 8, 39, 1, 6, 30, 1, 5654, 62, 1, 342, 335, 1, 2, 7, 1, 719, 63, 8, 2, 8, 2, 1, 51, 8, 64, 1, 6199, 63, 64, 9, 1, 1, 25, 259, 63, 64, 1, 0, 9, 9, 6, 1, 3, 30, 1, 5697, 62, 6, 1, 1, 25, 259, 8, 1, 5420, 63, 6, 1, 51, 30, 1, 5703, 62, 9, 1, 2, 25, 259, 8, 1, 5665, 61, 10, 1, 4331, 63, 11, 64, 1, 2, 7, 1, 0, 30, 1, 5717, 62, 9, 2, 1, 5703, 61, 9, 2, 64, 5, 1, 27, 30, 6, 1, 33, 30, 41, 6, 1, 45, 30, 41, 6, 1, 39, 30, 41, 6, 1, 21, 30, 41, 8, 2, 64, 5, 1, 21, 30, 1, 5792, 62, 5, 1, 27, 30, 1, 5804, 62, 5, 1, 33, 30, 1, 5815, 62, 5, 1, 45, 30, 1, 5820, 62, 5, 1, 39, 30, 1, 5825, 62, 1, 381, 335, 1, 4686, 63, 1, 10, 75, 0, 2, 8, 1, 2, 25, 259, 1, 1, 25, 259, 8, 64, 2, 8, 1, 2, 25, 259, 8, 1, 5830, 63, 64, 2, 1, 5898, 63, 64, 2, 1, 6044, 63, 64, 2, 1, 6148, 63, 64, 6, 1, 3, 30, 1, 5876, 62, 6, 1, 1, 25, 259, 1, 1, 25, 259, 8, 1, 5420, 63, 6, 1, 51, 30, 1, 5895, 62, 8, 1, 4583, 63, 1, 0, 30, 1, 5877, 62, 8, 1, 2, 25, 259, 8, 1, 5830, 61, 64, 8, 1, 1, 25, 259, 1, 2, 25, 259, 1, 1, 25, 259, 8, 1, 5420, 63, 64, 9, 2, 64, 6, 1, 2, 25, 259, 1, 1, 25, 259, 5, 259, 1, 8, 39, 1, 2, 30, 1, 0, 30, 1, 5989, 62, 5, 1, 2, 7, 1, 4870, 63, 1, 0, 30, 1, 0, 30, 1, 6001, 62, 9, 1, 2, 25, 259, 1, 2, 25, 259, 1, 1, 25, 259, 9, 1, 5420, 63, 6, 1, 51, 30, 1, 6041, 62, 1, 2, 7, 1, 2, 7, 1, 4835, 63, 6, 259, 1, 8, 39, 1, 5, 30, 1, 6016, 62, 8, 2, 8, 2, 1, 9, 8, 64, 1, 326, 335, 1, 719, 63, 8, 2, 1, 51, 8, 64, 1, 307, 335, 1, 4686, 63, 1, 10, 75, 8, 2, 1, 51, 8, 64, 1, 2, 7, 1, 2, 7, 5, 1, 3, 25, 259, 1, 4845, 63, 9, 1, 3, 25, 260, 1, 9, 8, 9, 2, 64, 9, 2, 64, 8, 6, 6, 1, 2, 25, 259, 1, 1, 25, 259, 8, 1, 6091, 63, 6, 1, 51, 30, 1, 6086, 62, 8, 1, 2, 25, 259, 1, 2, 25, 259, 1, 1, 25, 259, 8, 1, 5420, 63, 2, 8, 64, 9, 2, 9, 2, 64, 6, 1, 3, 30, 1, 6140, 62, 6, 1, 1, 25, 259, 5, 1, 1, 25, 259, 8, 1, 2, 25, 259, 1, 1, 25, 259, 9, 1, 5420, 63, 6, 1, 51, 30, 1, 6143, 62, 1, 4845, 63, 8, 1, 2, 25, 259, 8, 1, 6091, 61, 8, 2, 64, 9, 2, 9, 2, 64, 6, 1, 2, 25, 259, 1, 1, 25, 259, 9, 1, 2, 25, 259, 1, 2, 25, 259, 1, 1, 25, 259, 1, 2, 7, 1, 5003, 63, 1, 4108, 63, 8, 6, 1, 3, 25, 260, 8, 6, 1, 2, 25, 260, 8, 6, 1, 1, 25, 260, 8, 64, 8, 1, 2, 7, 1, 1, 25, 259, 1, 3, 7, 1, 3, 25, 259, 1, 6233, 63, 1, 2, 7, 1, 2, 25, 259, 8, 1, 5420, 63, 2, 8, 9, 2, 64, 6, 1, 3, 30, 1, 3, 7, 1, 3, 30, 41, 1, 6277, 62, 6, 1, 1, 25, 259, 1, 3, 7, 1, 1, 25, 259, 9, 1, 4845, 63, 9, 1, 2, 25, 259, 9, 1, 2, 25, 259, 9, 1, 6233, 61, 8, 2, 8, 2, 64, 5, 259, 1, 8, 39, 1, 2, 30, 6, 259, 1, 8, 39, 1, 3, 30, 41, 6, 259, 1, 8, 39, 1, 4, 30, 41, 8, 2, 64, 84, 1, 6402, 63, 1, 6334, 63, 8, 6, 1, 51, 30, 8, 1, 10, 30, 41, 1, 6333, 62, 86, 2, 64, 5, 1, 34, 30, 1, 6383, 62, 5, 1, 40, 30, 1, 6391, 62, 5, 1, 6731, 63, 1, 6387, 62, 5, 1, 40, 30, 1, 6391, 62, 5, 1, 41, 30, 1, 6395, 62, 5, 1, 6418, 63, 1, 6398, 62, 5, 1, 7195, 63, 1, 51, 64, 1, 7071, 63, 64, 1, 6762, 63, 64, 1, 6634, 63, 64, 1, -1, 64, 1, 6502, 63, 64, 5, 1, 6463, 63, 1, 0, 30, 1, 6417, 62, 2, 84, 1, 6402, 61, 64, 5, 1, 39, 30, 6, 1, 34, 30, 41, 6, 1, 123, 30, 41, 6, 1, 125, 30, 41, 6, 1, 91, 30, 41, 6, 1, 93, 30, 41, 6, 1, 46, 30, 41, 6, 1, 59, 30, 41, 1, 0, 30, 8, 2, 64, 5, 1, 32, 30, 6, 1, 10, 30, 41, 6, 1, 9, 30, 41, 6, 1, 44, 30, 41, 8, 2, 64, 5, 1, 40, 30, 6, 1, 41, 30, 41, 6, 1, 6463, 63, 41, 8, 2, 64, 1, 6540, 63, 1, 0, 30, 1, 6537, 62, 67, 1, 4915, 63, 5, 1, 6536, 62, 2, 1, 5009, 63, 1, 4593, 63, 67, 6, 1, 4612, 63, 5, 1, 4903, 63, 64, 64, 1, 51, 64, 1, 0, 10, 11, 1, 1, 25, 10, 84, 5, 1, 6418, 63, 1, 0, 30, 1, 6596, 62, 5, 1, 6485, 63, 1, 6604, 62, 12, 1, 744, 63, 1, 6581, 62, 11, 1, 1, 25, 10, 1, 6548, 61, 8, 93, 12, 1, 2, 28, 88, 11, 1, 1, 25, 10, 1, 6548, 61, 1, 7173, 63, 11, 2, 1, 51, 64, 12, 1, 744, 63, 1, 6623, 62, 1, 0, 12, 1, 2, 28, 88, 11, 2, 1, 9, 64, 8, 12, 1, 2, 28, 88, 11, 2, 1, 9, 64, 2, 84, 1, 6402, 63, 1, 6334, 63, 5, 1, -1, 30, 1, 6714, 62, 5, 1, 51, 30, 1, 6730, 62, 1, 4991, 63, 1, 4242, 63, 10, 12, 1, 1, 25, 260, 12, 8, 1, 6402, 63, 1, 6334, 63, 5, 1, -1, 30, 1, 6718, 62, 5, 1, 51, 30, 1, 6730, 62, 9, 8, 1, 4991, 63, 1, 4242, 63, 5, 9, 8, 1, 1, 25, 260, 5, 9, 1, 2, 25, 260, 1, 6669, 61, 2, 1, 3, 64, 2, 2, 84, 1, 3, 9, 1, 2, 25, 260, 11, 64, 64, 5, 1, 45, 30, 6, 1, 43, 30, 41, 8, 1, 6746, 63, 41, 64, 5, 1, 48, 1, 1, 26, 32, 8, 1, 57, 1, 1, 25, 31, 40, 64, 1, 6793, 63, 5, 1, 51, 30, 1, 6792, 62, 1, -2, 30, 1, 6785, 62, 1, 4991, 63, 1, 4455, 63, 64, 1, 4991, 63, 1, 4441, 63, 64, 64, 1, 0, 10, 1, 6984, 63, 11, 25, 10, 84, 5, 1, 46, 30, 1, 6847, 62, 5, 1, 6485, 63, 6, 1, 6463, 63, 41, 1, 6935, 62, 5, 1, 6746, 63, 1, 0, 30, 1, 6963, 62, 1, 10, 12, 30, 1, 6952, 62, 11, 1, 1, 25, 10, 1, 6802, 61, 2, 1, 0, 10, 84, 5, 1, 6485, 63, 6, 1, 6463, 63, 41, 1, 6900, 62, 5, 1, 6746, 63, 1, 0, 30, 1, 6963, 62, 1, 3, 12, 30, 1, 6971, 62, 11, 12, 6, 10, 25, 1, 10, 30, 1, 6950, 62, 11, 1, 1, 25, 10, 1, 6851, 61, 1, 3, 12, 30, 1, 6918, 62, 1, 48, 8, 11, 1, 1, 25, 10, 1, 6900, 61, 11, 11, 25, 1, 1, 26, 8, 10, 1, 7019, 63, 11, 9, 9, 1, -3, 64, 11, 1, 1, 26, 8, 10, 1, 7019, 63, 11, 9, 9, 1, -2, 64, 11, 2, 1, 219, 335, 1, 10, 75, 11, 2, 1, 51, 64, 1, 7217, 63, 11, 2, 1, 51, 64, 1, 255, 335, 1, 10, 75, 11, 2, 11, 2, 1, 51, 64, 5, 1, 45, 30, 1, 7005, 62, 5, 1, 43, 30, 1, 7012, 62, 13, 0, 1, 9, 1, 1, 64, 2, 13, -1, -1, 1, 0, 64, 2, 13, 0, 1, 1, 0, 64, 5, 10, 13, 0, 0, 20, 8, 1, 48, 26, 6, 12, 26, 1, 2, 27, 1, 131, 25, 271, 9, 1, 0, 8, 45, 9, 10, 43, 11, 9, 9, 1, 0, 12, 30, 1, 7065, 62, 11, 1, 1, 26, 10, 1, 7024, 61, 9, 2, 45, 11, 2, 64, 1, 7120, 63, 84, 8, 64, 5, 1, 92, 30, 1, 7088, 62, 1, 4198, 63, 64, 2, 84, 1, 7097, 63, 1, 4220, 63, 64, 5, 1, 110, 30, 1, 7112, 62, 5, 1, 116, 30, 1, 7116, 62, 64, 2, 1, 10, 64, 2, 1, 9, 64, 84, 6, 6, 30, 1, 7153, 62, 1, 7077, 63, 10, 12, 84, 1, 2, 7, 6, 30, 1, 7159, 62, 1, 7077, 63, 5, 9, 1, 1, 25, 260, 1, 7132, 61, 2, 2, 1, 4175, 63, 64, 2, 2, 2, 11, 64, 1, 10, 1, 7120, 63, 1, 10, 8, 64, 1, 168, 335, 1, 39, 75, 5, 75, 1, 39, 75, 1, 40, 75, 69, 1, 41, 75, 1, 10, 75, 64, 1, 151, 335, 1, 39, 75, 5, 75, 1, 39, 75, 1, 40, 75, 69, 1, 41, 75, 1, 10, 75, 64, 1, 185, 335, 1, 39, 75, 5, 75, 1, 39, 75, 1, 40, 75, 69, 1, 41, 75, 1, 10, 75, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30052, 28781, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16625, 16568, 16614, 0, 512, 28515, 29550, 0, 0, 57, 1536, 1095, 15506, 3840, 15506, 15512, 75, 512, 24931, 114, 0, 0, 15506, 1536, 1156, 15519, 3840, 15519, 15525, 15515, 512, 25699, 114, 0, 0, 15519, 1536, 1234, 15532, 3840, 15532, 15538, 15528, 512, 29029, 63, 0, 0, 15532, 1536, 1309, 15545, 3840, 15545, 15551, 15541, 512, 29793, 28015, 63, 0, 15545, 1536, 1414, 15558, 3840, 15558, 15564, 15554, 512, 25697, 100, 0, 0, 15558, 1536, 1449, 15571, 3840, 15571, 15577, 15567, 512, 30067, 98, 0, 0, 15571, 1536, 1536, 15584, 3840, 15584, 15590, 15580, 512, 30061, 29804, 0, 0, 15584, 1536, 1495, 15597, 3840, 15597, 15603, 15593, 512, 26980, 118, 0, 0, 15597, 1536, 1635, 15610, 3840, 15610, 15616, 15606, 512, 28525, 100, 0, 0, 15610, 1536, 1734, 15623, 3840, 15623, 15629, 15619, 512, 25964, 29555, 0, 0, 15623, 1536, 1846, 15636, 3840, 15636, 15642, 15632, 512, 29287, 24933, 25972, 114, 15636, 1536, 1987, 15649, 3840, 15649, 15655, 15645, 512, 28257, 100, 0, 0, 15649, 1536, 2118, 15662, 3840, 15662, 15668, 15658, 512, 29295, 0, 0, 0, 15662, 1536, 2173, 15675, 3840, 15675, 15681, 15671, 512, 28526, 116, 0, 0, 15675, 1536, 2231, 15688, 3840, 15688, 15694, 15684, 512, 24941, 112, 0, 0, 15688, 1536, 2273, 15701, 3840, 15701, 15707, 15697, 512, 26982, 29804, 29285, 0, 15701, 1536, 2384, 15714, 3840, 15714, 15720, 15710, 512, 25970, 30052, 25955, 0, 15714, 1536, 2525, 15727, 3840, 15727, 15733, 15723, 512, 28516, 0, 0, 0, 15727, 1536, 2661, 15740, 3840, 15740, 15746, 15736, 512, 30309, 27745, 0, 0, 15740, 1536, 2717, 15753, 3840, 15753, 15759, 15749, 512, 26988, 29811, 0, 0, 15753, 1536, 2770, 15766, 3840, 15766, 15772, 15762, 512, 28769, 27760, 121, 0, 15766, 1536, 2792, 15779, 3840, 15779, 15785, 15775, 512, 28524, 25697, 0, 0, 15779, 1536, 2865, 15792, 3840, 15792, 15798, 15788, 512, 30052, 28781, 0, 0, 15792, 1536, 2920, 15805, 3840, 15805, 15811, 15801, 512, 25970, 25697, 28205, 28021, 15805, 1536, 3093, 15818, 3840, 15818, 15824, 15814, 512, 25970, 25697, 0, 0, 15818, 1536, 3004, 15831, 3840, 15831, 15837, 15827, 512, 25970, 25697, 28268, 0, 15831, 1536, 3058, 15844, 3840, 15844, 15850, 15840, 512, 29296, 110, 0, 0, 15844, 1536, 3162, 15857, 3840, 15857, 15863, 15853, 512, 29296, 27758, 110, 0, 15857, 1536, 3207, 15870, 3840, 15870, 15876, 15866, 512, 29296, 28265, 116, 0, 15870, 1536, 3235, 15883, 3840, 15883, 15889, 15879, 512, 29296, 28265, 27764, 110, 15883, 1536, 3290, 15896, 3840, 15896, 15902, 15892, 512, 28515, 28277, 116, 0, 15896, 1536, 3318, 15909, 3840, 15909, 15915, 15905, 512, 29806, 104, 0, 0, 15909, 1536, 3576, 15922, 3840, 15922, 15928, 15918, 512, 24948, 25963, 0, 0, 15922, 1536, 3382, 15935, 3840, 15935, 15941, 15931, 512, 29284, 28783, 0, 0, 15935, 1536, 3478, 15948, 3840, 15948, 15954, 15944, 512, 28531, 29810, 0, 0, 15948, 1536, 3614, 15961, 3840, 15961, 15967, 15957, 512, 25965, 26482, 101, 0, 15961, 1536, 3922, 15974, 3840, 15974, 15980, 15970, 0, 33, 15996, 512, 25959, 11636, 26988, 29811, 15974, 0, 15990, 16080, 0, 39, 16011, 512, 110, 0, 0, 0, 15990, 0, 16002, 3, 0, 16008, 16077, 0, 27, 16035, 0, 15545, 16020, 0, 16002, 16026, 768, 0, 0, 0, 16023, 3, 0, 16017, 16032, 0, 3, 3, 0, 16029, 16074, 0, 9, 16071, 0, 15506, 16047, 0, 15831, 3, 0, 16044, 16068, 0, 15990, 16065, 0, 15584, 16056, 0, 16002, 16062, 768, 0, 1, 0, 16059, 3, 0, 16053, 3, 0, 16050, 3, 0, 16041, 3, 0, 16038, 3, 0, 16014, 3, 0, 15999, 3, 1280, 16008, 16014, 16091, 3840, 15990, 16083, 15983, 3840, 15990, 16083, 15983, 0, 33, 16104, 512, 25959, 11636, 25711, 100, 16002, 0, 16098, 16236, 0, 39, 16119, 512, 30055, 29541, 29556, 0, 16098, 0, 16110, 3, 0, 16116, 16233, 0, 15727, 16218, 0, 39, 16146, 512, 25441, 99, 0, 0, 16110, 0, 16128, 16143, 512, 120, 0, 0, 0, 16128, 0, 16137, 3, 0, 16134, 16215, 0, 15740, 16158, 0, 15883, 16155, 0, 16137, 3, 0, 16152, 16212, 0, 27, 16191, 0, 15545, 16167, 0, 16137, 16176, 0, 15519, 16173, 0, 16128, 3, 0, 16170, 3, 0, 16164, 16188, 0, 15532, 16185, 0, 16128, 3, 0, 16182, 3, 0, 16179, 16209, 0, 9, 16206, 0, 15506, 16200, 0, 16137, 16203, 0, 16128, 3, 0, 16197, 3, 0, 16194, 3, 0, 16161, 3, 0, 16149, 3, 0, 16125, 16221, 0, 3, 16230, 0, 15961, 16227, 0, 16110, 3, 0, 16224, 3, 0, 16122, 3, 0, 16107, 3, 1280, 16116, 16122, 16247, 3840, 16098, 16239, 16087, 3840, 16098, 16239, 16087, 0, 33, 16260, 512, 28531, 30316, 101, 0, 16137, 0, 16254, 16394, 0, 39, 16278, 512, 105, 0, 0, 0, 16254, 0, 16266, 16275, 0, 16110, 3, 0, 16272, 16391, 0, 15740, 16302, 0, 15883, 16299, 2115, 16289, 2145, 16291, 2163, 16293, 2149, 16295, 2080, 16297, 2083, 3, 0, 16287, 3, 0, 16284, 16311, 0, 15883, 16308, 0, 16266, 3, 0, 16305, 16324, 0, 15883, 16321, 2106, 16319, 2080, 3, 0, 16317, 3, 0, 16314, 16377, 0, 15701, 16371, 0, 39, 16342, 512, 97, 0, 0, 0, 16266, 0, 16333, 3, 0, 16339, 16368, 0, 15740, 16354, 0, 15883, 16351, 0, 16333, 3, 0, 16348, 16365, 0, 15883, 16362, 2080, 3, 0, 16360, 3, 0, 16357, 3, 0, 16345, 3, 0, 16330, 16374, 0, 16110, 3, 0, 16327, 16388, 0, 15883, 16385, 2314, 3, 0, 16383, 3, 0, 16380, 3, 0, 16281, 3, 0, 16263, 3, 1280, 16272, 16281, 16405, 3840, 16254, 16397, 16243, 3840, 16254, 16397, 16243, 0, 33, 16418, 512, 25972, 29811, 0, 0, 16333, 0, 16412, 16529, 0, 39, 16430, 0, 16002, 16427, 0, 16266, 3, 0, 16424, 16526, 0, 27, 16454, 0, 15545, 16439, 0, 16002, 16445, 768, 0, 0, 0, 16442, 3, 0, 16436, 16451, 0, 3, 3, 0, 16448, 16523, 0, 9, 16520, 0, 15740, 16481, 0, 16254, 16466, 0, 16266, 16478, 0, 15990, 16475, 0, 15831, 3, 0, 16472, 3, 0, 16469, 3, 0, 16463, 16517, 0, 16412, 16499, 0, 15584, 16490, 0, 16002, 16496, 768, 0, 1, 0, 16493, 3, 0, 16487, 16514, 0, 15571, 16505, 0, 16266, 16511, 768, 0, 1, 0, 16508, 3, 0, 16502, 3, 0, 16484, 3, 0, 16460, 3, 0, 16457, 3, 0, 16433, 3, 0, 16421, 3, 1280, 16424, 16433, 16540, 3840, 16412, 16532, 16401, 3840, 16412, 16532, 16401, 0, 33, 16547, 0, 57, 16607, 0, 39, 16562, 512, 95, 0, 0, 0, 16412, 0, 16553, 3, 0, 16559, 16604, 0, 45, 16586, 512, 78, 0, 0, 0, 16553, 0, 16568, 16580, 0, 15831, 3, 0, 16577, 3, 0, 16574, 3, 0, 16583, 16601, 0, 16412, 16592, 0, 16568, 16598, 768, 0, 1, 0, 16595, 3, 0, 16589, 3, 0, 16565, 3, 0, 16550, 3, 1280, 16559, 16565, 16618, 3840, 57, 16610, 16536, 3840, 57, 16610, 16536, 0, 15805, 3, 0 };
  memcpy(mem, program, 33252);
}
