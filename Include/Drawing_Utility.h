/*
**	Compute axo view params.	
**	Input args:
parameter      type        description
rot            float       angle between view direction and X axis              
slope          float       angle between view direction and XY PLANE
xmin           float       Minimum value of x coordinate of view box
xmax           float       Maximum value of x coordinate of view box
ymin           float       Minimum value of y coordinate of view box
ymax           float       Maximum value of y coordinate of view box
zmin           float       Minimum value of z coordinate of view box
zmax           float       Maximum value of z coordinate of view box
**	Output args:
parameter      type        description 
vpx            float       x coordinate of view point
vpy            float       y coordinate of view point
vpz            float       z coordinate of view point
vdx            float       x value of view direction
vdy            float       y value of view direction
vdz            float       z value of view direction
umin           float       Minimum value of view horizon coordinate
vmin           float       Minimum value of view vertical coordinate
umax           float       Maximum value of view horizon coordinate
vmax           float       Maximum value of view vertical coordinate
depth          float       view depth
*/
Compute_Axo_Params(rot, slope, xmin,xmax, ymin,ymax, zmin, zmax, vpx, vpy, vpz, vdx, vdy, vdz, udx, udy, udz, umin, vmin, umax, vmax, depth)
{
	xxmin = xmin; 
	xxmax= xmax; 
	yymin= ymin; 
	yymax= ymax; 
	zzmin= zmin; 
	zzmax= zmax;

	set_m_x = (xxmin+xxmax)/2;
	set_m_y = (yymin+yymax)/2;
	set_m_z = (zzmin+zzmax)/2;
	
	xx2=(xxmax - xxmin)*(xxmax - xxmin);
	yy2=(yymax - yymin)*(yymax - yymin);
	zz2=(zzmax - zzmin)*(zzmax - zzmin);
	
	mbb_diag = SQRT( xx2+ yy2+ zz2);

    vdx = COS(rot)*COS(slope);
    vdy = SIN(rot)*COS(slope);
    vdz = SIN(slope);
    
    udx = COS(rot)*COS(slope+90);
    udy = SIN(rot)*COS(slope+90);
    udz = SIN(slope+90);    

	/*
		Position view point on the surface of a sphere with radius long enough so that
		the front plane does not clip objects in the active set.
	*/

	vpx = set_m_x + vdx*(mbb_diag/2 + 100);
	vpy = set_m_y + vdy*(mbb_diag/2 + 100);
	vpz = set_m_z + vdz*(mbb_diag/2 + 100);

	d = mbb_diag/2 + 100;

	/* Use default limits for axo view */
	umin = -d; umax = d; vmin = -d; vmax = d; depth = mbb_diag + 200;
	
	return(0);
}

/*获取数据库中的第一个标注风格的定义*/
GetFirstDraftingStyle(string style_name)
{
	obj_list = COS_QUERY("Drafting Style", "");
	nr_objs = A_SIZE(obj_list);
	if(nr_objs < 1){
		A_FREE(obj_list);
		return("");
	}
	cos_id = A_GET(obj_list, 0);
	COS_READ_OBJECT(cos_id);
	attrs = COS_GET_OBJECT_ATTRIBUTES(cos_id);
	if(ISINT(attrs)){
		COS_FREE_OBJECT(cos_id);
		A_FREE(obj_list);
		return("");
	}
	
	style_name = DM_GET_TAGVAL(attrs, ".dD");
	
	DM_FREE_TAGREC(attrs);
	COS_FREE_OBJECT(cos_id);
	A_FREE(obj_list);
	return(cos_id);
}

/* translate 3d point coordinate to view coordinate */
Translate_3D_To_2D(view_handle,view_scale,view_pos_x,view_pos_y,x,y,z)
{
   to_view = 1;      
   PM_VIEW_TRANSFORMATION(view_handle, to_view, x, y, z);
   x0 = x / view_scale;
   y0 = y / view_scale;
   z0 = z / view_scale;
   x = view_pos_x + x0;
   y = view_pos_y + y0;  
   return(0); 
}

