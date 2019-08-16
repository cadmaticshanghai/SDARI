/*Tag definition of Penetration*/

#define  Planning_Unit        "~PS1"                                 /* 贯通件托盘号取值管子托盘 */
#define  Surface_Treatment    "Udc"

#define	Tag_Spool_PlanningUnit              "PS1"                   /*管段托盘代号*/
#define	Tag_Spool_Name                      "PSA"                   /*管段管件号*/

#define	Tag_Penetration_FabDn			    "FB1"					/*贯通件型号规格*/
#define	Tag_Penetration_WeldType		    "FB2"					/*贯通件焊接类型*/
#define	Tag_Penetration_Remark      	    "FB3"					/*贯通件备注信息*/
#define	Tag_Penetration_Name        	    "FB4"					/*贯通件编号名称*/
#define	Tag_Penetration_SurfaceTreatment    "FB5"					/*贯通件表面处理*/
#define	Tag_Hole_Stage                      "HM1"					/*开孔阶段*/
#define	Tag_Hole_Type                       "HM2"					/*开孔类型*/

/* 定义项目信息对应的TAG定义 */
#define Tag_Project_Description              ".dG"                   /* 项目描述     */
#define Tag_Project_Number                   "U32"                   /* 项目编号     */

global string  File_Path    = "C:\\";
global string System_1 = "N-C Hull HoleRequests";
global string System_2 = "NAPA HolesRequests";


/*贯通件编号*/
auto_naming(planning_unit,models)
{
    index_number = "000";
    for(i=1;i<1000;i=i+1;){
        hole_name = planning_unit + "-PH-" + HEAD(index_number,STRLEN(index_number) - STRLEN(ITOASCII(i))) + ITOASCII(i);
        find = find_isnamed(hole_name,models);
        if(find<1){
            return(hole_name);
        }
    }
    return("Undefined"); 
}

find_isnamed(hole_name,models)
{
	hole_number = PM_NR_MEMBERS_IN_SET(models);
	for (i = 0; i < hole_number; i = i + 1;){
		hole = PM_GET_MEMBER_IN_SET(models, i);	
        value = PM_GET_OBJDATA(hole,0,Tag_Penetration_Name);
        if(ISSTRING(value)){
            if(hole_name==value){
                return (1);
            }
        }	
	}
    return (0);
}

/*字符串包含*/
string_find(string_input, patt)
{
    check = PATTERN_MATCH(string_input,"*"+patt+"*");
    return(check);
}

relocate(hole, description, face, x1,y1,z1, x2,y2,z2)
{
    set = PM_INIT_SET();
    PM_ADD_OBJECT_TO_SET(hole, set);
    nr = PM_NR_MEMBERS_IN_SET(set);
    if (nr == 0) {
        U_MESSAGE("Empty set, operation canceled");
        return(-1);
    }
    if (PM_WRITE_MDL_OF_SET(File_Path+"hole.mdl", set) != 0){
        return(-1);
    }
    
    /*解析mdl文件*/
    relocate_mdl_file(description, face, x1,y1,z1, x2,y2,z2);
    PM_FREE_SET(set);

    if( F_DELETE_FILE(File_Path+"hole.mdl") ){
        U_MESSAGE("Cannot delete file hole.mdl.");
    }
	return(1);
}

