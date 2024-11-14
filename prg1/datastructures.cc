/**
 * @brief The Datastructures class
 * STUDENTS: Modify the code below to implement the functionality of the class.
 * Also remove comments from the parameter names when you implement an operation
 * (Commenting out parameter name prevents compiler from warning about unused
 * parameters on operations you haven't yet implemented.)
 */

#include "datastructures.hh"
#include "customtypes.hh"

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::get_bite_count()
{
    return _bites.size();
}

void Datastructures::clear_all()
{
    _bites.clear();
    _bites_alphabetical.clear();
    _contours.clear();
    _coord_to_bite.clear();
    _bites_alphabetical_v.clear();
    _bites_distance_increasing_v.clear();
    _all_bites_v.clear();
    _isAlphaVectorUpdated = false;
    _isDistanceVectorUpdated = false;
    _isAllBitesUpdated = false;
}

std::vector<BiteID> Datastructures::all_bites()
{
    if (!_isAllBitesUpdated) {
        _all_bites_v.clear();
        _all_bites_v.reserve(_bites.size());

        // Collect keys from map.
        for (const auto& [key, _] : _bites) {
            _all_bites_v.emplace_back(key);
        }

        _isAllBitesUpdated = true;
    }

    return _all_bites_v;
}

bool Datastructures::add_bite(BiteID id, const Name& name, Coord xy)
{
    // Case: Bite already exists
    if (_bites.find(id) != _bites.end()) {
        return false;
    }

    // Case: Coord already exists
    if (_coord_to_bite.find(xy) != _coord_to_bite.end()) {
        return false;
    }

    // Bite can be added.
    BiteData newBite = {id, name, xy, false, nullptr};

    // Add bite into datastructs and update flags
    _bites.emplace(id, newBite);
    _bites_alphabetical.emplace(newBite.id);
    _coord_to_bite.emplace(xy, newBite);
    _isAlphaVectorUpdated = false;
    _isDistanceVectorUpdated = false;
    _isAllBitesUpdated = false;

    return true;
}

Name Datastructures::get_bite_name(BiteID id)
{
    const auto& bite_iter = _bites.find(id);
    // If bite found return its name
    if (bite_iter != _bites.end()) {
        return bite_iter->second.name;
    }

    return NO_NAME;
}

Coord Datastructures::get_bite_coord(BiteID id)
{
    const auto& bite_iter = _bites.find(id);
    // If bite found return its name
    if (bite_iter != _bites.end()) {
        return bite_iter->second.coord;
    }

    return NO_COORD;
}

std::vector<BiteID> Datastructures::get_bites_alphabetically()
{
    if (!_isAlphaVectorUpdated) {
        _bites_alphabetical_v.assign(_bites_alphabetical.begin(), _bites_alphabetical.end());
        _isAlphaVectorUpdated = true;
    }

    return _bites_alphabetical_v;
}

std::vector<BiteID> Datastructures::get_bites_distance_increasing()
{
    if (_bites.empty()) {return std::vector<BiteID>(0);}
    if (_bites.size() == 1) {return {_bites.begin()->first};}

    if (!_isDistanceVectorUpdated) {
        _bites_distance_increasing_v.clear();
        _bites_distance_increasing_v.reserve(_bites.size());

        // Copy bite data to a vector
        std::vector<BiteID> bite_data;
        bite_data.reserve(_bites.size());
        for (const auto& [id, _] : _bites) {
            bite_data.push_back(id);
        }

        // Sort the vector based on the custom compare function
        std::sort(bite_data.begin(), bite_data.end(), [this](const BiteID& b1, const BiteID& b2) {
            return compare_coordinates(b1, b2);
        });

        // Extract the sorted BiteIDs
        for (const auto& bite : bite_data) {
            _bites_distance_increasing_v.push_back(bite);
        }

        _isDistanceVectorUpdated = true;
    }

    return _bites_distance_increasing_v;
}

BiteID Datastructures::find_bite_with_coord(Coord xy)
{
    const auto& coord_iter = _coord_to_bite.find(xy);
    if (coord_iter != _coord_to_bite.end()) {
        // Just to make sure the bite is valid
        if (get_bite_name(coord_iter->second.id) != NO_NAME) {
            return coord_iter->second.id;
        }
    }

    return NO_BITE;
}

