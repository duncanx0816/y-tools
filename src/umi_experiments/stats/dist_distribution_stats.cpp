#include <logger/logger.hpp>
#include "dist_distribution_stats.hpp"
#include "../../fast_ig_tools/banded_half_smith_waterman.hpp"
#include "utils.hpp"

DistDistributionStats DistDistributionStats::GetStats(const std::vector<seqan::Dna5String>& input_reads, const std::unordered_map<Umi, std::vector<size_t> >& umi_to_reads, unsigned thread_count) {
    size_t max_read_length = 0;
    for (auto& read : input_reads) {
        max_read_length = std::max(max_read_length, length(read));
    }

    auto get_hamming_dist = [&](const seqan::Dna5String& s1, const seqan::Dna5String& s2) -> size_t {
        return static_cast<size_t>(-half_sw_banded(s1, s2, 0, -1, -1, [](int) -> int { return 0; }, 0));
    };

    std::vector<std::vector<size_t>> read_groups;
    for (auto& entry : umi_to_reads) {
        read_groups.push_back(entry.second);
    }
    std::map<size_t, std::map<size_t, size_t>> hamming_dist_distribution;
    std::map<size_t, std::map<size_t, size_t>> sw_dist_distribution;
    std::atomic<size_t> processed;
    processed = 0;
    size_t next_percent = 1;
    omp_set_num_threads(thread_count);
#pragma omp parallel for schedule(dynamic)
    for (size_t group = 0; group < read_groups.size(); group ++) {
        auto& reads = read_groups[group];
        std::map<size_t, size_t> current_hamming_distribution;
        std::map<size_t, size_t> current_sw_distribution;
        for (size_t i = 0; i < reads.size(); i++) {
            for (size_t j = 0; j < i; j++) {
                auto& first = input_reads[reads[i]];
                auto& second = input_reads[reads[j]];
                size_t hamming_dist = get_hamming_dist(first, second);
                size_t sw_dist = get_sw_dist(first, second);
                current_hamming_distribution[hamming_dist] ++;
                current_sw_distribution[sw_dist] ++;
            }
        }
#pragma omp critical
        {
            for (auto &entry : current_hamming_distribution) {
                hamming_dist_distribution[reads.size()][entry.first] += entry.second;
                sw_dist_distribution[reads.size()][entry.first] += current_sw_distribution[entry.first];
            }
            processed += reads.size();
            while (processed * 100 >= input_reads.size() * next_percent) {
                if (processed * 100 >= input_reads.size() * next_percent) {
                    INFO(next_percent << "% of " << input_reads.size() << " reads processed");
                    next_percent++;
//                std::cout << DistDistributionStats(hamming_dist_distribution, sw_dist_distribution).ToString();
                }
            }
        }
    }
    return DistDistributionStats(hamming_dist_distribution, sw_dist_distribution);
}

std::vector<size_t> DistDistributionStats::GetSizes() {
    std::vector<size_t> sizes(hamming_dist_distribution_.size());
    size_t current = 0;
    for (auto& entry : hamming_dist_distribution_) {
        sizes[current ++] = entry.first;
    }
    return sizes;
}

std::string DistDistributionStats::ToString(size_t umi_size) {
    size_t max_dist = 0;
    for (size_t i = 0; i < hamming_dist_distribution_[umi_size].size(); i ++) {
        if (hamming_dist_distribution_[umi_size][i] != 0 || sw_dist_distribution_[umi_size][i] != 0) {
            max_dist = i;
        }
    }

    std::stringstream ss;
    char buf[100];
    sprintf(buf, "%5s%10s%10s", "dist", "hamming", "sw");
    ss << buf << std::endl;
    for (size_t i = 0; i <= max_dist; i ++) {
        sprintf(buf, "%5d%10d%10d", static_cast<int>(i), static_cast<int>(hamming_dist_distribution_[umi_size][i]), static_cast<int>(sw_dist_distribution_[umi_size][i]));
        ss << buf << std::endl;
    }
    return ss.str();
}