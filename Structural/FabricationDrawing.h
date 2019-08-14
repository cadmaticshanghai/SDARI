/* define drawing tag name */ 
#define	Tag_Drawing_ShipNumber      "Ua4"      /* 船号 */
#define	Tag_Drawing_DrawingNumber   "Dno"      /* 图号 */
#define	Tag_Drawing_Scale			"U35"      /* 比例 */
#define	Tag_Drawing_PageNumber      "Ub9"      /* 页码 */
#define	Tag_Drawing_TotalPage       "Uba"      /* 总页数 */
#define	Tag_Drawing_ModuleName      "Dsc"      /* 舾装件名称 */
#define	Tag_Drawing_UnitName        "dwt"      /* 舾装件描述 */
#define	Tag_Drawing_TotalWeight		"Ud5"      /* 舾装件重量 */

/* 项目属性 */
#define	Tag_Pro_ShipNumber			"U32"		/* 项目号 */

/*尺寸标注设置*/
global int		DimLinePen = 1;
global string		DimLineWidthName = "0.25";
global int		DimLineColor = 1;
global string		DimLineColorName = "red";
global int		DimLineLayer = 104;
global string		DimLineLayerName = "PM_dim_layer";
global float		DimTextHeight = 2.5;
global float		DimTextWidth = 1.5;
global float		DimTextSpc = 0.5;
global float		DimSymHeight = 2.0;

/*define tag of drawing*/
#define  Tag_DrawingStatus   "U39"
#define  Tag_CreationTime    ".c6"

/* define view position*/
global int	View1_X = 85, View1_Y = 200;
global int 	View2_X = 200,View2_Y = 200;
global int 	View3_X = 85, View3_Y = 77;
global int 	View4_X = 320,View4_Y = 200;

/*define view size*/
global float	View_Size  = 150;
global float	View_Scale = 50;
global handle View_Info,View_Handles,View_Names,View_Positions;


/*define ISO view direction*/
global float AxoRotAngle=-120.0;
global float AxoSlopeAngle=30.0;


global int Text_Colour = 1;
global int Text_Height = 3;

global   int      Group_Type_I = 7;

/* 零件标注相关系统变量 */
#define	Lbl_Name_Part_No	"Part_no_ref"

/*保存管段零件表内容的全局数组*/
global handle Mto_Table;
global handle Mto_Label_Table;
global int Mto_Number = 0;


/*零件表相关设置*/
global int Mto_Column_Number = 8;
global float Mto_Table_Orign_X = 272;
global float Mto_Table_Orign_Y = 35;
global float Mto_Table_W = 143;
global float Mto_Table_H = 6;
global float Mto_Table_Column1_W = 8;
global float Mto_Table_Column2_W = 30;
global float Mto_Table_Column3_W = 34.05;
global float Mto_Table_Column4_W = 16.21;
global float Mto_Table_Column5_W = 12.54;
global float Mto_Table_Column6_W = 12.21;
global float Mto_Table_Column7_W = 10.37;
global float Mto_Table_Column8_W = 19.63;

global int Mto_Table_Line_Type = 0;
global int Mto_Table_Line_Colour = 1;
global int Mto_Table_Pattern_Length = 0;
global int Mto_Table_Text_Colour = 2;
global int Mto_Table_Text_Height = 2.5;

global int Max_Parts_Number = 500;


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

Get_Drawing_View_Scale(view_scale)
{
	scale_number = 35;
	view_scales = A_ALLOC(scale_number);
	A_PUT(view_scales,0,0.02);
	A_PUT(view_scales,1,0.025);
	A_PUT(view_scales,2,0.04);
	A_PUT(view_scales,3,0.05);
	A_PUT(view_scales,4,0.067);
	A_PUT(view_scales,5,0.1);
	A_PUT(view_scales,6,0.2);
	A_PUT(view_scales,7,0.4);
	A_PUT(view_scales,8,0.5);
	A_PUT(view_scales,9,0.67);
	A_PUT(view_scales,10,1);
	A_PUT(view_scales,11,1.5);
	A_PUT(view_scales,12,2);
	A_PUT(view_scales,13,2.5);
	A_PUT(view_scales,14,3);
	A_PUT(view_scales,15,4);
	A_PUT(view_scales,16,5);
	A_PUT(view_scales,17,6);
	A_PUT(view_scales,18,7.5);
	A_PUT(view_scales,19,10);
	A_PUT(view_scales,20,15);
	A_PUT(view_scales,21,20);
	A_PUT(view_scales,22,25);
	A_PUT(view_scales,23,30);
	A_PUT(view_scales,24,40);
	A_PUT(view_scales,25,50);
	A_PUT(view_scales,26,60);
	A_PUT(view_scales,27,75);
	A_PUT(view_scales,28,100);
	A_PUT(view_scales,29,125);
	A_PUT(view_scales,30,150);
	A_PUT(view_scales,31,200);
	A_PUT(view_scales,32,250);
	A_PUT(view_scales,33,300);
	A_PUT(view_scales,34,400);
	for(i=0;i<scale_number;i=i+1;){
		value = A_GET(view_scales,i);
		if(view_scale <= value){
			value1 = A_GET(view_scales,i-1);
			if(view_scale - value1 < value - view_scale){
				view_scale = value1;
			}
			else{
				view_scale = value;
			}
			A_FREE(view_scales);
			return(0);
		}
	}
	view_scale = 400;
	A_FREE(view_scales);
	return(0);
}