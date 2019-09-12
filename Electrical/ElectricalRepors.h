#define	Tag_Block_Number             ".m1"			/*�����ֶ�*/
#define	Tag_PlanningUnit             "~G2"			/*���̴���*/
#define	Tag_Description              "~C1"			/*��������*/
#define	Tag_Tray_Remark              ""			    /*��ע��Ϣ*/
#define	Tag_Tray_Type                "~C2"          /*int - 0 ����������, 1 ����, 2 ˫��, 3 ���� */
#define	Tag_Tray_Group               "~01"          /*string - �м�������� */


#define	Tag_Pad_Type				 "~P2"			/* ������Ƭ���� */
#define	Tag_Part_Number			     "~P0"			/* ���������� */
#define	Tag_Seat_Desc				 "~G3"			/* ��������     */
#define	Tag_Seat_Type			     "~G1"			/* ��������     */
#define	Tag_Seat_Para                "SSP"          /*string - �洢�Ļ������� */
#define	Tag_Seat_No			         "~G4"			/* ����������   */

#define	Tag_Equip_Desc				 ".dG"			/* �豸����     */
#define	Tag_Equip_Type				 "UG3"			/* �豸�ͺ�     */
#define	Tag_Equip_Supplier		     "UG2"			/* �豸����     */



/*ͨ���ռ䷶Χ�ж�֧�ܲ���*/
get_tray_layer(bar_handle)
{
    layer = 0;
    obj_type = PM_GET_OBJDATA(bar_handle,0, MMT_TAG_OBJTYPE);
    if (obj_type==5){
        xmin = 0; xmax = 0; ymin = 0; ymax = 0; zmin = 0; zmax = 0;
        res = PM_GET_BBOX_OF_OBJECT(bar_handle, xmin,xmax, ymin,ymax, zmin,zmax); 
        trays = search_mdels_intersect_box(xmin,ymin,zmin, xmax,ymax,zmax, 7);
        if(!ISINT(trays)){
            tray_number = PM_NR_MEMBERS_IN_SET(trays); 
            layer = tray_number;
            return(layer);
        }
    }
	return(layer);
}

search_mdels_intersect_box(x1,y1,z1,x2,y2,z2,obj_type)
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
	return(0);
}

/*���ϻ���*/
/*inputlist  �ַ�����ά����ARRAY*/
/*judgecols  �ж���ͬ����ARRAY*/
/*sumcols    Ҫ�ۼӵ���ARRAY*/
/*reslist    ���ض�ά����ARRAY*/
/*tagindexs  ���������������ARRAY*/
summation(inputlist, judgecols, sumcols, tagindexs)
{
    rows = nrows_2darray(inputlist);
    cols = ncols_2darray(inputlist);
    judge_rows = get_array_length(judgecols);

    /*��Ч����*/
    inputnum = 0;
    templist = DM_VECTOR_CREATE();
    for (i=0;i<rows;i=i+1){
        tempstr = ""; 
        for (j=0;j<judge_rows;j=j+1){
            part_val = string_val(get_2darray(inputlist,i,A_GET(judgecols,j))); 
            if(part_val!="0"){
                tempstr = tempstr+ "<*>" + part_val; 
            }
        }
        if(tempstr!=""){
            DM_VECTOR_PUSH_BACK(templist, tempstr);
            inputnum = inputnum + 1;
        }
    }

    uniquelist = DM_VECTOR_CREATE();
    vector_unique(templist,uniquelist);
    DM_VECTOR_SORT(uniquelist, "string", DM_ARRAY_SORT_ASCENDING); 
    urows = DM_VECTOR_SIZE(uniquelist);
    reslist = alloc_2darray(urows, cols);
    taglist = DM_VECTOR_CREATE();

    /*�ۼ����*/
    sum_rows = get_array_length(sumcols);
    for (m=0;m<urows;m=m+1){
        for (s=0;s<sum_rows;s=s+1){
            sum_m = 0.0;
            tag_m = "";
            cell_m = DM_VECTOR_GET(uniquelist,m); 
            for (n=0;n<inputnum;n=n+1){
                cell_n = DM_VECTOR_GET(templist,n); 
                if (cell_m==cell_n){
                    sum_m = sum_m + float_val(get_2darray(inputlist,n,A_GET(sumcols,s)));
                    /* sum_m = sum_m + float_val(get_2darray(inputlist,n,sumindex)); */
                    tag_m = tag_m + string_val(n) + ",";
                }
            }
            
            /*����3λС��*/
            str_sum_m = round_float_as_string(sum_m, 3);
            put_2darray(reslist,m,A_GET(sumcols,s),rtrim_number(str_sum_m));
        }
        DM_VECTOR_PUSH_BACK(taglist, string_val(tag_m));
    }

    for (i=0;i<urows;i=i+1){
        /*����,�ָ�*/
        tag  = DM_VECTOR_GET(taglist,i); 
        if(tagindexs) A_PUT(tagindexs,i,tag);

        index = String_To_Int(STRINGTERM(tag, ","));
        for (j=0;j<cols;j=j+1){
            flag = get_2darray(reslist,i,j);
            if(flag<=0){
                put_2darray(reslist,i,j, get_2darray(inputlist,index,j));
            }
        }
    }
    
    DM_VECTOR_DELETE(taglist); 
    DM_VECTOR_DELETE(templist); 
    DM_VECTOR_DELETE(uniquelist);
    return(reslist);
}

