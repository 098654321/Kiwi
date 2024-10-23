#include "parser.hh"
#include "../circuit/basedie.cc"
#include <iostream>
#include <sstream>
#include <xlnt/xlnt.hpp>
#include <regex>

#define DEBUG_firstshee
#define DEBUG_secondsheet
#define DEBUG_thirdshee
#define DEBUG


namespace kiwi::parser{

enum NodeType{ 
    aBump,
    aTrack
};

const int Parser::ExtIOIndex[] = {0, 9, 18, 27, 36, 45, 54, 63, 120, 113, 106, 99, 92, 85, 78, 71};

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    
    return result;
}


int extractBumpIndex(const String& bumpName)
{
    // 根据 bump 名称计算 bump 索引
    std::regex pattern(R"(bump_bank(\d+)_(\d+))");
    std::smatch matches;

    if (std::regex_search(bumpName, matches, pattern)) {
        return 64 * std::stoi(matches[1]) + std::stoi(matches[2]);
    } else {
        std::cerr << "Bump Name Error in sheet 0" << std::endl;
        exit(1);
    }
}

void extractIOcoor(const String& IOcoorstring, Array<int, 4>& coor)
{
    // 根据 bump 名称计算 bump 索引
    std::regex pattern(R"((\d+) (\d+) (\d+))");
    std::smatch matches;

    if (std::regex_search(IOcoorstring, matches, pattern)) {
        coor[0] = std::stoi(matches[1]);
        coor[1] = std::stoi(matches[2]);
        coor[2] = std::stoi(matches[3]);
    } else {
        std::cerr << "IO coordinate Error in sheet 2, cannot math the regular expression((\\d+) (\\d+) (\\d+))" << std::endl;
        exit(1);
    }
}


Option<Bump*> constructBump(const Vector<String>& split, const HashMap<String, usize>& SignalPinMap, Interposer& interposer)
{
    int tobnum = std::stoi(split[1]);
    int tobx = tobnum / 4;
    int toby = tobnum % 4;

    // get signal name & bump index
    String signalName("");
    for (int splitindex = 2; splitindex < split.size(); ++splitindex)
    {
        signalName += split[splitindex];
        if (splitindex < split.size() - 1)
            signalName += "_";
    }
    auto cite_bumpIndex = SignalPinMap.find(signalName);
    if (cite_bumpIndex == SignalPinMap.end())
    {
        std::cerr << "muyan_" << tobnum << " signal name error: " << signalName << std::endl;
        return std::nullopt;
    }
    int bumpindex = cite_bumpIndex->second;
    Coord coord(tobx, toby);
    auto ptob = interposer.get_tob(coord);
    if (ptob.has_value())
    {
        Bump* pbump = new Bump(coord, bumpindex, ptob.value());
        return pbump;
    }
    else{
        std::cerr << "No tob at (" << tobx << ", " << toby << ")" << std::endl;
        return std::nullopt;
    }
}


