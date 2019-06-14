license()
{
    if(!check_license()){
        U_CONFIRM("请使用正版软件！");
        return(0);
    }
    
    if(!check_date()){
        U_CONFIRM("软件已过有效期！");
        return(0);        
    }    
    return(1);
}

check_license()
{
    return(1);
}

check_date()
{
    year = 2019;
    month = 12;
    date = 31;
    
    current_time = GET_TIME();  
    current = CNV_DATE(current_time,7);
    int current_year;
    current_year = SCAN_QUANT(3,STRINGTERM(current,"-"));
    current = SUBSTRING(SEARCH(current,"-"),1);
    int current_month;
    current_month = SCAN_QUANT(3,STRINGTERM(current,"-"));
    int current_date;
    current_date = SCAN_QUANT(3,SUBSTRING(SEARCH(current,"-"),1));
    if(current_year > year){       
        return(0);
    }
    else if(current_month > month){
        return(0);
    }
    else if(current_date > date){
        return(0);
    }
    return(1);
}