/*文件获取参数*/
relocate_mdl_file(description, face, x1,y1,z1, x2,y2,z2)
{
	file_path = File_Path+"hole.mdl";
    if(!F_EXIST(file_path)){
        U_CONFIRM("文件" + file_path + "不存在");
        return(-1);
    }
    f = F_OPEN_FILE(file_path, "r");
    if( f < 0 ){
		U_CONFIRM("Cannot open file <"+file_path+">");
		return(-1);
	}
    
    /*文件行数*/
    file_lines = F_NR_LINES(f);
    F_CLOSE_FILE(f);
    
    line_val = "";
    shape = "";
    para_str = "";
    pos_str = "";

    f = F_OPEN_FILE(file_path,"r");
    for( i=0; i<file_lines; i=i+1; ){
        F_READ_LINE(f, line_val);
        /*剔除空格*/
        line_val = rtrim(line_val); 

        /*形状参数*/
        if (SEARCH(line_val, "CURVE")!=""){
            para_str = line_val;
        }
        if (SEARCH(line_val, "Circle")!=""){
            shape = "D";
        }
        if (SEARCH(line_val, "BicycleChain")!=""){
            shape = "HOR";
        }
        if (SEARCH(line_val, "RectR")!=""){
            shape = "HR";
        }
        if (SEARCH(line_val, "TriangR")!=""){
            shape = "TR";
        }
        
        /*坐标行*/
        if (SEARCH(line_val, "P(")!="" & SEARCH(line_val, "SWEEP")==""){
            pos_str = line_val;
        }
    }
    F_CLOSE_FILE(f);
    U_MESSAGE("");

    get_face(pos_str,face);
    if(shape=="D"){
        d  = "?";
        get_para_for_type_d(para_str,d);
        description = shape+d;
        get_pos_for_type_d(pos_str,x1,y1,z1);
    }
    if(shape=="HOR"){
        a  = "?";
        r1 = "?";
        r2 = "?";
        get_para_for_type_hor(para_str,a,r1,r2);
        description = shape+a+"x"+r1+"x"+r2;
        get_pos_for_type_hor(pos_str,a,r1,r2,x1,y1,z1,x2,y2,z2);
    }
    if(shape=="HR"){
        a  = "?";
        b  = "?";
        r  = "?";
        get_para_for_type_hr(para_str,a,b,r);
        if(string_to_abs_float(b)-2*string_to_abs_float(r)<2){
            shape = "HO";
            description = shape+a+"x"+b;
            get_pos_for_type_ho(pos_str,a,b,x1,y1,z1,x2,y2,z2);
        }
        else{
            description = shape+a+"x"+b+"x"+r;
            get_pos_for_type_hr(pos_str,a,r,x1,y1,z1,x2,y2,z2);
        }
    }
    if(shape=="TR"){
        a  = "?";
        b  = "?";
        r1 = "?";
        r2 = "?";
        r3 = "?";
        get_para_for_type_tr(para_str,a,b,r1,r2,r3);
        description = shape+a+"x"+b+"x"+r1+"x"+r2+"x"+r3;
    }
    return(0);
}

/*开孔面*/
get_face(pos_str,face)
{
    ux = "";uy = "";uz = "";
    vx = "";vy = "";vz = "";
    get_udir_center(pos_str,ux,uy,uz);
    get_vdir_center(pos_str,vx,vy,vz);
    
    w_x = 0.0;w_y = 0.0;w_z = 0.0;
    Vector3d_CrossProduct(string_to_float(ux), string_to_float(uy),string_to_float(uz),
                          string_to_float(vx), string_to_float(vy),string_to_float(vz),
                          w_x,w_y,w_z);
    maxValue = abs(w_x);
    face = "X";
    if(maxValue<abs(w_y)){
        maxValue = abs(w_y);
        face = "Y";
    }
    if(maxValue<abs(w_z)){
        face = "Z";
    }
}

abs(float x)
{
    if(x>=0){
        return(x);
    }
    return(-1*x);
}

get_position_ref(int x0, int y0, int z0, string x_ref, string y_ref, string z_ref)
{
   int x_off,y_off,z_off;
   x_off = 0;y_off = 0;z_off = 0;
   GetReferenceCoords(x0, y0, z0, x_ref, x_off, y_ref, y_off, z_ref, z_off);
   if(x_off > 0){
      x_ref = x_ref + "+" + ITOASCII(x_off);
   }
   else if(x_off < 0){
      x_ref = x_ref + ITOASCII(x_off);
   }
   if(y_off > 0){
      y_ref = y_ref + "+" + ITOASCII(y_off);
   }
   else if(y_off < 0){
      y_ref = y_ref + ITOASCII(y_off);
   }
   if(z_off > 0){
      z_ref = z_ref + "+" + ITOASCII(z_off);
   }
   else if(z_off < 0){
      z_ref = z_ref + ITOASCII(z_off);
   }
   return(0);
}

