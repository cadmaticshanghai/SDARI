/*Define penetration tag - 定义贯通相关属性*/
#define	Tag_Penetration_FabDn			"FB1"							/*2.复板型号规格及标准号-制作图号及页码*/
#define	Tag_Penetration_WeldType		"FB2"							/*3.复板焊接类型*/
#define	Tag_Penetration_Remark      	"FB3"							/*4.复板备注信息*/
#define	Tag_Penetration_Name        	"FB4"							/*5.复板名称*/
#define	Tag_Penetration_PlanningUnit	"PS1"							/*6.复板所属托盘*/

#define	Tag_BlockNumber					".m1"							    /*分段名称*/
#define	Tag_SupperBlock				    ".m0"							    /*总段名称*/

#define	Maximum_Thickness			20

#define	Tag_Spool_Name              "PSA"                          /*1.管段名称*/
#define	Tag_Spool_PlanningUnit      "PS1"                          /*2.托盘代号*/


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
    return(planning_unit);
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