Get_DoublePlate_Source(obj_h)
{
	nth = 0;
	source = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_CMDLEMBGDL);
	return(source);
}

/*	points
	0, name of point
	1, X coordinate
	2, Y coordinate
	3, Z coordinate
*/
Get_DoublePlate_Points(source,points)
{
	flag = 1;
	str = source;
	while(flag){
		check = SEARCH(str," = POINT(");
		if(check == ""){
			flag = 0;
		}		
		else{
			str_head = STRINGTERM(str," = POINT(");
			str_tail = TAIL(check,STRLEN(check)-9);
			name = get_element_name(str_head,"Pnt");
			x_s = STRINGTERM(str_tail,",");
			x = String_To_Float(x_s);
			
			if(x == 0){
				x_s = remove_unuseful_char(x_s);
				x = String_To_Float(x_s);
			}
			str_tail = SUBSTRING(str_tail,STRLEN(STRINGTERM(str_tail,","))+1);
			y_s = STRINGTERM(str_tail,",");
			y = String_To_Float(y_s);
			if(y == 0){
				y_s = remove_unuseful_char(y_s);
				y = String_To_Float(y_s);
			}			
			str_tail = SUBSTRING(str_tail,STRLEN(STRINGTERM(str_tail,","))+1);			
			z_s = STRINGTERM(str_tail,");");
			z = String_To_Float(z_s);
			if(z == 0){
				z_s = remove_unuseful_char(z_s);
				z = String_To_Float(z_s);
			}			
			DM_2D_ARRAY_PUSH_BACK_ROW(points,name,x,y,z);
			str = str_tail;
		}		
	}
	return(0);
}

get_element_name(string origin_string,string pattern)
{
	str_len = STRLEN(origin_string);
	origin = origin_string;
	pattern_len = STRLEN(pattern);
	for(i = 0;i<str_len;i=i+1;){
		check = TAIL(origin,pattern_len);
		if(check == pattern){
			name_length = i + pattern_len;
			name = TAIL(origin_string,name_length);
			return(name);
		}	
		origin = HEAD(origin,STRLEN(origin)-1);		
	}
	return(origin);
}

/*	directions
	0, name of direction
	1, X component value
	2, Y component value
	3, Z component value
*/
Get_DoublePlate_Directions(source,directions)
{
	flag = 1;
	str = source;
	while(flag){
		check = SEARCH(str," = DIRECTION(");
		if(check == ""){
			flag = 0;
		}		
		else{
			str_head = STRINGTERM(str," = DIRECTION(");
			str_tail = TAIL(check,STRLEN(check)-13);
			name = get_element_name(str_head,"Dir");
			dx_s =STRINGTERM(str_tail,",");
			dx = String_To_Float(dx_s);
			if(dx == 0){
				dx_s = remove_unuseful_char(dx_s);
				dx = String_To_Float(dx_s);
			}			
			str_tail = SUBSTRING(str_tail,STRLEN(STRINGTERM(str_tail,","))+1);
			dy_s = STRINGTERM(str_tail,",");
			dy = String_To_Float(dy_s);
			if(dy == 0){
				dy_s = remove_unuseful_char(dy_s);
				dy = String_To_Float(dy_s);
			}						
			str_tail = SUBSTRING(str_tail,STRLEN(STRINGTERM(str_tail,","))+1);
			dz_s = STRINGTERM(str_tail,");");
			dz = String_To_Float(dz_s);
			if(dz == 0){
				dz_s = remove_unuseful_char(dz_s);
				dz = String_To_Float(dz_s);
			}	
			DM_2D_ARRAY_PUSH_BACK_ROW(directions,name,dx,dy,dz);
			str = str_tail;
		}		
	}
	return(0);
}

/*	curves
	0, type of curve
	1, name of curve
	2, value
	....
*/
Get_DoublePlate_Curves(source,curves)
{
	flag = 1;
	column_size = DM_2D_ARRAY_SIZE_COLUMNS(curves);
	str = source;
	while(flag){
		check = SEARCH(str," = CURVE(");
		if(check == ""){
			flag = 0;
		}		
		else{
			DM_2D_ARRAY_PUSH_BACK_ROW(curves);
			row = DM_2D_ARRAY_SIZE_ROWS(curves) - 1;
			str_head = STRINGTERM(str," = CURVE(");
			name = get_element_name(str_head,"Std");
			DM_2D_ARRAY_SET(curves,row,1,name);			
			str = TAIL(check,STRLEN(check) - STRLEN(" = CURVE(") - 1);
			str_curve = STRINGTERM(str,");");	
			type = STRINGTERM(str_curve,"\",");
			DM_2D_ARRAY_SET(curves,row,0,type);	
			str_curve = TAIL(str_curve,STRLEN(str_curve) - STRLEN(type) - 2);
			find_value = 1;
			value_number = 0;
			while(find_value){
				value_s = STRINGTERM(str_curve,",");
				if( value_s == str_curve ){
					find_value = 0;	
					value = String_To_Float(value_s);
					DM_2D_ARRAY_SET(curves,row,value_number+2,value);
					value_number = value_number + 1;					
				}
				else{
					value = String_To_Float(value_s);
					DM_2D_ARRAY_SET(curves,row,value_number+2,value);
					value_number = value_number + 1;
					str_curve = TAIL(str_curve,STRLEN(str_curve) - STRLEN(value_s) - 1);				
				}
			}
			for(i=value_number+2;i<column_size;i=i+1;){
				DM_2D_ARRAY_SET(curves,row,i,0);			
			}			
		}		
	}
	return(0);
}

/*	section
	0, name of curve
	1, u coordinate
	2, v coordinate
*/
Get_DoublePlate_Section(source,section)
{
	str = source;
	str_head = STRINGTERM(str," = SECTION(");
	section_name = get_element_name(str_head,"Sec");
	str_tail = SEARCH(str," = SECTION(");	
	data = TAIL(str_tail,STRLEN(str_tail) - STRLEN(" = SECTION("));
	data = STRINGTERM(data,");");
	curve_name = STRINGTERM(data,",");
	DM_2D_ARRAY_PUSH_BACK_ROW(section,curve_name,0,0);
	data = TAIL(data,STRLEN(data) - STRLEN(curve_name) - 1);	
	flag = 1;
	while(flag){	
		curve_name = STRINGTERM(data,",");
		curve_name = remove_incorrect_char(curve_name);
		if(curve_name == data){
			flag = 0;		
		}
		else{
			data = TAIL(data,STRLEN(data) - STRLEN(curve_name) - 1);
			data = remove_incorrect_char(data);
			if(data != ""){
				u_s = STRINGTERM(data,",");			
				u = String_To_Float(u_s);
				if(u == 0){
					u_s = remove_unuseful_char(u_s);
					u = String_To_Float(u_s);
				}
				data = TAIL(data,STRLEN(data) - STRLEN(u_s) - 1);
				v_s = STRINGTERM(data,",");			
				v = String_To_Float(v_s);
				if(v == 0){
					v_s = remove_unuseful_char(v_s);
					v = String_To_Float(v_s);
				}
				data = TAIL(data,STRLEN(data) - STRLEN(v_s) - 1);
				DM_2D_ARRAY_PUSH_BACK_ROW(section,curve_name,u,v);
			}
			else{
				flag = 0;
			}
		}				
	}
	return(section_name);
}

