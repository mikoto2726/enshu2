// M32R assembler (2024 version)
// 名前 (Name)：　学籍番号 (ID)：

#include <stdio.h>

// Instruction macros for code generation
#define ADD(d,s)	code_rr(0x00A0,d,s,"ADD")
#define ADD3(d,s,imm16)	code_rri(0x80A0,d,s,imm16,"ADD3")
#define ADDI(d,imm8)	code_rr(0x4000,d,(imm8)&0xff,"ADDI")
#define AND(d,s)    code_rr(0x00C0,d,s,"AND")	
#define BC(label)	code_bc(0x7C00,label,"BC")
#define BEQ(s1,s2,label) code_beq(0xB000,s1,s2,label,"BEQ")
#define BRA(label)    code_bc(0x7F00,label,"BRA")
#define CMP(d,s)    code_rr(0x0040,d,s,"CMP")
#define CMPI(s,imm16) code_rri(0x8040 | ((s) & 0xF), 0, 0, imm16, "CMPI")
#define JL(s)       code_rr(0x1EC0 | ((s) & 0xF), 0, 0, "JL")
#define JMP(s)		code_rr(0x1FC6,s,0,"JMP")
#define LD(d,s)	    code_rr(0x20C0,d,s,"LD")
#define LDB(d,s)	code_rr(0x2080,d,s,"LDB")
#define LDI(d,imm16)	code_rri(0x90F0,d,0,imm16,"LDI")
#define MUL(d,s)    code_rr(0x1060,d,s,"MUL")
#define MV(d,s)	    code_rr(0x1080,d,s,"MV")
#define NEG(d,s)	code_rr(0x0030,d,s,"NEG")
#define NOP()	code_rr(0x7000,0,0,"NOP")
#define NOT(d,s)	code_rr(0x00B0,d,s,"NOT")
#define OR(d,s)	    code_rr(0x00E0,d,s,"OR")	
#define OR3(d,s,imm16)	code_rri(0x80E0,d,s,imm16,"OR3")
#define SETH(d,imm16)	code_rri(0xD0C0,d,0,imm16,"SETH")
#define SLL(d,s)    code_rr(0x1040,d,s,"SLL")
#define SLLI(d,imm5)	code_rr(0x5040,d,(imm5)&0x1f,"SLLI")
#define SRL(d,s)	code_rr(0x1000,d,s,"SRL")
#define SRLI(d,imm5)	code_rr(0x5000,d,(imm5)&0x1f,"SRLI")
#define ST(s1,s2)	code_rr(0x2040,s1,s2,"ST")
#define STB(s1,s2)	code_rr(0x2000,s1,s2,"STB")
#define SUB(d,s)	code_rr(0x0020,d,s,"SUB")
#define XOR(d,s)	code_rr(0x00D0,d,s,"XOR")

// Register name and register number 
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define R8 8
#define R9 9
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define R14 14
#define R15 15

int addr; // Memory address to place generated code and data
int pass; // Pass number of the assembly process

// Pseudo inctruction functions
#define org(x)          ( addr = x )
#define aligncode()     ( ( addr%4 == 2 ) ? code_rr(0x7000,0,0,"NOP") : 0 )
#define aligndata()     ( ( addr%4 != 0 ) ? addr = (addr&0xfffffffc)+4 : 0 )
#define trap(n)         code_rr(0x10F0,0,n,"trap")

int data(int x)
{
    if( pass > 0 )
        printf("%04x %08x // Word data\n", addr, x);
    addr += 4;
    return 0;
}

int string(char *s)
{
    char *p;

    for( p = s; *p != 0; addr++, p++ )
        if( pass > 0 )
            printf("%04x %02x // Byte data\n", addr, *p);
    if( pass > 0 )
        printf("%04x 00 // Byte data\n", addr);
    addr++;
    return 0;
}

