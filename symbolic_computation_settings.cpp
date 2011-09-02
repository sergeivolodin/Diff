#include "symbolic_computation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string.h>

using std::string;
using std::ifstream;
using std::ofstream;
using std::cerr;
using std::endl;
using std::vector;
using std::stringstream;

void symbolic_computation::write_settings() {
    ofstream dfile(settings_filename.c_str());

    dfile << settings_comment_symbol << "symbolic computations 2.0" << endl;

    dfile << endl;

    dfile << "#sets symbols" << endl;
    dfile << "set_all" << settings_name_value_separator << set_all << endl;
    dfile << "set_variables" << settings_name_value_separator << set_variables << endl;
    dfile << "set_numbers" << settings_name_value_separator << set_numbers << endl;

    dfile << endl;

    dfile << "#debug_level" << settings_name_value_separator << "DNONE/DEASY/DALL" << endl;
    dfile << "debug_level" << settings_name_value_separator;
    if(debug_level==DNONE) dfile << "DNONE";
    else if(debug_level==DEASY) dfile << "DEASY";
    else if(debug_level==DALL) dfile << "DALL";
    dfile << endl;

    dfile << endl;

    dfile << "#brackets" << endl;
    dfile << "BR1" << settings_name_value_separator << BR1 << endl;
    dfile << "BR2" << settings_name_value_separator << BR2 << endl;

    dfile << endl;

    dfile << "#arithmetic_operations" << endl;
    vector<arithmetic_operation>::iterator it;
    for(it=arithmetic_operations.begin();it<arithmetic_operations.end();it++) {
        dfile << "arithmetic_operation" << settings_name_value_separator << it->name << endl;
        dfile << "arithmetic_operation_priority" << settings_name_value_separator
         << it->priority << endl;
    }

    dfile.close();
}

void symbolic_computation::read_settings() {
    ifstream sfile(settings_filename.c_str());

    string t_str,t_str1,t_str2;
    const unsigned int BUFLEN=500;
    char* t_buf=new char[BUFLEN];

    //arithmetic_operations.clear();
    bool arithmetic_operations_cleared=false;
    //bool arithmetic_operations_name=false;
    arithmetic_operation t_operation;

    stringstream t_sstream;

    unsigned int t_pos;
    while(!sfile.eof()) {
        sfile.getline(t_buf,BUFLEN);
        t_str=t_buf;
        if(t_buf[0]!=settings_comment_symbol) {
            t_pos=t_str.find(settings_name_value_separator);
            if(t_pos!=string::npos) {
                t_str1=t_str.substr(0,t_pos);
                t_str2=t_str.substr(t_pos+1);
                cerr << t_str1 << "<=>" << t_str2 << endl;
                if(!t_str1.compare("set_all")) {
                    set_all=t_str2;
                }
                else if(!t_str1.compare("set_variables")) {
                    set_variables=t_str2;
                }
                else if(!t_str1.compare("set_numbers")) {
                    set_numbers=t_str2;
                }
                else if(!t_str1.compare("BR1")) {
                    BR1=t_str2;
                }
                else if(!t_str1.compare("BR2")) {
                    BR2=t_str2;
                }
                else if(!t_str1.compare("debug_level")) {
                    if(!t_str2.compare("DNONE")) debug_level=DNONE;
                    else if(!t_str2.compare("DALL")) debug_level=DALL;
                    else if(!t_str2.compare("DEASY")) debug_level=DEASY;
                }
                else if(!t_str1.compare("arithmetic_operation")) {
                    t_operation.name=t_str2;
                }
                else if(!t_str1.compare("arithmetic_operation_priority")) {
                    if(t_operation.name.compare("")) {
                        if(!arithmetic_operations_cleared) {
                            arithmetic_operations.clear();
                            arithmetic_operations_cleared=true;
                        }
                        t_sstream.clear();
                        t_sstream<<t_str2;
                        t_sstream>>t_operation.priority;
                        arithmetic_operations.push_back(t_operation);
                        t_operation.name="";
                    }
                }
            }
        }
    }
    delete[] t_buf;
    sfile.close();
}
