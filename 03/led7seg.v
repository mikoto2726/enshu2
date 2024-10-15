module led7seg(i1, i2, led):

input i1, i2;
output [6:0] led;
    assign led[0] = i1 & ~i2;
    assign led[1] = 0;
    assign led[2] = ~i1 & i2;
    assign led[3] = i1 & ~i2;
    assign led[4] = i1;
    assign led[5] = i1 || i2;
    assign led[6] = ~i2;
endmodule
