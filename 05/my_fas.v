// my_fas.v
module my_fas(input s_sub, input cin, input a, input b, output r, output cout);
    assign r = a ^ b ^ cin;            // 和（Sum）出力
    assign cout = (a & b) | (cin & (a ^ b));  // キャリー出力
endmodule

