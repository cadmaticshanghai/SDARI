Delete_Row_2darray(handle a_h, int row)
{
	o_r = A_GET(a_h,0);
	o_c = A_GET(a_h,1);
	new_rows = o_r - 1;
    new = A_ALLOC( 2 + new_rows * o_c );
    A_PUT(new,0,new_rows);
    A_PUT(new,1,o_c);
    index = 2;
    for(i=0;i<o_r;i=i+1){
        if(i != row){
            for(k=0;k<o_c;k=k+1;){
                value = A_GET(a_h,2+i*k + k);
                A_PUT(new,index,value);
                index = index + 1;
            }            
        }       
    }
	tmp = a_h;
	a_h = new;
	A_FREE(tmp);
	return(a_h);
}

Delete_Col_2darray(handle a_h, int col)
{
    return(0);
}

Append_Row_2darray(handle a_h, handle row)
{
	o_r = A_GET(a_h,0);
	o_c = A_GET(a_h,1);
	new_rows = o_r + 1;
    new = A_ALLOC( 2 + new_rows * o_c );
    A_PUT(new,0,new_rows);
    A_PUT(new,1,o_c);
    index = 2;
    for(i=0;i<o_r;i=i+1){
        for(k=0;k<o_c;k=k+1;){
            value = A_GET(a_h,2+i*k + k);
            A_PUT(new,index,value);
            index = index + 1;
        }                  
    }
    index = 2 + o_r*o_c;
    for(i=0;i<o_c;i=i+1;){
        value = A_GET(row,i);
        A_PUT(new,index,value);
        index = index + 1;        
    }
	tmp = a_h;
	a_h = new;
	A_FREE(tmp);
	return(a_h);
}