// Code generator for the format of "*-*-" (Register-register)
int code_rr(int opcode, int m, int n, char *iname)
{
    if( pass > 0 )
        printf("%04x %04x // %s\n", addr, opcode|(m<<8)|n, iname);
    addr += 2;
    return 0;
}

// Code generator for the format of "*-*- imm16" (Register-register and 16-bit constant)
int code_rri(int opcode, int m, int n, int imm16, char *iname)
{
    if( pass > 0 )
        printf("%04x %04x%04x // %s\n", addr, opcode|(m<<8)|n, imm16&0xffff, iname);
    addr += 4;
    return 0;
}

// Code generator for the format of "**--" (Instruction format of BC)
int code_bc(int opcode, int label, char *iname)
{
    int pcdisp8;

    pcdisp8 = ((label&0xfffffffc)-(addr&0xfffffffc))>>2;
    if( pass > 0 )
        printf("%04x %04x // %s\n", addr, opcode|(pcdisp8&0xff), iname);
    addr += 2;
    return 0;
}

// Code generator for the format of "*-*- pcdisp16" (Instruction format of BEQ)
int code_beq(int opcode, int m, int n, int label, char *iname)
{
    int pcdisp16;

    pcdisp16 = ((label&0xfffffffc)-(addr&0xfffffffc))>>2;
    if( pass > 0 )
        printf("%04x %04x%04x // %s\n", addr, opcode|(m<<8)|n, pcdisp16&0xffff, iname);
    addr += 4;
    return 0;
}

// Main function of the M32R assembler
int main()
{
    int L0, L1, L2, L3, L4, L5, L6, L7;

    for( pass = 0; pass < 2; pass++ ) { // Process the assembler code two times

// Sample code: Generate code for all instructions supported
    org(0);
trap(1);

L0 = addr;
    LDI(R0,0);    // R0 = 0
    LDI(R1,1);    // R1 = 1
    LDI(R2,2);    // R2 = 2
    LDI(R3,-3);   // R3 = -3
    LDI(R4,4);    // R4 = 4
    LDI(R5,5);    // R5 = 5
    LDI(R6,6);    // R6 = 6
    LDI(R7,7);    // R7 = 7
    LDI(R8,8);    // R8 = 8
    LDI(R9,9);    // R9 = 9
    LDI(R10,10);  // R10 = 10
    LDI(R11,11);  // R11 = 11
    LDI(R12,12);  // R12 = 12
    LDI(R13,13);  // R13 = 13
    LDI(R14,14);  // R14 = 14
    LDI(R15,15);  // R15 = 15
    ADD(R4,R5);
    ADD3(R6,R7,0x0fff);
    ADDI(R8,-2);
    AND(R9,R10);
    CMP(R0,R0);   // C = 0
    BC(L0);
    BEQ(R0,R1,L0);
    CMP(R0,R1);   // C = 1
    BRA(L2);
    aligncode();
L2 = addr;
    CMP(R4,R5);
    CMPI(R6,-4);
    LDI(R5,L3);
    JL(R5);
    aligncode();
L3 = addr;
    LDI(R6,L4);
    JMP(R6);
    aligncode();
L4 = addr;
    LDI(R4,0);
    LD(R5,R4);
    LDI(R5,0);
    LDB(R8,R5);
    MUL(R8,R9);
    MV(R10,R11);
    NEG(R12,R13);
    NOP();
    NOT(R5,R6);
    OR(R7,R8);
    OR3(R9,R10,0x1111);
    SETH(R13,0x1234);
    SLL(R4,R5);
    SLLI(R7,0x3);
    SRL(R13,R4);
    SRLI(R7,0xB);
    LDI(R4,0x1234);  // R4 = 0x1234
    LDI(R5,L7);      // R5 = L7
    ST(R4,R5);
    STB(R0,R5);
    SUB(R6,R7);
    XOR(R8,R9);
trap(0);
L7 = addr;
    addr = addr+256;

    }
    return 0;
}
