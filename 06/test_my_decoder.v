module test_my_decoder;
    reg [15:0] ir;
    wire s_sub, s_fas, s_and, s_or, s_xor, s_not;

    my_decoder md1(.ir(ir), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));

    initial begin
        $monitor("%t: IR=%b, S_SUB=%b, S_FAS=%b, S_AND=%b, S_OR=%b, S_XOR=%b, S_NOT=%b", 
                 $time, ir, s_sub, s_fas, s_and, s_or, s_xor, s_not);

        // 各命令のテストケース
        #10 ir = 16'b0000_xxxx_1010_xxxx; // ADD
        #10 ir = 16'b0000_xxxx_1001_xxxx; // ADDI
        #10 ir = 16'b0000_xxxx_1100_xxxx; // SUB
        #10 ir = 16'b0000_xxxx_1110_xxxx; // AND
        #10 ir = 16'b0000_xxxx_0010_xxxx; // OR
        #10 ir = 16'b0000_xxxx_0110_xxxx; // XOR
        #10 ir = 16'b0000_xxxx_1011_xxxx; // NOT

        $finish;
    end
endmodule

