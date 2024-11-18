
module my_regfile(n1, rd1);
    input [3:0] n1;             // 4ビットの入力信号
    output [15:0] rd1;          // 16ビットの出力信号
    reg [15:0] rf [0:15];       // 16ビットレジスタが16個

    // 初期化
    initial begin
        rf[0] = 16'd0;
        rf[1] = 16'd1;
        rf[2] = 16'd2;
        rf[3] = 16'd3;
        rf[4] = 16'd4;
        rf[5] = 16'd5;
        rf[6] = 16'd6;
        rf[7] = 16'd7;
        rf[8] = 16'd8;
        rf[9] = 16'd9;
        rf[10] = 16'd10;
        rf[11] = 16'd11;
        rf[12] = 16'd12;
        rf[13] = 16'd13;
        rf[14] = 16'd14;
        rf[15] = 16'd15;
    end

    // レジスタの値を読み出す
    assign rd1 = rf[n1];
endmodule
