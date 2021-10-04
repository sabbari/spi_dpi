

module spi_dpi()
  parameter port = 1234
)(
    output reg spi_clk_o,
    output reg spi_mosi_o,
    output reg spi_cs_o,
    input spi_miso_i,
    input sys_clk
);



reg [7:0] buffer;




import "DPI-C" context function int spi( output bit spi_cs,
                                             output bit spi_sclk,
                                             output bit spi_mosi,
                                             input  bit spi_miso,
                                             input  int port
);

import "DPI-C" context function     create_socket_and_bind() ;
//import "DPI-C" context function     close_server()  ;


initial 
    begin 
        spi_cs_o    =1;
        spi_clk_o   =1;
        spi_mosi_o  =0;
	    buffer =0;
	 //create_socket_and_bind() ;
    end

always @(posedge sys_clk)
begin

	spi(spi_cs_o,spi_clk_o,spi_mosi_o,spi_miso_i,port);
    	
end
endmodule
