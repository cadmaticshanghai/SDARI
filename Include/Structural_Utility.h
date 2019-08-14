/* 检查零件是否为等边角钢 */
Is_Equal_Angle_Bar(string pid)
{
	if(!ISSTRING(pid))
		return(FALSE);

	shape = DM_PARTID_DATA(pid, "SH");
	if(shape == 61)
		return(TRUE);
	else if(shape == 62){
		len1 = DM_PARTID_DATA(pid, "D31");
		len2 = DM_PARTID_DATA(pid, "D32");
		if(len1 == len2)
			return(TRUE);
		else {
		   beam_descr = Pid_To_Description(pid);
			msg = "<" + beam_descr + "> 不是等边角钢!";
			U_MESSAGEBOX(msg, U_MB_ERROR, U_MB_OK, U_OK);
			return(FALSE);
		}
	}
	else {
	   beam_descr = Pid_To_Description(pid);
		msg = "<" + beam_descr + "> 不是等边角钢!";
		U_MESSAGEBOX(msg, U_MB_ERROR, U_MB_OK, U_OK);
		return(FALSE);
	}
	return(FALSE);
}

/* 检查零件是否为等边角钢 */
Is_Channel_Bar(string pid)
{
	if(!ISSTRING(pid)){
		return(FALSE);
	}

	shape = DM_PARTID_DATA(pid, "SH");
	if(shape == 64){
		return(TRUE);
	}
	return(FALSE);
}

Is_Plate_Ok(string pid)
{
	if(!ISSTRING(pid))
		return(FALSE);

	geo_type = DM_PARTID_DATA(pid, "GT");
	if(geo_type == 15){
		dt_name = DM_PARTID_DATA(pid, "DT");
		plate_type = DM_DIMTBL_NAMED_DIM(dt_name, 0, "PlateType");
		if(plate_type == -1){
			msg = "PlateType not defined in the dimension table of Plate";
			U_MESSAGEBOX(msg, U_MB_ERROR, U_MB_OK, U_OK);
			return(FALSE);
		}
		else if(plate_type != 0 & plate_type != 1){
			msg = "PlateType <" + ITOASCII(plate_type) + "> is not supported";
			U_MESSAGEBOX(msg, U_MB_ERROR, U_MB_OK, U_OK);
			return(FALSE);
		}
		return(TRUE);
	}
	else {
		msg = "<" + pid + "> is not a valid fixing plate!";
		U_MESSAGEBOX(msg, U_MB_ERROR, U_MB_OK, U_OK);
		return(FALSE);
	}	
	return(FALSE);
}

