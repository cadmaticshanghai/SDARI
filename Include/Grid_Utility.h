Set_Column_Title(handle wobj, column_titles, int column_number)
{

	for (i = 0; i < column_number; i = i + 1;)
	{
		title = A_GET(column_titles,i);
		if (ISSTRING(title))
		{
			W_SET_WINDOW_ARGS(wobj, W_LISTW_COLUMN, i, W_LISTW_HEADER, title);
			W_SET_WINDOW_ARGS(wobj, W_LISTW_COLUMN, i, W_LISTW_COLUMNWIDTH,-1);
			W_SET_WINDOW_ARGS(wobj, W_LISTW_COLUMN, i, W_LISTW_ITEMTYPE, W_LISTW_STRING_COLUMN); 
		}
		else
		{
			return(0);
		}
	}
	return(0);
}

Get_Selected_Items(handle wobj, handle object_list, handle selected_objects, int selected_object_number)
{
	PM_SETOP_SUB(selected_objects, selected_objects);
	selected_rows = W_GET_WINDOW_ARG( wobj, W_LISTW_MSVALUE );
	total_lines = STRLEN(selected_rows);
	k = 0;
	for (i = 0; i < total_lines; i = i + 1){
		temp_string = SUBSTRING(selected_rows,i);
		flag = HEAD(temp_string,1);
		if (flag == "1"){
			W_SET_WINDOW_ARGS( wobj, W_LISTW_ROW, i);
			selected_row = W_GET_WINDOW_ARG( wobj, W_LISTW_ITEMID );	
			model_handle = PM_GET_MEMBER_IN_SET(object_list, selected_row);	
			PM_ADD_OBJECT_TO_SET(model_handle,selected_objects);
			k = k + 1;
		}
	}
	selected_objects_number = k;
	return (0);
}

Get_Row_By_ItemID(handle wobj, item_id)
{
    total_lines = W_GET_WINDOW_ARG(wobj,W_LISTW_NUMOFCHOICES);
    for(i=0;i<total_lines;i=i+1;){
        W_SET_WINDOW_ARGS( wobj, W_LISTW_ROW, i);
        id = W_GET_WINDOW_ARG( wobj, W_LISTW_ITEMID );
        if(item_id == id){
            return(i);
        }
    } 
    return(-1);
}

Auto_Highlight(int flag, handle high_light_list, handle selected_objects)
{
	if (flag == 1){
		/* restore color of highlighted objects */
		model_number = PM_NR_MEMBERS_IN_SET(high_light_list);
		for (i = 0 ; i < model_number; i = i + 1){
			model = PM_GET_MEMBER_IN_SET(high_light_list,i);
			Highlight_Object(model,FALSE);
		}
		temp = PM_INIT_SET();
		high_light_list = PM_COPY_SET(temp);
		PM_FREE_SET(temp);

		/* hightlight selected objects */
		model_number = PM_NR_MEMBERS_IN_SET(selected_objects);
		for (i = 0; i < model_number; i = i + 1){
			model = PM_GET_MEMBER_IN_SET(selected_objects,i);
			Highlight_Object(model,TRUE);
			PM_ADD_OBJECT_TO_SET(model,high_light_list);
		}
	}
}

Auto_Scale_Model(int flag, handle selected_objects)
{
	/* handle model scale */
	if (flag){
		model_number = PM_NR_MEMBERS_IN_SET(selected_objects);
		if (model_number == 1){
			model_handle = PM_GET_MEMBER_IN_SET(selected_objects,0);
			Auto_Scale(model_handle);
		}
	}
}

Publish_Data(handle grid,int row_number, int column_number, int sort_column, int sort_direction, handle data_array)
{
	W_SET_WINDOW_ARGS(grid, W_LISTW_ROW, 0);
	W_SET_WINDOW_ARGS(grid, W_LISTW_DELETEROWS, -1);
	for (i = 0; i < row_number; i = i + 1;){
		W_SET_WINDOW_ARGS(grid, W_LISTW_ROW, 0);
		W_SET_WINDOW_ARGS(grid, W_LISTW_INSERTROWS, 1);
		for ( j = 0; j < column_number; j = j + 1;){
			value = get_2darray(data_array,i,j);
			if (ISSTRING(value)){
				W_SET_WINDOW_ARGS(grid, W_LISTW_ROW, 0, W_LISTW_COLUMN, j, W_LISTW_STRING, value);
			}
		}
		W_SET_WINDOW_ARGS(grid, W_LISTW_ITEMID, i);
	}

	W_SET_WINDOW_ARGS(grid, W_LISTW_ROW, 1);

	/* Adjust the width of column automatically*/
	for (i=0; i < column_number; i = i + 1;){
		W_SET_WINDOW_ARGS(grid, W_LISTW_COLUMN, i, W_LISTW_COLUMNWIDTH, -1);
	}

	/* sort list */
	W_SET_WINDOW_ARGS(grid, W_LISTW_COLUMN, sort_column, W_LISTW_SORT, sort_direction);
}