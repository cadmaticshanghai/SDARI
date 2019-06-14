/*定义主窗口的列数*/
#define  Column_Number 7

/*管段信息对应的TAG定义*/

#define  SketchName     "U50"
/*#define  SketchName     "U3a"*/
#define  SketchStatus   "U39"
#define  CreationTime   ".c6"
#define  DocName        ".ba"


/* Duct shape type*/
#define  Duct_Shape                          ".mn"
#define  Rectangular_Bend                    1
#define  Rectangular_Asymmetric_Bend         2
#define  Rectangular_Transition              3
#define  Rectangular_T_branch                4
#define  Rectangular_To_Round                5
#define  Lateral_Radius_Y                    6
#define  Rectangular_Frame                   7
#define  Unequal_Circular_Radius_Lateral_Y   8
#define  Rectangular_End_Cap                 9
#define  Rounded_Rectangular_Boot_Outlet     10

/*定义管段视图位置*/
global int  View1_X = 65, View1_Y = 200;
global int 	View2_X = 155,View2_Y = 200;
global int 	View3_X = 65, View3_Y = 110;
global int 	View4_X = 155,View4_Y = 110;

/*定义管段视图大小*/
global int 	View_Size = 85;
global handle View_Info,View_Handles,View_Names,View_Positions;

/**/
global float View_Scale = 50;

/*三维视图的方向*/
global float AxoRotAngle=-120.0;
global float AxoSlopeAngle=30.0;

/*管段part列表及part数量*/
global int Spool_Part_Number = 0;
global handle Spool_Parts;

/*管段最大的PART数量*/
global int Max_Spool_Parts_Number = 50;


/*定义保存尺寸标注信息的全局数组*/
global handle Dim_Table;
global int    Dim_Number = 0;

/*保存管段零件表内容的全局数组*/
global handle Mto_Table;
global handle Mto_Label_Table;
global int Mto_Number = 0;

/*零件表相关设置*/
global int Mto_Table_Orign_X = 20;
global int Mto_Table_Orign_Y = 280;
global int Mto_Table_W = 185;
global int Mto_Table_H = 6;
global int Mto_Table_Column1_W = 10;
global int Mto_Table_Column2_W = 95;
global int Mto_Table_Column3_W = 30;
global int Mto_Table_Column4_W = 20;
global int Mto_Table_Column5_W = 15;
global int Mto_Table_Column6_W = 15;
global int Mto_Table_Line_Type = 0;
global int Mto_Table_Line_Colour = 1;
global int Mto_Table_Pattern_Length = 0;
global int Mto_Table_Text_Colour = 2;
global int Mto_Table_Text_Height = 2.5;

/*零件号标注设置*/
global int Mto_Label_Text_Colour = 1;
global int Mto_Label_Text_Height = 2.5;
global int Mto_Label_Leader_Line_Colour = 2;
global int Mto_Label_Leader_Line_Type = 0;
global int Mto_Label_Leader_Line_PattenLength = 0;
global int Mto_Label_Circle_Radius = 2;

/*管端连接表相关设置*/
global int Connection_Table_Orign_X = 115;
global int Connection_Table_Orign_Y = 52;
global int Connection_Table_W = 90;
global int Connection_Table_H = 4;
global int Connection_Table_Column1_W = 10;
global int Connection_Table_Column2_W = 15;
global int Connection_Table_Column3_W = 15;
global int Connection_Table_Column4_W = 15;
global int Connection_Table_Column5_W = 35;
global string Connection_Table_Direction = "Up"; /* Down - 从上向下 Up - 从下向上*/
global int Connection_Table_Line_Type = 0;
global int Connection_Table_Line_Colour = 1;
global int Connection_Table_Pattern_Length = 0;
global int Connection_Table_Text_Colour = 2;
global int Connection_Table_Text_Height = 2.5;

/*保存管段连接表标注信息的全局数组*/
global handle Connection_Table;
global handle Connection_Label_Table;
global int Connection_Label_Number = 2;
global int Max_Spool_Connection_Points = 10;

/*管端连接号标注设置*/
global int Connection_Label_Text_Colour = 1;
global int Connection_Label_Text_Height = 2.5;
global int Connection_Label_Leader_Line_Colour = 2;
global int Connection_Label_Leader_Line_Type = 0;
global int Connection_Label_Leader_Line_PattenLength = 0;
global int Connection_Label_Circle_Radius = 2;

/*保存所有标注信息的全局数组*/
global handle Label_Table;
global int Label_Number = 0;

/*字体设置*/
global string General_Font = "Arial Unicode MS";

/*尺寸标注设置*/
global int     DimLinePen = 1;
global string  DimLineWidthName = "0.25";
global int     DimLineColor = 1;
global string  DimLineColorName = "red";
global int     DimLineLayer = 104;
global string  DimLineLayerName = "PM_dim_layer";
global float   DimTextHeight = 2.5;
global float   DimTextWidth = 1.5;
global float   DimTextSpc = 0.5;
global float   DimSymHeight = 2.0;

