project(Cython)

function( add_GUDHI_PYTHON_lib THE_LIB )
  if(EXISTS ${THE_LIB})
    get_filename_component(THE_LIB_FILE_NAME ${THE_LIB} NAME_WE)
    if(WIN32)
      message("++ ${THE_LIB} => THE_LIB_FILE_NAME = ${THE_LIB_FILE_NAME}")
      set(GUDHI_PYTHON_LIBRARIES "${GUDHI_PYTHON_LIBRARIES}'${THE_LIB_FILE_NAME}', " PARENT_SCOPE)
    else(WIN32)
      STRING(REGEX REPLACE "lib" "" UNIX_LIB_FILE_NAME ${THE_LIB_FILE_NAME})
      message("++ ${THE_LIB} => UNIX_LIB_FILE_NAME = ${UNIX_LIB_FILE_NAME}")
      set(GUDHI_PYTHON_LIBRARIES "${GUDHI_PYTHON_LIBRARIES}'${UNIX_LIB_FILE_NAME}', " PARENT_SCOPE)
    endif(WIN32)
  endif(EXISTS ${THE_LIB})
endfunction( add_GUDHI_PYTHON_lib )

# THE_TEST is the python test file name (without .py extension) containing tests functions
function( add_gudhi_py_test THE_TEST )
  if(PYTEST_FOUND)
    # use ${PYTHON_EXECUTABLE} -B, otherwise a __pycache__ directory is created in sources by python
    # use py.test no cache provider, otherwise a .cache file is created in sources by py.test
    add_test(NAME ${THE_TEST}_py_test
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND ${PYTHON_EXECUTABLE} -B -m pytest -p no:cacheprovider ${CMAKE_CURRENT_SOURCE_DIR}/test/${THE_TEST}.py)
  endif()
endfunction( add_gudhi_py_test )

# Set gudhi.__debug_info__
# WARNING : to be done before setup.py.in configure_file
function( add_gudhi_debug_info DEBUG_INFO )
  set(GUDHI_PYTHON_DEBUG_INFO "${GUDHI_PYTHON_DEBUG_INFO}    \"${DEBUG_INFO}\\n\" \\\n" PARENT_SCOPE)
endfunction( add_gudhi_debug_info )

