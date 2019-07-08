
/*����������Ϣ������*/
#define  Group_Type                    "20"
#define  Planning_Unit                 ".ba"

/* ������Ŀ��Ϣ��Ӧ��TAG���� */
#define Tag_Project_Description      ".dG"                          /* ��Ŀ���� - ��Ŀ���ƣ�ϵͳĬ������ */
#define Tag_Project_Number           "U32"                          /* ��Ŀ��� */


set_column_title(handle wobj, column_titles, column_number)
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

get_random_string()
{
   time = GET_TIME();
   date_string = CNV_DATE(time,2);
   time_sring = TRANS(CNV_TIME(time,1),":","-");
   random_string = date_string + "-" + time_sring; 
   return random_string;
}