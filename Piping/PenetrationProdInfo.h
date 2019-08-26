/*Define penetration tag - �����ͨ�������*/
#define	Tag_Penetration_FabDn			"FB1"							/*2.�����ͺŹ�񼰱�׼��-����ͼ�ż�ҳ��*/
#define	Tag_Penetration_WeldType		"FB2"							/*3.���庸������*/
#define	Tag_Penetration_Remark      	"FB3"							/*4.���屸ע��Ϣ*/
#define	Tag_Penetration_Name        	"FB4"							/*5.��������*/
#define	Tag_Penetration_PlanningUnit	"PS1"							/*6.������������*/

#define	Tag_BlockNumber					".m1"							    /*�ֶ�����*/
#define	Tag_SupperBlock				    ".m0"							    /*�ܶ�����*/

#define	Maximum_Thickness			20

#define	Tag_Spool_Name              "PSA"                          /*1.�ܶ�����*/
#define	Tag_Spool_PlanningUnit      "PS1"                          /*2.���̴���*/


/* ���� */
get_planning_unit_from_part(part_handle)
{
    planning_unit = "Undefined";
    
    /* ȡ��¼�����̺� */
    value = PM_GET_OBJDATA(part_handle,0, Tag_Spool_PlanningUnit);
    if(ISSTRING(value)){
        if(value!=""){
            planning_unit = value;
        }
    }
    if(planning_unit=="Undefined"){
        /* ����зֶΣ�Ĭ������Ϊ�ֶ��� */
        planning_unit = PM_GET_OBJDATA(part_handle,0, Tag_BlockNumber);
        if(ISINT(planning_unit)){
            planning_unit = PM_GET_OBJDATA(part_handle,0, Tag_SupperBlock);
            if(ISINT(planning_unit)){
                planning_unit = "999";
            }
        }
    }
    return(planning_unit);
}

/* �ܼ��� */
/* ͨ������ƴ���ܶκ� */
/* ���̺�-���ߺ�-��� */
get_spool_name_from_part(part_handle)
{
    spool_name = "Undefined";
    
    /* ȡ��¼�Ĺܶκ� */
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
            /* U_CONFIRM("��ѡ����������κιܶ�"); */
            spool_name = pipe_line;
        }
        else{
            spool_name = planning_unit + "-" + pipe_line + "-" + spool_number;
        }
    }
    /* U_MESSAGE("spool_name="+spool_name); */
    return(spool_name);
}