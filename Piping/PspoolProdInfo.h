/*Define system tag - ����ϵͳ�������*/
#define	Tag_System_SurfTreatPre     "~S1"                       /*1. ϵͳ����Ԥ���� */
#define	Tag_System_SurfTreatIn      "~S2"                       /*2. ϵͳ�ڱ��洦�� */
#define	Tag_System_SurfTreatOut     "~S3"                       /*3. ϵͳ����洦�� */
#define	Tag_System_GasketMaterial   "~S4"                       /*4. ϵͳ��Ƭ����   */
#define	Tag_System_TestPressure     "~S5"                       /*5. ϵͳ����ѹ��   */
#define	Tag_System_Description      "~S6"                       /*6. ϵͳ����   */

/*Define pipeline tag - ��������������*/
#define	Tag_PipeLine_SurfTreatPre   "~PA"                       /*1. ���߱���Ԥ���� */
#define	Tag_PipeLine_SurfTreatIn    "~PB"                       /*2. �����ڱ��洦�� */
#define	Tag_PipeLine_SurfTreatOut   "~PC"                       /*2. ��������洦�� */
#define	Tag_PipeLine_GasketMaterial "~PD"                       /*4. ���ߵ�Ƭ����   */
#define	Tag_PipeLine_TestPressure   "~PE"                       /*5. ��������ѹ��   */


/*Define spool tag - ����ܶ��������*/
#define	Tag_Spool_Name              "PSA"                          /*1.�ܶ�����*/
#define	Tag_Spool_PlanningUnit      "PS1"                          /*2.���̴���*/
#define	Tag_Spool_PrevTreatment     "PSB"                          /*3.�ܲ�Ԥ����*/
#define	Tag_Spool_SurfTreatmentIn   "PS3"                          /*4.�����ڱ��洦��*/
#define	Tag_Spool_SurfTreatmentOut  "PS4"                          /*5.��������洦��*/
#define	Tag_Spool_GasketMaterial    "PSG"                          /*6.���ӵ�Ƭ����*/
#define	Tag_Spool_Insulation        "PSC"                          /*7.���Ӿ�Ե��������*/
#define	Tag_Spool_Type              "PS7"                          /*8.�ܶ�����*/
#define	Tag_Spool_TestPressure      "PS5"                          /*9.�ܶ�����ѹ��*/
#define	Tag_Spool_Weight            "PS6"                          /*10.�ܶ�����*/
#define	Tag_Spool_Remark            "PSD"                          /*11.�ܶα�ע*/
#define	Tag_Spool_Page              "P56"                          /*12.СƱҳ��*/
#define	Tag_Spool_TotalPage         "P57"                          /*13.��ҳ��*/
#define	Tag_Spool_DrawingNumber     "P66"                          /*14.ͼ��*/

#define Tag_BlockNumber              ".m1"                         /*�ֶ�����*/
#define Tag_SupperBlock              ".m0"                         /*�ֶ�����*/

global int  Debug = 1;

/*
Input parameter
No.       Name           Type          Description
1.        object_handle  handle        handle of pipe spool
2.        tag_array      Array         array of pipe spool data tag 
3.        tag_number     int           number of tag
############����ֵ##########################################
���ղ���PDM_Tag_Array�е����Զ��巵�ض�Ӧ��ֵ   
############################################################    
*/
Get_Production_Data(object_handle, tag_array, tag_number)
{
    /*��ȡ�ܶ������е�pipe��standard component*/
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
	system_name = PM_GET_OBJDATA(part,0,MMT_TAG_SYSTEM_NAME);
	pipeline_name = PM_GET_OBJDATA(part,0,MMT_TAG_PIPELINE_NAME);	
	line_number_name = Get_Last_Substring(pipeline_name,"-");
	if (line_number_name == ""){
		line_number = -1;
	}
	else{
		result = SCAN_QUANT(0,line_number_name);
		if (ISFLOAT(result)){
			line_number = result;
		}
		else{
			line_number = -1;
		}
	}
	/*���屣��ܶ���Ϣ��һά����*/
	production_data = A_ALLOC(tag_number);
	/*�����ܶε����������鲢�������������ض�Ӧ��ֵ*/
	for (i = 0; i < tag_number; i = i + 1;){
	    /*��ȡ������*/
		att = A_GET(tag_array,i);
		/*������������ȡ��Ӧ������ֵ*/
		data = Get_Attribute_Value(object_handle,att);
		if (ISSTRING(data) & data != ""){
			A_PUT(production_data,i,data);
		}
		/*�����ȡ������Ӧ������ֵ���������߳�ȡ������Ե�Ĭ��ֵ*/
		else 
		{
			data = get_def_data(object_handle, att, system_name, line_number);
			A_PUT(production_data,i,data);
		}
	}
	return(production_data);
}