/* return creation date of specified drawing */
Get_Creation_Date(string drawing_name)
{
   doc_guid = "";
   res = PM_FIND_DRAWING_WITH_NAME(drawing_name, doc_guid);
   if(doc_guid != ""){
      oid = COS_OID_FROM_GUIDSTRING(doc_guid);
      COS_READ_OBJECT(oid);
      props  = COS_GET_OBJECT_PROPERTIES(oid);      
      time_string = DM_GET_TAGVAL(props, COS_PROPERTY_TIME_CREATED);      
      time_int = String_To_Int(time_string);      
      date_string = CNV_DATE(time_int,7);      
      COS_FREE_OBJECT(oid);   
      return(date_string);
   }
   return(-1);
}

/*Added by Pekka Martin April 2010*/
Export_Active_Drw(format, template_dwr, output_dir)
{
	rec = DM_INIT_TAGREC(); 
	DM_SET_TAGVAL(rec, G2D_EXT_TAG_OUTPUT_DIR, output_dir);
	DM_SET_TAGVAL(rec, G2D_EXT_TAG_TEMPLATE_DRW, template_dwr);
	DM_SET_TAGVAL(rec, G2D_EXT_TAG_FORMAT, ITOASCII(format));
	DM_SET_TAGVAL(rec, G2D_EXT_TAG_ONE_TO_ONE_SCALING, "0");
	
	r = PM_EXPORT_ACTIVE_DRW(rec);
	
	DM_FREE_TAGREC(rec);
	
	return(r);
}

/* 初始化出图风格 */
InitDrawingSettings()
{
   /*
   **	TEXT params
   */
	Txt_color	= 1 ;
	Txt_pen		= 1 ;
	Txt_layer	= 101 ;
	Txt_type	= 0 ;
	Txt_height	= 2.0 ;
	Txt_width	= Txt_height * 0.7 ;
	Txt_spacing	= Txt_height * 0.15;
	Txt_slant	= 0.0 ;
	Txt_lorg	= 5 ;
	Txt_dirct	= 0.0 ;

   /*
**	LINE params
   */
	Lin_color	= 1   ;
	Lin_pen		= 1   ;
	Lin_layer	= 102   ;
	Lin_type	= 1   ;
	Lin_typelen	= 2.0 ;
	Lin_ends	= 1   ;

   /*
   **	SYMBOL params
   */
	Sym_color	= 1		;
	Sym_pen		= 1		;
	Sym_layer	= 103		;
	Sym_x_scale	= 1.0	;
	Sym_y_scale	= 1.0	;
	Sym_direct	= 0.0	;


   /*
   **	DIMENSIONLINE params
   */
	Dim_color		= 1     		;
	Dim_pen			= 1     		;
	Dim_layer		= 104     		;
	Dim_ofsetex		= 1.5   		;
	Dim_form		= "%Qa" 		;
	Dim_symheight	= 2.0   		;
	Dim_type		= 1     		;
	Dim_valdist		= 1.0   		;
	Dim_prefix		= ""    		;
	Dim_postfix		= ""    		;
	Dim_value		= ""    		;
	Dim_underline	= 0     		;
	Dim_txt_type	= 0				;
	Dim_txt_height	= Txt_height	;
	Dim_txt_width	= Txt_width		;
	Dim_txt_spacing	= Txt_spacing	;
	Dim_ext_line_gap= 1.0			;
	
/*
**	ARCDIMENSIONLINE params
*/
	ArcDim_ofsetex		= 1.5   		;
	ArcDim_form			= "%Qa" 		;
	ArcDim_symheight	= 2.0   		;
	ArcDim_valdist		= 1.15   	;
	ArcDim_prefix		= ""    		;
	ArcDim_postfix		= ""    		;
	ArcDim_value		= ""    		;
	ArcDim_underline	= 0     		;
	
/*
**	ARC params
*/
	Arc_color			=  1   ;
	Arc_pen				=  1   ;
	Arc_layer			=  105   ;

/*
**	HATCH params
*/
	Hatch_color			=  1   ;
	Hatch_pen			=  1   ;
	Hatch_layer			=  106   ;
	Hatch_style			=  1   ;
	Hatch_draw_boundary	=  1   ;
	Hatch_x_orig		=  0.0 ;
	Hatch_y_orig		=  0.0 ;
	Hatch_direct		= 45.0 ;
   return(0);
}