/*P(16493.98,839.23,12800),D(1,0,0),D(0,-1,0),*/
get_pos_center(pos_str,x,y,z)
{
    vector = DM_VECTOR_CREATE();
    string_split(pos_str,"),",vector);
    temp_str = DM_VECTOR_GET(vector, 0); 
    temp_str = SUBSTRING(SEARCH(temp_str,"P("),2);
    string_split(temp_str,",",vector);
    x = DM_VECTOR_GET(vector, 0); 
    y = DM_VECTOR_GET(vector, 1); 
    z = DM_VECTOR_GET(vector, 2); 
    DM_VECTOR_DELETE(vector); 
}

/*P(16493.98,839.23,12800),D(1,0,0),D(0,-1,0),*/
get_udir_center(pos_str,ux,uy,uz)
{
    vector = DM_VECTOR_CREATE();
    string_split(pos_str,"),",vector);
    temp_str = DM_VECTOR_GET(vector, 1); 
    temp_str = SUBSTRING(SEARCH(temp_str,"D("),2);
    string_split(temp_str,",",vector);
    ux = DM_VECTOR_GET(vector, 0); 
    uy = DM_VECTOR_GET(vector, 1); 
    uz = DM_VECTOR_GET(vector, 2); 
    DM_VECTOR_DELETE(vector); 
}

/*P(16493.98,839.23,12800),D(1,0,0),D(0,-1,0),*/
get_vdir_center(pos_str,vx,vy,vz)
{
    vector = DM_VECTOR_CREATE();
    string_split(pos_str,"),",vector);
    temp_str = DM_VECTOR_GET(vector, 2); 
    temp_str = SUBSTRING(SEARCH(temp_str,"D("),2);
    string_split(temp_str,",",vector);
    vx = DM_VECTOR_GET(vector, 0); 
    vy = DM_VECTOR_GET(vector, 1); 
    vz = DM_VECTOR_GET(vector, 2); 
    DM_VECTOR_DELETE(vector); 
}

/*点1延方向移动一定距离得到点2*/
point_move(float x1, float y1, float z1, float x2, float y2, float z2, float v_x, float v_y, float v_z, float distance)
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

/*向量的叉积*/      
Vector3d_CrossProduct(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float dx3, float dy3, float dz3)
{
    dx3 = dy1*dz2 - dz1*dy2;
    dy3 = dz1*dx2 - dx1*dz2;
    dz3 = dx1*dy2 - dy1*dx2;
} 


get_pos_for_type_d(pos_str,x1,y1,z1)
{
    x0 = "";y0 = "";z0 = "";
    get_pos_center(pos_str,x0,y0,z0);
    get_position_ref(string_to_int(x0), string_to_int(y0), string_to_int(z0), x1,y1,z1);
}

/*c=CURVE(\"Circle\",0,95);*/
get_para_for_type_d(para_str,d)
{
    temp_str = STRINGTERM(para_str, ";");
    temp_str = SEARCH(temp_str, "0,");
    temp_str = SUBSTRING(temp_str, 2);
    d = HEAD(temp_str, STRLEN(temp_str)-1);
    S_PRINTF(d,"%.0f",string_to_abs_float(d));
}

get_pos_for_type_hor(pos_str,a,r1,r2,x1,y1,z1,x2,y2,z2)
{
    x0 = "";y0 = "";z0 = "";
    get_pos_center(pos_str,x0,y0,z0);
    get_position_ref(string_to_int(x0), string_to_int(y0), string_to_int(z0), x1,y1,z1);

    ux = "";uy = "";uz = "";
    get_udir_center(pos_str,ux,uy,uz);
    
    dist = string_to_float(a) - string_to_float(r1) - string_to_float(r2);
    rx=0.0;ry=0.0;rz=0.0;
    point_move(string_to_float(x0), 
               string_to_float(y0),
               string_to_float(z0),
               rx,ry,rz,
               string_to_float(ux),
               string_to_float(uy),
               string_to_float(uz),
               dist);
    get_position_ref(string_to_int(FTOASCII(rx)), string_to_int(FTOASCII(ry)), string_to_int(FTOASCII(rz)), x2,y2,z2);
}

