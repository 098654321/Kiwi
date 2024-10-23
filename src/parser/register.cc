#include "register.hh"
#include <assert.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>

namespace kiwi::parser{


    
Array<String, 4> convertToHexString(const Array<int, COB::INDEX_SIZE>& Reg) {
    std::stringstream hexString;

    for (size_t i = 0; i < 32; ++i) {
        hexString << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(Reg[i * 4]) 
                  << static_cast<int>(Reg[i * 4 + 1]) 
                  << static_cast<int>(Reg[i * 4 + 2]) 
                  << static_cast<int>(Reg[i * 4 + 3]);
        if (i < 31) {
            hexString << " "; // 添加空格分隔
        }
    }

    String fullHex = hexString.str();
    std::array<std::string, 4> hexParts;

    // 拆分成4个8位字符串
    for (size_t i = 0; i < 4; ++i) {
        hexParts[i] = fullHex.substr(i * 8, 8);
        reverse(hexParts[i].begin(), hexParts[i].end());
    }

    return std::move(hexParts);
}

Array<String, 4> convertToHexString(const Vector<int>& Reg) {
    assert(Reg.size() == 128);
    std::stringstream hexString;

    for (size_t i = 0; i < 32; ++i) {
        hexString << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(Reg[i * 4]) 
                  << static_cast<int>(Reg[i * 4 + 1]) 
                  << static_cast<int>(Reg[i * 4 + 2]) 
                  << static_cast<int>(Reg[i * 4 + 3]);
        if (i < 31) {
            hexString << " "; // 添加空格分隔
        }
    }

    String fullHex = hexString.str();
    std::array<std::string, 4> hexParts;

    // 拆分成4个8位字符串
    for (size_t i = 0; i < 4; ++i) {
        hexParts[i] = fullHex.substr(i * 8, 8);
        reverse(hexParts[i].begin(), hexParts[i].end());
    }

    return std::move(hexParts);
}

std::string toHexString(uint32_t num) {
    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << num;
    return ss.str();
}

std::vector<std::string> convertmuxtoHex(const std::vector<int>& vec) {
    std::vector<std::string> result;
    int size = vec.size();
    
    // 遍历每8个数为一组
    for (int i = 0; i < size; i += 8) {
        uint32_t combined = 0;
        
        // 将每个数的3位二进制拼接到combined中
        for (int j = 0; j < 8; ++j) {
            combined <<= 3;  // 左移3位
            combined |= (vec[i + j] & 0x7);  // 保留低3位
        }
        
        // 将24位二进制数变成32位
        combined <<= 8;  // 高位填充0，使其成为32位
        
        // 转换为十六进制字符串
        result.push_back(toHexString(combined));
    }

    // 反转高位和低位
    for (auto& s: result)
    {
        reverse(s.begin(), s.end());
    }
    
    return result;
}


// 将std::vector<int>（含0和1的64位向量）拆成两个32位的无符号整数，并返回对应的16进制字符串
std::pair<std::string, std::string> convertBankSelToHex(const std::vector<int>& vec) {
    if (vec.size() != 64) {
        throw std::invalid_argument("The input vector must have exactly 64 elements.");
    }
    
    uint32_t highPart = 0;  // 存储前32位
    uint32_t lowPart = 0;   // 存储后32位
    
    // 处理前32位
    for (int i = 0; i < 32; ++i) {
        highPart <<= 1;               // 左移1位
        highPart |= (vec[i] & 0x1);   // 只取0或1的值
    }
    
    // 处理后32位
    for (int i = 32; i < 64; ++i) {
        lowPart <<= 1;                // 左移1位
        lowPart |= (vec[i] & 0x1);    // 只取0或1的值
    }
    
    // 转换为十六进制字符串
    std::string highHex = toHexString(highPart);
    std::string lowHex = toHexString(lowPart);
    
    // 反转高位和低位
    reverse(highHex.begin(), highHex.end());
    reverse(lowHex.begin(), lowHex.end());

    return {highHex, lowHex};
}

