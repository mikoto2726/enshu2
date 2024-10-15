module my_ha(a, b, r, c);
input a, b;
output r, c;

    assign r = (a & ~b) | (~a & b);
    assign c = a & b;

endmodule
