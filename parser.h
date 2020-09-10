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
        /// <param name="name">Назва тегу</param>
        /// <param name="propn">Назва властивостi</param>
        /// <param name="propv">Значення властивостi</param>
        /// <returns>HtmlBlock знайденого тегу</returns>
        htmlblock get_block(str name, int index = 0, str propn = "\0", str propv = "\0");
        /// <returns>Тiло тегу</returns>
        str get_body();
        /// <summary>
        /// Вертаэ значення властивостi тегу
        /// </summary>
        /// <param name="blockname">Назва тегу</param>
        /// <param name="propname">Назва властивостi</param>
        /// <returns>Значення властивостi тегу</returns>
        str get_prop_value(str blockname, str propname);
    };
}