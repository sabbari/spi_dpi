

module spi_master_dpi(
    output reg spi_clk_o,
    output reg spi_mosi_o,
    output reg spi_cs_o,
    input spi_miso_i,
    input sys_clk
);

parameter port = 1234;





import "DPI-C" context function int spi_master( output bit spi_cs,
                                             output bit spi_sclk,
                                             output bit spi_mosi,
                                             input  bit spi_miso,
                                             input  int port
); // spi() defined in file server_spi.c



initial 
    begin 
        spi_cs_o    =1;
        spi_clk_o   =1;
        spi_mosi_o  =0;
    end

always @(posedge sys_clk)
begin

	spi_master(spi_cs_o,spi_clk_o,spi_mosi_o,spi_miso_i,port);
    	
end
endmodule



module spi_slave_dpi(
    input  spi_clk_i,
    input  spi_mosi_i,
    input  spi_cs_i,
    output reg spi_miso_o
);

parameter port = 1234;





import "DPI-C" context function int spi_slave( input bit spi_cs,
                                             input bit spi_sclk,
                                             input bit spi_mosi,
                                             output bit spi_miso,
                                             input  int port
); // spi() defined in file server_spi.c



initial 
    begin 
        spi_miso_o    =0;
	spi_slave(spi_cs_i,spi_clk_i,spi_mosi_i,spi_miso_o,port);
    end

always @(posedge spi_clk_i,spi_cs_i)
begin

	spi_slave(spi_cs_i,spi_clk_i,spi_mosi_i,spi_miso_o,port);

    	
end
endmodule
