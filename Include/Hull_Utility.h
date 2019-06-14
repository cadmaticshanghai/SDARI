Get_Panel_Data(hull,x,y,z,dx,dy,dz,t)
{
   xs = PM_GET_CONPNT_DATA(hull,2,"x");
   if(ISSTRING(xs)){
      /*node point position*/
      x = String_To_Float(PM_GET_CONPNT_DATA(hull,2,"x"));
      y = String_To_Float(PM_GET_CONPNT_DATA(hull,2,"y"));
      z = String_To_Float(PM_GET_CONPNT_DATA(hull,2,"z"));
      /*normal direction of panel*/
      fnr = PM_GET_CONPNT_DATA(hull,2,"fnr");
      fns = PM_GET_CONPNT_DATA(hull,2,"fns");
   }
   else{
      x = String_To_Float(PM_GET_CONPNT_DATA(hull,1,"x"));
      y = String_To_Float(PM_GET_CONPNT_DATA(hull,1,"y"));
      z = String_To_Float(PM_GET_CONPNT_DATA(hull,1,"z"));
      fnr = PM_GET_CONPNT_DATA(hull,1,"fnr");
      fns = PM_GET_CONPNT_DATA(hull,1,"fns");
   }
   rotation_angle = String_To_Float(fnr);
   slope_angle = String_To_Float(fns);
   dx = COS(slope_angle)*COS(rotation_angle);
   dy = COS(slope_angle)*SIN(rotation_angle);
   dz = SIN(slope_angle);
   VEC_UNITV(dx,dy,dz);
   ts = PM_GET_OBJDATA(hull,0,NCH_Thickness);
   if(ISSTRING(ts)){
      t = String_To_Float(ts);
   }
   return(0);
}

Pick_Hull_Panel(string prompt)
{
   flag = 1;
   while(flag){
      tmp = 0;
      picked_hull_handle = PM_PICK_OBJECT(prompt, tmp, "EQUIPMENT");
      if(!ISINT(picked_hull_handle)){
         if(Is_Hull_Part(picked_hull_handle)){
            /*set flag for skip looping*/
            flag = 0;
         }
      }
   }
   return(picked_hull_handle);
}

/*check if the passed model is hull structure*/
Is_Hull_Part(handle part_handle)
{
   check = PM_GET_OBJDATA(part_handle,0,".qL");
   if(check == "1"){
      return(1);
   }
   return(0);
}