    void Register::outputCobSwInfo(
        const Vector<String>& names, COB* pcob, Array<int, 128>& Reg, ofstream& file, std::i64 row, std::i64 col
    )
    {
        Array<COBSwState, COB::INDEX_SIZE> swReg;
        if (names[1] == "ru")
            swReg = pcob->all_sw_reg_value(COBDirection::Right, COBDirection::Up);
        else if (names[1] == "lu")
            swReg = pcob->all_sw_reg_value(COBDirection::Left, COBDirection::Up);
        else if (names[1] == "rd")
            swReg = pcob->all_sw_reg_value(COBDirection::Right, COBDirection::Down);
        else if (names[1] == "ld")
            swReg = pcob->all_sw_reg_value(COBDirection::Left, COBDirection::Down);
        else if (names[1] == "v")
            swReg = pcob->all_sw_reg_value(COBDirection::Up, COBDirection::Down);
        else if (names[1] == "h")
            swReg = pcob->all_sw_reg_value(COBDirection::Left, COBDirection::Right);   
        for (size_t i = 0; i < COB::INDEX_SIZE; ++i)
        {
            if (swReg[i] == hardware::COBSwState::DisConnected)
                Reg[i] = 0;
            else
                Reg[i] = 1;
        }
        Array<String, 4> reginfo = convertToHexString(Reg);
        for (size_t i = 0; i < 4; ++i)
        {
            file << reginfo[i] << " cob_" << row << "_" << col << "_sw_" << names[1] << "_" << i << std::endl;
        }
    }   

    void Register::outputCobPortInfo(
        const Vector<String>& names, COB* pcob, Array<int, 128>& Reg, ofstream& file, std::i64 row, std::i64 col
    )
    {
        Array<COBSelRegister, COB::INDEX_SIZE> selReg;
        if (names[0] == "right")
            selReg = pcob->all_sel_reg_value(COBDirection::Right);
        if (names[0] == "left")
            selReg = pcob->all_sel_reg_value(COBDirection::Left);
        if (names[0] == "up")
            selReg = pcob->all_sel_reg_value(COBDirection::Up);
        if (names[0] == "down")
            selReg = pcob->all_sel_reg_value(COBDirection::Down);
        for (size_t i = 0; i < COB::INDEX_SIZE; ++i)
        {
            if (selReg[i].get() == hardware::COBSignalDirection::TrackToCOB)
                Reg[i] = 0;
            else
                Reg[i] = 1;
        }
        Array<String, 4> reginfo = convertToHexString(Reg);
        for (size_t i = 0; i < 4; ++i)
        {
            file << reginfo[i] << " cob_" << row << "_" << col << "_" << names[0] << "_" << "sel_" << i << std::endl;
        }
    }

    void Register::outputTobhvMuxInfo(
        Vector<int>& Reg, TOB* ptob, const String& message
    )
    {
        Reg = ptob->all_bump_to_hori_reg();
        Vector<String> result = convertmuxtoHex(Reg);
        for(size_t i = 0; i < 16; ++i)
        {
            String controlbits = result[i];
            std::cout << controlbits << " " << message << i/8 << i%8 << std::endl;
            
        }
    }

    void Register::outputTobtrackMuxInfo(
        Vector<int>& Reg, TOB* ptob, const String& message
    )
    {
        Reg = ptob->all_vert_to_track_reg();
        std::pair<String, String> result = convertBankSelToHex(Reg);
        std::cout << result.first << " " << message << "_0" << std::endl;
        std::cout << result.second << " " << message << "_1" << std::endl;
    }

    void Register::outputTobBumpInfo(
        Vector<int>& Reg, TOB* ptob, const String& message, bool reverse
    )
    {
        Reg = ptob->all_bump_dir_reg();
        if (reverse)
        {
            for (auto& r: Reg)
            {
                if (r)
                    r = 0;
                else 
                    r = 1;
            }
        }
        std::Array<String, 4> result = convertToHexString(Reg);
        for (size_t j = 0; j < 4; ++j)
        {
            std::cout << result[j] << " " << message << j/2 << "_en_" << j%2 << std::endl;
        }
    }

    void Register::outputTobTrackInfo(
        Vector<int>& Reg, TOB* ptob, const String& message, bool reverse
    )
    {
        Reg = ptob->all_track_dir_reg();
        if (reverse)
        {
            for (auto& r: Reg)
            {
                if (r)
                    r = 0;
                else
                    r = 1;
            }
        }
        std::Array<String, 4> result = convertToHexString(Reg);
        for (size_t j = 0; j < 4; ++j)
        {
            std::cout << result[j] << " " << message << j << std::endl;
        }
    }

}