/* 删除字符串首尾非数字的字符，字符串开始的“-”字符保留 */
remove_unuseful_char(str)
{
	/*检查字符串首部*/
	check = 0;
	while(!check){
		char = HEAD(str,1);
		if (char == "-") {
			check = 1;
		}		
		else{
			check = REGEX_MATCH(char,"^(?:0|[1-9]?|9)$");
			if(!check){
				str = TAIL(str,STRLEN(str) - 1);
			}
			else{
				check = 1;
			}
		}
	}
	/*检查字符串尾部*/
	check = 0;
	while(!check){
		char = TAIL(str,1);
		check = REGEX_MATCH(char,"^(?:0|[1-9]?|9)$");
		if(!check){
			str = HEAD(str,STRLEN(str) - 1);
		}
		else{
			check = 1;
		}
	}		
	return(str);
}

/* 删除字符串首尾非数字及字母的字符 */
remove_incorrect_char(str)
{
	/*检查字符串首部*/
	check = 0;
	while(!check){
		char = HEAD(str,1);
		if (char == "-") {
			check = 1;
		}
		else{
			check = REGEX_MATCH(char,"^[0-9a-zA-Z]*$");
			if(!check){
				str = TAIL(str,STRLEN(str) - 1);
			}
			else{
				check = 1;
			}
		}
	}
	/*检查字符串尾部*/
	check = 0;
	while(!check){
		char = TAIL(str,1);
		check = REGEX_MATCH(char,"^[0-9a-zA-Z]*$");
		if(!check){
			str = HEAD(str,STRLEN(str) - 1);
		}
		else{
			check = 1;
		}
	}		
	return(str);
}

/*	name of object (point,direction,section ...)
	0	origin
	1	u direction
	2	v direction
	3	section
	4	plate thickness
*/
Get_DoublePlate_Plate(source,plate)
{
	str = source;
	str_head = STRINGTERM(str," = PLATE(");
	plate_name = get_element_name(str_head,"Plt");
	str_tail = SEARCH(str," = PLATE(");	
	data = TAIL(str_tail,STRLEN(str_tail) - STRLEN(" = PLATE("));
	data = STRINGTERM(data,");");
	origin_name = STRINGTERM(data,",");
	DM_2D_ARRAY_PUSH_BACK_ROW(plate);
	DM_2D_ARRAY_SET(plate,1,0,origin_name);
	
	data = TAIL(data,STRLEN(data) - STRLEN(origin_name) - 1);	
	u_name = STRINGTERM(data,",");
	DM_2D_ARRAY_SET(plate,1,1,u_name);
	
	data = TAIL(data,STRLEN(data) - STRLEN(u_name) - 1);	
	v_name = STRINGTERM(data,",");
	DM_2D_ARRAY_SET(plate,1,2,v_name);	
	
	data = TAIL(data,STRLEN(data) - STRLEN(v_name) - 1);	
	section_name = STRINGTERM(data,",");
	DM_2D_ARRAY_SET(plate,1,3,section_name);

	data = TAIL(data,STRLEN(data) - STRLEN(section_name) - 1);	
	thickness = String_To_Float(data);
	DM_2D_ARRAY_SET(plate,1,4,thickness);
	return(plate_name);
}

/*	0.	x_coordinate
	1.	y_coordinate
	2.	z_coordinate
	3.	diameter
*/
Get_Hole_List(penetration,hole_list)
{
	source = Get_DoublePlate_Source(penetration);
	/*	0, name of point
		1, X coordinate
		2, Y coordinate
		3, Z coordinate
	*/
	
	points = DM_2D_ARRAY_CREATE(1,4);
	if(ISINT(points)){
		return(-1);
	}
	DM_2D_ARRAY_SET(points,0,0,"Name");
	DM_2D_ARRAY_SET(points,0,1,"X");
	DM_2D_ARRAY_SET(points,0,2,"Y");
	DM_2D_ARRAY_SET(points,0,3,"Z");
	Get_DoublePlate_Points(source,points);
	point_number = DM_2D_ARRAY_SIZE_ROWS(points);
	
	/*	0, name of direction
		1, X component value
		2, Y component value
		3, Z component value
	*/
	directions = DM_2D_ARRAY_CREATE(1,4);
	if(ISINT(directions)){
		return(-1);
	}
	
	DM_2D_ARRAY_SET(directions,0,0,"Name");
	DM_2D_ARRAY_SET(directions,0,1,"dx");
	DM_2D_ARRAY_SET(directions,0,2,"dy");
	DM_2D_ARRAY_SET(directions,0,3,"dz");	
	Get_DoublePlate_Directions(source,directions);
	direction_number = DM_2D_ARRAY_SIZE_ROWS(directions);	

	/*	0, type of curve
		1, name of curve
		2, value
		....
	*/
	max_column = 20;
	curves = DM_2D_ARRAY_CREATE(1,max_column);
	if(ISINT(curves)){
		return(-1);
	}
	DM_2D_ARRAY_SET(curves,0,0,"Type");
	DM_2D_ARRAY_SET(curves,0,1,"Name");
	for(i=2;i<max_column;i=i+1;){
		DM_2D_ARRAY_SET(curves,0,i,"Value");	
	}	
	Get_DoublePlate_Curves(source,curves);
		
	/*	0, name of curve
		1, u coordinate
		2, v coordinate
	*/
	section = DM_2D_ARRAY_CREATE(1,3);
	if(ISINT(section)){
		return(-1);
	}
	DM_2D_ARRAY_SET(directions,0,0,"Name");
	DM_2D_ARRAY_SET(directions,0,1,"u");
	DM_2D_ARRAY_SET(directions,0,2,"v");	
	section_name = Get_DoublePlate_Section(source,section);			
	/*	name of object (point,direction,section ...)
		0	origin
		1	u direction
		2	v direction
		3	section
		4	plate thickness
	*/
	plate = DM_2D_ARRAY_CREATE(1,5);
	if(ISINT(plate)){
		return(-1);
	}
	DM_2D_ARRAY_SET(plate,0,0,"origin");
	DM_2D_ARRAY_SET(plate,0,1,"u");
	DM_2D_ARRAY_SET(plate,0,2,"v");
	DM_2D_ARRAY_SET(plate,0,3,"section");
	DM_2D_ARRAY_SET(plate,0,4,"thickness");	
	plate_name = Get_DoublePlate_Plate(source,plate);	

    /* get position and orientation penetration*/ 
	lox = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_ORIG_X));
	loy = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_ORIG_Y));
	loz = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_ORIG_Z));   
	axis_rotate = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_AXIS_SLP_ANGLE));
	udx = COS(axis_slope)*COS(axis_rotate);
	udy = COS(axis_slope)*SIN(axis_rotate);
	udz = SIN(axis_slope);
	saxis_rotate = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_SAXIS_ROT_ANGLE));
	saxis_slope = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_SAXIS_SLP_ANGLE));
	vdx = COS(saxis_slope)*COS(saxis_rotate);
	vdy = COS(saxis_slope)*SIN(saxis_rotate);
	vdz = SIN(saxis_slope);	
	tmat_h = PM_CREATE_TMAT(1,lox,loy,loz,udx,udy,udz,vdx,vdy,vdz);
		
	curve_number = DM_2D_ARRAY_SIZE_ROWS(curves);
	for(i=2;i<curve_number;i=i+1;){
		x0 = 0;y0 = 0;z0 = 0;
		curve_name = DM_2D_ARRAY_GET(curves,i,1);
		diameter = DM_2D_ARRAY_GET(curves,i,3);
		obj_number = DM_2D_ARRAY_SIZE_ROWS(section);

		for(j=1;j<obj_number;j=j+1;){
			name = DM_2D_ARRAY_GET(section,j,0);
			if(curve_name == name){
				x0 = DM_2D_ARRAY_GET(section,j,1);
				y0 = DM_2D_ARRAY_GET(section,j,2);			
			}		
		}
		TRF_POINT(tmat_h, x0, y0, z0);
		DM_2D_ARRAY_PUSH_BACK_ROW(hole_list);
		row = DM_2D_ARRAY_SIZE_ROWS(hole_list) - 1;
		DM_2D_ARRAY_SET(hole_list,row,0,x0);
		DM_2D_ARRAY_SET(hole_list,row,1,y0);
		DM_2D_ARRAY_SET(hole_list,row,2,z0);
		DM_2D_ARRAY_SET(hole_list,row,3,diameter);		
	}
	/*release resource*/
	DM_2D_ARRAY_DELETE(points);
	DM_2D_ARRAY_DELETE(directions);
	DM_2D_ARRAY_DELETE(curves);
	DM_2D_ARRAY_DELETE(section);
	DM_2D_ARRAY_DELETE(plate);
	return(0);
}

