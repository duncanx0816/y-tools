#include "base_cdr3_hg_cc_processor.hpp"

namespace antevolo {
    class Kruskal_CDR3_HG_CC_Processor : public Base_CDR3_HG_CC_Processor {

        void SetUndirectedComponentParentEdge(size_t root_num, EvolutionaryEdgePtr edge);

        void PrepareSubtree(std::vector<std::pair<size_t, size_t>>& edge_vector,
                                                              size_t root_num);

        void PrepareSubtreeVertices(boost::unordered_set<size_t>& vertices_set,
                                    size_t root_num);

        void PrepareSubtreeKruskal(
                std::vector<std::pair<size_t, size_t>>& edge_vector,
                size_t root_vertex,
                CloneSetWithFakes& clone_set,
                std::shared_ptr<EvolutionaryEdgeConstructor> edge_constructor);
        void ReconstructAncestralLineage(EvolutionaryEdgePtr edge,
                                         EvolutionaryTree& tree,
                                         const std::shared_ptr<EvolutionaryEdgeConstructor> &edge_constructor,
                                         std::vector<size_t>& roots);

        void SetUndirectedComponentsParentEdges(SparseGraphPtr hg_component,
                                                size_t component_id,
                                                boost::disjoint_sets<AP_map, AP_map> ds_on_undirected_edges) override;
        void SetDirections(const boost::unordered_set<size_t>& vertices_nums,
                           EvolutionaryTree& tree,
                           boost::disjoint_sets<AP_map, AP_map> ds_on_undirected_edges) override;

        void ReconstructMissingVertices(const boost::unordered_set<size_t> &vertices_nums,
                                                EvolutionaryTree &tree, SparseGraphPtr hg_component,
                                                size_t component_id) override;

        void HandleRoot(size_t root_num,
                        const std::vector<size_t>& clones_sharing_cdr3,
                        EvolutionaryTree& tree,
                        boost::unordered_map<size_t, EvolutionaryEdgePtr>& roots_nearest_neighbours,
                        const std::shared_ptr<EvolutionaryEdgeConstructor>& edge_constructor);
    public:
        Kruskal_CDR3_HG_CC_Processor(CloneSetWithFakes &clone_set,
                                     const AntEvoloConfig::AlgorithmParams &config,
                                     const AnnotatedCloneByReadConstructor& clone_by_read_constructor,
                                     GraphComponentMap& graph_component,
                                     const UniqueCDR3IndexMap& unique_cdr3s_map,
                                     const CDR3ToIndexMap& cdr3_to_index_map,
                                     const std::vector<std::string>& unique_cdr3s)
                : Base_CDR3_HG_CC_Processor(clone_set,
                                            config,
                                            clone_by_read_constructor,
                                            graph_component,
                                            unique_cdr3s_map,
                                            cdr3_to_index_map,
                                            unique_cdr3s) {}


    };
}