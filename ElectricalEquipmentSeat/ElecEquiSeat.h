/*定义基座信息属性*/
#define	PadType				"~P2"			/* 基座垫片类型 */
#define	Part_Number			"~P0"			/* 基座零件编号 */
#define	SeatDes				"~G3"			/* 基座名称     */
#define	SeatType			"~G1"			/* 基座类型     */

#define	SeatPara			"SSP"			/* 基座参数     */
#define	NCH_Thickness		"!0H"			/* 船体板厚度   */
#define	SeatSize			"SSP"			/* 基座的尺寸参数   */					

global string	Group_Type  =  "7";
global int		Group_Type_I = 7;

global string	Earth_Bolt_Cata =  "ROC_ELEC_Earth_Bolt";
global string	Beam_part_Cata  =  "Equal Leg Angle";
global string	Hole_Part_Cata  =  "虚拟开孔零件";
global string	Beam_Part_Code  =  "";
global string	Steel_Plate_Code =  "";
global string	Hole_Part_Code  =  "";
global string	Tube_Part_Code  =  "";

global string	Earth_Bolt_Pid1 = "";
global string	Earth_Bolt_Pid2 = "";
global string	Earth_Bolt_Pid3 = "";
global string	Beam_Part1_Pid  = "";
global string	Beam_Part2_Pid  = "";
global string	Steel_Plate_Pid  = "";
global string	Seat_Hole_Pid   = "";

global string	Virtual_Hole_DT = "*Virtual*";
global string	Seat_Name_Prev	= "";
global string	Seat_Name_Index = "00";

global float	Rect_Pad_Gap	= 20;
global float	Circle_Pad_Gap	= 50;
global float	Panel_Default_Thickness	= 8;

global int		Max_Panel_Node_Number	= 10;
global float	Center_Hole_Diameter	= 10.0;
global float	Earth_Bolt_Rotate_Angle = 15.0;
global float	Default_Plate_Corner_R  = 15.0;
global float   Max_Angle_Tolerence     = 0.1;


/*向量的叉积*/      
Vector3d_CrossProduct(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2, float dx3, float dy3, float dz3)
{
    dx3 = dy1*dz2 - dz1*dy2;
    dy3 = dz1*dx2 - dx1*dz2;
    dz3 = dx1*dy2 - dy1*dx2;
} 

/*点1延方向移动一定距离得到点2*/
Point_Move(float x1, float y1, float z1, float x2, float y2, float z2, float v_x, float v_y, float v_z, float distance)
{
    vx = 0.0; vy=0.0; vz=0.0;
    Vector3d_ToUnitVector(v_x,v_y,v_z, vx,vy,vz);
    x2 = x1+vx*distance;
    y2 = y1+vy*distance;
    z2 = z1+vz*distance;
    return (1);
}

/*向量的单位向量*/      
Vector3d_ToUnitVector(float dx1, float dy1, float dz1, float dx2, float dy2, float dz2)
{
    length = Vector3d_Length(dx1, dy1, dz1);
    if (length >= 1.0E-15){
        dx2 = dx1 / length;
        dy2 = dy1 / length;
        dz2 = dz1 / length;
    }
    else{
        dx2 = dx1;
        dy2 = dy1;
        dz2 = dz1;
    }
} 

/*向量的模*/      
Vector3d_Length(float dx, float dy, float dz)
{
    return (SQRT(dx*dx + dy*dy + dz*dz));
} 