module test_my_alui;
reg cin, a, b, s_sub, s_fas, s_and, s_or, s_xor, s_not;
wire r, cout;

my_alui alui(.cin(cin), .a(a), .b(b), .r(r), .cout(cout),
             .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and),
             .s_or(s_or), .s_xor(s_xor), .s_not(s_not));

initial begin
    $monitor("%t: A = %b B = %b R = %b", $time, a, b, r);
    cin = 0; a = 0; b = 0; s_sub = 0; s_fas = 0; s_and = 0; s_or = 0; s_xor = 0; s_not = 0;

    #10 a = 1; b = 1; s_fas = 1;
    #10 s_sub = 1;
    #10 b = 0; s_fas = 0; s_and = 1;
    #10 a = 0; b = 1; s_and = 0; s_or = 1;
    #10 a = 1; s_or = 0; s_xor = 1;
    #10 a = 0; s_xor = 0; s_not = 1;

    $finish;
end
endmodule

