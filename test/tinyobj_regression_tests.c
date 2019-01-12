#define TEST_NO_MAIN

#include "tinyobj_regression_tests.h"
#include "tinyobj_loader_c.h"
#include "acutest.h"


void test_tinyobj_crlf_string(void)
{
    {
        const char * filename = "fixtures/texname-crlf.mtl";

        tinyobj_material_t * material;
        size_t num_materials;

        TEST_CHECK(tinyobj_parse_mtl_file(&material, &num_materials, filename) == TINYOBJ_SUCCESS);

        TEST_CHECK(num_materials == 1);
        TEST_CHECK(strcmp(material->name, "CubeMaterial") == 0);

        TEST_CHECK(strcmp(material->diffuse_texname, "input.jpg") == 0);
    }
}

