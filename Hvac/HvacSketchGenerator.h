/*define column number of list window*/
#define  Column_Number 7

/*define tag for hvac spool*/
#define  Group_Type     "20"
#define  SketchName     "U50"
#define  SketchStatus   "U39"
#define  CreationTime   ".c6"
#define  DocName        ".ba"
#define  Spool_Name     "~GA"
#define  Planning_Unit  "~G2"
#define  Weight         "mas"
#define  TotalPage      "~D1"
#define  PageNumber     "~D2"

/* Duct shape type*/
#define  Duct_Shape                          ".mn"
#define  Rectangular_Bend                    1
#define  Rectangular_Asymmetric_Bend         2
#define  Rectangular_Transition              3
#define  Rectangular_T_branch                4
#define  Rectangular_To_Round                5
#define  Lateral_Radius_Y                    6
#define  Rectangular_Frame                   7
#define  Unequal_Circular_Radius_Lateral_Y   8
#define  Rectangular_End_Cap                 9
#define  Rounded_Rectangular_Boot_Outlet     10

/*defnie view position on sketch*/
global int  View1_X = 65, View1_Y = 200;
global int 	View2_X = 155,View2_Y = 200;
global int 	View3_X = 65, View3_Y = 110;
global int 	View4_X = 155,View4_Y = 110;

/*define view size and default scale*/
global int 	 View_Size = 85;
global float View_Scale = 50;

global handle View_Info,View_Handles,View_Names,View_Positions;


/*define view direction of ISO view*/
global float AxoRotAngle=-120.0;
global float AxoSlopeAngle=30.0;

/*管段part列表及part数量*/
global int Spool_Part_Number = 0;
global handle Spool_Parts;

/*define maximum part number of have spool*/
global int Max_Spool_Parts_Number = 50;


/*define array to save dimension information*/
global handle Dim_Table;
global int    Dim_Number = 0;

/*define array to save mto information of spool */
global handle Mto_Table;
global handle Mto_Label_Table;
global int Mto_Number = 0;

/*setting of mto table on sketch*/
global int Mto_Table_Orign_X = 20;
global int Mto_Table_Orign_Y = 280;
global int Mto_Table_W = 185;
global int Mto_Table_H = 6;
global int Mto_Table_Column1_W = 10;
global int Mto_Table_Column2_W = 95;
global int Mto_Table_Column3_W = 30;
global int Mto_Table_Column4_W = 20;
global int Mto_Table_Column5_W = 15;
global int Mto_Table_Column6_W = 15;
global int Mto_Table_Line_Type = 0;
global int Mto_Table_Line_Colour = 1;
global int Mto_Table_Pattern_Length = 0;
global int Mto_Table_Text_Colour = 2;
global int Mto_Table_Text_Height = 2.5;

/*setting of label on sketch*/
global int Mto_Label_Text_Colour = 1;
global int Mto_Label_Text_Height = 2.5;
global int Mto_Label_Leader_Line_Colour = 2;
global int Mto_Label_Leader_Line_Type = 0;
global int Mto_Label_Leader_Line_PattenLength = 0;
global int Mto_Label_Circle_Radius = 2;

/*setting of connection table on sketch*/
global int Connection_Table_Orign_X = 115;
global int Connection_Table_Orign_Y = 52;
global int Connection_Table_W = 90;
global int Connection_Table_H = 4;
global int Connection_Table_Column1_W = 10;
global int Connection_Table_Column2_W = 15;
global int Connection_Table_Column3_W = 15;
global int Connection_Table_Column4_W = 15;
global int Connection_Table_Column5_W = 35;
global string Connection_Table_Direction = "Up"; /* Down - 从上向下 Up - 从下向上*/
global int Connection_Table_Line_Type = 0;
global int Connection_Table_Line_Colour = 1;
global int Connection_Table_Pattern_Length = 0;
global int Connection_Table_Text_Colour = 2;
global int Connection_Table_Text_Height = 2.5;

/*保存管段连接表标注信息的全局数组*/
global handle Connection_Table;
global handle Connection_Label_Table;
global int Connection_Label_Number = 2;
global int Max_Spool_Connection_Points = 10;

/*管端连接号标注设置*/
global int Connection_Label_Text_Colour = 1;
global int Connection_Label_Text_Height = 2.5;
global int Connection_Label_Leader_Line_Colour = 2;
global int Connection_Label_Leader_Line_Type = 0;
global int Connection_Label_Leader_Line_PattenLength = 0;
global int Connection_Label_Circle_Radius = 2;

/*保存所有标注信息的全局数组*/
global handle Label_Table;
global int Label_Number = 0;

/*字体设置*/
global string General_Font = "Arial Unicode MS";

/*尺寸标注设置*/
global int     DimLinePen = 1;
global string  DimLineWidthName = "0.25";
global int     DimLineColor = 1;
global string  DimLineColorName = "red";
global int     DimLineLayer = 104;
global string  DimLineLayerName = "PM_dim_layer";
global float   DimTextHeight = 2.5;
global float   DimTextWidth = 1.5;
global float   DimTextSpc = 0.5;
global float   DimSymHeight = 2.0;

/* 获取管段的名称
**输入参数
**参数名       类型     说明
**ModelObject  Handle   Spool类型的ModelGroup的handle
**
**返回值
**返回管段名称的字符串，失败则返回整数-1;
*/
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
	/*
	spool_name = area_name + "-" + line_name + "-" + spool_number;
	*/
	return(spool_name);
}

node_id_to_part_number(handle obj_handle, int node_id)
{
	obt = PM_GET_OBJDATA(obj_handle, 0, MMT_TAG_OBJTYPE);
	if(obt != "3") return(0);

	if(node_id == 1) return(1);
	if(node_id == 2) return(PM_NR_PARTS_IN_PIPE(obj_handle));

	distance = 0.0;
	nr_of_part = PM_FIND_BRANCH_IN_PIPE(obj_handle, node_id, distance);
	if(nr_of_part > 0) return(nr_of_part);

	return(-1);
}