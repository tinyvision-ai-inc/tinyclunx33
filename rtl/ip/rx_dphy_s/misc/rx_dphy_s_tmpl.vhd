component rx_dphy_s is
    port(
        pll_lock_i: in std_logic;
        sync_clk_i: in std_logic;
        sync_rst_i: in std_logic;
        ready_o: out std_logic;
        clk_byte_o: out std_logic;
        clk_byte_hs_o: out std_logic;
        clk_byte_fr_i: in std_logic;
        reset_n_i: in std_logic;
        reset_byte_n_i: in std_logic;
        reset_byte_fr_n_i: in std_logic;
        clk_p_io: inout std_logic;
        clk_n_io: inout std_logic;
        d_p_io: inout std_logic_vector(0 to 0);
        d_n_io: inout std_logic_vector(0 to 0);
        lp_d_rx_p_o: out std_logic_vector(0 to 0);
        lp_d_rx_n_o: out std_logic_vector(0 to 0);
        bd_o: out std_logic_vector(7 downto 0);
        cd_clk_o: out std_logic;
        cd_d0_o: out std_logic;
        hs_d_en_o: out std_logic;
        hs_sync_o: out std_logic;
        lp_hs_state_clk_o: out std_logic_vector(1 downto 0);
        lp_hs_state_d_o: out std_logic_vector(1 downto 0);
        term_clk_en_o: out std_logic;
        term_d_en_o: out std_logic_vector(0 to 0);
        payload_en_o: out std_logic;
        payload_o: out std_logic_vector(7 downto 0);
        dt_o: out std_logic_vector(5 downto 0);
        vc_o: out std_logic_vector(1 downto 0);
        wc_o: out std_logic_vector(15 downto 0);
        ecc_o: out std_logic_vector(7 downto 0);
        ref_dt_i: in std_logic_vector(5 downto 0);
        tx_rdy_i: in std_logic;
        sp_en_o: out std_logic;
        lp_en_o: out std_logic;
        lp_av_en_o: out std_logic
    );
end component;

__: rx_dphy_s port map(
    pll_lock_i=>,
    sync_clk_i=>,
    sync_rst_i=>,
    ready_o=>,
    clk_byte_o=>,
    clk_byte_hs_o=>,
    clk_byte_fr_i=>,
    reset_n_i=>,
    reset_byte_n_i=>,
    reset_byte_fr_n_i=>,
    clk_p_io=>,
    clk_n_io=>,
    d_p_io=>,
    d_n_io=>,
    lp_d_rx_p_o=>,
    lp_d_rx_n_o=>,
    bd_o=>,
    cd_clk_o=>,
    cd_d0_o=>,
    hs_d_en_o=>,
    hs_sync_o=>,
    lp_hs_state_clk_o=>,
    lp_hs_state_d_o=>,
    term_clk_en_o=>,
    term_d_en_o=>,
    payload_en_o=>,
    payload_o=>,
    dt_o=>,
    vc_o=>,
    wc_o=>,
    ecc_o=>,
    ref_dt_i=>,
    tx_rdy_i=>,
    sp_en_o=>,
    lp_en_o=>,
    lp_av_en_o=>
);