/*ת��Ϊ�ַ���*/
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

/*�����±�*/
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

/*ȥ��*/
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

string_to_int(string int_string)
{
    int value;
    value = 0;
    temp = HEAD(int_string,1);
    if(temp>="0" & temp<="9"){
        value = SCAN_QUANT(3,int_string);
    }
    return(value);
}

string_to_abs_int(string int_string)
{
    int value;
    value = 0;
    temp = HEAD(int_string,1);
    if(temp>="0" & temp<="9"){
        value = SCAN_QUANT(3,int_string);
        if (value<0) value=-value;
    }
    return(value);
}

string_to_abs_float(string float_string)
{
    value = 0.0;
    temp = HEAD(float_string,1);
    if(temp>="0" & temp<="9"){
        value = SCAN_QUANT(3,float_string);
        if(value<0) value=-value;
    }
    return(value);
}

string_to_float(string float_string)
{
    value = 0.0;
    temp = HEAD(float_string,1);
    if(temp>="0" & temp<="9"){
        value = SCAN_QUANT(5,float_string);
    }
    return(value);
}

/*��ֵ�ַ������޳������0*/
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

/*ת��Ϊ��*/
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

/* ����nλС������Ч���� */
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

/*һά���鳤�ȣ����ַ���������Ч*/
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
    U_CONFIRM("�����±�Խ��");
    return(i);
}

/*�ж����м�֧�ż�*/
is_trayleg_part(part)
{
    group_handle = PM_GET_OBJECT_GROUP(part, 7);
    if(ISINT(group_handle)) return(0);        

    /* �ж��Ƿ�����м�֧�� */
    seat_type = PM_GET_OBJDATA(group_handle, 0, Tag_Seat_Type);
    if(ISINT(seat_type)) return(0);
    check = get_string_split(seat_type,"_",0);
    if(check != "CTS") return(0); 
    
    /*
    part_type = PM_GET_OBJDATA(part,0, MMT_TAG_OBJTYPE);
    if(part_type!="4"&part_type!="5") return(0); 
    */
    
    /*
    part_number = PM_GET_OBJDATA(part, 0, Tag_Part_Number);
    if(ISINT(part_number)) return(0);
    check = get_string_split(part_number,"-",0);
    if(float_val(check)>0) return(1);
    */
    
    return(1);
}

/*�ж����豸����*/
is_equipseat_group(group_handle)
{
    /* �ж��Ƿ�����豸���� */
    seat_type = PM_GET_OBJDATA(group_handle, 0, Tag_Seat_Type);
    if(ISINT(seat_type)){
        return(0);             
    }
    else{
        check = get_string_split(seat_type,"_",0);
        if(check == "EES"){
            return(1);
        }
    }	
    return(0);
}

/*�ж����豸����*/
is_equipseat_part(part)
{
    /*��ȡ�����豸����������*/
    group_handle = PM_GET_OBJECT_GROUP(part, 7);
    if(ISINT(group_handle)){
        return(0);        
    }
    /* �ж��Ƿ�����豸���� */
    res = is_equipseat_group(group_handle);
    return(res);
}