/*
**check if tag already exit
*/
Validate_Attribute(tag,tag_description)
{
	attr_h = PM_FIND_ATTRIBUTE(tag);
	if (attr_h == 0) {
		message = "����<" + tag_description + ">δ���塣";
		st = U_MESSAGE(message);
		return(-1);
	}
	return(0);
}

/*
#############����˵��FunctionDescription####################
��ȡָ���ܶζ����Ĭ��������Ϣ
Get PDM data of the indicated pipe spool
#############����˵��#######################################
���      ����           ����          ˵��
1.        object_handle  handle        ģ�Ͷ���ľ��handle of model
2.        PDM_Tag_Array  Array         ģ��������Ϣ��������array storing the PDM data of model
3.        PDM_Number     int           ������Ϣ����number of PDM data
############����ֵ##########################################
���ղ���PDM_Tag_Array�е����Զ��巵�ض�Ӧ��Ĭ��ֵ   
############################################################    
*/
get_def_production_data( object_handle, tag_array, tag_number)
{
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
	system_name = PM_GET_OBJDATA(part,0,MMT_TAG_SYSTEM_NAME);
	pipeline_name = PM_GET_OBJDATA(part,0,MMT_TAG_PIPELINE_NAME);	
	line_number_name = Get_Last_Substring(pipeline_name,"-");
	if (line_number_name == ""){
		line_number = -1;
	}
	else{
		result = SCAN_QUANT(0,line_number_name);
		if (ISFLOAT(result)){
			line_number = result;
		}
		else{
			line_number = -1;
		}
	}
	
	production_data = A_ALLOC(tag_number);
	for (i = 0; i < tag_number; i = i + 1;){
		att = A_GET(tag_array,i);
		data = get_def_data(object_handle, att, system_name, line_number);
		A_PUT(production_data,i,data);
	}
	return(production_data);
}

/*
**set pipe spool default production data, this is used for new pipe spool
*/
Set_Def_Production_Data( object_handle, tag_array, tag_number)
{
	access = CheckAccessRight(object_handle);
	production_data = get_def_production_data( object_handle, tag_array, tag_number);
	for (i = 0; i < tag_number; i = i + 1){
		tag = A_GET(tag_array,i);
		if(access){
			attribute_value = A_GET(production_data, i);
			Set_Attribute_Value( object_handle, tag, attribute_value);
		}
	}	
	return(0);
}