Option<Track*> constructTrack(
    const String& name, const Vector<String>& splitName, const HashMap<String, Array<int, 4>>& IOcoorMap, Interposer& interposer, 
    Vector<Track*>& ptracks
    )
{
#ifdef DEBUG_secondsheet
std::cout << "name: " << name << std::endl;
#endif
    if (splitName[1] != "nege" && splitName[1] != "pose")
    {
#ifdef DEBUG_secondsheet
std::cout << "a normal track" << std::endl;
#endif
        Array<int, 4> coor = IOcoorMap.find(name)->second;
        TrackDirection td = coor[2] == 0 ? TrackDirection::Horizontal : TrackDirection::Vertical;
        // 还要根据所在边调整坐标，因为 tarck 在外面
        std::i64 x = coor[1], y = coor[2];
        if (td == TrackDirection::Horizontal)
        {
            if (y == Interposer::COB_ARRAY_WIDTH - 1)
            {
                y++;
            }
        }
        if (td == TrackDirection::Vertical)
        {
            if (x == Interposer::COB_ARRAY_HEIGHT - 1)
            {
                x++;
            }
        }
        Track* ptrack = new Track(x, y, td, coor[3]);
#ifdef DEBUG_secondsheet
std::cout << "got a track" << std::endl;
std::string direc = ptrack->coord().dir == TrackDirection::Vertical ? "V" : "H";
std::cout << "x: " << ptrack->coord().row << ", y: " << ptrack->coord().col << ", direction: " << direc << ", index: " << ptrack->coord().index << std::endl;
#endif
        return std::make_optional(ptrack);
    }
    else if (splitName[1] == "nege" || splitName[1] == "pose")
    {
#ifdef DEBUG_secondsheet
std::cout << "POWER/GND track" << std::endl;
#endif
        // 检查边缘的 track 有哪些是空的
        for (std::i64 row = 0; row < Interposer::COB_ARRAY_HEIGHT; row++)
        {
            std::i64 col = 0;
            for (size_t index = 0; index < COB::INDEX_SIZE; ++index)
            {
                Track* ptrack = new Track(row, col, TrackDirection::Horizontal, index);
                if (interposer.is_idle_tracks(ptrack))
                {
                    ptracks.emplace_back(ptrack);
                    return std::nullopt;
                }
                else{
                    delete ptrack;
                }
            }
            col = Interposer::COB_ARRAY_WIDTH;
            for (size_t index = 0; index < COB::INDEX_SIZE; ++index)
            {
                Track* ptrack = new Track(row, col, TrackDirection::Horizontal, index);
                if (interposer.is_idle_tracks(ptrack))
                {
                    ptracks.emplace_back(ptrack);
                    return std::nullopt;
                }
                else{
                    delete ptrack;
                }
            }
        }
        for (std::i64 col = 0; col < Interposer::COB_ARRAY_WIDTH; ++col)
        {
            std::i64 row = 0;
            for (size_t index = 0; index < COB::INDEX_SIZE; ++index)
            {
                Track* ptrack = new Track(row, col, TrackDirection::Horizontal, index);
                if (interposer.is_idle_tracks(ptrack))
                {
                    ptracks.emplace_back(ptrack);
                    return std::nullopt;
                }
                else{
                    delete ptrack;
                }
            }
            row = Interposer::COB_ARRAY_WIDTH;
            for (size_t index = 0; index < COB::INDEX_SIZE; ++index)
            {
                Track* ptrack = new Track(row, col, TrackDirection::Horizontal, index);
                if (interposer.is_idle_tracks(ptrack))
                {
                    ptracks.emplace_back(ptrack);
                    return std::nullopt;
                }
                else{
                    delete ptrack;
                }
            }
        }
        std::cerr << "find idle I/O track failed" << std::endl;
        return std::nullopt;
    }
    else{
        std::cerr << "xinzhai signal name error: " << name << std::endl;
        exit(4);
    }
}

