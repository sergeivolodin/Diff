#include "symbolic_computation.h"
#include "tree.h"
#include <vector>
using std::vector;

symbolic_computation::symbolic_computation() {
    //settings
    BR1="(";
    BR2=")";
    debug_level=DALL;

    arithmetic_operation x;
    x.name="+";
    x.priority=0;
    arithmetic_operations.push_back(x);

    x.name="-";
    x.priority=0;
    arithmetic_operations.push_back(x);

    x.name="*";
    x.priority=1;
    arithmetic_operations.push_back(x);

    x.name="/";
    x.priority=1;
    arithmetic_operations.push_back(x);

    x.name="^";
    x.priority=2;
    arithmetic_operations.push_back(x);


    set_all="fgh";
    set_numbers="abcd";
    set_variables="xyz";

    settings_filename="symbolic_computation.ini";

    settings_name_value_separator=":";
    settings_comment_symbol='#';
}

unsigned int symbolic_computation::op_prio(string x) {
    for(vector<arithmetic_operation>::iterator it=arithmetic_operations.begin();it<arithmetic_operations.end();it++) {
        if((*it).name==x) {
            return((*it).priority);
        }
    }
    return(0);
}

symbolic_computation::stringtype_t symbolic_computation::stringtype(string x) {
    if((x[0]>='0'&&x[0]<='9')||x[0]=='.') {
        return(SNUMBER);
    }
    else if(x[0]>='a'&&x[0]<='z') {
        return(SVARIABLE);
    }
    else if(x==BR1) {
        return(SBR1);
    }
    else if(x==BR2) {
        return(SBR2);
    }
    else {
        vector<arithmetic_operation>::iterator it;
        for(it=arithmetic_operations.begin();it<arithmetic_operations.end();it++) {
            if(!x.compare((*it).name)) return(SOPERATION);
        }
    }
    return(SOTHER);
}
