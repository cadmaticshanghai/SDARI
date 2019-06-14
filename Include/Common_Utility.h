Highlight_Object(handle object_handle, int flag)
{
   obj_type = PM_GET_OBJDATA(object_handle,0,MMT_TAG_OBJTYPE);
   if(obj_type == MMT_OBT_GROUP){
	   group_members = PM_GET_OBJECTS_IN_GROUP(object_handle);
	   member_numbers = PM_NR_MEMBERS_IN_SET(group_members);
	   for (i = 0; i < member_numbers; i = i + 1;){
		   part_handle = PM_GET_MEMBER_IN_SET(group_members, i);
		   PM_HIGHLIGHT_OBJECT(part_handle, 0, flag);
		}
	}
	else{
	   PM_HIGHLIGHT_OBJECT(object_handle, 0, flag);
	}
}

/*
**设置指定模型对象属性的值
*/
Set_Attribute_Value(handle object_handle, string tag, value)
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
	      float_value = String_To_Float(value);
	   }
	   st = PM_ASSIGN_ATTRIBUTE(attr_h, object_handle, 0, float_value);
	   if(st<0){
	      PM_DEASSIGN_ATTRIBUTE(attr_h, object_handle, 0);
	      st = PM_ASSIGN_ATTRIBUTE(attr_h, object_handle, 0, float_value);
		   if (st < 0){
		      int_value = 0;
      	   if(ISFLOAT(value)){
      	      int_value = String_To_Int(FTOASCII(value));
      	   }
      	   else if(ISINT(value)){
      	      int_value = value;
      	   }
      	   else{
      	      int_value = String_To_Int(value);
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

/*
**获取指定模型对象属性的值
*/
Get_Attribute_Value(handle ModelObject, Tag_Name)
{
	Att_Value = PM_GET_OBJDATA(ModelObject, 0, Tag_Name);
	if(ISSTRING(Att_Value)){
		return(Att_Value);
	}
	else{
		return(-1);
	}
}

/*
**将指定的模型自动放大到整个视口
*/
Auto_Scale(model_handle)
{
	next = 0;
	while (TRUE){
		source = "";
		status = 0;
		view_name = PM_GET_NAME_OF_NEXT_VIEW(source, next, status);
		if ( ISINT(view_name)){
			return(0);
		}
		else{
			if (status == 1){
				flag = 0;
				view_handle = PM_GET_HANDLE_OF_OPENED_VIEW(view_name,flag);
				if ( !ISINT(view_handle)){
					type = PM_GET_OBJDATA( model_handle, 0, MMT_TAG_OBJTYPE);
					if (type == MMT_OBT_GROUP){
						umin = 0;
						umax = 0;
						vmin = 0;
						vmax = 0;
						wmin = 0;
						wmax = 0;
						result = Get_Bbox_Of_Group_In_View(model_handle, view_handle, umin, umax, vmin, vmax, wmin, wmax);
						if ( result >= 0 ){
							PM_SET_VIEW_PARAMETERS(view_handle,"WINDOW",umin,umax, vmin,vmax);
						}
					}
					else{
						umin = 0;
						umax = 0;
						vmin = 0;
						vmax = 0;
						wmin = 0;
						wmax = 0;
						result = PM_GET_BBOX_OF_OBJECT_IN_VIEW(model_handle, view_handle, umin, umax, vmin, vmax, wmin, wmax);
						if ( result >= 0){
							PM_SET_VIEW_PARAMETERS(view_handle,"WINDOW",umin,umax, vmin,vmax);
						}
					}
				}
			}
		}
	}
	return(0);
}

/*
**获取模型组的空间范围
*/
Get_Bbox_Of_Group_In_View(model_handle, view_handle, umin, umax, vmin, vmax, wmin, wmax)
{
	/* 获取模型组中的零件 */
	parts = PM_GET_OBJECTS_IN_GROUP(model_handle);
	if ( ISINT(parts)){
		return(-1);
	}
	else{
		part_number = PM_NR_MEMBERS_IN_SET(parts);
		if (part_number > 0){
			for (i = 0; i < part_number; i = i + 1){
				umin1 = 0;
				umax1 = 0;
				vmin1 = 0;
				vmax1 = 0;
				wmin1 = 0;
				wmax1 = 0;
				part = PM_GET_MEMBER_IN_SET( parts, i);
				flag = PM_GET_BBOX_OF_OBJECT_IN_VIEW(part, view_handle, umin1, umax1, vmin1, vmax1, wmin1, wmax1);
				if (i == 0){
					umin = umin1;
					umax = umax1;
					vmin = vmin1;
					vmax = vmax1;
					wmin = wmin1;
					wmax = wmax1;
				}
				if ( flag >= 0 ){
					if (umin1 < umin){
						umin = umin1;
					}
					if (umax1 > umax){
						umax = umax1;
					}
					if (vmin1 < vmin){
						vmin = vmin1;
					}
					if (vmax1 > vmax){
						vmax = vmax1;
					}
					if (wmin1 < wmin){
						wmin = wmin1;
					}
					if (wmax1 > wmax){
						wmax = wmax1;
					}
				}
			}
		}
		return(0);
	}
}


/*
**获取模型集合的空间范围
*/
Get_Bbox_Of_Model_Set_In_View(set, view_handle, umin, umax, vmin, vmax, wmin, wmax)
{
    model_number = PM_NR_MEMBERS_IN_SET(set);
    if (model_number > 0){
        for (i = 0; i < model_number; i = i + 1){
            umin1 = 0;
            umax1 = 0;
            vmin1 = 0;
            vmax1 = 0;
            wmin1 = 0;
            wmax1 = 0;
            model = PM_GET_MEMBER_IN_SET( set, i);
            flag = PM_GET_BBOX_OF_OBJECT_IN_VIEW(model, view_handle, umin1, umax1, vmin1, vmax1, wmin1, wmax1);
            if (i == 0){
                umin = umin1;
                umax = umax1;
                vmin = vmin1;
                vmax = vmax1;
                wmin = wmin1;
                wmax = wmax1;
            }
            if ( flag >= 0 ){
                if (umin1 < umin){
                    umin = umin1;
                }
                if (umax1 > umax){
                    umax = umax1;
                }
                if (vmin1 < vmin){
                    vmin = vmin1;
                }
                if (vmax1 > vmax){
                    vmax = vmax1;
                }
                if (wmin1 < wmin){
                    wmin = wmin1;
                }
                if (wmax1 > wmax){
                    wmax = wmax1;
                }
            }
        }
    }
    return(0);
}

/*
                      Convert          absolute model coordinates to relative coordinates
                      i.e. distance, direction and name of the nearest reference plane.

                      Input arguments:
                      float               x                    absolute X-coordinate
                      float               y                    absolute Y-coordinate
                      float               z                    absolute Z-coordinate

                      Output arguments:
                      string              x_ref              distance, direction and name of the nearest ref. plane in X-direction
                      string              y_ref              distance, direction and name of the nearest ref. plane in Y-direction
                      string              z_ref               distance, direction and name of the nearest ref. plane in Z-direction

                      return values:
                      int                  0                    success
                      int                  -1                   failure (e.g. if label "#Pm_RefPlXYZ" is not defined in the environment)
*/

GetReferencePlaneCoords( float x, float y, float z, string x_ref, string y_ref, string z_ref)
{
   index = 0;
   dummy_handle =  PM_NEXT_OBJECT("GROUP", index);
   result = A_ALLOC(2);
   check = PM_EVALUATE_DATA_FOR_LABEL("#Pm_RefPlXYZ", dummy_handle, 0, 1, x, y, z, result);
   if(check == 0){
      data = A_GET(result, 0);
      x_ref = STRINGTERM(data, "\\");
      yz_coord = SUBSTRING(SEARCH(data, "\\"), 1);
      y_ref = STRINGTERM(yz_coord, "\\");
      z_ref = SUBSTRING(SEARCH(yz_coord, "\\"), 1);
      A_FREE(result);
      return(0);
      }
   else{
      U_CONFIRM("请先定义名为:#Pm_RefPlXYZ的LABEL");
      A_FREE(result);
      return(-1);
   }
   return(-1);
}

GetReferenceCoords( float x, float y, float z, string x_ref, float x_offset, string y_ref, float y_offset, string z_ref, float z_offset)
{
   x_ref_plane = "";
   y_ref_plane = "";
   z_ref_plane = "";

   res = GetReferencePlaneCoords( x, y, z, x_ref_plane, y_ref_plane, z_ref_plane);

   if (res == 0){

      x_off = STRINGTERM( x_ref_plane, " ");
      temp = SUBSTRING(SEARCH(x_ref_plane, " "), 1);
      x_sign = STRINGTERM(temp, " ");
      x_ref = SUBSTRING(SEARCH(temp, " "), 1);

      y_off = STRINGTERM( y_ref_plane, " ");
      temp = SUBSTRING(SEARCH(y_ref_plane, " "), 1);
      y_sign = STRINGTERM(temp, " ");
      y_ref = SUBSTRING(SEARCH(temp, " "), 1);

      z_off = STRINGTERM( z_ref_plane, " ");
      temp = SUBSTRING(SEARCH(z_ref_plane, " "), 1);
      z_sign = STRINGTERM(temp, " ");
      z_ref = SUBSTRING(SEARCH(temp, " "), 1);

      /* 修正偏移量为0时，偏移量符号缺失的问题 */

      if (x_off == "0"){
         x_sign = "+";
      }

      if (y_off == "0"){
         y_sign = "+";
      }

      if (z_off == "0"){
         z_sign = "+";
      }

      /*修正偏移量符号*/
      if(x_sign == "After"){
         x_sign = "-";
      }
      else if(x_sign == "Fore"){
         x_sign = "+";
      }

      if(y_sign == "SB"){
         y_sign = "-";
      }
      else if(y_sign == "PS"){
         y_sign = "+";
      }

      if(z_sign == "Beneath"){
         z_sign = "-";
      }
      else if(z_sign == "Above"){
         z_sign = "+";
      }

      /* 检查偏移量符号，参考面的偏移量必须用“+”或者“—”表示 */

      if (x_sign != "-" & x_sign != "+"){
         return(-1);
      }
      if (y_sign != "-" & y_sign != "+"){
         return(-1);
      }
      if (z_sign != "-" & z_sign != "+"){
         return(-1);
      }


      /* 获取正确的偏移量（结果是字符串） */

      if (x_sign == "-"){
         x_str = x_sign + x_off;
      }
      else if(x_sign == "+"){
         x_str = x_off;
      }

      if (y_sign == "-"){
         y_str = y_sign + y_off;
      }
      else if(y_sign == "+"){
         y_str = y_off;
      }

      if (z_sign == "-"){
         z_str = z_sign + z_off;
      }
      else if(z_sign == "+"){
         z_str = z_off;
      }

      /* 将相对坐标系的偏移量由字符串转换成数值 */
      x_offset = String_To_Int(x_str);
      y_offset = String_To_Int(y_str);
      z_offset = String_To_Int(z_str);

      return(0);
   }
   return(-1);
}

String_To_Int(string int_string)
{
   int value;
   value = 0;
   value = SCAN_QUANT(3,int_string);
   return(value);
}

String_To_Float(string float_string)
{
   value = 0.0;
   value = SCAN_QUANT(3,float_string);
   return(value);
}

Rotate_Set(x0, y0, z0, axis, set, angle)
{

   nr = PM_NR_MEMBERS_IN_SET(set);

   if (nr == 0) {
	   U_MESSAGE("Empty set, operation canceled");
	   return(-1);
   }

   if (PM_WRITE_MDL_OF_SET("rotate.mdl", set) != 0){
      return(-1);
   }


   cur_dx = -x0; cur_dy = -y0; cur_dz = -z0;

	local_to_global = PM_CREATE_TMAT(1, cur_dx,cur_dy,cur_dz, 1,0,0, 0,1,0);
	global_to_local = PM_INVERSE_TMAT(local_to_global);

   rotm = PM_CREATE_TMAT(1, 0, 0, 0, 1, 0, 0, 0, 1, 0);

	if (axis == "x" | axis == "X")
      rotm = PM_CREATE_TMAT(1, 0, 0, 0, 1, 0, 0, 0, COS(angle), SIN(angle));
	else if (axis == "y" | axis == "Y")
      rotm = PM_CREATE_TMAT(1, 0, 0, 0, COS(angle), 0, -SIN(angle), 0, 1, 0);
	else if (axis == "z" | axis == "Z")
      rotm = PM_CREATE_TMAT(1, 0, 0, 0, COS(angle), SIN(angle), 0, -SIN(angle), COS(angle), 0);
	else {
	   return(-1);
   }

   tm1  = PM_JOIN_TMATS(local_to_global, rotm);
	tmat = PM_JOIN_TMATS(tm1,global_to_local);

	/* open undo task */
   PM_UM_OPEN_CHANGE("Rotate set");

	/*	Notify PM that we are going to relocate objects using a
		sequence of deletes and MDL-loads. */
	if (!PM_OK_TO_RELOCATE_OBJECTS_IN_SET(set)) {
		F_DELETE_FILE("rotate.mdl");
		PM_UM_CLOSE_CHANGE();
		return(-1);
	}

	PM_DELETE_OBJECTS_IN_SET(set, 0);

	PM_READ_MDL_INTO_SET("rotate.mdl", 1, "", tmat, set);

	PM_RELOCATION_ACCEPTED();

	PM_UM_CLOSE_CHANGE();

	F_DELETE_FILE("rotate.mdl");

	return(0);
}

Move_Set(set, axis, distance)
{
	mdl_mode = 1;

	nr = PM_NR_MEMBERS_IN_SET(set);

	if (nr == 0){
		U_MESSAGE("没有指定任何要移动的模型");
		return(-1);
	}

	if (PM_WRITE_MDL_OF_SET("move.mdl", set) != 0) return(-1);

	cur_dx = 0; cur_dy = 0; cur_dz = 0;

	PM_UM_OPEN_CHANGE("Move set");

	if (axis == "x" | axis == "X"){
      cur_dx = distance;
   }
	else if (axis == "y" | axis == "Y"){
      cur_dy = distance;
   }
	else if (axis == "z" | axis == "Z"){
      cur_dz = distance;
   }
	else {
      return(-1);
	}

	cur_tm = PM_CREATE_TMAT(1, cur_dx,cur_dy,cur_dz, 1,0,0, 0,1,0);

	PM_DELETE_OBJECTS_IN_SET(set, 0);

	PM_READ_MDL_INTO_SET("move.mdl", mdl_mode, "", cur_tm, set);

	PM_UM_CLOSE_CHANGE();

	/*	Inform PM that we are done and accept relocation. */
	PM_RELOCATION_ACCEPTED();

	F_DELETE_FILE("move.mdl");	/* not needed anymore. */

	return(0);
}

Move_Model_Set(set, dx, dy, dz)
{
	mdl_mode = 1;

	nr = PM_NR_MEMBERS_IN_SET(set);

	if (nr == 0){
		U_MESSAGE("没有指定任何要移动的模型");
		return(-1);
	}

	if (PM_WRITE_MDL_OF_SET("move.mdl", set) != 0) return(-1);

	cur_dx = dx; cur_dy = dy; cur_dz = dz;

	PM_UM_OPEN_CHANGE("Move Models");

	cur_tm = PM_CREATE_TMAT(1, cur_dx,cur_dy,cur_dz, 1,0,0, 0,1,0);

	PM_DELETE_OBJECTS_IN_SET(set, 0);

	PM_READ_MDL_INTO_SET("move.mdl", mdl_mode, "", cur_tm, set);

	PM_UM_CLOSE_CHANGE();

	/*	Inform PM that we are done and accept relocation. */
	PM_RELOCATION_ACCEPTED();

	F_DELETE_FILE("move.mdl");	/* not needed anymore. */

	return(0);
}

/*
创建一个指定类型及名称的模型组
如果成功，则返回模型组的句柄
如果失败，在消息窗口提示相关信息并返回0
*/

Create_Group(string group_type, string group_name)
{
	rec = DM_INIT_TAGREC();
	DM_SET_TAGVAL(rec, MMT_TAG_GROUPTYPE, group_type);
	DM_SET_TAGVAL(rec, MMT_TAG_OBJNAME, group_name);

	inv_tag = "";
	obj = PM_CREATE_OBJECT("GROUP", rec, inv_tag);

	DM_FREE_TAGREC(rec);

	if (!ISINT(obj)){
		return(obj);

	}

	U_MESSAGE("Failed to create group.");
	U_MESSAGE("Error : " + DM_GET_ERRMSG(obj));
	if(inv_tag != ""){
		U_MESSAGE("Bad value in : " + inv_tag);
	}

	return(0);
}

String_To_Rec(tag, value, rec)
{
	DM_SET_TAGVAL(rec, tag, value);
}

Point_To_Rec(tag, x, y, z, rec)
{
	s = "";
	S_PRINTF(s, "%f %f %f", x, y, z);
	DM_SET_TAGVAL(rec, tag, s);
}

Dir_To_Rec(tag, dx, dy, dz, rec)
{
	s = "";
	S_PRINTF(s, "%f %f %f", dx, dy, dz);
	DM_SET_TAGVAL(rec, tag, s);
}

Int_To_Rec(tag, value, rec)
{
	DM_SET_TAGVAL(rec, tag, ITOASCII(value));
}

Open_Mdl_File(string pathname)
{
	if(!F_EXIST(pathname))
		F_CREATE_FILE(pathname);
	f = F_OPEN_FILE(pathname, "w");
	if(f < 0){
		msg = "无法生成文件 <" + pathname + ">.";
		U_MESSAGEBOX(msg, U_MB_ERROR, U_MB_OK, U_OK);
		return(-1);
	}

	return(f);
}

/* 由零件的ID号获取零件的描述及尺寸描述 */
Pid_To_Description(string part_id)
{
	description = DM_PARTID_DATA(part_id, "DE");
	dimensions  = DM_PARTID_DATA(part_id, "DD");
	return(description + " " + dimensions);
}

Pid_To_Description1(string part_id)
{
	description = DM_PARTID_DATA(part_id, "DD");
	return(description);
}

Pick_Cable_Tray(type)
{
   /*select cable tray*/
   pick_loop = 1;
   while(pick_loop){
      nth_part = 0;
      if(type == MMT_CABLETRP_TYPE_STANDARD){
         prompt = "请选择电缆托架标准件";
      }
      else if(type == MMT_CABLETRP_TYPE_STRAIGHT){
         prompt = "请选择电缆托架直段";
      }
      else if(type == MMT_CABLETRP_TYPE_BEND){
         prompt = "请选择电缆托架水平弯头";
      }
      else if(type == MMT_CABLETRP_TYPE_BEND_UP){
         prompt = "请选择电缆托架向上弯头";
      }
      else if(type == MMT_CABLETRP_TYPE_BEND_DOWN){
         prompt = "请选择电缆托架向下弯头";
      }
      else if(type == MMT_CABLETRP_TYPE_NETWORKSEGMENT){
         prompt = "请选择电缆通道";
      }
      else if(type == MMT_CABLETRP_TYPE_NETWORKNODE){
         prompt = "请选择电缆节点";
      }
      else{
         prompt = "请选择电缆托架零件";
      }
      tray = PM_PICK_OBJECT(prompt,nth_part,"CABLETRAY");
      if(!ISINT(tray)){
         if(type == ""){
            pick_loop = 0;
         }
         else{
			selected_type = PM_GET_OBJDATA(tray,nth_part,MMT_TAG_CABLETRP_TYPE);
			if(selected_type == type){
				pick_loop = 0;
            }
			else{               
					U_CONFIRM("请选择正确的类型");
				}
			}
		}
	}
	return(tray);
}

Search_First_Model(x1,y1,z1,x2,y2,z2,type)
{
	models = PM_OBJECTS_IN_BOX(1,x1,y1,z1,x2,y2,z2);
	model_number = PM_NR_MEMBERS_IN_SET(models);
	if(model_number<=0){
		return(-1);
	}
	for(i=0;i<model_number;i=i+1;){
		model = PM_GET_MEMBER_IN_SET(models,i);
		model_type = PM_GET_OBJDATA(model,0,MMT_TAG_OBJTYPE);
		if(model_type == type){
			return(model);
		}
	}
	return(0);
}

/* 用于查找指定空间范围内或者部分在指定的空间范围内的模型 */
Search_Models_Intersect_Box(x1,y1,z1,x2,y2,z2,obj_type)
{
	models = PM_OBJECTS_IN_BOX(1,x1,y1,z1,x2,y2,z2);
	model_number = PM_NR_MEMBERS_IN_SET(models);
	objects = PM_INIT_SET();
	if(model_number<=0){
		return(-1);
	}
	object_number = 0;
	for(i=0;i<model_number;i=i+1;){
		model = PM_GET_MEMBER_IN_SET(models,i);
		model_type = PM_GET_OBJDATA(model,0,MMT_TAG_OBJTYPE);
		if(model_type == obj_type){
			PM_ADD_OBJECT_TO_SET(model,objects);
			object_number = object_number+1;
		}
	}
	if(object_number>0){
		return(objects);	
	}
	else{
		return(0);
	}	
}

Get_Standard_Part_Para(part_handle,lox,loy,loz,udx,udy,udz,vdx,vdy,vdz)
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


Get_Object_Access_Status(object)
{
	server_name = "";
	server_guid = "";
	server = COS_GET_SERVER_INFO(server_name,server_guid);
	owner_id = PM_GET_OBJDATA(object,0,MMT_TAG_OWNER_ID);
	owner = PM_GET_OBJDATA(object,0,MMT_TAG_COS_PROPERTY_OWNED_BY_NAME); 
	owner_guid = PM_GET_OBJDATA(object,0,MMT_TAG_COS_PROPERTY_OWNED_BY_GUID);
	area_id= PM_GET_CURRENT_DESAREA();
	if(ISSTRING(owner)){
		if(owner == server_name){
			if(owner_id == ITOASCII(area_id)){
				return("CheckOutByMe");
			}
			else if(owner_id == "0"){
				return("Local");
			}
			else if(owner_id == MMT_DA_ID_LOCAL_REPLICA){
				return("Local");            
			}
			else{
				return("CheckOutByOther");
			}
		}
		else if(owner_id == MMT_DA_ID_REPLICAS_MASTER | owner_id == MMT_DA_ID_REPLICA){
			return("Replica");
		} 
		else{
			return("CheckOutByReplicaUser");
		}
	}
	else{
		obj_id = PM_GET_OBJDATA(object,0,MMT_TAG_OBJID);
		message = "请使用命令<Model->Manage Area->Rebuild>刷新模型，否则模型"+obj_id+"权限信息不正确";
		U_MESSAGE(message);
		return("CheckOutByMe");
	}
	return(0);  
}

Get_Random_String()
{
	time = GET_TIME();
	date_string = CNV_DATE(time,2);
	time_sring = TRANS(CNV_TIME(time,1),":","-");
	random_string = date_string + "-" + time_sring; 
	return random_string;
}

/* Attribute types */
#define COS_ATTRTYPE_STRING	1
#define COS_ATTRTYPE_INT	2
#define COS_ATTRTYPE_TIME	3
#define COS_ATTRTYPE_ENUM	4
#define COS_ATTRTYPE_OBJREF	5
#define COS_ATTRTYPE_FLOAT	6
#define COS_ATTRTYPE_CODED	7
/*获取枚举型属性的属性值*/
Get_Enumerated_Desc(string tag, int value)
{
	handle info, choices;
	int i_value, id;
	info = 0; 
	choices = 0;
	nr_choices = 0;
	s = 0;
	
	st = COS_SCHEMA_GET_ATTR_INFO(tag, info, choices);
	if((st == 0) & (info != 0) & (choices != 0)) {
		if(DM_GET_TAGVAL(info, COS_ATTR_INFO_TYPE) == COS_ATTRTYPE_CODED) {	
			i_value = value;
			nr_choices = A_SIZE(choices);
			if(nr_choices) {
				for(i = 0; i < nr_choices; i = i + 1; ) {
					choice_rec = A_GET(choices, i);
					id = DM_GET_TAGVAL(choice_rec, COS_ATTR_INFO_CHOICE_ID);
					if(id == i_value)
						s = DM_GET_TAGVAL(choice_rec, COS_ATTR_INFO_CHOICE_NAME);
				}
			}
		}
	}
	
	/*free allocated memory*/
	if(info != 0)
		DM_FREE_TAGREC(info);
	if(choices != 0) {
		nr_choices = A_SIZE(choices);
		for(i = 0; i < nr_choices; i = i + 1; ) 
			DM_FREE_TAGREC(A_GET(choices, i));
		A_FREE(choices);
	}	
	return(s);
}

/*this function is used to check if the passed part_id is existed, if not, then try to get it
**from default data, if it doesn't exist in default data, let user select from library and save
**'to default data
*/
Get_Default_Part(string app_name,string part_name, string part_id, string prompt, string keyword)
{
	part = "";
	st = GET_STRING_DEFAULT(app_name, part_name, part);
	if(st == 0){
		part_id = part;
		return(0);
	}
	else{
		if(Is_Part_Exited(part_id)){
			return(0);
		}
   	U_CONFIRM(prompt);
   	selectmask = DM_INIT_TAGREC();
   	DM_SET_TAGVAL(selectmask, "KW", keyword);
   	loop_flag = 1;
	while(loop_flag){
		part_id = DM_BROWSE_PARTCODE("", 1, selectmask);
			if(ISSTRING(part_id)){
				loop_flag = 0;
				DM_FREE_TAGREC(selectmask);
				SET_STRING_DEFAULT(app_name, part_name, part_id);
			}
		}
	}
	return(0);
}

Get_Default_PartCode(string app_name,string cata_name, string part_code, string prompt, string keyword)
{
	code = "";
	st = GET_STRING_DEFAULT(app_name, cata_name, code);
	if(st == 0){
		part_code = code;
		return(0);
	}
	else{
		if(Find_Part_Code(part_code)){
			return(0);
		} 
   	U_CONFIRM(prompt);
   	selectmask = DM_INIT_TAGREC();
   	DM_SET_TAGVAL(selectmask, "KW", keyword);
   	loop_flag = 1;
   	while(loop_flag){
		part_code = DM_BROWSE_PARTCODE("", 0, selectmask);
			if(ISSTRING(part_code)){
				loop_flag = 0;
				DM_FREE_TAGREC(selectmask);
				SET_STRING_DEFAULT(app_name, cata_name, part_code);
			}
		}    
	}
	return(0);
}

/*check the specified part id exist in library or not*/
Is_Part_Exited(string part_id)
{
	descr = DM_PARTID_DATA(part_id,"DE");
	if(descr == ""){
		return(0);
	}
	else{
		return(1);
	}
	return(0);
}

/*check the part_code exist in library or not*/
Find_Part_Code(part_code)
{
	i = 0;
	s = "dummy";
	while(!ISINT(s)){
		s = DM_GET_NEXT_PARTCODE(i);
		if(!ISINT(s)){
			if(part_code == s){
				return(1);
			}
		}
	}
   return(0);
}

/*return system color index by name of system*/
Get_System_Color(system_name)
{
	nr_systems = 0;
	nr_columns = 0;
	v = PM_GET_SYSTEM_INFO(nr_systems, nr_columns);
	loop_flag = 1;
	i = 0;
	while(loop_flag){
	    v_value = DM_VECTOR_GET(v, (i * nr_columns) + 1);
        if(system_name == v_value){	    
	        loop_flag = 0;
	        color_index = DM_VECTOR_GET(v, (i * nr_columns) + 2);
	        return(color_index);
        }	
        i = i + 1;
        if(i > nr_systems){
            loop_flag = 0;
        }    
	}
    return(-1);  
}

Get_System_Data(system_name,tag)
{
	nr_systems = 0;
	nr_columns = 0;
	v = PM_GET_SYSTEM_INFO(nr_systems, nr_columns);
	loop_flag = 1;
	i = 0;
	while(loop_flag){
	    v_value = DM_VECTOR_GET(v, (i * nr_columns) + 1);
        if(system_name == v_value){
	        rec = DM_VECTOR_GET(v, (i * nr_columns) + 4);
	        value = DM_GET_TAGVAL(rec,tag);
	        return(value);
        }	
        i = i + 1;
        if(i > nr_systems){
            loop_flag = 0;
        }    
	}
    return(-1);     
    
}

/*
	Project point to line.
	
	Input & Output arguments:
	handle	point_set	Handle of point set, where totally 6 points
						3 input points:
						index 0	= coordinates of the 1st point of line
						index 1	= coordinates of the 2nd point of line
						index 2	= coordinates of the point to be projected
						1 output points:
						index 3	= coordinates of the projected CL point
	float	distance	Distance between nearest points
						
	Return values:
	int		0			Success
	int		-1			Failure
*/
Project_Point_To_Line(handle point_set, float distance)
{
	ax1 = 0.0;	ay1 = 0.0;	az1 = 0.0;
	ax2 = 0.0;	ay2 = 0.0;	az2 = 0.0;
	bx = 0.0;	by = 0.0;	bz = 0.0;
	
	PM_GET_PNT(point_set, 0, ax1, ay1, az1);
	PM_GET_PNT(point_set, 1, ax2, ay2, az2);
	PM_GET_PNT(point_set, 2, bx, by, bz);
	
	/* Direction of line A */
	adx = ax2 - ax1;
	ady = ay2 - ay1;
	adz = az2 - az1;
	len1 = VEC_UNITV(adx, ady, adz);
	if(len1 < 0.1)
		return(-1);
	
	/* Transform line A point 1 to origin & along X-direction */
	orig_tm = PM_CREATE_TMAT(3, ax1, ay1, az1, adx, ady, adz, 0, 0, 0);
	inv_tm 	= PM_INVERSE_TMAT(orig_tm);
	
	TRF_POINT(inv_tm, bx, by, bz);
	
	dx = 0.0;
	dy = by;
	dz = bz;
	distance = VEC_UNITV(dx, dy, dz);
	
	/* Transform to original coordinate system */
	x = bx;		y = 0.0;	z = 0.0;
	TRF_POINT(orig_tm, x, y, z);
	PM_SET_PNT(point_set, 3, x, y, z);
	
	return(0);
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
Get_Nearest_Points_Of_Two_3D_Lines(handle point_set, float distance, float angle,float tolerance)
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

/*This function is used to get group name of passed model object
**If passed model object is belonged to piping support, then return piping support name
**If passed model object is belonged to several model groups, return the first group name 
*/
Get_Model_Group_Name(obj_handle)
{
    nth = 0;
    group_name = PM_GET_OBJDATA(obj_handle,nth,MMT_TAG_SUPPORT_PIPING_NAME);
    if(ISSTRING(group_name)){
        return(group_name);
    }
    else{
        group_name = PM_GET_OBJDATA(obj_handle,nth,MMT_TAG_MEMBERSHIPPATH);
        if(ISSTRING(group_name)){
            group_name = SUBSTRING(group_name,1);    
            str = STRINGTERM(group_name,":");  
            if(ISSTRING(str)){
                return(str);
            }
            else{
                return(group_name);
            }  
        }
        else{
            return("Undefined");
        }    
    }
}

InputString(string title,string prompt,string default)
{
	U_INIT_DIALOG(title);
	i_s = U_SET_STR(prompt,default,25);
	if( U_DIALOG() < 0 ){
		return(0);
	}
	s_val = U_GET_STR(i_s);
	return(s_val);      
}

/*this function used to check access right of passed object
**for model group, if user hasn't access right to group or any member, then return FALSE, else return TRUE
**for other object, if user hasn't access right, return FALSE, else return TRUE
*/
CheckAccessRight(obj_h)
{
    nth = 0;
    obj_type = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_OBJTYPE);
    /*if object is model group*/
    if(obj_type == "15"){
        access = Get_Object_Access_Status(obj_h);
        if(access != "CheckOutByMe"){
            return(0);
        }
        parts = PM_GET_OBJECTS_IN_GROUP(obj_h,1);
        part_number = PM_NR_MEMBERS_IN_SET(parts);
        for(i=0;i<part_number;i=i+1;){ 
            part = PM_GET_MEMBER_IN_SET(parts,i);
            access = Get_Object_Access_Status(part);
            if(access != "CheckOutByMe"){
                return(0);
            }
        }
    } 
    else{
        access = Get_Object_Access_Status(obj_h);
        if(access != "CheckOutByMe"){
            return(0);
        }       
    }
    return(1); 
}

CheckIn_Group_With_Members(group_h)
{
    set = PM_INIT_SET();
    if(Get_Object_Access_Status(group_h) == "CheckOutByMe"){
        PM_ADD_OBJECT_TO_SET(group_h,set);
    }
    members = PM_GET_OBJECTS_IN_GROUP(group_h,1);
    member_number = PM_NR_MEMBERS_IN_SET(members);
    for(i=0;i<member_number;i=i+1;){
        member = PM_GET_MEMBER_IN_SET(members,i);
        if(Get_Object_Access_Status(member) == "CheckOutByMe"){
            PM_ADD_OBJECT_TO_SET(member,set);
        }   
    }
	res = PM_SAVE_AND_CHECK_IN(set,1);
	if(res == 0){
	    PM_FREE_SET(set);
	    return(1);
    }
    else{
        PM_FREE_SET(set);
        return(0);
    }
}

CheckOut_Group_With_Members(group_h)
{
    members = PM_GET_OBJECTS_IN_GROUP(group_h,1);
    PM_ADD_OBJECT_TO_SET(group_h,members);
	res = PM_CHECK_OUT(members,1);
	if(res >= 0){
	    return(res);
    }
    else if(res == -1){
        U_CONFIRM("脚本接口错误");
        return(0);
    }
    else if(res == -2){
        U_CONFIRM("模型组包含无效对象");
        return(0);
    }
    else if(res == -3){
        U_CONFIRM("无法申请模型组或其中某一成员的权限");
        return(0);
    }
    else if(res == -4){
        U_CONFIRM("模型组的成员超出当前AREA范围");
        return(0);
    }
}

Request_Group_With_Members(group_h)
{
    members = PM_GET_OBJECTS_IN_GROUP(group_h,1);
    PM_ADD_OBJECT_TO_SET(group_h,members);    
	res = PM_REQUEST_OWNERSHIP(members,0);
    if(res > 0){
        return(res);
	}
    else if(res == -1){
        U_CONFIRM("脚本接口错误");
        return(res);
    }
    else if(res == -2){
        U_CONFIRM("模型组包含无效对象");
        return(res);
    }
    else if(res == -3){
        U_CONFIRM("无法申请模型组或其中某一成员的权限");
        return(res);
    }
}

Relinquish_Group_With_Members(group_h)
{
    members = PM_GET_OBJECTS_IN_GROUP(group_h,1);
    PM_ADD_OBJECT_TO_SET(group_h,members);    
	res = PM_RELINQUISH_OWNERSHIP(members,0);
    if(res > 0){
        return(res);
	}
    else if(res == -1){
        U_CONFIRM("脚本接口错误");
        return(res);
    }
    else if(res == -2){
        U_CONFIRM("模型组包含无效对象");
        return(res);
    }
    else if(res == -3){
        U_CONFIRM("无法Relinquish模型组或其中某一成员");
        return(res);
    }
}



/*this function used to receive user input
  Input arguments:
  1.    string      title           Titel of dialog;
  2.    array       init            a 2D array use to store initiate information of dialog and receive user Input;
  3.    int         input_number    number of input fields;
  array struture:
  0     type            use to store input data type, 'string','int','float','quant';
  1     prompt          use to store input data name 
  2     default value   use to store input data default value, also use to receive user input;
  3     minimum value   use to store input data minimum value, for 'string', it is maximun length; 
  4     maximun value   use to store input data maximun value;
  5     id              use to store quantity id;
  Output arguments:
  1.    array       init        see input arguments
  return value
  0     input successly
  -1    user cancel input
**
*/

InputDialog(string title, handle init, int input_number)
{
    U_INIT_DIALOG(title);
    vars = A_ALLOC(input_number);
    for(i=0;i<input_number;i=i+1;){
        type = get_2darray(init,i,0);
        if(type == "string"){
            prompt = get_2darray(init, i, 1);
            default = get_2darray(init, i, 2);
            max_length = get_2darray(init, i, 3);
            A_PUT(vars,i,U_SET_STR(prompt,default,max_length));
        }
        else if(type == "float"){
            prompt = get_2darray(init, i, 1);
            default = get_2darray(init, i, 2);
            min = get_2darray(init, i, 3);
            max = get_2darray(init, i, 4);
            A_PUT(vars,i,U_SET_FLOAT(prompt,default,min,max));                        
        }
        else if(type == "int"){
            prompt = get_2darray(init, i, 1);
            default = get_2darray(init, i, 2);
            min = get_2darray(init, i, 3);
            max = get_2darray(init, i, 4);
            A_PUT(vars,i,U_SET_INT(prompt,default,min,max));                        
        }
        else if(type == "quant"){
            prompt = get_2darray(init, i, 1);
            default = get_2darray(init, i, 2);
            min = get_2darray(init, i, 3);
            max = get_2darray(init, i, 4);
            quant_id = get_2darray(init, i, 5);
            A_PUT(vars,i,U_SET_QUANT(prompt,default,min,max,quant_id));        
        }       
    }
    if( U_DIALOG() < 0 ){
		U_MESSAGE("取消输入");
		return(-1);
	}
	for(i=0;i<input_number;i=i+1;){
	    type = get_2darray(init,i,0);
        if(type == "string"){
            value = U_GET_STR(A_GET(vars,i));
            put_2darray(init,i,2,value);
        }
        else if(type == "float"){
            value = U_GET_FLOAT(A_GET(vars,i));
            put_2darray(init,i,2,value);                        
        }
        else if(type == "int"){
            value = U_GET_INT(A_GET(vars,i));
            put_2darray(init,i,2,value);                       
        }
        else if(type == "quant"){
            value = U_GET_QUANT(A_GET(vars,i));
            put_2darray(init,i,2,value);       
        }
	} 
	return(0);  
}

Get_Project_Attribute(STRING tag)
{	
    /* 获取当前项目名称 */
	proj_name = SYS_GETENV("PMS_PROJNAME");
	/* 设置查询条件 */
	test_string = DM_COSA_NAME + "=" + proj_name;
	/* 查询COS服务器中名称与设定条件相同的项目对象 */
	cos_oids = COS_QUERY(DM_COST_PROJECT, test_string);
	if(ISINT(cos_oids)) {
		return(0);
	}
	nr_projs = A_SIZE(cos_oids);
	if(nr_projs != 1){
		return(0);
	}
	project_oid = A_GET(cos_oids, 0);
	A_FREE(cos_oids);
	
	/* 读取项目对象的属性 */
	COS_READ_OBJECT(project_oid);
	att = COS_GET_OBJECT_ATTRIBUTES(project_oid);
	COS_FREE_OBJECT(project_oid);
	r = DM_GET_TAGVAL(att, tag);
	if(ISSTRING(r) & r != ""){
	    return(r);
	}
	DM_FREE_TAGREC(att);	
	return(0);
}
