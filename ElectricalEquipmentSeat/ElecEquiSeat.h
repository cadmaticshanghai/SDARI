/*定义基座信息属性*/
#define	PadType				"~P2"			/* 基座垫片类型 */
#define	Part_Number			"~P0"			/* 基座零件编号 */
#define	SeatDes				"~G3"			/* 基座名称     */
#define	SeatType			"~G1"			/* 基座类型     */

#define	SeatPara			"SSP"			/* 基座参数     */
#define	NCH_Thickness		"!0H"			/* 船体板厚度   */
#define	SeatSize			"SSP"			/* 基座的尺寸参数   */					

global string	Group_Type  =  "7";
global int		Group_Type_I = 7;

global string	Earth_Bolt_Cata =  "ROC_ELEC_Earth_Bolt";
global string	Beam_part_Cata  =  "Equal Leg Angle";
global string	Hole_Part_Cata  =  "虚拟开孔零件";
global string	Beam_Part_Code  =  "";
global string	Steel_Plate_Code =  "";
global string	Hole_Part_Code  =  "";
global string	Tube_Part_Code  =  "";
global string Pad_Part_Code   =  "";

global string	Earth_Bolt_Pid1 = "";
global string	Earth_Bolt_Pid2 = "";
global string	Earth_Bolt_Pid3 = "";
global string	Beam_Part1_Pid  = "";
global string	Beam_Part2_Pid  = "";
global string	Steel_Plate_Pid = "";
global string	Steel_Plate2_Pid = "";
global string	Seat_Hole_Pid   = "";
global string	Pad_Plate_Pid   = "";

global string	Virtual_Hole_DT = "*Virtual*";
global string	Seat_Name_Prev	= "";
global string	Seat_Name_Index = "00";

global float	Rect_Pad_Gap	= 20;
global float	Circle_Pad_Gap	= 50;
global float	Panel_Default_Thickness	= 8;

global int		Max_Panel_Node_Number	= 10;
global float	Center_Hole_Diameter	= 10.0;
global float	Earth_Bolt_Rotate_Angle = 15.0;
global float	Default_Plate_Corner_R  = 15.0;
global float   Max_Angle_Tolerence     = 0.1;


/* 根据型材创建垫板 */
create_bar_pad_plate(seat, bar_handle, node, plate_id, system_id, part_num)
{
    hole_list = alloc_2darray(0, 0);
    bar_id = PM_GET_OBJDATA(bar_handle,0, MMT_TAG_PARTID);
    if(!ISSTRING(bar_id)){
        U_CONFIRM("型材pid为空，不能创建垫板。");
        return(-1);
    }
    curve = get_pad_curve(bar_id);
    if(curve==""){
        U_CONFIRM("该型材适用的垫板形状未定义。");
        return(-1);
    }
    
    /* 型材截面uv轴 */
    udx=0;udy=0;udz=0;vdx=0;vdy=0;vdz=0;wdx=0;wdy=0;wdz=0;
    get_bar_uvwdir(bar_handle,udx,udy,udz,vdx,vdy,vdz,wdx,wdy,wdz);
    px=0;py=0;pz=0;
    get_bar_node_pos(bar_handle,node,px,py,pz);
    tmat_h = PM_CREATE_TMAT(1,px,py,pz,udx,udy,udz,vdx,vdy,vdz);
    
    /*
    size = 1.0*DM_PARTID_DATA(bar_id, "D31");
    Point_3D_Move_With_DirAndDis(x2,y2,z2,x_dx,x_dy,x_dz,size/2);
    Point_3D_Move_With_DirAndDis(x2,y2,z2,x_dx,x_dy,x_dz,size/2);
    */
    
    part_handle = create_plate(plate_id, system_id, curve, tmat_h, hole_list);
    set_attribute_value(part_handle, Part_Number, part_num); 
    
    PM_ADD_OBJECT_TO_SET(part_handle, seat);
    return(part_handle);
}  

string_to_int(string int_string)
{
   int value;
   value = 0;
   value = SCAN_QUANT(3,int_string);
   return(value);
}

string_to_float(string float_string)
{
    value = 0.0;
    value = SCAN_QUANT(3,float_string);
    return(value);
}

dir_to_rec(tag, dx, dy, dz, rec)
{
	s = "";
	S_PRINTF(s, "%f %f %f", dx, dy, dz);
	DM_SET_TAGVAL(rec, tag, s);
}

point_to_rec(tag, x, y, z, rec)
{
	s = "";
	S_PRINTF(s, "%f %f %f", x, y, z);
	DM_SET_TAGVAL(rec, tag, s);
}