Set_Dimensioning_Properties(float scale)
{
	DW_TSLANT(0);
	DW_TTYPE(0);
	DW_DIMFORM("%Qa");
	
	DW_LAYER(DimLineLayer);
	DW_PEN(DimLinePen);
	DW_COLOR(DimLineColor);
	DW_TSIZE(scale * DimTextHeight, scale * DimTextWidth, scale * DimTextSpc);
	DW_DIMSYMHGHT(scale * DimSymHeight);
	
	DW_DIMVALDIST(scale * 1.0);
	DW_DIMOFFSETEX(scale * 1.0);
	DW_DIMSYM(1, 1);
	DW_ZLEVEL(0);
	
	return(0);
}


/* parameter       type     description
-- view_handle     handle   handle of dimensioning view
-- dim_type        int      
-- u1              float    x coordinate of first dimension point
-- v1              float    y coordinate of first dimension point  
-- w1              float    z coordinate of first dimension point 
-- u2              float    x coordinate of second dimension point
-- v2              float    y coordinate of second dimension point  
-- w2              float    z coordinate of second dimension point
-- dx              float    x value of text offset vector
-- dy              float    y value of text offset vector
-- dz              float    z value of text offset vector
-- location        int       
-- distance        int     
-- txt_h           float
-- txt_w           float
-- txt_s           float
-- sym_h           float
-- dim_text        string
-- gap             float   
*/
Add_2d_Dimension_A(handle view_h, int dim_type, float u1, float v1, float w1,
					float u2, float v2, float w2, float dx, float dy, float dz,
					int location, float distance, float txt_h, float txt_w, float txt_s, 
					float sym_h, string dim_text, float gap)
{
	dum = 0.0;	scale = 0.0;
	DW_GET_VIEW_PARAMS(dum, dum, dum, dum, dum, dum, scale);
	
	ddu = u2 - u1;
	ddv = v2 - v1;
	ddw = 0.0;
	
	/*if horizontal dimensioning*/
	if(dim_type == 1){
		if(ddu < 1.0 & ddu > -1.0){
		   return(0);
		}
		ddv = 0.0;
	}
	/* if vertical dimensioning */
	else if(dim_type == 4){
		if(ddv < 1.0 & ddv > -1.0){
		   return(0);
		}
		ddu = 0.0;
	}
	
	/* dimension text */
	len = VEC_UNITV(ddu, ddv, ddw);
	dim_string = "";
	S_PRINTF(dim_string, "%.0f", len);
	nr_char = STRLEN(dim_text);
	dim_string_len = nr_char * txt_w + (nr_char - 1) * txt_s;
	text_move = scale * (sym_h + dim_string_len/2);
	Set_Dimensioning_Properties(scale);
	level = distance + scale*gap;
		
	/* the normal */
	ndu = -ddv;
	ndv = ddu;
	if(FloatsEqual(v1, v2, 0.1))
		dim_type = 1;
	else if(FloatsEqual(u1, u2, 0.1))
		dim_type = 4;
	else if(FloatsEqual(v1, v2, 1.5) & FloatsEqual(ddv/ddu, 0, 0.005))
		dim_type = 1;
	else if(FloatsEqual(u1, u2, 1.5) & FloatsEqual(ddu/ddv, 0, 0.005))
		dim_type = 4;
	DW_DIMTYPE(dim_type);
	
	if(location == 1){
		DW_DIMSYM(2, 2);
		text_u = u1 - ddu * text_move + ndu * level;
		text_v = v1 - ddv * text_move + ndv * level;
		if(dim_type == 1 & v2 > v1)
			text_v = v2 + ndv * level;
		else if(dim_type == 4 & u2 < u1)
			text_u = u2 + ndu * level;
	}
	else if(location == 2){
		text_u = (u1 + u2)/2 + ndu * level;
		text_v = (v1 + v2)/2 + ndv * level;
		if(dim_type == 1 & v2 > v1)
			text_v = v2 + ndv * level;
		else if(dim_type == 1 & v2 < v1)
			text_v = v1 + ndv * level;
		else if(dim_type == 4 & u1 > u2)
			text_u = u2 + ndu * level;
		else if(dim_type == 4 & u1 < u2)
			text_u = u1 + ndu * level;
	}
	else if(location == 3){
		DW_DIMSYM(2, 2);
		text_u = u2 + ddu * text_move + ndu * level;
		text_v = v2 + ddv * text_move + ndv * level;
		if(dim_type == 1 & v1 > v2)
			text_v = v1 + ndv * level;
		else if(dim_type == 4 & u1 < u2)
			text_u = u1 + ndu * level;
	}
	
	DW_OPNDIM();
	DW_PNT(text_u, text_v, w1);	/* dim text level		*/
	DW_PNT(u1, v1, w1);				/* first end point		*/
	DW_DIMVAL("","", dim_text);	/* dimension value		*/
	DW_DIMVALORG(text_u, text_v);	/* dim text location	   */
	DW_PNT(u2, v2, w2);				/* second end point		*/
	DW_CLSDIM();
	
	return(0);
}

