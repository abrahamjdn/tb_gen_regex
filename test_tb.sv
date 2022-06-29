`timescale 1ns/1ns

module test_tb;

 reg	a_tb;
 reg	b_tb;
 reg	c_tb;
 reg	d_tb;
 reg	sal_0_tb;
 reg	zz_tb;
 reg	yy_tb;
 reg	ss_tb;
 reg	dd_tb;
 reg	clk_tb;
 reg [31:0]	ye_tb;
 reg [31:0]	f_tb;

 wire	sal_1_tb;
 wire	k_tb;
 wire	test123456_tb;
 wire [3:0]	sal_2_tb;
 wire [3:0]	sal_3_tb;
 wire [3:0]	sal_4_tb;
 wire [3:0]	sal_5_tb;
 wire [3:0]	sal_6_tb;

 integer i;

test UUT( 
			.a	( a_tb ),
			.b	( b_tb ),
			.c	( c_tb ),
			.d	( d_tb ),
			.sal_0	( sal_0_tb ),
			.zz	( zz_tb ),
			.yy	( yy_tb ),
			.ss	( ss_tb ),
			.dd	( dd_tb ),
			.clk	( clk_tb ),
			.ye	( ye_tb ),
			.f	( f_tb ),
			.sal_1	( sal_1_tb ),
			.k	( k_tb ),
			.test123456	( test123456_tb ),
			.sal_2	( sal_2_tb ),
			.sal_3	( sal_3_tb ),
			.sal_4	( sal_4_tb ),
			.sal_5	( sal_5_tb ),
			.sal_6	( sal_6_tb )  );

initial begin

 $dumpvars( 1, test_tb );
 $dumpfile( "dump.vcd" );

 a_tb	=	1'b0;
 b_tb	=	1'b0;
 c_tb	=	1'b0;
 d_tb	=	1'b0;
 sal_0_tb	=	1'b0;
 zz_tb	=	1'b0;
 yy_tb	=	1'b0;
 ss_tb	=	1'b0;
 dd_tb	=	1'b0;
 clk_tb	=	1'b0;
 ye_tb	=	32'b00000000000000000000000000000000;
 f_tb	=	32'b00000000000000000000000000000000;

 #1;

/*
  for ( i=0 ; i<2**10 ; i=i+1 )
  begin
     { a_tb, b_tb, c_tb, d_tb, sal_0_tb, zz_tb, yy_tb, ss_tb, dd_tb, clk_tb }_tb = i; 
     #1;
  end
*/

/*
  for ( i=0 ; i<2**32 ; i=i+1 )
  begin
     ye_tb = i; 
     #1;
  end
*/

/*
  for ( i=0 ; i<2**32 ; i=i+1 )
  begin
     f_tb = i; 
     #1;
  end
*/

 $finish();

 end
endmodule