#include "parser.h"

#ifndef PARSERLIB
#define PARSERLIB

namespace htmlparser {
    struct hproperty {
        str name;
        str value;
    };
    struct elementinfo {
        str type;
        std::vector<hproperty> properties;
        bool br = false;
    };
    hproperty htmlblock::get_property_info(str prop) {
        return hproperty{
            prop.between(0, prop.index('=')),
            prop.between('\"','\"')
        };
    }
    elementinfo htmlblock::get_element_info(str text) {
        elementinfo           si;
        std::vector<hproperty> prs;
        bool                  check = true;

        for(int i = 1; i < text.size(); i++) {
            if(text[i] == '\"') check = !check;
            if(!check) continue;
            if(text[i] == '/') {
                si.br = true;
                continue;
            }
            if(text[i] == '!') return elementinfo{ "comment", prs, false };
            else if(text[i] == ' ' || text[i] == '>') break;
            else si.type += text[i];
        }

        std::vector<str> props;

        for(int i = 0; i < text.size(); i++) {
            if(text[i] == '\"') check = !check;
            if(check) {
                if(text[i] == ' ') {
                    props.push_back("");
                    continue;
                }
                else if(text[i] == '>') {
                    break;
                }
            }
            if(props.size() > 0)
                props[props.size() - 1] += text[i];
        }

        for(int i = 0; i < props.size(); i++) {
            si.properties.push_back(get_property_info(props[i]));
        }

        return si;
    }
    std::vector<elementinfo> htmlblock::get_elements(str text) {
        std::vector<elementinfo>  ei;
        std::vector<str>         eis;
        std::stack<str>        count;
        bool                     add = false;
        bool                   check =  true;
        for(int i = 0; i < text.size(); i++) {
            if(text[i] == '\"') check = !check;
            if(text[i] == '<' && check) {
                eis.push_back("");
                add = true;
            }
            else if(text[i] == '>' && check) {
                add = false;
            }
            if(eis.size() > 0 && add) {
                eis[eis.size() - 1] += text[i];
            }
        }
        for(int i = 0; i < eis.size(); i++) {
            ei.push_back(get_element_info(eis[i]));
        }
        return ei;
    }
    bool htmlblock::find_property(str n, str v, elementinfo e) {
        bool nn = n == "\0";
        bool vn = v == "\0";
        if(nn && vn) return true;
        for(int i = 0; i < e.properties.size(); i++) {
            bool ng = (nn || e.properties[i].name == n);
            bool vg = (vn || e.properties[i].value == v);

            if(vg && ng) return true;
        }
        return false;
    }
    str htmlblock::_get_block(str source, str name, str propn, str propv, int index) {
        int    count = 0;
        int   bCount = -1;
        bool   bOpen = false;
        bool     add = false;
        bool   check = true;
        bool founded = false;
        str     temp = "";
        auto      el = get_elements(source);

        if(index > 1) index = (index * 2) - 1;
        for(int i = 0, j = -1; i < source.size(); i++) {
            if(source[i] == '\"')
                check = !check;

            if(source[i] == '<' && check) {
                j++;
                if(el[j].type != "br" || el[j].type != "meta") {
                    count += !el[j].br ? 1 : -1;

                    if(el[j].type == name && find_property(propn, propv, el[j])) {
                        if(index > 0) {
                            index--;
                        }
                        else if(!el[j].br) {
                            bCount = count;
                            bOpen = true;
                        }
                    }

                    if(add && count == bCount - 1 && index == 0)
                        break;
                }
            }

            add = (add || (bOpen && source[i] == '>' && check));

            if(add)
                temp += source[i];
        }
        return temp.cut(0, 0);
    }
    htmlblock::htmlblock(str text) {
        body = text.remove("\n").remove("\t");
    }
    htmlblock htmlblock::get_block(str name, int index, str propn, str propv) {
        return htmlblock(_get_block(body, name, propn, propv, index));
    }
    str htmlblock::get_body() { return body; }
    str htmlblock::get_prop_value(str blockname, str propname) {
        auto  elements = get_elements(body);

        for(int i = 0; i < elements.size(); i++)
            if(elements[i].type == blockname)
                for(int j = 0; j < elements[i].properties.size(); j++)
                    if(elements[i].properties[j].name == propname)
                        return elements[i].properties[j].value;
        return "";
    }
}
#endif