get_def_data(handle object_handle, string tag, string system_name, int line_number)
{	
	access = CheckAccessRight(object_handle);
	if ( tag == Tag_Spool_Name ){
		data = get_def_spool_name(object_handle);
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}
	else if ( tag == Tag_BlockNumber ){
		data = get_def_block_number(object_handle);
		return(data);	    
	}	
	else if ( tag == Tag_Spool_PlanningUnit ){
		data = get_def_planning_unit(object_handle);
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}	
	else if ( tag == Tag_Spool_SurfTreatmentIn  ){
		data = get_def_tube_inside_surface_treatment(object_handle);
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}	
	else if ( tag == Tag_Spool_SurfTreatmentOut  ){
		data = get_def_tube_outside_surface_treatment(object_handle);
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}
	else if ( tag == Tag_Spool_TestPressure ){
		data = get_def_pressure_test(object_handle);
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}		
	else if ( tag == Tag_Spool_Type ){
		data = get_def_spool_type(object_handle);
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}
	else if ( tag == Tag_Spool_PrevTreatment ){
		data = get_tube_surface_pre_treatment(object_handle);
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}	
	else if ( tag == Tag_Spool_GasketMaterial ){
		data = get_def_spool_gasket(object_handle);
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}
	else if ( tag == Tag_System_Description ){
		data = Get_Def_Spool_System_Name(object_handle);
		return(data);
	}
	else if ( tag == Tag_Spool_Weight ){
		data = FTOASCII(Get_Pipe_Spool_Weight(object_handle));
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}
	else if ( tag == Tag_Spool_Page ){
		data = "0";
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}
	else if ( tag == Tag_Spool_TotalPage ){
		data = "0";
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}
	else if ( tag == Tag_Spool_DrawingNumber ){
		data = "DrawingNo";
		if(access){
			Set_Attribute_Value(object_handle, tag, data);
		}
		return(data);
	}
}

get_def_spool_gasket(handle object_handle)
{
    /*��ȡ�ܶΰ���������pipe����*/
	pipes = PM_GET_OBJECTS_IN_GROUP(object_handle);
	/*��ȡ�ܶ��������ߵĵ�Ƭ����*/
	pipe0 = PM_GET_MEMBER_IN_SET(pipes,0);
	gasket_material = PM_GET_OBJDATA(pipe0,0,Tag_PipeLine_GasketMaterial);
	if(ISSTRING(gasket_material)){
	    return(gasket_material);
	}
	/*��ȡ�ܶ�����ϵͳ�ĵ�Ƭ����*/
	gasket_material = PM_GET_OBJDATA(pipe0,0,Tag_System_GasketMaterial);
	if(ISSTRING(gasket_material)){
	    return(gasket_material);
	}	
	/* �����ܶ����е����ӵ㣬����������κη��������棬�򷵻ؿ��ַ��� */
	pipe_number = PM_NR_MEMBERS_IN_SET(pipes);
	/* �����ܶ���������pipe���� */
	for(i=0;i<pipe_number;i=i+1;){
	    pipe = PM_GET_MEMBER_IN_SET(pipes,i);
	    /* ��ȡpipe��������ӵ����� */
	    con_number = PM_GET_NR_OF_CONPNTS(pipe);
	    /* ����pipe�����ӵ㣬��ȡ���ӵ�ĵ�Ƭ��Ϣ */
	    for(k=0;k<con_number;k=k+1;){
            fc1 = PM_GET_CONPNT_DATA(pipe,k,"fc1");
            if(ISSTRING(fc1)){
                check_string = HEAD(fc1,1);
                if(check_string == "1" | check_string == "3" | check_string == "G" | check_string == "J" | check_string == "K" | check_string == "L" | check_string == "M" | check_string == "O"){
                    connection_code1 = fc1;
                    connection_type1 = PM_GET_CONPNT_DATA(pipe,k,"ft1");
                    connection_code2 = PM_GET_CONPNT_DATA(pipe,k,"fc2");
                    connection_type2 = PM_GET_CONPNT_DATA(pipe,k,"ft2");
                    /*���������ӵ�û�����ӣ�����������ӵ�һ���������͸�����ͬ�����ӵ���*/
                    if(!ISSTRING(connection_code2)){
                        connection_code2 = connection_code1;
                        connection_type2 = connection_type1;
                    }
                    NS = PM_GET_CONPNT_DATA(pipe,k,"NS");
                    if(ISSTRING(NS)){
                        ns = String_To_Int(NS);  
                        pipeline = PM_GET_OBJDATA(pipe,0,MMT_TAG_PIPELINE_NAME);
                        if(ISSTRING(pipeline)){  
                            spec_n = PM_GET_OBJDATA(pipe,0,MMT_COSE_SPEC_NUMBER);
                            if(ISSTRING(spec_n)){
                                spec_nr = String_To_Int(spec_n);
                            }
                            else{
                                spec_nr = 1;
                            }
                            PM_ACT_PIPELINE(pipeline, spec_nr);
                            spec_name = PM_GET_OBJDATA(pipe,0,MMT_TAG_SPECNAME);
                            gasket_pid = PM_PID_FROMSPEC("g" + connection_code1 + connection_code2, ns, 0);
                            /*����ҵ���Ӧ�ĵ�Ƭ���ID���򷵻����������*/
                            if(ISSTRING(gasket_pid)){
                                gasket_material = DM_PARTID_DATA(gasket_pid,"MC");  
                                if(ISSTRING(gasket_material)){
                                    return(gasket_material);
                                }     
                            }                    
                        }                       
                    }
                }
            }
	    }
	}
	return(0);
}

