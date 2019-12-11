/*����������Ϣ������*/
#define  Surface_Treatment       "SIT"
#define  Structural_Unit_Name    "nam"
#define  DrawingName             "U3a"
#define	PDM_PS_DrawingNo        "U2f"

/*���������ڵ�����*/
#define  Column_Number 6

/*��װ����Ϣ��Ӧ��TAG����*/
#define  DrawingStatus   "U39"
#define  CreationTime   ".c6"

global   handle	Structural_Unit_Handle, Structural_Unit_Model_Set;
global   float		Support_Weight = 0.0;
global   string	PS_DrawingNo = "";

/*����ܶ���ͼλ��*/
global int		View1_X = 70, 	View1_Y = 150;
global int 	View2_X = 160,	View2_Y = 150;
global int 	View3_X = 70, 	View3_Y = 70;
global int 	View4_X = 160,	View4_Y = 70;

/*����ܶ���ͼ��С*/
global int 	View_Size = 80;
global handle	View_Info,View_Handles,View_Names,View_Positions;
global float	View_Scale = 50;

/*��ά��ͼ�ķ���*/
global float AxoRotAngle=-120.0;
global float AxoSlopeAngle=30.0;

/*���屣��ߴ��ע��Ϣ��ȫ������*/
global handle Dim_Table;
global int Dim_Number = 0;

/*����ܶ���������ݵ�ȫ������*/
global handle Mto_Table;
global handle Mto_Label_Table;

/*������������*/
global int Mto_Table_Orign_X = 205;
global int Mto_Table_Orign_Y = 187;
global int Mto_Table_W = 87;
global int Mto_Table_H = 4;
global int Mto_Table_Column1_W = 13;
global int Mto_Table_Column2_W = 61.5;
global int Mto_Table_Column3_W = 14;
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

/*����ű�ע����*/
global int Reference_Point_Label_Text_Colour = 1;
global int Reference_Point_Label_Text_Height = 2.5;
global int Reference_Point_Label_Leader_Line_Colour = 2;
global int Reference_Point_Label_Leader_Line_Type = 0;
global int Reference_Point_Label_Leader_Line_PattenLength = 0;
global int Reference_Point_Label_Circle_Radius = 2;

/*�������б�ע��Ϣ��ȫ������*/
global handle Label_Table;
global int Label_Number = 0;

/*��������*/
global string General_Font = "Arial Unicode MS";


/*�ߴ��ע����*/
global int		DimLinePen = 1;
global string	DimLineWidthName = "0.25";
global int		DimLineColor = 1;
global string	DimLineColorName = "red";
global int		DimLineLayer = 104;
global string	DimLineLayerName = "PM_dim_layer";
global float	DimTextHeight = 2.5;
global float	DimTextWidth = 1.5;
global float	DimTextSpc = 0.5;
global float	DimSymHeight = 2.0;





