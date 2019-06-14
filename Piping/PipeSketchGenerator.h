/*定义主窗口的列数*/
#define  Column_Number 4

/*管段信息对应的TAG定义*/

#define  Tag_Sketch_Name   "PS2"        /* 管段小票图形名称 */
#define  SketchStatus      "U39"        /* 管段工艺状态 - 暂时没有使用 */
#define  CreationTime      ".c6"        /* 管段小票图形生成时间 - 系统默认的属性 */

/* define drawing tag name */ 
#define	Tag_Drawing_ShipName         "~D0"                          /* 船名 */
#define	Tag_Drawing_ShipNumber       "~D1"                          /* 船号 */
#define	Tag_Drawing_DrawingNumber    "~D2"                          /* 图号 */
#define	Tag_Drawing_PageNumber       "~D3"                          /* 页码 */
#define	Tag_Drawing_TotalPage        "~D4"                          /* 总页数 */
#define	Tag_Drawing_PipePreTre       "~D5"                          /* 管子预处理 */
#define	Tag_Drawing_OutSurfTreat     "~D6"                          /* 管子外表面处理 */
#define	Tag_Drawing_InSurfTreat      "~DH"                          /* 管子内表面处理 */
#define	Tag_Drawing_GasketMat        "~D7"                          /* 垫片材料 */
#define	Tag_Drawing_Insulation       "~D8"                          /* 绝缘材料 */
#define	Tag_Drawing_PipeMaterial     "~D9"                          /* 管材信息 */
#define	Tag_Drawing_Note             "~DA"                          /* 注解 */
#define	Tag_Drawing_SystemName       "~DB"                          /* 系统名称 */
#define	Tag_Drawing_TestPress        "~DC"                          /* 试验压力 */
#define	Tag_Drawing_BendingRadiu     "~DD"                          /* 弯模半径 */
#define	Tag_Drawing_SpoolWeight      "~DE"                          /* 管段重量 */
#define	Tag_Drawing_PlanningUnit     "~DF"                          /* 托盘代号 */
#define	Tag_Drawing_SpoolName        "~DG"                          /* 管号 */

/* 定义项目信息对应的TAG定义 */
#define Tag_Project_Description      ".dG"                          /* 项目描述 - 项目名称，系统默认属性 */
#define Tag_Project_Number           "U32"                          /* 项目编号 */



global handle Current_Spool_Handle, Cur_Spool_Model_Set;

/*定义管段视图位置*/
global int  View1_X = 70, View1_Y = 200;
global int 	View2_X = 160,View2_Y = 200;
global int 	View3_X = 70, View3_Y = 130;
global int 	View4_X = 160,View4_Y = 130;

/*定义管段视图大小*/
global int 	View_Size = 80;
global handle View_Info,View_Handles,View_Names,View_Positions;

/**/
global float View_Scale = 50;

/*管段最多的零件个数*/
global int Max_Spool_Parts_Number = 100;

/*管段最多的支管个数*/
global int Max_Spool_Branch_Number = 20;

/*当前管段的分支数量*/
global int Spool_Branch_Number = 0;

/*保存管段各个支管的pipe的全局数组*/
global handle Spool_Branch;
global handle Spool_Branch_Direction;

/*保存管段各个分支零件个数的全局数组*/
global handle Spool_Branch_Part_Number;

/*三维视图的方向*/
global float AxoRotAngle=-120.0;
global float AxoSlopeAngle=30.0;

/*管段主管的pipe个数*/
global int Main_Branch_Pipe_Number = 1;

/*管段part列表及part数量*/
global int Spool_Part_Number = 1;
global handle Spool_Parts;

/*定义保存尺寸标注信息的全局数组*/
global handle Dim_Table;
global int Dim_Number = 0;

/*保存管段零件表内容的全局数组*/
global handle Mto_Table;
global handle Mto_Label_Table;
global int Mto_Number = 0;

/*零件表相关设置*/
global int Mto_Table_Orign_X = 20;
global int Mto_Table_Orign_Y = 285;
global int Mto_Table_W = 150;
global int Mto_Table_H = 4;
global int Mto_Table_Column1_W = 6;
global int Mto_Table_Column2_W = 12;
global int Mto_Table_Column3_W = 10;
global int Mto_Table_Column4_W = 110;
global int Mto_Table_Column5_W = 12;
global string Mto_Table_Direction = "Down"; /* Down - 从上向下 Up - 从下向上*/
global int Mto_Table_Line_Type = 0;
global int Mto_Table_Line_Colour = 1;
global int Mto_Table_Pattern_Length = 0;
global int Mto_Table_Text_Colour = 2;
global int Mto_Table_Text_Height = 2.5;

