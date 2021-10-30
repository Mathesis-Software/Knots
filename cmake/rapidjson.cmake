find_package(RapidJSON QUIET)

if (NOT ${RapidJSON_FOUND})
    # https://www.jibbow.com/posts/rapidjson-cmake/
    # Download RapidJSON
    include(ExternalProject)

    ExternalProject_Add(
            RapidJSON
            PREFIX "vendor/rapidjson"
            GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
            GIT_TAG v1.1.0
            CMAKE_ARGS
            -DRAPIDJSON_BUILD_TESTS=OFF
            -DRAPIDJSON_BUILD_DOC=OFF
            -DRAPIDJSON_BUILD_EXAMPLES=OFF
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
            UPDATE_COMMAND ""
    )

    # Prepare RapidJSON (RapidJSON is a header-only library)
    ExternalProject_Get_Property(RapidJSON source_dir)
    set(RAPIDJSON_INCLUDE_DIRS ${source_dir}/include)
endif()