/* 获取管段的名称 
**输入参数
**参数名       类型     说明
**ModelObject  Handle   Spool类型的ModelGroup的handle
**
**返回值
**返回管段名称的字符串，失败则返回整数-1;
*/
Get_SpoolName(handle ModelObject)
{	
	spool_number = PM_GET_OBJDATA(ModelObject, 0, MMT_COSA_SPOOL_NAME);
	area = PM_GET_OBJDATA(ModelObject, 0, MMT_TAG_OBJECT_IN_OUTFIT_AREA);
	if(ISINT(area)){
	   area_name = "";
	}
	else{
	   area_name = area;
	}
	system_name = PM_GET_OBJDATA(ModelObject, 0, MMT_TAG_SYSTEM_NAME);
	doc_name = PM_GET_OBJDATA(ModelObject, 0, DocName);
	parts = PM_GET_OBJECTS_IN_GROUP(ModelObject);
	if (!ISINT(parts)){
		part_number = PM_NR_MEMBERS_IN_SET(parts);
		line_name = "";
		if (part_number > 0){
		   for(i=0;i<part_number;i=i+1;){
   			part = PM_GET_MEMBER_IN_SET(parts,i);
   			line = PM_GET_OBJDATA(part,0,MMT_TAG_DUCT_LINE_NAME);
   			if(ISSTRING(line)){
   			   line_name = line;
   			}
   		}
   	   if (line_name == ""){
   		   return(-1);
   		}
		}
		else{
			return(-1);
		}
	}
	else{
		return(-1);
	}
	
	spool_name = doc_name + "-" + line_name + "-" + spool_number;	
	/*
	spool_name = area_name + "-" + line_name + "-" + spool_number;
	*/
	return(spool_name);
}