/*零件号标注设置*/
global int Mto_Label_Text_Colour = 1;
global int Mto_Label_Text_Height = 2.5;
global int Mto_Label_Leader_Line_Colour = 2;
global int Mto_Label_Leader_Line_Type = 0;
global int Mto_Label_Leader_Line_PattenLength = 0;
global int Mto_Label_Circle_Radius = 2;

/*保存管段校管表内容的全局数组*/
global handle Check_Node_Table;
global int Check_Node_Number = 0;

/*校管表相关设置*/
global int Check_Table_Orign_X = 170;
global int Check_Table_Orign_Y = 285;
global int Check_Table_W = 35;
global int Check_Table_H = 4;
global int Check_Table_Column1_W = 8;
global int Check_Table_Column2_W = 9;
global int Check_Table_Column3_W = 9;
global int Check_Table_Column4_W = 9;
global string Check_Table_Direction = "Down"; /* Down - 从上向下 Up - 从下向上*/
global int Check_Table_Line_Type = 0;
global int Check_Table_Line_Colour = 1;
global int Check_Table_Pattern_Length = 0;
global int Check_Table_Text_Colour = 2;
global int Check_Table_Text_Height = 2.5;

/*管端连接表相关设置*/
global int Connection_Table_Orign_X = 115;
global int Connection_Table_Orign_Y = 34;
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
global int Connection_Label_Number = 0;

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


/*弯管相关信息*/
global int Bent_Piece_Number = 0;
global handle Bending_Table_Head;
global handle Bending_Action_Number;
global handle Bending_Actions;

/*弯管表相关设置*/
global int Bending_Table_Orign_X = 144;
global int Bending_Table_Orign_Y = 52;
global int Bending_Table_W = 61;
global int Bending_Table_H = 4;
global int Bending_Table_Column1_W = 20;
global int Bending_Table_Column2_W = 21;
global int Bending_Table_Column3_W = 20;
global string Bending_Table_Direction = "Up"; /* Down - 从上向下 Up - 从下向上*/
global int Bending_Table_Line_Type = 0;
global int Bending_Table_Line_Colour = 1;
global int Bending_Table_Pattern_Length = 0;
global int Bending_Table_Text_Colour = 2;
global int Bending_Table_Text_Height = 2.5;

/*保存管段装配表内容的全局数组*/
global handle Assembly_Table;
global handle Assembly_Label_Table;
global int Assembly_Number = 0;
global int Assembly_Label_Number = 0;

/*装配表相关设置*/
global int Assembly_Table_Orign_X = 20;
global int Assembly_Table_Orign_Y = 34;
global int Assembly_Table_W = 95;
global int Assembly_Table_H = 4;
global int Assembly_Table_Column1_W = 12;
global int Assembly_Table_Column2_W = 14;
global int Assembly_Table_Column3_W = 14;
global int Assembly_Table_Column4_W = 14;
global int Assembly_Table_Column5_W = 41;
global string Assembly_Table_Direction = "Up"; /* Down - 从上向下 Up - 从下向上*/
global int Assembly_Table_Line_Type = 0;
global int Assembly_Table_Line_Colour = 1;
global int Assembly_Table_Pattern_Length = 0;
global int Assembly_Table_Text_Colour = 2;
global int Assembly_Table_Text_Height = 2.5;

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

/*定义尺寸标注模式 1 - 按点间长标注 0 - 按投影长度标注 */
#define	Dimension_Mode 0


/*根据零件是sorting code返回零件类型字符串*/
/*IHC在定义零件库的dimension table时，没有按照手册中的规定来定义零件的sort code
--零件类型的判断首先按照零件的key word进行归类，
--如果零件的key word没有定义，则查找零件的sort code
--如果零件的key word及sort code均没有定义，则将其归类为OTHER,在命令行中提示相关信息
*/

