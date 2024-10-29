module my_alu(a, b, r, cout, s_sub, s_fas, s_and, s_or, s_xor, s_not);
input [15:0] a, b;
input s_sub, s_fas, s_and, s_or, s_xor, s_not;
output [15:0] r;
output cout;
wire [14:0] c;

my_alui alui(.cin(s_sub), .a(a[0]), .b(b[0]), .r(r[0]), .cout(c[0]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui2(.cin(c[0]), .a(a[1]), .b(b[1]), .r(r[1]), .cout(c[1]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui3(.cin(c[1]), .a(a[2]), .b(b[2]), .r(r[2]), .cout(c[2]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui4(.cin(c[2]), .a(a[3]), .b(b[3]), .r(r[3]), .cout(c[3]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui5(.cin(c[3]), .a(a[4]), .b(b[4]), .r(r[4]), .cout(c[4]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui6(.cin(c[4]), .a(a[5]), .b(b[5]), .r(r[5]), .cout(c[5]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui7(.cin(c[5]), .a(a[6]), .b(b[6]), .r(r[6]), .cout(c[6]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui8(.cin(c[6]), .a(a[7]), .b(b[7]), .r(r[7]), .cout(c[7]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui9(.cin(c[7]), .a(a[8]), .b(b[8]), .r(r[8]), .cout(c[8]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui10(.cin(c[8]), .a(a[9]), .b(b[9]), .r(r[9]), .cout(c[9]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui11(.cin(c[9]), .a(a[10]), .b(b[10]), .r(r[10]), .cout(c[10]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui12(.cin(c[10]), .a(a[11]), .b(b[11]), .r(r[11]), .cout(c[11]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui13(.cin(c[11]), .a(a[12]), .b(b[12]), .r(r[12]), .cout(c[12]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui14(.cin(c[12]), .a(a[13]), .b(b[13]), .r(r[13]), .cout(c[13]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui15(.cin(c[13]), .a(a[14]), .b(b[14]), .r(r[14]), .cout(c[14]), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));
my_alui alui16(.cin(c[14]), .a(a[15]), .b(b[15]), .r(r[15]), .cout(cout), .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), .s_or(s_or), .s_xor(s_xor), .s_not(s_not));

endmodule

