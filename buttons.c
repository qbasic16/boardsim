#include "buttons.h"

struct board_button buttons[] = {
	{SDLK_q, 3, lin_scale(624, 791, 4, 0), 606, UP},
	{SDLK_w, 2, lin_scale(624, 791, 4, 1), 606, UP},
	{SDLK_e, 1, lin_scale(624, 791, 4, 2), 606, UP},
	{SDLK_r, 0, lin_scale(624, 791, 4, 3), 606, UP},
};

PLI_INT32 DE2_buttons_calltf(PLI_BYTE8 *user_data)
{
	vpiHandle systf;
	s_vpi_value val;
	s_vpi_vecval vec0;

	systf = vpi_handle(vpiSysTfCall, NULL);

	val.format = vpiVectorVal;
	val.value.vector = &vec0;
	vec0.aval = 0;
	vec0.bval = 0;

	size_t i;
	int buttonsSz = sizeof(buttons) / sizeof(buttons[0]);
	for (i = 0; i < buttonsSz; i++)
		if (buttons[i].state)
			vec0.aval |= (1 << (buttonsSz - 1)) >> i;

	vpi_put_value(systf, &val, NULL, vpiNoDelay);

	return 0;
}

PLI_INT32 DE2_buttons_sizetf(PLI_BYTE8 *user_data)
{
	return ((sizeof(buttons) / sizeof(buttons[0])));
}

void DE2_buttons_register()
{
	s_vpi_systf_data tf_data;

	tf_data.type = vpiSysFunc;
	tf_data.sysfunctype = vpiSizedFunc;
	tf_data.tfname = "$DE2_buttons";
	tf_data.calltf = DE2_buttons_calltf;
	tf_data.compiletf = NULL;
	tf_data.sizetf = DE2_buttons_sizetf;
	tf_data.user_data = NULL;

	vpi_register_systf(&tf_data);
}