module test_my_counter();
    reg clk, res;
    wire [3:0] q;

    // my_counter モジュールのインスタンス化
    my_counter counter1(.clk(clk), .res(res), .q(q));

    // クロック信号の生成
    always #5 clk = ~clk; // 5単位時間ごとにクロックを反転

    // 初期設定とテストデータ
    initial begin
        $monitor("%t: clk = %b, res = %b, q = %b", $time, clk, res, q);
        
        // 初期設定
        clk = 0; res = 0;

        #10 res = 1;   // 時間10でリセット
        #10 res = 0;   // 時間20でリセット解除
        #50 res = 1;   // 時間70でリセット
        #10 res = 0;   // 時間80でリセット解除
        #40 $finish;   // 時間120でシミュレーション終了
    end
endmodule