Get_Component_Type(part_handle)
{
	part_id = PM_GET_OBJDATA(part_handle,1, "pid");   
	sort_code = DM_PARTID_DATA(part_id, "sor");   
	key_word = DM_PARTID_DATA(part_id,"KW");
	gt = DM_PARTID_DATA(part_id,"GT");
	if(Debug){
		U_MESSAGE("Keyword-" + key_word);
	}

	/* 1. 先通过Geometry type判断零件类型*/
	if(gt == "1"){
		mth = PM_GET_OBJDATA(part_handle, 1, MMT_TAG_METHOD);
		if(ISSTRING(mth)){
			if(mth == "$B"){
				return("BEND"); 
			}                         
		} 
		else{ 
			return("TUBE");
		}		
	}
	else if(gt == "2"){
		return("ELBOW");
	}
	else if(gt == "6" | gt == "11" | gt == "12"){
		if(PATTERN_MATCH(key_word, "*[Ee][Ll][Bb][Oo][Ww]*")){		
			return("ELBOW");
		}	
	}
	else if(gt == "7"){
		if(PATTERN_MATCH(key_word, "*[Tt][Ee][Ee]*")){		
			return("TEE");
		}	
	}
	else if(gt == "8"){		
		return("CROSS");	
	}
	else{	
		/* 2. Geometry type无法区分零件类型，则通过keyword做进一步的判断 */
		if(key_word != ""){
			if(PATTERN_MATCH(key_word, "*[Pp][Ii][Pp][Ee]*") | PATTERN_MATCH(key_word, "*[Tt][Uu][Bb][Ee]*")){
				mth = PM_GET_OBJDATA(part_handle, 1, MMT_TAG_METHOD);
				if(ISSTRING(mth)){
					if(mth == "$B"){
						return("BEND"); 
					}                         
				} 
				else{ 
					return("TUBE");
				}
			}
			else if(PATTERN_MATCH(key_word, "*[Ee][Ll][Bb][Oo][Ww]*") | PATTERN_MATCH(key_word, "*[Bb][Ee][Nn][Dd]*")){
				return("ELBOW");     
			}
			else if(PATTERN_MATCH(key_word, "*[Ff][Ll][Aa][Nn][Gg][Ee]*") & 
				 !PATTERN_MATCH(key_word, "*[Bb][Ll][Ii][Nn][Dd]*") & 
				 !PATTERN_MATCH(key_word, "*[Bb][Uu][Ll][Kk][Hh][Ee][Aa][Dd]*") &
				 !PATTERN_MATCH(key_word, "*[Pp][Ee][Nn][Ee][Tt][Rr][Aa][Tt][Ii][Oo][Nn]*")){
					return("FLANGE");
			}
			else if(PATTERN_MATCH(key_word, "*[Ff][Ll][Aa][Nn][Gg][Ee]*") & 
				  PATTERN_MATCH(key_word, "*[Bb][Ll][Ii][Nn][Dd]*")){
				return("BLIND FLANGE");
			}
			else if(PATTERN_MATCH(key_word, "*[Pp][Ee][Nn][Ee][Tt][Rr][Aa][Tt][Ii][Oo][Nn]*")){
				return("PENETRATION");
			} 
			else if(PATTERN_MATCH(key_word, "*[Ff][Ll][Aa][Nn][Gg][Ee]*") & 
				  PATTERN_MATCH(key_word, "*[Bb][Ll][Ii][Nn][Dd]*")){ 
				return("PENETRATION");
			}   
			else if(PATTERN_MATCH(key_word, "*[Tt][Ee][Ee]*")){
				return("TEE");
			} 
			else if(PATTERN_MATCH(key_word, "*[Bb][Rr][Aa][Nn][Cc][Hh]*")){
				return("TEE");
			}
			else if(PATTERN_MATCH(key_word, "*[Bb][Oo][Ll][Tt]*")){
				return("BOLT");
			}
			else if(PATTERN_MATCH(key_word, "*[Uu][Nn][Ii][Oo][Nn]*")){
				return("UNION");
			}
			else if(PATTERN_MATCH(key_word, "*[Cc][Ll][Aa][Mm][Pp]*")){
				return("CLAMP");
			}
			else if(PATTERN_MATCH(key_word, "*[Nn][Ii][Pp][Pp][Ll][Ee]*")){
				return("NIPPLE");
			}
			else if(PATTERN_MATCH(key_word, "*[Cc][Oo][Uu][Pp][Ll][Ii][Nn][Gg]*") | 
				  PATTERN_MATCH(key_word, "*[Cc][Oo][Uu][Pp][Ll][Ee][Rr]*") |
				  PATTERN_MATCH(key_word, "*[Cc][Ll][Rr][Ii][Nn][Gg]*")){
				return("COUPLING");
			}
			else if(PATTERN_MATCH(key_word, "*[Ss][Cc][Uu][Pp][Pp][Pp][Ee][Rr]*")){
				return("SCUPPER");
			}
			else if(PATTERN_MATCH(key_word, "*[Rr][Ee][Dd][Uu][Cc][Ee][Rr]*")){
				return("REDUCER");
			}     
			else if(PATTERN_MATCH(key_word, "*[Ss][Tt][Rr][Aa][Ii][Nn][Ee][Rr]*")){
				return("STRAINER");
			}
			else if(PATTERN_MATCH(key_word, "*[Ss][Uu][Cc][Tt][Ii][Oo][Nn]*")){
				return("SUCTION");
			}  
			else if(PATTERN_MATCH(key_word, "*[Oo][Rr][Ii][Ff][Ii][Cc][Ee]*")){
				return("ORIFICE");     
			} 
			else if(PATTERN_MATCH(key_word, "*[Gg][Uu][Aa][Gg][Ee]*")){
				return("GAUGE");      
			}
			else if(PATTERN_MATCH(key_word, "*[Nn][Ii][Pp][Pp][Ll][Ee]*")){
				return("NIPPLE");
			}
			else if(PATTERN_MATCH(key_word, "*[Ss][Ii][Gg][Hh][Tt]*")){
				return("SIGHT GLASS");
			}    
			else if(PATTERN_MATCH(key_word, "*[Ss][Ll][Ee][Ee][Vv][Ee]*")){
				return("SLEEVE");      
			}
			else if(PATTERN_MATCH(key_word, "*[Vv][Aa][Ll][Vv][Ee]*")){
				return("VALVE");
			}
			else if(PATTERN_MATCH(key_word, "*[Ff][Ii][Ll][Tt][Ee][Rr]*")){
				return("FILTER");
			} 
			else{
				/* 3. 如果还无法区分零件类型，则通过零件的sort code进行判断*/
				if(sort_code !=""){
					if(ISSTRING(sort_code)){
						sort = String_To_Int(sort_code);
						if(sort>=10 & sort<=18){
							mth = PM_GET_OBJDATA(part_handle, 1, MMT_TAG_METHOD);
							if(ISSTRING(mth)){
								if(mth == "$B"){
									return("BEND"); 
								}                         
							} 
							else{
								return("TUBE");
							}         
						}
						else if(sort>=20 & sort<=28){
							return("ELBOW");
						}
						else if(sort>=30 & sort<=48){
							return("REDUCER");
						}
						else if(sort>=50 & sort<=58){
							return("TEE");
						}
						else if(sort==60){
							return("CLAMP");
						}
						else if(sort>=70 & sort<=78){
							return("CAP");
						}
						else if(sort>=80 & sort<=88){
							return("CROSS");
						}
						else if(sort>=90 & sort<=98){
							return("T-DRILL");
						}
						else if(sort>=100 & sort<=138){
							return("FLANGE");
						}
						else if(sort>=190 & sort<=198){
							return("SLEEVE");
						}
						else if(sort>=250 & sort<=258){
							return("PENETRATION");
						}
						else if(sort>=300 & sort<=308){
							return("CAP");
						}
						else if(sort>=400 & sort<=404){
							return("PLUG");
						}
						else if(sort>=410 & sort<=414){
							return("SOCKET");
						}
						else if(sort>=410 & sort<=424){
							return("SOCKET");
						}
						else if(sort>=430 & sort<=444){
							return("NIPPLE");
						}
						else if(sort>=450 & sort<=464){
							return("UNION");
						}
						else if(sort>=470 & sort<=474){
							return("ELBOW");
						}
						else if(sort>=480 & sort<=484){
							return("TEE");
						}
						else if(sort>=490 & sort<=494){
							return("CROSS");
						}
						else{
							return("OTHER");
						}
					}
				}
			}
		}
	}
}

