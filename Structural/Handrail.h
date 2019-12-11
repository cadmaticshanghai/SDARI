/* define tag*/
#define  HandrailType                "~01"            /* 代号           */
#define  HandrailDes                 "~02"            /* 描述           */
#define  PartNumber                  "~03"            /* 零件编号       */
#define  PlanningUnit                "~G2"            /* 托盘号         */
#define	HandrailSize			    "SSP"		     /* 存储尺寸参数   */	

global string   Group_Type    = "7";
global int      Group_Type_I  = 7;
global int     Ladder_Flatbar_Cut_Dist = 20;

global string Virtual_Hole_DT  = "Virtual Parts*";
global string Hole_Part_Code   = "+ZP2vIc3AGaEN3_ndq0M.GW";

/*方向夹角转换为方向向量*/
angle_to_direction(ang_s,ang_r, dx,dy,dz)
{
    slope_angle = String_To_Float(ang_s);
    rotation_angle = String_To_Float(ang_r);
    
    dx = COS(slope_angle) * COS(rotation_angle);
    dy = COS(slope_angle) * SIN(rotation_angle);
    dz = SIN(slope_angle);
    VEC_UNITV(dx,dy,dz);
    return (0);
}

/*获取零件的坐标*/
get_part_position(part_handle, px,py,pz)
{
    px = 1.0 * PM_GET_OBJDATA(part_handle,0,"lox");
    py = 1.0 * PM_GET_OBJDATA(part_handle,0,"loy");
    pz = 1.0 * PM_GET_OBJDATA(part_handle,0,"loz");
}

/*获取零件的方位*/
get_part_orientation(part_handle, ux,uy,uz, vx,vy,vz)
{
    wx=0;wy=0;wz=0;
    /*axis direction of beam*/
    axs = PM_GET_OBJDATA(part_handle,0,"axs");
    axr = PM_GET_OBJDATA(part_handle,0,"axr");
    angle_to_direction(axs,axr, wx,wy,wz);

    /*normal direction of beam*/
    sxs = PM_GET_OBJDATA(part_handle,0,"sxs");
    sxr = PM_GET_OBJDATA(part_handle,0,"sxr");
    angle_to_direction(sxs,sxr, ux,uy,uz);
    
    Vector3d_CrossProduct(wx,wy,wz, ux,uy,uz, vx,vy,vz);
}

