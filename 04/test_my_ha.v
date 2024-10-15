module test_my_ha;
reg in1, in2;
wire out;
my_ha mn1 (.a(in1), .b(in2), .r(out1), .c(out2));
initial begin
    $dumpfile("test_my_ha.vcd");
    $dumpvars(0, test_my_ha);
    $monitor ("%t: in1 = %b, in2 = %b, out1 = %b, out2 = %b", $time, in1, in2, out1, out2);

    in1 = 0; in2 = 0;

    #10 in2 = 1;

    #10 in2 = 1;
    #10 in1 = 1; in2 = 0;
    #10 in1 = 1; in2 = 1;
    $finish;
end
endmodule
