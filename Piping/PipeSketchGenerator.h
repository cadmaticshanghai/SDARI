/*���������ڵ�����*/
#define  Column_Number 4

/*�ܶ���Ϣ��Ӧ��TAG����*/

#define  Tag_Sketch_Name   "PS2"        /* �ܶ�СƱͼ������ */
#define  SketchStatus      "U39"        /* �ܶι���״̬ - ��ʱû��ʹ�� */
#define  CreationTime      ".c6"        /* �ܶ�СƱͼ������ʱ�� - ϵͳĬ�ϵ����� */

/* define drawing tag name */ 
#define	Tag_Drawing_ShipName         "~D0"                          /* ���� */
#define	Tag_Drawing_ShipNumber       "~D1"                          /* ���� */
#define	Tag_Drawing_DrawingNumber    "~D2"                          /* ͼ�� */
#define	Tag_Drawing_PageNumber       "~D3"                          /* ҳ�� */
#define	Tag_Drawing_TotalPage        "~D4"                          /* ��ҳ�� */
#define	Tag_Drawing_PipePreTre       "~D5"                          /* ����Ԥ���� */
#define	Tag_Drawing_OutSurfTreat     "~D6"                          /* ��������洦�� */
#define	Tag_Drawing_InSurfTreat      "~DH"                          /* �����ڱ��洦�� */
#define	Tag_Drawing_GasketMat        "~D7"                          /* ��Ƭ���� */
#define	Tag_Drawing_Insulation       "~D8"                          /* ��Ե���� */
#define	Tag_Drawing_PipeMaterial     "~D9"                          /* �ܲ���Ϣ */
#define	Tag_Drawing_Note             "~DA"                          /* ע�� */
#define	Tag_Drawing_SystemName       "~DB"                          /* ϵͳ���� */
#define	Tag_Drawing_TestPress        "~DC"                          /* ����ѹ�� */
#define	Tag_Drawing_BendingRadiu     "~DD"                          /* ��ģ�뾶 */
#define	Tag_Drawing_SpoolWeight      "~DE"                          /* �ܶ����� */
#define	Tag_Drawing_PlanningUnit     "~DF"                          /* ���̴��� */
#define	Tag_Drawing_SpoolName        "~DG"                          /* �ܺ� */

/* ������Ŀ��Ϣ��Ӧ��TAG���� */
#define Tag_Project_Description      ".dG"                          /* ��Ŀ���� - ��Ŀ���ƣ�ϵͳĬ������ */
#define Tag_Project_Number           "U32"                          /* ��Ŀ��� */



global handle Current_Spool_Handle, Cur_Spool_Model_Set;

/*����ܶ���ͼλ��*/
global int  View1_X = 70, View1_Y = 200;
global int 	View2_X = 160,View2_Y = 200;
global int 	View3_X = 70, View3_Y = 130;
global int 	View4_X = 160,View4_Y = 130;

/*����ܶ���ͼ��С*/
global int 	View_Size = 80;
global handle View_Info,View_Handles,View_Names,View_Positions;

/**/
global float View_Scale = 50;

/*�ܶ������������*/
global int Max_Spool_Parts_Number = 100;

/*�ܶ�����֧�ܸ���*/
global int Max_Spool_Branch_Number = 20;

/*��ǰ�ܶεķ�֧����*/
global int Spool_Branch_Number = 0;

/*����ܶθ���֧�ܵ�pipe��ȫ������*/
global handle Spool_Branch;
global handle Spool_Branch_Direction;

/*����ܶθ�����֧���������ȫ������*/
global handle Spool_Branch_Part_Number;

/*��ά��ͼ�ķ���*/
global float AxoRotAngle=-120.0;
global float AxoSlopeAngle=30.0;

/*�ܶ����ܵ�pipe����*/
global int Main_Branch_Pipe_Number = 1;

/*�ܶ�part�б�part����*/
global int Spool_Part_Number = 1;
global handle Spool_Parts;

/*���屣��ߴ��ע��Ϣ��ȫ������*/
global handle Dim_Table;
global int Dim_Number = 0;

/*����ܶ���������ݵ�ȫ������*/
global handle Mto_Table;
global handle Mto_Label_Table;
global int Mto_Number = 0;

/*������������*/
global int Mto_Table_Orign_X = 20;
global int Mto_Table_Orign_Y = 285;
global int Mto_Table_W = 150;
global int Mto_Table_H = 4;
global int Mto_Table_Column1_W = 6;
global int Mto_Table_Column2_W = 12;
global int Mto_Table_Column3_W = 10;
global int Mto_Table_Column4_W = 110;
global int Mto_Table_Column5_W = 12;
global string Mto_Table_Direction = "Down"; /* Down - �������� Up - ��������*/
global int Mto_Table_Line_Type = 0;
global int Mto_Table_Line_Colour = 1;
global int Mto_Table_Pattern_Length = 0;
global int Mto_Table_Text_Colour = 2;
global int Mto_Table_Text_Height = 2.5;

/*����ű�ע����*/
global int Mto_Label_Text_Colour = 1;
global int Mto_Label_Text_Height = 2.5;
global int Mto_Label_Leader_Line_Colour = 2;
global int Mto_Label_Leader_Line_Type = 0;
global int Mto_Label_Leader_Line_PattenLength = 0;
global int Mto_Label_Circle_Radius = 2;

