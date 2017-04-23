#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))
#define REGFLAG_DELAY             							0XFD
#define REGFLAG_END_OF_TABLE      							0xFE   // END OF REGISTERS MARKER


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)									lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
//#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

       

//#define LCM_DSI_CMD_MODE

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {

{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
{0xC8,1,{0x00}},//80

{0xB1,2,{0x60,0x21}},

{0xB8,4,{0x01,0x02,0x0C,0x02}}, //0C

{0xB6,1,{0x0F}},
//{0xBB,2,{0xAA,0xAA}},
{0xBC,2,{0x00,0x00}},
{0xBD,5,{0x02,0xB0,0x1E,0x1E,0x00}},
{0xE7,10,{0xF2,0xE6,0xD8,0xCC,0xBF,0xB2,0xA5,0x99,0x99,0x95}},
{0xE8,10,{0xF2,0xE6,0xD8,0xCC,0xBF,0xB2,0xA5,0x99,0x99,0x95}},


{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
{0xBC,2,{0x70,0x00}}, //70
{0xBD,2,{0x70,0x00}}, //70

{0xCA,1,{0x01}},
{0xC0,1,{0x0C}},
//{0xBE,1,{0x66}},//0x56

{0xB3,2,{0x38,0x38}},
{0xB4,2,{0x11,0x11}},
{0xB6,2,{0x05,0x05}},
{0xB9,2,{0x45,0x45}},
{0xBA,2,{0x25,0x25}},
{0xC4,2,{0x11,0x11}},
{0xCE,1,{0x66}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x02}},
{0xEE,1,{0x01}},
//{0xB0,16,{0x00,0x00,0x00,0x6B,0x00,0x91,0x00,0xAB,0x00,0xB6,0x00,0xD8,0x00,0xFC,0x01,0x27}},  //00 4F
//{0xB1,16,{0x01,0x4A,0x01,0x81,0x01,0xAA,0x01,0xEC,0x02,0x23,0x02,0x25,0x02,0x56,0x02,0x90}},
//{0xB2,16,{0x02,0xB3,0x02,0xE4,0x03,0x04,0x03,0x32,0x03,0x4D,0x03,0x73,0x03,0x8A,0x03,0xA7}},
//{0xB3,4,{0x03,0xD8,0x03,0xFF}},

{0xB0,16,{0x00,0x00,0x00,0x0F,0x00,0x31,0x00,0x4F,0x00,0x61,0x00,0x89,0x00,0xA8,0x00,0xDA}}, //00 4F
{0xB1,16,{0x00,0xFE,0x01,0x38,0x01,0x68,0x01,0xB1,0x01,0xED,0x01,0xEE,0x02,0x24,0x02,0x62}},
{0xB2,16,{0x02,0x89,0x02,0xBE,0x02,0xE4,0x03,0x15,0x03,0x34,0x03,0x5B,0x03,0x6F,0x03,0x8B}},
{0xB3,4,{0x03,0xDF,0x03,0xFF}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xD0,1,{0x04}},

{0xF0,5,{0x55,0xAA,0x52,0x08,0x06}},
{0xB0,2,{0x29,0x2A}},
{0xB1,2,{0x10,0x12}},
{0xB2,2,{0x14,0x16}},
{0xB3,2,{0x18,0x1A}},
{0xB4,2,{0x02,0x04}},
{0xB5,2,{0x34,0x34}},
{0xB6,2,{0x34,0x2E}},
{0xB7,2,{0x2E,0x2E}},
{0xB8,2,{0x34,0x00}},
{0xB9,2,{0x34,0x34}},
{0xBA,2,{0x34,0x34}},
{0xBB,2,{0x01,0x34}},
{0xBC,2,{0x2E,0x2E}},
{0xBD,2,{0x2E,0x34}},
{0xBE,2,{0x34,0x34}},
{0xBF,2,{0x05,0x03}},
{0xC0,2,{0x1B,0x19}},
{0xC1,2,{0x17,0x15}},
{0xC2,2,{0x13,0x11}},
{0xC3,2,{0x2A,0x29}},
{0xE5,2,{0x2E,0x2E}},
{0xC4,2,{0x29,0x2A}},
{0xC5,2,{0x1B,0x19}},
{0xC6,2,{0x17,0x15}},
{0xC7,2,{0x13,0x11}},
{0xC8,2,{0x01,0x05}},
{0xC9,2,{0x34,0x34}},
{0xCA,2,{0x34,0x2E}},
{0xCB,2,{0x2E,0x2E}},
{0xCC,2,{0x34,0x03}},
{0xCD,2,{0x34,0x34}},
{0xCE,2,{0x34,0x34}},
{0xCF,2,{0x02,0x34}},
{0xD0,2,{0x2E,0x2E}},
{0xD1,2,{0x2E,0x34}},
{0xD2,2,{0x34,0x34}},
{0xD3,2,{0x04,0x00}},
{0xD4,2,{0x10,0x12}},
{0xD5,2,{0x14,0x16}},
{0xD6,2,{0x18,0x1A}},
{0xD7,2,{0x2A,0x29}},
{0xE6,2,{0x2E,0x2E}},
{0xD8,5,{0x00,0x00,0x00,0x54,0x00}},
{0xD9,5,{0x00,0x15,0x00,0x00,0x00}},
{0xE7,1,{0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xB1,2,{0x00,0x00}},
{0xB0,2,{0x00,0x00}},
{0xB2,5,{0x05,0x00,0x00,0x00,0x00}},
{0xB3,5,{0x05,0x00,0x00,0x00,0x00}},
{0xB4,5,{0x05,0x00,0x00,0x00,0x00}},
{0xB5,5,{0x05,0x00,0x17,0x00,0x00}},
{0xB6,5,{0x12,0x00,0x19,0x00,0x00}},
{0xB7,5,{0x12,0x00,0x19,0x00,0x00}},
{0xB8,5,{0x12,0x00,0x19,0x00,0x00}},
{0xB9,5,{0x12,0x00,0x19,0x00,0x00}},
{0xBA,5,{0x57,0x00,0x00,0x00,0x00}},
{0xBB,5,{0x57,0x00,0x00,0x00,0x00}},
{0xBC,5,{0x75,0x00,0x1A,0x00,0x00}},
{0xBD,5,{0x53,0x00,0x1A,0x00,0x00}},
{0xC0,4,{0x00,0x34,0x00,0x00}},
{0xC1,4,{0x00,0x34,0x00,0x00}},
{0xC2,4,{0x00,0x34,0x00,0x00}},
{0xC3,4,{0x00,0x34,0x00,0x00}},
{0xC4,1,{0x20}},
{0xC5,1,{0x00}},
{0xC6,1,{0x00}},
{0xC7,1,{0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xED,1,{0x30}},
{0xB0,2,{0x17,0x06}},
{0xB8,1,{0x08}},
{0xBD,5,{0x03,0x07,0x00,0x03,0x00}},
{0xB1,2,{0x17,0x06}},
{0xB9,1,{0x00}},
{0xB2,2,{0x00,0x00}},
{0xBA,1,{0x00}},
{0xB3,2,{0x17,0x06}},
{0xBB,1,{0x0A}},
{0xB4,2,{0x17,0x06}},
{0xB5,2,{0x17,0x06}},
{0xB6,2,{0x14,0x03}},
{0xB7,2,{0x00,0x00}},
{0xBC,1,{0x02}},
{0xE5,1,{0x06}},
{0xE6,1,{0x06}},
{0xE7,1,{0x00}},
{0xE8,1,{0x06}},
{0xE9,1,{0x06}},
{0xEA,1,{0x06}},
{0xEB,1,{0x00}},
{0xEC,1,{0x00}},
{0xC0,1,{0x07}},
{0xC1,1,{0x80}},
{0xC2,1,{0xA4}},
{0xC3,1,{0x05}},
{0xC4,1,{0x00}},
{0xC5,1,{0x02}},
{0xC6,1,{0x22}},
{0xC7,1,{0x03}},
{0xC8,2,{0x05,0x30}},
{0xC9,2,{0x01,0x31}},
{0xCA,2,{0x03,0x21}},
{0xCB,2,{0x01,0x20}},
{0xD1,5,{0x00,0x05,0x09,0x07,0x10}},
{0xD2,5,{0x10,0x05,0x0E,0x03,0x10}},
{0xD3,5,{0x20,0x00,0x48,0x07,0x10}},
{0xD4,5,{0x30,0x00,0x43,0x07,0x10}},
{0xD0,1,{0x00}},
{0xCC,3,{0x00,0x00,0x3E}},
{0xCD,3,{0x00,0x00,0x3E}},
{0xCE,3,{0x00,0x00,0x02}},
{0xCF,3,{0x00,0x00,0x02}},
{0x6F,1,{0x11}},
{0xF3,1,{0x01}},
//{0x51,1,{0xFF}},
//{0x53,1,{0x2C}},
//{0x55,1,{0x03}},

{0x11,1,{0x00}},              
{REGFLAG_DELAY, 120, {}},

{0x29,	1,	{0x00}},
{REGFLAG_DELAY, 20, {}},

{REGFLAG_END_OF_TABLE, 0x00, {}},



};
static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 1, {0x00}},
    {REGFLAG_DELAY, 20, {}},

    // Display ON
	{0x29, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 200, {}},

    // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_compare_id_setting[] = {
	// Display off sequence
	{0xF0,	5,	{0x55, 0xaa, 0x52,0x08,0x00}},
	{REGFLAG_DELAY, 10, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
				MDELAY(2);
       	}
    }
	
}