/*
	Add dimension between 2 points.
	Dimension can be centered or moved to the left side or right side by using
	argument 'location'. 
	
	One argument defines if dimensioning is horizontal, vertical or diagonal.
	Anyway if dimensioning points are horizontal or vertical (tolerance 1.5mm),
	then result is horizontal or vertical dimensioning even if type is
	defined to diagonal.
	
	Distance of dimension line is from the level of highest (if distance up)
	or lowest (if distance down) point if horizontal or vertical dimensioning
	is used for points, which are diagonal.
	
	Input arguments:
	handle	view_h		Handle of opened view
	int		dim_type	Type of dimension
						1 = horizontal
						4 = vertical
						7 = diagonal
	float	u1			U-coordinate of the point on the left
	float	v1			V-coordinate of the point on the left
	float	w1			W-coordinate of the point on the left
	float	u2			U-coordinate of the point on the right
	float	v2			V-coordinate of the point on the right
	float	w2			W-coordinate of the point on the right
	int		location	Dimension on the left side (1), center (2) or right (3)
	float	distance	Distance of dimension text from the level of points
	
	Return values:
	int		0			Success
*/
Add_2p_Dimension(handle view_h, int dim_type, float u1, float v1, float w1,
					float u2, float v2, float w2, int location, float distance,
					float txt_h, float txt_w, float txt_s, float sym_h, string dim_text, float gap)
{
	dum = 0.0;	scale = 0.0;
	DW_GET_VIEW_PARAMS(dum, dum, dum, dum, dum, dum, scale);
	
	ddu = u2 - u1;
	ddv = v2 - v1;
	ddw = 0.0;
	
	/* if it is horizontal dimension */
	if(dim_type == 1){
		if(ddu < 1.0 & ddu > -1.0) return(0);
		ddv = 0.0;
	}
	/* if it is vertical dimension */
	else if(dim_type == 4){
		if(ddv < 1.0 & ddv > -1.0) return(0);
		ddu = 0.0;
	}
	
	/* dimension text */
	len = VEC_UNITV(ddu, ddv, ddw);
	dim_string = "";
	S_PRINTF(dim_string, "%.0f", len);
	nr_char = STRLEN(dim_text);
	dim_string_len = nr_char * txt_w + (nr_char - 1) * txt_s;
	text_move = scale * (sym_h + dim_string_len/2);
	Set_Dimensioning_Properties(scale);
	level = distance + scale*gap;
	
	level = 2*level;
		
	/* the normal */
	ndu = -ddv;
	ndv = ddu;
	if(FloatsEqual(v1, v2, 0.1))
		dim_type = 1;
	else if(FloatsEqual(u1, u2, 0.1))
		dim_type = 4;
	else if(FloatsEqual(v1, v2, 1.5) & FloatsEqual(ddv/ddu, 0, 0.005))
		dim_type = 1;
	else if(FloatsEqual(u1, u2, 1.5) & FloatsEqual(ddu/ddv, 0, 0.005))
		dim_type = 4;
	DW_DIMTYPE(dim_type);
	
	if(location == 1){
		DW_DIMSYM(2, 2);
		text_u = u1 - ddu * text_move + ndu * level;
		text_v = v1 - ddv * text_move + ndv * level;
		if(dim_type == 1 & v2 > v1)
			text_v = v2 + ndv * level;
		else if(dim_type == 4 & u2 < u1)
			text_u = u2 + ndu * level;
	}
	else if(location == 2){
		text_u = (u1 + u2)/2 + ndu * level;
		text_v = (v1 + v2)/2 + ndv * level;
		if(dim_type == 1 & v2 > v1)
			text_v = v2 + ndv * level;
		else if(dim_type == 1 & v2 < v1)
			text_v = v1 + ndv * level;
		else if(dim_type == 4 & u1 > u2)
			text_u = u2 + ndu * level;
		else if(dim_type == 4 & u1 < u2)
			text_u = u1 + ndu * level;
	}
	else if(location == 3){
		DW_DIMSYM(2, 2);
		text_u = u2 + ddu * text_move + ndu * level;
		text_v = v2 + ddv * text_move + ndv * level;
		if(dim_type == 1 & v1 > v2)
			text_v = v1 + ndv * level;
		else if(dim_type == 4 & u1 < u2)
			text_u = u1 + ndu * level;
	}
	
	DW_OPNDIM();
	DW_PNT(text_u, text_v, w1);	/* dim text level		*/
	DW_PNT(u1, v1, w1);				/* first end point		*/
	DW_DIMVAL("","", dim_text);	/* dimension value		*/
	DW_DIMVALORG(text_u, text_v);	/* dim text location	   */
	DW_PNT(u2, v2, w2);				/* second end point		*/
	DW_CLSDIM();
	
	return(0);
}

