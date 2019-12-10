/*Define penetration tag - 定义贯通相关属性*/
#define	Tag_Penetration_FabDn				"FB1"							/*1.挡水圈，复板型号规格*/
#define	Tag_Penetration_WeldType			"FB2"							/*2.复板焊接类型*/
#define	Tag_Penetration_Remark      		"FB3"							/*3.挡水圈，复板备注信息*/
#define	Tag_Penetration_PlanningUnit		"PS1"							/*4.挡水圈、复板所属托盘（所属管段托盘）*/
#define	Tag_Penetration_Name        		"FB4"							/*5.挡水圈，复板编号名称*/
#define	Tag_Penetration_SurfaceTreatment    "FB5"							/*6.挡水圈表面处理*/
#define	Tag_Penetration_Type				".bv"							/*7.挡水圈类型*/

#define	Tag_BlockNumber				".m1"							    /*分段名称*/
#define	Tag_SupperBlock				".m0"							    /*总段名称*/

#define	Maximum_Thickness			20

#define	Tag_Spool_Name              "PSA"                          /*1.管段名称*/
#define	Tag_Spool_PlanningUnit      "PS1"                          /*2.托盘代号*/
global string  File_Path    = "C:\\temp\\";

/* 保留n位小数点有效数字 */
round_float_as_string(float x, int n)
{
    flg = 1;
    for(i=0;i<n;i=i+1){
        flg = flg*10;
    }
    temp = x*flg+0.5;
    temp_str =STRINGTERM(FTOASCII(temp), ".");
    part1 = HEAD(temp_str,STRLEN(temp_str)-n);
    if(part1==""){
        part1="0";
    }
    part2 = TAIL(temp_str,n);
    if(part2=="0"){
        for(i=1;i<n;i=i+1){
            part2 = part2 + "0";
        }
    }
    if(n==0){
        return(part1);
    }
    return(part1+"."+part2);
}
/* 托盘 */
get_planning_unit_from_part(part_handle)
{
    planning_unit = "Undefined";
    
    /* 取记录的托盘号 */
    value = PM_GET_OBJDATA(part_handle,0, Tag_Spool_PlanningUnit);
    if(ISSTRING(value)){
        if(value!=""){
            planning_unit = value;
        }
    }
    if(planning_unit=="Undefined"){
        /* 如果有分段，默认托盘为分段名 */
        planning_unit = PM_GET_OBJDATA(part_handle,0, Tag_BlockNumber);
        if(ISINT(planning_unit)){
            planning_unit = PM_GET_OBJDATA(part_handle,0, Tag_SupperBlock);
            if(ISINT(planning_unit)){
                planning_unit = "999";
            }
        }
    }
    return(
    planning_unit);
}

/* 管件号 */
/* 通过属性拼出管段号 */
/* 托盘号-管线号-编号 */
get_spool_name_from_part(part_handle)
{
    spool_name = "Undefined";
    
    /* 取记录的管段号 */
    value = PM_GET_OBJDATA(part_handle,0, Tag_Spool_Name);
    if(ISSTRING(value)){
        if(value!=""){
            spool_name = value;
        }
    }

    if(spool_name=="Undefined"){
        planning_unit = get_planning_unit_from_part(part_handle);
        /* U_MESSAGE("planning_unit="+planning_unit); */

        pipe_line = PM_GET_OBJDATA(part_handle, 0, "pli");
        /* U_MESSAGE("pipe_line="+pipe_line); */
        
        spool_number = PM_GET_OBJDATA(part_handle, 0, "spn");
        if(ISINT(spool_number)){
            /* U_CONFIRM("所选零件不属于任何管段"); */
            spool_name = pipe_line;
        }
        else{
            spool_name = planning_unit + "-" + pipe_line + "-" + spool_number;
        }
    }
    /* U_MESSAGE("spool_name="+spool_name); */
    return(spool_name);
}

relocate(penetration,description,length,area,weight)
{
    set = PM_INIT_SET();
	/*
    pent_parts = PM_GET_OBJECTS_IN_GROUP(penetration);
    part_number = PM_NR_MEMBERS_IN_SET(pent_parts);  
    for(i=0;i<part_number;i=i+1;){
        part = PM_GET_MEMBER_IN_SET(pent_parts,i);
        obj_type = PM_GET_OBJDATA(part,0, MMT_TAG_OBJTYPE);
        if(obj_type==4){
            PM_ADD_OBJECT_TO_SET(part, set);
        }
    }
	*/
    PM_ADD_OBJECT_TO_SET(penetration, set);
    nr = PM_NR_MEMBERS_IN_SET(set);
    if (nr == 0) {
        U_MESSAGE("Empty set, operation canceled");
        return(-1);
    }
    if (PM_WRITE_MDL_OF_SET(File_Path+"penetration.mdl", set) != 0){
        return(-1);
    }
    
    /*解析mdl文件*/
    relocate_mdl_file(description,length,area,weight);
    PM_FREE_SET(set);

    if( F_DELETE_FILE(File_Path+"penetration.mdl") ){
        U_MESSAGE("Cannot delete file penetration.mdl.");
    }
	return(1);
}

