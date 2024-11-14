// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include "customtypes.hh"
#include <utility>
#include <vector>

// Add your own STL includes below this comment
#include <set>
#include <stack>
#include <iostream>
#include <math.h>
#include <functional>
#include <unordered_map>
#include <unordered_set>



template <typename Type>
Type random_in_range(Type start, Type end);

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: unordered_map::size is constant.
    unsigned int get_bite_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: clear() is linear in complexity
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterates through all bites
    std::vector<BiteID> all_bites();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Insert is linear in worst case.
    bool add_bite(BiteID id, const Name &name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function linear in worst case.
    Name get_bite_name(BiteID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function linear in worst case.
    Coord get_bite_coord(BiteID id);

    // We recommend you implement the operations below only after implementing the
    // ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Construct vector from already sorted set.
    std::vector<BiteID> get_bites_alphabetically();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Sort function is logarithmic.
    // Also constructing vector from set is O(n)
    std::vector<BiteID> get_bites_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function is linear in worst case.
    BiteID find_bite_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Insertion into unordered_map is linear worst case.
    bool add_contour(ContourID id, const Name &name, ContourHeight height,
                     std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterates through each ContourID in _countours.
    std::vector<ContourID> all_contours();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Function find is linear in worst case.
    Name get_contour_name(ContourID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Function find is linear in worst case.
    std::vector<Coord> get_contour_coords(ContourID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Function find is linear in worst case.
    ContourHeight get_contour_height(ContourID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find and insertion linear in worst case.
    bool add_subcontour_to_contour(ContourID id, ContourID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: get_contour_coords() & std::find are O(n)
    bool add_bite_to_contour(BiteID biteid, ContourID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find and while-loop linear.
    std::vector<ContourID> get_bite_in_contours(BiteID id);

    // Non-compulsory operations

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: Nested loops.
    std::vector<ContourID> all_subcontours_of_contour(ContourID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BiteID> get_bites_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Erasing vector.
    bool remove_bite(BiteID id);

    // Estimate of performance:
    // Short rationale for estimate:
    ContourID get_closest_common_ancestor_of_contours(ContourID id1,
                                                      ContourID id2);


    /**
   * ============================
   * ============================
   * ========== PRG2 ============
   * ============================
   * ============================
   */

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_connection(ConnectionID connectionid, BiteID id1, BiteID id2,
                        std::vector<Coord> xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Needs to iterate through all elements.
    void clear_connections();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BiteID> get_next_bites_from(BiteID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<ConnectionID> get_connections(BiteID id1 = NO_BITE, BiteID id2 = NO_BITE);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> get_connection_coords(BiteID biteid,
                                             ConnectionID connectionid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Coord, Distance>> path_any(BiteID fromid, BiteID toid);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Coord, Distance>> path_shortest(BiteID /*fromid*/,
                                                          BiteID /*toid*/);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Coord, Distance>> path_least_bites(BiteID /*fromid*/,
                                                             BiteID /*toid*/);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Coord, Distance>> path_least_uphill(BiteID /*fromid*/,
                                                              BiteID /*toid*/);



private:
    // Compare coordinates of two bites via Manhattan-method.
    bool compare_coordinates(const BiteID& b1, const BiteID& b2);

    // Datastructures
    struct ContourData {
        ContourID id;
        Name name;
        ContourHeight height;
        std::vector<Coord> coords;
        std::vector<BiteID> bites = {};
        ContourData* parent = nullptr;
    };

    struct BiteData {
        BiteID id;
        Name name;
        Coord coord;
        bool in_contour = false;
        ContourData* contour = nullptr;
    };

    struct ConnectionData {
        ConnectionID id;
        std::pair<BiteID*, BiteID*> start_end = {nullptr, nullptr};
        std::vector<Coord> path = {};
    };

    std::unordered_map<BiteID, BiteData> _bites;
    std::unordered_map<ContourID, ContourData> _contours;
    std::unordered_map<Coord, BiteData> _coord_to_bite;
    // Custom comparator for keeping set alphabetically sorted.
    std::set<BiteID, std::function<bool(const BiteID&, const BiteID&)>> _bites_alphabetical {
        [this](const BiteID& a, const BiteID& b) {
            if (get_bite_name(a) != get_bite_name(b)) {
                return get_bite_name(a) < get_bite_name(b);
            }
            // Else sort by ID
            return a < b;
        }
    };
    std::unordered_map<ConnectionID, ConnectionData> _connections;

    // Always check that these are updated before using them!
    std::vector<BiteID> _bites_distance_increasing_v;
    std::vector<BiteID> _bites_alphabetical_v;
    std::vector<BiteID> _all_bites_v;
    bool _isAlphaVectorUpdated = false;
    bool _isDistanceVectorUpdated = false;
    bool _isAllBitesUpdated = false;

    std::vector<ConnectionID> _all_connections;
    bool _isAllConnectionsUpdated = false;

    bool dfs(BiteID current_bite, BiteID end_bite, Coord last_coord,
             Distance current_distance,
             std::unordered_set<BiteID>& visited_bites,
             std::unordered_set<Coord>& visited_coords,
             std::vector<std::pair<Coord, Distance>>& path_result);

    Distance distance_between(Coord a, Coord b);

};

#endif // DATASTRUCTURES_HH
