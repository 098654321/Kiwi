#include "./route.hh"
#include <circuit/net/net.hh>
#include <algorithm>
#include <iostream>

#define DEBUG_algo


namespace kiwi::algo {

    auto route_nets(
        hardware::Interposer* interposer,
        std::Vector<std::Box<circuit::Net>>& nets,
        const RouteStrategy& strateg
    ) -> void {
        // 降序
#ifdef DEBUG_algo
std::cout << "sorting nets" << std::endl;
#endif
        auto compare = [] (const std::Box<circuit::Net>& n1, const std::Box<circuit::Net>& n2) -> bool {
            return n1->priority() > n2->priority();
        };
        std::sort(nets.begin(), nets.end(), compare);

#ifdef DEBUG_algo
std::cout << "sorting ended, go through all nets and route" << std::endl;
#endif

        for (auto& net : nets) {
            net->route(interposer, strateg);
        }
    }

}