/*
变量说明：
part_id           -- 型材的零件id
sys_id            -- 型材所属系统的id
x1,y1,z1          -- 型材起始点坐标
x2,y2,z2          -- 型材结束点坐标
x_dx,x_dy,x_dz    -- 型材截面X坐标轴的方向
e1_dx,e1_dy,e1_dz -- 型材起始点切面的法线方向
e2_dx,e2_dy,e2_dz -- 型材结束点切面的法线方向
tmat_h            -- 型材位置及方向的变换矩阵
*/
Create_Beam(string part_id, int sys_id, float x1, float y1, float z1, float x2, float y2, float z2, float x_dx, float x_dy, float x_dz,float e1_dx, float e1_dy, float e1_dz, float e2_dx, float e2_dy, float e2_dz, handle tmat_h)
{
	/* 将传递过来的参数值拷贝到本地变量，这样就不会改变传递过来的变量的值 */
	
	x1t = x1;		y1t = y1;		z1t = z1;
	x2t = x2;		y2t = y2;		z2t = z2;
	xt_dx = x_dx;	xt_dy = x_dy;	xt_dz = x_dz;
	e1t_dx = e1_dx;	e1t_dy = e1_dy;	e1t_dz = e1_dz;
	e2t_dx = e2_dx;	e2t_dy = e2_dy;	e2t_dz = e2_dz;
	
	/* 转换点和方向 */
	TRF_POINT(tmat_h, x1t, y1t, z1t);
	TRF_POINT(tmat_h, x2t, y2t, z2t);
	TRF_DIR(tmat_h, xt_dx, xt_dy, xt_dz);
	TRF_DIR(tmat_h, e1t_dx, e1t_dy, e1t_dz);
	TRF_DIR(tmat_h, e2t_dx, e2t_dy, e2t_dz);
	VEC_UNITV(xt_dx, xt_dy, xt_dz);
	VEC_UNITV(e1t_dx, e1t_dy, e1t_dz);
	VEC_UNITV(e2t_dx, e2t_dy, e2t_dz);
	
	rec = DM_INIT_TAGREC();
	
	DM_SET_TAGVAL(rec, MMT_TAG_SYSTEM_ID, ITOASCII(sys_id));
	DM_SET_TAGVAL(rec, MMT_TAG_PARTID, part_id);
	Point_To_Rec(MMT_TAG_1st_ENDPOINT, x1t, y1t, z1t, rec);
	Point_To_Rec(MMT_TAG_2nd_ENDPOINT, x2t, y2t, z2t, rec);
	Dir_To_Rec(MMT_TAG_LOCAL_XDIR,  xt_dx, xt_dy, xt_dz, rec);
	Dir_To_Rec(MMT_TAG_1st_ENDNORMAL, e1t_dx, e1t_dy, e1t_dz, rec);
	Dir_To_Rec(MMT_TAG_2nd_ENDNORMAL, e2t_dx, e2t_dy, e2t_dz, rec);
	
	inv_tag = "";
	obj = PM_CREATE_OBJECT("BEAM", rec, inv_tag);
	
	DM_FREE_TAGREC(rec);
	if(!ISINT(obj)){
		return(obj);
	}

	U_MESSAGE("生成型材失败");
	U_MESSAGE("错误信息: " + DM_GET_ERRMSG(obj));
	if(inv_tag != "")
		U_MESSAGE("Bad value in : " + inv_tag);
		
	return(0);
}

/*
变量说明：
part_id           -- 标准舾装件的零件id
sys_id            -- 标准舾装所属系统的id
x,y,z             -- 标准舾装本地坐标原点坐标
x_dx,x_dy,x_dz    -- 标准舾装本地坐标系X坐标轴的方向
y_dx,y_dy,y_dz    -- 标准舾装本地坐标系Y坐标轴的方向
tmat_h            -- 标准舾装位置及方向的变换矩阵
*/
Create_Standcomp(string part_id, int sys_id, float x, float y, float z, float x_dx, float x_dy, float x_dz, float y_dx, float y_dy, float y_dz, handle tmat_h)
{
	/* Copy values, because we must not change original values */
	xt = x;			yt = y;			zt = z;
	xt_dx = x_dx;	xt_dy = x_dy;	xt_dz = x_dz;
	yt_dx = y_dx;	yt_dy = y_dy;	yt_dz = y_dz;
	
	/* Transform point and directions */
	TRF_POINT(tmat_h, xt, yt, zt);
	TRF_DIR(tmat_h, xt_dx, xt_dy, xt_dz);
	TRF_DIR(tmat_h, yt_dx, yt_dy, yt_dz);
	
	rec = DM_INIT_TAGREC();
	
	DM_SET_TAGVAL(rec, MMT_TAG_SYSTEM_ID, ITOASCII(sys_id));
	DM_SET_TAGVAL(rec, MMT_TAG_PARTID, part_id);
	Point_To_Rec(MMT_TAG_ORIGIN, xt, yt, zt, rec);
	Dir_To_Rec(MMT_TAG_LOCAL_XDIR, xt_dx, xt_dy, xt_dz, rec);
	Dir_To_Rec(MMT_TAG_LOCAL_YDIR, yt_dx, yt_dy, yt_dz, rec);
	
	inv_tag = "";
	obj = PM_CREATE_OBJECT("STANDCMP", rec, inv_tag);
	
	DM_FREE_TAGREC(rec);
	
	if(!ISINT(obj)){
		return(obj);
	}

	U_MESSAGE("无法创建标准零件");
	U_MESSAGE("错误信息: " + DM_GET_ERRMSG(obj));
	if(inv_tag != "")
		U_MESSAGE("此属性的值有错误: " + inv_tag);
		
	return(0);
}

