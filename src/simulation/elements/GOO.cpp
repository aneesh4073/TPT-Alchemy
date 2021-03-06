#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GOO PT_GOO 12
Element_GOO::Element_GOO()
{
	Identifier = "DEFAULT_PT_GOO";
	Name = "GOO";
	Colour = PIXPACK(0x804000);
	MenuVisible = 0;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.97f;
	Loss = 0.50f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 12;

	Weight = 100;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 75;
	Description = "Deforms and disappears under pressure.";

	Properties = TYPE_SOLID | PROP_NEUTPENETRATE|PROP_LIFE_DEC|PROP_LIFE_KILL_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_GOO::update;
}

#define ADVECTION 0.1f

//#TPT-Directive ElementHeader Element_GOO static int update(UPDATE_FUNC_ARGS)
int Element_GOO::update(UPDATE_FUNC_ARGS)
{
	if (!parts[i].life && sim->pv[y/CELL][x/CELL]>1.0f)
		parts[i].life = rand()%80+300;
	if (parts[i].life)
	{
		parts[i].vx += ADVECTION*sim->vx[y/CELL][x/CELL];
		parts[i].vy += ADVECTION*sim->vy[y/CELL][x/CELL];
	}

	int r, rx, ry;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if(!r)
					continue;

				if(parts[i].type == PT_GOO && (r&0xFF) == PT_WATR && !(i%20))
				{
					sim->create_part(i, x, y, PT_PLNT);
					return 1;
				}
			}

	return 0;
}


Element_GOO::~Element_GOO() {}
