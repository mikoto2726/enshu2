module my_alui ( cin, a, b, r, cout, s_sub, s_fas, s_and, s_or, s_xor, s_not);
input   cin, a, b, s_sub, s_fas, s_and, s_or, s_xor, s_not;
output  r, cout;
wire r_in;

my_fas fas1(.s_sub(s_sub), .cin(cin), .a(a), .b(b), .r(r_in), .cout(cout));

assign r = (s_fas & r_in) | (s_and & (a&b)) | (s_or & (a|b))
           | (s_xor & (a^b)) | (s_not & (~a));
endmodule

