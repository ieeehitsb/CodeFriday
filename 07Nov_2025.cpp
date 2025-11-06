#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
#include <numeric>
#include <algorithm>
#include <limits>

using namespace std;

class ACO {
private:
    int n_ants;
    int n_iterations;
    double alpha;
    double beta;
    double evaporation_rate;
    double Q;
    
    int n_cities;
    vector<vector<double>> distances;
    vector<vector<double>> pheromones;
    vector<vector<double>> heuristic;

    vector<int> best_tour;
    double best_length;

    mt19937 gen;

    void calculate_distances(const vector<pair<double, double>>& points) {
        distances.resize(n_cities, vector<double>(n_cities, 0.0));
        heuristic.resize(n_cities, vector<double>(n_cities, 0.0));

        for (int i = 0; i < n_cities; ++i) {
            for (int j = i + 1; j < n_cities; ++j) {
                double dist = sqrt(pow(points[i].first - points[j].first, 2) +
                                   pow(points[i].second - points[j].second, 2));
                distances[i][j] = dist;
                distances[j][i] = dist;
                
                heuristic[i][j] = 1.0 / (dist + 1e-10);
                heuristic[j][i] = 1.0 / (dist + 1e-10);
            }
        }
    }

    double calculate_tour_length(const vector<int>& tour) {
        double length = 0.0;
        for (size_t i = 0; i < tour.size() - 1; ++i) {
            length += distances[tour[i]][tour[i+1]];
        }
        length += distances[tour.back()][tour.front()];
        return length;
    }

    vector<double> calculate_probabilities(int current_city, vector<bool>& visited) {
        vector<double> probs(n_cities, 0.0);
        double total_desirability = 0.0;

        for (int i = 0; i < n_cities; ++i) {
            if (!visited[i]) {
                double desirability = pow(pheromones[current_city][i], alpha) *
                                      pow(heuristic[current_city][i], beta);
                probs[i] = desirability;
                total_desirability += desirability;
            }
        }

        if (total_desirability == 0.0) {
            for (int i = 0; i < n_cities; ++i) {
                if (!visited[i]) {
                    probs[i] = 1.0; 
                    total_desirability += 1.0;
                }
            }
        }

        for (int i = 0; i < n_cities; ++i) {
            if (probs[i] > 0.0) {
                probs[i] /= total_desirability;
            }
        }
        return probs;
    }

    vector<vector<int>> build_tours() {
        vector<vector<int>> all_tours;
        uniform_int_distribution<> start_dist(0, n_cities - 1);

        for (int ant = 0; ant < n_ants; ++ant) {
            vector<int> tour;
            vector<bool> visited(n_cities, false);
            
            int current_city = start_dist(gen);
            tour.push_back(current_city);
            visited[current_city] = true;

            while (tour.size() < n_cities) {
                vector<double> probs = calculate_probabilities(current_city, visited);
                
                discrete_distribution<> dist(probs.begin(), probs.end());
                int next_city = dist(gen);

                tour.push_back(next_city);
                visited[next_city] = true;
                current_city = next_city;
            }
            all_tours.push_back(tour);
        }
        return all_tours;
    }

    void update_pheromones(const vector<vector<int>>& all_tours, const vector<double>& all_lengths) {
        for (int i = 0; i < n_cities; ++i) {
            for (int j = 0; j < n_cities; ++j) {
                pheromones[i][j] *= (1.0 - evaporation_rate);
            }
        }

        for (int ant = 0; ant < n_ants; ++ant) {
            double deposit_amount = Q / all_lengths[ant];
            const auto& tour = all_tours[ant];

            for (size_t i = 0; i < tour.size() - 1; ++i) {
                pheromones[tour[i]][tour[i+1]] += deposit_amount;
                pheromones[tour[i+1]][tour[i]] += deposit_amount;
            }
            pheromones[tour.back()][tour.front()] += deposit_amount;
            pheromones[tour.front()][tour.back()] += deposit_amount;
        }
    }

public:
    ACO(int ants, int iter, double a, double b, double evap, double q)
        : n_ants(ants), n_iterations(iter), alpha(a), beta(b), 
          evaporation_rate(evap), Q(q), 
          best_length(numeric_limits<double>::infinity()),
          gen(random_device{}()) {}

    void solve(const vector<pair<double, double>>& points) {
        n_cities = points.size();
        if (n_cities == 0) return;

        calculate_distances(points);
        pheromones.resize(n_cities, vector<double>(n_cities, 1.0));
        best_tour.clear();
        best_length = numeric_limits<double>::infinity();

        for (int iter = 0; iter < n_iterations; ++iter) {
            vector<vector<int>> all_tours = build_tours();
            vector<double> all_lengths;
            
            for (const auto& tour : all_tours) {
                all_lengths.push_back(calculate_tour_length(tour));
            }

            update_pheromones(all_tours, all_lengths);

            for (int ant = 0; ant < n_ants; ++ant) {
                if (all_lengths[ant] < best_length) {
                    best_length = all_lengths[ant];
                    best_tour = all_tours[ant];
                }
            }
            
            if ((iter + 1) % 10 == 0) {
                cout << "Iteration " << (iter + 1) << "/" << n_iterations
                     << " | Best Length: " << fixed << setprecision(2) << best_length << endl;
            }
        }
    }

    vector<int> get_best_tour() const {
        return best_tour;
    }

    double get_best_length() const {
        return best_length;
    }
};

int main() {
    
    vector<pair<double, double>> cities = {
        {60, 200}, {180, 200}, {80, 180}, {140, 180}, {20, 160},
        {100, 160}, {200, 160}, {140, 140}, {40, 120}, {100, 120},
        {180, 100}, {60, 80}, {120, 80}, {180, 60}, {20, 40},
        {100, 40}, {200, 40}, {20, 20}, {60, 20}, {160, 20}
    };

    ACO aco(50, 200, 1.0, 5.0, 0.5, 100.0);
    
    aco.solve(cities);

    cout << "Best length: " << fixed << setprecision(2) << aco.get_best_length() << endl;

    cout << "Best tour: ";
    vector<int> tour = aco.get_best_tour();
    for (int city : tour) {
        cout << city << " -> ";
    }
    cout << tour[0] << endl;

    return 0;
}
