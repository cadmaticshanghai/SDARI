/*定义支架信息属性*/
#define	SupportDes                  "~G3"   /*string - 支架名称 */
#define	SupportType                 "~G1"   /*string - 支架类型 */
#define  PadType                     "~P2"   /*string - 垫板类型 */
#define  Part_Number                 "~P0"   /*string - 零件编号 */

#define Tag_Tray_Label               "~C1"   /*string - 托架标注内容 */
#define Tag_Tray_Group               "~01"   /*string - 托架组名称 */
#define Tag_Tray_Type                "~C2"   /*int - 0 不计入托盘, 1 单层, 2 双层, 3 三层 */

/*  
    This tag may be change in different project, if you find that you can't get correct result,
    please check your hull model to find the correct one
*/
#define NCH_Thickness               "!0H"   /*string - 船体板厚度 */
#define PlanningUnit                "~G2"   /*string - 托盘编号 */

global string Group_Type            = "7";
global int    Group_Type_I          = 7;

global string AngleBar_Part_Code    =  "+N3cY4is9GUIlwVWRrSg4.W";   /* 默认的角钢零件标准 */
global string Plate_Part_Code       =  "+Ayc6zjQdGXoJr1SAdGiEbm";   /* 默认的钢板零件标准 */
global string Pad_Part_Code         =  "+zRTeATilJQwyeKsiW.nwk0";   /* 默认的标准垫板零件标准 */

global string Beam_Part_Pid         =  "+N3cY4is9GUIlwVWRrSg4.W-6"; /* 默认的角钢零件 */
global string Plate_Part_Pid        =  "+Ayc6zjQdGXoJr1SAdGiEbm-8"; /* 默认的钢板零件 */
global string Pad_Part_Pid          =  "+zRTeATilJQwyeKsiW.nwk0-0"; /* 默认的标准垫板零件 */

global float Pad_Gap                   = 10;   /* 默认的垫板与角钢的边距 */
global float Pad_Corner_Radius         = 10;   /* 默认的垫板圆角半径 */
global int   Max_Panel_Node_Number     = 10;   /* 最大的钢板轮廓顶点数量 */
global float Panel_Default_Thickness   = 12;   /* 默认的船体甲板/舱壁厚度 */
global float Straight_Default_End_Dist = 130;  /* 默认的直段托架端部角钢间距 */
global float Branch_Default_End_Dist   = 125;  /* 默认的三通托架端部角钢间距 */

global float Mult_Tray_Layer_Height1   = 120;  /* 默认的第二层托架层高(距第一层) */
global float Mult_Tray_Layer_Height2   = 140;  /* 默认的第三层托架层高(距第二层) */

Get_Tray_Leng_Code(length)
{
    if (length<661){
        return ("A");
    }
    else if (length<961){
        return ("B");
    }
    else if (length<1261){
        return ("C");
    }
    else if (length<1561){
        return ("D");
    }
    else if (length<1861){
        return ("E");
    }
    else if (length<2161){
        return ("F");
    }
    else{
        return ("X");
    }
}

/*判断是单层，双层，三层托架,返回层高*/
/*根据部件库特征判断是否多层，弃之不用*/
Get_Tray_OffHeight(tray_handle)
{
    tray_pid = PM_GET_OBJDATA(tray_handle,0,MMT_TAG_PARTID);
    pre = DM_PARTID_DATA(tray_pid,"DD");
    pre = STRINGTERM(pre, "-");
    flag = TAIL(pre,1);
    if (flag=="D"){
        return (115);
    }
    if (flag=="T"){
        return (265);
    }
    return (0);
}

/*双层间距120，三层宽度600(含)以下的层高120，以上140*/
/*20190930 修改为界面参数传递到Mult_Tray_Layer_Height*/
Get_Tray_Layer_Height(width,layer)
{
    if(layer<=2){
        return (120);
    }
    else{
        if (width<=600){
            return (120);
        }
    }
    return (140);
}

/*删除多层tray及相关space*/
Delete_Support_And_Tray(support_handle)
{
    bar_clearance = 20;
    supp_parts = PM_GET_OBJECTS_IN_GROUP(support_handle);
    part_number = PM_NR_MEMBERS_IN_SET(supp_parts);        
    for(i=0;i<part_number;i=i+1;){
        part = PM_GET_MEMBER_IN_SET(supp_parts,i);
        /* part_id = PM_GET_OBJDATA(part,0, MMT_TAG_PARTID); */
        obj_type = PM_GET_OBJDATA(part,0, MMT_TAG_OBJTYPE);
        if (obj_type==5){
            xmin = 0; xmax = 0; ymin = 0; ymax = 0; zmin = 0; zmax = 0;
            res = PM_GET_BBOX_OF_OBJECT(part, xmin, xmax, ymin, ymax, zmin, zmax); 
            trays = search_mdels_intersect_box(xmin,ymin,zmin,xmax,ymax,zmax,7);
            if(!ISINT(trays)){
                tray_number = PM_NR_MEMBERS_IN_SET(trays); 
                for(j=0;j<tray_number;j=j+1;){
                    tray_handle = PM_GET_MEMBER_IN_SET(trays,j);
                    PM_DELETE_OBJECT(tray_handle);
                }
            }
            
            xmin = xmin - bar_clearance;
            ymin = ymin - bar_clearance;
            zmin = zmin - bar_clearance;
            xmax = xmax + bar_clearance;
            ymax = ymax + bar_clearance;
            zmax = zmax + bar_clearance;

            spaces = search_mdels_intersect_box(xmin,ymin,zmin,xmax,ymax,zmax,12);
            if(!ISINT(spaces)){
                space_number = PM_NR_MEMBERS_IN_SET(spaces); 
                for(j=0;j<space_number;j=j+1;){
                    space_handle = PM_GET_MEMBER_IN_SET(spaces,j);
                    space_type = PM_GET_OBJDATA(space_handle,0, ".lu");
                    /* U_MESSAGE(space_type); */
                    if(space_type==13){
                        PM_DELETE_OBJECT(space_handle);
                    }
                }
            }
        }
        PM_DELETE_OBJECT(part);
    }
    PM_DELETE_OBJECT(support_handle);
}

