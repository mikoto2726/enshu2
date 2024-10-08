module test_my_nand;
reg in1, in2;
wire out;
my_nand mn1 (.a(in1), .b(in2), .y(out));
initial begin
    $dumpfile("test_my_nand.vcd");
    $dumpvars(0, test_my_nand);
    $monitor ("%t: in1 = %b, in2 = %b, out = %b", $time, in1, in2, out);
    in1 = 0; in2 = 0;
    #10 in2 = 1;
    #10 in1 = 1; in2 = 0;
    #10 in1 = 1; in2 = 1;
    #10 in1 = 0; in2 = 0;
    $finish;
end
endmodule
