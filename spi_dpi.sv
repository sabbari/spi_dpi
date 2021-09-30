

module spi_dpi(
    output reg spi_clk_o,
    output reg spi_mosi_o,
    output reg spi_cs_o,
    input spi_miso_i,
    input sys_clk
);



reg [7:0] buffer;




import "DPI-C" context function int write_SPI( output bit spi_cs,
                                             output bit spi_sclk,
                                             output bit spi_mosi,
                                             input  bit spi_miso
);

import "DPI-C" context function     create_socket_and_bind() ;
import "DPI-C" context function     close_server()  ;


initial 
    begin 
        spi_cs_o    =1;
        spi_clk_o   =1;
        spi_mosi_o  =0;
	buffer =0;
	 create_socket_andbind() ;
    end

always @(posedge sys_clk)
begin
	if(buffer <32)
	begin	
		buffer= buffer +1 ;
	end ;
	if (buffer==32)
	begin
		if(write_SPI(spi_cs_o,spi_clk_o,spi_mosi_o,spi_miso_i)==0)
		begin
			close_server()  ;
			buffer=33;
			
		end;
	end;
	if(buffer>32)
	begin
		buffer=buffer+1;
		
	end;
	if(buffer ==64)
	begin
		$finish;
	end;
    	
end
endmodule