static void init_lcm_registers(void)
{
	unsigned int data_array[16];
}
// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type   = LCM_TYPE_DSI;

	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

    params->physical_width = 63;
    params->physical_height = 111;	

	// enable tearing-free
	params->dbi.te_mode 			= LCM_DBI_TE_MODE_VSYNC_ONLY;
	params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

#if defined(LCM_DSI_CMD_MODE)
	params->dsi.mode   = CMD_MODE;
#else
	params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif
	
	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM				= LCM_THREE_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

	// Highly depends on LCD driver capability.
	// Not support in MT6573

	params->dsi.DSI_WMEM_CONTI=0x3C; 
	params->dsi.DSI_RMEM_CONTI=0x3E; 

		
	params->dsi.packet_size=256;

	// Video mode setting		
	params->dsi.intermediat_buffer_num = 2;

	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

	params->dsi.vertical_sync_active				= 5;//5
	params->dsi.vertical_backporch					= 20;//20
	params->dsi.vertical_frontporch					= 20;//20
	params->dsi.vertical_active_line				= FRAME_HEIGHT;

	params->dsi.horizontal_sync_active				= 30;//20
	params->dsi.horizontal_backporch				= 80;//80
	params->dsi.horizontal_frontporch				= 100;//80
	params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

	// Bit rate calculation
	//modify by yangbo
	params->dsi.PLL_CLOCK				= 305;
	params->dsi.ssc_disable=1;
	params->dsi.HS_TRAIL=100;    //modify for mipi high timing

	params->dsi.clk_lp_per_line_enable = 0;
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.lcm_esd_check_table[0].cmd          = 0x0A;
	params->dsi.lcm_esd_check_table[0].count        = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;

        //params->dsi.lcm_esd_check_table[1].cmd          = 0x0c;
	//params->dsi.lcm_esd_check_table[1].count        = 1;
	//params->dsi.lcm_esd_check_table[1].para_list[0] = 0x70;

        params->dsi.lcm_esd_check_table[1].cmd          = 0x0e;
	params->dsi.lcm_esd_check_table[1].count        = 1;
	params->dsi.lcm_esd_check_table[1].para_list[0] = 0x00;

}

