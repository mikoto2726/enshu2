module test_my_fa;
reg in1, in2, in3;
wire out1, out2;
my_fa mn1 (.a(in1), .b(in2), .cin(in3), .r(out1), .cout(out2));
initial begin
    $dumpfile("test_my_fa.vcd");
    $dumpvars(0, test_my_fa);
    $monitor ("%t: in1 = %b, in2 = %b, in3 = %b, out1 = %b, out2 = %b", $time, in1, in2, in3, out1, out2);

    in1 = 0; in2 = 0; in3 = 0;

    #10 in3 = 1;
    #10 in2 = 1; in3 = 0;
    #10 in3 = 1;
    #10 in1 = 1; in2 = 0; in3 = 0;
    #10 in3 = 1;
    #10 in2 = 1; in3 = 0;
    #10 in3 = 1;
    $finish;
end
endmodule
