module my_nor(a, b, y);
input a, b;
output y;
    assign y = ~(a || b);
endmodule
