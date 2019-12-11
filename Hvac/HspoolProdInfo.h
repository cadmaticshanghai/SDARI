
/*定义生产信息宏名称*/
#define  Group_Type                    "20"
#define  Planning_Unit                 ".ba"
#define  Outfit_Area                   ".lz"
#define  DocName                       ".ba"
#define  Part_Area                     "are"

/* 定义项目信息对应的TAG定义 */
#define Tag_Project_Description        ".dG"                          /* 项目描述 - 项目名称，系统默认属性 */
#define Tag_Project_Number             "U32"                          /* 项目编号 */


set_column_title(handle wobj, column_titles, column_number)
{
	for (i = 0; i < column_number; i = i + 1;)
	{
		title = A_GET(column_titles,i);
		if (ISSTRING(title))
		{
			W_SET_WINDOW_ARGS(wobj, W_LISTW_COLUMN, i, W_LISTW_HEADER, title);
			W_SET_WINDOW_ARGS(wobj, W_LISTW_COLUMN, i, W_LISTW_COLUMNWIDTH,-1);
			W_SET_WINDOW_ARGS(wobj, W_LISTW_COLUMN, i, W_LISTW_ITEMTYPE, W_LISTW_STRING_COLUMN); 
		}
		else 
		{
			return(0);
		}
	}
	return(0);
}

get_random_string()
{
   time = GET_TIME();
   date_string = CNV_DATE(time,2);
   time_sring = TRANS(CNV_TIME(time,1),":","-");
   random_string = date_string + "-" + time_sring; 
   return random_string;
}

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
	/* spool_name = area_name + "-" + line_name + "-" + spool_number; */
	return(spool_name);
}

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

Get_Part_Surface_Area(obj_h)
{
    nth = 0;
    obj_type = PM_GET_OBJDATA(obj_h,nth,MMT_TAG_OBJTYPE);
    /*if object is standard component*/
    if(obj_type == "2"){
        return(0.00);
    }
    /* if object is structural component*/
    else if(obj_type == "4"){
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