/* parameter       type     description
-- view_handle     handle   handle of dimensioning view
-- dim_type        int      
-- u1              float    x coordinate of first dimension point
-- v1              float    y coordinate of first dimension point  
-- w1              float    z coordinate of first dimension point 
-- u2              float    x coordinate of second dimension point
-- v2              float    y coordinate of second dimension point  
-- w2              float    z coordinate of second dimension point
-- dx              float    x value of text offset vector
-- dy              float    y value of text offset vector
-- dz              float    z value of text offset vector
-- location        int       
-- distance        int     
-- txt_h           float
-- txt_w           float
-- txt_s           float
-- sym_h           float
-- dim_text        string
-- gap             float   
*/ 

Add_Linear_Dimension(handle view_h,float x1,float y1,float z1,float x2,float y2,float z2,float dx,float dy,float dz,float distance,float offset,float txt_h,float txt_w,float txt_s,float sym_h,string dim_text)
{
	dum = 0.0;	scale = 0.0;
	DW_GET_VIEW_PARAMS(dum, dum, dum, dum, dum, dum, scale);
	
	/*calculate and determine if draw this linear dimension*/
	v_dx = 0; v_dy = 0; v_dz = 0;
	PM_GET_VIEW_PARAMETERS(view_h,"VDIR",v_dx,v_dy,v_dz);
		
	angle = 0;
	GU_GetAngleBetweenTwoVectors(dx,dy,dz,v_dx,v_dy,v_dz,angle);
	if(angle < 45 & angle > -45){
	   return(0);
	}
	else if(angle > 135){
	   return(0);
	}
	else if(angle < - 135){
	   return(0);
	}	
	
	line_dx = x2-x1;
	line_dy = y2-y1;
	line_dz = z2-z1;
	VEC_UNITV(line_dx,line_dy,line_dz);
	angle = 0;
	GU_GetAngleBetweenTwoVectors(line_dx,line_dy,line_dz,v_dx,v_dy,v_dz,angle);
	if(angle < 45 & angle > -45){
	   return(0);
	}
	else if(angle > 135){
	   return(0);
	}
	else if(angle < - 135){
	   return(0);
	}
	
	distance = distance;	
	
	level = offset/scale;	
		
	/* dimension text */
	nr_char = STRLEN(dim_text);
	dim_string_len = nr_char * txt_w + (nr_char - 1) * txt_s;
	Set_Dimensioning_Properties(scale);
	
	mx1 = (x1+x2)/2;
	my1 = (y1+y2)/2;
	mz1 = (z1+z2)/2;
	
	mx2 = mx1 + 100*dx;
	my2 = my1 + 100*dy;
	mz2 = mz1 + 100*dz;

   PM_VIEW_TRANSFORMATION(view_h,1,mx1,my1,mz1);
	PM_VIEW_TRANSFORMATION(view_h,1,mx2,my2,mz2);
	
	off_dx = mx2 - mx1;
	off_dy = my2 - my1;
	off_dz = 0;
	
	VEC_UNITV(off_dx,off_dy,off_dz);
	
	text_u = mx1 + scale*(distance+level)*off_dx;
	text_v = my1 + scale*(distance+level)*off_dy;
	
   u1=x1;v1=y1;w1=z1;u2=x2;v2=y2;w2=z2;
   PM_VIEW_TRANSFORMATION(view_h,1,u1,v1,w1);
	PM_VIEW_TRANSFORMATION(view_h,1,u2,v2,w2);
	
	du = u2 - u1;
	dv = v2 - v1; 
	dw = 0;
	
	check_distance = VEC_UNITV(du,dv,dw);
	
	test_length = scale*(dim_string_len + 2*sym_h + 2);
	test_length1 = scale*(dim_string_len + 2*sym_h);
	
	DW_DIMSYM(1, 1);
	
	if(check_distance > test_length){
	   val_dis = - scale*txt_h/2;
	   DW_DIMVALDIST(val_dis);	   
	}
	else if(check_distance > test_length1){
	   val_dis = - scale*txt_h/2;
	   DW_DIMVALDIST(val_dis);	
	   DW_DIMSYM(2, 2);
	}
	else{
	   DW_DIMVALDIST(0.5*scale);	   
	   text_u = text_u - du*(check_distance/2 + dim_string_len/2*scale + scale*1);
	   text_v = text_v - dv*(check_distance/2 + dim_string_len/2*scale + scale*1);	   
	}
				
	DW_DIMTYPE(7);
	DW_OPNDIM();
	DW_PNT(text_u, text_v, w1);	/* dim text level		   */
	DW_PNT(u1, v1, w1);				/* first end point		*/
	DW_DIMVAL("","", dim_text);	/* dimension value		*/
	DW_DIMVALORG(text_u, text_v);	/* dim text location	   */
	DW_PNT(u2, v2, w2);				/* second end point		*/
	DW_CLSDIM();	
	return(0);
}

