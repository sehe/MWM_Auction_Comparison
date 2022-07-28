#include "BipartiteGraph.h"
#include "MaximumWeightedMatching.h"
#include "Auction.h"
#include "AuctionArray.h"
#include <vector>

int main(int argc, const char* argv[]) {
    std::vector<std::string_view> args{argv + 1, argv + argc};

    bool const verbose = std::count(args.begin(), args.end(), "-v");
    args.erase(std::remove(args.begin(), args.end(), "-v"), args.end());

    if (args.size() != 2) {
        std::cout << argv[0] << " min max [-v]\n";
        return 1;
    }
    int min = std::atoi(args[0].data());
    int max = std::atoi(args[1].data());

    assert(("The starting number must be greater or equal to the ending number",
            min <= max));

    std::ofstream stream("../data/results.csv");
    stream << "Edge per part,Execution Time MWM,Seconds MWM,Total Cost "
              "MWM,Execution Time AU,Seconds AU,Total Cost AU,Execution Time "
              "AU-A,Seconds AU-A,Total Cost AU-A,Winner Execution Time,Winner "
              "Total Cost\n";

    std::cout << "-------- BECHMARK --------\n\n";

    for (int n = min; n <= max; ++n) {
        std::cout << "\n\n\nGeneration of a Bipartite Graph with " << n
                  << " verticies per part: ";

        duration elapsed_mwm, elapsed_au, elapsed_aua;
        Weight   total_cost_mwm, total_cost_au, total_cost_aua;

        // Graph graph = generateData(n);
        auto [cost_matrix, graph] = generateData(n);
        assert(("Number of verticies not correct",
                boost::num_vertices(graph) == 2 * n));
        assert(
            ("Number of edges not correct", boost::num_edges(graph) == n * n));
        std::cout << "done\n";

        if (verbose)
            printGraph(graph);

        // MAXIMUM WEIGHTED MATCHING
        std::cout << "Execution time of Maximum Weight Matching";
        total_cost_mwm = perform_mwm(graph, elapsed_mwm);
        std::cout << std::fixed << fmt{elapsed_mwm}
                  << ", with total cost: " << (total_cost_mwm / 10'000.0)
                  << "\n\n";

        // AUCTION  ALGOROTHM
        std::cout << "Execution time of Auction Algorithm";
        total_cost_au = perform_au(graph, elapsed_au);
        std::cout << std::fixed << fmt{elapsed_au}
                  << ", with total cost: " << (total_cost_au / 10'000.0)
                  << "\n\n";

        // AUCTION ARRAY ALGOROTHM
        std::cout << "Execution time of Auction Algorithm";
        // total_cost_au = perform_au(graph, elapsed_au);
        total_cost_aua = perform_au_array(cost_matrix, n, elapsed_aua);
        std::cout << std::fixed << fmt{elapsed_aua}
                  << ", with total cost: " << (total_cost_aua / 10'000.0)
                  << "\n\n";

        // std::cout << "Same solution? ";
        //(total_cost_mwm == total_cost_au) ? std::cout << "Yes" : std::cout <<
        //"No";

        stream << n << "," << fmt{elapsed_mwm} << "," << (elapsed_mwm / 1.0s)
               << "," << (total_cost_mwm / 10'000.0) << "," << fmt{elapsed_au}
               << "," << (elapsed_au / 1.0s) << ","
               << (total_cost_au / 10'000.0) << "," << fmt{elapsed_aua} << ","
               << (elapsed_aua / 1.0s) << "," << (total_cost_aua / 10'000.0)
               << ","
               << ((elapsed_mwm / 1.0s) == (elapsed_aua / 1.0s)      ? "None"
                       : (elapsed_mwm / 1.0s) < (elapsed_aua / 1.0s) ? "MWM"
                                                                     : "AU")
               << ","
               << ((total_cost_mwm == total_cost_au)      ? "None"
                       : (total_cost_mwm > total_cost_au) ? "MWM"
                                                          : "AU")
               << "\n";
    }

    std::cout << "\n";

    return 0;
}
