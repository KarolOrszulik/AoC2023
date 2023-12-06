#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Record
{
    size_t time = 0;
    size_t dist = 0;
};

std::vector<Record> load_records_from_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error("Could not open file");
    }

    std::string line_time, line_dist;
    std::getline(file, line_time);
    std::getline(file, line_dist);

    std::istringstream iss_time(line_time);
    std::istringstream iss_dist(line_dist);

    std::string null;
    iss_time >> null;
    iss_dist >> null;

    std::vector<Record> result;

    size_t t = -1, d = -1;
    while(iss_time >> t && iss_dist >> d)
    {
        result.push_back({t, d});
    }

    return result;
}

Record unify_records(const std::vector<Record>& records)
{
    std::string total_time_str, total_dist_str;

    for (const auto& r : records)
    {
        total_time_str += std::to_string(r.time);
        total_dist_str += std::to_string(r.dist);
    }

    return { std::stoul(total_time_str), std::stoul(total_dist_str) };
}

size_t count_winning_strategies(size_t total_time, size_t distance_to_beat)
{
    auto is_winning = [](size_t total_time, size_t time_held, size_t distance_to_beat) -> bool {
        const size_t speed = time_held;
        const size_t time_ran = total_time - time_held;
        const size_t distance = speed * time_ran;

        return distance > distance_to_beat;
    };

    size_t first_winning = 0, last_winning = 0;

    for (size_t i = 0; i <= total_time; i++)
    {
        if (is_winning(total_time, i, distance_to_beat))
        {
            first_winning = i;
            break;
        }
    }

    for (size_t i = total_time; i >= 0; i--)
    {
        if (is_winning(total_time, i, distance_to_beat))
        {
            last_winning = i;
            break;
        }
    }

    return last_winning - first_winning + 1;
}

void boats(const std::string& path)
{
    std::vector<Record> records = load_records_from_file(path);

    // ======== PART 1 ========
    size_t winning_strats_pt1 = 1;
    for (const auto [time, d_to_beat] : records)
    {
        winning_strats_pt1 *= count_winning_strategies(time, d_to_beat);
    }

    // ======== PART 2 ========
    const Record record = unify_records(records);
    size_t winning_strats_pt2 = count_winning_strategies(record.time, record.dist);

    // ======== PRESENT RESULTS ========
    std::cout << "Result for pt1: " << winning_strats_pt1 << std::endl;
    std::cout << "Result for pt2: " << winning_strats_pt2 << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "No path provided, falling back to default: ../input.txt" << std::endl;
        boats("../input.txt");
    }
    else
    {
        boats(argv[1]);
    }
}
