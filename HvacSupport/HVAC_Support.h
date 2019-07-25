/*Tag definition of Support*/
#define  Part_Number          "T2b"
#define  Planning_Unit        "~G2"
#define  Outfit_Area          ".lz"
#define  NCH_Thickness        "!0F"
#define  Surface_Treatment    "Udc"
#define	SupportType          "~G1"
#define	PadType              "ABC"
#define	SupportLiner         "U36"

/* 定义项目信息对应的TAG定义 */
#define Tag_Project_Description      ".dG"                          /* 项目描述 - 项目名称，系统默认属性 */
#define Tag_Project_Number           "U32"                          /* 项目编号 */

/*support material catalogue*/
#define  FlatBar_Part_Code       "+btMGnT9DISMrEvs5W4DSlG"
#define  AngleBar_Part_Code      "+71KhnCUDHnkdrImr1uyu70"
#define  SteelPlate_Part_Code    "+5DIPRKWfGx2lm0528XldV0"
#define  Bolt_Part_Code          "+Bw6..z41J1ICoodHv7HAeW"
#define  Nut_Part_Code           "+nFNlL0QvGzwCEo5kCPDjcG"
#define  FlatWasher_Part_Code    "+2McqQrqcHBYpjHeUV.ZEq0"
#define  SpringWasher_Part_Code   "+w8ZSqcOCHlczbSAduBYpSG"

/* default part id */
global   string   Support_FlatBar_Pid        = "+btMGnT9DISMrEvs5W4DSlG-1";
global   string   Support_Frame_Pid          = "+71KhnCUDHnkdrImr1uyu70-10";
global   string   Support_Pad_Pid            = "+5DIPRKWfGx2lm0528XldV0-9";
global   string   Support_Bolt_Pid           = "+Bw6..z41J1ICoodHv7HAeW-275";
global   string   Support_Nut_Pid            = "+nFNlL0QvGzwCEo5kCPDjcG-2";
global   string   Support_FlatWasher_Pid     = "+2McqQrqcHBYpjHeUV.ZEq0-5";
global   string   Support_SpringWasher_Pid   = "+w8ZSqcOCHlczbSAduBYpSG-5";

global   string   Group_Type = "7";
global   int      Group_Type_I = 7;

global   float    Rect_Pad_Gap     = 20;
global   float    Circle_Pad_Gap   = 50;
global   float    Max_Ecc_Angle = 5;
global   float    Bolt_Extra_Length = 2;

get_support_name()
{
   support_name = "";
   flag = 1;
   i = 1;
   while(flag){
      sequence = ITOASCII(i);
      length = STRLEN(sequence);
      if(length >= 4){
         support_name = "HS-" + sequence;
      }
      else if(length == 3){
         support_name = "HS-0" + sequence;
      }
      else if(length == 2){
         support_name = "HS-00" + sequence;
      }    
      else if(length == 1){
         support_name = "HS-000" + sequence;
      }         
      res = PM_FIND_BY_NAME(support_name);
      if(ISINT(res)){
         flag = 0;
      }     
      i = i + 1;
   }
   return(support_name);  
}

get_beam_para(part_handle,x1,y1,z1,x2,y2,z2,x_dx,x_dy,x_dz,end1x,end1y,end1z,end2x,end2y,end2z)
{
	x1 = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_X));
	y1 = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Y));
	z1 = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Z));
	length = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_LEN));
	axis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_SLP_ANGLE));
	dx = COS(axis_slope)*COS(axis_rotate);
	dy = COS(axis_slope)*SIN(axis_rotate);
	dz = SIN(axis_slope);
	x2 = x1 + length * dx;
	y2 = y1 + length * dy;
	z2 = z1 + length * dz;
	x_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_ROT_ANGLE));
	x_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_SLP_ANGLE));
	x_dx = COS(x_slope)*COS(x_rotate);
	x_dy = COS(x_slope)*SIN(x_rotate);
	x_dz = SIN(x_slope);	
	dx = x1-x2;
	dy = y1-y2;
	dz = z1-z2;
	VEC_UNITV(dx,dy,dz);
	end1x = dx;
	end1y = dy;
	end1z = dz; 
	end2x = -dx;
	end2y = -dy;
	end2z = -dz;			 
	return(0);  
}

get_plate_para(part_handle,lox,loy,loz,udx,udy,udz,vdx,vdy,vdz)
{
	lox = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_X));
	loy = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Y));
	loz = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Z));   
	axis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_SLP_ANGLE));
	udx = COS(axis_slope)*COS(axis_rotate);
	udy = COS(axis_slope)*SIN(axis_rotate);
	udz = SIN(axis_slope);
	saxis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_ROT_ANGLE));
	saxis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_SLP_ANGLE));
	vdx = COS(saxis_slope)*COS(saxis_rotate);
	vdy = COS(saxis_slope)*SIN(saxis_rotate);
	vdz = SIN(saxis_slope);	
   return(0);
}

get_standard_part_para(part_handle,lox,loy,loz,udx,udy,udz,vdx,vdy,vdz)
{
	lox = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_X));
	loy = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Y));
	loz = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_ORIG_Z));  
	axis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_ROT_ANGLE)); 
	axis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_AXIS_SLP_ANGLE));
	udx = COS(axis_slope)*COS(axis_rotate);
	udy = COS(axis_slope)*SIN(axis_rotate);
	udz = SIN(axis_slope);
	saxis_rotate = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_ROT_ANGLE));
	saxis_slope = String_To_Float(PM_GET_OBJDATA(part_handle,0,MMT_TAG_SAXIS_SLP_ANGLE));
	vdx = COS(saxis_slope)*COS(saxis_rotate);
	vdy = COS(saxis_slope)*SIN(saxis_rotate);
	vdz = SIN(saxis_slope);	
   return(0);	 
}