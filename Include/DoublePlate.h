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
		if(curve_name == data){
			flag = 0;		
		}
		else{
			data = TAIL(data,STRLEN(data) - STRLEN(curve_name) - 1);
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
	}
	return(section_name);
}

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
	1, X coordinate of point 1
	2, Y coordinate of point 1
	3, Z coordinate of point 1
	4, X coordinate of point 2
	5, Y coordinate of point 2
	6, Z coordinate of point 2	
	7, dimension text
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
		/* 复板外径标注信息 */
		x01 = 0; y01 = 0; z01 = 0;
		x02 = 0; y02 = 0; z02 = 0;
		x03 = 0; y03 = 0; z03 = 0;
		x04 = 0; y04 = 0; z04 = 0;		
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
							x1 = x0 ;y1 = y0;z1 = 0;
							x2 = x0 - radius1; y2 = y0 ;z2 = 0;		
							TRF_POINT(tmat_h, x1, y1, z1);
							TRF_POINT(tmat_h, x2, y2, z2);
							x01 = x1; y01 = y1; z01 = z1;
							x03 = x2; y03 = y2; z03 = z2;							
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
		DM_2D_ARRAY_SET(dimension_list,row,0,"Line");
		DM_2D_ARRAY_SET(dimension_list,row,1,x01);
		DM_2D_ARRAY_SET(dimension_list,row,2,y01);
		DM_2D_ARRAY_SET(dimension_list,row,3,z01);
		DM_2D_ARRAY_SET(dimension_list,row,4,x02);
		DM_2D_ARRAY_SET(dimension_list,row,5,y02);
		DM_2D_ARRAY_SET(dimension_list,row,6,z02);
		text = STRINGTERM(FTOASCII(SQRT((x01-x02)^2 + (y01-y02)^2 +(z01-z02)^2)),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);	
		/* 复板总长标注*/
		DM_2D_ARRAY_PUSH_BACK_ROW(dimension_list);
		row = DM_2D_ARRAY_SIZE_ROWS(dimension_list) - 1;
		DM_2D_ARRAY_SET(dimension_list,row,0,"Line");
		DM_2D_ARRAY_SET(dimension_list,row,1,x03);
		DM_2D_ARRAY_SET(dimension_list,row,2,y03);
		DM_2D_ARRAY_SET(dimension_list,row,3,z03);
		DM_2D_ARRAY_SET(dimension_list,row,4,x04);
		DM_2D_ARRAY_SET(dimension_list,row,5,y04);
		DM_2D_ARRAY_SET(dimension_list,row,6,z04);
		text = STRINGTERM(FTOASCII(SQRT((x03-x04)^2 + (y03-y04)^2 +(z03-z04)^2)),".");
		DM_2D_ARRAY_SET(dimension_list,row,7,text);					
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
		return(0);	
	}	
	else if(type == "RectR"){
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
		return(0);	
	}
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