/* 获取管段的名称 
**输入参数
**参数名       类型     说明
**ModelObject  Handle   Spool类型的ModelGroup的handle
**
**返回值
**返回管段名称的字符串，失败则返回整数-1；
*/
Get_SpoolName(handle ModelObject)
{	
	/*
	spool_number = PM_GET_OBJDATA(ModelObject, 0, MMT_COSA_SPOOL_NAME);
	parts = PM_GET_OBJECTS_IN_GROUP(ModelObject);
	if (parts != 0){
		part_number = PM_NR_MEMBERS_IN_SET(parts);
		if (part_number != 0){
			part = PM_GET_MEMBER_IN_SET(parts,0);
			line_name = PM_GET_OBJDATA(part, 0, MMT_TAG_PIPELINE_NAME);
			if (ISINT(line_name)){
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
	spool_name = line_name + "-" + spool_number;
	*/
	parts = PM_GET_OBJECTS_IN_GROUP(ModelObject);
	part = PM_GET_MEMBER_IN_SET(parts,0);
	spool_name = PM_GET_OBJDATA(part,0,Tag_Spool_Name);
	return(spool_name);
}

Set_Pipe_Sketch_Dimensioning_Properties(float scale)
{
	DW_TSLANT(0);
	DW_TTYPE(0);
	DW_DIMFORM("%Qa");
	
	DW_LAYER(DimLineLayer);
	DW_PEN(DimLinePen);
	DW_COLOR(DimLineColor);
	DW_TSIZE(scale * DimTextHeight, scale * DimTextWidth, scale * DimTextSpc);
	DW_DIMSYMHGHT(scale * DimSymHeight);
	
	DW_DIMVALDIST(scale * 1.0);
	DW_DIMOFFSETEX(scale * 1.0);
	DW_DIMSYM(1, 1);
	DW_ZLEVEL(0);
	
	return(0);
}