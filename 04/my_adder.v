module my_adder(a, b, out);
    input [15:0] a, b;
    output [15:0] out;
    wire [15:0] carry;

    my_ha ha0 (.a(a[0]), .b(b[0]), .r(out[0]), .c(carry[0]));

    genvar i;
    generate
        for (i = 1; i < 16; i = i + 1) begin
            my_fa fa (.a(a[i]), .b(b[i]), .cin(carry[i-1]), .r(out[i]), .cout(carry[i]));
        end
    endgenerate
endmodule