/*�ж��ǵ����豸*/
is_equip_part(part)
{
    equi_type = PM_GET_OBJDATA(part,0, "eTy");
    if(!ISINT(equi_type)){
        if(equi_type=="�����豸"){
            return(1);
        }
    }
    return(0);
}

/*�ж������Ϻ�*/
is_stuffing_part(part)
{
    equi_type = PM_GET_OBJDATA(part,0, "eTy");
    if(!ISINT(equi_type)){
        if(equi_type=="���Ϻ�"){
            return(1);
        }
    }
    return(0);
}

/*�ж��ǵ��¹�(�쾱��)*/
is_cabletube_part(part)
{
    equi_type = PM_GET_OBJDATA(part,0, "eTy");
    if(!ISINT(equi_type)){
        if(equi_type=="���¹�"){
            return(1);
        }
    }
    return(0);
}

/*�ж��ǵ��±���*/
is_flatbar_part(part)
{
    part_type = PM_GET_OBJDATA(part,0, MMT_TAG_OBJTYPE);
    if(part_type== "7"){
        part_pid = PM_GET_OBJDATA(part,0, "pid");
        if(ISSTRING(part_pid)){
            value = DM_PARTID_DATA(part_pid,"KW");
            if(ISSTRING(value)){
                if(value=="FB"|value=="FC"){
                    return(1);
                }
            }
        }
    }
    return(0);
}

/*�ж��ǵ��¹�ͨ��*/
is_penetration_part(part)
{
    equi_type = PM_GET_OBJDATA(part,0, "eTy");
    if(!ISINT(equi_type)){
        if(equi_type=="���¹�ͨ��"){
            return(1);
        }
    }
    return(0);
}

/*�ж��ǵ����м�*/
is_cabletray_part(part)
{
    part_type = PM_GET_OBJDATA(part,0, MMT_TAG_OBJTYPE);
    if(part_type== "7"){
        tray_type = PM_GET_OBJDATA(part,0, Tag_Tray_Type);
        if(tray_type>0){
            part_pid = PM_GET_OBJDATA(part,0, "pid");
            if(ISSTRING(part_pid)){
                keyword = DM_PARTID_DATA(part_pid,"KW");
                if(ISSTRING(keyword)){
                    if(keyword!="FB" & keyword!="FC"){
                        matrial = DM_PARTID_DATA(part_pid,"MC");
                        if(ISSTRING(keyword)){
                            if(matrial=="�����"){
                                return(2);
                            }
                        }
                        return(1);
                    }
                }
            }
        }
    }
    return(0);
}

/*��ȡ���ݿ��еĵ�һ����ע���Ķ���*/
get_first_drafting_style(string style_name)
{
	obj_list = COS_QUERY("Drafting Style", "");
	nr_objs = A_SIZE(obj_list);
	if(nr_objs < 1){
		A_FREE(obj_list);
		return("");
	}
	cos_id = A_GET(obj_list, 0);
	COS_READ_OBJECT(cos_id);
	attrs = COS_GET_OBJECT_ATTRIBUTES(cos_id);
	if(ISINT(attrs)){
		COS_FREE_OBJECT(cos_id);
		A_FREE(obj_list);
		return("");
	}
	
	style_name = DM_GET_TAGVAL(attrs, ".dD");
	
	DM_FREE_TAGREC(attrs);
	COS_FREE_OBJECT(cos_id);
	A_FREE(obj_list);
	return(cos_id);
}

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

/*�ָ�ȡֵ*/
get_string_split(input_string,separator,index)
{
    res_str = "";
    temp_str = input_string;
    vector = DM_VECTOR_CREATE();
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
    if(index>=0){
        if(index<DM_VECTOR_SIZE(vector)){
            res = DM_VECTOR_GET(vector,index);
            if(ISSTRING(res)){
                res_str = res;
            }
        }
    }
    else{
        if(-1*index<DM_VECTOR_SIZE(vector)+1){
            rindex = DM_VECTOR_SIZE(vector)+index;
            res = DM_VECTOR_GET(vector,rindex);
            if(ISSTRING(res)){
                res_str = res;
            }
        }
    }

    DM_VECTOR_DELETE(vector); 
    return(res_str);
}

/*�ָ�*/
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

/*�滻*/
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
