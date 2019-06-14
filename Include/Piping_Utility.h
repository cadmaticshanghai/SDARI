/* This function use to get component type of pipe part
1. Judge pipe part component type base on sort code
2. if no sort code, then judge by KW
3. if no KW ...
*/

Get_Pipe_Component_Type(part_handle)
{
    part_id = PM_GET_OBJDATA(part_handle, 1, "pid"); 
    sort_code = DM_PARTID_DATA(part_id, "sor");
    g_type = DM_PARTID_DATA(part_id, "GT");  
    key_word = DM_PARTID_DATA(part_id,"KW");
    des = DM_PARTID_DATA(part_id, "DE");
    /* if sort code exist */
    if(sort_code != ""){                        
        sort = String_To_Int(sort_code);
        if(sort>=10 & sort<=18){
            mth = PM_GET_OBJDATA(part_handle, 1, MMT_TAG_METHOD);
            if(ISSTRING(mth)){
                if(mth == "$B"){
                    return("Bend"); 
                }                         
            } 
            else{
               return("Tube");
            }         
         }
         else if(sort>=20 & sort<=28){
            return("Elbow");
         }
         else if(sort>=30 & sort<=38){
             return("CReducer");
         }
         else if(sort>=40 & sort<=48){
             return("EReducer");
         }
         else if(sort>=50 & sort<=58){
             return("Tee");
         }
         else if(sort==60){
             return("Clamp");
         }
         else if(sort>=70 & sort<=78){
             return("Cap");
         }
         else if(sort>=80 & sort<=88){
             return("Cross");
         }
         else if(sort>=90 & sort<=98){
             return("T-Drill");
         }
         else if(sort>=100 & sort<=138){
             return("Flange");
         }
         else if(sort>=140 & sort<=148){
             return("BlindFlange");
         }
         else if(sort>=150 & sort<=158){
             return("OrificePlate");
         }         
         else if(sort>=160 & sort<=168){
             return("SpectacleFlange");
         }          
         else if(sort>=170 & sort<=188){
             return("Flange");
         }         
         else if(sort>=190 & sort<=198){
             return("Sleeve");
         }
         else if(sort>=250 & sort<=258){
             return("Penetration");
         }
         else if(sort>=300 & sort<=308){
             return("Cap");
         }
         else if(sort>=400 & sort<=404){
             return("Plug");
         }
         else if(sort>=410 & sort<=414){
             return("Socket");
         }
         else if(sort>=420 & sort<=424){
             return("ReducingSocket");
         }
         else if(sort>=430 & sort<=434){
             return("Nipple");
         }
         else if(sort>=440 & sort<=444){
             return("ReducingNipple");
         }
         else if(sort>=450 & sort<=454){
             return("Union");
         }
         else if(sort>=460 & sort<=464){
             return("ReducingUnion");
         }
         else if(sort>=470 & sort<=474){
             return("ThreadElbow");
         }
         else if(sort>=480 & sort<=484){
             return("ThreadTee");
         }
         else if(sort>=490 & sort<=494){
             return("ThreadCross");
         }
         /**/
         else if(sort>=600 & sort<=650){
             return("QuickCoupling");
         }
         else if(sort>=900 & sort<=920){
             return("Bellows");
         }
         else if(sort>=950 & sort<=970){
             return("Compensators");
         }
         else if(sort==1000){   
             return("BallValve");   /*球阀*/
         }
         else if(sort==1010){
             return("SegmentValve"); /*弓形阀*/
         }
         else if(sort==1020){
             return("GlobeValve");  /*球阀*/
         }
         else if(sort==1030){
             return("GateValve");  /*闸阀*/
         }
         else if(sort==1040){
             return("SlideValve");  /*滑阀*/
         }
         else if(sort==1050){
             return("ButterflyValve");  /*碟阀*/
         }
         else if(sort==1060){
             return("DiaphgramValve");  /*隔膜阀*/
         }
         else if(sort==1070 | sort==1080){
             return("NonReturnValve");  /*止回阀*/
         }
         else if(sort==1090){
             return("PressureReliefValve");  /*压力释放阀*/
         }
         else if(sort==1100){
             return("PressureReductionValve");  /*减压阀*/
         }
         else if(sort==1110){
             return("PressureKeepingValve");  /*保压阀*/
         }
         else if(sort==1120){
             return("ShutterValve");  /*百叶阀*/
         }
         else if(sort==1130){
             return("FloaterValve");  /*浮球阀*/
         }
         else if(sort==1140){
             return("SafetyValve");  /*安全阀*/
         }
         else if(sort==1150){
             return("SelfClosingValve");  /*自闭阀*/
         }
         else if(sort==1160){
             return("QuickClosingValve");  /*快关阀*/
         }
         else if(sort==1170){
             return("AirVentValve");  /*排气阀*/
         }
         else if(sort==1180){
             return("PlugValve");  /*旋塞阀*/
         }
         else if(sort==1190){
             return("NeedleValve");  /*针阀*/
         }
         else if(sort==1200){
             return("3-WayValve");  /*三通阀*/
         }
         else if(sort==1205){
             return("OtherValve");  /*其它阀*/
         }
         else if(sort==1210){
             return("BallValveActuator");  /*带驱动器的球阀*/
         }
         else if(sort==1220){
             return("SegmentValveActuator");  /*带驱动器的弓形阀*/
         }
         else if(sort==1230){
             return("GlobeValveActuator");  /*带驱动器的球阀*/
         }
         else if(sort==1240){
             return("GateValveActuator");  /*带驱动器的闸阀*/
         }
         else if(sort==1250){
             return("SlideValveActuator");  /*带驱动器的滑阀*/
         }
         else if(sort==1260){
             return("ButterflyValveActuator");  /*带驱动器的碟阀*/
         }         
         else if(sort==1270){
             return("DiaphgramValveActuator");  /*带驱动器的隔膜阀*/
         }         
         else if(sort==1280){
             return("3-WayValveActuator");  /*带驱动器的三通阀*/
         }           
         else if(sort==1295){
             return("OtherValveActuator");  /*带驱动器的三通阀*/
         }
         else if(sort==1300 | sort==1350){
             return("Filter");  /*滤器*/
         }  
         else if(sort==1400){
             return("AirSeparator");  /*汽水分离器*/
         } 
         else if(sort==1420){
             return("SteamTrap");  /*蒸汽滤器*/
         }   
         else if(sort==1500){
             return("FlowMeter");  /*流量计*/
         }  
         else if(sort==1510){
             return("FlowDirectionIndicator");  /*流向指示器*/
         }
         else if(sort==1520){
             return("Thermometer");  /*温度计*/
         } 
         else if(sort==1530){
             return("PressureMeter");  /*压力表*/
         }
         else if(sort==1540){
             return("LevelIndicator");  /*液位指示器*/
         }
         else if(sort==1550){
             return("SightGlass");  /*液流观察器*/
         }
         else if(sort==1560){
             return("OtherIndicator");  /*其它指示器*/
         }
         else{
            return("OTHER");
         }
    }
    else if(key_word != ""){
        if(PATTERN_MATCH(key_word,"*PIPE*")){
            mth = PM_GET_OBJDATA(part_handle, 1, MMT_TAG_METHOD);
            if(ISSTRING(mth)){
               if(mth == "$B"){
                  return("Bend"); 
               }                         
            } 
            else{
               return("Tube");
            }                    
        } 
        else if(PATTERN_MATCH(key_word,"*FLANGE*")){
            return("Flange");
        }
        else if(PATTERN_MATCH(key_word,"*ELBOW*")){
            return("Elbow");
        }        
        else if(PATTERN_MATCH(key_word,"*TEE*")){
            return("Tee");
        }   
        else if(PATTERN_MATCH(key_word,"*CROSS*")){
            return("Cross");
        }  
        else if(PATTERN_MATCH(key_word,"*LATERAL*")){
            return("Branch");
        }
        else if(PATTERN_MATCH(key_word,"*NIPPLE*")){
            return("Nipple");
        } 
        else if(PATTERN_MATCH(key_word,"*COUPLING*")){
            return("Coupling");
        }
        else if(PATTERN_MATCH(key_word,"*UNION*")){
            return("Union");
        }
        else if(PATTERN_MATCH(key_word,"*BUSHING*")){
            return("Sleeve");
        }
        else if(PATTERN_MATCH(key_word,"*BUSHING*")){
            return("Sleeve");
        }
        else if(PATTERN_MATCH(key_word,"*STUD*")){
            return("Branch");
        }
        else if(PATTERN_MATCH(key_word,"*NOZZLE*")){
            return("Nozzle");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE GLOBE*")){
            return("GlobeValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE GATE*")){
            return("GateValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE BALL*")){
            return("BallValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE NEEDLE*")){
            return("NeedleValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE PLUG*")){
            return("PlugValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE BUTTERFLY*")){
            return("ButterflyValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE CHECK*")){
            return("CheckValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE PISTON*")){
            return("PistonValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE DIAPHRAGM*")){
            return("DiaphragmValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE ANGLE*")){
            return("AngleValve");
        }
        else if(PATTERN_MATCH(key_word,"*VALVE*")){
            return("OtherValve");
        }
        else if(PATTERN_MATCH(key_word,"*FILTER*")){
            return("Filter");
        }
        else if(PATTERN_MATCH(key_word,"*THERMOMETER*")){
            return("Thermometer");
        }
        else if(PATTERN_MATCH(key_word,"*FLOWMETER*")){
            return("FlowMeter");
        }
        else if(PATTERN_MATCH(key_word,"*HOSE*")){
            return("Hose");
        }
        else if(PATTERN_MATCH(key_word,"*COMPENSATOR*")){
            return("Compensators");
        }
        else if(PATTERN_MATCH(key_word,"*CAMLOCK*")){
            return("Camlock");
        }
        else if(PATTERN_MATCH(key_word,"*GAUGE*")){
            return("Gauge");
        }
        else{
            return("Other");
        }
    }
    else if(des != ""){
        return("Other");   
    }
    else{
        return("Other");
    }
}

