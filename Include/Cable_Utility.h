/* This script use to get cable objects that pass through specified node
** InputParameter    Type     Description
** node_h            handle   handle of cable node
** ReturnValue
** model set include handle of cables pass through this node.
*/
get_cables_pass_through_node(node_h)
{
	ao_ord	= 0;
	st1		= 0; st2 = 0; st3 = 0; st4 = 0;
	order	= 0;
	pc_ord  = 0;

	ao_obj_set_h = PM_INIT_SET();

	go_on = 1;
	while (go_on) {
		value  = 0;	
	   type = 0;
		ao_obj = PM_NEXT_ASSOCIATION(node_h, pc_ord, ao_ord, type, value, order, 
								st1, st2, st3, st4);
		if(ao_obj == 0){
		   go_on = 0;
		}
		else{
	      type = PM_GET_OBJDATA(ao_obj,0,MMT_TAG_OBJTYPE);
   	   if(!ISINT(type)){
   	      if(type == "8"){
         		PM_ADD_OBJECT_TO_SET(ao_obj, ao_obj_set_h);
         	}
      	}
      }
	}
	return(ao_obj_set_h);
}