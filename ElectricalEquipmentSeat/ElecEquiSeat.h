/*���������Ϣ����*/
#define	PadType				"~P2"			/* ������Ƭ���� */
#define	Part_Number			"~P0"			/* ���������� */
#define	SeatDes				"~G3"			/* ��������     */
#define	SeatType			"~G1"			/* ��������     */

#define	SeatPara			"SSP"			/* ��������     */
#define	NCH_Thickness		"!0H"			/* �������   */
#define	SeatSize			"SSP"			/* �����ĳߴ����   */					

global string	Group_Type  =  "7";
global int		Group_Type_I = 7;

global string	Earth_Bolt_Cata =  "ROC_ELEC_Earth_Bolt";
global string	Beam_part_Cata  =  "Equal Leg Angle";
global string	Hole_Part_Cata  =  "���⿪�����";
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
