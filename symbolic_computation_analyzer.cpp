#include "symbolic_computation.h"
#include <vector>
#include <iostream>
using std::endl;
using std::cerr;
using std::vector;
using std::min;

symbolic_computation::lexer_answer symbolic_computation::lexer(string source) {
    stringtype_t current_type=SOTHER,previous_type=SOTHER;
    lexer_answer res;
    string current;
    token new_token;
    res.errors=false;
    for(unsigned int i=0;i<source.length();i++) {
        current=source.substr(i,1);
        current_type=stringtype(current);
        if(current_type==SOTHER) {
            if(debug_level==DALL) {
                cerr << "Lexer:[" << source << "]: wrong symbol [" << current << "] at " << i << endl;
                res.errors=true;
            }
        }
        else {
            //creating new token
            if(!i || i==(source.length()-1) || current_type==SBR1 ||
             current_type==SBR2 || current_type==SOPERATION || current_type!=previous_type) {
                new_token.content=current;
                new_token.type=current_type;
                res.tokens.push_back(new_token);
            }
            //adding content to last existing token
            else {
                res.tokens.back().content.append(current);
            }
        }
        previous_type=current_type;
    }
    return(res);
}

bool symbolic_computation::testbrackets(lexer_answer src,int pmin,int pmax) {
    if(pmax==-1) pmax=src.tokens.size()-1;
    int bc1=0,bc2=0;
    while(pmin<=pmax) {
        if(src.tokens[pmin].type==SBR1) bc1++;
        if(src.tokens[pmin].type==SBR2) bc2++;
        pmin++;
    }
    return(bc1==bc2);
}

int symbolic_computation::getlast
(lexer_answer src,int type,int pmin,int i,bool notinsidebrackets,bool nounary) {
    if(i==-1) i=src.tokens.size()-1;
    int bc=0;;
    while(i>=pmin) {
        if(src.tokens[i].type==SBR2) bc++;
        if(src.tokens[i].type==SBR1) bc--;
        if(src.tokens[i].type==type) {
            if((!notinsidebrackets)||(bc==0)) {
                if(!nounary) {
                    return(i);
                }
                else if(i>pmin) {
                    if(src.tokens[i-1].type!=SBR1&&src.tokens[i-1].type!=SOPERATION) return(i);
                }
            }
        }
        i--;
    }
    return(-1);
}

bool symbolic_computation::brackets_nospace(lexer_answer src,int left,int right) {
    if(src.tokens[left].type==SBR1&&src.tokens[right].type==SBR2) {
        int br=1;
        left++;
        while(left<right) {
            if(src.tokens[left].type==SBR1) br++;
            if(src.tokens[left].type==SBR2) br--;
            if(br==0) return(false);
            left++;
        }
    }
    else return(false);
    return(true);
}

unsigned int symbolic_computation::minopprio(lexer_answer src,int pmin,int pmax) {
    if(pmax==-1) pmax=src.tokens.size()-1;
    unsigned int minprio=100;
    int bc=0;
    while(pmax>=pmin) {
        if(src.tokens[pmax].type==SBR2) bc++;
        if(src.tokens[pmax].type==SBR1) bc--;
        if(src.tokens[pmax].type==SOPERATION) {
            if(!bc) minprio=min(minprio,op_prio(src.tokens[pmax].content));
        }
        pmax--;
    }
    return(minprio);
}

tree* symbolic_computation::parser_atomic(lexer_answer src,int pmin,int pmax) {
    if(pmin==pmax) return(new tree(src.tokens[pmin].content));
    return(NULL);
}

tree* symbolic_computation::parser_brackets(lexer_answer src,int pmin,int pmax) {
    if((pmax-pmin)>=2&&src.tokens[pmin].type==SBR1&&
     src.tokens[pmax].type==SBR2&&brackets_nospace(src,pmin,pmax)) {
        return(parser(src,pmin+1,pmax-1));
    }
    return(NULL);
}

tree* symbolic_computation::parser_unary(lexer_answer src,int pmin,int pmax) {
    if((pmax-pmin)>=1&&src.tokens[pmin].type==SOPERATION) {
        return(new tree(src.tokens[pmin].content,parser(src,pmin+1,pmax)));
    }
    return(NULL);
}

tree* symbolic_computation::parser_function(lexer_answer src,int pmin,int pmax) {
    if((pmax-pmin)>=3&&src.tokens[pmin].type==SVARIABLE&&src.tokens[pmin+1].type==SBR1
       &&src.tokens[pmax].type==SBR2&&brackets_nospace(src,pmin+1,pmax)) {
        return(new tree(src.tokens[pmin].content,parser(src,pmin+2,pmax-1)));
    }
    return(NULL);
}

tree* symbolic_computation::parser(lexer_answer src,int pmin,int pmax,bool binary_allowed) {
    if(pmax==-1) pmax=src.tokens.size()-1;
    if(pmax<pmin) return(NULL);
    int op_pos=getlast(src,SOPERATION,pmin,pmax,true,true);
    tree* r1=NULL,*r2=NULL;
    if(op_pos!=-1) {
        if(binary_allowed) {
            bool btest=false;
            while(!btest) {
                if(op_pos!=-1) {
                    if((r1=parser(src,pmin,op_pos-1,false))!=NULL) btest=true;
                    else if(minopprio(src,pmin,op_pos-1)>=op_prio(src.tokens[op_pos].content)) {
                        btest=true;
                        r1=parser(src,pmin,op_pos-1);
                    }
                    if(btest) {
                        if(!(r2=parser(src,op_pos+1,pmax,false))) r2=parser(src,op_pos+1,pmax);
                    }
                    else {
                        op_pos=getlast(src,SOPERATION,pmin,op_pos-1,true,true);
                        continue;
                    }
                    if(btest&&r1&&r2) return(new tree(r1,src.tokens[op_pos].content,r2));
                    else return(NULL);
                }
                else btest=true;
            }
        }
    }
    else {
        if((r1=parser_function(src,pmin,pmax))!=NULL) return(r1);
        if((r1=parser_unary(src,pmin,pmax))!=NULL) return(r1);
        if((r1=parser_brackets(src,pmin,pmax))!=NULL) return(r1);
        if((r1=parser_atomic(src,pmin,pmax))!=NULL) return(r1);
    }
    return(NULL);
}

tree* symbolic_computation::parse_string(string source) {
    lexer_answer l=lexer(source);
    if(!l.errors) {
        return(parser(l));
    }
    else return(NULL);
}