/* 设置指定模型对象属性的值 */
set_attribute_value(handle object_handle, string tag, value)
{
	/*根据属性TAG获取属性的handle*/
	attr_h = PM_FIND_ATTRIBUTE(tag);
	if (attr_h == 0) {
		message = "属性<" + tag + ">未定义";
		U_MESSAGE(message);
		return(-1);
	}

	/*获取属性的值类型*/
	type = PM_GET_ATTR_INFO(attr_h,"type");

	/*检查模型是否已经CHECKOUT*/
	set_handle = PM_INIT_SET();
	PM_ADD_OBJECT_TO_SET(object_handle,set_handle);
	res = PM_CHECK_OUT(set_handle,0);
	if(res<0){
		U_MESSAGE("你没有写权限");
		return(-1);
	}

	/*根据属性的值类型进行赋值*/
	if(type == 2){
		if(ISINT(value)){
			attr_value = ITOASCII(value);
		}
		else if(ISFLOAT(value)){
			attr_value = FTOASCII(value);
		}
		else{
			attr_value = value;
		}
		st = PM_ASSIGN_ATTRIBUTE(attr_h, object_handle, 0, attr_value);
		if(st<0){
			PM_DEASSIGN_ATTRIBUTE(attr_h, object_handle, 0);
			st = PM_ASSIGN_ATTRIBUTE(attr_h, object_handle, 0, attr_value);
		}
	}
	else{
		float_value = 0.0;
		if(ISFLOAT(value)){
			float_value = value;
		}
		else if(ISINT(value)){
			float_value = value;
		}
		else{
			float_value = string_to_float(value);
		}
		st = PM_ASSIGN_ATTRIBUTE(attr_h, object_handle, 0, float_value);
		if(st<0){
			PM_DEASSIGN_ATTRIBUTE(attr_h, object_handle, 0);
			st = PM_ASSIGN_ATTRIBUTE(attr_h, object_handle, 0, float_value);
			if (st < 0){
				int_value = 0;
				if(ISFLOAT(value)){
					int_value = string_to_int(FTOASCII(value));
				}
				else if(ISINT(value)){
					int_value = value;
				}
				else{
					int_value = string_to_int(value);
				}
				st = PM_ASSIGN_ATTRIBUTE(attr_h, object_handle, 0, int_value);
				if (st < 0){
					PM_DEASSIGN_ATTRIBUTE(attr_h, object_handle, 0);
					st = PM_ASSIGN_ATTRIBUTE(attr_h, object_handle, 0, int_value);
					if (st < 0){
						PM_FREE_SET(set_handle);
						return(-1);
					}
				}
			}
		}
	}
	PM_FREE_SET(set_handle);
	return(0);
}

/*角钢45度切角*/
anglebar_headside_a_cut(bar_handle,size)
{
    /* cut_para = "0 0 0 -22.5 0.0 -45.0;"; */
    bar_id = PM_GET_OBJDATA(bar_handle,0, MMT_TAG_PARTID);
    bar_width = 1.0 * DM_PARTID_DATA(bar_id, "D31");
    offset = bar_width-size;
    offset_str = rtrim_number(FTOASCII(-offset));
    cut_para = "0 0 0 "+offset_str+" 0.0 -45.0;";
    set_attribute_value(bar_handle, ".ky", cut_para);
}

/*角钢45度切角*/
anglebar_headside_b_cut(bar_handle,size)
{
    /* cut_para = "0 0 0 -22.5 -45.0 0.0;"; */
    bar_id = PM_GET_OBJDATA(bar_handle,0, MMT_TAG_PARTID);
    bar_width = 1.0 * DM_PARTID_DATA(bar_id, "D31");
    offset = bar_width-size;
    offset_str = rtrim_number(FTOASCII(-offset));
    cut_para = "0 0 0 "+offset_str+" -45.0 0.0;";
    set_attribute_value(bar_handle, ".ky", cut_para);
}

/*角钢45度切角*/
anglebar_tailside_a_cut(bar_handle,size)
{
    /* cut_para = "0 0 0 -22.5 0.0 -45.0;"; */
    bar_id = PM_GET_OBJDATA(bar_handle,0, MMT_TAG_PARTID);
    bar_width = 1.0 * DM_PARTID_DATA(bar_id, "D31");
    offset = bar_width-size;
    offset_str = rtrim_number(FTOASCII(-offset));
    cut_para = "0 0 0 "+offset_str+" 0.0 -45.0;";
    set_attribute_value(bar_handle, ".kz", cut_para);
}

