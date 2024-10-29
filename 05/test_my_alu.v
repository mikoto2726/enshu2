module test_my_alu;
    reg [15:0] a, b;
    reg s_sub, s_fas, s_and, s_or, s_xor, s_not;
    wire [15:0] r;
    wire cout;

    my_alu alu1(.a(a), .b(b), .r(r), .cout(cout), 
                .s_sub(s_sub), .s_fas(s_fas), .s_and(s_and), 
                .s_or(s_or), .s_xor(s_xor), .s_not(s_not));

    initial begin
        $monitor("%t: A=%d B=%d R=%d Cout=%d S_SUB=%d S_FAS=%d S_AND=%d S_OR=%d S_XOR=%d S_NOT=%d", 
                 $time, a, b, r, cout, s_sub, s_fas, s_and, s_or, s_xor, s_not);

        #10 a=65280; b=257; s_sub=0; s_fas=1; s_and=0; s_or=0; s_xor=0; s_not=0;
        #10 a=16; b=9; s_sub=1; s_fas=1; s_and=0; s_or=0; s_xor=0; s_not=0;
        #10 a=65280; b=255; s_sub=0; s_fas=0; s_and=1; s_or=0; s_xor=0; s_not=0;
        #10 a=43520; b=21760; s_sub=0; s_fas=0; s_and=0; s_or=1; s_xor=0; s_not=0;

        $finish;
    end
endmodule