/*字符串包含*/
string_find(string_input, patt)
{
    check = PATTERN_MATCH(string_input,"*"+patt+"*");
    return(check);
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

/*延指定方向旋转*/
rotate_part(px,py,pz, nx,ny,nz, angle, part_handle)
{
    temp_set = PM_INIT_SET();
    PM_ADD_OBJECT_TO_SET(part_handle, temp_set);
    if (PM_WRITE_MDL_OF_SET("rotate_part.mdl", temp_set) != 0){
        PM_FREE_SET(temp_set);
        return(-1);
    }
    
    /* 旋转前 */
    px1=0;py1=0;pz1=0;
    ux1=0;uy1=0;uz1=0;
    vx1=0;vy1=0;vz1=0;
    get_part_position(part_handle, px1,py1,pz1);
    get_part_orientation(part_handle, ux1,uy1,uz1, vx1,vy1,vz1);
    
    /* 旋转后 */
    px2=0;py2=0;pz2=0;
    ux2=0;uy2=0;uz2=0;
    vx2=0;vy2=0;vz2=0;
    Point_Rotate(px1,py1,pz1, px2,py2,pz2, px,py,pz, nx,ny,nz, angle);
    Vector3d_Rotate(ux1,uy1,uz1, nx,ny,nz, angle, ux2,uy2,uz2);
    Vector3d_Rotate(vx1,vy1,vz1, nx,ny,nz, angle, vx2,vy2,vz2);
    
    /* 旋转前的转换矩阵 */
	trans_1 = PM_CREATE_TMAT(1, px1,py1,pz1, ux1,uy1,uz1, vx1,vy1,vz1);
	trans_1 = PM_INVERSE_TMAT(trans_1);

    /* 变换到原点 */
    trans_0 = TRF_TMAT_CREATE();
    trans_2 = PM_JOIN_TMATS(trans_0, trans_1);
    
    /* 变换到新的方位 */
    trans_3 = PM_CREATE_TMAT(1, px2,py2,pz2, ux2,uy2,uz2, vx2,vy2,vz2);
    trans_2 = PM_JOIN_TMATS(trans_2, trans_3);
    /* print_matrix(trans); */
    
	/*Notify PM that we are going to relocate objects using a sequence of deletes and MDL-loads. */
	if (!PM_OK_TO_RELOCATE_OBJECTS_IN_SET(temp_set)) {
		F_DELETE_FILE("rotate_part.mdl");
        PM_FREE_SET(temp_set);
		return(-1);
	}

	PM_DELETE_OBJECTS_IN_SET(temp_set, 0);
	PM_READ_MDL_INTO_SET("rotate_part.mdl", 1, "", trans_2, temp_set);
	PM_RELOCATION_ACCEPTED();
	F_DELETE_FILE("rotate_part.mdl");
    PM_FREE_SET(temp_set);
	return(0);
}

/*延指定方向复制*/
copy_move_step(wx,wy,wz, distance, step_handle)
{
    step_set = PM_INIT_SET();
    PM_ADD_OBJECT_TO_SET(step_handle, step_set);
    if (PM_WRITE_MDL_OF_SET("copy_move_step.mdl", step_set) != 0){
        PM_FREE_SET(step_set);
        return(-1);
    }

    px=0;py=0;pz=0;
    Point_Move(px,py,pz, px,py,pz, wx,wy,wz, distance);

    /* 平移变换 */
    trans = TRF_TMAT_CREATE();
    trans = Trans_Translate(trans, px,py,pz);
    /* print_matrix(trans); */
    
	/*Notify PM that we are going to relocate objects using a sequence of deletes and MDL-loads. */
	if (!PM_OK_TO_RELOCATE_OBJECTS_IN_SET(step_set)) {
		F_DELETE_FILE("copy_move_step.mdl");
        PM_FREE_SET(step_set);
		return(-1);
	}

	/* PM_DELETE_OBJECTS_IN_SET(step_set, 0); */
	PM_READ_MDL_INTO_SET("copy_move_step.mdl", 2, "", trans, step_set);
	PM_RELOCATION_ACCEPTED();
	F_DELETE_FILE("copy_move_step.mdl");
    
    part_number = PM_NR_MEMBERS_IN_SET(step_set);        
    for(i=0;i<part_number;i=i+1;){
        part_handle = PM_GET_MEMBER_IN_SET(step_set,i);
        /* part_id = PM_GET_OBJDATA(part_handle,0, MMT_TAG_PARTID); */
        if(part_handle!=step_handle){
            PM_FREE_SET(step_set);
            return(part_handle);
        }
    }
    PM_FREE_SET(step_set);
	return(-1);
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

/*向量的比例放大*/      
Vector3d_BlankProduct(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float sc)
{
    dx2 = dx1 * sc;
    dy2 = dy1 * sc;
    dz2 = dz1 * sc;
} 

/*向量的模*/      
Vector3d_Length(float dx, float dy, float dz)
{
    return (SQRT(dx*dx + dy*dy + dz*dz));
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

Abs(float x)
{
    if(x>=0){
        return(x);
    }
    return(-1*x);
}

Eq(float x, float x0)
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

/*判定奇数和偶数*/
IsEven(int x)
{
    int tmp;
    tmp = 0;
    tmp = x/2;
    tmp = tmp*2;
    if(tmp==x){
        return(1);
    }
    return (0);
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

/*分割取值*/
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
