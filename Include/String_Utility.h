/* 
1. if the original string doesn't include delimiter, then function will return a empty string;
2. if the original string include delimiter, then function will split the original string by the
    delimiter and return the sub string specify by sequence (start from 0);
3. if the sub string specified by sequence doesn't exist, function will return empty string;
*/
Get_Substring( string origin_string, string delimiter, int sequence)
{
	length = STRLEN(delimiter);
	remaining_string = "";
	sub_string = "";
	for (i = 0; i< sequence; i = i +1){
		remaining_string = SEARCH(origin_string,delimiter);
		sub_string = STRINGTERM(origin_string,delimiter);
		if ( remaining_string == ""){
			if ( i < sequence - 1){
				return "";	
			}
		}
		origin_string = SUBSTRING(remaining_string,length);	
	}
	return sub_string;
}

/* 
1. if the original string doesn't include delimiter, then function will return a empty string;
2. if the number of delimiter in the original string exceed 100, function will return the sub 
   string after the 100th delimiter;
*/
Get_Last_Substring( string origin_string, string delimiter)
{
	length = STRLEN(delimiter);
	sub_string = "";
	for (i = 0; i< 100; i = i +1){
		sub_string = SEARCH(origin_string,delimiter);
		if ( sub_string == ""){
			if (i == 0){
				return "";
			}
			else{
				return origin_string;
			}	
		}
		origin_string = SUBSTRING(sub_string,length);		
	}
	return origin_string;
}


/*This function is used to split a string to several string with specified sub-string
Input Arguments
Order        Argument       Type        Description
1            str            String      string to be split
2            sep_str        String      seperator
3            str_list       Array       handle of sub-string array
4            str_number     int         Maximum sub-string number
Output Arguments
Order        Argument       Type        Description
3            str_list       Array       handle sub-string array
4            str_number     int         sub-string number
Return Value
-1    fail
0     successful
*/
Split_String(string str,string sep_str, handle str_list, int str_number)
{
    index = 0;
    loop_flag = 1;
    /*if str start with sep_string*/
    if(HEAD(str,STRLEN(sep_str)) == sep_str){
        str = SUBSTRING(str,STRLEN(sep_str));
        while(loop_flag){
            sub_string = STRINGTERM(str,sep_str);
            A_PUT(str_list,index,sub_string);
            index = index + 1;
            str = SUBSTRING(SEARCH(str,sep_str),STRLEN(sep_str));
            /*if sub string number exceed maxiumum sub string number, then set the loop flag to fail*/
            if(index >= str_number){
                loop_flag = 0;
            }
            /*if str is split to the end, then set the loop flag to fail*/
            if(str == ""){
                loop_flag = 0;
            }
        }   
        str_number = index;
        return(0);   
    }
    /*if str doesn't start with sep_char*/
    else{
        while(loop_flag){
            sub_string = STRINGTERM(str,sep_str);
            A_PUT(str_list,index,sub_string);
            index = index + 1;
            str = SUBSTRING(SEARCH(str,sep_str),STRLEN(sep_str));
            /*if sub string number exceed maxiumum sub string number, then set the loop flag to fail*/
            if(index >= str_number){
                loop_flag = 0;
            }
            /*if str is split to the end, then set the loop flag to fail*/
            if(str == ""){
                loop_flag = 0;
            }
        }   
        str_number = index;
        return(0);  
    }
}