/*	dimension_list
	0, type of dimension
		LineN - 	直线尺寸标注，N=1表示最内层的标准，N=2表示往外第二层的标注，以此类推
		Radius - 	半径标注
		Diameter -	直径标注
	1, X coordinate of point 1
	2, Y coordinate of point 1
	3, Z coordinate of point 1
	4, X coordinate of point 2
	5, Y coordinate of point 2
	6, Z coordinate of point 2	
	7, dimension text
	8, X coordinate of reference point
	9, Y coordinate of reference point
	10,Z coordinate of reference point		
*/
Get_Dimension_List(penetration,dimension_list)
{
	source = Get_DoublePlate_Source(penetration);
	/*	0, name of point
		1, X coordinate
		2, Y coordinate
		3, Z coordinate
	*/
	points = DM_2D_ARRAY_CREATE(1,8);
	if(ISINT(points)){
		return(-1);
	}
	DM_2D_ARRAY_SET(points,0,0,"Name");
	DM_2D_ARRAY_SET(points,0,1,"X");
	DM_2D_ARRAY_SET(points,0,2,"Y");
	DM_2D_ARRAY_SET(points,0,3,"Z");
	Get_DoublePlate_Points(source,points);
	point_number = DM_2D_ARRAY_SIZE_ROWS(points);
	
	/*	0, name of direction
		1, X component value
		2, Y component value
		3, Z component value
	*/
	directions = DM_2D_ARRAY_CREATE(1,4);
	if(ISINT(directions)){
		return(-1);
	}	
	DM_2D_ARRAY_SET(directions,0,0,"Name");
	DM_2D_ARRAY_SET(directions,0,1,"dx");
	DM_2D_ARRAY_SET(directions,0,2,"dy");
	DM_2D_ARRAY_SET(directions,0,3,"dz");	
	Get_DoublePlate_Directions(source,directions);
	direction_number = DM_2D_ARRAY_SIZE_ROWS(directions);	

	/*	0, type of curve
		1, name of curve
		2, value
		....
	*/
	max_column = 20;
	curves = DM_2D_ARRAY_CREATE(1,max_column);
	if(ISINT(curves)){
		return(-1);
	}
	DM_2D_ARRAY_SET(curves,0,0,"Type");
	DM_2D_ARRAY_SET(curves,0,1,"Name");
	for(i=2;i<max_column;i=i+1;){
		DM_2D_ARRAY_SET(curves,0,i,"Value");	
	}	
	Get_DoublePlate_Curves(source,curves);
	
	/*	0, name of curve
		1, u coordinate
		2, v coordinate
	*/
	section = DM_2D_ARRAY_CREATE(1,3);
	if(ISINT(section)){
		return(-1);
	}
	DM_2D_ARRAY_SET(directions,0,0,"Name");
	DM_2D_ARRAY_SET(directions,0,1,"u");
	DM_2D_ARRAY_SET(directions,0,2,"v");	
	section_name = Get_DoublePlate_Section(source,section);			
	/*	name of object (point,direction,section ...)
		0	origin
		1	u direction
		2	v direction
		3	section
		4	plate thickness
	*/
	plate = DM_2D_ARRAY_CREATE(1,5);
	if(ISINT(plate)){
		return(-1);
	}
	DM_2D_ARRAY_SET(plate,0,0,"origin");
	DM_2D_ARRAY_SET(plate,0,1,"u");
	DM_2D_ARRAY_SET(plate,0,2,"v");
	DM_2D_ARRAY_SET(plate,0,3,"section");
	DM_2D_ARRAY_SET(plate,0,4,"thickness");	
	plate_name = Get_DoublePlate_Plate(source,plate);	

    /* get position and orientation of penetration*/ 
	lox = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_ORIG_X));
	loy = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_ORIG_Y));
	loz = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_ORIG_Z));   
	axis_rotate = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_AXIS_SLP_ANGLE));
	udx = COS(axis_slope)*COS(axis_rotate);
	udy = COS(axis_slope)*SIN(axis_rotate);
	udz = SIN(axis_slope);
	saxis_rotate = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_SAXIS_ROT_ANGLE));
	saxis_slope = String_To_Float(PM_GET_OBJDATA(penetration,0,MMT_TAG_SAXIS_SLP_ANGLE));
	vdx = COS(saxis_slope)*COS(saxis_rotate);
	vdy = COS(saxis_slope)*SIN(saxis_rotate);
	vdz = SIN(saxis_slope);	
	tmat_h = PM_CREATE_TMAT(1,lox,loy,loz,udx,udy,udz,vdx,vdy,vdz);
		
	curve_number = DM_2D_ARRAY_SIZE_ROWS(curves);
	type = DM_2D_ARRAY_GET(curves,1,0);
	/* 复板为单贯通 */
	if(type == "Circle"){
		/* 复板外径标注信息 */
		diameter = DM_2D_ARRAY_GET(curves,1,3);
		x1 = diameter * COS(45) / 2 ;y1 = diameter * SIN(45) / 2;z1 = 0;
		x2 =  - diameter * COS(45) / 2;y2 = - diameter * SIN(45) / 2;z2 = 0;		
		TRF_POINT(tmat_h, x1, y1, z1);
		TRF_POINT(tmat_h, x2, y2, z2);
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Diameter");
		DM_2D_ARRAY_SET(dimension_list,row,1,x1);
		DM_2D_ARRAY_SET(dimension_list,row,2,y1);
		DM_2D_ARRAY_SET(dimension_list,row,3,z1);
		DM_2D_ARRAY_SET(dimension_list,row,4,x2);
		DM_2D_ARRAY_SET(dimension_list,row,5,y2);
		DM_2D_ARRAY_SET(dimension_list,row,6,z2);
		text = "\u00F8" + STRINGTERM(FTOASCII(diameter),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);	
		/* 复板内径标注信息 */
		diameter = DM_2D_ARRAY_GET(curves,2,3);
		x1 = diameter / 2 ;y1 = 0;z1 = 0;
		x2 =  - diameter / 2;y2 = 0;z2 = 0;		
		TRF_POINT(tmat_h, x1, y1, z1);
		TRF_POINT(tmat_h, x2, y2, z2);
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Diameter");
		DM_2D_ARRAY_SET(dimension_list,row,1,x1);
		DM_2D_ARRAY_SET(dimension_list,row,2,y1);
		DM_2D_ARRAY_SET(dimension_list,row,3,z1);
		DM_2D_ARRAY_SET(dimension_list,row,4,x2);
		DM_2D_ARRAY_SET(dimension_list,row,5,y2);
		DM_2D_ARRAY_SET(dimension_list,row,6,z2);
		text = "\u00F8" + STRINGTERM(FTOASCII(diameter),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);		
		return(0);	
	}
	/* 复板为自行车链形的双贯通 */
	else if(type == "BicycleChain"){	
		/* 复板内径标注信息 */
		curve_number = DM_2D_ARRAY_SIZE_ROWS(curves);
		for(i = 1;i<curve_number;i=i+1;){
			curve_type = DM_2D_ARRAY_GET(curves,i,0);
			if(curve_type == "Circle"){
				diameter = DM_2D_ARRAY_GET(curves,i,3);
				curve_name = DM_2D_ARRAY_GET(curves,i,1);
				hole_number = DM_2D_ARRAY_SIZE_ROWS(section);
				x0 = 0;y0 = 0;
				for(j=1;j<hole_number;j=j+1;){
					name = DM_2D_ARRAY_GET(section,j,0);
					if(name == curve_name){
						x0 = DM_2D_ARRAY_GET(section,j,1);
						y0 = DM_2D_ARRAY_GET(section,j,2);
					}
				}
				x1 = x0 + diameter * COS(45) / 2 ;y1 = y0 + diameter * SIN(45) / 2;z1 = 0;
				x2 = x0 - diameter * COS(45) / 2; y2 = y0 - diameter * SIN(45) / 2;z2 = 0;		
				TRF_POINT(tmat_h, x1, y1, z1);
				TRF_POINT(tmat_h, x2, y2, z2);
				DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
				row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
				DM_2D_ARRAY_SET(dimension_list,row,0,"Diameter");
				DM_2D_ARRAY_SET(dimension_list,row,1,x1);
				DM_2D_ARRAY_SET(dimension_list,row,2,y1);
				DM_2D_ARRAY_SET(dimension_list,row,3,z1);
				DM_2D_ARRAY_SET(dimension_list,row,4,x2);
				DM_2D_ARRAY_SET(dimension_list,row,5,y2);
				DM_2D_ARRAY_SET(dimension_list,row,6,z2);
				text = "\u00F8" + STRINGTERM(FTOASCII(diameter),".");
				DM_2D_ARRAY_SET(dimension_list,row,7,text);								
			}		
		}
		/* 抽取复板外径 */
		radius1 = 0;
		radius2 = 0;
		for(i = 1;i<curve_number;i=i+1;){
			curve_type = DM_2D_ARRAY_GET(curves,i,0);
			if(curve_type == "BicycleChain"){
				radius1 = DM_2D_ARRAY_GET(curves,i,4);
				radius2 = DM_2D_ARRAY_GET(curves,i,5);
			}
		}
		/* 确定尺寸标注参考点位置 */
		ref_offset = 0;
		if(radius1 > radius2){
			ref_offset = radius1;		
		}
		else{
			ref_offset = radius2;	
		}
		/* 复板外径标注信息 */
		x01 = 0; y01 = 0; z01 = 0;
		x02 = 0; y02 = 0; z02 = 0;
		x03 = 0; y03 = 0; z03 = 0;
		x04 = 0; y04 = 0; z04 = 0;
		x05 = 0; y05 = 0; z05 = 0;	
		x06 = 0; y06 = 0; z06 = 0;		
		for(i = 1;i<curve_number;i=i+1;){
			curve_type = DM_2D_ARRAY_GET(curves,i,0);
			if(curve_type == "Circle"){
				curve_name = DM_2D_ARRAY_GET(curves,i,1);
				hole_number = DM_2D_ARRAY_SIZE_ROWS(section);
				x0 = 0;y0 = 0;
				for(j=1;j<hole_number;j=j+1;){
					name = DM_2D_ARRAY_GET(section,j,0);
					if(name == curve_name){
						x0 = DM_2D_ARRAY_GET(section,j,1);
						y0 = DM_2D_ARRAY_GET(section,j,2);
						dx = x0;dy=y0;dz=0;
						length = VEC_UNITV(dx,dy,dz);
						if(length < 1){
							x1 = x0; y1 = y0; z1 = 0;
							x2 = x0 - radius1; y2 = y0 ;z2 = 0;	
							x3 = x2; y3 = y2 + radius1 ;z3 = 0;
							x4 = x0; y4 = y0 + radius1 ;z4 = 0;
							TRF_POINT(tmat_h, x1, y1, z1);
							TRF_POINT(tmat_h, x2, y2, z2);
							TRF_POINT(tmat_h, x3, y3, z3);
							TRF_POINT(tmat_h, x4, y4, z4);
							x01 = x1; y01 = y1; z01 = z1;
							x03 = x2; y03 = y2; z03 = z2;
							x05 = x3; y05 = y3; z05 = z3;
							x06 = x4; y06 = y4; z06 = z4;							
							DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
							row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
							DM_2D_ARRAY_SET(dimension_list,row,0,"Radius");
							DM_2D_ARRAY_SET(dimension_list,row,1,x1);
							DM_2D_ARRAY_SET(dimension_list,row,2,y1);
							DM_2D_ARRAY_SET(dimension_list,row,3,z1);
							DM_2D_ARRAY_SET(dimension_list,row,4,x2);
							DM_2D_ARRAY_SET(dimension_list,row,5,y2);
							DM_2D_ARRAY_SET(dimension_list,row,6,z2);
							text = "R" + STRINGTERM(FTOASCII(radius1),".");
							DM_2D_ARRAY_SET(dimension_list,row,7,text);								
						}							
						else{
							x1 = x0 ;y1 = y0; z1 = 0;
							x2 = x0 + radius2; y2 = y0 ;z2 = 0;		
							TRF_POINT(tmat_h, x1, y1, z1);
							TRF_POINT(tmat_h, x2, y2, z2);
							x02 = x1; y02 = y1; z02 = z1;
							x04 = x2; y04 = y2; z04 = z2;							
							DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
							row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
							DM_2D_ARRAY_SET(dimension_list,row,0,"Radius");
							DM_2D_ARRAY_SET(dimension_list,row,1,x1);
							DM_2D_ARRAY_SET(dimension_list,row,2,y1);
							DM_2D_ARRAY_SET(dimension_list,row,3,z1);
							DM_2D_ARRAY_SET(dimension_list,row,4,x2);
							DM_2D_ARRAY_SET(dimension_list,row,5,y2);
							DM_2D_ARRAY_SET(dimension_list,row,6,z2);
							text = "R" + STRINGTERM(FTOASCII(radius2),".");
							DM_2D_ARRAY_SET(dimension_list,row,7,text);												
						}
					}
				}											
			}		
		}
		/* 复板开孔中心距标注*/
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Line1");
		DM_2D_ARRAY_SET(dimension_list,row,1,x01);
		DM_2D_ARRAY_SET(dimension_list,row,2,y01);
		DM_2D_ARRAY_SET(dimension_list,row,3,z01);
		DM_2D_ARRAY_SET(dimension_list,row,4,x02);
		DM_2D_ARRAY_SET(dimension_list,row,5,y02);
		DM_2D_ARRAY_SET(dimension_list,row,6,z02);
		text = STRINGTERM(FTOASCII(SQRT((x01-x02)^2 + (y01-y02)^2 +(z01-z02)^2)),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);	
		DM_2D_ARRAY_SET(dimension_list,row,8,x06);
		DM_2D_ARRAY_SET(dimension_list,row,9,y06);
		DM_2D_ARRAY_SET(dimension_list,row,10,z06);		
		/* 复板总长标注*/
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Line2");
		DM_2D_ARRAY_SET(dimension_list,row,1,x03);
		DM_2D_ARRAY_SET(dimension_list,row,2,y03);
		DM_2D_ARRAY_SET(dimension_list,row,3,z03);
		DM_2D_ARRAY_SET(dimension_list,row,4,x04);
		DM_2D_ARRAY_SET(dimension_list,row,5,y04);
		DM_2D_ARRAY_SET(dimension_list,row,6,z04);
		text = STRINGTERM(FTOASCII(SQRT((x03-x04)^2 + (y03-y04)^2 +(z03-z04)^2)),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);	
		DM_2D_ARRAY_SET(dimension_list,row,8,x05);
		DM_2D_ARRAY_SET(dimension_list,row,9,y05);
		DM_2D_ARRAY_SET(dimension_list,row,10,z05);			
		return(0);		
	}
	/* 复板为 */
	else if(type == "TriangR"){
		curve_number = DM_2D_ARRAY_SIZE_ROWS(curves);
		for(i = 1;i<curve_number;i=i+1;){
			curve_type = DM_2D_ARRAY_GET(curves,i,0);
			if(curve_type == "Circle"){
				diameter = DM_2D_ARRAY_GET(curves,i,3);
				curve_name = DM_2D_ARRAY_GET(curves,i,1);
				hole_number = DM_2D_ARRAY_SIZE_ROWS(section);
				x0 = 0;y0 = 0;
				for(j=1;j<hole_number;j=j+1;){
					name = DM_2D_ARRAY_GET(section,j,0);
					if(name == curve_name){
						x0 = DM_2D_ARRAY_GET(section,j,1);
						y0 = DM_2D_ARRAY_GET(section,j,2);
					}
				}				
				x1 = x0 + diameter * COS(45) / 2 ;y1 = y0 + diameter * SIN(45) / 2;z1 = 0;
				x2 = x0 - diameter * COS(45) / 2; y2 = y0 - diameter * SIN(45) / 2;z2 = 0;		
				TRF_POINT(tmat_h, x1, y1, z1);
				TRF_POINT(tmat_h, x2, y2, z2);
				DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
				row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
				DM_2D_ARRAY_SET(dimension_list,row,0,"Diameter");
				DM_2D_ARRAY_SET(dimension_list,row,1,x1);
				DM_2D_ARRAY_SET(dimension_list,row,2,y1);
				DM_2D_ARRAY_SET(dimension_list,row,3,z1);
				DM_2D_ARRAY_SET(dimension_list,row,4,x2);
				DM_2D_ARRAY_SET(dimension_list,row,5,y2);
				DM_2D_ARRAY_SET(dimension_list,row,6,z2);
				text = "\u00F8" + STRINGTERM(FTOASCII(diameter),".");
				DM_2D_ARRAY_SET(dimension_list,row,7,text);								
			}		
		}
		/* 抽取复板外形尺寸 */
		size1 = 0;
		size2 = 0;
		type = "";
		for(i = 1;i<curve_number;i=i+1;){
			curve_type = DM_2D_ARRAY_GET(curves,i,0);
			if(curve_type == "RectR"){
				size1 = DM_2D_ARRAY_GET(curves,i,3);
				size2 = DM_2D_ARRAY_GET(curves,i,4);
			}
		}
		/* 根据外形尺寸判断复板是卵圆形（复板高度等于圆弧半径的两倍）的还是长圆形的*/
		if( size1 - 2 * size2 < 1 & size1 - 2 * size2 > -1){
			type = "Oval";
		}
		else{
			type = "RectR";
		}
		/* 抽取复板的配置信息 */
		settings = Get_Penetration_Setting(penetration);		
		/* 抽取复板中的管材通径及对应的外径 */	
		xmin = 0; ymin = 0; zmin = 0; xmax = 0; ymax = 0; zmax = 0;
		PM_GET_BBOX_OF_OBJECT(penetration, xmin, xmax, ymin, ymax, zmin, zmax);
		pipes = PM_INIT_SET();
		get_pipes_on_penetration(xmin,ymin,zmin,xmax,ymax,zmax,pipes);
		pipe_number = PM_NR_MEMBERS_IN_SET(pipes);
		/* 遍历复板上的所有管子模型 */
		for(i=0;i<pipe_number;i=i+1;){
			pipe = PM_GET_MEMBER_IN_SET(pipes,i);
			/* 获取管子上的直管零件 */
			part_number = PM_NR_PARTS_IN_PIPE(pipe);
			is_tube = 0;
			nth = 0;
			while(!is_tube){
				nth = nth + 1;
				part_id = PM_GET_OBJDATA(pipe, nth, MMT_TAG_PARTID);
				g_type = DM_PARTID_DATA(part_id,"gty");
				if(g_type == 1){
					is_tube = 1;
					
				}
				if(nth > part_number){
					is_tube = 1;
				}
			}		
		}

		
		/* 根据管材外径及复板配置信息计算复板的轮廓数据 */

		/* 释放相关资源 */
		PM_FREE_SET(pipes);
		DM_2D_ARRAY_DELETE(settings);
		return(0);	
	}	
	else if(type == "RectR"){
		curve_number = DM_2D_ARRAY_SIZE_ROWS(curves);
		/* 记录开孔位置，用于后面的位置标注 */
		holes = DM_2D_ARRAY_CREATE(0,2);
		for(i = 1;i<curve_number;i=i+1;){
			curve_type = DM_2D_ARRAY_GET(curves,i,0);
			if(curve_type == "Circle"){
				diameter = DM_2D_ARRAY_GET(curves,i,3);
				curve_name = DM_2D_ARRAY_GET(curves,i,1);
				hole_number = DM_2D_ARRAY_SIZE_ROWS(section);
				x0 = 0;y0 = 0;
				for(j=1;j<hole_number;j=j+1;){
					name = DM_2D_ARRAY_GET(section,j,0);
					if(name == curve_name){
						x0 = DM_2D_ARRAY_GET(section,j,1);
						y0 = DM_2D_ARRAY_GET(section,j,2);
					}
				}				
				DM_2D_ARRAY_PUSH_BACK_ROW(holes);
				row = DM_2D_ARRAY_SIZE_ROWS(holes) - 1;		
				DM_2D_ARRAY_SET(holes,row,0,x0);
				DM_2D_ARRAY_SET(holes,row,1,y0);				
				x1 = x0 - diameter * COS(45) / 2 ;y1 = y0 + diameter * SIN(45) / 2;z1 = 0;
				x2 = x0 + diameter * COS(45) / 2; y2 = y0 - diameter * SIN(45) / 2;z2 = 0;		
				TRF_POINT(tmat_h, x1, y1, z1);
				TRF_POINT(tmat_h, x2, y2, z2);
				DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
				row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
				DM_2D_ARRAY_SET(dimension_list,row,0,"Diameter");
				DM_2D_ARRAY_SET(dimension_list,row,1,x1);
				DM_2D_ARRAY_SET(dimension_list,row,2,y1);
				DM_2D_ARRAY_SET(dimension_list,row,3,z1);
				DM_2D_ARRAY_SET(dimension_list,row,4,x2);
				DM_2D_ARRAY_SET(dimension_list,row,5,y2);
				DM_2D_ARRAY_SET(dimension_list,row,6,z2);
				text = "\u00F8" + STRINGTERM(FTOASCII(diameter),".");
				DM_2D_ARRAY_SET(dimension_list,row,7,text);								
			}		
		}
		
		/* 抽取复板外形尺寸 */
		p_width = 0;
		p_height = 0;
		p_radius = 0;
		sub_type = "";
		for(i = 1;i<curve_number;i=i+1;){
			curve_type = DM_2D_ARRAY_GET(curves,i,0);
			if(curve_type == "RectR"){
				p_width = DM_2D_ARRAY_GET(curves,i,3);
				p_height = DM_2D_ARRAY_GET(curves,i,4);
				p_radius = DM_2D_ARRAY_GET(curves,i,5);
			}
		}
		
		/* 复板总长标注*/
		x1 = - p_width / 2.0;
		x2 = p_width / 2.0;
		x3 = x1;
		y1 = 0;
		y2 = 0;
		y3 = p_height / 2.0;
		z1 = 0;
		z2 = 0;
		z3 = 0;
		TRF_POINT(tmat_h, x1, y1, z1);
		TRF_POINT(tmat_h, x2, y2, z2);
		TRF_POINT(tmat_h, x3, y3, z3);
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Line2");
		DM_2D_ARRAY_SET(dimension_list,row,1,x1);
		DM_2D_ARRAY_SET(dimension_list,row,2,y1);
		DM_2D_ARRAY_SET(dimension_list,row,3,z1);
		DM_2D_ARRAY_SET(dimension_list,row,4,x2);
		DM_2D_ARRAY_SET(dimension_list,row,5,y2);
		DM_2D_ARRAY_SET(dimension_list,row,6,z2);
		text = STRINGTERM(FTOASCII(SQRT((x1-x2)^2 + (y1-y2)^2 +(z1-z2)^2)),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);	
		DM_2D_ARRAY_SET(dimension_list,row,8,x3);
		DM_2D_ARRAY_SET(dimension_list,row,9,y3);
		DM_2D_ARRAY_SET(dimension_list,row,10,z3);		
		
		/* 复板总高标注*/
		x1 = p_width / 2.0 - p_radius;
		x2 = x1;
		x3 = p_width / 2.0;
		y1 = p_height / 2.0;
		y2 = - p_height / 2.0;
		y3 = y1;
		z1 = 0;
		z2 = 0;
		z3 = 0;
		TRF_POINT(tmat_h, x1, y1, z1);
		TRF_POINT(tmat_h, x2, y2, z2);
		TRF_POINT(tmat_h, x3, y3, z3);
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Line2");
		DM_2D_ARRAY_SET(dimension_list,row,1,x1);
		DM_2D_ARRAY_SET(dimension_list,row,2,y1);
		DM_2D_ARRAY_SET(dimension_list,row,3,z1);
		DM_2D_ARRAY_SET(dimension_list,row,4,x2);
		DM_2D_ARRAY_SET(dimension_list,row,5,y2);
		DM_2D_ARRAY_SET(dimension_list,row,6,z2);
		text = STRINGTERM(FTOASCII(SQRT((x1-x2)^2 + (y1-y2)^2 +(z1-z2)^2)),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);	
		DM_2D_ARRAY_SET(dimension_list,row,8,x3);
		DM_2D_ARRAY_SET(dimension_list,row,9,y3);
		DM_2D_ARRAY_SET(dimension_list,row,10,z3);
		
		/* 标注半径 */
		x1 = - p_width / 2.0 + p_radius;
		y1 = - p_height / 2.0 + p_radius;
		z1 = 0;
		x2 = x1 - p_radius * COS(45);
		y2 = y1 - p_radius * COS(45);
		z2 = 0;		
		TRF_POINT(tmat_h, x1, y1, z1);
		TRF_POINT(tmat_h, x2, y2, z2);							
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Radius");
		DM_2D_ARRAY_SET(dimension_list,row,1,x1);
		DM_2D_ARRAY_SET(dimension_list,row,2,y1);
		DM_2D_ARRAY_SET(dimension_list,row,3,z1);
		DM_2D_ARRAY_SET(dimension_list,row,4,x2);
		DM_2D_ARRAY_SET(dimension_list,row,5,y2);
		DM_2D_ARRAY_SET(dimension_list,row,6,z2);
		text = "R" + STRINGTERM(FTOASCII(p_radius),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);

		/* 标注开孔位置 */
		sort_holes(holes);
		hole_number = DM_2D_ARRAY_SIZE_ROWS(holes);
		x1 = - p_width / 2.0;
		y1 = 0;
		z1 = 0;
		x2 = DM_2D_ARRAY_GET(holes,0,0);
		y2 = y1;
		z2 = 0;
		x3 = x1;
		y3 = y1 + p_height / 2.0;
		z3 = 0;
		TRF_POINT(tmat_h, x1, y1, z1);
		TRF_POINT(tmat_h, x2, y2, z2);	
		TRF_POINT(tmat_h, x3, y3, z3);		
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Line1");
		DM_2D_ARRAY_SET(dimension_list,row,1,x1);
		DM_2D_ARRAY_SET(dimension_list,row,2,y1);
		DM_2D_ARRAY_SET(dimension_list,row,3,z1);
		DM_2D_ARRAY_SET(dimension_list,row,4,x2);
		DM_2D_ARRAY_SET(dimension_list,row,5,y2);
		DM_2D_ARRAY_SET(dimension_list,row,6,z2);
		text = STRINGTERM(FTOASCII(SQRT((x1-x2)^2 + (y1-y2)^2 +(z1-z2)^2)),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);		
		DM_2D_ARRAY_SET(dimension_list,row,8,x3);
		DM_2D_ARRAY_SET(dimension_list,row,9,y3);
		DM_2D_ARRAY_SET(dimension_list,row,10,z3);
		/*标注孔间距*/
		for(i=0;i<hole_number-1;i=i+1;){
			x1 = DM_2D_ARRAY_GET(holes,i,0);
			y1 = 0;
			z1 = 0;
			x2 = DM_2D_ARRAY_GET(holes,i+1,0);
			y2 = 0;
			z2 = 0;
			x3 = x1;
			y3 = p_height / 2.0;
			z3 = 0;
			TRF_POINT(tmat_h, x1, y1, z1);
			TRF_POINT(tmat_h, x2, y2, z2);	
			TRF_POINT(tmat_h, x3, y3, z3);		
			DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
			row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
			DM_2D_ARRAY_SET(dimension_list,row,0,"Line1");
			DM_2D_ARRAY_SET(dimension_list,row,1,x1);
			DM_2D_ARRAY_SET(dimension_list,row,2,y1);
			DM_2D_ARRAY_SET(dimension_list,row,3,z1);
			DM_2D_ARRAY_SET(dimension_list,row,4,x2);
			DM_2D_ARRAY_SET(dimension_list,row,5,y2);
			DM_2D_ARRAY_SET(dimension_list,row,6,z2);
			text = STRINGTERM(FTOASCII(SQRT((x1-x2)^2 + (y1-y2)^2 +(z1-z2)^2)),".");
			DM_2D_ARRAY_SET(dimension_list,row,7,text);		
			DM_2D_ARRAY_SET(dimension_list,row,8,x3);
			DM_2D_ARRAY_SET(dimension_list,row,9,y3);
			DM_2D_ARRAY_SET(dimension_list,row,10,z3);				
		}
		/*标注开孔距复板边缘距离*/
		for(i=0;i<hole_number;i=i+1;){
			x1 = DM_2D_ARRAY_GET(holes,i,0);
			y1 = DM_2D_ARRAY_GET(holes,i,1);
			z1 = 0;
			x2 = x1;
			y2 = - p_height / 2.0;
			z2 = 0;
			x3 = x1;
			y3 = y1;
			z3 = 0;
			TRF_POINT(tmat_h, x1, y1, z1);
			TRF_POINT(tmat_h, x2, y2, z2);	
			TRF_POINT(tmat_h, x3, y3, z3);		
			DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
			row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
			DM_2D_ARRAY_SET(dimension_list,row,0,"Line1");
			DM_2D_ARRAY_SET(dimension_list,row,1,x1);
			DM_2D_ARRAY_SET(dimension_list,row,2,y1);
			DM_2D_ARRAY_SET(dimension_list,row,3,z1);
			DM_2D_ARRAY_SET(dimension_list,row,4,x2);
			DM_2D_ARRAY_SET(dimension_list,row,5,y2);
			DM_2D_ARRAY_SET(dimension_list,row,6,z2);
			text = STRINGTERM(FTOASCII(SQRT((x1-x2)^2 + (y1-y2)^2 +(z1-z2)^2)),".");
			DM_2D_ARRAY_SET(dimension_list,row,7,text);		
			DM_2D_ARRAY_SET(dimension_list,row,8,x3);
			DM_2D_ARRAY_SET(dimension_list,row,9,y3);
			DM_2D_ARRAY_SET(dimension_list,row,10,z3);				
		}				
	}
	/*其它形状复板*/
	else{
		curve_number = DM_2D_ARRAY_SIZE_ROWS(curves);
		for(i = 1;i<curve_number;i=i+1;){
			curve_type = DM_2D_ARRAY_GET(curves,i,0);
			if(curve_type == "Circle"){
				diameter = DM_2D_ARRAY_GET(curves,i,3);
				curve_name = DM_2D_ARRAY_GET(curves,i,1);
				hole_number = DM_2D_ARRAY_SIZE_ROWS(section);
				x0 = 0;y0 = 0;
				for(j=1;j<hole_number;j=j+1;){
					name = DM_2D_ARRAY_GET(section,j,0);
					if(name == curve_name){
						x0 = DM_2D_ARRAY_GET(section,j,1);
						y0 = DM_2D_ARRAY_GET(section,j,2);
					}
				}				
				x1 = x0 + diameter * COS(45) / 2 ;y1 = y0 + diameter * SIN(45) / 2;z1 = 0;
				x2 = x0 - diameter * COS(45) / 2; y2 = y0 - diameter * SIN(45) / 2;z2 = 0;		
				TRF_POINT(tmat_h, x1, y1, z1);
				TRF_POINT(tmat_h, x2, y2, z2);
				DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
				row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
				DM_2D_ARRAY_SET(dimension_list,row,0,"Diameter");
				DM_2D_ARRAY_SET(dimension_list,row,1,x1);
				DM_2D_ARRAY_SET(dimension_list,row,2,y1);
				DM_2D_ARRAY_SET(dimension_list,row,3,z1);
				DM_2D_ARRAY_SET(dimension_list,row,4,x2);
				DM_2D_ARRAY_SET(dimension_list,row,5,y2);
				DM_2D_ARRAY_SET(dimension_list,row,6,z2);
				text = "\u00F8" + STRINGTERM(FTOASCII(diameter),".");
				DM_2D_ARRAY_SET(dimension_list,row,7,text);								
			}		
		}	
		return(-1);	
	}
	/*release resource*/
	DM_2D_ARRAY_DELETE(points);
	DM_2D_ARRAY_DELETE(directions);
	DM_2D_ARRAY_DELETE(curves);
	DM_2D_ARRAY_DELETE(section);
	DM_2D_ARRAY_DELETE(plate);
	return(0);
}

