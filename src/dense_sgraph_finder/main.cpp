#include "standard.hpp"
#include "logger/log_writers.hpp"

#include "segfault_handler.hpp"
#include "stacktrace.hpp"
#include "memory_limit.hpp"
#include "copy_file.hpp"
#include "perfcounter.hpp"
#include "runtime_k.hpp"
#include "io/ireadstream.hpp"
#include "io/library.hpp"
#include "io/single_read.hpp"
#include "io/io_helper.hpp"

//#include "config_struct.hpp"

#include "stage.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "segfault_handler.hpp"

#include "include_me.hpp"

#include "dsf_config.hpp"

#include "graph_utils/crs_matrix.hpp"
#include "graph_utils/graph_io.hpp"

#include "metis_permutation_constructor/metis_permutation_constructor.hpp"

void make_dirs(){
    make_dir(dsf_cfg::get().io.output_dir);
//    make_dir(dsf_cfg::get().io.temp_files);
//    if (dsf_cfg::get().rp.developer_mode)
//        make_dir(dsf_cfg::get().io.output_saves);
//    make_dir(dsf_cfg::get().hgc_params.hgc_io_params.hg_output_dir);
//    if(dsf_cfg::get().hgc_params.hgc_io_params.output_dense_subgraphs)
//        make_dir(dsf_cfg::get().hgc_params.hgc_io_params.dense_subgraphs_dir);
}

void copy_configs(string cfg_filename, string to) {
    //using namespace debruijn_graph;

    if (!make_dir(to)) {
        WARN("Could not create files use in /tmp directory");
    }
    path::copy_files_by_ext(path::parent_path(cfg_filename), to, ".info", true);
}

void load_config(string cfg_filename) {
    path::CheckFileExistenceFATAL(cfg_filename);
    dsf_cfg::create_instance(cfg_filename);
    string path_to_copy = path::append_path(dsf_cfg::get().io.output_dir, "configs");
    copy_configs(cfg_filename, path_to_copy);
}

void create_console_logger(string cfg_filename) {
    using namespace logging;

    string log_props_file = dsf_cfg::get().io.log_filename;

    if (!path::FileExists(log_props_file)){
        log_props_file = path::append_path(path::parent_path(cfg_filename), dsf_cfg::get().io.log_filename);
    }

    logger *lg = create_logger(path::FileExists(log_props_file) ? log_props_file : "");
    lg->add_writer(std::make_shared<console_writer>());
    attach_logger(lg);
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "dense_sgraph_finder config.info" << std::endl;
        return 1;
    }

    load_config(argv[1]);

    std::string graph_fname = dsf_cfg::get().io.graph_filename;
    GraphReader graph_reader(graph_fname);
    CRS_HammingGraph_Ptr graph_ptr = graph_reader.CreateGraph();
    HG_CollapsedStructs_Ptr collapsed_struct_ptr = HG_CollapsedStructs_Ptr(new HG_CollapsedStructs(graph_ptr));

    dense_subgraph_finder::MetisPermutationConstructor permutation_constructor(graph_ptr,
                                                                               collapsed_struct_ptr,
                                                                               0,
                                                                               dsf_cfg::get().hgc_params.hgc_io_params);

}