/*角钢45度切角*/
anglebar_tailside_b_cut(bar_handle,size)
{
    /* cut_para = "0 0 0 -22.5 45.0 0.0;"; */
    bar_id = PM_GET_OBJDATA(bar_handle,0, MMT_TAG_PARTID);
    bar_width = 1.0 * DM_PARTID_DATA(bar_id, "D31");
    offset = bar_width-size;
    offset_str = rtrim_number(FTOASCII(-offset));
    cut_para = "0 0 0 "+offset_str+" 45.0 0.0;";
    set_attribute_value(bar_handle, ".kz", cut_para);
}

/* 创建板材 */
create_plate(string part_id, int sys_id, string curve, handle tmat_h, handle holes_array)
{
	/* Copy values, because we must not change original values */
	xt = 0;	   yt = 0;		zt = 0;
	xt_dx = 1;	xt_dy = 0;	xt_dz = 0;
	yt_dx = 0;	yt_dy = 1;	yt_dz = 0;
	
	/* Transform point and directions */
	TRF_POINT(tmat_h, xt, yt, zt);
	TRF_DIR(tmat_h, xt_dx, xt_dy, xt_dz);
	TRF_DIR(tmat_h, yt_dx, yt_dy, yt_dz);
	
	rec = DM_INIT_TAGREC();
	
	DM_SET_TAGVAL(rec, MMT_TAG_SYSTEM_ID, ITOASCII(sys_id));
	DM_SET_TAGVAL(rec, MMT_TAG_PARTID, part_id);
	point_to_rec(MMT_TAG_ORIGIN, xt, yt, zt, rec);
	dir_to_rec(MMT_TAG_LOCAL_XDIR, xt_dx, xt_dy, xt_dz, rec);
	dir_to_rec(MMT_TAG_LOCAL_YDIR, yt_dx, yt_dy, yt_dz, rec);
	
	p1 = "_UseDegrees = 1\; \norig = POINT(0,0,0)\; \ndir1 = DIRECTION(0,0,1)\; \ndir2 = DIRECTION(1,0,0)\; ";
	curve_beg = "\ncur = CURVE(";
	curve_end = ")\; ";
	
	hole_number = nrows_2darray(holes_array);
	gdl = "";
    if (hole_number <= 0){	
	    p2 = "\nsec = SECTION(cur)\; \nplt = PLATE(orig, dir1, dir2, sec, 20)\;";
	    gdl = p1 + curve_beg + curve + curve_end + p2;
	}
	else{
	    hole_list = "\n";
	    hole_pos = "";
	    for(i = 0;i<hole_number;i=i+1;){
	        hole_list = hole_list + get_2darray(holes_array,i,0)+"\n";
	        hole_pos = hole_pos + get_2darray(holes_array,i,1);	      
	    }
	    p2 = "\nsec = SECTION(cur";
	    p3 = ")\; \nplt = PLATE(orig, dir1, dir2, sec, 20)\;";
	    gdl = p1 + curve_beg + curve + curve_end + hole_list + p2 + hole_pos + p3;	   
	}		
	DM_SET_TAGVAL(rec, MMT_TAG_CMDLEMBGDL, gdl);
	
	inv_tag = "";
	obj = PM_INSERT_PLATE("STANDCMP", rec, inv_tag);
	
	DM_FREE_TAGREC(rec);
	
	if(!ISINT(obj)){
		return(obj);
	}

	U_MESSAGE("创建板材失败");
	U_MESSAGE("错误信息:" + DM_GET_ERRMSG(obj));
	if(inv_tag != "")
		U_MESSAGE("这个属性的值不正确: " + inv_tag);		
	return(0);
}

get_bar_node_pos(bar_handle,node,x,y,z)
{
    x = 1.0 * PM_GET_OBJDATA(bar_handle,0,"lox");
    y = 1.0 * PM_GET_OBJDATA(bar_handle,0,"loy");
    z = 1.0 * PM_GET_OBJDATA(bar_handle,0,"loz");
    if(node==1) return(0);
    
    len = 1.0 * PM_GET_OBJDATA(bar_handle,0,"len");
    udx=0;udy=0;udz=0;vdx=0;vdy=0;vdz=0;wdx=0;wdy=0;wdz=0;
    get_bar_uvwdir(bar_handle,udx,udy,udz,vdx,vdy,vdz,wdx,wdy,wdz);
    Point_Move(x,y,z, x,y,z, wdx,wdy,wdz, len);
    return(0);
}

