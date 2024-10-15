module my_fa(cin, a, b, r, cout);
input cin, a, b;
output r, cout;
wire r1, c1, c2;

    my_ha ha1(.a(a), .b(b), .r(r1), .c(c1));
    my_ha ha2(.a(r1), .b(cin), .r(r), .c(c2));

    assign cout = c1 | c2;

endmodule