#define LCM_ID_CHANNEL_NUM  1

extern int IMM_GetOneChannelValue_Cali(int Channel, int *voltage);   //add by taoxiaodong

static unsigned int lcm_compare_id(void)
{
                       //modify by taoxiaodong
	unsigned int id=0;
	unsigned char buffer[3];
	unsigned int array[16];  
	unsigned int data_array[16];
	int Lcm_id_volt = 0;
	int ret = 0;
	 
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50); 
	SET_RESET_PIN(1);
	MDELAY(120); 
	 
	    data_array[0] = 0x00063902;
	    data_array[1] = 0x52AA55F0;  
	    data_array[2] = 0x00000108;                
	    dsi_set_cmdq(&data_array, 3, 1); 
	 
	   array[0] = 0x00033700;        // read id return two byte,version and id
	   dsi_set_cmdq(array, 1, 1);
	   read_reg_v2(0xC5, buffer, 3);
	   id = buffer[1];               //we only need ID

	    #ifdef BUILD_LK
		printf("%s, LK TXD_NT35521S  id = 0x%08x buffer[0]=0x%08x,buffer[1]=0x%08x,buffer[2]=0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2]);
	    #else
		printk("%s, LK TXD_NT35521S  id = 0x%08x buffer[0]=0x%08x,buffer[1]=0x%08x,buffer[2]=0x%08x\n", __func__, id,buffer[0],buffer[1],buffer[2]);
	    #endif


		ret = IMM_GetOneChannelValue_Cali(LCM_ID_CHANNEL_NUM, &Lcm_id_volt);
		if (ret != 0)
			#ifdef BUILD_LK
			printf("Lcm_id_volt read fail \n");
		    #else
			printk("Lcm_id_volt read fail \n");
		    #endif
		else
		    #ifdef BUILD_LK
			printf("Lcm_id_volt = %d \n",Lcm_id_volt);
		    #else
			printk("Lcm_id_volt = %d \n",Lcm_id_volt);
		    #endif
	
	   // if(id == LCM_ID_NT35521)
	    if(buffer[0]==0x55 && buffer[1]==0x21 && Lcm_id_volt < 700000)
	    	return 1;
	    else
		return 0;    


}