Get_Pipe_Connection_Type(part_handle,node)
{
    part_id = PM_GET_OBJDATA(part_handle,0,MMT_TAG_PARTID);
    conn_type = DM_PARTID_DATA(part_id,DM_COSA_1stCONNECTION_TYPE);
    if(conn_type != ""){
        return(conn_type);
    }
    else{
        return("WELDING");
    }
}

Get_Connection_Type(part_id,connection_number)
{
    connect_type = "";
    connection_code = DM_PARTID_DATA(part_id,"CO");
    main_type = HEAD(SUBSTRING(connection_code,2*(connection_number-1)),1);
    if(main_type == "1" | main_type == "3" | main_type == "D" | main_type == "G" | main_type == "J"
    |  main_type == "K" | main_type == "L" | main_type == "M" | main_type == "O"){
        connect_type = "Flange";
    }
    else if(main_type == "4"){
        connect_type = "NeckWeld";
    }
    else if(main_type == "6" | main_type == "7"){
        connect_type = "Branch";     
    }
    else if(main_type == "B" | main_type == "C"){
        connect_type = "Thread";     
    }    
    else{
        connect_type = "Other";
    }
    return(connect_type);
}

Get_Component_Normal_Size(part_id,connection_number)
{
    data = "D0" + ITOASCII(connection_number);
    dn = DM_PARTID_DATA(part_id,data);
    if(dn == ""){
        dn = DM_PARTID_DATA(part_id,"D01");
        if(dn == ""){
            dn = "0.0";
        }
    }
    return(dn);  
}