/*
变量说明：
part_id           -- 板材的零件id
sys_id            -- 板材所属系统的id
curve             -- 板材截面各个顶点坐标描述字符串
tmat_h            -- 板材位置及方向的变换矩阵
holes_array       -- 板材上的开孔信息
*/
Create_Plate(string part_id, int sys_id, string curve, handle tmat_h, handle holes_array)
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
	Point_To_Rec(MMT_TAG_ORIGIN, xt, yt, zt, rec);
	Dir_To_Rec(MMT_TAG_LOCAL_XDIR, xt_dx, xt_dy, xt_dz, rec);
	Dir_To_Rec(MMT_TAG_LOCAL_YDIR, yt_dx, yt_dy, yt_dz, rec);
	
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

/*
Parameters：
beam_handle    handle      handle of beam part           
point_flag     int         indicate which end will be trimed or stretched 1 - start point, 2 - end point
length         float       specify the trim or stretch length, positive will be stretched, nagative will be trim
*/
Trim_Or_Stretch_Beam(beam_handle,point_flag,length)
{
	x1 = String_To_Float(PM_GET_OBJDATA(beam_handle,0,MMT_TAG_ORIG_X));
	y1 = String_To_Float(PM_GET_OBJDATA(beam_handle,0,MMT_TAG_ORIG_Y));
	z1 = String_To_Float(PM_GET_OBJDATA(beam_handle,0,MMT_TAG_ORIG_Z));
	len = String_To_Float(PM_GET_OBJDATA(beam_handle,0,MMT_TAG_AXIS_LEN));
	axis_rotate = String_To_Float(PM_GET_OBJDATA(beam_handle,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(beam_handle,0,MMT_TAG_AXIS_SLP_ANGLE));
	dx = COS(axis_slope)*COS(axis_rotate);
	dy = COS(axis_slope)*SIN(axis_rotate);
	dz = SIN(axis_slope);
	x2 = x1 + len * dx;
	y2 = y1 + len * dy;
	z2 = z1 + len * dz;   
	res = 0;
	if(point_flag == 1){
	   x = x1 - length*dx;
	   y = y1 - length*dy;
	   z = z1 - length*dz;
	   rec = DM_INIT_TAGREC();
	   Point_To_Rec(MMT_TAG_1st_ENDPOINT, x, y, z, rec);
	   inv_tag = "";
	   res = PM_SET_OBJECT(beam_handle, rec, inv_tag);	
	   DM_FREE_TAGREC(rec);
	}	
	else{   
	   x = x2 + length*dx;
	   y = y2 + length*dy;
	   z = z2 + length*dz;
	   rec = DM_INIT_TAGREC();
	   Point_To_Rec(MMT_TAG_2nd_ENDPOINT, x, y, z, rec);
	   inv_tag = "";
	   res = PM_SET_OBJECT(beam_handle, rec, inv_tag);	
	   DM_FREE_TAGREC(rec);	   
	}   
   return(res);
}

Get_Earth_Bolt_Para(part_handle,lox,loy,loz,udx,udy,udz,vdx,vdy,vdz)
{
	lox = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_X));
	loy = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Y));
	loz = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Z));  
	axis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_SLP_ANGLE));
	udx = COS(axis_slope)*COS(axis_rotate);
	udy = COS(axis_slope)*SIN(axis_rotate);
	udz = SIN(axis_slope);
	saxis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_ROT_ANGLE));
	saxis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_SLP_ANGLE));
	vdx = COS(saxis_slope)*COS(saxis_rotate);
	vdy = COS(saxis_slope)*SIN(saxis_rotate);
	vdz = SIN(saxis_slope);	
   return(0);	 
}