bool construct_None_PG_Net(
    const String& input, const String& output, Box<Net>& pnet, 
    const HashMap<String, usize>& signalPinMap, const HashMap<String, Array<int, 4>>& IOcoorMap,
    Interposer& interposer, Vector<Bump*>& PGbumps, Vector<Track*>& PGtracks
    )
{
    Vector<String> splitIn = splitString(input, '_');
    Vector<String> splitOut = splitString(output, '_');
    // bump 的坐标按照 tob 来算
    if (splitIn[0] == "xinzhai" && splitOut[0] == "muyan")
    {
#ifdef DEBUG_secondsheet
std::cout << "constructing begin track & end bump" << std::endl;
#endif
        size_t prev_len = PGtracks.size();
        Option<Bump*> pbump = constructBump(splitOut, signalPinMap, interposer);
        Option<Track*> ptrack = constructTrack(input, splitIn, IOcoorMap, interposer, PGtracks);
#ifdef DEBUG_secondsheet
std::cout << "constructing ended" << std::endl;
#endif
        if (pbump.has_value() && ptrack.has_value())
        {
#ifdef DEBUG_secondsheet
std::cout << "got a normal TrackToBumpNet" << std::endl;
#endif
            TrackToBumpNet pttb(ptrack.value(), pbump.value());
            pnet = std::make_unique<TrackToBumpNet>(pttb);
        }
        else{
            if (!ptrack.has_value())
            {
                if (PGtracks.size() == prev_len)    // 没有成功生成新的 I/O track
                    std::cerr << "construct track failed: " << input << std::endl;
                else if (PGtracks.size() < prev_len)
                    std::cerr << "error behaviour, for PGtracks shrinks after constructing a new track" << std::endl;
                else{
#ifdef DEBUG_secondsheet
std::cout << "got a POWER/GND net" << std::endl;
#endif
                    PGbumps.emplace_back(pbump.value());
                }
            }
                
            if (!pbump.has_value())
            {
                std::cerr << "construct bump failed: " << output << std::endl;
                return false;
            }
        }
    }
    else if (splitIn[0] == "muyan" && splitOut[0] == "muyan")
    {
#ifdef DEBUG_secondsheet
std::cout << "constructing begin bump & end bump" << std::endl;
#endif
        Option<Bump*> pbump1 = constructBump(splitIn, signalPinMap, interposer);
        Option<Bump*> pbump2 = constructBump(splitOut, signalPinMap, interposer);
#ifdef DEBUG_secondsheet
std::cout << "constructing ended" << std::endl;
#endif
        if (pbump1.has_value() && pbump2.has_value())
        {
#ifdef DEBUG_secondsheet
std::cout << "got a normal BumpToBumpNet" << std::endl;
#endif
            BumpToBumpNet pbtb(pbump1.value(), pbump2.value());
            pnet = std::make_unique<BumpToBumpNet>(pbtb);
        }
        else{
            if (!pbump1.has_value())
                std::cerr << "construct input bump failed: " << input << std::endl;
            if (!pbump2.has_value())
                std::cerr << "construct output bump failed: " << output << std::endl;
            return false;
        }
    }
    else if (splitIn[0] == "muyan" && splitOut[0] == "xinzhai")
    {
#ifdef DEBUG_secondsheet
std::cout << "constructing begin bump & end track" << std::endl;
#endif
        size_t prev_len = PGtracks.size();
        Option<Bump*> pbump = constructBump(splitIn, signalPinMap, interposer);
        Option<Track*> ptrack = constructTrack(output, splitOut, IOcoorMap, interposer, PGtracks);
#ifdef DEBUG_secondsheet
std::cout << "constructing ended" << std::endl;
#endif
        if (pbump.has_value() && ptrack.has_value())
        {
#ifdef DEBUG_secondsheet
std::cout << "got a bumptotracknet" << std::endl;
#endif
            BumpToTrackNet pbtt(pbump.value(), ptrack.value());
            pnet = std::make_unique<BumpToTrackNet>(pbtt);
        }
        else{
            if (!ptrack.has_value())
            {
                if (PGtracks.size() == prev_len)    // 没有成功生成新的 I/O track
                    std::cerr << "construct track failed: " << input << std::endl;
                else if (PGtracks.size() < prev_len)
                    std::cerr << "error behaviour, for PGtracks shrinks after constructing a new track" << std::endl;
                else{
#ifdef DEBUG_secondsheet
std::cout << "A POWER/GND net" << std::endl;
#endif
                    PGbumps.emplace_back(pbump.value());
                }
            }
                
            if (!pbump.has_value())
            {
                std::cerr << "construct bump failed: " << output << std::endl;
                return false;
            }
        }
    }
    else{
        std::cerr << "net type error!" << std::endl;
        return false;
    }
    return true;
}


