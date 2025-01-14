
// asm3.c
// M32R assembler (2024 version)
// 名前 (Name)：　学籍番号 (ID)：

#include <stdio.h>

// Instruction macros for code generation
#define ADD(d,s)        code_rr(0x00A0,d,s,"ADD")
#define ADD3(d,s,imm16) code_rri(0x80A0,d,s,imm16,"ADD3")
#define ADDI(d,imm8)    code_rr(0x4000,d,(imm8)&0xff,"ADDI")
#define AND(d,s)        code_rr(0x00C0,d,s,"AND") 
#define BC(label)       code_bc(0x7C00,label,"BC")
#define BEQ(s1,s2,label) code_beq(0xB000,s1,s2,label,"BEQ")
#define BRA(label)      code_bc(0x7F00,label,"BRA")
#define CMP(d,s)        code_rr(0x0040,d,s,"CMP")
#define CMPI(s,imm16)   code_rri(0x8040 | ((s) & 0xF), 0, 0, imm16, "CMPI")
#define JL(s)           code_rr(0x1EC0 | ((s) & 0xF), 0, 0, "JL")
#define JMP(s)          code_rr(0x1FCE,s,0,"JMP")
#define LD(d,s)         code_rr(0x20C0,d,s,"LD")
#define LDB(d,s)        code_rr(0x2080,d,s,"LDB")
#define LDI(d,imm16)    code_rri(0x90F0,d,0,imm16,"LDI")
#define MUL(d,s)        code_rr(0x1060,d,s,"MUL")
#define MV(d,s)         code_rr(0x1080,d,s,"MV")
#define NEG(d,s)        code_rr(0x0030,d,s,"NEG")
#define NOP()           code_rr(0x7000,0,0,"NOP")
#define NOT(d,s)        code_rr(0x00B0,d,s,"NOT")
#define OR(d,s)         code_rr(0x00E0,d,s,"OR")
#define OR3(d,s,imm16)  code_rri(0x80E0,d,s,imm16,"OR3")
#define SETH(d,imm16)   code_rri(0xD0C0,d,0,imm16,"SETH")
#define SLL(d,s)        code_rr(0x1040,d,s,"SLL")
#define SLLI(d,imm5)    code_rr(0x5040,d,(imm5)&0x1f,"SLLI")
#define SRL(d,s)        code_rr(0x1000,d,s,"SRL")
#define SRLI(d,imm5)    code_rr(0x5000,d,(imm5)&0x1f,"SRLI")
#define ST(s1,s2)       code_rr(0x2040,s1,s2,"ST")
#define STB(s1,s2)      code_rr(0x2000,s1,s2,"STB")
#define SUB(d,s)        code_rr(0x0020,d,s,"SUB")
#define XOR(d,s)        code_rr(0x00D0,d,s,"XOR")

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
    int L_REVBIT, L_ATOI, L_STRCPY, L_FACT, L_NEXT, L_NEXT2, L_NEXT3, L_END,
        L0, L1, L2, L3, L4, L5, L6, L7, L8,
        L_S0, L_S1, L_S2, L_S3, L_S4, L_S5, L_S6, L_S7;

    for( pass = 0; pass < 2; pass++ ) { // Process the assembler code two times

        //-------------------------------------------------------------------------
        // Call L_REVBIT
        //-------------------------------------------------------------------------
        org(0);
        // 文字列表示: "Call function 1: Reverse 32-bit bit-order"
        LDI(R0, (L_S0));     // (期待出力で) 00000022 の位置に相当
        trap(4);             // 表示 "Call function 1: Reverse 32-bit bit-order"
        
        // R0に 0x12345678 をセット
        SETH(R0,0x1234);
        OR3(R0,R0,0x5678);
        SUB(R1,R1);
        SUB(R2,R2);
        SUB(R3,R3);

        trap(2);            // R0,R1,R2,R3 表示 (最初の表示)
        LDI(R13, (L_REVBIT));
        JL(R13);            // 関数呼び出し
        aligncode();

        trap(2);            // R0,R1,R2,R3 表示 (反転後)
        trap(6);            // 改行
        BRA(L_NEXT);

        // 文字列本体
        L_S0 = addr;
        string("Call function 1: Reverse 32-bit bit-order");
        aligndata();

        //-------------------------------------------------------------------------
        // Call L_ATOI
        //-------------------------------------------------------------------------
        aligncode();
        L_NEXT = addr;

        LDI(R0,(L_S1)); 
        trap(4);            // "Call function 2: Ascii string to integer (atoi)"

        // R0=256 にする (ASCII文字列が置いてあるアドレスを想定)
        LDI(R0,256);
        SUB(R1,R1);
        SUB(R2,R2);
        SUB(R3,R3);

        trap(2);            // R0,R1,R2,R3 (呼び出し前の表示)
        LDI(R13,(L_ATOI));
        JL(R13);
        aligncode();

        trap(2);            // R0,R1,R2,R3 (変換後の表示)
        trap(6);            // 改行
        BRA(L_NEXT2);

        // 文字列本体
        L_S1 = addr;
        string("Call function 2: Ascii string to integer (atoi)");
        aligndata();

        // 実際の "1234" をメモリ番地 256 に格納しておく
        org(256);
        string("1234");
        aligndata();

        //-------------------------------------------------------------------------
        // Call L_STRCPY
        //-------------------------------------------------------------------------
        aligncode();
        L_NEXT2 = addr;

        LDI(R0,(L_S3));
        trap(4);            // "Call function 3: String copy (strcpy)"

        // R0=512, R1=768 にして、元文字列(512)とバッファ(768)を引数にする
        LDI(R0,512);        
        LDI(R1,768);        
        SUB(R2,R2);
        SUB(R3,R3);

        trap(5);  // (呼び出し前) R0の文字列 と R1の文字列 を表示

        LDI(R13,(L_STRCPY));
        JL(R13);

        aligncode();
        trap(5);  // (呼び出し後) コピー結果を表示
        trap(6);  // 改行
        BRA(L_NEXT3);

        // 文字列本体
        L_S3 = addr;
        string("Call function 3: String copy (strcpy)");
        aligndata();

        // 実際の "Hello world!" をアドレス 512 に
        org(512);
        string("Hello world!");

        // コピー先バッファ (768) は適当に "*******************"
        org(768);
        string("*******************");
        aligndata();

        //-------------------------------------------------------------------------
        // Call L_FACT
        //-------------------------------------------------------------------------
        aligncode();
        L_NEXT3 = addr;

        LDI(R0,(L_S6));
        trap(4);            // "Call function 4: Factorial"

        // 期待では R0=5 で factorial(5)
        LDI(R0,5);
        SUB(R1,R1);
        SUB(R2,R2);
        SUB(R3,R3);

        trap(2);           // (呼び出し前) R0,R1,R2,R3 
        LDI(R13,(L_FACT));
        JL(R13);
        aligncode();

        trap(2);           // (呼び出し後) R0,R1,R2,R3 
        trap(6);           // 改行
        BRA(L_END);

        // 文字列本体
        L_S6 = addr;
        string("Call function 4: Factorial");
        aligndata();

        //-------------------------------------------------------------------------
        // Ending
        //-------------------------------------------------------------------------
        aligncode();
        L_END = addr;
        LDI(R0,(L_S7));
        trap(4);
        trap(0);           // Stop by trap(0)

        L_S7 = addr;
        string("End.");
        aligndata();

        //-------------------------------------------------------------------------
        // Function 1: Reverse 32-bit bit-order (L_REVBIT)
        //-------------------------------------------------------------------------
        aligncode();
        L_REVBIT = addr;
        LDI(R1,0);
        MV(R2,R0);
        LDI(R3,32);
        aligncode();
        L0 = addr;
        {
            LDI(R4,0x1);
            AND(R4,R2);     // R4 = (R2 & 1)
            OR(R1,R4);      // R1 |= R4
            ADDI(R3,-1);
            CMPI(R3,0x1);
            BC(L1);         // if (R3<1) break
            SLLI(R1,1);
            SRLI(R2,1);
            BRA(L0);
        }
        aligncode();
        L1 = addr;
        JMP(R14);          // return

        //-------------------------------------------------------------------------
        // Function 2: Ascii string to integer (L_ATOI)
        //-------------------------------------------------------------------------
        aligncode();
        L_ATOI = addr;
        LDI(R1,0);
        MV(R2,R0);
        LDI(R3,10);
        aligncode();
        L2 = addr;
        {
            LDB(R4,R2);
            LDI(R6,0);
            BEQ(R4,R6,L3);    // if(*p==0) return
            ADD3(R5,R4,-'9');
            CMP(R6,R5);
            BC(L4);           // if(R5>0)エラー
            ADDI(R4,-'0');
            CMP(R4,R6);
            BC(L4);           // if(R4<0)エラー
            MUL(R1,R3);       // R1 *= 10
            ADD(R1,R4);       // R1 += R4
            ADDI(R2,1);
            BRA(L2);
        }
        aligncode();
        L3 = addr;
        JMP(R14);          // return

        aligncode();
        L4 = addr;
        LDI(R1,-1);        // R1 = -1 エラー
        JMP(R14);          // return

        //-------------------------------------------------------------------------
        // Function 3: String copy (L_STRCPY)
        //-------------------------------------------------------------------------
        aligncode();
        L_STRCPY = addr;
        MV(R2,R0);
        MV(R3,R1);
        aligncode();
        L5 = addr;
        {
            LDB(R4,R2);
            STB(R4,R3);
            ADDI(R2,1);
            ADDI(R3,1);
            LDI(R6,0);
            BEQ(R4,R6,L6); // if(*p==0) return
            BRA(L5);
        }
        aligncode();
        L6 = addr;
        JMP(R14);         // return

        //-------------------------------------------------------------------------
        // Function 4: Factorial (L_FACT)
        //-------------------------------------------------------------------------
        aligncode();
        L_FACT = addr;
        LDI(R1,1);
        MV(R2,R0);
        aligncode();
        L7 = addr;
        {
            LDI(R6,0);
            BEQ(R2,R6,L8);  // if(R2==0) goto L8
            MUL(R1,R2);
            ADDI(R2,-1);
            BRA(L7);
        }
        aligncode();
        L8 = addr;
        JMP(R14);         // return
    }
    return 0;
}