/*复制多层托架*/
Copy_Tray_Layer(tray_handle, w_x,w_y,w_z, u_x,u_y,u_z, layer)
{
    /* part_id = PM_GET_OBJDATA(tray_handle,0,MMT_TAG_PARTID); */
    /* tray_width = 1.0 * DM_PARTID_NAMED_DIM(part_id,"Width"); */
    /* space_height = 1.0 * DM_PARTID_NAMED_DIM(part_id,"SHeight"); */
    /* distance = Get_Tray_Layer_Height(tray_width,layer); */
    /* U_MESSAGE("distance="+FTOASCII(distance)); */
    part_type = PM_GET_OBJDATA(tray_handle,0, ".qm");
    /* U_MESSAGE("part_type="+part_type); */
    label = Get_Attribute_Value(tray_handle,Tag_Tray_Label);

    /*main tray space*/
    space_handle = Create_Tray_Space(tray_handle,0);
    Set_Attribute_Value(space_handle,Tag_Tray_Label,label);
    
    if (layer==1){
        U_MESSAGE(" ");
        Set_Attribute_Value(tray_handle,Tag_Tray_Type,1);
    }
    else if (layer==2){
        if(part_type==0){
            tray_handle2 = Copy_Move_Tray(w_x,w_y,w_z, u_x,u_y,u_z, Mult_Tray_Layer_Height1,   0, tray_handle);
        }
        else if(part_type==1){
            tray_handle2 = Copy_Move_Tray(w_x,w_y,w_z, u_x,u_y,u_z, Mult_Tray_Layer_Height1,  50, tray_handle);
        }
        space_handle2 = Create_Tray_Space(tray_handle2,0);
        
        Set_Attribute_Value(tray_handle, Tag_Tray_Type,2);
        Set_Attribute_Value(tray_handle2,Tag_Tray_Type,0);
        Set_Attribute_Value(space_handle2,Tag_Tray_Label,label);
    }
    else if (layer==3){
        if(part_type==0){
            tray_handle2 = Copy_Move_Tray(w_x,w_y,w_z, u_x,u_y,u_z, Mult_Tray_Layer_Height1,   0, tray_handle);
            tray_handle3 = Copy_Move_Tray(w_x,w_y,w_z, u_x,u_y,u_z, Mult_Tray_Layer_Height2,   0, tray_handle2);
        }
        else if(part_type==1){
            tray_handle2 = Copy_Move_Tray(w_x,w_y,w_z, u_x,u_y,u_z, Mult_Tray_Layer_Height1,  50, tray_handle);
            tray_handle3 = Copy_Move_Tray(w_x,w_y,w_z, u_x,u_y,u_z, Mult_Tray_Layer_Height2, -50, tray_handle2);
        }
        space_handle2 = Create_Tray_Space(tray_handle2,0);
        space_handle3 = Create_Tray_Space(tray_handle3,0);
        
        Set_Attribute_Value(tray_handle, Tag_Tray_Type,3);
        Set_Attribute_Value(tray_handle2,Tag_Tray_Type,0);
        Set_Attribute_Value(tray_handle3,Tag_Tray_Type,0);
        
        Set_Attribute_Value(space_handle2,Tag_Tray_Label,label);
        Set_Attribute_Value(space_handle3,Tag_Tray_Label,label);
    }
    return (0);
}

/*延托架支撑方向复制*/
Copy_Move_Tray(wx,wy,wz, vx,vy,vz, distance, offset, tray_handle)
{
    tray_set = PM_INIT_SET();
    PM_ADD_OBJECT_TO_SET(tray_handle, tray_set);
    if (PM_WRITE_MDL_OF_SET("copy_move_tray.mdl", tray_set) != 0){
        PM_FREE_SET(tray_set);
        return(-1);
    }

    px=0;py=0;pz=0;
    Point_Move(px,py,pz, px,py,pz, wx,wy,wz, distance);
    Point_Move(px,py,pz, px,py,pz, vx,vy,vz, offset);

    /* 平移变换 */
    trans = TRF_TMAT_CREATE();
    trans = Trans_Translate(trans, px,py,pz);
    /* print_matrix(trans); */
    
	/*Notify PM that we are going to relocate objects using a sequence of deletes and MDL-loads. */
	if (!PM_OK_TO_RELOCATE_OBJECTS_IN_SET(tray_set)) {
		F_DELETE_FILE("copy_move_tray.mdl");
        PM_FREE_SET(tray_set);
		return(-1);
	}

	/* PM_DELETE_OBJECTS_IN_SET(tray_set, 0); */
	PM_READ_MDL_INTO_SET("copy_move_tray.mdl", 2, "", trans, tray_set);
	PM_RELOCATION_ACCEPTED();
	F_DELETE_FILE("copy_move_tray.mdl");
    
    part_number = PM_NR_MEMBERS_IN_SET(tray_set);        
    for(i=0;i<part_number;i=i+1;){
        part_handle = PM_GET_MEMBER_IN_SET(tray_set,i);
        /* part_id = PM_GET_OBJDATA(part_handle,0, MMT_TAG_PARTID); */
        if(part_handle!=tray_handle){
            PM_FREE_SET(tray_set);
            return(part_handle);
        }
    }
    PM_FREE_SET(tray_set);
	return(-1);
}