bool constructTopdie(
    const String& input, HashMap<String, TopDie>& topdies, const HashMap<String, usize>& SignalPinMap
    )
{
    Vector<String> splitIn = splitString(input, '_');
    if (splitIn[0] == "muyan")
    {
        String tobName = "muyan_" + splitIn[1];
        topdies.emplace(tobName, TopDie(tobName));
        String signalname;
        for(int signalindex = 2; signalindex < splitIn.size(); ++signalindex)
        {
            signalname += splitIn[signalindex];
            if (signalindex != splitIn.size())
                signalname += "_";
        }
        auto ite = topdies.find(tobName);
        ite->second.add_pin(signalname, SignalPinMap.find(signalname)->second);
        return true;
    }
    return false;
}


bool constructTopdieInst(
    const String& input, HashMap<String, TopDie>& topdies, Interposer& interposer, 
    HashMap<String, TopDieInstance>& topdie_insts
)
{
    Vector<String> splitIn = splitString(input, '_');

    if (splitIn[0] == "muyan")
    {
        String topdieName = "muyan_" + splitIn[1];
        TopDie* ptd = &(topdies.find(topdieName)->second);
        int tob_x = std::stoi(splitIn[1])/4;
        int tob_y = std::stoi(splitIn[1])%4;
        TOB* ptdi = (interposer.get_tob(tob_x, tob_y)).value();
        TopDieInstance topdieinst(topdieName, ptd, ptdi);
        topdie_insts.emplace(topdieName, topdieinst);
        return true;
    }
    return false;
}


void constructSynsNet(HashMap<int, std::Vector<std::Box<Net>>>& tempNets, BaseDie& basedie)
{
    // 假设一组同步线要么全都到bump，要么全都到track
    Box<Net> pnet;
    std::Vector<std::Box<Net>> basedieNet;
    for(
        HashMap<int, std::Vector<std::Box<Net>>>::iterator ite = tempNets.begin(); 
        ite != tempNets.end(); ++ite
        )
    {
        if (ite->first > 0) //同步线
        {
            //同步线
            switch((ite->second)[0]->getNettype())
            {
                case SingleNETtype::BBnet: {
#ifdef DEBUG_secondsheet
std::cout << "BumpToBumpSyncNet" << std::endl;
#endif
                    Vector<BumpToBumpNet> vbbNet;
                    for (auto& box_net: ite->second)
                    {
                        Net* temppnet = &(*box_net);
                        BumpToBumpNet* temppbbnet = dynamic_cast<BumpToBumpNet*>(temppnet);
                        vbbNet.emplace_back(*temppbbnet);
                    }
                    BumpToBumpSyncNet bbsyncNet(vbbNet);
                    pnet = std::make_unique<BumpToBumpSyncNet>(bbsyncNet);
                    break;
#ifdef DEBUG_secondsheet
std::cout << "constructing bumptobumpsyncnet ended" << std::endl;
#endif
                }
                case SingleNETtype::Btnet: {
#ifdef DEBUG_secondsheet
std::cout << "BumpToTrackSyncNet" << std::endl;
#endif
                    Vector<BumpToTrackNet> vbtNet;
                    for (auto& box_net: ite->second)
                    {
                        Net* temppnet = &(*box_net);
                        BumpToTrackNet* temppbtnet = dynamic_cast<BumpToTrackNet*>(temppnet);
                        vbtNet.emplace_back(*temppbtnet);
                    }
                    BumpToTrackSyncNet btsyncnet(vbtNet);
                    pnet = std::make_unique<BumpToTrackSyncNet>(btsyncnet);
                    break;
#ifdef DEBUG_secondsheet
std::cout << "constructing bumptotracksyncnet ended" << std::endl;
#endif
                }
                case SingleNETtype::TBnet: {
#ifdef DEBUG_secondsheet
std::cout << "TrackToBumpSyncNet" << std::endl;
#endif
                    Vector<TrackToBumpNet> vtbNet;
                    for (auto& box_net: ite->second)
                    {
                        Net* temppnet = &(*box_net);
                        TrackToBumpNet* tempptbnet = dynamic_cast<TrackToBumpNet*>(temppnet);
                        if (tempptbnet == NULL)
                            continue;
                        vtbNet.emplace_back(*tempptbnet);
                    }
#ifdef DEBUG_secondsheet
std::cout << "construct SyncNet using vector vtbNet" << std::endl;
#endif
                    TrackToBumpSyncNet tbsyncnet(vtbNet);
                    pnet = std::make_unique<TrackToBumpSyncNet>(tbsyncnet);
                    break;
#ifdef DEBUG_secondsheet
std::cout << "constructing TrackToBumpSyncNet ended" << std::endl;
#endif
                }
                default:
                    std::cerr << "single net type error, not in the range of [bump2bump, bump2track, track2bump]" << std::endl;
                    exit(4);
            }
            basedieNet.emplace_back(std::move(pnet));
        }
        else if (ite->first < 0)
        {
            // 非同步线
            for (auto& box_net: ite->second)
            {
                basedieNet.push_back(std::move(box_net));
            }
        }
        else
        {
            std::cerr << "group number error: " << ite->first << std::endl;
            exit(4);
        }
    }
    basedie.set_net(basedieNet);
}


