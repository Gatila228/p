#include "str"
#include <vector>
#include <stack>

namespace htmlparser {
    struct hproperty;
    struct elementinfo;
    class htmlblock {
    private:
        str body;
        hproperty get_property_info(str prop);
        elementinfo get_element_info(str text);
        std::vector<elementinfo> get_elements(str text);
        bool find_property(str n, str v, elementinfo e);
        str _get_block(str source, str name, str propn = "\0", str propv = "\0", int index = 0);
    public:
        htmlblock(str text);
        /// <param name="name">����� ����</param>
        /// <param name="propn">����� ����������i</param>
        /// <param name="propv">�������� ����������i</param>
        /// <returns>HtmlBlock ���������� ����</returns>
        htmlblock get_block(str name, int index = 0, str propn = "\0", str propv = "\0");
        /// <returns>�i�� ����</returns>
        str get_body();
        /// <summary>
        /// ������ �������� ����������i ����
        /// </summary>
        /// <param name="blockname">����� ����</param>
        /// <param name="propname">����� ����������i</param>
        /// <returns>�������� ����������i ����</returns>
        str get_prop_value(str blockname, str propname);
    };
}