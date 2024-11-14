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

    clear_connections();
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

/**
 * ============================
 * ============================
 * ========== PRG2 ============
 * ============================
 * ============================
 */

bool Datastructures::add_connection(ConnectionID connectionid, BiteID id1, BiteID id2, std::vector<Coord> xy)
{
    // Check if connection ID already exists
    if (_connections.find(connectionid) != _connections.end()) {
        return false;
    }

    const auto& bite1_it = _bites.find(id1);
    const auto& bite2_it = _bites.find(id2);
    // Check if both bites exist
    if (bite1_it == _bites.end() || bite2_it == _bites.end()) {
        return false;
    }

    // Init new connection
    ConnectionData new_connection;
    new_connection.id = connectionid;
    new_connection.start_end = {&bite1_it->second.id, &bite2_it->second.id};

    // Add starting bite to the path
    new_connection.path.push_back(bite1_it->second.coord);

    Coord prev = bite1_it->second.coord;

    // Traverse through the provided coordinates (xy)
    for (const auto& point : xy) {
        // Ensure movement is not diagonal (either x or y must remain the same)
        if (prev.x != point.x && prev.y != point.y) { return false; }

        new_connection.path.emplace_back(point);
        prev = point;
    }

    // Ensure the final coordinate matches the second bite's coordinates
    const Coord& last_coord = bite2_it->second.coord;
    if (prev.x != last_coord.x && prev.y != last_coord.y) { return false; }

    // Add the end bite to the path
    new_connection.path.push_back(bite2_it->second.coord);
    _connections.insert({connectionid, new_connection});

    return true;
}

void Datastructures::clear_connections()
{
    _connections.clear();
    _all_connections.clear();
    _isAllConnectionsUpdated = false;
}

std::vector<BiteID> Datastructures::get_next_bites_from(BiteID id)
{
    const auto& bite_it = _bites.find(id);

    // Case: Unknown ID
    if (bite_it == _bites.end()) { return {NO_BITE}; }

    std::unordered_set<BiteID> next_bites;

    for (const auto& [conn_id, conn_data] : _connections) {
        const BiteID& start_bite = *conn_data.start_end.first;
        const BiteID& end_bite = *conn_data.start_end.second;

        if (start_bite == id) {
            next_bites.insert(end_bite);
        } else if (end_bite == id) {
            next_bites.insert(start_bite);
        }
    }

    std::vector<BiteID> result(next_bites.begin(), next_bites.end());

    // Return an empty vector if no next bites were found
    return result.empty() ? std::vector<BiteID>{} : result;
}

std::vector<ConnectionID> Datastructures::get_connections(BiteID id1, BiteID id2)
{
    // Case: Return all connections.
    if (id1 == NO_BITE && id2 == NO_BITE) {
        if (!_isAllConnectionsUpdated) {
            _all_connections.clear();
            _all_connections.reserve(_connections.size());

            for (const auto& [id, _] : _connections) {
                _all_connections.emplace_back(id);
            }
            _isAllConnectionsUpdated = true;
        }

        return _all_connections;
    }

    std::vector<ConnectionID> result;

    // Case: Single bite given.
    if (id2 == NO_BITE) {
        for (const auto& [id, data] : _connections) {
            const BiteID& start_bite = *data.start_end.first;
            const BiteID& end_bite = *data.start_end.second;

            // If the connection involves the provided Bite ID, add to result
            if (start_bite == id1 || end_bite == id1) {
                result.emplace_back(id);
            }
        }

        return result.empty() ? std::vector<ConnectionID>{NO_CONNECTION} : result;
    }

    if (_bites.find(id1) == _bites.end() ||
        _bites.find(id2) == _bites.end()) { return {NO_CONNECTION}; }

    // Case: Return connections between two specific bites (id1 and id2)
    for (const auto& [id, data] : _connections) {
        const BiteID& start_bite = *data.start_end.first;
        const BiteID& end_bite = *data.start_end.second;

        // Check if the connection involves both id1 and id2 (in either direction)
        if ((start_bite == id1 && end_bite == id2) || (start_bite == id2 && end_bite == id1)) {
            result.emplace_back(id);
        }
    }

    return result.empty() ? std::vector<ConnectionID>{} : result;
}