void testInput(BaseDie& basedie)
{
    ofstream outputfile("./testinput.txt");
    if (!outputfile)
    {
        std::cerr << "testinput.txt open failed" << std::endl;
        exit(1);
    }

    outputfile << "Nets:" << std::endl;
    for (auto& box_net: basedie.get_nets())
    {
        if (box_net != NULL) 
        {
            Net* pnet = box_net.get();
            pnet->checkNetInfo(outputfile);
        }
    }
    
    outputfile << "TopDies: " << std::endl;
    for (auto& td: basedie.get_topdies())
    {
        td.second.checkInfo(outputfile);
    }

    outputfile << "TopDie instances:" << std::endl;

}


void Parser::inputParse(const String& fileName, BaseDie& basedie, Interposer& interposer)
{
    xlnt::workbook wb;
    HashMap<String, usize> SignalPinMap;
    HashMap<String, Array<int, 4>> IOcoorMap; 
    auto& topdies = basedie.get_topdies();
    auto& topdie_insts = basedie.get_topdieinsts();

    // 检测打开失败
    try{
        wb.load(fileName);
        std::cout << "Workbook loaded successfully " << std::endl; 
    } catch(const std::exception& e)
    {
        std::cerr << "Failed to load workbook: " << e.what() << std::endl;
        exit(2);
    }

    //读第一个 sheet ，获取信号名称有 bump 索引的对应关系
    std::cout << "read the first sheet" << std::endl;
    xlnt::worksheet ws1 = wb.sheet_by_index(0);
    for(size_t row = 2; row < ws1.highest_row(); row++)
    {
        if (ws1.cell(xlnt::cell_reference(2, row)).has_value())
        {
            String bumpName = ws1.cell(xlnt::cell_reference(1, row)).to_string();
            String SignalName = ws1.cell(xlnt::cell_reference(2, row)).to_string();
            int bumpindex = extractBumpIndex(bumpName);
            SignalPinMap[SignalName] = bumpindex;
        }
    }

#ifdef DEBUG_firstsheet
std::cout << "SignalPinMap" << std::endl;
for (auto& s: SignalPinMap)
{
    std::cout << s.first << ", index=" << s.second << std::endl;
}
std::cout << std::endl;
#endif
    
    //读第三个 sheet ，获取 IO 信号的坐标信息
    std::cout << "read the third sheet" << std::endl;
    xlnt::worksheet ws3 = wb.sheet_by_index(2);
    for (const auto& row : ws3.rows())
    {
        if (row[0].to_string() == "")
            continue;
        String IOname = row[0].to_string();
        String IOcoorstring = row[1].to_string();
        Array<int, 4> coor;
        extractIOcoor(IOcoorstring, coor);
        coor[3] = ExtIOIndex[std::stoi(row[2].to_string())];
        IOcoorMap.emplace(IOname, coor);
    }
#ifdef DEBUG_thirdsheet
std::cout << "IOcoorMap" << std::endl;
for (auto& io: IOcoorMap)
{
    std::cout << io.first << std::endl;
    std::cout << "Coor: ";
    for (auto& c: io.second)
    {
        std::cout << c << ", ";
    }
    std::cout << std::endl;
}
#endif

    //读第二个 sheet ，建立 nets
    std::cout << "read the second sheet" << std::endl;
    xlnt::worksheet ws2 = wb.sheet_by_index(1);
    HashMap<int, std::Vector<std::Box<Net>>> tempNets;
    Vector<Bump*> PGbumps;      // 等所有线网都构造结束，再构造 POWRE/GND
    Vector<Track*> PGtracks;
    for (size_t row = 2; row < ws2.highest_row(); row++)
    {
#ifdef DEBUG_secondsheet
std::cout << "reading rows " << row << std::endl;
#endif
        // 跳过空行
        if (ws2.cell(xlnt::cell_reference(1, row)).to_string() == "")
            continue;
        int groupnum = std::stoi(ws2.cell(xlnt::cell_reference(3, row)).to_string());
        Box<Net> pnet;

        String input = ws2.cell(xlnt::cell_reference(1, row)).to_string();
        String output = ws2.cell(xlnt::cell_reference(2, row)).to_string();
        bool constructSucceed = construct_None_PG_Net(
            input, output, pnet, SignalPinMap, IOcoorMap, interposer, PGbumps, PGtracks
            );
        if (!constructSucceed)
        {
            std::cerr << "construct net failed in row " << row << std::endl;
            exit(2);
        }
        if (tempNets.find(groupnum) == tempNets.end())
        {
#ifdef DEBUG_secondsheet
std::cout << "A new group of nets, with groupnum = " << groupnum << std::endl;
#endif
            Vector<Box<Net>> vnet;
            vnet.push_back(std::move(pnet));
            for (auto& box_net: vnet)
            {
                tempNets[groupnum].emplace_back(std::move(box_net));
            }
        }
        else{
#ifdef DEBUG_secondsheet
std::cout << "add a net to group: " << groupnum << std::endl;
#endif
            tempNets[groupnum].emplace_back(std::move(pnet));
        }

        // 构造 Topdie
        constructTopdie(input, topdies, SignalPinMap);
        constructTopdie(output, topdies, SignalPinMap);

        // 构造 Topdie_inst
        constructTopdieInst(input, topdies, interposer, topdie_insts);
        constructTopdieInst(output, topdies, interposer, topdie_insts);

    }

    std::cout << "creating POWER/GND nets" << std::endl;
    //构造 POWER/GND 线
    int min_group = 0;
    for (HashMap<int, std::Vector<std::Box<Net>>>::const_iterator con_ite = tempNets.begin(); con_ite != tempNets.end(); ++con_ite)
    {
        min_group = con_ite->first < min_group ? con_ite->first : min_group;
    }
    tempNets[min_group-1].emplace_back(std::make_unique<TracksToBumpsNet>(TracksToBumpsNet(PGtracks, PGbumps)));

    std::cout << "creating synchronized nets" << std::endl;
    // 整理 tempNets 里面的信息，构造同步线
    constructSynsNet(tempNets, basedie);

    std::cout << "distributing nets to topdie instances" << std::endl;
    // 补上 topdie_inst 里面的 net
    for (const auto& net: basedie.get_nets())
    {
        for (const auto& coor: net->coords())
        {
            int muyan_index = 4*coor.row+coor.col;
            String muyan_name = "muyan_" + std::to_string(muyan_index);
#ifdef DEBUG_secondsheet
std::cout << "got topdie instance: " << muyan_name << std::endl;
#endif
            auto ite = basedie.get_topdieinsts().find(muyan_name);
#ifdef DEBUG_secondsheet
std::cout << "adding net to topdie instance" << std::endl;
#endif
            Net* ptempnet = net.get();
            ite->second.add_net(ptempnet);
#ifdef DEBUG_secondsheet
std::cout << "addind net ended" << std::endl;
#endif
        }
    }

#ifdef DEBUG
std::cout << "checking nets" << std::endl;
testInput(basedie);
#endif

}


