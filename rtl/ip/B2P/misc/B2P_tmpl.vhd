component B2P is
    port(
        reset_byte_n_i: in std_logic;
        clk_byte_i: in std_logic;
        sp_en_i: in std_logic;
        dt_i: in std_logic_vector(5 downto 0);
        lp_av_en_i: in std_logic;
        payload_en_i: in std_logic;
        payload_i: in std_logic_vector(7 downto 0);
        wc_i: in std_logic_vector(15 downto 0);
        reset_pixel_n_i: in std_logic;
        clk_pixel_i: in std_logic;
        fv_o: out std_logic;
        lv_o: out std_logic;
        pd_o: out std_logic_vector(9 downto 0);
        p_odd_o: out std_logic_vector(1 downto 0)
    );
end component;

__: B2P port map(
    reset_byte_n_i=>,
    clk_byte_i=>,
    sp_en_i=>,
    dt_i=>,
    lp_av_en_i=>,
    payload_en_i=>,
    payload_i=>,
    wc_i=>,
    reset_pixel_n_i=>,
    clk_pixel_i=>,
    fv_o=>,
    lv_o=>,
    pd_o=>,
    p_odd_o=>
);
