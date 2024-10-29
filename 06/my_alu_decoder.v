module my_alu_decoder(ir, a, b, r, cout);
    input [15:0] ir, a, b;
    output [15:0] r;
    output cout;

    wire s_sub, s_fas, s_and, s_or, s_xor, s_not;

    // 命令デコーダのインスタンス化
    my_decoder decoder(.ir(ir), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));

    // ALUのインスタンス化
    my_alu alu(.a(a), .b(b), .r(r), .cout(cout), 
               .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), 
               .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
endmodule

