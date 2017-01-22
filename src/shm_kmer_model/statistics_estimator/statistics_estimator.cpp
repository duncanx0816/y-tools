//
// Created by Andrew Bzikadze on 5/24/16.
//

#include "mutation_strategies/trivial_strategy.hpp"
#include "mutation_strategies/no_k_neighbours.hpp"
#include "statistics_estimator.hpp"

using namespace ns_gene_alignment;

StatisticsEstimator::StatisticsEstimator(const shm_config::mutations_strategy_params &config) :
    kmer_len_(config.kmer_len) {
    using MutationStrategyMethod = shm_config::mutations_strategy_params::MutationsStrategyMethod;
    if (config.mutations_strategy_method == MutationStrategyMethod::Trivial)
        mutation_strategy_ = std::make_shared<TrivialMutationStrategy>(TrivialMutationStrategy(config));
    else if (config.mutations_strategy_method == MutationStrategyMethod::NoKNeighbours)
        mutation_strategy_ = std::make_shared<NoKNeighboursMutationStrategy>(NoKNeighboursMutationStrategy(config));
}

void StatisticsEstimator::calculate_mutation_statistics_per_position(MutationsStatistics &mutations_statistics,
                                                                     const size_t center_nucl_pos,
                                                                     const EvolutionaryEdgeAlignment& alignment) const
{
    std::string gene_substring = alignment.parent().substr(center_nucl_pos - kmer_len_ / 2, kmer_len_);
    if ((alignment.son()[center_nucl_pos] == 'N') ||
        (gene_substring.find_first_of('N') != std::string::npos)) {
        return;
    }
    size_t position = seqan::ordValue(static_cast<seqan::Dna>(alignment.son()[center_nucl_pos]));
    mutations_statistics.at(gene_substring).at(position)++;
}

std::pair<MutationsStatistics, MutationsStatistics>
StatisticsEstimator::calculate_mutation_statistics(VectorEvolutionaryEdgeAlignments &alignments) const {
    MutationsStatistics mutations_statistics_fr(kmer_len_);
    MutationsStatistics mutations_statistics_cdr(kmer_len_);
    for (auto &alignment : alignments) {
        std::vector<size_t> relevant_positions = mutation_strategy_->calculate_relevant_positions(alignment);
        size_t i = 0;
        while (i < relevant_positions.size() and relevant_positions[i] < alignment.cdr1_start()) {
            calculate_mutation_statistics_per_position(mutations_statistics_fr, relevant_positions[i], alignment);
            i++;
        }
        while (i < relevant_positions.size() and relevant_positions[i] <= alignment.cdr1_end()) {
            calculate_mutation_statistics_per_position(mutations_statistics_cdr, relevant_positions[i], alignment);
            i++;
        }
        while (i < relevant_positions.size() and relevant_positions[i] < alignment.cdr2_start()) {
            calculate_mutation_statistics_per_position(mutations_statistics_fr, relevant_positions[i], alignment);
            i++;
        }
        while (i < relevant_positions.size() and relevant_positions[i] < alignment.cdr2_end()) {
            calculate_mutation_statistics_per_position(mutations_statistics_cdr, relevant_positions[i], alignment);
            i++;
        }
        while (i < relevant_positions.size()) {
            calculate_mutation_statistics_per_position(mutations_statistics_fr, relevant_positions[i], alignment);
            i++;
        }
    }
    return std::make_pair(mutations_statistics_fr, mutations_statistics_cdr);
}