get_bar_uvwdir(bar_handle,u_x,u_y,u_z,v_x,v_y,v_z,w_x,w_y,w_z)
{
    /*w normal direction of bar*/
    axs = PM_GET_OBJDATA(bar_handle,0,"axs");
    axr = PM_GET_OBJDATA(bar_handle,0,"axr");
    angle_to_direction(axs,axr, w_x,w_y,w_z);

    /*v axis direction of bar*/
    sxs = PM_GET_OBJDATA(bar_handle,0,"sxs");
    sxr = PM_GET_OBJDATA(bar_handle,0,"sxr");
    angle_to_direction(sxs,sxr, v_x,v_y,v_z);
    
    /*u axis direction of bar*/
    Vector3d_CrossProduct(v_x,v_y,v_z, w_x,w_y,w_z, u_x,u_y,u_z);
    return(0);
}

/*方向夹角转换为方向向量*/
angle_to_direction(ang_s,ang_r, dx,dy,dz)
{
    slope_angle = string_to_float(ang_s);
    rotation_angle = string_to_float(ang_r);
    dx = COS(slope_angle) * COS(rotation_angle);
    dy = COS(slope_angle) * SIN(rotation_angle);
    dz = SIN(slope_angle);
    VEC_UNITV(dx,dy,dz);
    return (0);
}

get_rectangular_curve(float length,float width,float radius)
{
    curve = "";
    if(radius == 0){
        p1x = length/2;
        p1y = width/2;
        p2x = p1x;
        p2y = - p1y;
        p3x = - p1x;
        p3y = - p1y;
        p4x = - p1x;
        p4y = p1y;
        
        p1xs = STRINGTERM(FTOASCII(p1x),".")+",";
        p1ys = STRINGTERM(FTOASCII(p1y),".")+",";
        p2xs = STRINGTERM(FTOASCII(p2x),".")+",";
        p2ys = STRINGTERM(FTOASCII(p2y),".")+",";
        p3xs = STRINGTERM(FTOASCII(p3x),".")+",";
        p3ys = STRINGTERM(FTOASCII(p3y),".")+",";
        p4xs = STRINGTERM(FTOASCII(p4x),".")+",";
        p4ys = STRINGTERM(FTOASCII(p4y),".");
        curve = curve+p1xs+p1ys+p2xs+p2ys+p3xs+p3ys+p4xs+p4ys;
    }
    else{
        p1x = -length/2;
        p1y = width/2-radius;
        p2x = p1x;
        p2y = - p1y;
        p3x = p1x - radius*SIN(45) + radius;
        p3y = p2y - radius*COS(45);
        p4x = -length/2 + radius;
        p4y = -width/2;
        p5x = -p4x;
        p5y = p4y;
        p6x = - p3x;
        p6y = p3y;
        p7x = - p2x;
        p7y = p2y;
        p8x = p7x;
        p8y = - p7y;
        p9x = p6x;
        p9y = -p6y;
        p10x = p5x;
        p10y = -p5y;
        p11x = p4x;
        p11y = -p4y;
        p12x = p3x;
        p12y = -p3y;

        p1xs = STRINGTERM(FTOASCII(p1x),".")+",";
        p1ys = STRINGTERM(FTOASCII(p1y),".")+",";
        p2xs = STRINGTERM(FTOASCII(p2x),".")+",";
        p2ys = STRINGTERM(FTOASCII(p2y),".")+",";
        p3xs = STRINGTERM(FTOASCII(p3x),".")+",";
        p3ys = STRINGTERM(FTOASCII(p3y),".")+",";
        p4xs = STRINGTERM(FTOASCII(p4x),".")+",";
        p4ys = STRINGTERM(FTOASCII(p4y),".")+",";
        p5xs = STRINGTERM(FTOASCII(p5x),".")+",";
        p5ys = STRINGTERM(FTOASCII(p5y),".")+",";
        p6xs = STRINGTERM(FTOASCII(p6x),".")+",";
        p6ys = STRINGTERM(FTOASCII(p6y),".")+",";
        p7xs = STRINGTERM(FTOASCII(p7x),".")+",";
        p7ys = STRINGTERM(FTOASCII(p7y),".")+",";
        p8xs = STRINGTERM(FTOASCII(p8x),".")+",";
        p8ys = STRINGTERM(FTOASCII(p8y),".")+",";
        p9xs = STRINGTERM(FTOASCII(p9x),".")+",";
        p9ys = STRINGTERM(FTOASCII(p9y),".")+",";
        p10xs = STRINGTERM(FTOASCII(p10x),".")+",";
        p10ys = STRINGTERM(FTOASCII(p10y),".")+",";
        p11xs = STRINGTERM(FTOASCII(p11x),".")+",";
        p11ys = STRINGTERM(FTOASCII(p11y),".")+",";
        p12xs = STRINGTERM(FTOASCII(p12x),".")+",";
        p12ys = STRINGTERM(FTOASCII(p12y),".")+",";
        p1ys0 = STRINGTERM(FTOASCII(p1y),".");
        curve = curve+p1xs+p1ys+p2xs+p2ys+"ARP,"+p3xs+p3ys+p4xs+p4ys+p5xs+p5ys+"ARP,"+p6xs+p6ys+p7xs+p7ys+p8xs+p8ys+"ARP,"+p9xs+p9ys+p10xs+p10ys+p11xs+p11ys+"ARP,"+p12xs+p12ys+p1xs+p1ys0;
    }
	return(curve);
}