Get_Beam_Para(part_handle,x1,y1,z1,x2,y2,z2,x_dx,x_dy,x_dz,end1x,end1y,end1z,end2x,end2y,end2z)
{
	x1 = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_X));
	y1 = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Y));
	z1 = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Z));
	length = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_LEN));
	axis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_SLP_ANGLE));
	dx = COS(axis_slope)*COS(axis_rotate);
	dy = COS(axis_slope)*SIN(axis_rotate);
	dz = SIN(axis_slope);
	x2 = x1 + length * dx;
	y2 = y1 + length * dy;
	z2 = z1 + length * dz;
	x_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_ROT_ANGLE));
	x_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_SLP_ANGLE));
	x_dx = COS(x_slope)*COS(x_rotate);
	x_dy = COS(x_slope)*SIN(x_rotate);
	x_dz = SIN(x_slope);	
	dx = x1-x2;
	dy = y1-y2;
	dz = z1-z2;
	VEC_UNITV(dx,dy,dz);
	end1x = dx;
	end1y = dy;
	end1z = dz; 
	end2x = -dx;
	end2y = -dy;
	end2z = -dz;			 
	return(0);  
}

Get_Plate_Para(part_handle,lox,loy,loz,udx,udy,udz,vdx,vdy,vdz)
{
	lox = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_X));
	loy = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Y));
	loz = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Z));   
	axis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_SLP_ANGLE));
	udx = COS(axis_slope)*COS(axis_rotate);
	udy = COS(axis_slope)*SIN(axis_rotate);
	udz = SIN(axis_slope);
	saxis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_ROT_ANGLE));
	saxis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_SLP_ANGLE));
	vdx = COS(saxis_slope)*COS(saxis_rotate);
	vdy = COS(saxis_slope)*SIN(saxis_rotate);
	vdz = SIN(saxis_slope);	
   return(0);
}

Create_Circle_Curve(float radius)
{
    radius_s = STRINGTERM(FTOASCII(radius),".");
	curve = "";
	curve = curve + radius_s + ",0,ARC,0,0,360";
	return(curve);
}

Create_Rectangular_Curve(float length,float width,float radius)
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

Create_Triangle_Curve(float width,float height, float t1,float t2, float gap,float radius)
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
      p3x = -gap + radius;
      p3y = -gap;
      angle = ATAN((width-t2)/(height-t1));
      p4x = width+((t1+gap)*(width-t2)+gap*SQRT((width-t2)*(width-t2)+(height-t1)*(height-t1)))/(height-t1);
      p4x = p4x-radius/(TAN(angle/2)) ;
      p4y = -gap;
      p5x = p4x+radius*COS(angle/2);
      p5y = -gap+radius-radius*SIN(angle/2);
      p6x = p4x+radius*COS(90-angle);
      p6y = p4y+radius+radius*SIN(90-angle);
      p9x = -gap;
      p9y = height+((t2+gap)*(height-t1)+gap*SQRT((width-t2)*(width-t2)+(height-t1)*(height-t1)))/(width-t2);
      angle1 = (90-angle)/2;
      p9y = p9y-radius/SIN(angle1);
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