/*  This function use to project branch connection point to the center line of main pipe
    Parameter   type    meaning 
    mother_obj  handle  handle of main pipe object
    mother_nod  int     connection node id
    x           float   x coordinate value of connection point
    y           float   y coordinate value of connection point
    z           float   z coordinate value of connection point
*/
Transform_Branch_Surface_Point_To_CL(handle mother_obj, int mother_nod, float x, float y, float z)
{   
	distance = 0.0;
	mother_nth_part = PM_FIND_BRANCH_IN_PIPE(mother_obj, mother_nod, distance);
	if(mother_nth_part < 0)
		return(-1);
		
	branch_nod = 0;
	branch_obj = PM_GET_CONNECTED_OBJECT(mother_obj, mother_nod, branch_nod);
	if(ISINT(branch_obj))
		return(-1);
		
	branch_nth_part = Node_Id_To_Part_Number(branch_obj, branch_nod);
	
	mother_pid = PM_GET_OBJDATA(mother_obj, mother_nth_part, MMT_TAG_PARTID);
	branch_pid = PM_GET_OBJDATA(branch_obj, branch_nth_part, MMT_TAG_PARTID);
	
	/* Branch point in case it is not possible to get CL point */
	branch_x = 1.0 * PM_GET_CONPNT_DATA(branch_obj, branch_nod, "x");
	branch_y = 1.0 * PM_GET_CONPNT_DATA(branch_obj, branch_nod, "y");
	branch_z = 1.0 * PM_GET_CONPNT_DATA(branch_obj, branch_nod, "z");
	x = branch_x;
	y = branch_y;
	z = branch_z;
	
	mother_geo_type = DM_PARTID_DATA(mother_pid, "GT");
	if(mother_geo_type != "1" & mother_geo_type != "4")
		return(-1);
		
	/* End points of mother part */
	objgeom = PM_CREATE_PNTSET(10);
	mx1 = 0.0;	my1 = 0.0;	mz1 = 0.0;
	mx2 = 0.0;	my2 = 0.0;	mz2 = 0.0;
	PM_GET_OBJECT_GEOM(mother_obj, mother_nth_part, objgeom);
	PM_GET_PNT(objgeom, 0, mx1, my1, mz1);
	PM_GET_PNT(objgeom, 1, mx2, my2, mz2);
	PM_FREE_PNTSET(objgeom);
		
	is_eccentric = FALSE;
	branch_geo_type = DM_PARTID_DATA(branch_pid, "GT");
	if(branch_geo_type == "1" | branch_geo_type == "4"){
		/* End points of branch part */
		objgeom = PM_CREATE_PNTSET(10);
		bx1 = 0.0;	by1 = 0.0;	bz1 = 0.0;
		bx2 = 0.0;	by2 = 0.0;	bz2 = 0.0;
		PM_GET_OBJECT_GEOM(branch_obj, branch_nth_part, objgeom);
		PM_GET_PNT(objgeom, 0, bx1, by1, bz1);
		PM_GET_PNT(objgeom, 1, bx2, by2, bz2);
		PM_FREE_PNTSET(objgeom);
	
		point_set = PM_CREATE_PNTSET(10);
		PM_SET_PNT(point_set, 0, mx1, my1, mz1);
		PM_SET_PNT(point_set, 1, mx2, my2, mz2);
		PM_SET_PNT(point_set, 2, bx1, by1, bz1);
		PM_SET_PNT(point_set, 3, bx2, by2, bz2);

      /*原来的程序中，如果支管中心线的两个位置坐标的顺序是按照由远到近的话，得到的结果是错误的
      **添加下面的一个段代码来修正这个错误
      */     
      chech_and_fix_point_sequence(point_set);
			
		distance = 0.0;
		angle = 0.0;
		tolerance = 0.1;
		Get_Nearest_Points_Of_Two_3D_Lines(point_set, distance, angle, tolerance);
		PM_GET_PNT(point_set, 4, x, y, z);		
		PM_FREE_PNTSET(point_set);
		if(distance > 2)
			is_eccentric = TRUE;
	}
	if(is_eccentric | (branch_geo_type != "1" & branch_geo_type != "4")){
		point_set = PM_CREATE_PNTSET(10);
		PM_SET_PNT(point_set, 0, mx1, my1, mz1);
		PM_SET_PNT(point_set, 1, mx2, my2, mz2);
		PM_SET_PNT(point_set, 2, branch_x, branch_y, branch_z);
		distance = 0.0;
		Project_Point_To_Line(point_set, distance);
		PM_GET_PNT(point_set, 3, x, y, z);
		PM_FREE_PNTSET(point_set);
	}
	
	return(0);
}