static void lcm_init(void)
{
	  SET_RESET_PIN(1);
   MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(120);

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}



static void lcm_suspend(void)
{
	unsigned int data_array[2];

	data_array[0] = 0x00280500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 

	data_array[0] = 0x00290500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 

	data_array[0] = 0x00280500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 

	data_array[0] = 0x00290500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 

	data_array[0] = 0x00280500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 

	data_array[0] = 0x00290500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 

	data_array[0] = 0x00280500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 

	data_array[0] = 0x00290500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 

	data_array[0] = 0x00280500; // Display Off
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(25); 


	data_array[0] = 0x00100500; // Sleep In
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(120);

    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(120);	

#ifdef BUILD_LK
	printf("uboot %s\n", __func__);
#else
	printk("kernel %s\n", __func__);
#endif
}


static void lcm_resume(void)
{
	lcm_init();

#ifdef BUILD_LK
	printf("uboot %s\n", __func__);
#else
	printk("kernel %s\n", __func__);
#endif
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}



static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];
	
#ifdef BUILD_LK
	printf("uboot %s\n", __func__);
#else
	printk("kernel %s\n", __func__);	
#endif

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	data_array[3]= 0x00053902;
	data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[5]= (y1_LSB);
	data_array[6]= 0x002c3909;

	dsi_set_cmdq(&data_array, 7, 0);

}



LCM_DRIVER zaw806_nt35521s_hd720_dsi_vdo_txdqc_TXDT500SYPA_lcm_drv = 
{
    .name			= "zaw806_nt35521s_hd720_dsi_vdo_txdqc_TXDT500SYPA",
	.set_util_funcs = lcm_set_util_funcs,
	.compare_id     = lcm_compare_id,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
#if defined(LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
    };