/* parameter       type     description
-- view_handle     handle   handle of dimensioning view      
-- x0              float    x coordinate of vertex point
-- y0              float    y coordinate of vertex point
-- z0              float    z coordinate of vertex point
-- x1              float    x coordinate of point on the start edge
-- y1              float    y coordinate of point on the start edge 
-- z1              float    z coordinate of point on the start edge
-- x2              float    x coordinate of point on the end edge
-- y2              float    y coordinate of point on the end edge
-- z2              float    z coordinate of point on the end edge      
-- distance        int     
-- txt_h           float
-- txt_w           float
-- txt_s           float
-- sym_h           float
-- dim_text        string 
*/ 
Add_Angle_Dimension(handle view_h,float x0,float y0, float z0, float x1, float y1, float z1, float x2,float y2,float z2,float txt_h,float txt_w,float txt_s,float sym_h,string dim_text)
{
	dum = 0.0;	scale = 0.0;
	DW_GET_VIEW_PARAMS(dum, dum, dum, dum, dum, dum, scale);
	
	v_dx = 0; v_dy = 0; v_dz = 0;
	PM_GET_VIEW_PARAMETERS(view_h,"VDIR",v_dx,v_dy,v_dz);

	dx1 = x1 - x0;
	dy1 = y1 - y0;
	dz1 = z1 - z0;
	VEC_UNITV(dx1,dy1,dz1);

	dx2 = x2 - x0;
	dy2 = y2 - y0;
	dz2 = z2 - z0;
	VEC_UNITV(dx2,dy2,dz2);
	
	dx=0;dy=0;dz=0;
	VEC_CROSS_PRODUCT(dx1,dy1,dz1,dx2,dy2,dz2,dx,dy,dz); 
	VEC_UNITV(dx,dy,dz);
		
	angle = 0;
	GU_GetAngleBetweenTwoVectors(dx,dy,dz,v_dx,v_dy,v_dz,angle);
	U_MESSAGE(FTOASCII(angle));	
	if((angle > 30 & angle < 90)| (angle > -90 & angle < -30)){
	   return(0);
	}
	else if(angle >= 90 & angle <= 150){
	   return(0);
	}
	else if(angle <= -90 & angle >= - 150){
	   return(0);
	}
			
	if(angle < -90 | angle > 90){
	   x = x1;y = y1;z = z1;
	   x1 = x2;y1 = y2;z1 = z2;
	   x2 = x;y2 = y;z2 = z;
	}
	
   u0=x0;v0=y0;w0=z0;u1=x1;v1=y1;w1=z1;u2=x2;v2=y2;w2=z2; 
   
   PM_VIEW_TRANSFORMATION(view_h,1,u0,v0,w0);
   PM_VIEW_TRANSFORMATION(view_h,1,u1,v1,w1);	
	PM_VIEW_TRANSFORMATION(view_h,1,u2,v2,w2);   
	
	du = u1-u0;
	dv = v1-v0;
	dw = 0;	
	dis = VEC_UNITV(du,dv,dw);	
	u1 = u0 + 0.7*dis*du;
	v1 = v0 + 0.7*dis*dv;

	du = u2-u0;
	dv = v2-v0;
	dw = 0;	
	dis = VEC_UNITV(du,dv,dw);	
	u2 = u0 + 0.7*dis*du;
	v2 = v0 + 0.7*dis*dv;	
		
	u3 = (u1+u2)/2; 
	v3 = (v1+v2)/2; 
	
   DW_DIMOFFSETSHORTER(0);
   DW_DIMOFFSETEX(1.0*scale);
   DW_DIMTYPE(10);
   DW_DIMSYM(1,1);
   DW_OPNDIM();
   DW_PNT(u1, v1, 0.0 );   
   DW_PNT(u0, v0, 0.0 );
   DW_DIMVAL("","°",dim_text);   
   DW_DIMVALORG(u3,v3);
   DW_PNT(u2, v2, 0.0 );
   DW_DIMVAL("","°",dim_text);   
   DW_DIMVALORG(u1,v1);
   DW_PNT(u0, v0, 0.0 );
   DW_DIMVALORG(u2,v2);
   dw_objid = DW_CLSDIM();
   return(dw_objid);
}

