Line_Intersect_Line(x1,y1,z1,dx1,dy1,dz1,x2,y2,z2,dx2,dy2,dz2,x,y,z)
{  
   tolerance = 0.0000001;
   check1 = dx1*dy2-dx2*dy1;
   if(check1 < tolerance & check1 > -tolerance){
      check1 = 0;
   }
   else{
      check1 = 1;
   }
   check2 = dy1*dz2-dy2*dz1;
   if(check2 < tolerance & check2 > -tolerance){
      check2 = 0;
   }   
   else{
      check2 = 1;
   }
   check3 = dx1*dz2-dx2*dz1;
   if(check3 < tolerance & check3 > -tolerance){
      check3 = 0;
   }   
   else{
      check3 = 1;
   }
   if(!check1 & !check2 & !check3){
      return(-1);
   }
   if(check1){
      t = (dy1*(x2-x1)-dx1*(y2-y1))/(dx1*dy2-dx2*dy1);
      x = x2+dx2*t;
      y = y2+dy2*t;
      z = z2+dz2*t;  
      return(0);
   }
   else if(check2){
      t = (dy1*(z2-z1)-dz1*(y2-y1))/(dy2*dz1-dy1*dz2);
      x = x2+dx2*t;
      y = y2+dy2*t;
      z = z2+dz2*t;        
      return(0);            
   }
   else if(check3){
      t = (dz1*(x2-x1)-dx1*(z2-z1))/(dx1*dz2-dx2*dz1);
      x = x2+dx2*t;
      y = y2+dy2*t;
      z = z2+dz2*t;        
      return(0);            
   }   
}

/*Parameter    type     description 
--x            float    x coordinate value of point
--y            float    y coordinate value of point
--z            float    z coordinate value of point   
--dx           float    vector value of move direction      
--dy           float    vector value of move direction      
--dz           float    vector value of move direction      
--distance     float    move distance
*/  

Point_3D_Move_With_DirAndDis(float x,float y,float z,float dx, float dy, float dz, float distance)
{
   x = x + dx*distance;
   y = y + dy*distance;
   z = z + dz*distance;
   return(0);
}

/*
	Project point to line.
	
	Input & Output arguments:
	handle	point_set	Handle of point set, where totally 6 points
						3 input points:
						index 0	= coordinates of the 1st point of line
						index 1	= coordinates of the 2nd point of line
						index 2	= coordinates of the point to be projected
						1 output points:
						index 3	= coordinates of the projected CL point
	float	distance	Distance between nearest points
						
	Return values:
	int		0			Success
	int		-1			Failure
*/



/*Parameter    type     description 
--plane_dx     float    vector value of plane
--plane_dy     float    vector value of plane
--plane_dz     float    vector value of plane
--plane_x      float    x coordinate value of plane
--plane_y      float    y coordinate value of plane
--plane_z      float    z coordinate value of plane   
--line_dx      float    vector value of line      
--line_dy      float    vector value of line      
--line_dz      float    vector value of line      
--line_x       float    x coordinate value of line
--line_y       float    y coordinate value of line
--line_z       float    z coordinate value of line
--x,y,z        float    intersection point coordinate
*/ 
PlaneLineIntersectPoint(plane_dx,plane_dy,plane_dz,plane_x,plane_y,plane_z,line_dx,line_dy,line_dz,line_x,line_y,line_z,x,y,z)  
{ 
   vp1 = plane_dx;  
   vp2 = plane_dy;  
   vp3 = plane_dz;  
   n1 = plane_x;  
   n2 = plane_y;  
   n3 = plane_z;  
   v1 = line_dx;  
   v2 = line_dy;  
   v3 = line_dz;  
   m1 = line_x;  
   m2 = line_y;  
   m3 = line_z;  
   vpt = v1 * vp1 + v2 * vp2 + v3 * vp3;  
   /*首先判断直线是否与平面平行*/  
   if (vpt == 0)  
   {  
      return(-1);  
   }   
   t = ((n1 - m1) * vp1 + (n2 - m2) * vp2 + (n3 - m3) * vp3) / vpt;  
   x = m1 + v1 * t;  
   y = m2 + v2 * t;  
   z = m3 + v3 * t;  
   return(0);  
}  

Get_Point_Distance(x1,y1,z1,x2,y2,z2)
{
   dx = x2 - x1;
   dy = y2 - y1;
   dz = z2 - z1;
   distance = VEC_UNITV(dx,dy,dz);
   return(distance);
}

/*
	Get nearest points of two 3D lines (line A and line B). Two points are given from
	both lines. If lines are parallel (tolerance), then point 1 of line A and nearest
	point related to this point from line B are given as nearest points. Distance
	between nearest points and deviation between lines are given as additional output
	arguments.
	
	Input & Output arguments:
	handle	point_set	Handle of point set, where totally 6 points
						4 input points:
						index 0	= coordinates of the 1st point of 1st line
						index 1	= coordinates of the 2nd point of 1st line
						index 2	= coordinates of the 1st point of 2nd line
						index 3	= coordinates of the 2nd point of 2nd line
						2 output points:
						index 4	= coordinates of the nearest point of 1st line
						index 5	= coordinates of the nearest point of 2nd line
	float	distance	Distance between nearest points
	float	angle		Deviation between lines (-180 < deviation <= 180)
	float	tolerance	Tolerance of parallelity (usually 0.1 is quite good value)
						
	Return values:
	int		0			Success
	int		-1			Failure
*/

fleq(float a, float b, float dif)
{
	if(a > b - dif & a < b + dif)
		return(TRUE);
	else
		return(FALSE);
}