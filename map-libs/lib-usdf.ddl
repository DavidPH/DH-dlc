// USDF for DH-dlc definitions.

#typedefnew:CHOICE      :object;
#typedefnew:CONVERSATION:object;
#typedefnew:COST        :object;
#typedefnew:IFITEM      :object;
#typedefnew:PAGE        :object;



#defaulttype:include  :string;
#defaulttype:namespace:string;



#defaulttype:text       :string:CHOICE;
#defaulttype:displaycost:bool  :CHOICE;
#defaulttype:yesmessage :string:CHOICE;
#defaulttype:nomessage  :string:CHOICE;
#defaulttype:log        :string:CHOICE;
#defaulttype:giveitem   :int   :CHOICE;
#defaulttype:special    :int   :CHOICE;
#defaulttype:arg0       :int   :CHOICE;
#defaulttype:arg1       :int   :CHOICE;
#defaulttype:arg2       :int   :CHOICE;
#defaulttype:arg3       :int   :CHOICE;
#defaulttype:arg4       :int   :CHOICE;
#defaulttype:nextpage   :PAGE  :CHOICE;
#defaulttype:closedialog:bool  :CHOICE;

#defaulttype:actor:int:CONVERSATION;

#defaulttype:item  :int:COST;
#defaulttype:amount:int:COST;

#defaulttype:item  :int :IFITEM;
#defaulttype:amount:int :IFITEM;
#defaulttype:page  :PAGE:IFITEM;

#defaulttype:name  :string:PAGE;
#defaulttype:panel :string:PAGE;
#defaulttype:voice :string:PAGE;
#defaulttype:dialog:string:PAGE;
#defaulttype:drop  :int   :PAGE;