/*c=CURVE(\"BicycleChain\",0,252.5,72.5,50);*/
get_para_for_type_hor(para_str,a,r1,r2)
{
    vector = DM_VECTOR_CREATE();
    string_split(STRINGTERM(para_str, ")"),",",vector);
    a  = DM_VECTOR_GET(vector, 2); 
    r1 = DM_VECTOR_GET(vector, 3); 
    r2 = DM_VECTOR_GET(vector, 4); 
    DM_VECTOR_DELETE(vector); 
    S_PRINTF(a,"%.0f",string_to_abs_float(a));
    S_PRINTF(r1,"%.0f",string_to_abs_float(r1));
    S_PRINTF(r2,"%.0f",string_to_abs_float(r2));
}

get_pos_for_type_ho(pos_str,a,b,x1,y1,z1,x2,y2,z2)
{
    x0 = "";y0 = "";z0 = "";
    get_pos_center(pos_str,x0,y0,z0);
    
    ux = "";uy = "";uz = "";
    get_udir_center(pos_str,ux,uy,uz);
    
    dist = 0.5*(string_to_float(a) - string_to_float(b));
    rx1=0.0;ry1=0.0;rz1=0.0;
    rx2=0.0;ry2=0.0;rz2=0.0;
    point_move(string_to_float(x0), 
               string_to_float(y0),
               string_to_float(z0),
               rx1,ry1,rz1,
               string_to_float(ux),
               string_to_float(uy),
               string_to_float(uz),
               dist);
    point_move(string_to_float(x0), 
               string_to_float(y0),
               string_to_float(z0),
               rx2,ry2,rz2,
               string_to_float(ux),
               string_to_float(uy),
               string_to_float(uz),
               -dist);
    get_position_ref(string_to_int(FTOASCII(rx1)), string_to_int(FTOASCII(ry1)), string_to_int(FTOASCII(rz1)), x1,y1,z1);
    get_position_ref(string_to_int(FTOASCII(rx2)), string_to_int(FTOASCII(ry2)), string_to_int(FTOASCII(rz2)), x2,y2,z2);
}

get_pos_for_type_hr(pos_str,a,r,x1,y1,z1,x2,y2,z2)
{
    x0 = "";y0 = "";z0 = "";
    get_pos_center(pos_str,x0,y0,z0);
    
    ux = "";uy = "";uz = "";
    get_udir_center(pos_str,ux,uy,uz);
    
    dist = 0.5*(string_to_float(a) - 2*string_to_float(r));
    rx1=0.0;ry1=0.0;rz1=0.0;
    rx2=0.0;ry2=0.0;rz2=0.0;
    point_move(string_to_float(x0), 
               string_to_float(y0),
               string_to_float(z0),
               rx1,ry1,rz1,
               string_to_float(ux),
               string_to_float(uy),
               string_to_float(uz),
               dist);
    point_move(string_to_float(x0), 
               string_to_float(y0),
               string_to_float(z0),
               rx2,ry2,rz2,
               string_to_float(ux),
               string_to_float(uy),
               string_to_float(uz),
               -dist);
    U_MESSAGE();
    get_position_ref(string_to_int(FTOASCII(rx1)), string_to_int(FTOASCII(ry1)), string_to_int(FTOASCII(rz1)), x1,y1,z1);
    get_position_ref(string_to_int(FTOASCII(rx2)), string_to_int(FTOASCII(ry2)), string_to_int(FTOASCII(rz2)), x2,y2,z2);
}

/*c=CURVE(\"RectR\",0,516.423,239,119.5);*/
get_para_for_type_hr(para_str,a,b,r)
{
    vector = DM_VECTOR_CREATE();
    string_split(STRINGTERM(para_str, ")"),",",vector);
    a  = DM_VECTOR_GET(vector, 2); 
    b  = DM_VECTOR_GET(vector, 3); 
    r  = DM_VECTOR_GET(vector, 4); 
    DM_VECTOR_DELETE(vector); 
    S_PRINTF(a,"%.0f",string_to_abs_float(a));
    S_PRINTF(b,"%.0f",string_to_abs_float(b));
    S_PRINTF(r,"%.0f",string_to_abs_float(r));
}

