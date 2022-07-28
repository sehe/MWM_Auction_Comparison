#include "../include/Auction.h"
#include "../include/BipartiteGraph.h"
#include <boost/algorithm/cxx11/iota.hpp>
#include <set>

namespace {
    auto auction_algorithm(Graph& graph, const int& n, duration& elapsed) {
        const Weight eps = 1;
        std::set<int> unassigned;
        boost::algorithm::iota_n(inserter(unassigned, unassigned.end()), 0, n);
        auto const items = boost::irange(n, 2 * n);

        // mappings
        std::vector<int> b2i(n, -1), i2b(n, -1);

        auto t_start = now();

        while (!unassigned.empty()) {
            for (auto b : unassigned) {
                Bidder& bidder = boost::get<Bidder>(graph[b]);

                // 1 Bid
                int    id_item1  = -1;
                Weight val_item1 = -1;
                Weight val_item2 = -1;

                for (auto i : items) {
                    Item& item = boost::get<Item>(graph[i]);

                    // TODO make less clumsy. If you want to iterate edges, do
                    // so instead...
                    Weight val = boost::get(boost::edge_weight, graph,
                                            (edge(b, i, graph)).first) -
                        item.cost;

                    if (val > val_item1) {
                        val_item2 = val_item1;
                        val_item1 = val;
                        id_item1 = item.id;
                    }
                    else if (val > val_item2) {
                        val_item2 = val;
                    }
                }

                bidder.best_item = id_item1 + n;
                bidder.val_first_best_item = val_item1;
                bidder.val_second_best_item = val_item2;

                // 2 Compete

                Weight bid = bidder.val_first_best_item -
                    bidder.val_second_best_item + eps;
                Item& best_item = boost::get<Item>(graph[bidder.best_item]);
                if (bid > best_item.high_bid) {
                    best_item.high_bid    = bid;
                    best_item.high_bidder = bidder.id;
                }
            }

            // 3 Assign
            for (auto i : items) {
                Item& item = boost::get<Item>(graph[i]);
                if (item.high_bid == -1)
                    continue;

                item.cost += item.high_bid;

                auto& b = i2b[item.id];
                if (b != -1) {
                    b2i[b] = -1;
                    unassigned.insert(b);
                }

                b      = item.high_bidder;
                b2i[b] = item.id;
                unassigned.equal_range(b);
            }
        }

        elapsed = now() - t_start;
        return b2i;
    }
} // namespace

Weight perform_au(Graph& graph, duration& elapsed) {
    int n = int(boost::num_vertices(graph) / 2);
    Weight total_cost_auction = 0;

    auto const b2i = auction_algorithm(graph, n, elapsed);

    std::cout << "\nThe matching is: ";
    for (int bidder = 0; bidder < n; ++bidder) {
        int item = b2i[bidder];
        std::cout << "(" << bidder << "," << item << ")";
        total_cost_auction += get(boost::edge_weight_t(), graph,
                                  edge(bidder, item + n, graph).first);
    }
    std::cout << "\n";
    return total_cost_auction;
}
