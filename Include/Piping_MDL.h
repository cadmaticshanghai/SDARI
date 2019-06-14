/*
	Get radius of default curve. For free bending returned radius is 0.
	
	Input arguments:
	string		pline		Name of pipeline
	int			spec_nr		Ordinal number of spec
	float		dn			Nominal size
	
	Output arguments:
	string		curve_type	Type of curve. Can be one of following:
							Elbow
							Bending
							Free
							
	Return values:
	float		value		Radius of default curve
	int			-1			If failure
*/
Get_Radius_Of_Default_Curve(string pline, int spec_nr, float dn, string curve_type)
{
	if(PM_ACT_PIPELINE(pline, spec_nr) < 0)
		return(-1);
		
	part_id = PM_PID_FROMSPEC("e", dn, 0.);
	if(ISINT(part_id))
		return(-1);
		
	if(HEAD(part_id,1) != "$"){
		curve_type = "Elbow";
		radius = 1. * DM_PARTID_DATA(part_id, "D31");
	}
	else {
		rest = SUBSTRING(part_id, 2);	/* Remove $B */
		dimtbl = STRINGTERM(rest, "-");
		if(dimtbl != ""){
			curve_type = "Bending";
			radius = 0.;
			if(PM_GET_BENDCRV(dimtbl, dn, radius) == -1)
				return(-1);
		}
		if(dimtbl == ""){
			curve_type = "Free";
			radius = 0.;
		}
	}
	
	return(radius);
}

Write_ABRV(f, string group_name, int group_number)
{
	F_PRINTF(f,"ABRV(%s, g%d)", group_name, group_number);
}

Begin_Pipe_mdl(f)
{
	F_PRINTF(f, ",\nPIPE(ST(0,0,1,0,0),MIG(g0)");
}

Pipe_Node_Mdl(f, handle tmat_h, int node_id, float x, float y, float z,
					float dx, float dy, float dz, int type, float ns)
{
	F_PRINTF(f, ",\nNO(NI(%d),", node_id);
	write_point(f, tmat_h, x, y, z);
	F_PRINTF(f, ",");
	write_direction(f, tmat_h, dx, dy, dz);
	F_PRINTF(f, ",CT(%d),NS(%.0f))", type, ns);
	node_id = node_id + 1;
}

Pipe_Mdl(f, handle tmat_h, string pid, float x, float y, float z)
{
	F_PRINTF(f, ",\nPC(%s,1,", pid);
	write_point(f, tmat_h, x, y, z);
	F_PRINTF(f, ",SD(1,p))");
}

Curve_Mdl(f, handle tmat_h, string pid, float x, float y, float z, int is_bend)
{
	F_PRINTF(f, ",\nPC(%s,1,", pid);
	write_point(f, tmat_h, x, y, z);
	if(is_bend)
		F_PRINTF(f, ",BND");
	F_PRINTF(f, ",SD(1,e))");
}

Write_Point(f, handle tmat_h, float x, float y, float z)
{
	tx = x;
	ty = y;
	tz = z;
	TRF_POINT(tmat_h, tx, ty, tz);
	return(F_PRINTF(f, "P(%.1f,%.1f,%.1f)", tx, ty, tz));
}

Write_Direction(f, handle tmat_h, float dx, float dy, float dz)
{
	tdx = dx;
	tdy = dy;
	tdz = dz;
	TRF_DIR(tmat_h, tdx, tdy, tdz);
	return(F_PRINTF(f, "D(%.7f,%.7f,%.7f)", tdx, tdy, tdz));
}

Open_Mdl_File(string pathname)
{
	if(!F_EXIST(pathname))
		F_CREATE_FILE(pathname);
	f = F_OPEN_FILE(pathname, "w");
	if(f < 0){
		msg = "Can't Open file <" + pathname + ">.";
		U_MESSAGEBOX(msg, U_MB_ERROR, U_MB_OK, U_OK);
		return(-1);
	}
	
	return(f);
}