/*This function use to calculate surface area of BEAM
**Surface area of user defined profile doesn't archieve
**The effect of end cut of beam isn't considered
*/
Calculate_Beam_Surface_Area(obj_h)
{
    nth = 0;
    part_id = PM_GET_OBJDATA(obj_h, nth, MMT_TAG_PARTID);
    shape = DM_PARTID_DATA(part_id,"SH");
    /*user define profile*/
    if(shape == "60"){
        return(0.0);
    }
    else if(shape == "61" | shape == "67"){
        l = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,1);
        round = 4*l;
        length = String_To_Float(PM_GET_OBJDATA(obj_h, nth, MMT_TAG_LENGTH));
        area = round * length / 1000000.0;
        return(area);
    }
    else if(shape == "62" | shape == "63"  | shape == "65" | shape == "68"){
        h = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,1);
        w = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,2);
        round = 2*(h+w);
        length = String_To_Float(PM_GET_OBJDATA(obj_h, nth, MMT_TAG_LENGTH));
        area = round * length / 1000000.0;
        return(area);
    }
    else if(shape == "64" | shape == "66"){
        h = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,1);
        w = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,2);
        th = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,3);
        tw = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,4);
        round = 2*(h+2*w-th);
        length = String_To_Float(PM_GET_OBJDATA(obj_h, nth, MMT_TAG_LENGTH));
        area = round * length / 1000000.0;
        return(area);
    }
    else if(shape == "69"){
        n = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,1);
        d = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,2);
        round = n*d*SIN(180.0/n);
        length = String_To_Float(PM_GET_OBJDATA(obj_h, nth, MMT_TAG_LENGTH));
        area = round * length / 1000000.0;
        return(area);
    }
    return(0.00); 
}


/*This function use to calculate surface area of steel plate
**The effect of hole inside surface isn't considered
*/
Calculate_Plate_Surface_Area(obj_h)
{
    nth = 0;
    part_id = PM_GET_OBJDATA(obj_h, nth, MMT_TAG_PARTID);
    shape = DM_PARTID_DATA(part_id,"SH");
    t = DM_PARTID_NTH_DIM(part_id,DM_Q_LENGTH,1);
    single_area = String_To_Float(PM_GET_OBJDATA(obj_h, nth, MMT_TAG_PLATE_AREA));
    round = String_To_Float(PM_GET_OBJDATA(obj_h, nth, MMT_TAG_PLATE_PERIMETER));
    area = (2*single_area + t*round)/1000000.0;
    return(area); 
}

/*let user pick a structural model group in active view
**if success, return group handle, otherwise return 0
*/
Pick_Structural_Group(string prompt)
{
    flag = 1;
    while(flag){
        tmp = 0;
        picked_object_handle = PM_PICK_OBJECT(prompt, tmp, "STRUCTCMP","BEAM","STANDCMP","PIPE","EQUIPMENT");
        if (!ISINT(picked_object_handle)){
            /*get group handle of picked object*/
            group_handle = PM_GET_OBJECT_GROUP(picked_object_handle,7);
            if (ISINT(group_handle)){
				group_handle = PM_GET_OBJECT_GROUP(picked_object_handle,52);
				if (ISINT(group_handle)){
					U_CONFIRM("此零件不属于舾装件，请重新选择");
				}
				else{
					return(group_handle); 
				}
            }
            else{
                return(group_handle);                
            }
        }
        else{
            return(0);
        }
    }
}

/*let user pick a structural part in active view
**if success, return part handle, otherwise return 0
*/
Pick_Structural_Part(string prompt)
{
    flag = 1;
    while(flag){
        nth = 0;
        picked_object_handle = PM_PICK_OBJECT(prompt, nth, "STRUCTCMP","BEAM","STANDCMP","PIPE","EQUIPMENT");
        if (!ISINT(picked_object_handle)){
			obj_type = PM_GET_OBJDATA(picked_object_handle,0,MMT_TAG_OBJTYPE);
			if(obj_type == "3"){			
				part_obj_id = PM_GET_OBJDATA(picked_object_handle,nth,MMT_TAG_OBJID);
				part_handle = PM_FIND_BY_OBJID(part_obj_id);			
				return(part_handle);
			}		
            return(picked_object_handle);
        }
        else{
            return(0);
        }
    }
}