Node_Id_To_Part_Number(handle obj_handle, int node_id)
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

chech_and_fix_point_sequence(point_set)
{
	x0 = 0.0;	y0 = 0.0;	z0 = 0.0;
	x1 = 0.0;	y1 = 0.0;	z1 = 0.0;
	x2 = 0.0;	y2 = 0.0;	z2 = 0.0;	
	x3 = 0.0;	y3 = 0.0;	z3 = 0.0;
	
	PM_GET_PNT(point_set, 0, x0, y0, z0);
	PM_GET_PNT(point_set, 1, x1, y1, z1);
	PM_GET_PNT(point_set, 2, x2, y2, z2);
	PM_GET_PNT(point_set, 3, x3, y3, z3);
	
	dx20 = x2-x0;dy20 = y2-y0;dz20 = z2-z0;
	dx21 = x2-x1;dy21 = y2-y1;dz21 = z2-z1;
	dx30 = x3-x0;dy30 = y3-y0;dz30 = z3-z0;
	dx31 = x3-x1;dy31 = y3-y1;dz31 = z3-z1;
	
   dis20 = SQRT(dx20*dx20+dy20*dy20+dz20*dz20);	
   dis21 = SQRT(dx21*dx21+dy21*dy21+dz21*dz21);	
   dis30 = SQRT(dx30*dx30+dy30*dy30+dz30*dz30);	
   dis31 = SQRT(dx31*dx31+dy31*dy31+dz31*dz31);	
   
   if((dis20+dis21)<(dis30+dis31)){
      x=x2;y=y2;z=z2;
      x2=x3;y2=y3;z2=z3;
      x3=x;y3=y;z3=z;
	   PM_SET_PNT(point_set, 2, x2, y2, z2);
	   PM_SET_PNT(point_set, 3, x3, y3, z3);   
   }	
	return(0);
}
