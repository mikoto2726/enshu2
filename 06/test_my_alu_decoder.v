module test_my_alu_decoder;
    reg [15:0] ir, a, b;
    wire [15:0] r;
    wire cout;

    // テスト対象のモジュールインスタンス化
    my_alu_decoder alu_decoder(.ir(ir), .a(a), .b(b), .r(r), .cout(cout));

    initial begin
        // 結果を監視
        $monitor("%t: IR=%b A=%d B=%d R=%d C=%d", $time, ir, a, b, r, cout);

        // テストケース
        #10 ir = 16'b0000000000001010; a = 65280; b = 257;     // ADD
        #10 ir = 16'b0000000000001001; a = 16; b = 9;          // ADDI
        #10 ir = 16'b0000000000001100; a = 65280; b = 255;     // AND
        #10 ir = 16'b0000000000001110; a = 43520; b = 21760;   // OR
        #10 ir = 16'b0000000000000110; a = 255; b = 255;       // XOR
        #10 ir = 16'b0000000000001011; a = 0; b = 0;           // NOT

        $finish;
    end
endmodule