/* parameter       type     description
-- view_handle     handle   handle of dimensioning view
-- dim_type        int      
-- x1              float    x coordinate of first dimension point (center point)
-- y1              float    y coordinate of first dimension point (center point)
-- z1              float    z coordinate of first dimension point (center point)
-- x2              float    x coordinate of second dimension point (point on arc)
-- y2              float    y coordinate of second dimension point (point on arc)
-- z2              float    z coordinate of second dimension point (point on arc)      
-- distance        int     
-- txt_h           float
-- txt_w           float
-- txt_s           float
-- sym_h           float
-- dim_text        string 
*/ 
Add_Radius_Dimension(handle view_h, float x1, float y1, float z1,float x2, float y2, float z2, float distance, float txt_h, float txt_w, float txt_s, float sym_h, string dim_text)
{ 
	dum = 0.0;	scale = 0.0;
	DW_GET_VIEW_PARAMS(dum, dum, dum, dum, dum, dum, scale);
	
	v_dx = 0; v_dy = 0; v_dz = 0;
	PM_GET_VIEW_PARAMETERS(view_h,"VDIR",v_dx,v_dy,v_dz);
	
	dx = x2 - x1;
	dy = y2 - y1;
	dz = z2 - z1;
	VEC_UNITV(dx,dy,dz);
	
	angle = 0;
	GU_GetAngleBetweenTwoVectors(dx,dy,dz,v_dx,v_dy,v_dz,angle);
	if(angle < 60 & angle > -60){
	   return(0);
	}
	else if(angle > 120){
	   return(0);
	}
	else if(angle < - 120){
	   return(0);
	}
	
   u1=x1;v1=y1;w1=z1;u2=x2;v2=y2;w2=z2; 
   
   PM_VIEW_TRANSFORMATION(view_h,1,u1,v1,w1);	
	PM_VIEW_TRANSFORMATION(view_h,1,u2,v2,w2);
	
	dim_type = 7;		
	ddu = u2 - u1;
	ddv = v2 - v1;
	ddw = 0.0;
	VEC_UNITV(ddu, ddv, ddw);
		
	/* dimension text */
	nr_char = STRLEN(dim_text);
	dim_string_len = nr_char * txt_w + (nr_char - 1) * txt_s;
	text_move = scale * (sym_h + dim_string_len/2);
	Set_Dimensioning_Properties(scale);
	level = distance;
		
	/* the normal */
	ndu = -ddv;
	ndv = ddu;
		
	DW_DIMTYPE(dim_type);	
	DW_DIMSYM(3, 1);
	
	test_length = scale*(dim_string_len + 2*sym_h);
	line_length = SQRT(ddu*ddu+ddv*ddv);
	
	if(test_length >= line_length){
	   location = 1;
	}
	else{
	   location = 2;
	}
	
	if(location == 1){		
		text_u = u1 - ddu * text_move + ndu * level;
		text_v = v1 - ddv * text_move + ndv * level;
	}
	else if(location == 2){
		text_u = (u1 + u2)/2 + ndu * level;
		text_v = (v1 + v2)/2 + ndv * level;
	}
	else if(location == 3){
		text_u = u2 + ddu * text_move + ndu * level;
		text_v = v2 + ddv * text_move + ndv * level;
	}
	DW_DIMOFFSETEX(0);      
	DW_OPNDIM();
	DW_PNT(text_u, text_v, w1);	/* dim text level		*/
	DW_PNT(u1, v1, w1);				/* first end point		*/
	DW_DIMVAL("","", dim_text);	/* dimension value		*/
	DW_DIMVALORG(text_u, text_v);	/* dim text location	   */
	DW_PNT(u2, v2, w2);				/* second end point		*/
	DW_CLSDIM();
	
	return(0);
}

/* parameter       type     description
-- view_handle     handle   handle of view    
-- dx              float    x value of vector 
-- dy              float    y value of vector
-- dz              float    z value of vector
-- du              float    u value of vector
-- dv              float    v value of vector
*/
Vector_3D_To_2D(view_handle,dx,dy,dz,du,dv)
{
   to_view = 1; 
   x=0;y=0;z=0;     
   PM_VIEW_TRANSFORMATION(view_handle, to_view, x, y, z);
   x1=x+100*dx;
   y1=y+100*dy;
   z1=z+100*dz;
   PM_VIEW_TRANSFORMATION(view_handle, to_view, x1, y1, z1);
   du = x1-x;
   dv = y1-y;
   dw = z1-z;
   VEC_UNITV(du,dv,dw);  
   return(0); 
}

Get_Drawing_Form_Size_By_ICGD_Name(icgd)
{
	return("A4H");
}