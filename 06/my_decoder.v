module my_decoder ( ir, s_sub, s_fas, s_and, s_or, s_xor, s_not);
    input [15:0] ir;
    output s_sub, s_fas, s_and, s_or, s_xor, s_not;
    wire isADDI, isALU, isADD;

    // ALU操作の識別
    assign isALU = ~ir[15] & ir[14] & ~ir[13] & ir[12];
    assign isADDI = ir[15] & ir[14] & ~ir[13] & ~ir[12];
    assign isADD = ir[15] & ir[14] & ir[13] & ~ir[12];

    // 各操作の出力信号
    assign s_sub = isALU & ir[3];
    assign s_fas = isALU & ir[2];
    assign s_and = isALU & ir[1];
    assign s_or = isALU & ir[0];
    assign s_xor = isALU & ir[1] & ~ir[0];
    assign s_not = isALU & ~ir[3] & ~ir[2] & ~ir[1] & ~ir[0];

endmodule

