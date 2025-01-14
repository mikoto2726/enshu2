// M32R assembler (2024 version)
// 名前 (Name)：　学籍番号 (ID)：

#include <stdio.h>

// Instruction macros for code generation
#define ADD(d,s)	code_rr(0x00A0,d,s,"ADD")
#define ADD3(d,s,imm16)	code_rri(0x80A0,d,s,imm16,"ADD3")
#define ADDI(d,imm8)	code_rr(0x4000,d,(imm8)&0xff,"ADDI")
#define AND(d,s)	code_rr(0x00C0,d,s,"AND")
#define BC(label)	code_bc(0x7C00,label,"BC")
#define BEQ(s1,s2,label) code_beq(0xB000,s1,s2,label,"BEQ")
#define BRA(label)	code_bc(0x7F00,label,"BRA")
#define CMP(d,s)	code_rr(0x0040,d,s,"CMP")
#define CMPI(s,imm16)	code_rri(0x8040 | ((s) & 0xF), 0, 0, imm16, "CMPI")
#define JL(s)		code_rr(0x1EC0 | ((s) & 0xF), 0, 0, "JL")
#define JMP(s)		code_rr(0x1FC6,s,0,"JMP")
#define LD(d,s)		code_rr(0x20C0,d,s,"LD")
#define LDB(d,s)	code_rr(0x2080,d,s,"LDB")
#define LDI(d,imm16)	code_rri(0x90F0,d,0,imm16,"LDI")
#define MUL(d,s)	code_rr(0x1060,d,s,"MUL")
#define MV(d,s)		code_rr(0x1080,d,s,"MV")
#define NEG(d,s)	code_rr(0x0030,d,s,"NEG")
#define NOP()		code_rr(0x7000,0,0,"NOP")
#define NOT(d,s)	code_rr(0x00B0,d,s,"NOT")
#define OR(d,s)		code_rr(0x00E0,d,s,"OR")
#define OR3(d,s,imm16)	code_rri(0x80E0,d,s,imm16,"OR3")
#define SETH(d,imm16)	code_rri(0xD0C0,d,0,imm16,"SETH")
#define SLL(d,s)	code_rr(0x1040,d,s,"SLL")
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
#define aligndata()     ( ( (addr&0x00000003) != 0 ) ? addr = (addr&0xfffffffc)+4 : 0 )
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

// Code generator for the format of "**-- pcdisp24" (Instruction format of BC24)
int code_bc24(int opcode, int label, char *iname)
{
    int pcdisp24;

    pcdisp24 = ((label&0xfffffffc)-(addr&0xfffffffc))>>2;
    if( pass > 0 )
        printf("%04x %04x%04x // %s\n", addr, opcode|((pcdisp24>>16)&0xff), pcdisp24&0xffff, iname);
    addr += 4;
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

// Code generator for the format of "*--- imm24" (Instruction format of LD24)
int code_ld24(int opcode, int m, int imm24, char *iname)
{
    if( pass > 0 )
        printf("%04x %04x%04x // %s\n", addr, opcode|(m<<8)|((imm24>>16)&0xff), imm24&0xffff, iname);
    addr += 4;
    return 0;
}

// Main function of the M32R assembler
int main()
{
    int L_TOLOWER,
        L0, L1, L2, L3, L4, L_S0, L_S1, L_S2, L_S3, L_S4, L_S5, L_S6;

    for( pass = 0; pass < 2; pass++ ) { // Process the assembler code two times

// Call TOLOWER
    org(0);
// trap(1);
    LDI(R0,L_S1);       // R0 = L_S1
trap(4);                // Display string at L_S1
    LDI(R0,L_S2);       // R0 = L_S2
    LDI(R1,L_S3);       // R1 = L_S3
    SUB(R2,R2);         // R2 = 0
    SUB(R3,R3);         // R3 = 0
trap(5);                // Display strings at L_S2 and at L_S3
    LDI(R13,L_TOLOWER);
    JL(R13);            // Save return address & jump to L_TOLOWER
    aligncode();
trap(5);                // Display strings at L_S2 and at L_S3
trap(6);                // Display new-line
    LDI(R0,L_S4);       // R0 = L_S4
    LDI(R1,L_S5);       // R1 = L_S5
    SUB(R2,R2);         // R2 = 0
    SUB(R3,R3);         // R3 = 0
trap(5);                // Display strings at L_S4 and at L_S5
    LDI(R13,L_TOLOWER);
    JL(R13);            // Save return address & jump to L_TOLOWER
    aligncode();
trap(5);                // Display strings at L_S4 and at L_S5
trap(6);                // Display new-line
    LDI(R0,L_S6);       // R0 = L_S6
trap(4);                // Display string at L_S6
trap(0);                // Stop simulation

    aligndata();
L_S1 = addr;
    string("Call function: Convert to lowercase");
L_S2 = addr;
    string("It's a Small World, isn't it?");
L_S3 = addr;
    string("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
L_S4 = addr;
    string("[E-mail] Comp.Arch@INIAD.ORG");
L_S5 = addr;
    string("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy");
    aligndata();
L_S6 = addr;
    string("End.");
    aligndata();

// Function: Convert to lowercase
    aligncode();
L_TOLOWER = addr;
// You should write a code here
    JMP(R14);           // return

    }
    return 0;
}