/*c=CURVE(\"TriangR\",0,505,390,60,60,60);*/
get_para_for_type_tr(para_str,a,b,r1,r2,r3)
{
    vector = DM_VECTOR_CREATE();
    string_split(STRINGTERM(para_str, ")"),",",vector);
    a  = DM_VECTOR_GET(vector, 2); 
    b  = DM_VECTOR_GET(vector, 3); 
    r1 = DM_VECTOR_GET(vector, 4); 
    r2 = DM_VECTOR_GET(vector, 5); 
    r3 = DM_VECTOR_GET(vector, 6); 
    DM_VECTOR_DELETE(vector); 
    S_PRINTF(a,"%.0f",string_to_abs_float(a));
    S_PRINTF(b,"%.0f",string_to_abs_float(b));
    S_PRINTF(r1,"%.0f",string_to_abs_float(r1));
    S_PRINTF(r2,"%.0f",string_to_abs_float(r2));
    S_PRINTF(r3,"%.0f",string_to_abs_float(r3));
}

string_to_abs_int(string int_string)
{
   int value;
   value = 0;
   value = SCAN_QUANT(3,int_string);
   if (value<0) value=-value;
   return(value);
}

string_to_abs_float(string float_string)
{
   value = 0.0;
   value = SCAN_QUANT(3,float_string);
   if (value<0) value=-value;
   return(value);
}

string_to_float(string float_string)
{
   value = 0.0;
   value = SCAN_QUANT(3,float_string);
   return(value);
}

string_to_int(string int_string)
{
   int value;
   value = 0;
   value = SCAN_QUANT(3,int_string);
   return(value);
}

/*字符串剔除空格*/
trim(string_val)
{
    res_str = "";
    str_len = STRLEN(string_val);
    for (i=1;i<str_len+1;i=i+1){
        temp = HEAD(string_val,i);
        temp = TAIL(temp,1);
        if(temp!=" "){
            res_str = res_str+temp;
        }
    }
    return(res_str);
}

/*字符串右剔除空格*/
rtrim(string_val)
{
    res_str = string_val;
    str_len = STRLEN(string_val);
    for (i=0;i<str_len;i=i+1){
        temp = TAIL(res_str,1);
        if(temp!=" "){
            return(res_str);
        }
        else{
            res_str = HEAD(res_str,STRLEN(res_str)-1);
        }
    }
    return(res_str);
}