bool Datastructures::change_bite_coord(BiteID id, Coord newcoord)
{
    const auto& bite_iter = _bites.find(id);
    const auto& coord_iter = _coord_to_bite.find(newcoord);

    // Case: Unknown ID
    if (bite_iter == _bites.end()) { return false; }

    // Case: New coordinate is already occupied
    if (coord_iter != _coord_to_bite.end()) { return false; }

    // Case: New coord not apart of current contours range
    if (bite_iter->second.in_contour == true && bite_iter->second.contour != nullptr) {
        ContourData* bite_contour = bite_iter->second.contour;
        const auto& contour_range = std::find(bite_contour->coords.begin(),
                                               bite_contour->coords.end(),
                                               newcoord);
        if (contour_range == bite_contour->coords.end()) { return false; }
    }

    _coord_to_bite.erase(bite_iter->second.coord);
    bite_iter->second.coord = newcoord;
    _coord_to_bite[newcoord] = bite_iter->second;
    _isDistanceVectorUpdated = false;
    return true;
}

bool Datastructures::add_contour(ContourID id, const Name &name, ContourHeight height,
                                 std::vector<Coord> coords)
{
    // Case: ID already in-use
    if (_contours.find(id) != _contours.end()) {
        return false;
    }

    // Case: Invalid height
    if (height > MAX_CONTOUR_HEIGHT || height < -MAX_CONTOUR_HEIGHT) {
        return false;
    }

    // Case: No coords given
    if (coords.empty()) {
        return false;
    }

    ContourData new_contour = {id, name, height, coords};
    _contours.emplace( new_contour.id, new_contour );
    return true;
}

std::vector<ContourID> Datastructures::all_contours()
{
    std::vector<ContourID> result;

    // Collect all id's
    for (const auto& [contour, _] : _contours) {
        result.emplace_back(contour);
    }
    return result;
}

