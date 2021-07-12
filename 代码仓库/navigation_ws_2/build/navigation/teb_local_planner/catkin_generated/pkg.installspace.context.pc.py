# generated from catkin/cmake/template/pkg.context.pc.in
CATKIN_PACKAGE_PREFIX = ""
PROJECT_PKG_CONFIG_INCLUDE_DIRS = "${prefix}/include;/usr/include/suitesparse;/usr/local/include/g2o".split(';') if "${prefix}/include;/usr/include/suitesparse;/usr/local/include/g2o" != "" else []
PROJECT_CATKIN_DEPENDS = "base_local_planner;costmap_2d;costmap_converter;dynamic_reconfigure;geometry_msgs;interactive_markers;message_runtime;nav_core;nav_msgs;pluginlib;roscpp;std_msgs;tf2;tf2_ros;visualization_msgs".replace(';', ' ')
PKG_CONFIG_LIBRARIES_WITH_PREFIX = "-lteb_local_planner;-lamd;-lbtf;-lcamd;-lccolamd;-lcholmod;-lcolamd;-lcxsparse;-lklu;-lumfpack;-lspqr;/usr/local/lib/libg2o_csparse_extension.so;/usr/local/lib/libg2o_core.so;/usr/local/lib/libg2o_stuff.so;/usr/local/lib/libg2o_types_slam2d.so;/usr/local/lib/libg2o_types_slam3d.so;/usr/local/lib/libg2o_solver_cholmod.so;/usr/local/lib/libg2o_solver_pcg.so;/usr/local/lib/libg2o_solver_csparse.so;/usr/local/lib/libg2o_incremental.so".split(';') if "-lteb_local_planner;-lamd;-lbtf;-lcamd;-lccolamd;-lcholmod;-lcolamd;-lcxsparse;-lklu;-lumfpack;-lspqr;/usr/local/lib/libg2o_csparse_extension.so;/usr/local/lib/libg2o_core.so;/usr/local/lib/libg2o_stuff.so;/usr/local/lib/libg2o_types_slam2d.so;/usr/local/lib/libg2o_types_slam3d.so;/usr/local/lib/libg2o_solver_cholmod.so;/usr/local/lib/libg2o_solver_pcg.so;/usr/local/lib/libg2o_solver_csparse.so;/usr/local/lib/libg2o_incremental.so" != "" else []
PROJECT_NAME = "teb_local_planner"
PROJECT_SPACE_DIR = "/home/wu/navigation_ws_1/install"
PROJECT_VERSION = "0.8.4"