Get_Def_Spool_System_Name(handle object_handle)
{
    /* ��ȡ�ܶ����������ϵͳ���� */
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
    system_description = PM_GET_OBJDATA(part,0,Tag_System_Description);
	if(ISSTRING(system_description)){
	   return(system_description);
	} 
	return("δ����");
}

get_def_block_number(handle object_handle)
{
    /*��ȡ�ܶ������ķֶ����ƣ����û�����ã���Ĭ��ΪUnset*/
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
    block = PM_GET_OBJDATA(part,0,Tag_BlockNumber);
	if(ISINT(block)){
		block = PM_GET_OBJDATA(part,0,Tag_SupperBlock);
		if(ISINT(block)){
			return("999");
		}
		
	} 
	return(block);
}

get_def_planning_unit(handle object_handle)
{
    /*��ȡ�ܶ��������������ƣ����û�����ã���Ĭ��ΪUnset*/
	/*SDARI - Ҫ��Ĭ�ϵ��������Ƹ��ֶ�����һ�� */
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
    /*planning_unit = PM_GET_OBJDATA(part,0,Tag_Spool_PlanningUnit);*/
	planning_unit = PM_GET_OBJDATA(part,0,Tag_BlockNumber);
	if(ISINT(planning_unit)){
	   return("999");
	} 
	return(planning_unit);
}

get_def_tube_inside_surface_treatment_rainbow(handle object_handle)
{	
   /*��ȡ�ܶ�����ϵͳ�ı��洦���壬���û�����ã���Ĭ��Ϊ��п*/
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
	surface_treatment_code = PM_GET_OBJDATA(part,0,Tag_PipeLine_SurfTreatIn);
	if(surface_treatment_code == 0){
	   return("��п");
	} 
	return(surface_treatment_code);
}

get_tube_surface_pre_treatment(handle object_handle)
{	
   /*��ȡ�ܶ������Ĺ��ߵ�Ԥ�����壬���û�����ã���Ĭ��Ϊ��ϴ*/
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
	surface_treatment_code = PM_GET_OBJDATA(part,0,Tag_PipeLine_SurfTreatPre);
	if(surface_treatment_code == 0){
	   return("��ϴ");
	} 
	return(surface_treatment_code);
}

get_def_tube_inside_surface_treatment(handle object_handle)
{
   /*��ȡ�ܶ��������ߵı��洦���壬���û�����ã��������ȡ�ܶ�����ϵͳ�ı��洦����*/
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
	surface_treatment = PM_GET_OBJDATA(part,0,Tag_PipeLine_SurfTreatIn);
	if(ISSTRING(surface_treatment)){
	   return(surface_treatment);
	}
		
   /*��ȡ�ܶ�����ϵͳ�ı��洦���壬���û�����ã���Ĭ��Ϊ��п*/
	surface_treatment = PM_GET_OBJDATA(part,0,Tag_System_SurfTreatIn);
	if(ISSTRING(surface_treatment)){
	   return(surface_treatment);
	} 
	else{
	    return("��п");
	}
}

