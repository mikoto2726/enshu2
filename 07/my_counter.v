module my_counter(clk, res, q);
    input clk, res;
    output reg [3:0] q;

    // クロック信号の立ち上がりエッジで動作
    always @(posedge clk) begin
        if (res == 1'b1)
            q <= 4'b0000;      // リセット信号が1のとき、qを0に
        else
            q <= q + 4'b0001;  // それ以外の場合、qを1加算
    end
endmodule