/*文件获取参数*/
relocate_mdl_file(description,length,area,weight)
{
	file_path = File_Path+"penetration.mdl";
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
    shape = -1;
    para_str = "";
    weight = "";
    f = F_OPEN_FILE(file_path,"r");
    for( i=0; i<file_lines; i=i+1; ){
        F_READ_LINE(f, line_val);
        /*剔除空格*/
        line_val = rtrim(line_val); 

        /*重量*/
        if (SEARCH(line_val, "MAS")!=""){
            weight_str = SEARCH(line_val, "(");
            weight_str = STRINGTERM(weight_str, ")");
            weight_str = SUBSTRING(weight_str,1);
            f_weight   = string_to_float(weight_str);
            if (f_weight>0.01){
                S_PRINTF(weight,"%.2f",string_to_float(weight_str));
            }
        }
        
        /*形状参数*/
        if (SEARCH(line_val, "CommonSleeveParams")!=""){
            /*轮廓形状 0 圆形; 1 等腰圆*/
            if (SEARCH(line_val, "shp 0")!=""){
                shape = 0;
            }
            if (SEARCH(line_val, "shp 1")!=""){
                shape = 1;
            }
            para_str = line_val;
        }
    }
    F_CLOSE_FILE(f);
    
    if (shape==0){
        temp_l=0; temp_d=0; temp_t=0; 
        get_para_for_type_0(para_str,temp_l,temp_d,temp_t);
        /*挡水圈长度*/
        length = ITOASCII(temp_l);
        /*展开面积*/
        f_area = 2*3.14159*temp_d*temp_l * 0.001 * 0.001;
        S_PRINTF(area,"%.3f",f_area);
        /*规格描述*/
        description = "钢管 Φ"+ITOASCII(2*temp_d)+"X"+ITOASCII(temp_t);
        return(1);
    } 
    else if(shape==1){
        temp_l=0; temp_w=0; temp_h=0; temp_t=0; 
        get_para_for_type_1(para_str,temp_l,temp_w,temp_h,temp_t);
        /*中心线长度*/
        f_length = 2*((temp_w-temp_t)-(temp_h-temp_t)) + 3.14159*(temp_h-temp_t);
        S_PRINTF(length,"%.0f",f_length);
        /*展开面积*/
        f_area = length*temp_l * 0.001 * 0.001;
        S_PRINTF(area,"%.3f",f_area);
        /*规格描述*/
        description = "扁钢 "+ITOASCII(temp_l)+"X"+ITOASCII(temp_t);
        return(1);
    }
    else{
        U_MESSAGE("未定义的挡水圈形状");
    }
    return(-1);
}

/*@CommonSleeveParams=shp 0;l 150;t 20;d 235;;*/
get_para_for_type_0(para_str,l,d,t)
{
    temp_str = SEARCH(para_str, ";");
    temp_str = SEARCH(temp_str, "l ");
    
    temp_str_l = STRINGTERM(temp_str, ";");
    temp_str_l = SEARCH(temp_str_l, " ");
    l = string_to_abs_int(temp_str_l);
    
    temp_str_t = SEARCH(temp_str, "t");
    temp_str_t = STRINGTERM(temp_str_t, ";");
    temp_str_t = SEARCH(temp_str_t, " ");
    t = string_to_abs_int(temp_str_t);
    
    temp_str_d = SEARCH(temp_str, "d");
    temp_str_d = STRINGTERM(temp_str_d, ";");
    temp_str_d = SEARCH(temp_str_d, " ");
    d = string_to_abs_int(temp_str_d);
}

/*@CommonSleeveParams=shp 1;l 150;t 20;w 505;h 265;;*/
get_para_for_type_1(para_str,l,w,h,t)
{
    temp_str = SEARCH(para_str, ";");
    temp_str = SEARCH(temp_str, "l ");

    temp_str_l = STRINGTERM(temp_str, ";");
    temp_str_l = SEARCH(temp_str_l, " ");
    l = string_to_abs_int(temp_str_l);

    temp_str_t = SEARCH(temp_str, "t");
    temp_str_t = STRINGTERM(temp_str_t, ";");
    temp_str_t = SEARCH(temp_str_t, " ");
    t = string_to_abs_int(temp_str_t);
    
    temp_str_w = SEARCH(temp_str, "w");
    temp_str_w = STRINGTERM(temp_str_w, ";");
    temp_str_w = SEARCH(temp_str_w, " ");
    w = string_to_abs_int(temp_str_w);

    temp_str_h = SEARCH(temp_str, "h");
    temp_str_h = STRINGTERM(temp_str_h, ";");
    temp_str_h = SEARCH(temp_str_h, " ");
    h = string_to_abs_int(temp_str_h);
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