get_def_tube_outside_surface_treatment(handle object_handle)
{
   /*��ȡ�ܶ��������ߵı��洦���壬���û�����ã��������ȡ�ܶ�����ϵͳ�ı��洦����*/
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
	surface_treatment = PM_GET_OBJDATA(part,0,Tag_PipeLine_SurfTreatOut);
	if(ISSTRING(surface_treatment)){
	   return(surface_treatment);
	}
		
   /*��ȡ�ܶ�����ϵͳ�ı��洦���壬���û�����ã���Ĭ��Ϊ��п*/
	surface_treatment = PM_GET_OBJDATA(part,0,Tag_System_SurfTreatOut);
	if(ISSTRING(surface_treatment)){
	   return(surface_treatment);
	} 
	else{
	    return("��п");
	}
}

get_def_pressure_test(handle object_handle)
{
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
	/*��ȡ�ܶ��������ߵ�����ѹ�������û�����ã��������ȡ�ܶ�������ϵͳ������ѹ��*/
	pressure = PM_GET_OBJDATA(part,0,Tag_PipeLine_TestPressure);
	if(ISSTRING(pressure)){	    
	    return(pressure);
	}
	
	/*��ȡ�ܶ�����ϵͳ������ѹ�������û�����ã���Ĭ��Ϊ0*/
	pressure = PM_GET_OBJDATA(part,0,Tag_System_TestPressure);
	if(ISSTRING(pressure)){
	   return(pressure);
	}
	else{
	    return("0");
	}
}

get_def_spool_type(handle object_handle)
{
	return("PRE");
}

get_def_spool_name(handle object_handle)
{	
	spool_number = PM_GET_OBJDATA(object_handle, 0, MMT_COSA_SPOOL_NAME);
	parts = PM_GET_OBJECTS_IN_GROUP(object_handle);
	part_number = PM_NR_MEMBERS_IN_SET(parts);
	find = 0;
	sequence = 0;
	while(!find){
		part = PM_GET_MEMBER_IN_SET(parts,sequence);
		part_type = PM_GET_OBJDATA(part,0,MMT_TAG_OBJTYPE);
		if(part_type == "2" | part_type == "3"){
			find=1;
		}
		sequence = sequence + 1;
	}
	
	/*��������������޷���ȡ��������ͬϵͳ������*/
	line_name = PM_GET_OBJDATA(part, 0, MMT_TAG_PIPELINE_NAME);
	if(ISSTRING(line_name)){
	    pipe_line = line_name;
	}
	else{
	    pipe_line = "Undefined";
	}
	block_name = PM_GET_OBJDATA(part, 0, Tag_BlockNumber);
	if(ISSTRING(block_name)){
	    block = block_name;
	}
	else{
		block_name = PM_GET_OBJDATA(part, 0, Tag_SupperBlock);
		if(ISSTRING(block_name)){
			 block = block_name;
		}
		else{
			block = "999";
		}	    
	}	
	spool_name = block + "-" + pipe_line + "-" + spool_number;
	return(spool_name);
}

/*��ȡ�ܶ�����*/
Get_Pipe_Spool_Weight(spool)
{
   pipes = PM_GET_OBJECTS_IN_GROUP(spool);
   pipe_number = PM_NR_MEMBERS_IN_SET(pipes);
   total_weight = 0.0;
   for(i=0;i<pipe_number;i=i+1;){
      pipe = PM_GET_MEMBER_IN_SET(pipes,i);
      part_number = PM_NR_PARTS_IN_PIPE(pipe);
      if(part_number != -1){
         for(nth=1;nth<=part_number;nth=nth +1;){            
            weight_s = PM_GET_OBJDATA(pipe,nth,MMT_TAG_MASS);
            weight = String_To_Float(weight_s);
            total_weight = total_weight + weight;
         }
      }
      /*��׼��*/
      else{
         weight_s = PM_GET_OBJDATA(pipe,0,MMT_TAG_MASS);
         weight = String_To_Float(weight_s);
         total_weight = total_weight + weight;         
      }
   }
   return(total_weight);
}
