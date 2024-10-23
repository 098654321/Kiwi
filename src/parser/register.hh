#pragma once

#include "global/std/utility.hh"
#include "global/std/string.hh"
#include "hardware/interposer.hh"


namespace kiwi::parser{

    enum CobPortDirec
    {
        Right,
        Left,
        Down,
        Up
    };

    enum CobSwDirec
    {
        RU,
        LU, 
        RD, 
        LD, 
        V, 
        H
    };

    using namespace std;
    using namespace kiwi::hardware;

    //需要输出的寄存器，如果寄存器有改动直接在这里改名称然后添加新的解析方法
    const String cobRegName[] = {
        "right_sel", "left_sel", "down_sel", "up_sel"
        "sw_ru", "sw_lu", "sw_rd", "sw_ld", "sw_v", "sw_h"
        };
    const String tobRegName[] = {
        "tob2bump", "hctrl", "vctrl", "bank_sel", "tob2track", "bump2tob", "track2tob"
    };
    
    class Register{
    public:

        void outputCobPortInfo(
            const Vector<String>& names, COB* pcob, Array<int, 128>& Reg, ofstream& file, std::i64 row, std::i64 col
        );
        void outputCobSwInfo(
            const Vector<String>& names, COB* pcob, Array<int, 128>& Reg, ofstream& file, std::i64 row, std::i64 col
        );
        void outputTobhvMuxInfo(
            Vector<int>& Reg, TOB* ptob, const String& message
            );
        void outputTobtrackMuxInfo(
            Vector<int>& Reg, TOB* ptob, const String& message
        );
        void outputTobBumpInfo(
            Vector<int>& Reg, TOB* ptob, const String& message, bool reverse = false
        );
        void outputTobTrackInfo(
            Vector<int>& Reg, TOB* ptob, const String& message, bool reverse = false
        );
    };

}