/*let user pick one or more structural part in active view
**and check if it is in the passed model set
**if success, return part set handle, otherwise return 0
*/
Pick_Multi_Structural_Part(string prompt, handle model_set, handle parts)
{
	parts = PM_INIT_SET();
	objects = PM_DEFINE_SET(prompt,1,"STRUCTCMP","BEAM","STANDCMP","PIPE","EQUIPMENT");
	if(ISINT(objects)){
		return(-1);
	}
	
	find = 0;
	part_number = PM_NR_MEMBERS_IN_SET(objects);
	for(i=0;i<part_number;i=i+1;){
		part = PM_GET_MEMBER_IN_SET(objects,i);
		part_index = PM_FIND_OBJECT_FROM_SET(part, model_set);
		if(part_index >=0){
			PM_ADD_OBJECT_TO_SET(part,parts);			
		}
		else{
			find = 1;
			PM_HIGHLIGHT_OBJECT(part,0,0);
		}
	}
	
	if(find){		
		U_CONFIRM("选择的全部或者部分零件不属于当前舾装件");
	}
	
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	if(part_number > 0){
		return(0);
	}
	else{
		return(-1);
	}	
}

/*let user pick structural parts that doesn't belong to any group in active view
**if success, return part set, otherwise return 0
*/
Pick_Free_Structural_Parts(string prompt)
{
    flag = 1;
    parts = PM_INIT_SET();
    while(flag){
        nth = 0;
		check = 0;
        picked_object_handle = PM_PICK_OBJECT(prompt, nth, "STRUCTCMP","BEAM","STANDCMP","PIPE","EQUIPMENT");
        if (!ISINT(picked_object_handle)){
			group_handle = PM_GET_OBJECT_GROUP(picked_object_handle,7);
            if (ISINT(group_handle)){
				group_handle = PM_GET_OBJECT_GROUP(picked_object_handle,52);			
				if (ISINT(group_handle)){
					check = 1;
				}
			}	
            if(!check){
                U_CONFIRM("此零件已经属于其它舾装件！"); 
            }
            else{
				obj_type = PM_GET_OBJDATA(picked_object_handle,0,MMT_TAG_OBJTYPE);
				if(obj_type == "3"){				
					part_obj_id = PM_GET_OBJDATA(picked_object_handle,nth,MMT_TAG_OBJID);
					part_handle = PM_FIND_BY_OBJID(part_obj_id);
					PM_ADD_OBJECT_TO_SET(part_handle,parts);
				}
				else{
					PM_ADD_OBJECT_TO_SET(picked_object_handle,parts);
				}                
            }
        }
        else{
            return(parts);
        }
    }
}


/*let user pick structural parts that doesn't belong to any group in active view
**if success, return part set, otherwise return 0
*/
Pick_Free_Models_For_Structural_Unit(string prompt)
{
    models = PM_DEFINE_SET(prompt);
    if(ISINT(models)){
        U_CONFIRM("未选择任何模型");
        return(0);
    }   
    model_number = PM_NR_MEMBERS_IN_SET(models);
    if(model_number <= 0){
        U_CONFIRM("未选择任何模型");
        return(0);    
    }
    for(i=0;i<model_number;i=i+1;){
        model = PM_GET_MEMBER_IN_SET(models,i);
        check = 0;
        group_handle = PM_GET_OBJECT_GROUP(model,7);
        if(ISINT(group_handle)){
            group_handle = PM_GET_OBJECT_GROUP(model,52);			
            if(ISINT(group_handle)){
                check = 1;
            }
        }	
        if(!check){
            U_CONFIRM("零件已经属于其它舾装件！"); 
            return(0);
        }        
    }
    return(models);
}

