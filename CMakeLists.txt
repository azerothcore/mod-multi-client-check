CU_SET_PATH("CMAKE_MOD_MULTILOGINCHECK_DIR" "${CMAKE_CURRENT_LIST_DIR}")

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_multilogincheck.cpp")

AC_ADD_SCRIPT_LOADER("MultiLoginCheck" "${CMAKE_CURRENT_LIST_DIR}/src/loader_multilogincheck.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod_multilogin.conf.dist")
