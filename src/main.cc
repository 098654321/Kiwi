#include <string>
#include <iostream>

#include "./parser/parser.hh"
#include "./circuit/basedie.hh"
#include "./hardware/interposer.hh"
#include "./algo/router/maze/mazeroutestrategy.hh"
#include "./algo/router/routestrategy.hh"
#include "./algo/router/route.hh"

#include "./algo/router/route.cc"


int main()
{
    std::string filename = "E:/FDUFiles/Tao_group/kiwi/kiwi_new_version/noi_connection_for_sw_08061.xlsx";
    kiwi::circuit::BaseDie basedie;
    kiwi::hardware::Interposer interposer;
    kiwi::parser::Parser parser;
    kiwi::algo::MazeRouteStrategy mazeRouter;

    std::cout << "start parsing" << std::endl;
    parser.inputParse(filename, basedie, interposer); 
    std::cout << "end parsing" << std::endl;

    std::cout << "start routing" << std::endl;
    kiwi::algo::route_nets(&interposer, basedie.get_nets(), mazeRouter);
    std::cout << "end routing" << std::endl;

    return 0;
}