if(PYTHONINTERP_FOUND)
  if(PYBIND11_FOUND)
    add_gudhi_debug_info("Pybind11 version ${PYBIND11_VERSION}")
    set(GUDHI_PYTHON_MODULES_EXTRA "${GUDHI_PYTHON_MODULES_EXTRA}'hera', ")
  endif()
  if(CYTHON_FOUND)
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'off_reader', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'simplex_tree', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'rips_complex', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'cubical_complex', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'periodic_cubical_complex', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'persistence_graphical_tools', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'reader_utils', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'witness_complex', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'strong_witness_complex', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'bottleneck', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'nerve_gic', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'subsampling', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'tangential_complex', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'alpha_complex', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'euclidean_witness_complex', ")
    set(GUDHI_PYTHON_MODULES "${GUDHI_PYTHON_MODULES}'euclidean_strong_witness_complex', ")
    # Modules that should not be auto-imported in __init__.py
    set(GUDHI_PYTHON_MODULES_EXTRA "${GUDHI_PYTHON_MODULES_EXTRA}'representations', ")
    set(GUDHI_PYTHON_MODULES_EXTRA "${GUDHI_PYTHON_MODULES_EXTRA}'wasserstein', ")

    add_gudhi_debug_info("Python version ${PYTHON_VERSION_STRING}")
    add_gudhi_debug_info("Cython version ${CYTHON_VERSION}")
    if(PYTEST_FOUND)
      add_gudhi_debug_info("Pytest version ${PYTEST_VERSION}")
    endif()
    if(MATPLOTLIB_FOUND)
      add_gudhi_debug_info("Matplotlib version ${MATPLOTLIB_VERSION}")
    endif()
    if(NUMPY_FOUND)
      add_gudhi_debug_info("Numpy version ${NUMPY_VERSION}")
    endif()
    if(SCIPY_FOUND)
      add_gudhi_debug_info("Scipy version ${SCIPY_VERSION}")
    endif()
    if(SKLEARN_FOUND)
      add_gudhi_debug_info("Scikit-learn version ${SKLEARN_VERSION}")
    endif()
    if(OT_FOUND)
      add_gudhi_debug_info("POT version ${OT_VERSION}")
    endif()

    set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DBOOST_RESULT_OF_USE_DECLTYPE', ")
    set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DBOOST_ALL_NO_LIB', ")
    set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DBOOST_SYSTEM_NO_DEPRECATED', ")

    # Gudhi and CGAL compilation option
    if(MSVC)
      set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'/fp:strict', ")
    else(MSVC)
      set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-std=c++14', ")
    endif(MSVC)
    if(CMAKE_COMPILER_IS_GNUCXX)
      set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-frounding-math', ")
      set(GUDHI_PYBIND11_EXTRA_COMPILE_ARGS "${GUDHI_PYBIND11_EXTRA_COMPILE_ARGS}'-fvisibility=hidden', ")
    endif(CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_CXX_COMPILER_ID MATCHES Intel)
      set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-fp-model strict', ")
    endif(CMAKE_CXX_COMPILER_ID MATCHES Intel)
    if (DEBUG_TRACES)
      # For programs to be more verbose
      set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DDEBUG_TRACES', ")
    endif()

    if (EIGEN3_FOUND)
      add_gudhi_debug_info("Eigen3 version ${EIGEN3_VERSION}")
      # No problem, even if no CGAL found
      set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DCGAL_EIGEN3_ENABLED', ")
    endif (EIGEN3_FOUND)

    set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'off_reader', ")
    set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'simplex_tree', ")
    set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'rips_complex', ")
    set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'cubical_complex', ")
    set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'periodic_cubical_complex', ")
    set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'reader_utils', ")
    set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'witness_complex', ")
    set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'strong_witness_complex', ")
    if (NOT CGAL_VERSION VERSION_LESS 4.11.0)
      set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'bottleneck', ")
      set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'nerve_gic', ")
    endif ()
    if (NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
      set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'alpha_complex', ")
      set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'subsampling', ")
      set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'tangential_complex', ")
      set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'euclidean_witness_complex', ")
      set(GUDHI_PYTHON_MODULES_TO_COMPILE "${GUDHI_PYTHON_MODULES_TO_COMPILE}'euclidean_strong_witness_complex', ")
    endif ()

    if(CGAL_FOUND)
      can_cgal_use_cxx11_thread_local()
      if (NOT CGAL_CAN_USE_CXX11_THREAD_LOCAL_RESULT)
        if(CMAKE_BUILD_TYPE MATCHES Debug)
          add_GUDHI_PYTHON_lib("${Boost_THREAD_LIBRARY_DEBUG}")
        else()
          add_GUDHI_PYTHON_lib("${Boost_THREAD_LIBRARY_RELEASE}")
        endif()
        message("** Add Boost ${Boost_LIBRARY_DIRS}")
        set(GUDHI_PYTHON_LIBRARY_DIRS "${GUDHI_PYTHON_LIBRARY_DIRS}'${Boost_LIBRARY_DIRS}', ")
      endif()
      # Add CGAL compilation args
      if(CGAL_HEADER_ONLY)
        add_gudhi_debug_info("CGAL header only version ${CGAL_VERSION}")
        set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DCGAL_HEADER_ONLY', ")
      else(CGAL_HEADER_ONLY)
        add_gudhi_debug_info("CGAL version ${CGAL_VERSION}")
        add_GUDHI_PYTHON_lib("${CGAL_LIBRARY}")
        set(GUDHI_PYTHON_LIBRARY_DIRS "${GUDHI_PYTHON_LIBRARY_DIRS}'${CGAL_LIBRARIES_DIR}', ")
        message("** Add CGAL ${CGAL_LIBRARIES_DIR}")
        # If CGAL is not header only, CGAL library may link with boost system,
        if(CMAKE_BUILD_TYPE MATCHES Debug)
          add_GUDHI_PYTHON_lib("${Boost_SYSTEM_LIBRARY_DEBUG}")
        else()
          add_GUDHI_PYTHON_lib("${Boost_SYSTEM_LIBRARY_RELEASE}")
        endif()
        set(GUDHI_PYTHON_LIBRARY_DIRS "${GUDHI_PYTHON_LIBRARY_DIRS}'${Boost_LIBRARY_DIRS}', ")
        message("** Add Boost ${Boost_LIBRARY_DIRS}")
      endif(CGAL_HEADER_ONLY)
      # GMP and GMPXX are not required, but if present, CGAL will link with them.
      if(GMP_FOUND)
        add_gudhi_debug_info("GMP_LIBRARIES = ${GMP_LIBRARIES}")
        set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DCGAL_USE_GMP', ")
        add_GUDHI_PYTHON_lib("${GMP_LIBRARIES}")
        set(GUDHI_PYTHON_LIBRARY_DIRS "${GUDHI_PYTHON_LIBRARY_DIRS}'${GMP_LIBRARIES_DIR}', ")
        message("** Add gmp ${GMP_LIBRARIES_DIR}")
        if(GMPXX_FOUND)
          add_gudhi_debug_info("GMPXX_LIBRARIES = ${GMPXX_LIBRARIES}")
          set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DCGAL_USE_GMPXX', ")
          add_GUDHI_PYTHON_lib("${GMPXX_LIBRARIES}")
          set(GUDHI_PYTHON_LIBRARY_DIRS "${GUDHI_PYTHON_LIBRARY_DIRS}'${GMPXX_LIBRARIES_DIR}', ")
          message("** Add gmpxx ${GMPXX_LIBRARIES_DIR}")
        endif(GMPXX_FOUND)
      endif(GMP_FOUND)
      if(MPFR_FOUND)
        add_gudhi_debug_info("MPFR_LIBRARIES = ${MPFR_LIBRARIES}")
        set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DCGAL_USE_MPFR', ")
        add_GUDHI_PYTHON_lib("${MPFR_LIBRARIES}")
        # In case CGAL is not header only, all MPFR variables are set except MPFR_LIBRARIES_DIR - Just set it
        if(NOT MPFR_LIBRARIES_DIR)
          get_filename_component(MPFR_LIBRARIES_DIR ${MPFR_LIBRARIES} PATH)
        endif(NOT MPFR_LIBRARIES_DIR)
        set(GUDHI_PYTHON_LIBRARY_DIRS "${GUDHI_PYTHON_LIBRARY_DIRS}'${MPFR_LIBRARIES_DIR}', ")
        message("** Add mpfr ${MPFR_LIBRARIES}")
      endif(MPFR_FOUND)
    endif(CGAL_FOUND)

    # Specific for Mac
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-mmacosx-version-min=10.12', ")
        set(GUDHI_PYTHON_EXTRA_LINK_ARGS "${GUDHI_PYTHON_EXTRA_LINK_ARGS}'-mmacosx-version-min=10.12', ")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    # Loop on INCLUDE_DIRECTORIES PROPERTY
    get_property(GUDHI_INCLUDE_DIRECTORIES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach(GUDHI_INCLUDE_DIRECTORY ${GUDHI_INCLUDE_DIRECTORIES})
      set(GUDHI_PYTHON_INCLUDE_DIRS "${GUDHI_PYTHON_INCLUDE_DIRS}'${GUDHI_INCLUDE_DIRECTORY}', ")
    endforeach()
    set(GUDHI_PYTHON_INCLUDE_DIRS "${GUDHI_PYTHON_INCLUDE_DIRS}'${CMAKE_SOURCE_DIR}/${GUDHI_PYTHON_PATH}/include', ")

    if (TBB_FOUND AND WITH_GUDHI_USE_TBB)
      add_gudhi_debug_info("TBB version ${TBB_INTERFACE_VERSION} found and used")
      set(GUDHI_PYTHON_EXTRA_COMPILE_ARGS "${GUDHI_PYTHON_EXTRA_COMPILE_ARGS}'-DGUDHI_USE_TBB', ")
      if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_GUDHI_PYTHON_lib("${TBB_DEBUG_LIBRARY}")
        add_GUDHI_PYTHON_lib("${TBB_MALLOC_DEBUG_LIBRARY}")
      else()
        add_GUDHI_PYTHON_lib("${TBB_RELEASE_LIBRARY}")
        add_GUDHI_PYTHON_lib("${TBB_MALLOC_RELEASE_LIBRARY}")
      endif()
      set(GUDHI_PYTHON_LIBRARY_DIRS "${GUDHI_PYTHON_LIBRARY_DIRS}'${TBB_LIBRARY_DIRS}', ")
      message("** Add tbb ${TBB_LIBRARY_DIRS}")
      set(GUDHI_PYTHON_INCLUDE_DIRS "${GUDHI_PYTHON_INCLUDE_DIRS}'${TBB_INCLUDE_DIRS}', ")
    endif()

    if(UNIX AND WITH_GUDHI_PYTHON_RUNTIME_LIBRARY_DIRS)
      set( GUDHI_PYTHON_RUNTIME_LIBRARY_DIRS "${GUDHI_PYTHON_LIBRARY_DIRS}")
    endif(UNIX AND WITH_GUDHI_PYTHON_RUNTIME_LIBRARY_DIRS)

    # Generate setup.py file to cythonize Gudhi - This file must be named setup.py by convention
    configure_file(setup.py.in "${CMAKE_CURRENT_BINARY_DIR}/setup.py" @ONLY)

    # Generate gudhi/__init__.py
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/gudhi")
    configure_file("gudhi/__init__.py.in" "${CMAKE_CURRENT_BINARY_DIR}/gudhi/__init__.py" @ONLY)

    # Other .py files
    file(COPY "gudhi/persistence_graphical_tools.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/gudhi")
    file(COPY "gudhi/representations" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/gudhi/")
    file(COPY "gudhi/wasserstein.py" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/gudhi")

    add_custom_command(
        OUTPUT gudhi.so
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/setup.py" "build_ext" "--inplace")

    add_custom_target(python ALL DEPENDS gudhi.so
                      COMMENT "Do not forget to add ${CMAKE_CURRENT_BINARY_DIR}/ to your PYTHONPATH before using examples or tests")

    install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/setup.py install)")

    # Test examples
    if (NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
      # Bottleneck and Alpha
      add_test(NAME alpha_rips_persistence_bottleneck_distance_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/alpha_rips_persistence_bottleneck_distance.py"
               -f ${CMAKE_SOURCE_DIR}/data/points/tore3D_300.off -t 0.15 -d 3)
      if(MATPLOTLIB_FOUND AND NUMPY_FOUND)
        # Tangential
        add_test(NAME tangential_complex_plain_homology_from_off_file_example_py_test
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                 COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
                 ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/tangential_complex_plain_homology_from_off_file_example.py"
                 --no-diagram -i 2 -f ${CMAKE_SOURCE_DIR}/data/points/tore3D_300.off)

        add_gudhi_py_test(test_tangential_complex)

        # Witness complex AND Subsampling
        add_test(NAME euclidean_strong_witness_complex_diagram_persistence_from_off_file_example_py_test
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                 COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
                 ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/euclidean_strong_witness_complex_diagram_persistence_from_off_file_example.py"
                 --no-diagram -f ${CMAKE_SOURCE_DIR}/data/points/tore3D_300.off -a 1.0 -n 20 -d 2)

        add_test(NAME euclidean_witness_complex_diagram_persistence_from_off_file_example_py_test
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                 COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
                 ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/euclidean_witness_complex_diagram_persistence_from_off_file_example.py"
                 --no-diagram -f ${CMAKE_SOURCE_DIR}/data/points/tore3D_300.off -a 1.0 -n 20 -d 2)
      endif()

      # Subsampling
      add_gudhi_py_test(test_subsampling)

    endif (NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
    if (NOT CGAL_VERSION VERSION_LESS 4.11.0)
      # Bottleneck
      add_test(NAME bottleneck_basic_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/bottleneck_basic_example.py")

      add_gudhi_py_test(test_bottleneck_distance)

      # Cover complex
      file(COPY ${CMAKE_SOURCE_DIR}/data/points/human.off DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
      file(COPY ${CMAKE_SOURCE_DIR}/data/points/COIL_database/lucky_cat.off DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
      file(COPY ${CMAKE_SOURCE_DIR}/data/points/COIL_database/lucky_cat_PCA1 DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
      add_test(NAME cover_complex_nerve_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/nerve_of_a_covering.py"
               -f human.off -c 2 -r 10 -g 0.3)

      add_test(NAME cover_complex_coordinate_gic_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/coordinate_graph_induced_complex.py"
               -f human.off -c 0 -v)

      add_test(NAME cover_complex_functional_gic_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/functional_graph_induced_complex.py"
               -o lucky_cat.off
               -f lucky_cat_PCA1 -v)

      add_test(NAME cover_complex_voronoi_gic_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/voronoi_graph_induced_complex.py"
               -f human.off -n 700 -v)

      add_gudhi_py_test(test_cover_complex)
    endif (NOT CGAL_VERSION VERSION_LESS 4.11.0)

    if (NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
      # Alpha
      add_test(NAME alpha_complex_from_points_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/alpha_complex_from_points_example.py")
      if(MATPLOTLIB_FOUND AND NUMPY_FOUND)
        add_test(NAME alpha_complex_diagram_persistence_from_off_file_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/alpha_complex_diagram_persistence_from_off_file_example.py"
               --no-diagram -f ${CMAKE_SOURCE_DIR}/data/points/tore3D_300.off -a 0.6)
      endif()
      add_gudhi_py_test(test_alpha_complex)
    endif (NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)

    if (NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
      # Euclidean witness
      add_gudhi_py_test(test_euclidean_witness_complex)

    endif (NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)

    # Cubical
    add_test(NAME periodic_cubical_complex_barcode_persistence_from_perseus_file_example_py_test
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
             ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/periodic_cubical_complex_barcode_persistence_from_perseus_file_example.py"
             --no-barcode -f ${CMAKE_SOURCE_DIR}/data/bitmap/CubicalTwoSphere.txt)

    if(NUMPY_FOUND)
      add_test(NAME random_cubical_complex_persistence_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/random_cubical_complex_persistence_example.py"
               10 10 10)
    endif()

    add_gudhi_py_test(test_cubical_complex)

    # Rips
    if(MATPLOTLIB_FOUND AND NUMPY_FOUND)
      add_test(NAME rips_complex_diagram_persistence_from_distance_matrix_file_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/example/rips_complex_diagram_persistence_from_distance_matrix_file_example.py"
               --no-diagram -f ${CMAKE_SOURCE_DIR}/data/distance_matrix/lower_triangular_distance_matrix.csv -e 12.0 -d 3)

      add_test(NAME rips_complex_diagram_persistence_from_off_file_example_py_test
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
               ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/example/rips_complex_diagram_persistence_from_off_file_example.py
               --no-diagram -f ${CMAKE_SOURCE_DIR}/data/points/tore3D_300.off  -e 0.25 -d 3)
    endif()

    add_test(NAME rips_complex_from_points_example_py_test
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
             ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/example/rips_complex_from_points_example.py)

    add_gudhi_py_test(test_rips_complex)

    # Simplex tree
    add_test(NAME simplex_tree_example_py_test
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
             ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/example/simplex_tree_example.py)

    add_gudhi_py_test(test_simplex_tree)

    # Witness
    add_test(NAME witness_complex_from_nearest_landmark_table_py_test
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
             ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/example/witness_complex_from_nearest_landmark_table.py)

    add_gudhi_py_test(test_witness_complex)

    # Reader utils
    add_gudhi_py_test(test_reader_utils)

    # Wasserstein
    if(OT_FOUND AND PYBIND11_FOUND)
      add_gudhi_py_test(test_wasserstein_distance)
    endif()

    # Representations
    if(SKLEARN_FOUND AND MATPLOTLIB_FOUND)
      add_gudhi_py_test(test_representations)
    endif()

    # Documentation generation is available through sphinx - requires all modules
    if(SPHINX_PATH)
      if(MATPLOTLIB_FOUND)
        if(NUMPY_FOUND)
          if(SCIPY_FOUND)
            if(SKLEARN_FOUND)
              if(OT_FOUND)
                if(PYBIND11_FOUND)
                  if(NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
                    set (GUDHI_SPHINX_MESSAGE "Generating API documentation with Sphinx in ${CMAKE_CURRENT_BINARY_DIR}/sphinx/")
                    # User warning - Sphinx is a static pages generator, and configured to work fine with user_version
                    # Images and biblio warnings because not found on developper version
                    if (GUDHI_PYTHON_PATH STREQUAL "src/python")
                      set (GUDHI_SPHINX_MESSAGE "${GUDHI_SPHINX_MESSAGE} \n WARNING : Sphinx is configured for user version, you run it on developper version. Images and biblio will miss")
                    endif()
                    # sphinx target requires gudhi.so, because conf.py reads gudhi version from it
                    add_custom_target(sphinx
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
                        COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
                        ${SPHINX_PATH} -b html ${CMAKE_CURRENT_SOURCE_DIR}/doc ${CMAKE_CURRENT_BINARY_DIR}/sphinx
                        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/gudhi.so"
                        COMMENT "${GUDHI_SPHINX_MESSAGE}" VERBATIM)
  
                    add_test(NAME sphinx_py_test
                             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                             COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}"
                             ${SPHINX_PATH} -b doctest ${CMAKE_CURRENT_SOURCE_DIR}/doc ${CMAKE_CURRENT_BINARY_DIR}/doctest)
  
                    # Set missing or not modules
                    set(GUDHI_MODULES ${GUDHI_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MODULES")
                  else(NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
                    message("++ Python documentation module will not be compiled because it requires a Eigen3 and CGAL version >= 4.11.0")
                    set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MISSING_MODULES")
                  endif(NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
                else(PYBIND11_FOUND)
                  message("++ Python documentation module will not be compiled because pybind11 was not found")
                  set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MISSING_MODULES")
                endif(PYBIND11_FOUND)
              else(OT_FOUND)
                message("++ Python documentation module will not be compiled because POT was not found")
                set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MISSING_MODULES")
              endif(OT_FOUND)
            else(SKLEARN_FOUND)
              message("++ Python documentation module will not be compiled because scikit-learn was not found")
              set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MISSING_MODULES")
            endif(SKLEARN_FOUND)
          else(SCIPY_FOUND)
            message("++ Python documentation module will not be compiled because scipy was not found")
            set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MISSING_MODULES")
          endif(SCIPY_FOUND)
        else(NUMPY_FOUND)
          message("++ Python documentation module will not be compiled because numpy was not found")
          set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MISSING_MODULES")
        endif(NUMPY_FOUND)
      else(MATPLOTLIB_FOUND)
        message("++ Python documentation module will not be compiled because matplotlib was not found")
        set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MISSING_MODULES")
      endif(MATPLOTLIB_FOUND)
    else(SPHINX_PATH)
      message("++ Python documentation module will not be compiled because sphinx and sphinxcontrib-bibtex were not found")
      set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python-documentation" CACHE INTERNAL "GUDHI_MISSING_MODULES")
    endif(SPHINX_PATH)


    # Set missing or not modules
    set(GUDHI_MODULES ${GUDHI_MODULES} "python" CACHE INTERNAL "GUDHI_MODULES")
  else(CYTHON_FOUND)
    message("++ Python module will not be compiled because cython was not found")
    set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python" CACHE INTERNAL "GUDHI_MISSING_MODULES")
  endif(CYTHON_FOUND)
else(PYTHONINTERP_FOUND)
  message("++ Python module will not be compiled because no Python interpreter was found")
  set(GUDHI_MISSING_MODULES ${GUDHI_MISSING_MODULES} "python" CACHE INTERNAL "GUDHI_MISSING_MODULES")
endif(PYTHONINTERP_FOUND)