/*
	Get nearest points of two 3D lines (line A and line B). Two points are given from
	both lines. If lines are parallel (tolerance), then point 1 of line A and nearest
	point related to this point from line B are given as nearest points. Distance
	between nearest points and deviation between lines are given as additional output
	arguments.
	
	Input & Output arguments:
	handle	point_set	Handle of point set, where totally 6 points
						4 input points:
						index 0	= coordinates of the 1st point of 1st line
						index 1	= coordinates of the 2nd point of 1st line
						index 2	= coordinates of the 1st point of 2nd line
						index 3	= coordinates of the 2nd point of 2nd line
						2 output points:
						index 4	= coordinates of the nearest point of 1st line
						index 5	= coordinates of the nearest point of 2nd line
	float	distance	Distance between nearest points
	float	angle		Deviation between lines (-180 < deviation <= 180)
	float	tolerance	Tolerance of parallelity (usually 0.1 is quite good value)
						
	Return values:
	int		0			Success
	int		-1			Failure
*/
get_nearest_points_of_two_3D_lines(handle point_set, float distance, float angle,float tolerance)
{
	ax1 = 0.0;	ay1 = 0.0;	az1 = 0.0;
	ax2 = 0.0;	ay2 = 0.0;	az2 = 0.0;
	bx1 = 0.0;	by1 = 0.0;	bz1 = 0.0;
	bx2 = 0.0;	by2 = 0.0;	bz2 = 0.0;
	
	PM_GET_PNT(point_set, 0, ax1, ay1, az1);
	PM_GET_PNT(point_set, 1, ax2, ay2, az2);
	PM_GET_PNT(point_set, 2, bx1, by1, bz1);
	PM_GET_PNT(point_set, 3, bx2, by2, bz2);
	
	/* Direction of line A */
	adx = ax2 - ax1;
	ady = ay2 - ay1;
	adz = az2 - az1;
	len1 = VEC_UNITV(adx, ady, adz);
	if(len1 < 0.1)
		return(-1);
	
	/* Direction of line B */
	bdx = bx2 - bx1;
	bdy = by2 - by1;
	bdz = bz2 - bz1;
	len2 = VEC_UNITV(bdx, bdy, bdz);
	if(len2 < 0.1)
		return(-1);
	
	/* Deviation between lines A and B */
	dot_prod = 0.0;
	VEC_DOT_PRODUCT(adx, ady, adz, bdx, bdy, bdz, dot_prod);
	angle = ACOS(dot_prod);
	
	/* Transform line A point 1 to origin & along X-direction */
	orig_tm = PM_CREATE_TMAT(3, ax1, ay1, az1, adx, ady, adz, 0, 0, 0);
	inv_tm 	= PM_INVERSE_TMAT(orig_tm);
	
	/* Transform points of line B using previous matrix	*/
	/* Don't change original points						*/
	cbx1 = bx1;	cby1 = by1;	cbz1 = bz1;
	cbx2 = bx2;	cby2 = by2;	cbz2 = bz2;
	TRF_POINT(inv_tm, cbx1, cby1, cbz1);
	TRF_POINT(inv_tm, cbx2, cby2, cbz2);
	
	/* If lines are parallel within given tolerace */
	if(fleq(angle, 0, tolerance) | fleq(angle, 180, tolerance) | fleq(angle, -180, tolerance)){
		dx = 0.0;	dy = by1;	dz = bz1;
		distance = VEC_UNITV(dx, dy, dz);
		/* Get nearest point of line B related to point 1 of line A */
		nbx = 0.0;
		nby = by1;
		nbz = bz1;
		TRF_POINT(orig_tm, nbx, nby, nbz);
		PM_SET_PNT(point_set, 4, ax1, ay1, az1);
		PM_SET_PNT(point_set, 5, nbx, nby, nbz);
		return(0);
	}
	
	/* Get angle of line B projected to YZ-plane */
	bdy = cby2 - cby1;
	bdz = cbz2 - cbz1;
	
	/* Rotate points of line B around X-axis to Y-direction */
	bdx = 0.0;
	VEC_UNITV(bdx, bdy, bdz);
	rotate_tm = PM_CREATE_TMAT(1, 0, 0, 0, 1, 0, 0, bdx, bdy, bdz);
	inv_rotate_tm = PM_INVERSE_TMAT(rotate_tm);
	TRF_POINT(inv_rotate_tm, cbx1, cby1, cbz1);
	
	distance = cbz1;
	if(distance < 0)
		distance = -distance;
		
	/* Calculate X-intersection in transformed coordinate system */
	dy_10 = cby1;
	dy_21 = cby2 - cby1;
	if(dy_21 == 0){
		U_MESSAGE("Error in transformation!");
		return(0);
	}
	dx_21 = cbx2 - cbx1;
	dx_10 = dy_10/dy_21 * dx_21;
	x = cbx1 - dx_10;
	
	/* Transform to original coordinate system */
	nax = x;	nay = 0.0;	naz = 0.0;
	nbx = x;	nby = 0.0;	nbz = cbz1;
	TRF_POINT(rotate_tm, nax, nay, naz);
	TRF_POINT(rotate_tm, nbx, nby, nbz);
	TRF_POINT(orig_tm, nax, nay, naz);
	TRF_POINT(orig_tm, nbx, nby, nbz);
	
	PM_SET_PNT(point_set, 4, nax, nay, naz);
	PM_SET_PNT(point_set, 5, nbx, nby, nbz);
	
	return(0);
}
/*
fleq(float a, float b, float dif)
{
	if(a > b - dif & a < b + dif)
		return(TRUE);
	else
		return(FALSE);
}
*/
node_id_to_part_number(handle obj_handle, int node_id)
{
	obt = PM_GET_OBJDATA(obj_handle, 0, MMT_TAG_OBJTYPE);
	if(obt != "3") return(0);
	
	if(node_id == 1) return(1);	
	if(node_id == 2) return(PM_NR_PARTS_IN_PIPE(obj_handle));
	
	distance = 0.0;
	nr_of_part = PM_FIND_BRANCH_IN_PIPE(obj_handle, node_id, distance);
	if(nr_of_part > 0) return(nr_of_part);
	
	return(-1);
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

add_2p_dimension(handle view_h, int dim_type, float u1, float v1, float w1,
					float u2, float v2, float w2, int location, float distance,
					float txt_h, float txt_w, float txt_s, float sym_h, string dim_text, float gap)
{
	dum = 0.0;	scale = 0.0;
	DW_GET_VIEW_PARAMS(dum, dum, dum, dum, dum, dum, scale);
	
	ddu = u2 - u1;
	ddv = v2 - v1;
	ddw = 0.0;
	
	/*if horizontal dimensioning*/
	if(dim_type == 1){
		if(ddu < 1.0 & ddu > -1.0) return(0);
		ddv = 0.0;
	}
	/* if vertical dimensioning */
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
	set_dimensioning_properties(scale);
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

set_dimensioning_properties(float scale)
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

/* Added by Pekka Martin April 2010 */
export_active_drw(format, template_dwr, output_dir)
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
add_2d_dimension(handle view_h, int dim_type, float u1, float v1, float w1,
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
	set_dimensioning_properties(scale);
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
add_radius_dimension(handle view_h, float x1, float y1, float z1,float x2, float y2, float z2, float distance, float txt_h, float txt_w, float txt_s, float sym_h, string dim_text)
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
	set_dimensioning_properties(scale);
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
add_angle_dimension(handle view_h,float x0,float y0, float z0, float x1, float y1, float z1, float x2,float y2,float z2,float txt_h,float txt_w,float txt_s,float sym_h,string dim_text)
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

add_linear_dimension(handle view_h,float x1,float y1,float z1,float x2,float y2,float z2,float dx,float dy,float dz,float distance,float offset,float txt_h,float txt_w,float txt_s,float sym_h,string dim_text)
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
	set_dimensioning_properties(scale);
	
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