/*this function used to rename structural model group
**if success, return new name, otherwise return 0
**passed parameter also used to receive new group handle
*/
Rename_Structural_Unit(group_h)
{
    /*check access right, if no access right to model group or any part in group, return*/
    if(!CheckAccessRight(group_h)){
        U_CONFIRM("权限不足");
        return(0);
    }
    nth = 0;
    old_name = PM_GET_OBJDATA(group_h,nth,MMT_TAG_OBJNAME); 
    /*input new name*/
    name = InputString("输入","舾装件名称","");
    if(ISSTRING(name)){
        /*check if new name is same as old name*/
        if(name != old_name){
            /*check if model with this name exist*/
            check = PM_FIND_BY_NAME(name);
            if(ISINT(check)){
                title = "舾装件《" + old_name + "》改名为《" + name + "》";
                PM_UM_OPEN_CHANGE(title);
                /*create a new model group with input name*/
                new_group_h = Create_Group("7",name);
                if(ISINT(new_group_h)){
                    PM_UM_CLOSE_CHANGE();
                    PM_UM_UNDO_LAST_CHANGE();
                    return(0);
                }
                /*remove all parts from old model group*/
                parts = PM_GET_OBJECTS_IN_GROUP(group_h,1);
                res = PM_RM_SET_FROM_GROUP(parts,group_h);
                if(res == -1){
                    PM_UM_CLOSE_CHANGE();
                    PM_UM_UNDO_LAST_CHANGE();
                    U_CONFIRM("无法将零件从舾装件《" + old_name + "》中删除");
                    return(0);                        
                }
                res = PM_ADD_SET_TO_GROUP(parts,new_group_h);
                if(res == -1){
                    PM_UM_CLOSE_CHANGE();
                    PM_UM_UNDO_LAST_CHANGE();
                    U_CONFIRM("无法将零件加入舾装件《" + name + "》");
                    return(0);                        
                }                                       
                group_h = new_group_h;
                PM_UM_CLOSE_CHANGE();
                return(name);                
            }               
        }
        else{
            U_CONFIRM("输入的名字与原名字相同");
        }            
    }      
}

Delete_Structural_Unit(group_h)
{
    /*check access right, if no access right to model group or any part in group, return*/
    if(!CheckAccessRight(group_h)){
        U_CONFIRM("权限不足");
        return(0);
    }
    nth = 0; 
    name = PM_GET_OBJDATA(group_h,nth,MMT_TAG_OBJNAME);
    title = "删除舾装件《" + name + "》";
    PM_UM_OPEN_CHANGE(title);        
    /*remove all parts from model group*/
    parts = PM_GET_OBJECTS_IN_GROUP(group_h,1);
    res = PM_RM_SET_FROM_GROUP(parts,group_h);
    if(res == -1){
        PM_UM_CLOSE_CHANGE();
        PM_UM_UNDO_LAST_CHANGE();
        U_CONFIRM("无法将零件从舾装件《" + name + "》中移除");
        return(0);                        
    } 
    /*delete model group*/
    res = PM_DELETE_OBJECT(group_h);          
    if(res == -1){
        PM_UM_CLOSE_CHANGE();
        PM_UM_UNDO_LAST_CHANGE();
        U_CONFIRM("无法删除舾装件《" + name + "》");
        return(0);                        
    } 
    PM_UM_CLOSE_CHANGE();
    return(1);  
}


Delete_Structural_Unit_And_Parts(group_h)
{
    /*check access right, if no access right to model group or any part in group, return*/
    if(!CheckAccessRight(group_h)){
        U_CONFIRM("权限不足");
        return(0);
    }
    nth = 0; 
    name = PM_GET_OBJDATA(group_h,nth,MMT_TAG_OBJNAME);
    title = "删除舾装件《" + name + "》及其零件";
    PM_UM_OPEN_CHANGE(title);        
    /*delete all parts from model group*/
    parts = PM_GET_OBJECTS_IN_GROUP(group_h,1);
    res = PM_DELETE_OBJECTS_IN_SET(parts, 0);
    if(res == -1){
        PM_UM_CLOSE_CHANGE();
        PM_UM_UNDO_LAST_CHANGE();
        U_CONFIRM("无法将零件从舾装件《" + name + "》中删除");
        return(0);                        
    } 
    /*delete model group*/
    res = PM_DELETE_OBJECT(group_h);          
    if(res == -1){
        PM_UM_CLOSE_CHANGE();
        PM_UM_UNDO_LAST_CHANGE();
        U_CONFIRM("无法删除舾装件《" + name + "》及其零件");
        return(0);                        
    } 
    PM_UM_CLOSE_CHANGE();
    return(1);  
}