Name Datastructures::get_contour_name(ContourID id)
{
    const auto& contour_iter = _contours.find(id);
    if (contour_iter != _contours.end()) {
        return contour_iter->second.name;
    }
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_contour_coords(ContourID id)
{
    const auto& contour_iter = _contours.find(id);
    if (contour_iter != _contours.end()) {
        return contour_iter->second.coords;
    }
    std::vector<Coord> empty_result = {NO_COORD};
    return empty_result;
}

ContourHeight Datastructures::get_contour_height(ContourID id)
{
    const auto& contour_iter = _contours.find(id);
    if (contour_iter != _contours.end()) {
        return contour_iter->second.height;
    }
    return NO_CONTOUR_HEIGHT;
}

bool Datastructures::add_subcontour_to_contour(ContourID id,
                                               ContourID parentid)
{
    const auto& child_iter = _contours.find(id);
    const auto& parent_iter = _contours.find(parentid);

    // Case: Unknown ID
    if (child_iter == _contours.end() ||
        parent_iter == _contours.end()) {
        return false;
    }

    // Case: Already has a parent
    if (child_iter->second.parent != nullptr) { return false; }

    const int& child_height = get_contour_height(id);
    const int& parent_height = get_contour_height(parentid);

    // Case: Childs height smaller than parents
    if (std::abs(child_height) < std::abs(parent_height)) { return false; }

    // Case: Height difference other than 1
    if (std::abs(child_height - parent_height) != 1) { return false; }

    _contours[id].parent = &_contours[parentid];
    return true;
}

bool Datastructures::add_bite_to_contour(BiteID biteid, ContourID contourid)
{
    const auto& bite_iter = _bites.find(biteid);
    const auto& contour_iter = _contours.find(contourid);

    // Case: Unknown ID
    if (bite_iter == _bites.end() ||
        contour_iter == _contours.end()) { return false; }

    // Case: Bite already in another contour
    if (bite_iter->second.in_contour == true) { return false; }

    // Case: Bite not in contour area
    Coord bite_xy = bite_iter->second.coord;
    const std::vector<Coord>& contour_xy = get_contour_coords(contourid);
    if (std::find(contour_xy.begin(), contour_xy.end(), bite_xy) == contour_xy.end()) {
        return false;
    }

    bite_iter->second.in_contour = true;
    bite_iter->second.contour = &contour_iter->second;
    contour_iter->second.bites.emplace_back(bite_iter->second.id);

    return true;
}

std::vector<ContourID> Datastructures::get_bite_in_contours(BiteID id)
{
    const auto& bite_iter = _bites.find(id);

    // Case: Unknown ID
    if (bite_iter == _bites.end()) {
        std::vector<ContourID> empty = {NO_CONTOUR};
        return empty;
    }

    // Case: Bite does not belong to any contour
    if (bite_iter->second.in_contour == false) { return std::vector<ContourID>(0); }

    std::vector<ContourID> result;
    ContourData* current_contour = bite_iter->second.contour;

    // Traverse contours from bite
    while (current_contour != nullptr) {
        ContourID curr_contour_id = current_contour->id;
        result.emplace_back(curr_contour_id);

        const auto& contour_iter = _contours.find(curr_contour_id);
        if (contour_iter == _contours.end()) { break; }

        current_contour = contour_iter->second.parent;
    }

    return result;
}

bool Datastructures::compare_coordinates(const BiteID& b1, const BiteID& b2)
{
    const Coord& coord1 =_bites.find(b1)->second.coord;
    const Coord& coord2 = _bites.find(b2)->second.coord;

    unsigned int b1_distance = abs(coord1.x) + abs(coord1.y);
    unsigned int b2_distance = abs(coord2.x) + abs(coord2.y);

    return b1_distance < b2_distance;
}

std::vector<ContourID>
Datastructures::all_subcontours_of_contour(ContourID id)
{
    const auto& contour_it = _contours.find(id);

    // Case: Unknown ID
    if (contour_it == _contours.end()) { return {NO_CONTOUR}; }

    // Vector to store all subcontours
    std::vector<ContourID> subcontours;

    // Start from the given contour and find all subcontours
    std::stack<const ContourData*> contour_stack;
    contour_stack.push(&contour_it->second);

    while (!contour_stack.empty()) {
        const ContourData* current_contour = contour_stack.top();
        contour_stack.pop();

        // Traverse all children (subcontours) of the current contour
        for (const auto& [child_id, child_data] : _contours) {
            if (child_data.parent == current_contour) {
                subcontours.push_back(child_id);
                contour_stack.push(&child_data);
            }
        }
    }

    return subcontours;
}

ContourID
Datastructures::get_closest_common_ancestor_of_contours(ContourID id1,
                                                        ContourID id2)
{
    const auto& contour1 = _contours.find(id1);
    const auto& contour2 = _contours.find(id2);

    if (contour1 == _contours.end() || contour2 == _contours.end()) { return NO_CONTOUR; }

    // Retrieve contour1 ancestors.
    std::unordered_set<ContourID> ancestors1;
    ContourData* curr_contour1 = contour1->second.parent;
    while (curr_contour1 != nullptr) {
        ancestors1.insert(curr_contour1->id);
        curr_contour1 = curr_contour1->parent;
    }

    // Find closest common ancestor.
    ContourData* curr_contour2 = contour2->second.parent;
    while (curr_contour2 != nullptr) {
        if (ancestors1.find(curr_contour2->id) != ancestors1.end()) {
            return curr_contour2->id;
        }
        curr_contour2 = curr_contour2->parent;
    }

    return NO_CONTOUR;
}

bool Datastructures::remove_bite(BiteID id)
{
    const auto& bite_it = _bites.find(id);

    // Case: Unknown ID
    if (bite_it == _bites.end()) { return false; }
    const auto& bite = bite_it->second;

    _bites_alphabetical.erase(id);
    _coord_to_bite.erase(bite.coord);

    // If the bite is part of a contour, remove it from the contour's bite list
    if (bite.in_contour && bite.contour != nullptr) {
        auto& contour_bites = bite.contour->bites;
        contour_bites.erase(std::remove(contour_bites.begin(), contour_bites.end(), id), contour_bites.end());
    }

    // Remove the bite from the bites map
    _bites.erase(bite_it);

    _isAllBitesUpdated = false;
    _isAlphaVectorUpdated = false;
    _isDistanceVectorUpdated = false;

    return true;
}

std::vector<BiteID> Datastructures::get_bites_closest_to(Coord xy)
{
    // Check if there are any bites to compare
    if (_bites.empty()) { return {}; }

    // Vector to store pairs of BiteID and their distance from the given coordinate
    std::vector<std::pair<BiteID, unsigned int>> bite_distances;
    bite_distances.reserve(_bites.size());

    // Calculate the Manhattan distance from `xy` to each bite's coordinates
    for (const auto& [bite_id, bite_data] : _bites) {
        const Coord& bite_coord = bite_data.coord;
        unsigned int distance = std::abs(bite_coord.x - xy.x) + std::abs(bite_coord.y - xy.y);

        bite_distances.emplace_back(bite_id, distance);
    }

    // Sort bites by distance (ascending)
    std::sort(bite_distances.begin(), bite_distances.end(),
              [this](const std::pair<BiteID, unsigned int>& a, const std::pair<BiteID, unsigned int>& b) {
                  // Compare by distance first
                  if (a.second != b.second) { return a.second < b.second; }

                  // If distances are equal, compare by y-coordinate
                  const Coord& coord_a = _bites.find(a.first)->second.coord;
                  const Coord& coord_b = _bites.find(b.first)->second.coord;

                  if (coord_a.y != coord_b.y) { return coord_a.y < coord_b.y; }

                  // If y-coordinates are the same, compare by BiteID
                  return a.first < b.first;
              });

    // Get the top 3 closest bites
    std::vector<BiteID> closest_bites;
    unsigned int target_amount = 3;
    unsigned int bite_distances_size = bite_distances.size();
    for (unsigned int i = 0; i < std::min(target_amount, bite_distances_size); ++i) {
        closest_bites.push_back(bite_distances[i].first);
    }

    return closest_bites;
}







