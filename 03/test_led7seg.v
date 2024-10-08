module test_led7seg;
reg in1, in2;
wire [6:0] out;
led7seg l7s (.i1(in1), .i2(in2), .led(out));
initial begin
    $monitor ("%t: in1 = %b, in2 = %b, out = %b", $time , in1, in2, out);
    in1 = 0; in2 = 2;
    #10 in2 = 3;
    #10 in1 = 1;
    #10 in2 = 2;
    $finish;
end
endmodule
