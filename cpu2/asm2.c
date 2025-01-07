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
#define BRA(label) code_bc(0x7F00,label, "BRA")	
#define CMP(d,s) code_rr(0x0040,d,s,"CMP")	
#define CMPI(s,imm16)	code_rri(0x8040 | ((s) & 0xF), 0, 0, imm16, "CMPI")
#define JL(s)	code_rr(0x1EC0 | ((s) & 0xF), 0, 0, "JL")
#define JMP(s)	code_rr(0x1FCe,s,0,"JMP")
#define LD(d,s)	    code_rr(0x20C0,d,s,"LD")
#define LDB(d,s)	code_rr(0x2080,d,s,"LDB")
#define LDI(d,imm16) code_rri(0x90F0,d,0,imm16,"LDI")
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
    int L1, L2, L3, L4, L5, L6, L7, L8, L9,L10,
        L11, L12, L13, L14, L15, L16, L17, L18, L19, L20,
        L_TEST, L_DATA, L_STARTTEST, L_ENDTEST;

    for( pass = 0; pass < 2; pass++ ) { // Process the assembler code two times

// Sample code: Test program
    org(0);

trap(1);
    LDI(R0,L_STARTTEST);
trap(4);

    LDI(R0,L_TEST);
    aligncode();        // JL must be aligned to 4-byte boundary
    JL(R0);             // Call TEST

    aligncode();
    LDI(R0,L_ENDTEST);
trap(4);
trap(0);

L_STARTTEST = addr;
    string("Start test.");
L_ENDTEST = addr;
    string("End of the test.");
    aligndata();

    aligncode();
L_TEST = addr;
    LDI(R0,1);          // Test No. R0 = 1, AND
    LDI(R1,0x1234);     // R1 = 0x00001234
    LDI(R2,0xF0F0);     // R2 = 0xFFFFF0F0
    LDI(R3,0x1030);     // R3 = 0x00001030
    AND(R2,R1);
    BEQ(R2,R3,L2);
trap(2); trap(6);

    aligncode();
L2 = addr;
    LDI(R0,2);          // Test No. R0 = 2, CMPI
    LDI(R1,-128);       // R1 = -128
    CMP(R1,R1);
    CMPI(R1,-127);
    BC(L3);
trap(2); trap(6);

    aligncode();
L3 = addr;
    LDI(R0,3);          // Test No. R0 = 3, LD
    LDI(R1,L_DATA);     // R1 = L_DATA
    SETH(R3,0x89AB);
    OR3(R3,R3,0xCDEF);  // R3 = 0x89ABCDEF
    LD(R2,R1);
    BEQ(R2,R3,L4);
trap(2); trap(6);

    aligncode();
L4 = addr;
    LDI(R0,4);          // Test No. R0 = 4, LDB
    LDI(R1,L_DATA+4);   // R1 = LDATA+4
    LDI(R3,0x76);       // R3 = 0x76
    LDB(R2,R1);
    BEQ(R2,R3,L5);
trap(2); trap(6);

    aligncode();
L5 = addr;
    LDI(R0,5);          // Test No. R0 = 5, LDB
    LDI(R1,L_DATA+1);   // R1 = LDATA+1
    SETH(R3,0xFFFF);
    OR3(R3,R3,0xFFAB);  // R3 = 0xFFFFFFAB
    LDB(R2,R1);
    BEQ(R2,R3,L6);
trap(2); trap(6);

    aligncode();
L6 = addr;
    LDI(R0,6);          // Test No. R0 = 6, MUL
    LDI(R1,-71);        // R1 = -71
    LDI(R2,-89);        // R2 = -89
    LDI(R3,6319);       // R3 = 6319
    MUL(R2,R1);
    BEQ(R2,R3,L7);
trap(2); trap(6);

    aligncode();
L7 = addr;
    LDI(R0,7);          // Test No. R0 = 7, MV
    LDI(R1,97);         // R1 = 97
    LDI(R2,0);          // R2 = 0
    LDI(R3,97);         // R3 = 97
    MV(R2,R1);
    BEQ(R2,R3,L8);
trap(2); trap(6);

    aligncode();
L8 = addr;
    LDI(R0,8);          // Test No. R0 = 8, NEG
    LDI(R1,-8);         // R1 = -8
    LDI(R2,0);          // R2 = 0
    LDI(R3,8);          // R3 = 8
    NEG(R2,R1);
    BEQ(R2,R3,L9);
trap(2); trap(6);

    aligncode();
L9 = addr;
    LDI(R0,9);          // Test No. R0 = 9, NOT
    LDI(R1,0xABCD);     // R1 = 0xFFFFABCD
    LDI(R3,0x5432);     // R3 = 0x00005432
    NOT(R2,R1);
    BEQ(R2,R3,L10);
trap(2); trap(6);

    aligncode();
L10 = addr;
    LDI(R0,10);         // Test No. R0 = 10, OR
    LDI(R1,0x1234);     // R1 = 0x00001234
    LDI(R2,0xF0F0);     // R2 = 0xFFFFF0F0
    LDI(R3,0xF2F4);     // R3 = 0xFFFFF2F4
    OR(R2,R1);
    BEQ(R2,R3,L11);
trap(2); trap(6);

    aligncode();
L11 = addr;
    LDI(R0,11);         // Test No. R0 = 11, SLL
    LDI(R1,12);         // R1 = 12
    SETH(R2,0xFEDC);
    OR3(R2,R2,0xBA98);  // R2 = 0xFEDCBA98
    SETH(R3,0xCBA9);
    OR3(R3,R3,0x8000);  // R3 = 0xCBA98000
    SLL(R2,R1);
    BEQ(R2,R3,L12);
trap(2); trap(6);

    aligncode();
L12 = addr;
    LDI(R0,12);         // Test No. R0 = 12, SLLI
    SETH(R1,0xFEDC);
    OR3(R1,R1,0xBA98);  // R1 = 0xFEDCBA98
    SETH(R3,0x9800);
    OR3(R3,R3,0x0000);  // R3 = 0x98000000
    SLLI(R1,24);
    BEQ(R1,R3,L13);
trap(2); trap(6);

    aligncode();
L13 = addr;
    LDI(R0,13);         // Test No. R0 = 13, SRL
    LDI(R1,12);         // R1 = 12
    SETH(R2,0xFEDC);
    OR3(R2,R2,0xBA98);  // R2 = 0xFEDCBA98
    SETH(R3,0x000F);
    OR3(R3,R3,0xEDCB);  // R3 = 0x000FEDCB
    SRL(R2,R1);
    BEQ(R2,R3,L14);
trap(2); trap(6);

    aligncode();
L14 = addr;
    LDI(R0,14);         // Test No. R0 = 14, SRLI
    SETH(R1,0xFEDC);
    OR3(R1,R1,0xBA98);  // R1 = 0xFEDCBA98
    SETH(R3,0x0FED);
    OR3(R3,R3,0xCBA9);  // R3 = 0x0FEDCBA9
    SRLI(R1,4);
    BEQ(R1,R3,L15);
trap(2); trap(6);

    aligncode();
L15 = addr;
    LDI(R0,15);         // Test No. R0 = 15, ST
    LDI(R1,L_DATA);     // R1 = LDATA
    SETH(R3,0x6789);
    OR3(R3,R3,0xABCD);  // R3 = 0x6789ABCD
    ST(R3,R1);
    LD(R2,R1);
    BEQ(R2,R3,L16);
trap(2); trap(6);

    aligncode();
L16 = addr;
    LDI(R0,16);         // Test No. R0 = 16, STB
    LDI(R1,L_DATA+5);   // R1 = LDATA+5
    LDI(R3,0xAB);
    STB(R3,R1);
    LDI(R1,L_DATA+4);   // R1 = LDATA+4
    SETH(R3,0x76AB);
    OR3(R3,R3,0x3210);  // R3 = 0x76AB3210
    LD(R2,R1);
    BEQ(R2,R3,L17);
trap(2); trap(6);

    aligncode();
L17 = addr;
    LDI(R0,17);         // Test No. R0 = 17, SUB
    LDI(R1,10);         // R1 = 10
    LDI(R2,8);          // R2 = 8
    LDI(R3,-2);         // R3 = -2
    SUB(R2,R1);
    BEQ(R2,R3,L18);
trap(2); trap(6);

    aligncode();
L18 = addr;
    LDI(R0,18);         // Test No. R0 = 18, XOR
    LDI(R1,0x0B0D);     // R1 = 0x00000B0D
    LDI(R2,0xABCD);     // R2 = 0xFFFFABCD
    SETH(R3,0xFFFF);
    OR3(R3,R3,0xA0C0);  // R3 = 0xFFFFA0C0
    XOR(R2,R1);
    BEQ(R2,R3,L19);
trap(2); trap(6);

    aligncode();
L19 = addr;
    JMP(R14);

    aligndata();
L_DATA = addr;
    data(0x89ABCDEF);
    data(0x76543210);

    }
    return 0;
}