/*生成托架space空间*/
Create_Tray_Space(tray_handle,w_offset)
{
    space_set = PM_INIT_SET();
    node = 1;
    cmd  = PM_GET_OBJDATA(tray_handle,0, "cmd");
    sys_id = PM_GET_OBJDATA(tray_handle,0, "sid");
    length = 1.0*PM_GET_OBJDATA(tray_handle,0, "len");
    part_type = PM_GET_OBJDATA(tray_handle,0, ".qm");

    cmd = SUBSTRING(cmd,1);

    /*start position of tray*/
    p_x = 1.0*PM_GET_CONPNT_DATA(tray_handle,node,"x");
    p_y = 1.0*PM_GET_CONPNT_DATA(tray_handle,node,"y");
    p_z = 1.0*PM_GET_CONPNT_DATA(tray_handle,node,"z");

    /*normal direction of tray*/
    u_x = 0.0; u_y = 0.0; u_z = 0.0;
    fns = PM_GET_CONPNT_DATA(tray_handle,node,"fns");
    fnr = PM_GET_CONPNT_DATA(tray_handle,node,"fnr");
    Angle_To_Direction(fns,fnr, u_x,u_y,u_z);
    
    /*width direction of tray*/
    v_x = 0.0; v_y = 0.0; v_z = 0.0;
    wxs = PM_GET_CONPNT_DATA(tray_handle,node,"wxs");
    wxr = PM_GET_CONPNT_DATA(tray_handle,node,"wxr");
    Angle_To_Direction(wxs,wxr, v_x,v_y,v_z);
    
    /*support direction of tray*/
    w_x = 0.0; w_y = 0.0; w_z = 0.0;
    Vector3d_CrossProduct(-u_x,-u_y,-u_z, -v_x,-v_y,-v_z,  w_x,w_y,w_z);
    
    /*start position of tray space*/
    s_x = 0.0; s_y = 0.0; s_z = 0.0;
    Point_Move(p_x,p_y,p_z, s_x,s_y,s_z, w_x,w_y,w_z, w_offset);

    if(part_type==1){
        Create_Straight_Tray_Space(space_set, cmd, sys_id, s_x,s_y,s_z, -u_x,-u_y,-u_z, -v_x,-v_y,-v_z, length);
    }
    else if(part_type==0){
        Create_Standard_Tray_Space(space_set, cmd, sys_id, s_x,s_y,s_z, -u_x,-u_y,-u_z, -v_x,-v_y,-v_z);
    }
    
    part_number = PM_NR_MEMBERS_IN_SET(space_set);        
    for(i=0;i<part_number;i=i+1;){
        part_handle = PM_GET_MEMBER_IN_SET(space_set,i);
        if(part_handle!=tray_handle){
            PM_FREE_SET(space_set);
            return(part_handle);
        }

    }
    PM_FREE_SET(space_set);
	return(-1);
}

/*方向夹角转换为方向向量*/
Angle_To_Direction(ang_s,ang_r, dx,dy,dz)
{
    slope_angle = String_To_Float(ang_s);
    rotation_angle = String_To_Float(ang_r);
    
    dx = COS(slope_angle) * COS(rotation_angle);
    dy = COS(slope_angle) * SIN(rotation_angle);
    dz = SIN(slope_angle);
    VEC_UNITV(dx,dy,dz);
    return (0);
}


/*生成部件托架space空间*/
Create_Standard_Tray_Space(set, cmd, sys_id, px,py,pz, ux,uy,uz, vx,vy,vz)
{
    /*  @(#)MDL3
        ver 62;cre ;org ;lib ;owi ;tim ;unm ;typ ;;
        SPACE(13,$M8ch.zJqHNYr3fjahEtll0-8,ST(1,any,11,0,0,""),MT(1),
        P(32945.4,-1533.3,16217.3),D(0,1,0),D(-1,0,0)) */

    temp1  = "SPACE(13,$"+cmd+",ST(1,any,"+sys_id+",0,0,''),MT(1),";
    temp21 = "P("+FTOASCII(px)+","+FTOASCII(py)+","+FTOASCII(pz)+"),";
    temp22 = "D("+FTOASCII(ux)+","+FTOASCII(uy)+","+FTOASCII(uz)+"),";
    temp23 = "D("+FTOASCII(vx)+","+FTOASCII(vy)+","+FTOASCII(vz)+"))";
    temp2  = temp21 + temp22 + temp23;
    
    file_lines = 4;
    records = A_ALLOC(file_lines);
    A_PUT(records,0,"@(#)MDL3");
    A_PUT(records,1,"ver 62;cre ;org ;lib ;owi ;tim ;unm ;typ ;;");
    A_PUT(records,2,temp1);
    A_PUT(records,3,temp2);
    
    /*写mdl文件*/
    file_path = "tray_space.mdl";
    F_DELETE_FILE(file_path);
    F_CREATE_FILE(file_path);
    file = F_OPEN_FILE(file_path, "w");
    for(i=0; i<file_lines; i=i+1;){
        val = A_GET(records,i);
        F_WRITE_STR(file,val);
        F_WRITE_NEWLINE(file);	
        /*U_MESSAGE("new_line_val= "+val);*/
    }
    F_CLOSE_FILE(file);
    A_FREE(records);
    
	/*Notify PM that we are going to relocate objects using a sequence of deletes and MDL-loads. */
	if (!PM_OK_TO_RELOCATE_OBJECTS_IN_SET(set)) {
		F_DELETE_FILE(file_path);
		return(-1);
	}
    trans = TRF_TMAT_CREATE();

	/* PM_DELETE_OBJECTS_IN_SET(set, 0); */
	PM_READ_MDL_INTO_SET(file_path, 2, "", trans, set);
	PM_RELOCATION_ACCEPTED();
	F_DELETE_FILE(file_path);
}