sort_holes(holes)
{
	hole_number = DM_2D_ARRAY_SIZE_ROWS(holes);
	for(i=0;i<hole_number;i=i+1){
		for(j=0;j<hole_number-1;j=j+1;){
			x1 = DM_2D_ARRAY_GET(holes,j,0);
			x2 = DM_2D_ARRAY_GET(holes,j+1,0);
			if(x1>x2){
				y1 = DM_2D_ARRAY_GET(holes,j,1);
				y2 = DM_2D_ARRAY_GET(holes,j+1,1);
				DM_2D_ARRAY_SET(holes,j,0,x2);
				DM_2D_ARRAY_SET(holes,j+1,0,x1);
				DM_2D_ARRAY_SET(holes,j,1,y2);
				DM_2D_ARRAY_SET(holes,j+1,1,y1);			
			}
		}	
	}
	return(0);
}

get_pipes_on_penetration(x1,y1,z1,x2,y2,z2,pipes)
{
	models = PM_OBJECTS_IN_BOX(1,x1,y1,z1,x2,y2,z2);
	model_number = PM_NR_MEMBERS_IN_SET(models);
	if(model_number<=0){
		return(-1);
	}
	for(i=0;i<model_number;i=i+1;){
		model = PM_GET_MEMBER_IN_SET(models,i);
		model_type = PM_GET_OBJDATA(model,0,MMT_TAG_OBJTYPE);
		if(model_type == "3"){
			PM_ADD_OBJECT_TO_SET(model,pipes);
		}
	}
	return(0);
}

