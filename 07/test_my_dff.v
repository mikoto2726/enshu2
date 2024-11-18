module test_my_dff();
    reg clk, d;
    wire q;

    // my_dff モジュールのインスタンス化
    my_dff dff1(.clk(clk), .d(d), .q(q));

    // クロック信号の生成
    always #10 clk = ~clk; // 10単位時間ごとにクロックを反転

    // 初期設定と入力信号の変化
    initial begin
        $monitor("%t: clk = %b, d = %b, q = %b", $time, clk, d, q);
        clk = 0; d = 0;    // 初期値の設定

        #10 d = 1;        // 時間10でdを1に
        #30 d = 0;        // 時間40でdを0に
        #30 d = 1;        // 時間70でdを1に
        #30 $finish;      // 時間100でシミュレーション終了
    end
endmodule