/*生成直托架space空间*/
Create_Straight_Tray_Space(set, cmd, sys_id, px,py,pz, ux,uy,uz, vx,vy,vz, length)
{
    /*  @(#)MDL3
        ver 62;cre ;org ;lib ;owi ;tim ;unm ;typ ;;
        SPACE(13,$M8ch.zJqHNYr3fjahEtll0-8,ST(1,any,11,0,0,""),MT(1),
        P(32945.4,-1533.3,16217.3),D(0,1,0),D(-1,0,0), CSLEN(320)) */

    temp1  = "SPACE(13,$"+cmd+",ST(1,any,"+sys_id+",0,0,''),MT(1),";
    temp21 = "P("+FTOASCII(px)+","+FTOASCII(py)+","+FTOASCII(pz)+"),";
    temp22 = "D("+FTOASCII(ux)+","+FTOASCII(uy)+","+FTOASCII(uz)+"),";
    temp23 = "D("+FTOASCII(vx)+","+FTOASCII(vy)+","+FTOASCII(vz)+"),";
    temp24 = "CSLEN("+FTOASCII(length)+"))";
    temp2  = temp21 + temp22 + temp23 + temp24;
    
    file_lines = 4;
    records = A_ALLOC(file_lines);
    A_PUT(records,0,"@(#)MDL3");
    A_PUT(records,1,"ver 62;cre ;org ;lib ;owi ;tim ;unm ;typ ;;");
    A_PUT(records,2,temp1);
    A_PUT(records,3,temp2);
    
    /*写mdl文件*/
    file_path = "tray_space.mdl";
    F_DELETE_FILE(file_path);
    F_CREATE_FILE(file_path);
    file = F_OPEN_FILE(file_path, "w");
    for(i=0; i<file_lines; i=i+1;){
        val = A_GET(records,i);
        F_WRITE_STR(file,val);
        F_WRITE_NEWLINE(file);	
        /*U_MESSAGE("new_line_val= "+val);*/
    }
    F_CLOSE_FILE(file);
    A_FREE(records);
    
	/*Notify PM that we are going to relocate objects using a sequence of deletes and MDL-loads. */
	if (!PM_OK_TO_RELOCATE_OBJECTS_IN_SET(set)) {
		F_DELETE_FILE(file_path);
		return(-1);
	}
    trans = TRF_TMAT_CREATE();

	/* PM_DELETE_OBJECTS_IN_SET(set, 0); */
	PM_READ_MDL_INTO_SET(file_path, 2, "", trans, set);
	PM_RELOCATION_ACCEPTED();
	F_DELETE_FILE(file_path);
}

/*弯托架角钢旋转角度*/
Get_Bend_Tray_Interval(angle,i)
{
    if (angle==90){
        if (i==0){
            return (15);
        }else{
            return (30);
        }
    }
    else if (angle==45){
        if (i==0){
            return (7.5);
        }else{
            return (15);
        }
    }
    return (10);
}

/*弯托架角钢是否创建*/
Get_Bend_Tray_Leg_is_create(angle,width,index)
{
    ids = A_ALLOC(6);
    if (Eq(angle, 90)){
        if (width<301){
           A_PUT(ids,0,1);
           A_PUT(ids,1,1);
           A_PUT(ids,2,0);
           A_PUT(ids,3,0);
           A_PUT(ids,4,1);
           A_PUT(ids,5,1);
        }else{
           A_PUT(ids,0,1);
           A_PUT(ids,1,1);
           A_PUT(ids,2,0);
           A_PUT(ids,3,1);
           A_PUT(ids,4,1);
           A_PUT(ids,5,1);
        }
    }
    else if (Eq(angle, 45)){
         if (width<301){
           A_PUT(ids,0,0);
           A_PUT(ids,1,1);
           A_PUT(ids,2,1);
           A_PUT(ids,3,0);
           A_PUT(ids,4,0);
           A_PUT(ids,5,1);
        }else{
           A_PUT(ids,0,1);
           A_PUT(ids,1,1);
           A_PUT(ids,2,0);
           A_PUT(ids,3,0);
           A_PUT(ids,4,1);
           A_PUT(ids,5,1);
        }
    }
    else{
        return (0);
    }
    res = A_GET(ids,index);
    return (res);
}

Eq(x, x0)
{
    deta = x-x0;
    if (deta<0){
        deta = -deta;
    }
    if (deta<0.001){
        return (1);
    }
    return (0);
}

/*分割*/
string_split(input_string,separator,vector)
{
    temp_str = input_string;
    DM_VECTOR_CLEAR(vector); 
    while (temp_str!=""){
        left_str = STRINGTERM(temp_str, separator);
        right_str = SEARCH(temp_str, separator);
        if(left_str!=""){
            DM_VECTOR_PUSH_BACK(vector, left_str);
        }
        if(right_str!=""){
            temp_str = TAIL(right_str,STRLEN(right_str)-STRLEN(separator));
        }
        else{
            temp_str = right_str;
        }
    }
}

/*替换*/
string_replace(input_string,patt1,patt2)
{
    temp_str = "";
    vector = DM_VECTOR_CREATE();
    string_split(input_string,patt1,vector);
    for (i=0;i<DM_VECTOR_SIZE(vector);i=i+1){
        temp_str = temp_str + DM_VECTOR_GET(vector,i) + patt2;
    }
    DM_VECTOR_DELETE(vector); 
    return(temp_str);
}

Debug(string title, x)
{
    if(ISSTRING(x)){
        U_MESSAGE(title+" = "+x);
    }
    else if(ISINT(x)){
        U_MESSAGE(title+" = "+ITOASCII(x));
    }
    else if(ISFLOAT(x)){
        U_MESSAGE(title+" = "+FTOASCII(x));
    }
    else{
        U_MESSAGE(title+" = Not Printable");
    }
    return(0);
}

/*点的矩阵变换*/
Point_Transform(float x0, float y0, float z0, float x1, float y1, float z1, handle trans)
{
    matrix11=0.0;
    matrix12=0.0;
    matrix13=0.0;
    matrix14=0.0;
    TRF_TMAT_GET(trans, 0, 0, matrix11);
    TRF_TMAT_GET(trans, 0, 1, matrix12);
    TRF_TMAT_GET(trans, 0, 2, matrix13);
    TRF_TMAT_GET(trans, 0, 3, matrix14);
    matrix21=0.0;
    matrix22=0.0;
    matrix23=0.0;
    matrix24=0.0;
    TRF_TMAT_GET(trans, 1, 0, matrix21);
    TRF_TMAT_GET(trans, 1, 1, matrix22);
    TRF_TMAT_GET(trans, 1, 2, matrix23);
    TRF_TMAT_GET(trans, 1, 3, matrix24);
    matrix31=0.0;
    matrix32=0.0;
    matrix33=0.0;
    matrix34=0.0;
    TRF_TMAT_GET(trans, 2, 0, matrix31);
    TRF_TMAT_GET(trans, 2, 1, matrix32);
    TRF_TMAT_GET(trans, 2, 2, matrix33);
    TRF_TMAT_GET(trans, 2, 3, matrix34);
    matrix41=0.0;
    matrix42=0.0;
    matrix43=0.0;
    matrix44=0.0;
    TRF_TMAT_GET(trans, 3, 0, matrix41);
    TRF_TMAT_GET(trans, 3, 1, matrix42);
    TRF_TMAT_GET(trans, 3, 2, matrix43);
    TRF_TMAT_GET(trans, 3, 3, matrix44);
    
    proj = x0*matrix14 + y0*matrix24 + z0*matrix34 + matrix44;
    if (proj <= 1.0E-15 & proj >= -1.0E-15){
        proj = 1.0;
    }
    x1 = (x0*matrix11 + y0*matrix21 + z0*matrix31 + matrix41)/proj;
    y1 = (x0*matrix12 + y0*matrix22 + z0*matrix32 + matrix42)/proj;
    z1 = (x0*matrix13 + y0*matrix23 + z0*matrix33 + matrix43)/proj;
    return (1);
}

