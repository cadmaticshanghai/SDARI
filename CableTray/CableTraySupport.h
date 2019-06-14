/*定义支架信息属性*/
#define	SupportDes                  "~G3"   /*string - 支架名称 */
#define	SupportType                 "~G1"   /*string - 支架类型 */
#define PadType                     "~P2"   /*string - 垫板类型 */
#define Part_Number                 "~P0"   /*string - 零件编号 */

#define Tag_Tray_Label              "Uba"   /*string - 托架标注内容 */

/*  
    This tag may be change in different project, if you find that you can't get correct result,
    please check your hull model to find the correct one
*/
#define NCH_Thickness               "!0H"   /*string - 船体板厚度 */
#define PlanningUnit                "~G2"   /*string - 托盘编号 */

global string Group_Type            = "7";
global int    Group_Type_I          = 7;

global string AngleBar_Part_Code    =  "+N3cY4is9GUIlwVWRrSg4.W";   /* 默认的角钢零件标准 */
global string Plate_Part_Code       =  "+Ayc6zjQdGXoJr1SAdGiEbm";   /* 默认的钢板零件标准 */
global string Pad_Part_Code         =  "+zRTeATilJQwyeKsiW.nwk0";   /* 默认的标准垫板零件标准 */

global string Beam_Part_Pid         =  "+N3cY4is9GUIlwVWRrSg4.W-6"; /* 默认的角钢零件 */
global string Plate_Part_Pid        =  "+Ayc6zjQdGXoJr1SAdGiEbm-8"; /* 默认的钢板零件 */
global string Pad_Part_Pid          =  "+zRTeATilJQwyeKsiW.nwk0-0"; /* 默认的标准垫板零件 */

global float Pad_Gap                = 10;   /* 默认的垫板与角钢的边距 */
global float Pad_Corner_Radius      = 10;   /* 默认的垫板圆角半径 */
global int   Max_Panel_Node_Number  = 10;   /* 最大的钢板轮廓顶点数量 */

global float Panel_Default_Thickness = 12;  /* 默认的船体甲板/舱壁厚度 */