get_circle_curve(float radius)
{
    radius_s = STRINGTERM(FTOASCII(radius),".");
	curve = "";
	curve = curve + radius_s + ",0,ARC,0,0,360";
	return(curve);
}

/* 三角垫板 */
get_triangle_curve(float width,float height, float t1,float t2, float gap,float radius)
{
    curve = "";
    if(radius == 0){
        p1x = -gap;
        p1y = -gap;
        p2x = width+((t1+gap)*(width-t2)+gap*SQRT((width-t2)*(width-t2)+(height-t1)*(height-t1)))/(height-t1);
        p2y = -gap;
        p3x = -gap;
        p3y = height+((t2+gap)*(height-t1)+gap*SQRT((width-t2)*(width-t2)+(height-t1)*(height-t1)))/(width-t2);
        
        p1xs = STRINGTERM(FTOASCII(p1x),".")+",";
        p1ys = STRINGTERM(FTOASCII(p1y),".")+",";
        p2xs = STRINGTERM(FTOASCII(p2x),".")+",";
        p2ys = STRINGTERM(FTOASCII(p2y),".")+",";
        p3xs = STRINGTERM(FTOASCII(p3x),".")+",";
        p3ys = STRINGTERM(FTOASCII(p3y),".");
        curve = curve+p1xs+p1ys+p2xs+p2ys+p3xs+p3ys;
    }
    else{
        p1x = -gap;
        p1y = -gap+radius;
        p2x = -gap+radius-radius*COS(45);
        p2y = -gap+radius-radius*COS(45);
        p3x = -gap+radius;
        p3y = -gap;
        angle = ATAN((width-t2)/(height-t1));
        /* 三角垫板 width&height是指角钢的尺寸 t1&t2是角钢厚度 */
        /* p4x = width+((t1+gap)*(width-t2)+gap*SQRT((width-t2)*(width-t2)+(height-t1)*(height-t1)))/(height-t1); */
        /* p4x = p4x-radius/(TAN(angle/2)); */
        /* 三角垫板 width&height修改为垫板外轮廓尺寸 t1&t2是角钢厚度 */
        p4x = width-2*radius;
        p4y = -gap;
        p5x = p4x+radius*COS(angle/2);
        p5y = -gap+radius-radius*SIN(angle/2);
        p6x = p4x+radius*COS(90-angle);
        p6y = p4y+radius+radius*SIN(90-angle);
        p9x = -gap;
        /* p9y = height+((t2+gap)*(height-t1)+gap*SQRT((width-t2)*(width-t2)+(height-t1)*(height-t1)))/(width-t2); */
        angle1 = (90-angle)/2;
        /* p9y = p9y-radius/SIN(angle1); */
        /* 三角垫板 width&height修改为垫板外轮廓尺寸 t1&t2是角钢厚度 */
        p9y = height-2*radius;
        
        p8x = p9x+radius-radius*SIN(angle1);
        p8y = p9y+radius*COS(angle1);
        p7x = p9x+radius+radius*COS(90-angle);
        p7y = p9y+radius*SIN(90-angle);
        p1xs = STRINGTERM(FTOASCII(p1x),".")+",";
        p1ys = STRINGTERM(FTOASCII(p1y),".")+",";
        p2xs = STRINGTERM(FTOASCII(p2x),".")+",";
        p2ys = STRINGTERM(FTOASCII(p2y),".")+",";
        p3xs = STRINGTERM(FTOASCII(p3x),".")+",";
        p3ys = STRINGTERM(FTOASCII(p3y),".")+",";
        p4xs = STRINGTERM(FTOASCII(p4x),".")+",";
        p4ys = STRINGTERM(FTOASCII(p4y),".")+",";
        p5xs = STRINGTERM(FTOASCII(p5x),".")+",";
        p5ys = STRINGTERM(FTOASCII(p5y),".")+",";
        p6xs = STRINGTERM(FTOASCII(p6x),".")+",";
        p6ys = STRINGTERM(FTOASCII(p6y),".")+",";
        p7xs = STRINGTERM(FTOASCII(p7x),".")+",";
        p7ys = STRINGTERM(FTOASCII(p7y),".")+",";
        p8xs = STRINGTERM(FTOASCII(p8x),".")+",";
        p8ys = STRINGTERM(FTOASCII(p8y),".")+",";
        p9xs = STRINGTERM(FTOASCII(p9x),".")+",";
        p9ys = STRINGTERM(FTOASCII(p9y),".")+",";
        p1ys0 = STRINGTERM(FTOASCII(p1y),".");
   	    curve = curve+p1xs+p1ys+"ARP,"+p2xs+p2ys+p3xs+p3ys+p4xs+p4ys+"ARP,"+p5xs+p5ys+p6xs+p6ys+p7xs+p7ys+"ARP,"+p8xs+p8ys+p9xs+p9ys+p1xs+p1ys0;
    }
    width1 = width+((t1+gap)*(width-t2)+gap*SQRT((width-t2)*(width-t2)+(height-t1)*(height-t1)))/(height-t1) + gap;
    height = height+((t2+gap)*(height-t1)+gap*SQRT((width-t2)*(width-t2)+(height-t1)*(height-t1)))/(width-t2) + gap;
    width = width1;
    return(curve);
}