/*点1通过镜像得到点2*/
Point_Mirror(float x1, float y1, float z1, float x2, float y2, float z2, float p_x, float p_y, float p_z, float v_dx, float v_dy, float v_dz)
{
    /*投影点*/
    ox=0.0;oy=0.0;oz=0.0;
    Point_ProjectOnPlane(x1,y1,z1, ox,oy,oz, p_x,p_y,p_z, v_dx,v_dy,v_dz);
    
    dist = Point_DistToPoint(x1,y1,z1, ox,oy,oz);
    Point_Move(x1,y1,z1, x2,y2,z2, v_dx,v_dy,v_dz, -2*dist);
    return (1);
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

/*点1绕点和方向旋转既定角度得到点2*/
Point_Rotate(float x1, float y1, float z1, float x2, float y2, float z2,  float px, float py, float pz, float pu, float pv, float pw, float angle)
{
    /*矩阵旋转变换*/
    trans = PM_CREATE_TMAT(1, 0,0,0, 1,0,0, 0,1,0);
    trans = Trans_Rotate(trans, px,py,pz, pu,pv,pw, angle);
    /*点的3D变换*/
    Point3d_Transform(x1,y1,z1, x2,y2,z2, trans);
    return(1);
}

/*点到点的距离*/
Point_DistToPoint(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return(SQRT((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2)));
}
    
/*点到平面的距离*/
Point_DistToPlane(float x, float y, float z, float px, float py, float pz, float nu, float nv, float nw)
{
    vx = x - px;
    vy = y - py;
    vz = z - pz;
    nx = nu;
    ny = nv;
    nz = nw;
    dist = Vector3d_DotProduct(vx,vy,vz, nx,ny,nz) / Vector3d_Length(nx,ny,nz);
    if (dist>=0){
        return(dist);
    }
    return(-dist);
}

/*点到直线的距离*/
Point_DistToLine(float x, float y, float z, float px1, float py1, float pz1, float px2, float py2, float pz2)
{
    dx1=0.0;dy1=0.0;dz1=0.0;
    Vector3d_FromPoints(px1,py1,pz1, x,y,z, dx1,dy1,dz1);
    dx2=0.0;dy2=0.0;dz2=0.0;
    Vector3d_FromPoints(px1,py1,pz1, px2,py2,pz2, dx2,dy2,dz2);

    /*法向量*/
    wx=0.0;wy=0.0;wz=0.0;
    Vector3d_CrossProduct(dx1,dy1,dz1, dx2,dy2,dz2, wx,wy,wz);
    /*垂线方向*/
    nx=0.0;ny=0.0;nz=0.0;
    Vector3d_Rotate(dx2,dy2,dz2, wx,wy,wz, 90, nx,ny,nz);

    dist = Vector3d_DotProduct(dx1,dy1,dz1, nx,ny,nz) / Vector3d_Length(nx,ny,nz);
    if (dist>=0){
        return(dist);
    }
    return(-dist);
}
  

/*点1在平面的投影得到点2*/
Point_ProjectOnPlane(float x1, float y1, float z1, float x2, float y2, float z2,  float px, float py, float pz, float nu, float nv, float nw)
{
    dist = Point_DistToPlane(x1,y1,z1, px,py,pz, nu,nv,nw);
    Point_Move(x1,y1,z1, x2,y2,z2, nu,nv,nw, -dist);
    return(1);
}

/*点1在直线的投影得到点2*/
Point_ProjectOnLine(float x1, float y1, float z1, float x2, float y2, float z2,  float px1, float py1, float pz1,  float px2, float py2, float pz2)
{
    dist = Point_DistToLine(x1,y1,z1, px1,py1,pz1, px2,py2,pz2);

    dx1=0.0;dy1=0.0;dz1=0.0;
    Vector3d_FromPoints(px1,py1,pz1, x1,y1,z1, dx1,dy1,dz1);
    dx2=0.0;dy2=0.0;dz2=0.0;
    Vector3d_FromPoints(px1,py1,pz1, px2,py2,pz2, dx2,dy2,dz2);

    /*法向量*/
    wx=0.0;wy=0.0;wz=0.0;
    Vector3d_CrossProduct(dx1,dy1,dz1, dx2,dy2,dz2, wx,wy,wz);
    /*垂线方向*/
    nx=0.0;ny=0.0;nz=0.0;
    Vector3d_Rotate(dx2,dy2,dz2, wx,wy,wz, 90, nx,ny,nz);
    
    Point_Move(x1,y1,z1, x2,y2,z2, nx,ny,nz, dist);
    return(1);
}

Abs(float x)
{
    if(x>=0){
        return(x);
    }
    return(-1*x);
}

/*矩阵镜像变换 X*/
Trans_ReflectX(handle trans1)
{
    trans_2 = PM_CREATE_TMAT(1, 0,0,0, 1,0,0, 0,1,0);
    TRF_TMAT_PUT(trans_2, 0, 0, -1.0);
    trans_3 = PM_JOIN_TMATS(trans1, trans_2);
    return(trans_3);
}

/*矩阵镜像变换 Y*/
Trans_ReflectY(handle trans1)
{
    trans_2 = PM_CREATE_TMAT(1, 0,0,0, 1,0,0, 0,1,0);
    TRF_TMAT_PUT(trans_2, 1, 1, -1.0);
    trans_3 = PM_JOIN_TMATS(trans1, trans_2);
    return(trans_3);
}

/*矩阵镜像变换 Z*/
Trans_ReflectZ(handle trans1)
{
    trans_2 = PM_CREATE_TMAT(1, 0,0,0, 1,0,0, 0,1,0);
    TRF_TMAT_PUT(trans_2, 2, 2, -1.0);
    trans_3 = PM_JOIN_TMATS(trans1, trans_2);
    return(trans_3);
}

/*矩阵平移变换*/
Trans_Translate(handle trans1, float v_x, float v_y, float v_z)
{
    trans_2 = PM_CREATE_TMAT(1, 0,0,0, 1,0,0, 0,1,0);
    TRF_TMAT_PUT(trans_2, 3, 0, v_x);
    TRF_TMAT_PUT(trans_2, 3, 1, v_y);
    TRF_TMAT_PUT(trans_2, 3, 2, v_z);
    trans_3 = PM_JOIN_TMATS(trans1, trans_2);
    return(trans_3);
}

/*矩阵旋转变换*/
Trans_Rotate(handle trans1, float p_x, float p_y, float p_z, float v_x, float v_y, float v_z, float angle)
{
    /* print_matrix(trans1); */
    w_vec_x = v_x; w_vec_y = v_y; w_vec_z = v_z;
    VEC_UNITV(w_vec_x,w_vec_y,w_vec_z);
    
    u_vec_x = 0.0; u_vec_y = 0.0; u_vec_z = 0.0; 
    if (Abs(w_vec_x) + Abs(w_vec_y) > 1.0E-15){
        u_vec_x = w_vec_y;
        u_vec_y = -w_vec_x;
    }else{
        u_vec_y = -w_vec_z;
        u_vec_z = w_vec_y;
    }
    
    v_vec_x = 0.0; v_vec_y = 0.0; v_vec_z = 0.0; 
    VEC_CROSS_PRODUCT(w_vec_x,w_vec_y,w_vec_z, u_vec_x,u_vec_y,u_vec_z, v_vec_x,v_vec_y,v_vec_z);
    
    /*注意 PM_CREATE_TMAT 方法有时候不灵，建议用 Trans_SetFromPointAndTwoVectors 替代*/
    trans_2 = PM_CREATE_TMAT(1, p_x,p_y,p_z, u_vec_x,u_vec_y,u_vec_z, v_vec_x,v_vec_y,v_vec_z);
    /* trans_2 = Trans_SetFromPointAndTwoVectors(p_x,p_y,p_z, u_vec_x,u_vec_y,u_vec_z, v_vec_x,v_vec_y,v_vec_z); */

    trans_3 = PM_CREATE_TMAT(1, 0,0,0, 1,0,0, 0,1,0);

    matrix11=COS(angle);
    matrix12=SIN(angle);
    matrix21=-1.0 * SIN(angle);
    matrix22=COS(angle);

    TRF_TMAT_PUT(trans_3, 0, 0, matrix11);
    TRF_TMAT_PUT(trans_3, 0, 1, matrix12);
    TRF_TMAT_PUT(trans_3, 1, 0, matrix21);
    TRF_TMAT_PUT(trans_3, 1, 1, matrix22);

    trans_3 = PM_JOIN_TMATS(trans_3, trans_2);
    trans_2 = PM_INVERSE_TMAT(trans_2);
    trans_2 = PM_JOIN_TMATS(trans_2, trans_3);
    trans   = PM_JOIN_TMATS(trans1, trans_2);
    return (trans);
}

/*2点定义向量*/      
Vector3d_FromPoints(float x1, float y1, float z1, float x2, float y2, float z2, float dx, float dy, float dz)
{
    dx = x2-x1;
    dy = y2-y1;
    dz = z2-z1;
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

/*向量的夹角*/      
Vector3d_AngleToVector(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2)
{
    len1 = Vector3d_Length(dx1, dy1, dz1);
    len2 = Vector3d_Length(dx2, dy2, dz2);
    if (len1 < 1.0E-15){
        return(ASIN(1));
    }
    else if(len2 < 1.0E-15){
        return(ASIN(1));
    }
    else{
        temp = Vector3d_DotProduct(dx1, dy1, dz1, dx2, dy2, dz2);
        if ((temp/(len1*len2)) > 1.0){
            return(ACOS(1));
        }
        else if ((temp/(len1*len2)) < -1.0){
            return(ACOS(-1));
        }
    }
    return(ACOS((temp/(len1*len2))));
} 
            
/*向量的比例放大*/      
Vector3d_BlankProduct(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float sc)
{
    dx2 = dx1 * sc;
    dy2 = dy1 * sc;
    dz2 = dz1 * sc;
} 

/*向量的加法*/      
Vector3d_Sum(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float dx3, float dy3, float dz3)
{
    dx3 = dx1 + dx2;
    dy3 = dy1 + dy2;
    dz3 = dz1 + dz2;
} 

/*向量的减法*/      
Vector3d_Difference(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float dx3, float dy3, float dz3)
{
    dx3 = dx1 - dx2;
    dy3 = dy1 - dy2;
    dz3 = dz1 - dz2;
} 
      
/*向量的模*/      
Vector3d_Length(float dx, float dy, float dz)
{
    return (SQRT(dx*dx + dy*dy + dz*dz));
} 

/*向量的点积*/      
Vector3d_DotProduct(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2)
{
    return (dx1*dx2 + dy1*dy2 + dz1*dz2);
} 

/*向量的叉积*/      
Vector3d_CrossProduct(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float dx3, float dy3, float dz3)
{
    dx3 = dy1*dz2 - dz1*dy2;
    dy3 = dz1*dx2 - dx1*dz2;
    dz3 = dx1*dy2 - dy1*dx2;
} 

/*向量投影于向量*/
Vector3d_ProjectOnVector(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float dx3, float dy3, float dz3)
{
    LL = dx2*dx2 + dy2*dy2 + dz2*dz2;
    if (LL >= 1.0E-15){
        LL = (dx1*dx2 + dy1*dy2 + dz1*dz2) / LL;
        dx3 = dx2 * LL;
        dy3 = dy2 * LL;
        dz3 = dz2 * LL;
    }
    else{
        dx3 = 0.0;
        dy3 = 0.0;
        dz3 = 0.0;
    }
    return (0);
}

/*向量投影于平面*/
Vector3d_ProjectOnPlane(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float v_dx, float v_dy, float v_dz)
{
    nx=v_dx;ny=v_dy;nz=v_dz;
    length = Vector3d_Length(nx, ny, nz);
    if (length >= 1.0E-15){
        sc = Vector3d_DotProduct(dx1,dy1,dz1, nx,ny,nz);
        sc = sc / (length * length);
        dx2 = dx1 - sc*nx;
        dy2 = dy1 - sc*ny;
        dz2 = dz1 - sc*nz;
    }
    else{
        dx2 = 0.0;
        dy2 = 0.0;
        dz2 = 0.0;
    }
}
         
/*向量的镜像*/
Vector3d_Mirror(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float v_dx, float v_dy, float v_dz)
{
    /*是否与v轴平行*/
    angle = Vector3d_AngleToVector(dx1,dy1,dz1, v_dx,v_dy,v_dz);
    if (Eq(angle, 0) |Eq(angle, 180)){
        dx2=dx1;dy2=dy1;dz2=dz1;
        return (1);
    }
    /*投影方向*/
    p_dx=0.0;p_dy=0.0;p_dz=0.0;
    Vector3d_ProjectOnVector(dx1,dy1,dz1, v_dx,v_dy,v_dz, p_dx,p_dy,p_dz);
    
    u_dx=0.0;u_dy=0.0;u_dz=0.0;
    Vector3d_Difference(dx1,dy1,dz1, p_dx,p_dy,p_dz, u_dx,u_dy,u_dz);
    Vector3d_BlankProduct(u_dx,u_dy,u_dz, u_dx,u_dy,u_dz, -1.0);
    Vector3d_Sum(u_dx,u_dy,u_dz, p_dx,p_dy,p_dz, dx2,dy2,dz2);
    return (0);
}

/*向量的旋转*/      
Vector3d_Rotate(float dx1, float dy1, float dz1, float rdx, float rdy, float rdz, float angle, float dx2, float dy2, float dz2)
{
    c1 = COS(angle);
    rvec_length = Vector3d_Length(rdx, rdy, rdz);
    if (rvec_length >= 1.0E-15){
        urvec_x = 0.0; urvec_y = 0.0; urvec_z = 0.0; 
        Vector3d_ToUnitVector(rdx,rdy,rdz, urvec_x,urvec_y,urvec_z);
        
        dotProd = Vector3d_DotProduct(dx1,dy1,dz1, urvec_x,urvec_y,urvec_z);
        c2 = (1.0 - c1) * dotProd;
        c3 = SIN(angle);
        
        dx2 = c1*dx1 + c2*urvec_x + c3*(urvec_y*dz1 - urvec_z*dy1);
        dy2 = c1*dy1 + c2*urvec_y + c3*(urvec_z*dx1 - urvec_x*dz1);
        dz2 = c1*dz1 + c2*urvec_z + c3*(urvec_x*dy1 - urvec_y*dx1);
    }
    else{
        Vector3d_BlankProduct(dx1,dy1,dz1, dx2,dy2,dz2, c1);
    }
}      

/*向量的3D变换*/      
Vector3d_Transform(float dx0, float dy0, float dz0, float dx1, float dy1, float dz1, handle trans)
{
    matrix11=0.0;
    matrix12=0.0;
    matrix13=0.0;
    matrix14=0.0;
    TRF_TMAT_GET(trans, 0, 0, matrix11);
    TRF_TMAT_GET(trans, 0, 1, matrix12);
    TRF_TMAT_GET(trans, 0, 2, matrix13);
    TRF_TMAT_GET(trans, 0, 3, matrix14);
    matrix21=0.0;
    matrix22=0.0;
    matrix23=0.0;
    matrix24=0.0;
    TRF_TMAT_GET(trans, 1, 0, matrix21);
    TRF_TMAT_GET(trans, 1, 1, matrix22);
    TRF_TMAT_GET(trans, 1, 2, matrix23);
    TRF_TMAT_GET(trans, 1, 3, matrix24);
    matrix31=0.0;
    matrix32=0.0;
    matrix33=0.0;
    matrix34=0.0;
    TRF_TMAT_GET(trans, 2, 0, matrix31);
    TRF_TMAT_GET(trans, 2, 1, matrix32);
    TRF_TMAT_GET(trans, 2, 2, matrix33);
    TRF_TMAT_GET(trans, 2, 3, matrix34);
    matrix41=0.0;
    matrix42=0.0;
    matrix43=0.0;
    matrix44=0.0;
    TRF_TMAT_GET(trans, 3, 0, matrix41);
    TRF_TMAT_GET(trans, 3, 1, matrix42);
    TRF_TMAT_GET(trans, 3, 2, matrix43);
    TRF_TMAT_GET(trans, 3, 3, matrix44);

    dx1 = dx0*matrix11 + dy0*matrix21 + dz0*matrix31;
    dy1 = dx0*matrix12 + dy0*matrix22 + dz0*matrix32;
    dz1 = dx0*matrix13 + dy0*matrix23 + dz0*matrix33;
    return (1);
}   
  
/*点的3D变换*/      
Point3d_Transform(float x0, float y0, float z0, float x1, float y1, float z1, handle trans)
{
    matrix11=0.0;
    matrix12=0.0;
    matrix13=0.0;
    matrix14=0.0;
    TRF_TMAT_GET(trans, 0, 0, matrix11);
    TRF_TMAT_GET(trans, 0, 1, matrix12);
    TRF_TMAT_GET(trans, 0, 2, matrix13);
    TRF_TMAT_GET(trans, 0, 3, matrix14);
    matrix21=0.0;
    matrix22=0.0;
    matrix23=0.0;
    matrix24=0.0;
    TRF_TMAT_GET(trans, 1, 0, matrix21);
    TRF_TMAT_GET(trans, 1, 1, matrix22);
    TRF_TMAT_GET(trans, 1, 2, matrix23);
    TRF_TMAT_GET(trans, 1, 3, matrix24);
    matrix31=0.0;
    matrix32=0.0;
    matrix33=0.0;
    matrix34=0.0;
    TRF_TMAT_GET(trans, 2, 0, matrix31);
    TRF_TMAT_GET(trans, 2, 1, matrix32);
    TRF_TMAT_GET(trans, 2, 2, matrix33);
    TRF_TMAT_GET(trans, 2, 3, matrix34);
    matrix41=0.0;
    matrix42=0.0;
    matrix43=0.0;
    matrix44=0.0;
    TRF_TMAT_GET(trans, 3, 0, matrix41);
    TRF_TMAT_GET(trans, 3, 1, matrix42);
    TRF_TMAT_GET(trans, 3, 2, matrix43);
    TRF_TMAT_GET(trans, 3, 3, matrix44);

    proj = x0*matrix14 + y0*matrix24 +z0*matrix34 + matrix44;
    if (proj <= 1.0E-15 & proj >= -1.0E-15){
        proj = 1.0;
    }
    x1 = (x0*matrix11 + y0*matrix21 + z0*matrix31 + matrix41)/proj;
    y1 = (x0*matrix12 + y0*matrix22 + z0*matrix32 + matrix42)/proj;
    z1 = (x0*matrix13 + y0*matrix23 + z0*matrix33 + matrix43)/proj;
    return (1);
}
  
/*转换矩阵的定义*/      
Trans_SetFromPointAndTwoVectors(float x0, float y0, float z0, float u_dx, float u_dy, float u_dz, float v_dx, float v_dy, float v_dz)
{
    /* update U - vector */
    length = Vector3d_Length(u_dx, u_dy, u_dz);
    matrix11=u_dx/length;
    matrix12=u_dy/length;
    matrix13=u_dz/length;
    matrix14=0.0;

    /* update V - vector */
    length = Vector3d_Length(v_dx, v_dy, v_dz);
    matrix21=v_dx/length;
    matrix22=v_dy/length;
    matrix23=v_dz/length;
    matrix24=0.0;

    /* update W - vector by calculation cross product of U and V */
    w_dx=0.0; w_dy=0.0; w_dz=0.0;
    Vector3d_CrossProduct(u_dx,u_dy,u_dz, v_dx,v_dy,v_dz, w_dx,w_dy,w_dz);
    length = Vector3d_Length(w_dx, w_dy, w_dz);
    matrix31=w_dx/length;
    matrix32=w_dy/length;
    matrix33=w_dz/length;
    matrix34=0.0;

    /* update the origo */
    matrix41=x0;
    matrix42=y0;
    matrix43=z0;
    matrix44=1.0;
    
    trans = PM_CREATE_TMAT(1, 0,0,0, 1,0,0, 0,1,0);
    TRF_TMAT_PUT(trans, 0, 0, matrix11);
    TRF_TMAT_PUT(trans, 0, 1, matrix12);
    TRF_TMAT_PUT(trans, 0, 2, matrix13);
    TRF_TMAT_PUT(trans, 0, 3, matrix14);
    
    TRF_TMAT_PUT(trans, 1, 0, matrix21);
    TRF_TMAT_PUT(trans, 1, 1, matrix22);
    TRF_TMAT_PUT(trans, 1, 2, matrix23);
    TRF_TMAT_PUT(trans, 1, 3, matrix24);
    
    TRF_TMAT_PUT(trans, 2, 0, matrix31);
    TRF_TMAT_PUT(trans, 2, 1, matrix32);
    TRF_TMAT_PUT(trans, 2, 2, matrix33);
    TRF_TMAT_PUT(trans, 2, 3, matrix34);
    
    TRF_TMAT_PUT(trans, 3, 0, matrix41);
    TRF_TMAT_PUT(trans, 3, 1, matrix42);
    TRF_TMAT_PUT(trans, 3, 2, matrix43);
    TRF_TMAT_PUT(trans, 3, 3, matrix44);
    return (trans);
}

/*打印转换矩阵*/
print_matrix(mat)
{
    U_MESSAGE("");
    elem_00=0.0;
    elem_01=0.0;
    elem_02=0.0;
    elem_03=0.0;
    TRF_TMAT_GET(mat, 0, 0, elem_00);
    TRF_TMAT_GET(mat, 0, 1, elem_01);
    TRF_TMAT_GET(mat, 0, 2, elem_02);
    TRF_TMAT_GET(mat, 0, 3, elem_03);
    U_MESSAGE(FTOASCII(elem_00)+" "+FTOASCII(elem_01)+" "+FTOASCII(elem_02)+" "+FTOASCII(elem_03));
    elem_10=0.0;
    elem_11=0.0;
    elem_12=0.0;
    elem_13=0.0;
    TRF_TMAT_GET(mat, 1, 0, elem_10);
    TRF_TMAT_GET(mat, 1, 1, elem_11);
    TRF_TMAT_GET(mat, 1, 2, elem_12);
    TRF_TMAT_GET(mat, 1, 3, elem_13);
    U_MESSAGE(FTOASCII(elem_10)+" "+FTOASCII(elem_11)+" "+FTOASCII(elem_12)+" "+FTOASCII(elem_13));
    elem_20=0.0;
    elem_21=0.0;
    elem_22=0.0;
    elem_23=0.0;
    TRF_TMAT_GET(mat, 2, 0, elem_20);
    TRF_TMAT_GET(mat, 2, 1, elem_21);
    TRF_TMAT_GET(mat, 2, 2, elem_22);
    TRF_TMAT_GET(mat, 2, 3, elem_23);
    U_MESSAGE(FTOASCII(elem_20)+" "+FTOASCII(elem_21)+" "+FTOASCII(elem_22)+" "+FTOASCII(elem_23));
    elem_30=0.0;
    elem_31=0.0;
    elem_32=0.0;
    elem_33=0.0;
    TRF_TMAT_GET(mat, 3, 0, elem_30);
    TRF_TMAT_GET(mat, 3, 1, elem_31);
    TRF_TMAT_GET(mat, 3, 2, elem_32);
    TRF_TMAT_GET(mat, 3, 3, elem_33);
    U_MESSAGE(FTOASCII(elem_30)+" "+FTOASCII(elem_31)+" "+FTOASCII(elem_32)+" "+FTOASCII(elem_33));    
    U_MESSAGE("");
}