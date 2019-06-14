/* This script used to get nozzle information of passed equipment*/
Get_Nozzle_List(equipment)
{
    temp = alloc_2darray(254,4);
    index = 0;
    for(i=0;i<=254;i=i+1;){
        ns = PM_GET_CONPNT_DATA(equipment,i,"NS");
        if(ISSTRING(ns)){
            name = PM_GET_CONPNT_DATA(equipment,i,"nod");
            if(!ISSTRING(name)){
                name = "Undefined";
            }
            rate = PM_GET_CONPNT_DATA(equipment,i,"RT");
            if(!ISSTRING(rate)){
                rate = "Undefined";
            }            
            face = PM_GET_CONPNT_DATA(equipment,i,"ft1");
            if(!ISSTRING(face)){
                face = "Undefined";
            } 
            put_2darray(temp,index,0,ns);  
            put_2darray(temp,index,1,name); 
            put_2darray(temp,index,2,rate); 
            put_2darray(temp,index,3,face);
            index = index + 1;
        }     
    }
    if(index == 0){
        return(0);
    }
    
    list = alloc_2darray(index,4);
    for(i=0;i<index;i=i+1;){
        for(j=0;j<4;j=j+1;){
            value = get_2darray(temp,i,j);
            put_2darray(list,i,j,value);
        }        
    }
    
    free_2darray(temp);
    return(list);
}

/* This script let user pick one equipment in active view and return its handle*/
Pick_Equipment(string prompt)
{
    flag = 1;
    while(flag){
        tmp = 0;
        picked_object_handle = PM_PICK_OBJECT(prompt, tmp, "EQUIPMENT");
        if (!ISINT(picked_object_handle)){
            return(picked_object_handle);
        }
        else{
            return(0);
        }
    }
}