/*����ܶ�У�ܱ����ݵ�ȫ������*/
global handle Check_Node_Table;
global int Check_Node_Number = 0;

/*У�ܱ��������*/
global int Check_Table_Orign_X = 170;
global int Check_Table_Orign_Y = 285;
global int Check_Table_W = 35;
global int Check_Table_H = 4;
global int Check_Table_Column1_W = 8;
global int Check_Table_Column2_W = 9;
global int Check_Table_Column3_W = 9;
global int Check_Table_Column4_W = 9;
global string Check_Table_Direction = "Down"; /* Down - �������� Up - ��������*/
global int Check_Table_Line_Type = 0;
global int Check_Table_Line_Colour = 1;
global int Check_Table_Pattern_Length = 0;
global int Check_Table_Text_Colour = 2;
global int Check_Table_Text_Height = 2.5;

/*�ܶ����ӱ��������*/
global int Connection_Table_Orign_X = 115;
global int Connection_Table_Orign_Y = 34;
global int Connection_Table_W = 90;
global int Connection_Table_H = 4;
global int Connection_Table_Column1_W = 10;
global int Connection_Table_Column2_W = 15;
global int Connection_Table_Column3_W = 15;
global int Connection_Table_Column4_W = 15;
global int Connection_Table_Column5_W = 35;
global string Connection_Table_Direction = "Up"; /* Down - �������� Up - ��������*/
global int Connection_Table_Line_Type = 0;
global int Connection_Table_Line_Colour = 1;
global int Connection_Table_Pattern_Length = 0;
global int Connection_Table_Text_Colour = 2;
global int Connection_Table_Text_Height = 2.5;

/*����ܶ����ӱ��ע��Ϣ��ȫ������*/
global handle Connection_Table;
global handle Connection_Label_Table;
global int Connection_Label_Number = 0;

/*�ܶ����Ӻű�ע����*/
global int Connection_Label_Text_Colour = 1;
global int Connection_Label_Text_Height = 2.5;
global int Connection_Label_Leader_Line_Colour = 2;
global int Connection_Label_Leader_Line_Type = 0;
global int Connection_Label_Leader_Line_PattenLength = 0;
global int Connection_Label_Circle_Radius = 2;

/*�������б�ע��Ϣ��ȫ������*/
global handle Label_Table;
global int Label_Number = 0;

/*��������*/
global string General_Font = "Arial Unicode MS";


/*��������Ϣ*/
global int Bent_Piece_Number = 0;
global handle Bending_Table_Head;
global handle Bending_Action_Number;
global handle Bending_Actions;

/*��ܱ��������*/
global int Bending_Table_Orign_X = 144;
global int Bending_Table_Orign_Y = 52;
global int Bending_Table_W = 61;
global int Bending_Table_H = 4;
global int Bending_Table_Column1_W = 20;
global int Bending_Table_Column2_W = 21;
global int Bending_Table_Column3_W = 20;
global string Bending_Table_Direction = "Up"; /* Down - �������� Up - ��������*/
global int Bending_Table_Line_Type = 0;
global int Bending_Table_Line_Colour = 1;
global int Bending_Table_Pattern_Length = 0;
global int Bending_Table_Text_Colour = 2;
global int Bending_Table_Text_Height = 2.5;

/*����ܶ�װ������ݵ�ȫ������*/
global handle Assembly_Table;
global handle Assembly_Label_Table;
global int Assembly_Number = 0;
global int Assembly_Label_Number = 0;

/*װ����������*/
global int Assembly_Table_Orign_X = 20;
global int Assembly_Table_Orign_Y = 34;
global int Assembly_Table_W = 95;
global int Assembly_Table_H = 4;
global int Assembly_Table_Column1_W = 12;
global int Assembly_Table_Column2_W = 14;
global int Assembly_Table_Column3_W = 14;
global int Assembly_Table_Column4_W = 14;
global int Assembly_Table_Column5_W = 41;
global string Assembly_Table_Direction = "Up"; /* Down - �������� Up - ��������*/
global int Assembly_Table_Line_Type = 0;
global int Assembly_Table_Line_Colour = 1;
global int Assembly_Table_Pattern_Length = 0;
global int Assembly_Table_Text_Colour = 2;
global int Assembly_Table_Text_Height = 2.5;

/*�ߴ��ע����*/
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

/*����ߴ��עģʽ 1 - ����䳤��ע 0 - ��ͶӰ���ȱ�ע */
#define	Dimension_Mode 0


/*���������sorting code������������ַ���*/
/*IHC�ڶ���������dimension tableʱ��û�а����ֲ��еĹ涨�����������sort code
--������͵��ж����Ȱ��������key word���й��࣬
--��������key wordû�ж��壬����������sort code
--��������key word��sort code��û�ж��壬�������ΪOTHER,������������ʾ�����Ϣ
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

	/* 1. ��ͨ��Geometry type�ж��������*/
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
		/* 2. Geometry type�޷�����������ͣ���ͨ��keyword����һ�����ж� */
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
				/* 3. ������޷�����������ͣ���ͨ�������sort code�����ж�*/
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

/* ��ȡ�ܶε����� 
**�������
**������       ����     ˵��
**ModelObject  Handle   Spool���͵�ModelGroup��handle
**
**����ֵ
**���عܶ����Ƶ��ַ�����ʧ���򷵻�����-1��
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