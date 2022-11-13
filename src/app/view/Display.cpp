#include "Display.h"
#include "ParamData.h"
#include <algorithm>

inline ResourceType getResourceType(std::string &s_rsrc) {
    static const std::unordered_map<std::string, ResourceType> _getResourceType = {
    #define X(name) {#name, name},
        RESOURCES
    #undef X
    };
    return _getResourceType.find(s_rsrc)->second;
}

void Display::drawBackground(ResourceType bg) {
    enum {Width = 256, Hight = 160};
    static const unsigned char* getResourceData[] = {
    #define X(name) [name] = name##_bits,
        RESOURCES
    #undef X
    };

    u8g2.drawXBM(0, 0, Width, Hight, getResourceData[bg]);
}

void Display::drawParamDatas(ResourceType bg, std::unordered_map<std::string, std::string> &data) {
    enum {MaxNumParams = 128};
    static const ParamData* getParamData[] = {
    #define X(name) [name] = name##_ParamDatas,
        RESOURCES
    #undef X
    };
    static const std::string _paramPrefix = "param_";

    char _itosBuf[5];
    const ParamData* paramDatas = getParamData[bg];
    for(int i = 0; i < MaxNumParams; i++) {
        auto param = _paramPrefix + itoa(i, _itosBuf, 10);
        auto it = data.find(param);
        if (it == data.end())
            break;

        u8g2.setFont(paramDatas[i].font);
        uint16_t x = paramDatas[i].x;
        uint16_t y = paramDatas[i].y;
        char str_c_tmp[2] = {0};
        int interval = paramDatas[i].interval;
        if ( interval >= 0) {
            for (auto _c = it->second.begin(); _c != it->second.end(); _c++) {
                str_c_tmp[0] = *_c;
                u8g2.drawStr(x, y, str_c_tmp);
                x += interval;
            }
        } else {
            for (auto _c = it->second.rbegin(); _c != it->second.rend(); _c++) {
                str_c_tmp[0] = *_c;
                u8g2.drawStr(x, y, str_c_tmp);
                x += interval;
            }
        }
    }
}


void Display::show(std::unordered_map<std::string, std::string> &data) {
    u8g2.clearBuffer();

    ResourceType background = getResourceType(data["state"]);

    drawBackground(background);

    drawParamDatas(background, data);

    u8g2.sendBuffer();
}