/* 基座垫板类型匹配 */
get_pad_type(part_id)
{
    pat_type = "";
    mat_code = get_material2(part_id);
    if(mat_code=="A" | mat_code=="B" | mat_code=="C" | mat_code=="D" | mat_code=="E" | mat_code=="F"){
        pat_type = "A";
    }
    else if(mat_code=="G" | mat_code=="H" | mat_code=="I" | mat_code=="J" | mat_code=="K" | mat_code=="L"){
        pat_type = "C";
    }
    else if(mat_code=="S" | mat_code=="T"){
        pat_type = "B";
    }
    return pat_type;
}

/* 基座垫板轮廓 */
get_pad_curve(part_id)
{
    curve = "";
    mat_code = get_material2(part_id);
    if(mat_code=="A"){
        t = 1.0*DM_PARTID_DATA(part_id, "D32");
        width  = 50;
        height = 50;
        t1     = t;
        t2     = t;
        gap    = 15;
        radius = 15;
        curve = get_triangle_curve(width,height,t1,t2,gap,radius);
    }
    else if(mat_code=="B" | mat_code=="C"){
        t = 1.0*DM_PARTID_DATA(part_id, "D32");
        width  = 80;
        height = 80;
        t1     = t;
        t2     = t;
        gap    = 15;
        radius = 15;
        curve = get_triangle_curve(width,height,t1,t2,gap,radius);
    }
    else if(mat_code=="D" | mat_code=="E"){
        t = 1.0*DM_PARTID_DATA(part_id, "D32");
        width  = 100;
        height = 100;
        t1     = t;
        t2     = t;
        gap    = 15;
        radius = 15;
        curve = get_triangle_curve(width,height,t1,t2,gap,radius);
    }
    else if(mat_code=="F"){
        t = 1.0*DM_PARTID_DATA(part_id, "D32");
        width  = 130;
        height = 130;
        t1     = t;
        t2     = t;
        gap    = 15;
        radius = 15;
        curve = get_triangle_curve(width,height,t1,t2,gap,radius);
    }
    else if(mat_code=="G" | mat_code=="H"){
        d = 55;
        curve = get_circle_curve(0.5*d);
    }
    else if(mat_code=="I"){
        d = 65;
        curve = get_circle_curve(0.5*d);
    }
    else if(mat_code=="J"){
        d = 70;
        curve = get_circle_curve(0.5*d);
    }
    else if(mat_code=="K"){
        d = 80;
        curve = get_circle_curve(0.5*d);
    }
    else if(mat_code=="L"){
        d = 90;
        curve = get_circle_curve(0.5*d);
    }
    else if(mat_code=="S"){
        length = 55;
        width  = 30;
        radius = 0.5*width;
        curve = get_rectangular_curve(length,width,radius);
    }
    else if(mat_code=="T"){
        length = 80;
        width  = 30;
        radius = 0.5*width;
        curve = get_rectangular_curve(length,width,radius);
    }
    return curve;
}