PlateWithSameSize(plate1,plate2)
{
    nth = 0;
    cut_area1 = String_To_Float(PM_GET_OBJDATA(plate1,nth,"are"));
    cut_area2 = String_To_Float(PM_GET_OBJDATA(plate2,nth,"are"));
    if(cut_area1-cut_area2<-0.000001 | cut_area1-cut_area2>0.000001){
        return(0);
    }
    real_area1 = String_To_Float(PM_GET_OBJDATA(plate1,nth,"_PA"));
    real_area2 = String_To_Float(PM_GET_OBJDATA(plate2,nth,"_PA"));
    if(real_area1-real_area2<-0.000001 | real_area1-real_area2>0.000001){
        return(0);
    }
    width1 = String_To_Float(PM_GET_OBJDATA(plate1,nth,"_PW"));
    width2 = String_To_Float(PM_GET_OBJDATA(plate2,nth,"_PW"));
    if(width1-width2<-1 | width1-width2>1){
        return(0);
    }
    height1 = String_To_Float(PM_GET_OBJDATA(plate1,nth,"_PH"));
    height2 = String_To_Float(PM_GET_OBJDATA(plate2,nth,"_PH"));
    if(height1-height2<-1 | height1-height2>1){
        return(0);
    }
    round1 = String_To_Float(PM_GET_OBJDATA(plate1,nth,"_PB"));
    round2 = String_To_Float(PM_GET_OBJDATA(plate2,nth,"_PB"));
    if(height1-height2<-1 | height1-height2>1){
        return(0);
    }
    return(1);
}

BeamWithSameSize(beam1,beam2)
{
    nth = 0;
    length1 = String_To_Float(PM_GET_OBJDATA(beam1,nth,MMT_TAG_AXIS_LEN));
    length2 = String_To_Float(PM_GET_OBJDATA(beam2,nth,MMT_TAG_AXIS_LEN));
    if(length1-length2<-1 | length1-length2>1){
        return(0);
    } 
    return(1);
}

Get_Part_Unit(obj_h)
{
    nth = 0;
    obj_type = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_OBJTYPE);
	part_id = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_PARTID);
	qty = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_BOM_QUANTITY);
	gt = DM_PARTID_DATA(part_id,"GT");
	/*equipment, standard part, structural component, pipe part beside tube*/
    if(obj_type == "1" | obj_type == "2" | (obj_type == "4" & ISSTRING(qty)) | 
	(obj_type == "3" & ISSTRING(qty)) | (obj_type == "10" & ISSTRING(qty))){
        return("PCS");
    }
	/* PLATE */
    else if(obj_type == "4" & ISINT(qty)){
        return("m2");
    }
	/*BEAM*/
    else if(obj_type == "5"){
        return("m");
    }
	/*OTHER*/
    else{
        return("PCS");
    } 
}

Get_Part_Surface_Area(obj_h)
{
    nth = 0;
    obj_type = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_OBJTYPE);
	part_id = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_PARTID);
	qty = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_BOM_QUANTITY);
	gt = "";
	if(ISSTRING(part_id)){
		gt = DM_PARTID_DATA(part_id,"GT");
	}
    /*if object is equopment, standard component, */
    if(obj_type == "1" | obj_type == "2" | obj_type == "3" | obj_type == "10" | (obj_type == "4" & ISSTRING(qty))){
        return(0.00);
    }
    /* if object is structural plate*/
    else if(obj_type == "4" & ISINT(qty)){
        area = Calculate_Plate_Surface_Area(obj_h);
        return(area);      
    }
    /* if object is beam*/
    else if(obj_type == "5"){
        area = Calculate_Beam_Surface_Area(obj_h);
        return(area);
    }
    else{
        return(0.00);        
    }
}