/*数值字符串右剔除多余的0*/
rtrim_number(string_val)
{
    res_str = SEARCH(string_val, ".");
    if(STRLEN(res_str)){
        res_str = string_val;
        str_len = STRLEN(string_val);
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
    return(string_val);
}

/*数组长度*/
len_vector2d(vector2d)
{
    return(DM_2D_ARRAY_PUSH_BACK_ROW(vector2d));
}

/*数组长度*/
len_vector(vector)
{
    return(DM_VECTOR_SIZE(vector));
}

/*转换为字符串*/
string_val(c)
{
    if(ISINT(c)){
        return(ITOASCII(c));
    }
    else if(ISFLOAT(c)){
        return(FTOASCII(c));
    }
    else if(ISSTRING(c)){
        return(c);
    }
    else{
        return("");
    }
}

/*转换为数*/
float_val(c)
{
    if(ISINT(c)){
        return(1.0*c);
    }
    else if(ISFLOAT(c)){
        return(c);
    }
    else if(ISSTRING(c)){
        return(string_to_float(c));
    }
    else{
        return(0);
    }
}

/*一维数组长度，对字符串数组有效*/
get_array_length(arr)
{
    i = 0;
    while(i<3000){
        res = A_GET(arr, i);
        if (res==-1) {
            return(i);
        }
        i = i+1;
    }
    U_CONFIRM("数组下标越界");
    return(i);
}

/*一维数组转换为vector*/
array_to_vector(arr)
{
    rows = get_array_length(arr);
    vec = DM_VECTOR_CREATE(rows);
    for (i=0;i<rows;i=i+1){
        value = A_GET(arr, i);
        DM_VECTOR_PUSH_BACK(vec, value);
    }
    return(vec);
}

/*vector转换为一维数组*/
vector_to_array(vec)
{
    rows = DM_VECTOR_SIZE(vec);
    arr = A_ALLOC(rows);
    for (i=0;i<rows;i=i+1){
        value = DM_VECTOR_GET(vec, i); 
        A_PUT(arr, i, value);
    }
    return(arr);
}

/*二维数组转换为vector2d*/
array2d_to_vector2d(arr2d)
{
    nrows = nrows_2darray(arr2d);
    ncols = ncols_2darray(arr2d);
    vec2d = DM_2D_ARRAY_CREATE(nrows, ncols);
    for (i=0;i<nrows;i=i+1){
        for (j=0;j<ncols;j=j+1){
            value = get_2darray(arr2d,i,j);
            DM_2D_ARRAY_SET(vec2d, i, j, value);
        }
    }
    return(vec2d);
}

/*vector2d转换为二维数组*/
vector2d_to_array2d(vec2d)
{
    nrows = 0; ncols = 0;
    DM_2D_ARRAY_SIZE(vec2d, nrows, ncols); 
    arr2d = alloc_2darray(nrows, ncols);
    for (i=0;i<nrows;i=i+1){
        for (j=0;j<ncols;j=j+1){
            value = DM_2D_ARRAY_GET(vec2d, i, j); 
            put_2darray(arr2d,i,j,value);
        }
    }
    return(arr2d);
}
    
/*返回下标*/
vector_index(vector,value)
{
    index = -1;
    map_handle = DM_MAP_CREATE("STRING","INT"); 
    for (i=0;i<DM_VECTOR_SIZE(vector);i=i+1){
        DM_MAP_INSERT(map_handle, string_val(DM_VECTOR_GET(vector,i)), i);
    }
    res = DM_MAP_FIND(map_handle, value, index); 
    return(index);
}

/*去重*/
vector_unique(vector1,vector2)
{
    DM_VECTOR_CLEAR(vector2); 
    rows = DM_VECTOR_SIZE(vector1);
    for (i=0;i<rows;i=i+1){
        value = DM_VECTOR_GET(vector1, i);
        index = vector_index(vector2, value);
        if (index<0){
            DM_VECTOR_PUSH_BACK(vector2, value);
        }
    }
}

/*分割*/
string_split(input_string,separator,vector)
{
    temp_str = input_string;
    DM_VECTOR_CLEAR(vector); 
    while (temp_str!=""){
        left_str = STRINGTERM(temp_str, separator);
        right_str = SEARCH(temp_str, separator);
        DM_VECTOR_PUSH_BACK(vector, left_str);
        if(right_str!=""){
            temp_str = TAIL(right_str,STRLEN(right_str)-STRLEN(separator));
        }
        else{
            temp_str = right_str;
        }
    }
}

/*取值*/
get_vector2d_row(vector2d,row_index,vector)
{
    DM_VECTOR_CLEAR(vector); 
	nr_cols = DM_2D_ARRAY_SIZE_COLUMNS(vector2d);
	for(i=0;i<nr_cols;i=i+1){
		value = DM_2D_ARRAY_GET(vector2d, row_index, i);
        DM_VECTOR_PUSH_BACK(vector, value);
	}
}

/*赋值*/
set_vector2d_row(vector2d,row_index,vector)
{
	nr_cols = DM_2D_ARRAY_SIZE_COLUMNS(vector2d);
	for(i=0;i<nr_cols;i=i+1){
        value = DM_VECTOR_GET(vector,i); 
		DM_2D_ARRAY_SET(vector2d, row_index, i, value);
	}
}

/*数组累加求和计数*/
/*inputlist  字符串二维数组VECTOR2D*/
/*judgecols  判断相同的列VECTOR*/
/*rnumindex  计数的结果列*/
/*sumindex   要累加的列*/
/*rsumindex  累加结果列*/
/*reslist    返回二维数组VECTOR2D*/
/*taglist    返回索引VECTOR*/
summation_vector(inputlist, judgecols, rnumindex, sumindex, rsumindex, taglist)
{
    rows = 0; cols = 0;
    DM_2D_ARRAY_SIZE(inputlist, rows, cols); 

    templist = DM_VECTOR_CREATE(rows);
    for (i=0;i<rows;i=i+1){
        tempstr = "";
        for (j=0;j<DM_VECTOR_SIZE(judgecols);j=j+1){
            col = DM_VECTOR_GET(judgecols,j); 
            tempstr = tempstr+ "<*>" + string_val(DM_2D_ARRAY_GET(inputlist,i,col)); 
        }
        DM_VECTOR_PUSH_BACK(templist, tempstr);
    }

    uniquelist = DM_VECTOR_CREATE();
    vector_unique(templist,uniquelist);
    DM_VECTOR_SORT(uniquelist, "string", DM_ARRAY_SORT_ASCENDING); 
    
    urows = DM_VECTOR_SIZE(templist);
    /*计数*/
    numlist = DM_VECTOR_CREATE(urows);
    /*累加*/
    sumlist = DM_VECTOR_CREATE(urows);
    /*索引*/
    DM_VECTOR_CLEAR(taglist); 

    for (m=0;m<urows;m=m+1){
        num_m = 0;
        sum_m = 0;
        tag_m = "";
        cell_m = DM_VECTOR_GET(uniquelist,m); 
        for (n=0;n<rows;n=n+1){
            cell_n = DM_VECTOR_GET(templist,n); 
            if (cell_m==cell_n){
                num_m = num_m + 1;
                sum_m = sum_m + float_val(DM_2D_ARRAY_GET(inputlist,n,sumindex));
                tag_m = tag_m + string_val(n) + " ";
            }
        }
        DM_VECTOR_PUSH_BACK(numlist, num_m);
        DM_VECTOR_PUSH_BACK(sumlist, sum_m);
        DM_VECTOR_PUSH_BACK(taglist, tag_m);
    }

    reslist = DM_2D_ARRAY_CREATE(urows, cols);
    for (k=0;k<urows;k=k+1){
        tempstr = DM_VECTOR_GET(taglist,k); 
        tagstr  = HEAD(tempstr,STRLEN(tempstr)-1);
        
        indexlist = DM_VECTOR_CREATE();
        string_split(tagstr," ",indexlist);
        
        templine  = DM_VECTOR_CREATE();
        get_vector2d_row(inputlist,DM_VECTOR_GET(indexlist,0),templine);

        DM_VECTOR_SET(templine, rnumindex, string_val(DM_VECTOR_GET(numlist,k))); 
        
        sum_count = "";
        S_PRINTF(sum_count,"%.3f",string_to_float(string_val(DM_VECTOR_GET(sumlist,k))));
        DM_VECTOR_SET(templine, rsumindex, sum_count); 

        DM_VECTOR_SET(templine, 0, string_val(k+1)); 
        set_vector2d_row(reslist,k,templine);
    }
    
    DM_VECTOR_DELETE(templist); 
    DM_VECTOR_DELETE(uniquelist);
    DM_VECTOR_DELETE(numlist);
    DM_VECTOR_DELETE(sumlist);
    DM_VECTOR_DELETE(indexlist);
    DM_VECTOR_DELETE(templine);
    return(reslist);
}

/*数组累加求和计数*/
/*inputlist  字符串二维数组ARRAY2D*/
/*judgecols  判断相同的列ARRAY*/
/*rnumindex  计数的结果列*/
/*sumindex   要累加的列*/
/*rsumindex  累加结果列*/
/*reslist    返回二维数组ARRAY2D*/
/*taglist    返回索引ARRAY*/
summation_array(a_inputlist, a_judgecols, rnumindex, sumindex, rsumindex, a_reslist, a_taglist)
{
    v_inputlist = array2d_to_vector2d(a_inputlist);
    v_judgecols = array_to_vector(a_judgecols);
    v_taglist   = DM_VECTOR_CREATE();
    v_reslist   = summation_vector(v_inputlist, v_judgecols, rnumindex, sumindex, rsumindex, v_taglist);
    
    A_FREE(a_taglist);
    a_taglist = vector_to_array(v_taglist);
    
    free_2darray(a_reslist);
    a_reslist = vector2d_to_array2d(v_reslist);
    
    DM_2D_ARRAY_DELETE(v_inputlist);
    DM_2D_ARRAY_DELETE(v_judgecols);
    DM_2D_ARRAY_DELETE(v_taglist);
    DM_2D_ARRAY_DELETE(v_reslist);
    return(0);
}
