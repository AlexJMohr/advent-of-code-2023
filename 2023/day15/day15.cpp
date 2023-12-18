#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> parse_file(const char *filename)
{
    std::vector<std::string> instructions;
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");
    std::string line;
    while (std::getline(file, line, ','))
        instructions.push_back(line);
    return instructions;
}

int hash(std::string const &instruction)
{
    int val = 0;
    for (char c : instruction)
    {
        val += static_cast<int>(c);
        val *= 17;
        val %= 256;
    }
    return val;
}

void part1(const char *filename)
{
    int sum = 0;
    auto instructions = parse_file(filename);
    for (auto &instruction : instructions)
        sum += hash(instruction);
    std::cout << "part 1: " << sum << "\n"; // 508498
}

class Box
{
public:
    Box(int id) : m_id(id) {}
    virtual ~Box() = default;

    void add_lens(std::string const &lens_label, int focal_length)
    {
        // if lens label is already in the box, replace it. Otherwise add it.
        if (m_lenses.find(lens_label) != m_lenses.end())
            m_lenses[lens_label] = focal_length;
        else
        {
            m_lenses.insert({lens_label, focal_length});
            m_insertion_order.push_back(lens_label);
        }
    }

    void remove_lens(std::string const &label)
    {
        // remove the lens if it's in the box
        if (m_lenses.find(label) != m_lenses.end())
        {
            m_lenses.erase(label);
            m_insertion_order.erase(std::remove(m_insertion_order.begin(), m_insertion_order.end(), label), m_insertion_order.end());
        }
    }

    bool empty() const { return m_lenses.empty(); }

    int focusing_power() const
    {
        int sum = 0;
        for (size_t i = 0; i < m_insertion_order.size(); ++i)
        {
            int slot_number = i + 1;
            auto label = m_insertion_order[i];
            auto focal_length = m_lenses.at(label);
            int focusing_power = (m_id + 1) * slot_number * focal_length;
            sum += focusing_power;
        }
        return sum;
    }

    void dump() const
    {
        for (auto &label : m_insertion_order)
            std::cout << "[" << label << " " << m_lenses.at(label) << "] ";
    }

private:
    int m_id;
    std::unordered_map<std::string, int> m_lenses;
    std::vector<std::string> m_insertion_order;
};

struct Instruction
{
    std::string label;
    char op;
    std::optional<int> focal_length;
};

Instruction parse_instruction(std::string const &instruction)
{
    size_t op_idx = instruction.find_first_of("=-");
    std::string label = instruction.substr(0, op_idx);
    char op = instruction[op_idx];
    std::optional<int> focal_length;
    if (op == '=')
        focal_length = std::stoi(instruction.substr(op_idx + 1));
    return {label, op, focal_length};
}

void part2(const char *filename)
{
    auto instructions = parse_file(filename);

    std::vector<Box> boxes;
    boxes.reserve(256);
    for (int i = 0; i < 256; ++i)
        boxes.emplace_back(i);

    for (auto &instruction_string : instructions)
    {
        Instruction instruction = parse_instruction(instruction_string);
        int box_idx = hash(instruction.label);

        if (instruction.op == '-')
            boxes[box_idx].remove_lens(instruction.label);
        else if (instruction.op == '=')
            boxes[box_idx].add_lens(instruction.label, instruction.focal_length.value());
    }

    int part2 = std::accumulate(boxes.begin(), boxes.end(), 0, [](int sum, Box const &box)
                                { return sum + box.focusing_power(); });
    std::cout << "part 2: " << part2 << "\n"; // 279116
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    part1(argv[1]);
    part2(argv[1]);

    return 0;
}