void Parser::output(
    const String& filename, Interposer& interposer, const std::Array<const String, 10> cobRegName,
    const std::Array<const String, 6> tobRegName
    )
{
    ofstream file(filename);
    if (!file)
    {
        std::cerr << "open file " << filename << "failed" << std::endl;
        exit(6);
    }

    // output cob controlbits
    for (std::i64 row = 0; row < Interposer::COB_ARRAY_HEIGHT; ++row)
    {
        for (std::i64 col = 0; col < Interposer::COB_ARRAY_WIDTH; ++col)
        {
            Option<COB*> opcob = interposer.get_cob(row, col);
            if (!opcob.has_value())
            {
                std::cerr << "no cob at position: " << row << ", " << col << std::endl;
                exit(5);
            }
            COB* pcob = opcob.value();        
            for (auto& pRegName: cobRegName)    
            {
                Vector<String> names = splitString(pRegName, '_');
                Array<int, 128> Reg;
                if (names[0] == "sw")
                {
                    _register.outputCobSwInfo(names, pcob, Reg, file, row, col);
                }
                if (names[1] == "sel")
                {
                    _register.outputCobPortInfo(names, pcob, Reg, file, row, col);
                }
            }
        }
    }
    // tob
    for (size_t row = 0; row < Interposer::TOB_ARRAY_HEIGHT; ++row)
    {
        for (size_t col = 0; col < Interposer::TOB_ARRAY_WIDTH; ++col)
        {
            Option<TOB*> optob = interposer.get_tob(row, col);
            if (!optob.has_value())
            {
                std::cerr << "no cob at position: " << row << ", " << col << std::endl;
                exit(5);
            }
            TOB* ptob = optob.value();
            for (auto& pRegName: tobRegName)
            {
                Vector<String> names = splitString(pRegName, '_');
                Vector<int> Reg;
                String message("tob_" + to_string(row) + "_" + to_string(col) + "_");
                if(names[0][0] == 'h')
                {
                    message +=  "hctrl_bank";
                    _register.outputTobhvMuxInfo(Reg, ptob, message);
                }
                else if (names[0][0] == 'v')
                {
                    message += "vctrl_bank";
                    _register.outputTobhvMuxInfo(Reg, ptob, message);
                }
                else if (names[0] == "bank")
                {
                    message += "bank_sel";
                    _register.outputTobtrackMuxInfo(Reg, ptob, message);
                }
                else if (names[0] == "bump2tob")
                {
                    message += "bump2tob_bank";
                    _register.outputTobBumpInfo(Reg, ptob, message);
                }
                else if (names[0] == "tob2bump")
                {
                    message += "tob2bump_bank";
                    _register.outputTobBumpInfo(Reg, ptob, message, true);
                }
                else if (names[0] == "track2tob")
                {
                    message += "track2tob_";
                    _register.outputTobTrackInfo(Reg, ptob, message, true);
                }
                else if (names[0] == "tob2track")
                {
                    message += "tob2track_";
                    _register.outputTobTrackInfo(Reg, ptob, message);
                }
                else{
                    std::cerr << "tob register name error" << std::endl;
                    exit(8);
                }
            }
            // dly 和 drv
            String controlbits = "00000000";
            std::cout << controlbits << " " << "tob_" << row << "_" << col << "_dly_0";
            std::cout << controlbits << " " << "tob_" << row << "_" << col << "_dly_1";
            std::cout << controlbits << " " << "tob_" << row << "_" << col << "_drv_0";
            std::cout << controlbits << " " << "tob_" << row << "_" << col << "_drv_1";
        }
    }

    // TODO:xinzhai_C4_noi_up_pad_ctrl 这种是什么

}
    
}