/*	用于获取复板对应的系统设置的相关参数 
**	penetration - 复板的obj_handle
**	返回值 - 包含复板对应的系统设置参数的二维数组
*/
Get_Penetration_Setting(penetration)
{
	/* 抽取复板对应的系统设置对象的COS_ID */
	nth = 0;
	cos_id = PM_GET_OBJDATA(penetration,nth,"_PS");
	/* 读取复板系统设置的数据 */
	COS_READ_OBJECT(cos_id);
	data = COS_GET_OBJECT_DATA(cos_id);
	/* 对复板系统设置的数据进行分析 */
	settings =	DM_2D_ARRAY_CREATE(1,7);
	DM_2D_ARRAY_SET(settings,0,0,"DN");		/* 管子通径 */
	DM_2D_ARRAY_SET(settings,0,1,"PPH");	/* 管子外径与复板开孔的间隙 */
	DM_2D_ARRAY_SET(settings,0,2,"SPH");	/* 套管外径与复板开孔的间隙 */
	DM_2D_ARRAY_SET(settings,0,3,"PTH");	/* 管子外径与甲板(舱壁)开孔的间隙 */
	DM_2D_ARRAY_SET(settings,0,4,"STH");	/* 套管外径与甲板(舱壁)开孔的间隙 */
	DM_2D_ARRAY_SET(settings,0,5,"FTH");	/* 法兰外径与甲板(舱壁)开孔的间隙 */
	DM_2D_ARRAY_SET(settings,0,6,"PO");		/* 复板与甲板(舱壁)的搭边长度 */
	while(data != ""){
		setting = STRINGTERM(data,";;");
		if(STRLEN(setting) > 20){
			/* 获取通径 */
			dn_s = STRINGTERM(setting,";.bu");
			dn_s = TAIL(dn_s,STRLEN(dn_s)-STRLEN(".d2 168;.bt"));
			dn_s = remove_unuseful_char(dn_s);
			dn = String_To_Float(dn_s);
			DM_2D_ARRAY_PUSH_BACK_ROW(settings);
			row = DM_2D_ARRAY_SIZE_ROWS(settings) - 1;
			DM_2D_ARRAY_SET(settings,row,0,dn);	
			/* 获取管子外径与复板开孔的间隙 */			
			pph_sph_s = STRINGTERM(setting,";.br");		
			pph_sph_s = SEARCH(pph_sph_s,";.bq");
			pph_sph_s = remove_unuseful_char(pph_sph_s);
			pph_s = STRINGTERM(pph_sph_s," ");
			pph_s = remove_unuseful_char(pph_s);
			pph = String_To_Float(pph_s);
			DM_2D_ARRAY_SET(settings,row,1,pph);			
			/* 获取套管外径与复板开孔的间隙 */
			sph_s = SEARCH(pph_sph_s," ");
			sph_s = remove_unuseful_char(sph_s);
			sph = String_To_Float(sph_s);
			DM_2D_ARRAY_SET(settings,row,2,sph);
			/* 管子外径与甲板(舱壁)开孔的间隙 */
			psf_s = STRINGTERM(setting,";.bs");
			psf_s = SEARCH(psf_s,";.br");
			psf_s = remove_unuseful_char(psf_s);	
			pth_s = STRINGTERM(psf_s," ");
			pth_s = remove_unuseful_char(pth_s);
			pth = String_To_Float(pth_s);
			DM_2D_ARRAY_SET(settings,row,3,sph);
			/* 套管外径与甲板(舱壁)开孔的间隙 */
			sth_s = TAIL(psf_s,STRLEN(psf_s) - STRLEN(pth_s));
			sth_s = remove_unuseful_char(sth_s);
			psf_s = sth_s;
			sth_s = STRINGTERM(sth_s," ");
			sth_s = remove_unuseful_char(sth_s);
			sth = String_To_Float(sth_s);
			DM_2D_ARRAY_SET(settings,row,4,sth);
			/* 法兰外径与甲板(舱壁)开孔的间隙 */
			fth_s = TAIL(psf_s,STRLEN(psf_s) - STRLEN(sth_s));
			fth_s = remove_unuseful_char(fth_s);
			fth = String_To_Float(fth_s);
			DM_2D_ARRAY_SET(settings,row,5,fth);			
			/* 复板与甲板(舱壁)的搭边长度 */
			po_s = SEARCH(setting,";.bs");
			check = TAIL(po_s,3);
			/* 搭边长度为甲板(或者舱壁)厚度乘以一个系数 */
			if(check == "HPT"){
				factor_s = remove_unuseful_char(po_s);
				factor = String_To_Float(factor_s);
				nth = 0;
				part_id = PM_GET_OBJDATA(penetration,nth,MMT_TAG_PARTID);
				thk = 1.0 * DM_PARTID_DATA(part_id,"D31");
				po = factor * thk;
				DM_2D_ARRAY_SET(settings,row,6,po);						
			}
			/* 搭边长度为复板厚度乘以一个系数 */
			else if(check == "PPT"){
				factor_s = remove_unuseful_char(po_s);
				factor = String_To_Float(factor_s);
				nth = 0;
				part_id = PM_GET_OBJDATA(penetration,nth,MMT_TAG_PARTID);
				thk = 1.0 * DM_PARTID_DATA(part_id,"D31");
				po = factor * thk;
				DM_2D_ARRAY_SET(settings,row,6,po);
			}
			else{
				po_s = remove_unuseful_char(po_s);
				po = String_To_Float(po_s);
				DM_2D_ARRAY_SET(settings,row,6,po);			
			}						
		}		
		data = SUBSTRING(data,STRLEN(setting) + 2);
	}
	COS_FREE_OBJECT(cos_id);
	return(settings);
}

/* 抽取字符串尾部的数字 */
get_tail_numeric(str)
{
	str_len = STRLEN(str);
	temp = str;
	for(i=0;i<str_len;i=i+1;){
		char = TAIL(temp,1);
		if(REGEX_MATCH(char,"^(?:0|[1-9]?|9)$")){
			temp = HEAD(temp,STRLEN(temp) - 1);
		}
		else{
			str = TAIL(str,i);
			return(str);
		}
	}
}