std::vector<Coord> Datastructures::get_connection_coords(BiteID biteid,
                                                         ConnectionID connectionid)
{
    const auto& bite_it = _bites.find(biteid);
    const auto& conn_it = _connections.find(connectionid);

    // Case: Unknown ID
    if (bite_it == _bites.end() || conn_it == _connections.end()) {
        return {NO_COORD};
    }

    // Case: Bite not in connection
    std::vector<Coord> connection_path = conn_it->second.path;
    const Coord& target_coord = bite_it->second.coord;
    if (std::find(connection_path.begin(),
                  connection_path.end(),
                  target_coord) == connection_path.end()) {
        return {NO_COORD};
    }

    std::vector<Coord> result;

    const BiteID connection_start = *conn_it->second.start_end.first;
    const BiteID connection_end = *conn_it->second.start_end.second;

    // Retrieve coords from path normally.
    if (biteid == connection_start) {
        for (const Coord& coord : connection_path) {
            result.emplace_back(coord);
        }
    }

    // Retrieve coords from path REVERSED.
    else if (biteid == connection_end) {
        for (auto reverse_it = connection_path.rbegin();
             reverse_it != connection_path.rend();
             ++reverse_it) {

            result.emplace_back(*reverse_it);
        }
    }

    return result;
}

std::vector<std::pair<Coord, Distance>> Datastructures::path_any(BiteID fromid, BiteID toid)
{
    const auto& from = _bites.find(fromid);
    const auto& to = _bites.find(toid);

    // Case: Unknown ID
    if (from == _bites.end() || to == _bites.end()) {
        return { {NO_COORD, NO_DISTANCE} };
    }

    // If start and end are the same, return empty vector
    if (from == to) { return {}; }

    std::unordered_set<BiteID> visited;
    std::unordered_set<Coord> visited_coords;
    std::vector<std::pair<Coord, Distance>> path_result;

    Coord start_coord = from->second.coord;
    // Start DFS from the starting bite
    if (dfs(fromid, toid, start_coord, 0, visited, visited_coords, path_result)) {
        return path_result;
    }

    // No path found, return empty vector
    return {};
}

// Non-compulsory operations
std::vector<std::pair<Coord, Distance>> Datastructures::path_shortest(BiteID /*fromid*/,
                                                                      BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_shortest");
}
std::vector<std::pair<Coord, Distance>> Datastructures::path_least_bites(BiteID /*fromid*/,
                                                                         BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_least_bites");
}
std::vector<std::pair<Coord, Distance>> Datastructures::path_least_uphill(BiteID /*fromid*/,
                                                                          BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_least_uphill");
}

bool Datastructures::dfs(BiteID current_bite, BiteID target_bite, Coord last_coord,
                         Distance current_distance,
                         std::unordered_set<BiteID>& visited,
                         std::unordered_set<Coord>& visited_coords,
                         std::vector<std::pair<Coord, Distance>>& path_result)
{
    // Base case: If the current bite is the target bite, path is found
    if (current_bite == target_bite) {
        return true;
    }

    visited.insert(current_bite);  // Mark current bite as visited

    // Explore connected bites
    for (const auto& [conn_id, conn_data] : _connections) {
        BiteID next_bite = NO_BITE;
        bool forward_traversal = false;

        if (*conn_data.start_end.first == current_bite) {
            next_bite = *conn_data.start_end.second;
            forward_traversal = true;
        } else if (*conn_data.start_end.second == current_bite) {
            next_bite = *conn_data.start_end.first;
            forward_traversal = false;
        } else {
            continue;
        }

        // Skip if already visited
        if (next_bite == NO_BITE || visited.find(next_bite) != visited.end()) {
            continue;
        }

        const std::vector<Coord>& connection_path = conn_data.path;
        Distance added_distance = 0;
        std::vector<Coord> added_coords;  // Keep track of added coordinates for backtracking
        Coord original_last_coord = last_coord;  // Save the original last_coord for backtracking

        // Traverse the path based on direction
        if (forward_traversal) {
            for (const Coord& coord : connection_path) {
                if (visited_coords.find(coord) == visited_coords.end()) {
                    added_distance += distance_between(last_coord, coord);
                    last_coord = coord;
                    path_result.push_back({coord, current_distance + added_distance});
                    visited_coords.insert(coord);
                    added_coords.push_back(coord);
                }
            }
        } else {
            for (auto it = connection_path.rbegin(); it != connection_path.rend(); ++it) {
                if (visited_coords.find(*it) == visited_coords.end()) {
                    added_distance += distance_between(last_coord, *it);
                    last_coord = *it;
                    path_result.push_back({*it, current_distance + added_distance});
                    visited_coords.insert(*it);
                    added_coords.push_back(*it);
                }
            }
        }

        // Recur for the next bite
        if (dfs(next_bite, target_bite, last_coord, current_distance + added_distance,
                visited, visited_coords, path_result)) {
            return true;  // Path found!
        }

        // Backtrack - remove added coordinates
        for (const Coord& coord : added_coords) {
            visited_coords.erase(coord);
            path_result.pop_back();
        }
        last_coord = original_last_coord;  // Restore the original last_coord position
    }

    // All paths from this bite explored and none worked
    return false;
}

Distance Datastructures::distance_between(Coord a, Coord b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

