/* 基座垫板厚度 */
check_pad_thickness(bar_part_id,pad_part_id)
{
    thk1 = get_pad_thickness(bar_part_id);
    thk2 = PM_GET_DIMENSION(pad_part_id, 3, 1);  
    if(thk1!=thk2){
        U_CONFIRM("所选垫板厚度与标准("+ITOASCII(thk1)+"t)不符，请重新选择！");
        return(0);
    }
    return(1);
}

/* 基座垫板厚度 */
get_pad_thickness(part_id)
{
    thk = 0;
    mat_code = get_material2(part_id);
    if(mat_code=="A" | mat_code=="B" | mat_code=="C" | mat_code=="D" | mat_code=="E"){
        thk = 8.0;  
    }
    else if(mat_code=="F"){
        thk = 10.0;  
    }
    else if(mat_code=="G" | mat_code=="H" | mat_code=="I" | mat_code=="J" | mat_code=="K" | mat_code=="L"){
        thk = 8.0;  
    }
    else if(mat_code=="S" | mat_code=="T"){
        thk = 8.0; 
    }
    return(thk);
}

/* 零件是否为等边角钢 */
is_equal_angle_bar(string pid)
{
	if(!ISSTRING(pid)) return(FALSE);
	shape = DM_PARTID_DATA(pid, "SH");
	if(shape == 61)
		return(TRUE);
	else if(shape == 62){
		len1 = DM_PARTID_DATA(pid, "D31");
		len2 = DM_PARTID_DATA(pid, "D32");
		if(len1 == len2) return(TRUE);
	}
	return(FALSE);
}

/* 零件是否为扁钢 */
is_flat_bar(string pid)
{
	if(!ISSTRING(pid)) return(FALSE);
	shape = DM_PARTID_DATA(pid, "SH");
	if(shape == 68) return(TRUE);
	return(FALSE);
}

/* 零件是否为钢板 */
is_steel_plate(string pid)
{
	if(!ISSTRING(pid)) return(FALSE);
	shape = DM_PARTID_DATA(pid, "SH");
	if(shape == 7) return(TRUE);
	return(FALSE);
}

/* 零件是否为钢管(圆钢) */
is_steel_pipe(string pid)
{
	if(!ISSTRING(pid)) return(FALSE);
	shape = DM_PARTID_DATA(pid, "SH");
	if(shape == "69") return(TRUE);
	return(FALSE);
}

/*材料1*/
get_material1(part_id)
{
    gt = DM_PARTID_DATA(part_id, "GT");
    
    /*型材*/
    if(gt == "1"){
        if(is_equal_angle_bar(part_id)){
            size1 = PM_GET_DIMENSION(part_id, 3, 1);  
            size2 = PM_GET_DIMENSION(part_id, 3, 2);  
            if(size1 == 30.0 & size2 == 4){
                return("A");
            }
            else if(size1 == 40.0 & size2 == 4){
                return("B");
            }
            else if(size1 == 50.0 & size2 == 5){
                return("C");
            }
            else if(size1 == 63.0 & size2 == 6){
                return("D");
            }
            else if(size1 == 75.0 & size2 == 8){
                return("E");
            }
            else if(size1 == 100.0 & size2 == 10){
                return("F");
            }
        }
        else if(is_flat_bar(part_id)){
            size1 = PM_GET_DIMENSION(part_id, 3, 1);  
            size2 = PM_GET_DIMENSION(part_id, 3, 2);  
            if(size1 == 40.0 & size2 == 4){
                return("S");
            }
            else if(size1 == 50.0 & size2 == 5){
                return("T");
            }
        }
        else if(is_steel_pipe(part_id)){
            U_CONFIRM("PIpe");
            return("");
        }
    }
    /*板材*/
    else if(gt == "15"){  
        size1 = PM_GET_DIMENSION(part_id, 3, 1);  
        if(size1 == 4.0){
            return("M");
        }
        else if(size1 == 5.0){
            return("N");
        }
        else if(size1 == 6.0){
            return("O");
        }
        else if(size1 == 8.0){
            return("P");
        }
        else if(size1 == 10.0){
            return("Q");
        }
    }
    return("");
}

