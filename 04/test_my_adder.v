module test_my_adder;
    reg  [15:0] in1, in2;
    wire [15:0] out;

    // my_adderインスタンス化
    my_adder ma1(.a(in1), .b(in2), .out(out));

    initial begin
        // 結果を監視
        $monitor("%t: A=%d  B=%d  OUT=%d", $time, in1, in2, out);

        // テストケースの入力
        in1 = 16'd1024; in2 = 16'd1;     #10;
        in1 = 16'd1024; in2 = 16'd1024;  #10;
        in1 = 16'd65534; in2 = 16'd1;    #10;
        in1 = 16'd65535; in2 = 16'd1;    #10;

        // シミュレーション終了
        $finish;
    end
endmodule

