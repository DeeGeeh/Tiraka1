// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include "customtypes.hh"
#include <utility>
#include <vector>
#include <unordered_map>

// Add your own STL includes below this comment
#include <algorithm>
#include <iostream>
#include <set>
#include <math.h>
#include <functional>
#include <unordered_set>
#include <stack>

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
    // Short rationale for estimate: Basic operations on unordered_map are linear.
    bool change_bite_coord(BiteID id, Coord newcoord);

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

    std::vector<BiteID> _bites_distance_increasing_v;
    std::vector<BiteID> _bites_alphabetical_v;
    std::vector<BiteID> _all_bites_v;
    bool _isAlphaVectorUpdated = false;
    bool _isDistanceVectorUpdated = false;
    bool _isAllBitesUpdated = false;

};
#endif // DATASTRUCTURES_HH