/*材料2*/
get_material2(part_id)
{
    gt = DM_PARTID_DATA(part_id, "GT");
    
    /*型材*/
    if(gt == "1"){
        if(is_equal_angle_bar(part_id)){
            size1 = PM_GET_DIMENSION(part_id, 3, 1);  
            size2 = PM_GET_DIMENSION(part_id, 3, 2);  
            if(size1 == 30.0 & size2 == 4){
                return("A");
            }
            else if(size1 == 40.0 & size2 == 4){
                return("B");
            }
            else if(size1 == 50.0 & size2 == 5){
                return("C");
            }
            else if(size1 == 63.0 & size2 == 6){
                return("D");
            }
            else if(size1 == 75.0 & size2 == 8){
                return("E");
            }
            else if(size1 == 100.0 & size2 == 10){
                return("F");
            }
        }
        else if(is_flat_bar(part_id)){
            size1 = PM_GET_DIMENSION(part_id, 3, 1);  
            size2 = PM_GET_DIMENSION(part_id, 3, 2);  
            if(size1 == 40.0 & size2 == 4){
                return("S");
            }
            else if(size1 == 50.0 & size2 == 5){
                return("T");
            }
        }
        else if(is_steel_pipe(part_id)){
            size1 = PM_GET_DIMENSION(part_id, 3, 1);  
            size2 = PM_GET_DIMENSION(part_id, 3, 2);  
            if(size2 == 19.0){
                return("R");
            }    
            else if(size2 == 27.0){
                return("G");
            }
            else if(size2 == 34.0){
                return("H");
            }  
            else if(size2 == 42.0){
                return("I");
            }   
            else if(size2 == 48.0){
                return("J");
            }    
            else if(size2 == 60.0){
                return("K");
            }     
            else if(size2 == 76.0){
                return("L");
            }
        }
    }  
    /*钢板*/
    else if(gt == "15"){  
        size1 = PM_GET_DIMENSION(part_id, 3, 1);  
        if(size1 == 4.0){
            return("M");
        }
        else if(size1 == 5.0){
            return("N");
        }
        else if(size1 == 6.0){
            return("O");
        }
        else if(size1 == 8.0){
            return("P");
        }
        else if(size1 == 10.0){
            return("Q");
        }
    }
    return("");    
}

/*向量的叉积*/      
Vector3d_CrossProduct(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float dx3, float dy3, float dz3)
{
    dx3 = dy1*dz2 - dz1*dy2;
    dy3 = dz1*dx2 - dx1*dz2;
    dz3 = dx1*dy2 - dy1*dx2;
} 

/*点1延方向移动一定距离得到点2*/
Point_Move(float x1, float y1, float z1, float x2, float y2, float z2, float v_x, float v_y, float v_z, float distance)
{
    vx = 0.0; vy=0.0; vz=0.0;
    Vector3d_ToUnitVector(v_x,v_y,v_z, vx,vy,vz);
    x2 = x1+vx*distance;
    y2 = y1+vy*distance;
    z2 = z1+vz*distance;
    return (1);
}

/*向量的单位向量*/      
Vector3d_ToUnitVector(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2)
{
    length = Vector3d_Length(dx1, dy1, dz1);
    if (length >= 1.0E-15){
        dx2 = dx1 / length;
        dy2 = dy1 / length;
        dz2 = dz1 / length;
    }
    else{
        dx2 = dx1;
        dy2 = dy1;
        dz2 = dz1;
    }
} 

/*向量的模*/      
Vector3d_Length(float dx, float dy, float dz)
{
    return (SQRT(dx*dx + dy*dy + dz*dz));
} 

/*基座参数处理*/
handle_para(float para)
{
    /* 保留2位小数 */
    return rtrim_number(round_float_as_string(para, 2));
}

/*数值字符串右剔除多余的0*/
rtrim_number(string str_val)
{
    res_str = SEARCH(str_val, ".");
    if(STRLEN(res_str)){
        res_str = str_val;
        str_len = STRLEN(str_val);
        for (i=0;i<str_len;i=i+1){
            temp = TAIL(res_str,1);
            if(temp=="."){
                res_str = HEAD(res_str,STRLEN(res_str)-1);
                return(res_str);
            }
            else if(temp=="0"){
                res_str = HEAD(res_str,STRLEN(res_str)-1);
            }
            else{
                return(res_str);
            }
        }
    }
    return(str_val);
}

/* 保留n位小数点有效数字 */
round_float_as_string(float x, int n)
{
    flg = 1;
    for(i=0;i<n;i=i+1){
        flg = flg*10;
    }
    temp = x*flg+0.5;
    temp_str = STRINGTERM(FTOASCII(temp), ".");
    part1 = HEAD(temp_str,STRLEN(temp_str)-n);
    if(part1==""){
        part1="0";
    }
    part2 = TAIL(temp_str,n);
    while(STRLEN(part2)<n){
        part2 = "0" + part2;
    }

    if(n==0){
        return(part1);
    }
    return(part1+"."+part2);
}