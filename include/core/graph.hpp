

namespace astar {

    struct Node {
        double lat;
        double lon;
    }

    struct Edge {
        Node to;
        double weight;
    }

}