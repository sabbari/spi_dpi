

`timescale 1 ns/10 ps  // time-unit = 1 ns, precision = 10 ps


module T22MEV0_tb; 


reg sys_clk,rstb,sclk,mosi,miso,cs ;

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
	$vcdpluson();
    $vcdplusmemon();
end

always 
begin

    sys_clk = 1'b1; 
    #10; // high for 10 * timescale = 10 ns

    sys_clk = 1'b0;
    #10; // low for 10 * timescale = 10 ns
end



endmodule