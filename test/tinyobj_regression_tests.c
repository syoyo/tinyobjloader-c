#define TEST_NO_MAIN

#include "tinyobj_regression_tests.h"
#include "tinyobj_loader_c.h"
#include "acutest.h"

#include <math.h>
#include <float.h>

static int float_equals(float x, float y)
{
  if (fabs(x - y) < FLT_EPSILON) {
    return 1;
  }

  return 0;
}

size_t loadFile_d(const char *filename, char **buffer) {
  long string_size = 0, read_size = 0;
  FILE *handler = fopen(filename, "r");
  *buffer = NULL;

  if (handler) {
    if (fseek(handler, 0, SEEK_END) != 0) {
      fprintf(stderr, "loadFile: fseek %s failure (errno %d)\n", filename,
              errno);
      return 0;
    }
    string_size = ftell(handler);
    rewind(handler);
    *buffer = (char *)malloc(sizeof(char) * (string_size + 1));
    read_size = (long)fread(*buffer, sizeof(char), string_size, handler);
    (*buffer)[string_size] = '\0';
    if (string_size != read_size && errno) {
      fprintf(stderr, "loadFile: fread %s failure (errno %d)\n", filename,
              errno);
      free(*buffer);
      *buffer = NULL;
    }
    fclose(handler);
  }
  return (size_t)read_size;
}

void test_tinyobj_crlf_string(void)
{
    {
        const char * filename = "fixtures/texname-crlf.mtl";

        tinyobj_material_t * material;
        unsigned int num_materials;
        int ret;

		ret = tinyobj_parse_mtl_file(&material, &num_materials, filename);
        TEST_CHECK(ret == TINYOBJ_SUCCESS);
		if(ret != TINYOBJ_SUCCESS)
			return;

        TEST_CHECK(num_materials == 1);
        TEST_CHECK(strcmp(material->name, "CubeMaterial") == 0);

        TEST_CHECK(strcmp(material->diffuse_texname, "input.jpg") == 0);
    }
}

void test_tinyobj_negative_exponent(void)
{
    {
        const char * filename = "fixtures/negative-exponent.obj";

        tinyobj_shape_t * shape = NULL;
        tinyobj_material_t * material = NULL;
        tinyobj_attrib_t attrib;

        unsigned int num_shapes;
        unsigned int num_materials;

        char * obj_contents;
        size_t file_size = loadFile_d(filename, &obj_contents);

        int result = tinyobj_parse_obj(&attrib, &shape, &num_shapes, &material, &num_materials, obj_contents, file_size, TINYOBJ_FLAG_TRIANGULATE);

        TEST_CHECK(result == TINYOBJ_SUCCESS);
		if(result != TINYOBJ_SUCCESS)
			return;

        TEST_CHECK(float_equals(attrib.v[0].x, 2.0e+5f));
        TEST_CHECK(float_equals(attrib.v[0].y, 2.0e-5f));
        TEST_CHECK(float_equals(attrib.v[0].z, 2.0e-0f));
    }
}
