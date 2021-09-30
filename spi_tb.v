

`timescale 1 ns/10 ps  // time-unit = 1 ns, precision = 10 ps


module spi_tb; 


reg sys_clk,sclk,mosi,miso,cs ;
reg [7:0] data;
reg [2:0] count ;

localparam period = 20;  

spi_dpi DUT (
    .spi_clk_o(sclk),
    .spi_mosi_o(mosi),
    .spi_cs_o(cs),
    .spi_miso_i(miso),
    .sys_clk(sys_clk)
);

// reset
initial 
begin 
	data=56;
	$vcdpluson();
    	$vcdplusmemon();
count =7;	
end

always 
begin

    sys_clk = 1'b1; 
    #10; // high for 10 * timescale = 10 ns

    sys_clk = 1'b0;
    #10; // low for 10 * timescale = 10 ns

end
always 
begin

    	#15;
	count = count-1;
	miso= (data & (1<<count))>>count;

end


endmodule