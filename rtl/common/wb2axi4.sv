`default_nettype none
module wb2axi4 #(
    parameter C_AXI_DATA_WIDTH = 64,  // Width of the AXI R&W data
    parameter C_AXI_ADDR_WIDTH = 26,  // AXI Address width (log wordsize)
    parameter C_AXI_ID_WIDTH = 1,
    parameter DW = 32,  // Wishbone data width
    parameter AW = 26,  // Wishbone address width (log wordsize)
    parameter [C_AXI_ID_WIDTH-1:0] AXI_WRITE_ID = 1'b0,
    parameter [C_AXI_ID_WIDTH-1:0] AXI_READ_ID = 1'b1
) (
    input  wire                          i_clk,          // System clock
    input  wire                          i_reset,        // Reset signal,drives AXI rst
    // AXI write address channel signals
    output reg                           o_axi_awvalid,  // Write address valid
    input  wire                          i_axi_awready,  // Slave is ready to accept
    output reg  [    C_AXI_ID_WIDTH-1:0] o_axi_awid,     // Write ID
    output reg  [  C_AXI_ADDR_WIDTH-1:0] o_axi_awaddr,   // Write address
    output reg  [                   7:0] o_axi_awlen,    // Write Burst Length
    output reg  [                   2:0] o_axi_awsize,   // Write Burst size
    output reg  [                   1:0] o_axi_awburst,  // Write Burst type
    output reg  [                   0:0] o_axi_awlock,   // Write lock type
    output reg  [                   3:0] o_axi_awcache,  // Write Cache type
    output reg  [                   2:0] o_axi_awprot,   // Write Protection type
    output reg  [                   3:0] o_axi_awqos,    // Write Quality of Svc
    // AXI write data channel signals
    output reg                           o_axi_wvalid,   // Write valid
    input  wire                          i_axi_wready,   // Write data ready
    output reg  [  C_AXI_DATA_WIDTH-1:0] o_axi_wdata,    // Write data
    output reg  [C_AXI_DATA_WIDTH/8-1:0] o_axi_wstrb,    // Write strobes
    output reg                           o_axi_wlast,    // Last write transaction
    // AXI write response channel signals
    input  wire                          i_axi_bvalid,   // Write reponse valid
    output reg                           o_axi_bready,   // Response ready
    input  wire [    C_AXI_ID_WIDTH-1:0] i_axi_bid,      // Response ID
    input  wire [                   1:0] i_axi_bresp,    // Write response
    // AXI read address channel signals
    output reg                           o_axi_arvalid,  // Read address valid
    input  wire                          i_axi_arready,  // Read address ready
    output reg  [    C_AXI_ID_WIDTH-1:0] o_axi_arid,     // Read ID
    output reg  [  C_AXI_ADDR_WIDTH-1:0] o_axi_araddr,   // Read address
    output reg  [                   7:0] o_axi_arlen,    // Read Burst Length
    output reg  [                   2:0] o_axi_arsize,   // Read Burst size
    output reg  [                   1:0] o_axi_arburst,  // Read Burst type
    output reg  [                   0:0] o_axi_arlock,   // Read lock type
    output reg  [                   3:0] o_axi_arcache,  // Read Cache type
    output reg  [                   2:0] o_axi_arprot,   // Read Protection type
    output reg  [                   3:0] o_axi_arqos,    // Read Protection type
    // AXI read data channel signals
    input  wire                          i_axi_rvalid,   // Read reponse valid
    output reg                           o_axi_rready,   // Read Response ready
    input  wire [    C_AXI_ID_WIDTH-1:0] i_axi_rid,      // Response ID
    input  wire [  C_AXI_DATA_WIDTH-1:0] i_axi_rdata,    // Read data
    input  wire [                   1:0] i_axi_rresp,    // Read response
    input  wire                          i_axi_rlast,    // Read last
    // We'll share the clock and the reset witht he Wishbone side
    input  wire                          wb_cyc_i,
    input  wire                          wb_stb_i,
    input  wire [                   2:0] wb_cti_i,
    input  wire [                   1:0] wb_bte_i,
    input  wire                          wb_we_i,
    input  wire [              (AW-1):0] wb_adr_i,
    input  wire [              (DW-1):0] wb_data_i,
    input  wire [            (DW/8-1):0] wb_sel_i,
    output reg                           wb_ack_o,
    output reg  [              (DW-1):0] wb_data_o,
    output reg                           wb_err_o
);

  // Things we're not changing ...
  localparam DWSIZE = $clog2(DW) - 3;
  assign o_axi_awid    = AXI_WRITE_ID;
  assign o_axi_awlen   = 8'h0;    // Burst length is one
  assign o_axi_awsize  = DWSIZE[2:0];
  assign o_axi_awburst = 2'b00;
  assign o_axi_awlock  = 1'b0;    // Normal signaling
  assign o_axi_awcache = 4'h3;    // Normal: no cache, modifiable
  assign o_axi_awprot  = 3'b010;  // Unpriviledged, unsecure, data access
  assign o_axi_awqos   = 4'h0;    // Lowest quality of service (unused)

  assign o_axi_arlock  = 1'b0;    // Normal signaling
  assign o_axi_arqos   = 4'h0;    // Lowest quality of service (unused)
  assign o_axi_arid    = AXI_READ_ID;
  assign o_axi_arlen   = 8'h0;    // Burst length is one
  assign o_axi_arsize  = DWSIZE[2:0];
  assign o_axi_arburst = 2'b00;
  assign o_axi_arcache = 4'h3;    // Normal: no cache, modifiable
  assign o_axi_arprot  = 3'b010;  // Unpriviledged, unsecure, data access

  // Always send only a single flit
  assign o_axi_wlast = o_axi_wvalid;

  wire valid = wb_cyc_i & wb_stb_i;

  //TODO:ck for errors
  assign wb_err_o = 1'b0;

  // State machine that will take Wishbone cycles and convert them to AXI
  // transactions
  //We need 2 state machines, one to send address requests and another one to send read/write requests. Only when both SM's are idle,
  //its OK to kick off a new transaction.

  enum {
    S_IDLE,
    S_WAIT_RD,
    S_SEND_WR,
    S_WAIT_B,
    S_WAIT_WB_ACK
  } n_state;
  enum {
    A_IDLE,
    A_WAIT_AXI_ACK,  // Wait for ack from AXI   
    A_WAIT_WB_ACK   // Wait for ack to propagate to the wishbone
  } addr_state;


  always_ff @(posedge i_clk) begin
    if (i_reset) begin
      n_state <= S_IDLE;
      addr_state <= A_IDLE;
    end else begin

      case (addr_state)
        A_IDLE:
        if (valid & ((wb_we_i & i_axi_awready) | (~wb_we_i & i_axi_arready))) addr_state <= A_WAIT_AXI_ACK;
        A_WAIT_AXI_ACK: if ((wb_we_i & i_axi_bvalid) | (~wb_we_i & i_axi_rvalid)) addr_state <= A_WAIT_WB_ACK;
        A_WAIT_WB_ACK: if (wb_ack_o) addr_state <= A_IDLE;
      endcase

      case (n_state)
        S_IDLE: begin
          if (valid) begin
            if (wb_we_i & i_axi_wready) n_state <= S_WAIT_B;
            else if (~wb_we_i) n_state <= S_WAIT_RD;
          end
        end

        S_WAIT_RD: if (i_axi_rvalid) n_state <= S_WAIT_WB_ACK;

        S_WAIT_B: if (i_axi_bvalid) n_state <= S_WAIT_WB_ACK;

        S_WAIT_WB_ACK: if (wb_ack_o) n_state <= S_IDLE;

      endcase
    end
  end

  always_ff @(posedge i_clk or posedge i_reset) begin
    if (i_reset) begin
      o_axi_awvalid <= 1'b0;
      o_axi_arvalid <= 1'b0;
    end else begin
      o_axi_awvalid <= 1'b0;
      o_axi_arvalid <= 1'b0;

      case (addr_state)
        A_IDLE: begin
          if (valid) begin
            o_axi_awvalid <= wb_we_i;
            o_axi_arvalid <= ~wb_we_i;
          end
        end

        A_WAIT_AXI_ACK, A_WAIT_WB_ACK: ;

      endcase
    end
  end

  always_comb begin
    o_axi_wvalid = 1'b0;
    o_axi_bready = 1'b1;
    case (n_state)
      S_IDLE: o_axi_wvalid = valid & wb_we_i;
      S_WAIT_RD: o_axi_rready = 1'b1;
      S_WAIT_B: o_axi_bready = 1'b1;
      S_WAIT_WB_ACK: ;
    endcase
  end

  // Register the WB ack for timing
  always_ff @(posedge i_clk or posedge i_reset) begin
    if (i_reset) begin
      wb_ack_o <= 1'b0;
    end else begin
      wb_ack_o <= 1'b0;

      case (n_state)
        S_WAIT_RD: wb_ack_o <= i_axi_rvalid;
        S_WAIT_B:  wb_ack_o <= i_axi_bvalid;
      endcase

    end
  end

  // Convert betweeen 64 and 32 bits
  wire odd_addr = wb_adr_i[2];

  // Register for timing
  always_ff @(posedge i_clk or posedge i_reset) begin
    if (i_reset) begin
      o_axi_awaddr <= 0;
      o_axi_wdata  <= 0;
      o_axi_wstrb  <= 0;
      o_axi_araddr <= 0;
      wb_data_o    <= 0;
    end else begin
      // Command port:
      o_axi_awaddr <= wb_adr_i;

      // Write port:
      o_axi_wdata <= odd_addr ? {wb_data_i, 32'd0} : {32'd0, wb_data_i};
      o_axi_wstrb <= odd_addr ? {wb_sel_i, 4'b0} : {4'b0, wb_sel_i};

      // Read address port:
      o_axi_araddr <= wb_adr_i;
      wb_data_o    <= odd_addr ? i_axi_rdata[63:32] : i_axi_rdata[31:0];
    end
  end

endmodule
`default_nettype wire
